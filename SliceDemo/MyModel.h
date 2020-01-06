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
	//两点之间的夹角
	double Angle(float x1, float y1, float x2, float y2, float x3, float y3);

	//从文件中读取已经切片的数据
	void readInToArray();

	//简化
	void Simplify();

	//得到两点间xoy平面上的距离
	double getDistanceXY(Point a, Point b);

	//得到在第layer_index层，距离点p最近的点的索引值
	int get_closest_Point(Point p, int layer_index);

	//得到第layer_index层最优的顶点顺序
	vector<int> get_optimize_order(Point p, int layer_index);

	//得到点cartesian_point的柱面坐标
	Point get_cylindrical_coordinates(Point cartesian_point);

	//得到每层所包含的hoop的索引值，赋值给“vector<vector<int>> layers”
	void init_layers();

	//对第layer_index层，用半径为R的圆进行裁剪
	vector<Hoop> clipper_operation(int layer_index, double R);

	//生成圆
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






