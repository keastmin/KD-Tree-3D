#include <iostream>
#include "GL/glut.h"
#include "Vec3.h"
#include "Particle.h"
#include "KdTree.h"
#include <vector>

using namespace std;

#define NUM_PARTICLE 20000

int lastx = 0;
int lasty = 0;
float _zoom = 0.0f;
double rotx = 0.0f;
double roty = 0.0f;
float _trans_x = 0.0f;
float _trans_y = 0.0f;
unsigned char Buttons[3] = { 0 };

vector<Particle*> _particles;
KdTree* _kdTree;

void InitParticles(void)
{
	for (int i = 0; i < NUM_PARTICLE; i++) {
		double x = (double)(rand()) / RAND_MAX;
		double y = (double)(rand()) / RAND_MAX;
		double z = (double)(rand()) / RAND_MAX;
		if (i < 40) {
			printf("%lf, %lf, %lf\n", x, y, z);
		}
		double sdf1 = x * x + y * y  + z * z - 0.4 * 0.4;
		double sdf2 = (1.0 - x) * (1.0 - x) + (1.0 - y) * (1.0 - y) + (1.0 - z) * (1.0 - z) - 0.4 * 0.4;
		double sdf3 = (0.5 - x) * (0.5 - x) + (0.5 - y) * (0.5 - y) + (0.5 - z) * (0.5 - z) - 0.3 * 0.3;
		//double sdf = sdf3;
		double sdf = fmin(fmin(sdf1, sdf2), sdf3);
		if (sdf <= 0.0) {
			_particles.push_back(new Particle(false, Vec3<double>(x, y, z)));
		}
	}
}

void InitFlag(void)
{
	for (auto p : _particles) {
		p->_active = false;
		p->_collision = false;
	}
}

void Init(void)
{
	glEnable(GL_DEPTH_TEST);
	InitParticles();

	_kdTree = new KdTree(100);
	_kdTree->buildTree(_particles);
	_kdTree->query(_particles, 0.08, 33);
}

void Rendering(void)
{
	_kdTree->draw();

	glDisable(GL_LIGHTING);

	glPointSize(2.0);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	for (auto p : _particles) {
		glColor3f(0.0f, 0.0f, 0.0f);
		if (p-> _active) {
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		if (p-> _collision) {
			glColor3f(1.0f, 1.0f, 0.0f);
		}
		auto pos = p->_pos;
		glVertex3f((float)pos.x(), (float)pos.y(), (float)pos.z());
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();
	glPopMatrix();
}

void Display(void)
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix;
	glTranslatef(0, 0, -_zoom);
	glTranslatef(_trans_x, _trans_y, 0);
	glRotatef(rotx, 1, 0, 0);
	glRotatef(roty, 0, 1, 0);
	Rendering();

	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / h, 0.1, 100);
	double r = (float)w / h;
	double xw = 1.0 * r;
	double yw = 1.0 / r;
	glTranslatef(-0.5f, -0.5f, -3.0f);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'Q':
	case 'q':
		exit(0);
	case 'S':
	case 's':
		InitFlag();
		_kdTree->query(_particles, 0.08, rand() % _particles.size());
		break;
	}
	glutPostRedisplay();
}

void Motion(int x, int y)
{
	int diffx = x - lastx;
	int diffy = y - lasty;
	lastx = x;
	lasty = y;

	if (Buttons[2]) {
		_zoom -= (float)0.02f * diffx;
	}
	else if (Buttons[1]) {
		_trans_x += (float)0.02f * diffx;
		_trans_y -= (float)0.02f * diffy;
	}
	else if (Buttons[0]) {
		rotx += (float)0.2f * diffy;
		roty += (float)0.2f * diffx;
	}

	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	lastx = x;
	lasty = y;
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		Buttons[0] = state == GLUT_DOWN ? 1 : 0;
		break;
	case GLUT_MIDDLE_BUTTON:
		Buttons[1] = state == GLUT_DOWN ? 1 : 0;
		break;
	case GLUT_RIGHT_BUTTON:
		Buttons[2] = state == GLUT_DOWN ? 1 : 0;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("kdTree 3");
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	Init();
	glutMainLoop();
}