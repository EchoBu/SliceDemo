#include "stdafx.h"
#include "MyModel.h"
#include "MyData.h"

#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <iomanip>

MyModel::MyModel() {}
MyModel::MyModel(string file_path) 
{
	this->file_path = file_path;
}
MyModel::~MyModel(){}

void  MyModel::initModel(string file_path)
{
	this->file_path = file_path;
	readInToArray();
	/*Simplify();
	Simplify();
	Simplify();*/
}

vector<string> MyModel::Split(const string& str, const string& delim) {

	vector<string> res;

	if ("" == str) return res;

	//先将要切割的字符串从string类型转换为char*类型  

	char * strs = new char[str.length() + 1]; //不要忘了  

	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while (p) {
		string s = p; //分割得到的字符串转换为string类型  
		res.push_back(s); //存入结果数组  
		p = strtok(NULL, d);
	}
	return res;
}

vector<Hoop> MyModel::get_hoops()
{
	return this->hoops;
}
vector<Hoop> MyModel::get_original_hoops()
{
	return this->original_hoops;
}

void MyModel::readInToArray()
{
	ifstream fin(file_path);
	string line_info, input_result;
	vector<string> vectorString;

	int count = 0;
	x_avg = 0;
	y_avg = 0;
	z_avg = 0;

	std::vector<Point> parray;
	std::vector<vector<Point>> pparray;// = std::vector<vector<Point>>(174);
	std::vector<vector<Point>> original_pparray;// = std::vector<vector<Point>>(174);
	int n = 0;
	if (fin) // 有该文件
	{
		while (getline(fin, line_info)) // line中不包括每行的换行符
		{
			
			stringstream input(line_info);

			vector<string> line_nums = Split(line_info, ",");
			double num1 = stod(line_nums[0], 0);
			double num2 = stod(line_nums[1], 0);
			double num3 = stod(line_nums[2], 0);		
			
			
			if (num1 == 0 && num2 == 0 && num3 == 0)
			{
				pparray.push_back(parray);
				original_pparray.push_back(parray);				
				parray.clear();
				continue;
			}
			else
			{
				x_avg += num1;
				y_avg += num2;
				z_avg += num3;
				parray.push_back(Point(num1, num2, num3));
				count++;
			}			
		}
		
		x_avg /= count;
		y_avg /= count;
		z_avg /= count;
		
		

		for (size_t j = 0; j < pparray.size(); j++)//归一化，便于显示
		{
			for (size_t i = 0; i < pparray[j].size(); i++)
			{
				pparray[j][i].x = (pparray[j][i].x - x_avg) / 30.0;
				pparray[j][i].y = (pparray[j][i].y - y_avg) / 30.0;
				pparray[j][i].z = (pparray[j][i].z - z_avg) / 30.0;
				original_pparray[j][i].x = original_pparray[j][i].x - x_avg;
				original_pparray[j][i].y = original_pparray[j][i].y - y_avg;
				original_pparray[j][i].z = original_pparray[j][i].z - z_avg;
			}
		}
		
		for (size_t j = 0; j < pparray.size(); j++)
		{
			//point_map[j] = pparray[j];
			this->hoops.push_back(pparray[j]);
			this->original_hoops.push_back(original_pparray[j]);
		}
		
	}
	
	else // 没有该文件
	{
		std::cout << "no such file" << endl;
	}

}

void MyModel::Simplify()
{
	std::cout << "before:" << this->hoops[31].size() << endl;
	//cout << "before:" << Angle(2,2,0,0,2,0) << endl;


	for (size_t i = 1; i < 2; i++)//实验一层
	{
		vector<int> to_earase;
		for (size_t j = 1; j < hoops[i].size() - 1; j++)
		{
			Point former_point = hoops[i][j - 1];
			Point current_point = hoops[i][j];
			Point latter_point = hoops[i][j + 1];

			Point vec1 = Point(current_point - former_point);
			Point vec2 = Point(latter_point - current_point);

			float x1 = former_point.x;
			float y1 = former_point.y;
			float x2 = current_point.x;
			float y2 = current_point.y;
			float x3 = latter_point.x;
			float y3 = latter_point.y;

			if (Angle(x1, y1, x2, y2, x3, y3) < 5)
			{
				to_earase.push_back(j);
				j++;
				//cout << j << endl;
			}

		}

		//Hoops[i].deletePoint(36);
		for (size_t j = 0; j < to_earase.size(); j++)
		{
			hoops[i].deletePoint(to_earase[j]);
			
			for (int q = 0; q < to_earase.size(); q++)
			{
				to_earase[q]--;
			}
		}
	}
	std::cout << "after:" << hoops[31].size() << endl;
}

