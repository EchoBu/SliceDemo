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
	void initModel();	
	vector<Hoop> get_hoops();
	vector<Hoop> get_original_hoops();
	Point get_cylindrical_coordinates(Point cartesian_point);
	int get_closest_Point(Point p, int layer_index);
	vector<int> get_optimize_order(Point p, int layer_index);
	void output_gcode();
	void output_gcode_2();
	void output_gcode_3();
	void output_start();
	void output_end();
	void output_number_of_polygon();
	vector<Hoop> clipper_operation(int layer_index, double R);
	vector<Hoop> merge();

protected:
	double Angle(float x1, float y1, float x2, float y2, float x3, float y3);
	void readInToArray();
	void Simplify();
	double getDistanceXY(Point a, Point b);
	ClipperLib::Path hoop2path(Hoop hoop);	
	ClipperLib::Path generate_circle(double R, Point2D center);
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

	/*string temp;
	std::ifstream infile;
	std::ofstream outfile;*/
};






