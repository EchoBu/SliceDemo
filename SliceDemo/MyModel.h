#pragma once

#include <vector>
#include "MyData.h"
#include "clipper.hpp"

using namespace std;
using namespace ClipperLib;
#define PI 3.14159265358979323846


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
	void output_gcode();	
	void output_gcode_whole();	
	void output_layers();
	
	vector<Hoop> merge();

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

	//�Ե�layer_index�㣬�ð뾶ΪR��Բ���вü�
	vector<Hoop> clipper_operation(int layer_index, double R);

	//����Բ
	ClipperLib::Path generate_circle(double R, Point2D center);

	//
	ClipperLib::Path hoop2path(Hoop hoop);	
	
	//
	vector<Hoop> paths2hoops(ClipperLib::Paths polys, double polys_z);
	
private:
	string file_path;
	double x_avg;
	double y_avg;
	double z_avg;
	vector<Hoop> hoops;
	vector<Hoop> original_hoops;
	vector<Hoop> merge_hoops;
	vector<vector<int>> layers;	
};






