// SliceDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <map> 
#include <string>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <GL/freeglut.h>
#include "MyModel.h"
#include "MyData.h"

#define PI 3.14159265358979323846
#define PI2 6.28318530717958647692
#define  GLUT_WHEEL_UP 3           //定义滚轮操作
#define  GLUT_WHEEL_DOWN 4

bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraDistanceX;
float cameraDistanceY;
float cameraAngleX;
float cameraAngleY;
float times = 1;

static MyModel model;
using namespace std;

void mouseCB(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;
	times = 1;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}

	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}

	/*
	* 鼠标滚轮控制图形缩放
	*/
	else if (state == GLUT_UP && button == GLUT_WHEEL_UP)
	{
		times = 0.03f + 1;
		glutPostRedisplay();
	}

	else if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)
	{
		times = -0.03f + 1;
		glutPostRedisplay();
	}
}

void mouseMotionCB(int x, int y)
{
	cameraAngleX =  0;
	cameraAngleY =  0;
	cameraDistanceX = cameraDistanceY = 0;

	if (mouseLeftDown)
	{
		cameraAngleY += (x - mouseX) * 0.15f;
		cameraAngleX += (y - mouseY) * 0.15f;
		mouseX = x;
		mouseY = y;
	}
	if (mouseRightDown)
	{
		cameraDistanceX = (x - mouseX) * 0.0025f;
		cameraDistanceY = -(y - mouseY) * 0.0025f;
		mouseY = y;
		mouseX = x;
	}

	glutPostRedisplay();
}

void init(void)
{	

	glClearColor(0, 1, 1, 1);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 1, 1, 1);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 0.2f };
	GLfloat light_diffuse[] = { 0.5f, 0.5f, 0.5f, 0.2f };
	GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 0.2f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
}


void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h / (GLfloat)w, 1.5 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-1.5*(GLfloat)w / (GLfloat)h, 1.5*(GLfloat)w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



void drawHoop()
{
	
	vector<Hoop> hoops = model.merge_newway();	//原始的get_original_hoops
	//vector<Hoop> hoops = model.output_gcode_newway();
	//vector<Hoop> hoops = model.clipper_operation(11,8.5);//最底下裁剪其中一层的

	/*vector<Hoop> hoops;	
	hoops.push_back(model.getGcode_one(11));
	hoops.push_back(model.stepThree(11));*/

	for (size_t i = 0; i < hoops.size(); i++) //i < hoops.size()
	{
		glBegin(GL_LINE_STRIP);
		for (size_t j = 0; j < hoops[i].size(); j++) //j < hoops[i].size()
		{
			double x = hoops[i][j].x / 30;
			double y = hoops[i][j].y / 30 ; //-1.2去掉 //double y = hoops[i][j].y / 30 /180 * PI * 20 - 1.5
			double z = hoops[i][j].z / 30;
			Point a = Point(x, y, z);

			glVertex3d(a.x, a.y, a.z);
		}
		glEnd();
	}

	/*vector<Hoop> hoops;
	hoops.push_back(model.getGcode_one(11));
	
	//hoops.push_back(model.stepOne(11));
	//hoops.push_back(model.stepTwo(11));
	hoops.push_back(model.stepThree(11));
	//model.stepThree(10);

	
	for (size_t i = 0; i < hoops.size(); i++) //i < hoops.size()
	{		
		glBegin(GL_LINE_STRIP);
		for (size_t j = 0; j < hoops[i].size(); j++) //j < hoops[i].size()
		{
			double x = hoops[i][j].x/30;
			double y = hoops[i][j].y/30-1.2;
			double z = hoops[i][j].z/30;
			Point a = Point(x, y, z);

			glVertex2d( a.y, a.z);
		}
		glEnd();
	}
	*/
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glTranslatef(0.0, 0.0, 0.0);//平移
	glScalef(times, times, times);//缩放
	
	glTranslatef(cameraDistanceX, cameraDistanceY, 0);//平移
	glRotatef(cameraAngleY, 1, 0, 0);
	glRotatef(cameraAngleY, 0, 1, 0);
	
	glColor3f(1.0, 0.0, 0.0);
	drawHoop();
	
	glutSwapBuffers();
}

//输出gcode；
void output_gcode()
{	
	//model.output_gcode_whole();
	//model.getFinalResult();
	model.getFinalResult_newway();
	model.output_layers();//输出每层hoop的分布
	////model.output_gcode_2();
}

int main(int argc, char* argv[])
{

	glutInit(&argc, argv);                                          //初始化glut库
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);      //设置初始显示模式
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("窗口");
	init();
	model = MyModel();//
	model.initModel("F:\\hediao2.txt");//F:\\heshang.txt    slicer_layers_R1_5.txt
	output_gcode();
	
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	
	return 0;

}


