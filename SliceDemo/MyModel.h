#pragma once

#include <vector>
#include "MyData.h"
#include "clipper.hpp"

using namespace std;
using namespace ClipperLib;
#define PI 3.1415926535 //3.14159265358979323846
//const double eps = 1e-12;


class MyModel
{
public:
	MyModel();
	MyModel(string file_path);
	~MyModel();
	vector<string> Split(const string& str, const string& delim);
	void initModel(string file_path);	
	vector<Hoop> get_hoops();
	vector<Hoop> get_original_hoops();	

	void output_gcode();	//32�㼰����
	void output_gcode_whole();	  //ȫ���Ĳ��ں���һ�����Gcode��nc�ļ�
	vector<Hoop> output_gcode_newway();
	vector<Hoop> get_gcode_newway();


	void output_layers(); //���ÿ�����ԭʼ����Щ��
	void getFinalResult();	
	void getFinalResult_newway();
	double get_angle(Point p1, Point p2, Point p3);
	
	vector<Hoop> merge();
	vector<Hoop> merge_newway();
	vector<Hoop> getGcode();
	Hoop getGcode_one(int layer_index);
	
	Hoop stepOne(int layer_index);//ȷ��y��һֱ���ӵ�
	Hoop stepTwo(int layer_index);//��one�Ľ����������������һ��
	Hoop stepThree(int layer_index);
	Hoop stepFour(int layer_index);
	vector<vector<int>> dosomething(vector<int> wa);//
	int getMaxZ(Hoop hoop);

	//�Ե�layer_index�㣬�ð뾶ΪR��Բ���вü�
	vector<Hoop> clipper_operation(int layer_index, double R);

	vector<double> getYOrder(Hoop hoop);

	Point corssProduct(Point a, Point b)
	{

		return Point(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}

	Point normalized(Point v)
	{
		//Point v = Point(x, y, z);
		float mag2 = v.x * v.x + v.y * v.y + v.z * v.z;
		if (mag2 > 0)
		{
			float invMag = 1 / sqrtf(mag2);
			return Point(v.x * invMag, v.y * invMag, v.z * invMag);
		}

		return v;
	}

protected:
	//����֮��ļн�
	double Angle(float x1, float y1, float x2, float y2, float x3, float y3);

	//���ļ��ж�ȡ�Ѿ���Ƭ������
	void readInToArray();

	//��
	void Simplify();

	//�õ������xoyƽ���ϵľ���
	double getDistanceXY(Point a, Point b);

	//�õ��ڵ�layer_index�㣬�����p����ĵ������ֵ
	int get_closest_Point(Point p, int layer_index);

	//�õ���layer_index�����ŵĶ���˳��
	vector<int> get_optimize_order(Point p, int layer_index);

	//�õ���cartesian_point����������
	Point get_cylindrical_coordinates(Point cartesian_point);

	//�õ�ÿ����������hoop������ֵ����ֵ����vector<vector<int>> layers��
	void init_layers();

	

	//����Բ
	ClipperLib::Path generate_circle(double R, Point2D center);

	//
	ClipperLib::Path hoop2path(Hoop hoop);	
	
	//
	vector<Hoop> paths2hoops(ClipperLib::Paths polys, double polys_z);

	
	Point getNormalsLeft(Hoop hoop, int index, int layer_index);
	Point getNormalsRight(Hoop hoop, int index, int layer_index);

	//�õ���һ��ƽ���İ뾶
	double getAvgHeight(Hoop hoop);

	int getMaxHoopIndexOfLayer(int layer_index);

	double getoffsetZShouldBe(double y)
	{
		return y * sqrt(3);
	}

	
	
	
private:
	string file_path;
	double x_avg;
	double y_avg;
	double z_avg;
	vector<Hoop> hoops;
	vector<Hoop> original_hoops;
	vector<Hoop> merge_hoops;

	vector<Hoop> Gcode_hoops; //�������Ƕ��������
	vector<Hoop> Gcode_hoops_newway;

	vector<vector<int>> layers;	
	Hoop formor_hoop_step4;
};






