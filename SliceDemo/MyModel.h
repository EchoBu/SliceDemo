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

	void output_gcode();	//32层及后面
	void output_gcode_whole();	  //全部的层融合在一起输出Gcode到nc文件
	vector<Hoop> output_gcode_newway();
	vector<Hoop> get_gcode_newway();


	void output_layers(); //输出每层包含原始的哪些层
	void getFinalResult();	
	void getFinalResult_newway();
	double get_angle(Point p1, Point p2, Point p3);
	
	vector<Hoop> merge();
	vector<Hoop> merge_newway();
	vector<Hoop> getGcode();
	Hoop getGcode_one(int layer_index);
	
	Hoop stepOne(int layer_index);//确保y是一直增加的
	Hoop stepTwo(int layer_index);//在one的结果基础上外面罩上一层
	Hoop stepThree(int layer_index);
	Hoop stepFour(int layer_index);
	vector<vector<int>> dosomething(vector<int> wa);//
	int getMaxZ(Hoop hoop);

	//对第layer_index层，用半径为R的圆进行裁剪
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

	

	//生成圆
	ClipperLib::Path generate_circle(double R, Point2D center);

	//
	ClipperLib::Path hoop2path(Hoop hoop);	
	
	//
	vector<Hoop> paths2hoops(ClipperLib::Paths polys, double polys_z);

	
	Point getNormalsLeft(Hoop hoop, int index, int layer_index);
	Point getNormalsRight(Hoop hoop, int index, int layer_index);

	//得到这一层平均的半径
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

	vector<Hoop> Gcode_hoops; //后续都是对这个操作
	vector<Hoop> Gcode_hoops_newway;

	vector<vector<int>> layers;	
	Hoop formor_hoop_step4;
};