double MyModel::Angle(float x1, float y1, float x2, float y2, float x3, float y3)

{

	double n1 = x2 - x1;
	double n2 = y2 - y1;
	double m1 = x3 - x2;
	double m2 = y3 - y2;


	double a[2] = { n1,n2 }, b[2] = { m1,m2 };
	double ab, a1, b1, cosr;
	ab = a[0] * b[0] + a[1] * b[1];
	a1 = sqrt(a[0] * a[0] + a[1] * a[1]);
	b1 = sqrt(b[0] * b[0] + b[1] * b[1]);
	cosr = ab / a1 / b1;
	//cout << acos(cosr) * 180 / PI << endl;
	return acos(cosr) * 180 / PI;
}

Point MyModel::get_cylindrical_coordinates(Point cartesian_point)
{
	Point output_point = Point(0,0,0);
	double x = cartesian_point.x;
	double y = cartesian_point.y;
	double z = cartesian_point.z;

	output_point.x = sqrt(x*x + y*y);

	
	if (x == 0)
	{
		if (y > 0)
		{
			output_point.y = PI / 2.0;
		}
		else if (y < 0)
		{
			output_point.y =  3.0 * PI / 2.0;
		}
		else
		{
			output_point.y = 0;
		}
	}
	else
	{
		if (x > 0 && y >= 0)//第一象限
		{
			output_point.y = atan(cartesian_point.y / cartesian_point.x * 1.0);
		}
		else if (x < 0 && y >= 0)//第二象限
		{
			output_point.y = atan(cartesian_point.y / cartesian_point.x * 1.0) + PI;
		}
		else if (x < 0 && y < 0)//第三象限
		{
			output_point.y = atan(cartesian_point.y / cartesian_point.x * 1.0) + PI;
		}
		else if (x > 0 && y < 0)//第四象限
		{
			output_point.y = atan(cartesian_point.y / cartesian_point.x * 1.0) + 2*PI;
		}
	}
	//cylindrical_point.y = atan(cartesian_point.y / cartesian_point.x * 1.0);
	output_point.z = z;
	return output_point;

}

int MyModel::get_closest_Point(Point p, int layer_index)
{
	//Hoop hoop = original_hoops[layer_index];

	Hoop hoop = merge_hoops[layer_index];
	float best_dis = std::numeric_limits<float>::infinity();
	float current_dis = 0;
	int best_i;

	Point cyl_point2 = get_cylindrical_coordinates(p);
	double theta2 = cyl_point2.y;

	for (int i = 0 ; i < hoop.size(); i++)
	{		
		current_dis = getDistanceXY(p, hoop[i]);
		if (current_dis < best_dis)
		{
			Point cyl_point1 = get_cylindrical_coordinates(hoop[i]);
			double theta1 = cyl_point1.y;
			
			if (theta1 >= theta2 )//TODO ,有没有可能它给的就是反的
			{
				best_dis = current_dis;
				best_i = i;
			}
			/*best_dis = current_dis;
			best_i = i;*/
		}			
	}
	//cout << "best_i =" << best_i << endl;
	return best_i;
}
	

double MyModel::getDistanceXY(Point a, Point b)
{
	double dis = sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
	return dis;
}
vector<int> MyModel::get_optimize_order(Point p, int layer_index) //得到跟当前点最近的起点的一圈
{
	int i_start = get_closest_Point(p, layer_index);	
	
	vector<int> index_order;
	//int index = 0;
	for (int i = i_start; i < merge_hoops[layer_index].size(); i++)
	{
		//points.push_back(original_hoops[layer_index][i]);
		index_order.push_back(i);
		
	}
	for (int i = 0; i < i_start; i++)
	{
		//points.push_back(original_hoops[layer_index][i]);
		index_order.push_back(i);
		
	}	
	
	return index_order;
}



