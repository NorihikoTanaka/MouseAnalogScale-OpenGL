#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//#define VAS_Circle
#define VAS_Bar

const int PointsNum = 2000;
double CPArray[PointsNum][2];   // �~�̓_�Q�z��
GLint LSArray[2][2];   // �����̎n�_�I�_���W�z��
int rubberband = 0;        /* ���o�[�o���h�̏��� */
const double PI = 3.141593;
const double WinSizeX = 600, WinSizeY = 600;
const double BarLPos = 100, BarRPos = 500;

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

void render_string(float x, float y, float z, const char* str) {
	glRasterPos3f(x, y, z);

	const char* c = str;
	while (*c) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c++);
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
#ifdef VAS_Circle
	// �����̕`��
	glColor3d(1.0, 0.0, 0.0);

	glLineWidth(5.0);
	glBegin(GL_LINES);
		glVertex2d(WinSizeX / 2, 0);
		glVertex2d(WinSizeX / 2, WinSizeY);
		glVertex2d(0, WinSizeY / 2);
		glVertex2d(WinSizeX, WinSizeY / 2);
	glEnd();
	
	// �~�̕`��
	CirclePointsInit();
	glLineWidth(5.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < PointsNum; i++) {
		//printf("C: x:%f, y:%f\n", PointsArray[i][0], PointsArray[i][1]);
		glVertex2d(WinSizeX / 2 + CPArray[i][0], WinSizeY / 2 + CPArray[i][1]);
	}
	glEnd();

	// �����̕`��
	glColor3d(0.0, 0.0, 0.0);
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glVertex2iv(LSArray[0]);
	glVertex2iv(LSArray[1]);
	glEnd();
#endif

#ifdef VAS_Bar
	// �����̕`��
	glColor3d(1.0, 0.0, 0.0);

	glLineWidth(3.0);
	glBegin(GL_LINES);
	//�
	//glVertex2d(WinSizeX / 10 * 2, WinSizeY / 2);   glVertex2d(WinSizeX / 10 * 8, WinSizeY / 2);
	glVertex2d(BarLPos, WinSizeY / 2);   glVertex2d(BarRPos, WinSizeY / 2);
	//�[
	glVertex2d(BarLPos, WinSizeY / 2 - 20);
	glVertex2d(BarLPos, WinSizeY / 2 + 20);
	glVertex2d(BarRPos, WinSizeY / 2 - 20);
	glVertex2d(BarRPos, WinSizeY / 2 + 20);
	glEnd();

	// �����̕`��
	//glColor3d(1.0, 1.0, 0.5);
	glColor3d(1.0, 0.0, 0.0);
	glLineWidth(4.0);
	glBegin(GL_LINES);
	glVertex3d(LSArray[0][0], LSArray[0][1], 1.0f);
	glVertex3d(LSArray[1][0], LSArray[1][1], 1.0f);
	glEnd();

	glColor3d(1.0, 0.0, 0.0);
	//printf("width: %f\n", WinSizeX / 10);
	render_string(BarLPos - 5, WinSizeY / 2 + 40, 0, "0");
	//render_string(WinSizeX / 10 * 2 - 35, WinSizeY / 2 + 60, 0, "Hard");
	//render_string(WinSizeX / 10 * 2 - 65, WinSizeY / 2 + 80, 0, "understand");
	render_string(BarRPos - 15, WinSizeY / 2 + 40, 0, "100");
	//render_string(WinSizeX / 10 * 8 - 35, WinSizeY / 2 + 60, 0, "Easy");
	//render_string(WinSizeX / 10 * 8 - 65, WinSizeY / 2 + 80, 0, "understand");

#endif

	glFlush();
}

