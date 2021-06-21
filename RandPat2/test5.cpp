#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

const int PointsNum = 2000;
double PointsArray[PointsNum][2];
const double PI = 3.141593;
const int WinSizeX = 600, WinSizeY = 600;

void ArrayInit(void) {
	double CircleRad = 0.5f;

	for (int i = 0; i < PointsNum; i++) {
		double Degree = ((double)360.0f / (double)PointsNum) * (double)i;
		PointsArray[i][0] = CircleRad * cos(Degree * PI / (double)180.0f);
		PointsArray[i][1] = CircleRad * sin(Degree * PI / (double)180.0f);
	}

}

void display(void)
{
	// ���̕`��
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(1.0, 0.0, 0.0);

	glLineWidth(5.0);
	glBegin(GL_LINE_LOOP);
		glVertex2d(0, 1);
		glVertex2d(0, -1);
	glEnd();
	glBegin(GL_LINE_LOOP);
		glVertex2d(1, 0);
		glVertex2d(-1, 0);
	glEnd();
	

	// �~�̕`��
	ArrayInit();
	glLineWidth(5.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < PointsNum; i++) {
		glVertex2d(PointsArray[i][0], PointsArray[i][1]);
	}
	glEnd();

	glFlush();
}

void init(void) {
	glClearColor(0.859, 0.859, 0.859, 1.0);
}

/* �}�E�X�C�x���g */
void LineDraw(int x, int y) {
	glColor3d(1.0, 0.0, 0.0);
	glLineWidth(5.0);
	glBegin(GL_LINE_LOOP);
		glVertex2d(0, 0);
		glVertex2d(100, 100);
		//glVertex2d(WinSizeX/2, WinSizeY/2);
		//glVertex2d(x, y);
	glEnd();
	glFlush();
}

//https://tokoik.github.io/opengl/libglut.html
int point[2][2];
void mouse(int button, int state, int x, int y) {   // x,y�̓E�C���h�E�T�C�Y���̃s�N�Z���l
	

	//LineDraw(x, y);
	switch (button) {
	case GLUT_LEFT_BUTTON:
		printf("x: %d, y: %d\n", x, y);
		point[0][0] = x;
		point[0][1] = y;

		glColor3d(1.0, 0.0, 0.0);
		glLineWidth(5.0);
		glBegin(GL_LINE_LOOP);
			glVertex2d(300, 300);
			glVertex2d(x, y);
		glEnd();
		glFlush();
	}
}

//void mouse(int button, int state, int x, int y) {   // x,y�̓E�C���h�E�T�C�Y���̃s�N�Z���l
//	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
//
//	printf("x: %d, y: %d\n", x, y);
//}


int main(int argc, char* argv[])
{
	// �E�B���h�E�̈ʒu�A�T�C�Y
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
	// �}�E�X
	glutMouseFunc(mouse);
	// init
	init();
	// �������[�v�ŃR�[���o�b�N
	glutMainLoop();
	return 0;
}