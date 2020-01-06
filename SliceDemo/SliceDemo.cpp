// SliceDemo.cpp : �������̨Ӧ�ó������ڵ㡣
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
static int du = 90, OriX = -1, OriY = -1;   //du���ӵ��x��ļн�
static float r = 1.5, h = 0.0;   //r���ӵ���y��İ뾶��h���ӵ�߶ȼ���y���ϵ�����
static float c = PI / 180.0;    //���ȺͽǶ�ת������

GLsizei width = 600, height = 600;
int uStepsNum = 50, vStepNum = 50;

static MyModel model;

static double num1_avg = 0, num2_avg = 0, num3_avg = 0;
using namespace std;


void Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)  //��¼��갴��λ��
		OriX = x, OriY = y;
}

void onMouseMove(int x, int y)   //��������϶�
{
	du += x - OriX; //����ڴ���x�᷽���ϵ������ӵ��ӵ���x��ļн��ϣ��Ϳ�������ת
	h += 0.03*(y - OriY);  //����ڴ���y�᷽���ϵĸı�ӵ��ӵ�y�������ϣ��Ϳ�������ת
	//if (h > 1.0)   h = 1.0;  //���ӵ�y������һЩ���ƣ�����ʹ�ӵ�̫���
	//else if (h < -1.0) h = -1.0;
	OriX = x, OriY = y;  //����ʱ��������Ϊ��ֵ��Ϊ��һ�μ���������׼��
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
	glViewport(0, 0, w, h);    //��ͼ;1��2Ϊ�ӿڵ����½�;3��4Ϊ�ӿڵĿ�Ⱥ͸߶�
	glMatrixMode(GL_PROJECTION);    //����ǰ����ָ��ΪͶӰ����
	glLoadIdentity();
	gluPerspective(75.0, (float)w / h, 1.0, 1000.0); //1����Ұ��Y-Zƽ��ĽǶ�[0,180];2��ͶӰƽ������߶ȵı���;3�����ؼ��浽�ӵ�ľ���;4��Զ�ؼ��浽�ӵ�ľ���
	glMatrixMode(GL_MODELVIEW);     //��ģ���Ӿ������ջӦ�����ľ������.	
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


void displayFunc() //û���õ�
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //������壬GL_COLOR_BUFFER_BIT ����ɫ�����־λ
	glLoadIdentity();                                       //���õ�ǰ����Ϊ4*4�ĵ�λ����
															
	gluLookAt(r*cos(c*du), h, r*sin(c*du), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);   //���ӵ㿴Զ��
	glColor3f(1.0, 0.0, 0.0);
	//glutWireTeapot(0.5f);
	drawHoop();
	glutSwapBuffers();                                      //��������������ָ��
}


int main(int argc, char* argv[])
{

	glutInit(&argc, argv);                                          //��ʼ��glut��
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);      //���ó�ʼ��ʾģʽ
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
	glutIdleFunc(renderScene);          //���ò��ϵ�����ʾ����
	glutMouseFunc(Mouse);
	glutMotionFunc(onMouseMove);
	//glutDisplayFunc(renderScene);                                 //���ú���
	glutMainLoop();//enters the GLUT event processing loop.
	
	return 0;	

}