void MyModel::output_gcode()
{
	vector<Hoop> hoops = this->original_hoops;
	

	string temp;
	ifstream infile;
	infile.open("start.txt");
	string name = "output_";
	string filename = name + "whole.nc";
	ofstream outfile;
	outfile.open(filename);
	if (!infile.is_open())
	{
		cout << "未成功打开文件" << endl;
	}
	while (getline(infile, temp))
	{
		outfile << temp << endl;
	}
	infile.close();

	init_layers();


	for (size_t i = 32; i < hoops.size(); i++) //i <hoops.size()
	{
		double theta;
		Point p;
		int start_i = 32;
		double x = hoops[start_i][0].x;
		double y = hoops[start_i][0].y;
		double z = hoops[start_i][0].z;
		p = Point(x, y, z);		
		Point cyl_point = get_cylindrical_coordinates(p);
		theta = cyl_point.y;			
		
		vector<int> index_order = get_optimize_order(p, i);
		int index_number = 0;
		
		if (hoops[i].size() < 50)
		{
			continue;
		}
		else
		{
			
			//跟本身的前后关系
			for (size_t j = 1; j <= hoops[i].size() - 1; j++)
			{
				if (i % 2 == 0)
				{
					double x = hoops[i][index_order[j]].x;
					double y = hoops[i][index_order[j]].y;
					double z = hoops[i][index_order[j]].z;
					Point cyl_point = get_cylindrical_coordinates(Point(x, y, z));
					double output_x = cyl_point.z;
					double output_y;
					if (cyl_point.y < theta)
					{
						output_y = (cyl_point.y - theta + 2 * PI) * 15;
					}
					else
					{
						output_y = (cyl_point.y - theta) * 15;
					}

					double output_z = cyl_point.x;
					Point p_current = Point(output_x, output_y, output_z);



					if (output_y >= 1 && output_y <= 100)
					{
						//cout << setiosflags(ios::fixed) << setprecision(4) << std::fixed << x << endl;
						outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << output_x;
						outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << output_y;
						outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << output_z << endl;
						//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
					}

				}
				else
				{
					double x = hoops[i][index_order[hoops[i].size() - j - 1]].x;
					double y = hoops[i][index_order[hoops[i].size() - j - 1]].y;
					double z = hoops[i][index_order[hoops[i].size() - j - 1]].z;
					Point cyl_point = get_cylindrical_coordinates(Point(x, y, z));
					double output_x = cyl_point.z;
					double output_y;
					if (cyl_point.y < theta)
					{
						output_y = (cyl_point.y - theta + 2 * PI) * 15;
					}
					else
					{
						output_y = (cyl_point.y - theta) * 15;
					}

					double output_z = cyl_point.x;
					Point p_current = Point(output_x, output_y, output_z);



					if (output_y >= 1 && output_y <= 100)//output_y >= 1 && output_y <= 100
					{
						//cout << setiosflags(ios::fixed) << setprecision(4) << std::fixed << x << endl;
						outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << output_x;
						outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << output_y;
						outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << output_z << endl;
						//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
					}


				}				
			}
			//index_number++;
		}
		
	}


	infile.open("end.txt");
	if (!infile.is_open())
	{
		cout << "未成功打开文件" << endl;
	}
	while (getline(infile, temp))
	{
		outfile << temp << endl;
	}
	infile.close();
	outfile.close();
}

