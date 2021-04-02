#include "freeglut_callbacks.h"
#include "Utilities.h"

bool initGL(int *argv, char **argc) {
 glutInit(argv, argc);
 glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
 glutInitWindowPosition(100, 100);
 glutInitWindowSize(stW, stH);
 glutCreateWindow("mate20");
 glClearColor(0.0, 0.0, 0.0, 1.0);

 GLenum err = glewInit();
 if (GLEW_OK != err)
	{
		std::cout << "could not load glew" << std::endl;
		std::cout << glewGetErrorString(err) << std::endl;
		return false;
	}

 return true;
}




void resize(int w, int h) {
	if (w > 0 && h > 0){
		glViewport(0,0,w,h);
		glutPostRedisplay();
	}
}

void idleFunc() {
	//
	glutPostRedisplay();
}



void onMouse(int button, int state, int x, int y) {
	
}

void onMotion(int x, int y) {

}