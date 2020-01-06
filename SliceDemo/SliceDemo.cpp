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
static int du = 90, OriX = -1, OriY = -1;   //du是视点和x轴的夹角
static float r = 1.5, h = 0.0;   //r是视点绕y轴的半径，h是视点高度即在y轴上的坐标
static float c = PI / 180.0;    //弧度和角度转换参数

GLsizei width = 600, height = 600;
int uStepsNum = 50, vStepNum = 50;

static MyModel model;

static double num1_avg = 0, num2_avg = 0, num3_avg = 0;
using namespace std;


void Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)  //记录鼠标按下位置
		OriX = x, OriY = y;
}

void onMouseMove(int x, int y)   //处理鼠标拖动
{
	du += x - OriX; //鼠标在窗口x轴方向上的增量加到视点与x轴的夹角上，就可以左右转
	h += 0.03*(y - OriY);  //鼠标在窗口y轴方向上的改变加到视点y的坐标上，就可以上下转
	//if (h > 1.0)   h = 1.0;  //对视点y坐标作一些限制，不会使视点太奇怪
	//else if (h < -1.0) h = -1.0;
	OriX = x, OriY = y;  //将此时的坐标作为旧值，为下一次计算增量做准备
}

void init()
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
	glViewport(0, 0, w, h);    //截图;1、2为视口的左下角;3、4为视口的宽度和高度
	glMatrixMode(GL_PROJECTION);    //将当前矩阵指定为投影矩阵
	glLoadIdentity();
	gluPerspective(75.0, (float)w / h, 1.0, 1000.0); //1、视野在Y-Z平面的角度[0,180];2、投影平面宽度与高度的比率;3、近截剪面到视点的距离;4、远截剪面到视点的距离
	glMatrixMode(GL_MODELVIEW);     //对模型视景矩阵堆栈应用随后的矩阵操作.	
}

void drawHoop()
{
	
	//vector<Hoop> hoops = model.get_original_hoops();	

	vector<Hoop> hoops = model.clipper_operation(3,8.5);

	//vector<Hoop> hoops = model.merge();
	/*for (size_t i = 0; i < 5; i++) //i < hoops.size()
	{		
		glBegin(GL_LINE_LOOP);
		for (size_t j = 0; j < hoops[i].size(); j++) //j < hoops[i].size()
		{
			double x = hoops[i][j].x/30;
			double y = hoops[i][j].y/30;
			double z = hoops[i][j].z/30;
			Point a = Point(x, y, z);

			glVertex3d(a.x, a.y, a.z);
		}
		glEnd();			
	}*/

	
	for (size_t i = 0; i < hoops.size(); i++) //i < hoops.size()
	{
		glBegin(GL_LINE_LOOP);
		for (size_t j = 0; j < hoops[i].size(); j++) //j < hoops[i].size()
		{
			double x = hoops[i][j].x / 30;
			double y = hoops[i][j].y / 30;
			double z = hoops[i][j].z / 30;
			Point a = Point(x, y, z);

			glVertex2d(a.x, a.y);
		}
		glEnd();
	}


	/*glBegin(GL_LINE_LOOP);
	for (size_t j = 0; j < 36; j++) //j < hoops[i].size()
	{
		
		double R = 1;

		double x = R * cos(j * (2 * PI) / 36.0);
		double y = R * sin(j * (2 * PI) / 36.0);

		glVertex2d(x, y);
	}
	glEnd();*/
	
	
}

void output_gcode()
{
	model.output_gcode_3();
	//model.output_gcode_2();
}


void displayFunc() //没有用到
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(1.0);
	glRotated(-150, 1, 0, 0);
	glRotated(90, 0, 1, 0);
	//glRotated(30, 0, 0, 1);
	drawHoop();
	glutSwapBuffers();
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除缓冲，GL_COLOR_BUFFER_BIT ：颜色缓冲标志位
	glLoadIdentity();                                       //重置当前矩阵为4*4的单位矩阵
															
	gluLookAt(r*cos(c*du), h, r*sin(c*du), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);   //从视点看远点
	glColor3f(1.0, 0.0, 0.0);
	//glutWireTeapot(0.5f);
	drawHoop();
	glutSwapBuffers();                                      //交换两个缓冲区指针
}


int main(int argc, char* argv[])
{

	glutInit(&argc, argv);                                          //初始化glut库
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);      //设置初始显示模式
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("***************");
	init();
	model = MyModel("F:\\slicer_layers129.txt");
	model.initModel();
	output_gcode();
	//MyFile::readInToArray();
	//write2txt();
	//simplify();
	glutReshapeFunc(reshape);       //
	glutDisplayFunc(renderScene);           //
	glutIdleFunc(renderScene);          //设置不断调用显示函数
	glutMouseFunc(Mouse);
	glutMotionFunc(onMouseMove);
	//glutDisplayFunc(renderScene);                                 //调用函数
	glutMainLoop();//enters the GLUT event processing loop.
	
	return 0;	

}