void MyModel::output_gcode_whole()
{
	vector<Hoop> hoops = merge();


	string temp;
	ifstream infile;
	infile.open("start.txt");
	string name = "output_";
	string filename = name + "whole.nc";
	ofstream outfile;
	outfile.open(filename);
	if (!infile.is_open())
	{
		cout << "未成功打开文件" << endl;
	}
	while (getline(infile, temp))
	{
		outfile << temp << endl;
	}
	infile.close();

	//output_number_of_polygon();

	bool flag = true;

	for (size_t i = 0; i < hoops.size(); i++) //i <hoops.size()
	{

		double theta;
		Point p;
		int start_i = 5;
		double x = hoops[21][0].x;
		double y = hoops[21][0].y;
		double z = hoops[21][0].z;
		p = Point(x, y, z);
		Point cyl_point = get_cylindrical_coordinates(p);
		theta = cyl_point.y;


		int index_number = 0;

		if (hoops[i].size() < 50)//hoops[i].size() < 50
		{
			cout << "hoops[i].size() < 50: " << i << endl;
			flag = !flag;
			continue;
		}
		else
		{

			vector<int> index_order = get_optimize_order(p, i);
			//跟本身的前后关系
			for (size_t j = 1; j <= hoops[i].size() - 1; j++)
			{
				if (flag )
				{
					if (i % 2 == 0)
					{
						
						double x = hoops[i][index_order[j]].x;
						double y = hoops[i][index_order[j]].y;
						double z = hoops[i][index_order[j]].z;
						Point cyl_point = get_cylindrical_coordinates(Point(x, y, z));
						double output_x = cyl_point.z;
						double output_y;
						if (cyl_point.y < theta)
						{
							output_y = (cyl_point.y - theta + 2 * PI) * 15;
						}
						else
						{
							output_y = (cyl_point.y - theta) * 15;
						}

						double output_z = cyl_point.x;
						Point p_current = Point(output_x, output_y, output_z);



						if (output_y >= 1.7 && output_y <= 93)
						{
							//cout << setiosflags(ios::fixed) << setprecision(4) << std::fixed << x << endl;
							outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << output_x;
							outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << output_y;
							outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << output_z << endl;
							//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
						}

					}
					else
					{						

						double x = hoops[i][index_order[hoops[i].size() - j - 1]].x;
						double y = hoops[i][index_order[hoops[i].size() - j - 1]].y;
						double z = hoops[i][index_order[hoops[i].size() - j - 1]].z;
						Point cyl_point = get_cylindrical_coordinates(Point(x, y, z));
						double output_x = cyl_point.z;
						double output_y;
						if (cyl_point.y < theta)
						{
							output_y = (cyl_point.y - theta + 2 * PI) * 15;
						}
						else
						{
							output_y = (cyl_point.y - theta) * 15;
						}

						double output_z = cyl_point.x;
						Point p_current = Point(output_x, output_y, output_z);



						if (output_y >= 1.7 && output_y <= 93)
						{
							//cout << setiosflags(ios::fixed) << setprecision(4) << std::fixed << x << endl;
							outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << output_x;
							outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << output_y;
							outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << output_z << endl;
							//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
						}


					}
				}
				else
				{
					if (i % 2 == 1)
					{
						
						double x = hoops[i][index_order[j]].x;
						double y = hoops[i][index_order[j]].y;
						double z = hoops[i][index_order[j]].z;
						Point cyl_point = get_cylindrical_coordinates(Point(x, y, z));
						double output_x = cyl_point.z;
						double output_y;
						if (cyl_point.y < theta)
						{
							output_y = (cyl_point.y - theta + 2 * PI) * 15;
						}
						else
						{
							output_y = (cyl_point.y - theta) * 15;
						}

						double output_z = cyl_point.x;
						Point p_current = Point(output_x, output_y, output_z);



						if (output_y >= 1.7 && output_y <= 93)
						{
							//cout << setiosflags(ios::fixed) << setprecision(4) << std::fixed << x << endl;
							outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << output_x;
							outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << output_y;
							outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << output_z << endl;
							//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
						}

					}
					else
					{
						
						double x = hoops[i][index_order[hoops[i].size() - j - 1]].x;
						double y = hoops[i][index_order[hoops[i].size() - j - 1]].y;
						double z = hoops[i][index_order[hoops[i].size() - j - 1]].z;
						Point cyl_point = get_cylindrical_coordinates(Point(x, y, z));
						double output_x = cyl_point.z;
						double output_y;
						if (cyl_point.y < theta)
						{
							output_y = (cyl_point.y - theta + 2 * PI) * 15;
						}
						else
						{
							output_y = (cyl_point.y - theta) * 15;
						}

						double output_z = cyl_point.x;
						Point p_current = Point(output_x, output_y, output_z);


						if (output_y >= 1.7 && output_y <= 93)
						{
							//cout << setiosflags(ios::fixed) << setprecision(4) << std::fixed << x << endl;
							outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << output_x;
							outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << output_y;
							outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << output_z << endl;
							//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
						}


					}
				}

			}
			//index_number++;
		}
		
	}


	infile.open("end.txt");
	if (!infile.is_open())
	{
		cout << "未成功打开文件" << endl;
	}
	while (getline(infile, temp))
	{
		outfile << temp << endl;
	}
	infile.close();
	outfile.close();
}

