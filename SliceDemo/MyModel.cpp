#include "stdafx.h"
#include "MyModel.h"
#include "MyData.h"

#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

MyModel::MyModel(){ }
MyModel::MyModel(string file_path) 
{
	this->file_path = file_path;
	
}
MyModel::~MyModel(){}

void  MyModel::initModel(string file_path)
{
	this->file_path = file_path;
	readInToArray();
	
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

	
	if (fabs(x - 0)<eps) // x==0
	{
		if (y >  0) //y > 0 a > b+EPS
		{
			output_point.y = PI / 2.0;
		}
		else if (y < 0) // y < 0
		{
			output_point.y =  1.5 * PI ;
		}
		else
		{
			output_point.y = 0;
		}
	}
	else
	{
		if (x > 0 && (y>0 ))//第一象限 //x > eps && (y>eps )
		{
			output_point.y = atan(y / x * 1.0);
		}
		else if (x < 0 && (y > 0 ))//第二象限
		{
			output_point.y = atan(y / x * 1.0) + PI;
		}
		else if (x < 0 && y < 0)//第三象限
		{
			output_point.y = atan(y / x * 1.0) + PI;
		}
		else if (x > 0 && y < 0)//第四象限
		{
			output_point.y = atan(y / x * 1.0) + 2*PI;
		}
		else if (x > 0 && fabs(y-0) < eps) //x轴正半轴
		{
			output_point.y = 0;
		}
		else if (x < 0 && fabs(y - 0) < eps) //x轴fu半轴
		{
			output_point.y = PI;
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
	int best_i = 0;

	Point cyl_point2 = get_cylindrical_coordinates(p);
	double theta2 = cyl_point2.y;

	for (int i = 0 ; i < hoop.size(); i++)
	{		
		current_dis = getDistanceXY(p, hoop[i]);
		if ((current_dis +eps) < best_dis)
		{
			Point cyl_point1 = get_cylindrical_coordinates(hoop[i]);
			double theta1 = cyl_point1.y;
			
			if (theta1 >= (theta2 +eps) )//TODO ,有没有可能它给的就是反的
			{
				best_dis = current_dis;
				best_i = i;
			}
			/*best_dis = current_dis;
			best_i = i;*/
		}			
	}
	
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
		std::cout << "未成功打开文件" << endl;
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
		std::cout << "未成功打开文件" << endl;
	}
	while (getline(infile, temp))
	{
		outfile << temp << endl;
	}
	infile.close();
	outfile.close();
}
vector<Hoop> MyModel::get_gcode_newway()
{
	init_layers();
	output_gcode_newway();
	return Gcode_hoops_newway;
}

vector<Hoop> MyModel::output_gcode_newway()
{
	Gcode_hoops.clear();
	vector<Hoop> result_hoops;
	vector<Hoop> hoops = merge_newway();//这里应该得到的每层，但是可能不止一个hoop，让他们个字生成，然后取高的哪个
	for (size_t i = 0; i < hoops.size(); i++) //i <hoops.size()
	{
		Hoop gcode_hoop = Hoop();
		double theta;
		Point p = hoops[41][20];//[41][20]
								//Point p = Point(0, 0, 0);

		float R = 180.0f / PI;// 16.2

		Point cyl_point = get_cylindrical_coordinates(p);
		theta = cyl_point.y;	

		vector<int> index_order = get_optimize_order(p, i);
		if (i % 2 == 0)
		{
			for (size_t j = 0; j < hoops[i].size(); j++)
			{


				double x = hoops[i][index_order[j]].x;
				double y = hoops[i][index_order[j]].y;
				double z = hoops[i][index_order[j]].z;


				Point cyl_point = get_cylindrical_coordinates(Point(x, y, z));
				double output_x = cyl_point.z;
				double output_y;

				int bound1, bound2;
				bound1 = index_order[0] + 20;
				bound2 = index_order[0] - 20;

				if ((cyl_point.y > theta + eps) && (index_order[j] > index_order[0]))
				{
					output_y = (cyl_point.y - theta) * R;

					double output_z = cyl_point.x;
					//Point p_current = Point(output_x, output_y, output_z);
					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

				}
				else if ((theta > cyl_point.y + eps) && (index_order[j] > bound1))
				{
					//theta > cyl_point.y + eps;
					output_y = (cyl_point.y - theta + 2.0 * PI) * R;

					double output_z = cyl_point.x;

					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

				}
				else if ((cyl_point.y > theta + eps) && (index_order[j] < bound2))
				{
					output_y = (cyl_point.y - theta) * R;

					double output_z = cyl_point.x;

					//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

				}
				//(theta >= cyl_point.y + eps)&&(index_order[j] < index_order[0])
				else if ((theta > cyl_point.y + eps) && (index_order[j] < index_order[0])) //cyl_point.y < theta //theta > cyl_point.y + eps
				{
					//theta > cyl_point.y + eps;
					output_y = (cyl_point.y - theta + 2.0 * PI) * R;
					double output_z = cyl_point.x;
					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

				}
			}

		}
		else //i % 2 =1;
		{
			for (size_t j = 0; j < hoops[i].size(); j++)
			{


				double x = hoops[i][index_order[hoops[i].size() - j - 1]].x;
				double y = hoops[i][index_order[hoops[i].size() - j - 1]].y;
				double z = hoops[i][index_order[hoops[i].size() - j - 1]].z;
				Point cyl_point = get_cylindrical_coordinates(Point(x, y, z));
				double output_x = cyl_point.z;
				double output_y;
				//(cyl_point.y > theta + eps) && (index_order[j] > index_order[0])
				if ((cyl_point.y > theta + eps) && (index_order[hoops[i].size() - j - 1] > index_order[hoops[i].size() - 1]))
				{
					output_y = (cyl_point.y - theta) * R;
					double output_z = cyl_point.x;
					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

				}
				else if ((theta > cyl_point.y + eps) && (index_order[hoops[i].size() - j - 1] > index_order[hoops[i].size() - 1] + 20))
				{
					//theta > cyl_point.y + eps;
					output_y = (cyl_point.y - theta + 2.0 * PI) * R;
					double output_z = cyl_point.x;
					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

				}
				//(cyl_point.y > theta + eps) && (index_order[hoops[i].size() - j - 1]> index_order[0])
				else if ((cyl_point.y > theta + eps) && (index_order[hoops[i].size() - j - 1] < index_order[hoops[i].size() - 1] - 20))
				{
					output_y = (cyl_point.y - theta) * R;
					double output_z = cyl_point.x;
					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

				}
				else if ((theta > cyl_point.y + eps) && (index_order[hoops[i].size() - j - 1] < index_order[hoops[i].size() - 1]))
				{
					output_y = (cyl_point.y - theta + 2.0 * PI) * R;
					double output_z = cyl_point.x;
					gcode_hoop.addPoint(Point(output_x, output_y, output_z));
				}
			}
		}		
		
		Gcode_hoops.push_back(gcode_hoop);
	}// end for i

	std::cout << "Gcode_hoops.size()" << Gcode_hoops.size() << std::endl;
	for (int q = 0; q < Gcode_hoops.size(); q++)
	{
		std::cout << "q=" << q << std::endl;
		//Hoop hoop = stepOne(t);
		result_hoops.push_back(stepOne(q));
	}
	//Hoop hoop = stepOne(0);
	//result_hoops.push_back(hoop);
	return result_hoops;
}

void MyModel::output_gcode_whole()//修改这个函数，
{
	vector<Hoop> hoops = merge();


	string temp;
	ifstream infile;
	infile.open("start.txt");
	string name = "output_";
	string filename = name + "whole_360.nc";
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

	for (size_t i = 0; i <hoops.size(); i++) //i <hoops.size()
	{
		Hoop gcode_hoop = Hoop();
		double theta;
		Point p = hoops[41][20];//[41][20]
								//Point p = Point(0, 0, 0);

		float R = 180.0f / PI;// 16.2

		Point cyl_point = get_cylindrical_coordinates(p);
		theta = cyl_point.y;

		//theta = 0;

		int index_number = 0;

		int test_index_i = 242;

		vector<int> index_order = get_optimize_order(p, i);
		if (i == test_index_i) //测试
		{
			cout << "i=" << i << endl;
			cout << "theta:" << theta << endl;
			cout << "z:" << hoops[test_index_i][index_order[0]].z << endl;
			std::cout << "index_order[0] " << index_order[0] << std::endl;//
			std::cout << "index_order[hoops[i].size -1] " << index_order[hoops[i].size() - 1] << std::endl;//
		}

		if (i % 2 == 0)//IF 
		{
			Point cyl_point0 = get_cylindrical_coordinates(hoops[i][index_order[0]]);
			outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << cyl_point0.z;
			outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << 0.0;
			outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << cyl_point0.x << endl;

			//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
			gcode_hoop.addPoint(Point(cyl_point0.z, 0.0, cyl_point0.x));
		}
		else
		{
			Point cyl_point1 = get_cylindrical_coordinates(hoops[i][index_order[0]]);//hoops[i][index_order[index_order.size()-1]
			outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << cyl_point1.z;
			outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << 359.9;
			outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << cyl_point1.x << endl;

			//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
			gcode_hoop.addPoint(Point(cyl_point1.z, 359.9, cyl_point1.x));
		}

		for (size_t j = 1; j <= hoops[i].size() - 1; j++)
		{

			if (i % 2 == 0)
			{

				double x = hoops[i][index_order[j]].x;
				double y = hoops[i][index_order[j]].y;
				double z = hoops[i][index_order[j]].z;
				/*if (j < 3 )
				{
				std::cout << "point"<<i<<" :" << x << " " << y << " " << z << std::endl;
				}*/


				Point cyl_point = get_cylindrical_coordinates(Point(x, y, z));
				double output_x = cyl_point.z;
				double output_y;

				int bound1, bound2;
				bound1 = index_order[0] + 20;
				bound2 = index_order[0] - 20;



				//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
				//gcode_hoop.addPoint(Point(output_x, output_y, output_z));


				if ((cyl_point.y > theta + eps) && (index_order[j] > index_order[0]))
				{
					output_y = (cyl_point.y - theta) * R;

					if (i == test_index_i)
					{
						std::cout << "case 1:" << " " << output_y << " " << cyl_point.y << " " << index_order[j] << std::endl;;
					}
					double output_z = cyl_point.x;
					Point p_current = Point(output_x, output_y, output_z);
					outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << output_x;
					outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << output_y;
					outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << output_z << endl;

					//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;

					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

				}
				else if ((theta > cyl_point.y + eps) && (index_order[j] > bound1))
				{
					//theta > cyl_point.y + eps;
					output_y = (cyl_point.y - theta + 2.0 * PI) * R;
					if (i == test_index_i)
					{
						std::cout << "case 2 :" << " " << output_y << " " << cyl_point.y << " " << index_order[j] << std::endl;;
					}
					double output_z = cyl_point.x;
					Point p_current = Point(output_x, output_y, output_z);
					outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << output_x;
					outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << output_y;
					outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << output_z << endl;
					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

					//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;

				}
				else if ((cyl_point.y >theta + eps) && (index_order[j] < bound2))
				{
					output_y = (cyl_point.y - theta) * R;
					if (i == test_index_i)
					{
						std::cout << "case 3 :" << " " << output_y << " " << cyl_point.y << " " << index_order[j] << std::endl;;
					}
					double output_z = cyl_point.x;
					Point p_current = Point(output_x, output_y, output_z);
					outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << output_x;
					outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << output_y;
					outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << output_z << endl;

					//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

				}
				//(theta >= cyl_point.y + eps)&&(index_order[j] < index_order[0])
				else if ((theta > cyl_point.y + eps) && (index_order[j] < index_order[0])) //cyl_point.y < theta //theta > cyl_point.y + eps
				{
					//theta > cyl_point.y + eps;
					output_y = (cyl_point.y - theta + 2.0 * PI) * R;
					if (i == test_index_i)
					{
						std::cout << "case 4 :" << " " << output_y << " " << cyl_point.y << " " << index_order[j] << std::endl;;
					}
					double output_z = cyl_point.x;
					Point p_current = Point(output_x, output_y, output_z);
					outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << output_x;
					outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << output_y;
					outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << output_z << endl;

					//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

				}



			}
			else //i % 2 =1 
			{

				double x = hoops[i][index_order[hoops[i].size() - j - 1]].x;
				double y = hoops[i][index_order[hoops[i].size() - j - 1]].y;
				double z = hoops[i][index_order[hoops[i].size() - j - 1]].z;
				Point cyl_point = get_cylindrical_coordinates(Point(x, y, z));
				double output_x = cyl_point.z;
				double output_y;
				//(cyl_point.y > theta + eps) && (index_order[j] > index_order[0])
				if ((cyl_point.y > theta + eps) && (index_order[hoops[i].size() - j - 1] > index_order[hoops[i].size() - 1])
					) //theta > cyl_point.y + eps
					  //
				{
					if (i == 67)
					{
						/*if (index_order[hoops[i].size() - j - 1] < hoops[i].size() - 10)
						{
						output_y = (cyl_point.y - theta) * R;
						if (i == test_index_i)
						{
						std::cout << "case 1 :" << " " << output_y << " " << cyl_point.y << " " << index_order[hoops[i].size() - j - 1] << std::endl;;
						}
						double output_z = cyl_point.x;
						Point p_current = Point(output_x, output_y, output_z);
						outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << output_x;
						outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << output_y;
						outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << output_z << endl;

						//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
						gcode_hoop.addPoint(Point(output_x, output_y, output_z));

						}*/
						output_y = (cyl_point.y - theta) * R;
						if (i == test_index_i)
						{
							std::cout << "case 1 :" << " " << output_y << " " << cyl_point.y << " " << index_order[hoops[i].size() - j - 1] << std::endl;;
						}
						double output_z = cyl_point.x;
						Point p_current = Point(output_x, output_y, output_z);
						outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << output_x;
						outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << output_y;
						outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << output_z << endl;

						/*if (gcode_hoop.isOK(output_y))//这是第一个佛像需要
						{
						//cout << "isOK" << endl;
						gcode_hoop.addPoint(p_current);
						}*/
						gcode_hoop.addPoint(p_current);
					}
					else
					{
						output_y = (cyl_point.y - theta) * R;
						if (i == test_index_i)
						{
							std::cout << "case 1 :" << " " << output_y << " " << cyl_point.y << " " << index_order[hoops[i].size() - j - 1] << std::endl;;
						}
						double output_z = cyl_point.x;
						Point p_current = Point(output_x, output_y, output_z);
						outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << output_x;
						outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << output_y;
						outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << output_z << endl;

						//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
						gcode_hoop.addPoint(Point(output_x, output_y, output_z));

					}


				}
				else if ((theta > cyl_point.y + eps) && (index_order[hoops[i].size() - j - 1] > index_order[hoops[i].size() - 1] + 20))
				{
					//theta > cyl_point.y + eps;
					output_y = (cyl_point.y - theta + 2.0 * PI) * R;
					if (i == test_index_i)
					{
						std::cout << "case 2 :" << " " << output_y << " " << cyl_point.y << " " << index_order[hoops[i].size() - j - 1] << std::endl;;
					}
					double output_z = cyl_point.x;
					Point p_current = Point(output_x, output_y, output_z);
					outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << output_x;
					outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << output_y;
					outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << output_z << endl;

					//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

				}
				//(cyl_point.y > theta + eps) && (index_order[hoops[i].size() - j - 1]> index_order[0])
				else if ((cyl_point.y >theta + eps) && (index_order[hoops[i].size() - j - 1] < index_order[hoops[i].size() - 1] - 20))
				{
					output_y = (cyl_point.y - theta) * R;
					if (i == test_index_i)
					{
						std::cout << "case 3 :" << " " << output_y << " " << cyl_point.y << " " << index_order[hoops[i].size() - j - 1] << std::endl;;
					}
					double output_z = cyl_point.x;
					Point p_current = Point(output_x, output_y, output_z);
					outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << output_x;
					outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << output_y;
					outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << output_z << endl;

					//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

				}
				else if ((theta > cyl_point.y + eps) && (index_order[hoops[i].size() - j - 1] < index_order[hoops[i].size() - 1]))
				{
					output_y = (cyl_point.y - theta + 2.0 * PI) * R;
					if (i == test_index_i)
					{
						std::cout << "case 4 :" << " " << output_y << " " << cyl_point.y << " " << index_order[hoops[i].size() - j - 1] << std::endl;
					}
					double output_z = cyl_point.x;
					Point p_current = Point(output_x, output_y, output_z);
					outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << output_x;
					outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << output_y;
					outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << output_z << endl;

					//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
					gcode_hoop.addPoint(Point(output_x, output_y, output_z));

				}

			}

		}

		if (i % 2 == 0)
		{
			Point cyl_point1 = get_cylindrical_coordinates(hoops[i][index_order[0]]);//index_order[index_order.size() - 1]
			outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << cyl_point1.z;
			outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << 359.9;
			outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << cyl_point1.x << endl;

			//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
			gcode_hoop.addPoint(Point(cyl_point1.z, 359.9, cyl_point1.x));
		}
		else
		{

			Point cyl_point0 = get_cylindrical_coordinates(hoops[i][index_order[0]]);
			outfile << "X" << setiosflags(ios::fixed) << setprecision(5) << cyl_point0.z;
			outfile << " Y" << setiosflags(ios::fixed) << setprecision(5) << 0.0;
			outfile << " Z" << setiosflags(ios::fixed) << setprecision(5) << cyl_point0.x << endl;

			//outfile << " X" << output_x << " Y" << output_y << " Z" << output_z << endl;
			gcode_hoop.addPoint(Point(cyl_point0.z, 0.0, cyl_point0.x));
		}

		Gcode_hoops.push_back(gcode_hoop);
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

void MyModel::getFinalResult_newway()
{
	output_gcode_newway();
	vector<Hoop> hoops;
	string temp;
	ifstream infile;
	infile.open("start.txt");
	string name = "output_";
	string filename = name + "newway.nc";
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
	for (int i = 0; i < Gcode_hoops.size(); i++) //Gcode_hoops.size()
	{
		hoops.push_back(stepOne(i));
	}
	//hoops = Gcode_hoops;
	for (int i = 0; i < hoops.size(); i++)
	{
		if (i % 2 == 0)
		{
			outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][0].x;
			outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << 0.0;
			outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][hoops[i].size() - 1].z << endl;
		}
		//int t = hoops.size() - i - 1;
		for (int j = 0; j < hoops[i].size(); j++)
		{
			outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][j].x;
			outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][j].y / 180.0*PI * 10.2;
			outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][j].z << endl;

		}
		if (i % 2 == 1)
		{
			outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][hoops[i].size() - 1].x;
			outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << 0.0;
			outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][0].z << endl;
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

