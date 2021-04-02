#include <iostream>
#include "freeglut_callbacks.h"
#include "Utilities.h"
#include "Scene.h"

GLuint pbo;
Scene gscene;
unsigned int elapsed;
unsigned int post;

void initPBO(void) {
	
	glGenBuffers(1, &pbo);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);

	glBufferData(GL_PIXEL_UNPACK_BUFFER, gscene.buf_size , &(gscene.m_image.data()[0]), GL_DYNAMIC_DRAW);
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "error " << std::hex << err << std::endl;
	}

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

}

void keyboard(unsigned char key, int x, int y) {
	
	point3D n_eye = gscene.m_cam.m_eye;
	point3D n_look = gscene.m_cam.m_lookat;
	glm::vec3 rotated;
	vetor dir;

	switch (key) {
	case 'w':
	case 'W':

		dir = n_look - n_eye;
		n_eye = n_eye + dir * 0.2f;
		gscene.change_eye(n_eye);

		break;	
	
	case 'a':
	case 'A':
		if (!gscene.rotate)
		{
			gscene.m_angle = -gscene.m_angle;
			gscene.rotate = true;
		}
		else {
			gscene.rotate = false;
		}
		break;

	case 's':
	case 'S':

		dir = n_look - n_eye;
		n_eye = n_eye - dir * 0.2f;
		gscene.change_eye(n_eye);
		
		break;

	case 'd':
	case 'D':
		if (!gscene.rotate)
		{
			gscene.m_angle = abs(gscene.m_angle);
			gscene.rotate = true;
		}
		else {
			gscene.rotate = false;
		}
		break;

	case 'r':
	case 'R':
		rotated = glm::rotate(n_eye, 0.05f, gscene.m_cam.m_u);
		gscene.change_eye(rotated);
		
		break;

	case 'f':
	case 'F':
		rotated = glm::rotate(n_eye, -0.05f, gscene.m_cam.m_u);
		gscene.change_eye(rotated);
		
		break;

	case '1':
		gscene.num_lights = 1;
		std::cout << "num of lights = " << gscene.num_lights << std::endl;
		break;
	case '2':
		gscene.num_lights = 2;
		std::cout << "num of lights = " << gscene.num_lights << std::endl;
		break;

	
	default:
		break;
	}
}

void drawText() {

}

void renderScene(void) {

	elapsed = glutGet(GLUT_ELAPSED_TIME);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
	
	gscene.render();		

	glBufferSubData(GL_PIXEL_UNPACK_BUFFER, 0, gscene.buf_size, &(gscene.m_image.data()[0]));	
	
	post = glutGet(GLUT_ELAPSED_TIME);

	elapsed = post - elapsed;
	float fps = 1000.f / elapsed;
	elapsed = post;
	std::cout << "FPS = " << fps << std::endl;
	
	
	glDrawPixels((GLsizei)stW, (GLsizei)stH, GL_RGB, GL_FLOAT, 0);

	//GLenum err;
	// while ((err = glGetError()) != GL_NO_ERROR) {
	//	std::cout << "error " << std::hex << err << std::endl;
	//}	
	
	

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glutSwapBuffers();
}

int main(int argc, char **argv) {
	
	if (!initGL(&argc, argv)) {
		return -1;
	}

	glutDisplayFunc(renderScene);
	glutReshapeFunc(resize);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMotion);
		
	//build world
	gscene.build_standard_scene();

	//gscene.render();
	
	initPBO();


	glutMainLoop();
	
	
	return 0;
}