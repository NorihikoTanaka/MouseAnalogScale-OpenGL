#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const int PointsNum = 2000;
double CPArray[PointsNum][2];   // 円の点群配列
GLint LSArray[2][2];   // 線分の始点終点座標配列
int rubberband = 0;        /* ラバーバンドの消去 */
const double PI = 3.141593;
const double WinSizeX = 600, WinSizeY = 600;

float ExAns[100][2];
int exnum = 0;

void CirclePointsInit(void) {
	//double CircleRad = WinSizeX * 0.3;
	double CircleRad = WinSizeX / 3.0f;

	for (int i = 0; i < PointsNum; i++) {
		double Degree = ((double)360.0f / (double)PointsNum) * (double)i;
		CPArray[i][0] = CircleRad * cos(Degree * PI / (double)180.0f);
		CPArray[i][1] = CircleRad * sin(Degree * PI / (double)180.0f);
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	// 軸線の描画
	glColor3d(1.0, 0.0, 0.0);

	glLineWidth(5.0);
	glBegin(GL_LINES);
		glVertex2d(WinSizeX / 2, 0);
		glVertex2d(WinSizeX / 2, WinSizeY);
		glVertex2d(0, WinSizeY / 2);
		glVertex2d(WinSizeX, WinSizeY / 2);
	glEnd();
	
	// 円の描画
	CirclePointsInit();
	glLineWidth(5.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < PointsNum; i++) {
		//printf("C: x:%f, y:%f\n", PointsArray[i][0], PointsArray[i][1]);
		glVertex2d(WinSizeX / 2 + CPArray[i][0], WinSizeY / 2 + CPArray[i][1]);
	}
	glEnd();

	// 線分の描画
	glColor3d(0.0, 0.0, 0.0);
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glVertex2iv(LSArray[0]);
	glVertex2iv(LSArray[1]);
	glEnd();


	glFlush();
}

void resize(int w, int h){
	/* ウィンドウ全体をビューポートにする */
	glViewport(0, 0, w, h);
	/* 変換行列の初期化 */
	glLoadIdentity();
	/* スクリーン上の座標系をマウスの座標系に一致させる */
	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
}


void init(void) { glClearColor(0.859, 0.859, 0.859, 1.0); }

float distance(GLint Array[][2]) {
	float dis = sqrt( pow( (float)Array[1][0] - (float)Array[0][0], 2 ) +
		pow((float)Array[1][1] - (float)Array[0][1], 2));
	//int a = Array[1][0] - Array[0][0];
	//printf("%f\n", dis);
	return dis;
}

float angle(GLint Array[][2]) {
	float dis = distance(Array);
	float theta = acos((Array[1][0] - Array[0][0]) / dis) * 180.0f / PI;
	return theta;
}


//https://tokoik.github.io/opengl/libglut.html
void mouse(int button, int state, int x, int y) {   // x,yはウインドウサイズ内のピクセル値
	LSArray[0][0] = WinSizeX / 2;   LSArray[0][1] = WinSizeY / 2;

	glColor3d(0.0, 1.0, 0.0);

	switch (button) {
	case GLUT_LEFT_BUTTON:

		if (state == GLUT_UP) {
			// 離した座標の保存
			LSArray[1][0] = x;   LSArray[1][1] = y;
			
			glColor3d(1.0, 0.0, 0.0);
			glBegin(GL_LINES);
				glVertex2d(LSArray[0][0], LSArray[0][1]);
				glVertex2d(LSArray[1][0], LSArray[1][1]);
			glEnd();
			glFlush();
			
			rubberband = 0;
			
		}

		break;
	default:
		break;
	}
	//printf("x: %d, y: %d\n", x, y);
}

void motion(int x, int y) {
	
	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_INVERT);
	
	glColor3d(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	if (rubberband) {
		/* 以前のラバーバンドを消す */
		glVertex2iv(LSArray[0]);
		glVertex2iv(LSArray[1]);
	}

		glVertex2iv(LSArray[0]);
		glVertex2d(x, y);
	glEnd();
	glFlush();

	/* 論理演算機能 OFF */
	glLogicOp(GL_COPY);
	glDisable(GL_COLOR_LOGIC_OP);

	float dis = distance(LSArray);
	float ang = angle(LSArray);
	ExAns[exnum][0] = ang;
	ExAns[exnum][1] = dis;
	printf(" ang:%f, dis:%f\n", ExAns[exnum][0], ExAns[exnum][1]);

	// 現在の座標の保存
	LSArray[1][0] = x;   LSArray[1][1] = y;
	
	
	rubberband = 1;
}


void exprocess(void) {
	exnum++;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		exprocess();
	case 'p':
		for (int i = 0; i < 10; i++) {
			printf("%d: ang:%f, dis:%f\n", i, ExAns[i][0], ExAns[i][1]);
		}
	case '\033':  /* '\033' は ESC の ASCII コード */
		exit(0);
	/*default:
		break;*/
	}
}

int main(int argc, char* argv[])
{
	for (int i = 0; i < 100; i++) {
		ExAns[i][0] = 0.0f;   ExAns[i][1] = 0.0f;
	}
	

	// ウィンドウを開く位置、そのサイズ
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(WinSizeX, WinSizeY);
	// OpenGL環境の初期化
	glutInit(&argc, argv);
	// 表示モードを設定する
	glutInitDisplayMode(GLUT_RGBA);   // GLUT_INDEXもある
	// ウインドウを開く
	glutCreateWindow("Study Hard");
		//glutCreateWindow(argv[0]);
	// 描く関数
	glutDisplayFunc(display);
	// ウィンドウリサイズ：ビューポートと座標の設定
	glutReshapeFunc(resize);
	// マウス
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	// キーボード
	glutKeyboardFunc(keyboard);
	// init
	init();
	// 無限ループでコールバック
	glutMainLoop();
	return 0;
}