void MyModel::init_layers()
{
	double z_formor = original_hoops[0][0].z;
	int index = 0;

	vector<int> current_layer;

	for (int i = 0 ; i < original_hoops.size(); i++)
	{
		int num1 = 0;
		double z = original_hoops[i][0].z;
		if (z != z_formor)
		{
			
			
			layers.push_back(current_layer);
			
			
			index++;
			z_formor = z;
			current_layer.clear();
			current_layer.push_back(i);
		}
		else 
		{
			//std::cout << " " << i;
			num1++;
			current_layer.push_back(i);
		}
	}
	
}

void MyModel::output_layers()
{
	if (layers.empty())
	{
		cout << "'layers'  hasn't been initialized" << endl;
		return;
	}
	for (size_t i = 0; i < layers.size(); i++)
	{
		std::cout << "layer:" << layers.size()-1 << " z=" << original_hoops[layers[i][0]][0].z <<"  Hoop index:" ;

		for (int j = 0; j < layers[i].size(); j++)
		{
			std::cout << " " << layers[i][j];
		}
		std::cout << std::endl;
	}
}

vector<Hoop> MyModel::clipper_operation(int layer_index , double R)
{
	vector<Hoop> new_hoops;
	Clipper clipper;

	for (size_t i = 0; i < layers[layer_index].size() ; i++)
	{
		Path p1 = hoop2path(original_hoops[layers[layer_index][i]]);
		clipper.AddPath(p1, ptSubject, true);
	}

	Point2D center = Point2D(0, 0);
	Path circle = generate_circle(R, center);
	clipper.AddPath(circle, ptClip, true);	

	Paths out_polys;
	clipper.Execute(ctUnion, out_polys, pftEvenOdd, pftEvenOdd);
	
	new_hoops = paths2hoops(out_polys, 0.5 * layer_index + 0.5-15.1794);
	
	return new_hoops;
}

ClipperLib::Path MyModel::hoop2path(Hoop hoop)
{
	ClipperLib::Path path;
	for (int i = 0; i < hoop.size(); i++)
	{
		double x = hoop[i].x * 1000;
		double y = hoop[i].y * 1000;
		ClipperLib::IntPoint p = ClipperLib::IntPoint(x, y);
		path.push_back(p);
	}
	return path;
}

ClipperLib::Path MyModel::generate_circle(double R, Point2D center)
{
	ClipperLib::Path path;
	for (int i = 0; i < 72; i++)
	{
		double x = (R * cos(i * (2 * PI) / 72.0) + center.x) * 1000;
		double y = (R * sin(i * (2 * PI) / 72.0) + center.y) * 1000;
		ClipperLib::IntPoint p = ClipperLib::IntPoint(x, y);
		path.push_back(p);
	}
	return path;
}

vector<Hoop> MyModel::paths2hoops(ClipperLib::Paths polys, double polys_z)
{
	vector<Hoop> new_hoops;
	for (size_t i = 0; i < polys.size(); i++)
	{
		Hoop hoop;
		for (size_t j = 0; j < polys[i].size(); j++)
		{
			double x = polys[i][j].X / 1000.0;
			double y = polys[i][j].Y / 1000.0;
			double z = polys_z;
			Point p = Point(x, y, z);
			hoop.addPoint(p);
		}
		new_hoops.push_back(hoop);
	}
	return new_hoops;
}


vector<Hoop> MyModel::merge()
{
	init_layers();
	vector<Hoop> new_hoops;// = clipper_operation(2);
	new_hoops.push_back(clipper_operation(0, 8.3)[0]);
	new_hoops.push_back(clipper_operation(1, 8.3)[0]);
	new_hoops.push_back(clipper_operation(2, 8.3)[0]);
	new_hoops.push_back(clipper_operation(3, 8.3)[0]);
	new_hoops.push_back(clipper_operation(4, 8.3)[0]);
	for (size_t i = 16; i < original_hoops.size(); i++)
	{
		new_hoops.push_back(original_hoops[i]);
	}
	merge_hoops = new_hoops;
	return new_hoops;
}