void resize(int w, int h){
	/* �E�B���h�E�S�̂��r���[�|�[�g�ɂ��� */
	glViewport(0, 0, w, h);
	/* �ϊ��s��̏����� */
	glLoadIdentity();
	/* �X�N���[����̍��W�n���}�E�X�̍��W�n�Ɉ�v������ */
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

#ifdef VAS_Circle
//https://tokoik.github.io/opengl/libglut.html
void mouse(int button, int state, int x, int y) {   // x,y�̓E�C���h�E�T�C�Y���̃s�N�Z���l
	LSArray[0][0] = WinSizeX / 2;   LSArray[0][1] = WinSizeY / 2;

	glColor3d(0.0, 1.0, 0.0);
	switch (button) {
	case GLUT_LEFT_BUTTON:

		if (state == GLUT_UP) {
			// ���������W�̕ۑ�
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
		/* �ȑO�̃��o�[�o���h������ */
		glVertex2iv(LSArray[0]);
		glVertex2iv(LSArray[1]);
	}

		glVertex2iv(LSArray[0]);
		glVertex2d(x, y);
	glEnd();
	glFlush();

	/* �_�����Z�@�\ OFF */
	glLogicOp(GL_COPY);
	glDisable(GL_COLOR_LOGIC_OP);

	float dis = distance(LSArray);
	float ang = angle(LSArray);
	ExAns[exnum][0] = ang;
	ExAns[exnum][1] = dis;
	printf(" ang:%f, dis:%f\n", ExAns[exnum][0], ExAns[exnum][1]);

	// ���݂̍��W�̕ۑ�
	LSArray[1][0] = x;   LSArray[1][1] = y;
	
	
	rubberband = 1;
}
#endif


#ifdef VAS_Bar
void mouse(int button, int state, int x, int y) {   // x,y�̓E�C���h�E�T�C�Y���̃s�N�Z���l
	//LSArray[0][0] = WinSizeX / 2;   LSArray[0][1] = WinSizeY / 2;

	glColor3d(0.0, 1.0, 0.0);

	switch (button) {
	case GLUT_LEFT_BUTTON:

		if (state == GLUT_UP) {
			// ���������W�̕ۑ�
			LSArray[0][0] = x;   LSArray[0][1] = WinSizeY / 2 - 30;
			LSArray[1][0] = x;   LSArray[1][1] = WinSizeY / 2 + 30;

			//if (x < WinSizeX / 10 * 2) { 
			if (x < BarLPos) {
				//LSArray[0][0] = WinSizeX / 10 * 2;   LSArray[1][0] = WinSizeX / 10 * 2;
				LSArray[0][0] = BarLPos;   LSArray[1][0] = BarLPos;
			}
			//if (x > WinSizeX / 10 * 8) { 
			if (x > BarRPos) {
				//LSArray[0][0] = WinSizeX / 10 * 8;   LSArray[1][0] = WinSizeX / 10 * 8;
				LSArray[0][0] = BarRPos;   LSArray[1][0] = BarRPos;
			}

			glColor3d(1.0, 0.0, 0.0);
			glBegin(GL_LINES);
			glVertex3d(LSArray[0][0], LSArray[0][1], 1.0f);
			glVertex3d(LSArray[1][0], LSArray[1][1], 1.0f);
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
		glVertex3d(LSArray[0][0], LSArray[0][1], 1.0f);
		glVertex3d(LSArray[1][0], LSArray[1][1], 1.0f);
	}

	// �񓚔͈͂̐ݒ�
	int posX = x;
	//if (x < WinSizeX / 10 * 2) { posX = WinSizeX / 10 * 2; }
	//if (x > WinSizeX / 10 * 8) { posX = WinSizeX / 10 * 8; }

	if (x < BarLPos) { posX = BarLPos; }
	if (x > BarRPos) { posX = BarRPos; }

	glVertex3d(posX, WinSizeY / 2 - 30, 1.0f);
	glVertex3d(posX, WinSizeY / 2 + 30, 1.0f);
	glEnd();
	glFlush();
		
	/* �_�����Z�@�\ OFF */
	glLogicOp(GL_COPY);
	glDisable(GL_COLOR_LOGIC_OP);
		
	ExAns[exnum][0] = posX - BarLPos;
	ExAns[exnum][1] = -1;
	printf(" length:%f, -:%f\n", ExAns[exnum][0], ExAns[exnum][1]);
	
	// ���݂̍��W�̕ۑ�
	LSArray[0][0] = posX;   LSArray[0][1] = WinSizeY / 2 - 30;
	LSArray[1][0] = posX;   LSArray[1][1] = WinSizeY / 2 + 30;

	rubberband = 1;
}
#endif


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
	case '\033':  /* '\033' �� ESC �� ASCII �R�[�h */
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
	

	// �E�B���h�E���J���ʒu�A���̃T�C�Y
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(WinSizeX, WinSizeY);
	// OpenGL���̏�����
	glutInit(&argc, argv);
	// �\�����[�h��ݒ肷��
	glutInitDisplayMode(GLUT_RGBA);   // GLUT_INDEX������
	// �E�C���h�E���J��
	glutCreateWindow("Study Hard");
		//glutCreateWindow(argv[0]);
	// �`���֐�
	glutDisplayFunc(display);
	// �E�B���h�E���T�C�Y�F�r���[�|�[�g�ƍ��W�̐ݒ�
	glutReshapeFunc(resize);
	// �}�E�X
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	// �L�[�{�[�h
	glutKeyboardFunc(keyboard);
	// init
	init();
	// �������[�v�ŃR�[���o�b�N
	glutMainLoop();
	return 0;
}