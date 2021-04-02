#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_

#include<gl\glew.h>
#include<gl\freeglut.h>
#include <iostream>

bool initGL(int *argv, char **argc);

void renderScene(void);

void resize(int w, int h);

void idleFunc();

void keyboard(unsigned char key, int x, int y);

void onMouse(int button, int state, int x, int y);

void onMotion(int x, int y);

#endif


