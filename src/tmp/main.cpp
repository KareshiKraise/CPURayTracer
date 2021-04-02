#include <iostream>
#include "World.h"
#include "freeglut_callbacks.h"


World _W;
GLuint pbo;
glm::vec3 eye(2.0f, 2.0f, 0.0f);
glm::vec3 rotated(0.0f);
int gsize = stW * stH * sizeof(rgb);


void initPBO(void) {
	
	glGenBuffers(1, &pbo);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);

	

	glBufferData(GL_PIXEL_UNPACK_BUFFER, gsize , &(_W.get_buffer().data()[0]), GL_DYNAMIC_DRAW);
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "error " << std::hex << err << std::endl;
	}

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

}

void keyboard(unsigned char key, int x, int y) {
	
	switch (key) {
	case 'w':
	case 'W':
		rotated = glm::rotate(eye, 0.2f, glm::vec3(1.0, 0.0, 0.0));
		_W.change_eye(rotated);
		eye = rotated;

		break;
	
	
	case 'a':
	case 'A':
		rotated = glm::rotate(eye, 0.2f, glm::vec3(0.0, 1.0, 0.0));
		_W.change_eye(rotated);
		eye = rotated;
		 break;

	case 's':
	case 'S':
		rotated = glm::rotate(eye, -0.2f, glm::vec3(1.0, 0.0, 0.0));
		_W.change_eye(rotated);
		eye = rotated;
		break;

	case 'd':
	case 'D':
		rotated = glm::rotate(eye, -0.2f, glm::vec3(0.0, 1.0, 0.0));
		_W.change_eye(rotated);
		eye = rotated;
		break;

	default:
		break;
	}
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
	
	_W.render();
	//std::cout << "rendering ended" << std::endl;
	
	glBufferSubData(GL_PIXEL_UNPACK_BUFFER, 0, gsize, &(_W.get_buffer().data()[0]));
	
	glDrawPixels(stW, stH, GL_RGB, GL_UNSIGNED_BYTE, 0);

	//GLenum err;
	//while ((err = glGetError()) != GL_NO_ERROR) {
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
		
	_W.build_standard_scene();
	//_W.render_to_ppm("rt.ppm");
	_W.render();
	
	initPBO();


	glutMainLoop();
	
	
	return 0;
}