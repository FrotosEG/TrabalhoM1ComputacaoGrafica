#include <GL/freeglut.h>
#include <cmath>

float angleX = 0.0f;
float angleY = 0.0f;
float angleZ = 0.0f;
int lastMouseX, lastMouseY;

void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void rotacionar(float angle, float x, float y, float z) {
	float magnitude = sqrt(x * x + y * y + z * z);
	if (magnitude > 0) {
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}

	float radians = angle * 3.141592653589793 / 180.0f; // Convertendo graus para radianos
	float c = cos(radians);
	float s = sin(radians);
	float t = 1 - c;

	// Criando a matriz de rotação
	float rotMatrix[16] = {
		t * x * x + c,        t * x * y - s * z,    t * x * z + s * y,    0,
		t * x * y + s * z,    t * y * y + c,        t * y * z - s * x,    0,
		t * x * z - s * y,    t * y * z + s * x,    t * z * z + c,        0,
		0,                    0,                    0,                    1
	};

	// Multiplicando a matriz de modelview atual pela matriz de rotação
	glMultMatrixf(rotMatrix);
}

void translacao(float x, float y, float z) {
	glMatrixMode(GL_MODELVIEW);
	GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix); 
	matrix[12] += matrix[0] * x + matrix[4] * y + matrix[8] * z;
	matrix[13] += matrix[1] * x + matrix[5] * y + matrix[9] * z;
	matrix[14] += matrix[2] * x + matrix[6] * y + matrix[10] * z;

	glLoadMatrixf(matrix);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	translacao(1.5f, 0.0f, -7.0f);

	rotacionar(angleX, 1.0f, 0.0f, 0.0f);
	rotacionar(angleY, 0.0f, 1.0f, 0.0f);
	rotacionar(angleZ, 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);
	// Face de cima (y = 1.0f)
	// Define os vértice em ordem anti-horário com a face apontando para cima
	glColor3f(0.0f, 1.0f, 0.0f);     // Verde
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Face de cima (y = -1.0f)
	glColor3f(1.0f, 0.5f, 0.0f);     // Laranja
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Face da frente  (z = 1.0f)
	glColor3f(1.0f, 0.0f, 0.0f);     // Vermelho
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Face de trás (z = -1.0f)
	glColor3f(1.0f, 1.0f, 0.0f);     // Amarelo
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Face esquerda (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f);     // Azul
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Face direita (x = 1.0f)
	glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();

	glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		//Rotação de cada eixo, x, y, z
	case 'x': angleX += 5.0f; break;
	case 'X': angleX += 5.0f; break;
	case 'y': angleY += 5.0f; break;
	case 'Y': angleY += 5.0f; break;
	case 'z': angleZ += 5.0f; break;
	case 'Z': angleZ += 5.0f; break;
	}
	if (angleX >= 360.0f) angleX -= 360.0f;
	if (angleY >= 360.0f) angleY -= 360.0f;
	if (angleZ >= 360.0f) angleZ -= 360.0f;

	glutPostRedisplay();
}

void movimentoMouse(int x, int y) {
	int dx = x - lastMouseX;
	int dy = y - lastMouseY;

	angleY += dx * 0.1f;
	angleX += dy * 0.1f;

	lastMouseX = x;
	lastMouseY = y;

	glutPostRedisplay();
}

void botaoMouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		lastMouseX = x;
		lastMouseY = y;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("3D Shapes");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard); // Registrar a função de callback para eventos de teclado
	glutMotionFunc(movimentoMouse); // Registra a função de movimento do mouse
	glutMouseFunc(botaoMouse); // Registra a função de botão do mouse

	initGL();
	glutMainLoop();
	return 0;
}