void MyModel::getFinalResult()
{
	output_gcode_whole();
	vector<Hoop> hoops;
	
	
	string temp;
	ifstream infile;
	infile.open("start2.txt");
	string name = "output_";
	string filename = name + "hediao2.nc";
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
	
	//出step4
	/*
	for (int i = 0; i < 112; i++) //Gcode_hoops.size()
	{
		hoops.push_back(stepThree(Gcode_hoops.size() - i - 1));
	}
	for (int i = 112; i < 119; i++) //Gcode_hoops.size()
	{
		hoops.push_back(stepFour(Gcode_hoops.size()-i-1));
	}
	for (int i = 119; i < Gcode_hoops.size(); i++) //Gcode_hoops.size()
	{
		hoops.push_back(stepThree(Gcode_hoops.size() - i - 1));
	}

	for (int i = 0; i < hoops.size(); i++)
	{
		if (i % 2 == 1)
		{
			outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << i*0.15;//hoops[i][0].x
			outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << 359.9;
			outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][hoops[i].size() - 1].z << endl;
		}
		//int t = hoops.size() - i - 1;
		for (int j = 0; j < hoops[i].size(); j++)
		{
			outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << i*0.15;//hoops[i][j].x
			outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << 359.9-hoops[i][j].y ;/// /180.0*PI * 16.2
			outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][j].z << endl;

		}
		if (i % 2 == 0)
		{
			outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << i*0.15;//hoops[i][hoops[i].size() - 1].x
			outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << 359.9;
			outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][0].z << endl;
		}
	}*/

	
	//出step3
/*
	for (int i = 0; i < Gcode_hoops.size()-3; i++) //Gcode_hoops.size()
	{
		hoops.push_back(stepFour(i));
	}
*/
	
	for (int i = 4; i < 124; i++) //Gcode_hoops.size()
	{
		hoops.push_back(stepThree(i));
	}
	for (int i = 124; i < 147; i++) //Gcode_hoops.size()
	{
		hoops.push_back(stepFour(i));
	}
	for (int i = 147; i < Gcode_hoops.size()-4; i++) //Gcode_hoops.size()
	{
		hoops.push_back(stepThree(i));
	}

	for (int i = 0; i < hoops.size(); i++)
	{
		if (i %2 == 0)
		{
			outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][0].x ;
			outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << 0.0;
			outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][hoops[i].size() - 1].z  << endl;
		}
		//int t = hoops.size() - i - 1;
		for (int j = 0; j < hoops[i].size(); j++)
		{
			outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][j].x ;
			outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][j].y;// / 180.0*PI * 10.2;
			outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][j].z  << endl;

		}
		if (i % 2 == 1)
		{
			outfile << "X" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][hoops[i].size()-1].x ;
			outfile << " Y" << setiosflags(ios::fixed) << setprecision(4) << 0.0 ;
			outfile << " Z" << setiosflags(ios::fixed) << setprecision(4) << hoops[i][0].z  << endl;
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
		std::cout << "layer:" << i << " z=" << original_hoops[layers[i][0]][0].z <<"  Hoop index:" ;

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
	
	new_hoops = paths2hoops(out_polys, original_hoops[layers[layer_index][0]][0].z); //0.2 * layer_index + 0.5 - 15.1794
	
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
	for (int i = 0; i <= 360; i++)
	{
		double x = (R * cos(i * (2 * PI) / 360.0) + center.x) * 1000;
		double y = (R * sin(i * (2 * PI) / 360.0) + center.y) * 1000;
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

vector<Hoop> MyModel::merge_newway()
{
	init_layers();	
	vector<Hoop> new_hoops;// = clipper_operation(2);	
	Hoop one_hoop;
	for (size_t i = 0; i < layers.size(); i++)//layers.size()
	{
		/*one_hoop.clear();
		for (size_t j = 0; j < layers[i].size(); j++)
		{
			for (size_t t = 0; t < original_hoops[layers[i][j]].size(); t++)
			{
				one_hoop.addPoint(original_hoops[layers[i][j]][t]);
			}
			
		}
		new_hoops.push_back(one_hoop);*/

		if (layers[i].size() > 1) {

/*

			if (i < 16) //最前面几层融合，用一个圆求并集
			{
			Hoop hoop_test = clipper_operation(i, 6.5 + i*0.15)[0];
			new_hoops.push_back(hoop_test);
			}
			else //后面的取最大的一圈
			{

			new_hoops.push_back(original_hoops[layers[i][getMaxHoopIndexOfLayer(i)]]);
			}*/
			new_hoops.push_back(original_hoops[layers[i][getMaxHoopIndexOfLayer(i)]]);
		}
		else
		{
			new_hoops.push_back(original_hoops[layers[i][0]]);
		}
	}
	

	merge_hoops = new_hoops;
	return new_hoops;
}

vector<Hoop> MyModel::merge()
{
	init_layers();
	vector<Hoop> new_hoops;// = clipper_operation(2);	

	for (size_t i = 0 ;i < layers.size(); i++)//layers.size()
	{
		if (layers[i].size() > 1)		{


			/*if (i < 16) //最前面几层融合，用一个圆求并集
			{				
				Hoop hoop_test = clipper_operation(i, 6.5 + i*0.15)[0];	
				new_hoops.push_back(hoop_test);
			}
			else //后面的取最大的一圈
			{
				
				new_hoops.push_back(original_hoops[layers[i][getMaxHoopIndexOfLayer(i)]]);
			}*/
			new_hoops.push_back(original_hoops[layers[i][getMaxHoopIndexOfLayer(i)]]);
		}
		else
		{	
			new_hoops.push_back(original_hoops[layers[i][0]]);
		}
	}

	merge_hoops = new_hoops;
	return new_hoops;
}
int MyModel::getMaxHoopIndexOfLayer(int layer_index) //得到layer_index层中最大的一圈
{
	if (original_hoops[layers[layer_index][0]].size() > original_hoops[layers[layer_index][1]].size())
	{
		return 0;
	}
	else
	{
		return 1;
	}
	
}


	
double MyModel::getAvgHeight(Hoop hoop) //得到这一层的平均z，即转换前的sqrt(x^2 + y^2)
{
	double r_avg = 0;
	for (int i = 0; i < hoop.size(); i++)
	{
		r_avg = r_avg + sqrt(hoop[i].x*hoop[i].x + hoop[i].y * hoop[i].y);
	}
	std::cout << "r_avg=" << r_avg / hoop.size() << std::endl;
	return r_avg / hoop.size();
	
}

vector<Hoop> MyModel::getGcode()
{
	return Gcode_hoops;
}

Hoop MyModel::getGcode_one(int layer_index)
{
	return Gcode_hoops[layer_index];
}

Hoop MyModel::stepOne(int layer_index)
{
	
	Hoop hoop = Gcode_hoops[layer_index];
	Hoop new_hoop;

	Lines lines = Lines(hoop);
	Hoop test_hoop;

	vector<double> y_order = getYOrder(hoop);
	/*if (layer_index == 0)//输出测试
	{
		for (size_t i = 0; i < y_order.size(); i++)
		{
			cout << "y_order[i]" << y_order[i] << endl;
		}
	}*/

	std::cout << "layer_index = " << layer_index << std::endl;
	std::cout << "x = " << hoop[0].x << std::endl; //y_min + dis * i

	if (layer_index % 2 == 0) //双数，正向的
	{
		double y_min = hoop[0].y;
		double y_max = hoop[hoop.size() - 1].y;
		double line_length = y_max - y_min;
		double dis = (y_max - y_min) / 600.0;
		

		for (size_t i = 0; i < y_order.size(); i++) //size_t i = 0; i < 600; i++
		{
			test_hoop.clear();
			vector<int> indexes = lines.getIntersectionLineSegmentIndex(y_order[i], 1);//

			if (indexes.size() == 0)
			{
				std::cout << "layer_index = " << layer_index << std::endl;
				std::cout << "y = " << y_order[i] << std::endl; //y_min + dis * i
			}
																				 
			for (int j = 0; j < indexes.size(); j++)
			{
				Point test_point = lines[indexes[j]].getIntersectionPoint(y_order[i]); //
				test_hoop.addPoint(test_point);
			}
			int t = getMaxZ(test_hoop);
			if (t != -1)
			{
				Point p = Point(test_hoop[t].x, test_hoop[t].y, test_hoop[t].z);
				new_hoop.addPoint(p);//hoop_test[t]
				//new_hoop.addPoint(test_hoop[t]);
			}
			else
			{
				cout << "t = -1,test_hoop is empty" << endl;
			}
			//Point new_point = lines[index].getIntersectionPoint(hoop[i].y);

		}

		return new_hoop;
	}
	else //layer_index是单数
	{
		double y_min = hoop[hoop.size() - 1].y;
		double y_max = hoop[0].y;
		double line_length = y_max - y_min;
		double dis = (y_max - y_min) / 600.0;
		

		for (size_t i = 0; i < y_order.size(); i++) //size_t i = 0; i < 600; i++
		{
			test_hoop.clear();
			vector<int> indexes = lines.getIntersectionLineSegmentIndex(y_order[y_order.size()-1-i], 1);//y_max - dis * i
			if (indexes.size() == 0)
			{
				std::cout << "layer_index = " << layer_index << std::endl;
				std::cout << "y = " << y_min + dis * i << std::endl;
			}
			for (int j = 0; j < indexes.size(); j++)
			{
				Point test_point = lines[indexes[j]].getIntersectionPoint(y_order[y_order.size() - 1 - i]); //
				test_hoop.addPoint(test_point);
			}
			int t = getMaxZ(test_hoop);
			if (t != -1)
			{
				Point p = Point(test_hoop[t].x, test_hoop[t].y, test_hoop[t].z   );
				new_hoop.addPoint(p);//hoop_test[t]
				//new_hoop.addPoint(test_hoop[t]);
			}
			else
			{
				cout << "t = -1,test_hoop is empty" << endl;
			}
			//Point new_point = lines[index].getIntersectionPoint(hoop[i].y);

		}

		return new_hoop;
	}
	
}
Hoop MyModel::stepTwo(int layer_index)
{

	
	Hoop hoop = stepOne(layer_index);
	Hoop new_hoop;

	for (size_t i = 0 ;i < hoop.size(); i++)
	{
		if (i ==0 || i== hoop.size()-1)
		{
			Point n = getNormalsLeft(hoop, i, layer_index);
			double R = 0.1;
			Point p = Point(hoop[i].x, hoop[i].y , hoop[i].z +R *0.4);//
			new_hoop.addPoint(p);
			//cout << " the y:" << new_hoop[i].y << endl;
		}
		else
		{
			Point n1 = getNormalsLeft(hoop, i, layer_index);
			Point n2 = getNormalsRight(hoop, i, layer_index);
			double R = 0.1;
			Point p1 = Point(hoop[i].x, hoop[i].y + n1.y*R, hoop[i].z + n1.z*R );//
			new_hoop.addPoint(p1);
			Point p2 = Point(hoop[i].x, hoop[i].y + n2.y*R, hoop[i].z + n2.z*R );//
			new_hoop.addPoint(p2);
		}
		
	}
	
	return new_hoop;
}

Hoop MyModel::stepThree(int layer_index)
{
	Hoop hoop = stepTwo(layer_index);
	Hoop new_hoop = Hoop();

	
	vector<double> y_order = getYOrder(hoop);

	cout << "stepThree, y_order.size:" << y_order.size() << endl;

	if (layer_index == 16)
	{
		for (size_t i = 0; i < y_order.size(); i++)
		{
			cout << "3 y_order[i]" << y_order[i] << endl;
		}
	}


	Lines lines = Lines(hoop);
	Hoop test_hoop;
	if (layer_index % 2 == 0)
	{
		//cout << "hoop.size():" << hoop.size() << endl;
		//cout << "lines.size():" << lines.size() << endl;

		double y_min = hoop[0].y;
		double y_max = hoop[hoop.size() - 1].y;
		int size_of_hoop = hoop.size();

		double dis = (y_max - y_min) / 600.0;

		for (size_t i = 0; i < y_order.size(); i++)
		{
			
			
			test_hoop.clear();
			vector<int> indexes = lines.getIntersectionLineSegmentIndex(y_order[i], 1);//
																				 
			for (int j = 0; j < indexes.size(); j++)
			{
				Point test_point = lines[indexes[j]].getIntersectionPoint(y_order[i]); //
				test_hoop.addPoint(test_point);
			}
			int t = getMaxZ(test_hoop);
			if (t != -1)
			{
				if (layer_index == 242)
				{
					new_hoop.addPoint(test_hoop[t]);
				}
				else
				{
					if (new_hoop.isOK(test_hoop[t].y))
					{
						//cout << "isOK" << endl;
						new_hoop.addPoint(test_hoop[t]);
					}
				}
				
				//new_hoop.addPoint(test_hoop[t]);
			}
			else
			{
				cout << "t != -1" << endl;
			}

			if (i != y_order.size() - 1)
			{
				if (fabs(y_order[i] - y_order[i + 1])<eps1)
				{
					i++;
				}
			}
			//Point new_point = lines[index].getIntersectionPoint(hoop[i].y);
		}

		return new_hoop;
	}
	else
	{
		double y_min = hoop[hoop.size() - 1].y;
		double y_max = hoop[0].y;
		double line_length = y_max - y_min;
		double dis = (y_max - y_min) / 600.0;

		for (size_t i = 0; i < y_order.size(); i++)
		{
			test_hoop.clear();
			vector<int> indexes = lines.getIntersectionLineSegmentIndex(y_order[y_order.size()-i-1], 1);//

			for (int j = 0; j < indexes.size(); j++)
			{
				Point test_point = lines[indexes[j]].getIntersectionPoint(y_order[y_order.size() - i - 1]); //
				test_hoop.addPoint(test_point);
			}
			int t = getMaxZ(test_hoop);
			if (t != -1)
			{
				new_hoop.addPoint(test_hoop[t]);
			}
			else
			{
				cout << "t != -1" << endl;
			}
			//Point new_point = lines[index].getIntersectionPoint(hoop[i].y);
			if (i != y_order.size() - 1)
			{
				if (fabs(y_order[y_order.size() - i - 1] - y_order[y_order.size() - i - 2])<eps)
				{
					//cout << "i++" << endl;
					i++;
				}
			}
			
		}

		return new_hoop;
	}	

	
}

Hoop MyModel::stepFour(int layer_index)
{
	Hoop hoop = stepThree(layer_index);
	//Hoop former_hoop = stepThree(layer_index - 1);
	
	Hoop new_hoop;
	for (int i = 0; i < hoop.size() - 1; i++)
	{
		if (i == 0 )
		{
			new_hoop.addPoint(hoop[0]);
		}
		else
		{
			double offsetz = new_hoop.getLastPoint().z - hoop[i].z;
			double offsety = new_hoop.getLastPoint().y - hoop[i].y;
			//offsety = offsety / 180.0*PI * 20.2;//
			if (fabs(offsetz) > fabs(getoffsetZShouldBe(offsety)))
			{
				if (offsetz > getoffsetZShouldBe(offsety))//正的，下降
				{
					Point p = Point(hoop[i].x, hoop[i].y, new_hoop.getLastPoint().z - fabs(getoffsetZShouldBe(offsety)));
					new_hoop.addPoint(p);
				}
				else
				{
					Point p = Point(hoop[i].x, hoop[i].y, new_hoop.getLastPoint().z + fabs(getoffsetZShouldBe(offsety)));
					new_hoop.addPoint(p);
				}
			}
			else
			{
				new_hoop.addPoint(hoop[i]);
			}
		}	

	}
		
		
	/*if (layer_index == Gcode_hoops.size() - 1)
	{
		formor_hoop_step4 = hoop;
		return hoop;
	}
	else
	{
		std::cout << "layer_index ： " << layer_index << std::endl;
		std::cout << "hoop.size ：" << hoop.size() << std::endl;
		std::cout << "formor_hoop_step4.size ：" << formor_hoop_step4.size() << std::endl;

		for(int i = 0 ; i < hoop.size();i++)
		{
			Point p = formor_hoop_step4[formor_hoop_step4.size() - i - 1];
			double h = p.z - hoop[i].z;
			if (h > 0.2)
			{
				std::cout << "这里 h > 0.2，i= " << i<< std::endl;
				
				Point p1 = Point(hoop[i].x, hoop[i].y , p.z-0.2);//
				new_hoop.addPoint(p1);
			}			
			else
			{
				
				new_hoop.addPoint(hoop[i]);
			}
			formor_hoop_step4 = new_hoop;
			std::cout << "formor_hoop_step4.size ：" << formor_hoop_step4.size() << std::endl;
			std::cout << "new_hoop.size ：" << new_hoop.size() << std::endl;
			return new_hoop;
		}
		
	}*/
	return new_hoop;
}
vector<vector<int>> MyModel::dosomething(vector<int> wa)
{
	vector<vector<int>> return_vector;
	vector<int> int_vector1, int_vector2;;
	for (int i = 0 ; i < wa.size(); i++)
	{
		
		if (wa[i] == -1)
		{
			if (int_vector2.size() != 0)
			{
				return_vector.push_back(int_vector2);
				int_vector2.clear();
			}
			
			int_vector1.push_back(-1);
		}
		else
		{
			if (int_vector1.size() != 0)
			{
				return_vector.push_back(int_vector1);
				int_vector1.clear();
			}
			
			int_vector2.push_back(wa[i]);
		}
	}
	if (return_vector.size() == 0)
	{
		for (int i = 0; i < wa.size(); i++)
		{
			int_vector1.push_back(-1);
		}
		return_vector.push_back(int_vector1);
	}

	return return_vector;
}
int MyModel::getMaxZ(Hoop hoop)
{
	if (hoop.size() == 0)
	{
		return -1;
	}
	double zmax = hoop[0].z;
    int	index = 0;
	for (int i = 1; i < hoop.size(); i++)
	{
		if (hoop[i].z > zmax)
		{
			zmax = hoop[i].z;
			index = i;
		}
	}
	return index;
}

double MyModel::get_angle(Point p1, Point p2, Point p3)
{
	//double x1, double y1, double x2, double y2, double x3, double y3;
	double x1 = p1.y;
	double y1 = p1.z;
	double x2 = p1.y;
	double y2 = p1.z;
	double x3 = p1.y;
	double y3 = p1.z;
	double theta = atan2(x1 - x3, y1 - y3) - atan2(x2 - x3, y2 - y3);
	if (theta > M_PI)
		theta -= 2 * M_PI;
	if (theta < -M_PI)
		theta += 2 * M_PI;

	theta = abs(theta * 180.0 / M_PI);
	return theta;
}



Point MyModel::getNormalsLeft(Hoop hoop, int index, int layer_index)
{
	Point nt = Point(0, 0, 1);
	if (index == 0)
	{
		Point2D n1;
		Point p, p2, p3;
		double x1 = 0, y1 = 0, z1 = 0;
		p2 = hoop[0];
		p3 = hoop[1];
		p = p3 - p2;
		if ( p.z >= 0) //p.y >= 0 && p.z >= 0
		{
			x1 = 0.0;
			y1 = -1.0 / p.y;
			z1 = 1.0 / p.z;
		}
		else if ( p.z < 0) //p.y >= 0 && p.z < 0
		{
			x1 = 0.0;
			y1 = 1.0 / p.y;
			z1 = -1.0 / p.z;
		}

		Point ns = Point(x1, y1, z1);
		Point n = corssProduct(nt,corssProduct(ns, nt));
		n = normalized(n);
		
		return n;
	}
	else if (index == hoop.size() - 1)
	{
		Point2D n1;
		Point p, p2, p3;
		p2 = hoop[hoop.size() - 2];
		p3 = hoop[hoop.size() - 1];

		double x1 = 0, y1 = 0, z1 = 0;
		
		p = p3 - p2;
		if ( p.z >= 0) //p.y >= 0 && p.z >= 0
		{
			x1 = 0;
			y1 = -1.0 / p.y;
			z1 = 1.0 / p.z;
		}
		else if ( p.z < 0) //p.y >= 0 && p.z < 0
		{
			
			x1 = 0;
			y1 = 1.0 / p.y;
			z1 = -1.0 / p.z;
		}
		
		Point ns = Point(x1, y1, z1);
		Point n = corssProduct(nt, corssProduct(ns, nt));
		n = normalized(n);

		//cout << "n_R[20]y    " << n.y << endl;//n设置不到point里
		return n;
	}
	else
	{
		Point p1 = hoop[index - 1];
		Point p2 = hoop[index];
		
		Point p4 = p2 - p1;		

		double x1 = 0, y1 = 0, z1 = 0;
		
		if (p4.z >= 0) //p4.y >= 0 &&
		{
			x1 = 0;
			y1 = -1.0 / p4.y;
			z1 = 1.0 / p4.z;
		}
		else if ( p4.z < 0) //p4.y >= 0 &&
		{
			x1 = 0;
			y1 = 1.0 / p4.y;
			z1 = -1.0 / p4.z;
		}	
		Point ns = Point(x1, y1, z1);
		Point n = corssProduct(nt, corssProduct(ns, nt));
		n = normalized(n);
		
		return n;
	}
	
}

Point MyModel::getNormalsRight(Hoop hoop, int index, int layer_index)
{
	Point nt = Point(0, 0, 1);
	if (index == 0)
	{
		
		Point p, p2, p3;
		double x1 = 0, y1 = 0, z1 = 0;
		p2 = hoop[0];
		p3 = hoop[1];
		p = p3 - p2;
		if ( p.z >= 0)
		{
			x1 = 0;
			y1 = -1.0 / p.y;
			z1 = 1.0 / p.z;
			//x = -1.0 / p.y;
			//y = 1.0 / p.z;
		}
		else if ( p.z < 0)
		{
			x1 = 0;
			y1 = 1.0 / p.y;
			z1 = -1.0 / p.z;
		}


		Point ns = Point(x1, y1, z1);
		Point n = corssProduct(nt, corssProduct(ns, nt));
		n = normalized(n);
		return n;
	}
	else if (index == hoop.size() - 1)
	{
		
		Point p, p2, p3;
		p2 = hoop[hoop.size() - 2];
		p3 = hoop[hoop.size() - 1];

		double x1 = 0, y1 = 0, z1 =0;

		p = p3 - p2;
		if ( p.z >= 0)//p.y >= 0 &&
		{
			x1 = 0;
			y1 = -1.0 / p.y;
			z1 = 1.0 / p.z;
		}
		else if ( p.z < 0)
		{
			x1 = 0;
			y1 = 1.0 / p.y;
			z1 = -1.0 / p.z;
		}

		Point ns = Point(x1, y1, z1);
		Point n = corssProduct(nt, corssProduct(ns, nt));
		n = normalized(n);
		return n;
	}
	else
	{
		
		Point p2 = hoop[index];
		Point p3 = hoop[index + 1];
		
		Point p5 = p3 - p2;		

		double x1 = 0, y1 = 0, z1 = 0;  //////////////////////////////////////////////////////////////////////////

		if ( p5.z >= 0) //p5.y >= 0 &&
		{
			x1 = 0;
			y1 = -1.0 / p5.y;
			z1 = 1.0 / p5.z;
		}
		else if (  p5.z < 0) //p5.y >= 0&&
		{
			x1 = 0;
			y1 = 1.0 / p5.y;
			z1 = -1.0 / p5.z;
		}		
		Point ns = Point(x1, y1, z1);
		Point n = corssProduct(nt, corssProduct(ns, nt));
		n = normalized(n);
		return n;
	}
}

vector<double> MyModel::getYOrder(Hoop hoop)
{
	vector<double> y_order;
	for (int i = 0; i < hoop.size(); i++)
	{
		y_order.push_back(hoop[i].y);
	}
	sort(y_order.begin(), y_order.end());
	return y_order;
}