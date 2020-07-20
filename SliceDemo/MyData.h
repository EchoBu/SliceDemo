#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include "clipper.hpp"
const double eps = 1e-6;
const double eps1 = 1e-2;
class Point2D
{
public:
	Point2D() {};
	Point2D(double a, double b) :x(a), y(b) {};
	Point2D operator-(Point2D a)
	{
		return Point2D(this->x - a.x, this->y - a.y);
	}
	Point2D operator+(Point2D a)
	{
		return Point2D(this->x + a.x, this->y + a.y);
	}
	Point2D operator=(Point2D a)
	{
		return Point2D(a.x, a.y);
	}
	Point2D operator*(double R)
	{
		return Point2D(R * this->x, R * this->y);
	}
	double distance2Point(Point2D a)
	{
		return sqrt((this->x - a.x)*(this->x - a.x) + (this->y - a.y)*(this->y - a.y));
	}
	void normalize()
	{
		double length = sqrt(this->x*this->x + this->y*this->y);
		//std::cout << "length:" << length << std::endl;
		this->x = this->x / length;
		this->y = this->y / length;
	}
public:
	double x;
	double y;
	//double z;
};

class Point
{
public:
	Point() {};
	Point(double a, double b, double c) :x(a), y(b), z(c) {};
	Point operator-(Point a)
	{
		return Point(x - a.x, y - a.y, z - a.z);
	}
	double distance2Point(Point a)
	{
		return sqrt((this->x - a.x)*(this->x - a.x) + (this->y - a.y)*(this->y - a.y) + (this->z - a.z)*(this->z - a.z));
	}
	double getNormalX() 
	{		
		
		return this->n_x;
	}
	double getNormalY()
	{
		
		return this->n_y;
	}
	void setNormal(Point2D p)
	{
		this->normal = Point2D(p.x, p.y); 
		this->n_x = p.x;
		this->n_y = p.y;
	}
	
	
public:
	double x;
	double y;
	double z;
	double n_x;
	double n_y;
	Point2D normal;
};




class Hoop
{
public:
	Hoop() {};
	Hoop(std::vector<Point> points) {
		this->points = points;
	};
	void addPoint(Point p)
	{
		this->points.push_back(p);
	}
	int size()
	{
		return points.size();
	}

	Point operator [](int i)
	{
		return points[i];
	}
	void deletePoint(int i)
	{
		if (i==0)
		{
			this->points.erase(points.begin());
		}
		else
		{
			this->points.erase(points.begin() + i-1);
		}
		
	}
	void reverse()
	{
		this->points.reserve(points.size());
	}
	void clear()
	{
		this->points.clear();
	}
	bool isOK(double test_y)
	{
		
		
		if (points.empty())
		{
			return true;
		}
		else if ((fabs(this->points.back().y - test_y) < 90.0) && !points.empty()) //10.0
		{
			return true;
		}
		else
		{
			return false;
		}
		
	}
	Point getLastPoint()
	{
		if (points.empty())
		{
			return Point(0,0,0);
		}
		else
		{
			return this->points.back();
		}
	}
protected:
private:
	std::vector<Point> points;
};

class Polygon
{
public:
protected:
private:
	ClipperLib::Path polygon;
};

class LineSegment
{
public:
	LineSegment() {};
	LineSegment(Point p1, Point p2)
	{
		this->p1 = p1;
		this->p2 = p2;
	}

	~LineSegment(){}

	Point getIntersectionPoint( double y)
	{
		double x_diff = this->p1.x - this->p2.x;
		double y_diff = this->p1.y - this->p2.y;
		double z_diff = this->p1.z - this->p2.z;

		double y1 = y-this->p1.y;
		//double y2 = y-this->p2.y;

		double ratio = y1 / y_diff;

		double new_x = this->p1.x + ratio * x_diff;
		double new_y = this->p1.y + ratio * y_diff;
		double new_z = this->p1.z + ratio * z_diff;

		Point p = Point(new_x, y, new_z);
		return p;
	}

	bool isIntersect(double y)
	{
		double y_max = this->p2.y;
		double y_min = this->p1.y;
		if (this->p1.y > this->p2.y)
		{
			y_max = this->p1.y;
			y_min = this->p2.y;
		}

		if (y_min < y && y <= y_max)
		{
			return true;
		}	
		
		else
		{
			return false;
		}
	}
	Point getFormerPoint()
	{
		return p1;
	}

	Point getLatterPoint()
	{
		return p2;
	}

private:
	Point p1;
	Point p2;
};

class Lines
{
public:
	Lines() {};
	~Lines() {};
	Lines(Hoop hoop)//构造函数
	{
		for (int i = 0 ; i < hoop.size() - 1 ; i++)
		{
			LineSegment l = LineSegment(hoop[i], hoop[i+1]);
			this->lines.push_back(l);
		}		
	}
	LineSegment operator [](int i)
	{
		return lines[i];
	}

	std::vector<int> getIntersectionLineSegmentIndex(double y , int flag) //flag=1代表stepone里面的操作
	{
		std::vector<int> arr;

		double disance = fabs(y - lines[0].getFormerPoint().y);
		float ratio = disance / getLineLengthY();
		//std::cout << "lines.size() " << lines.size() << std::endl;
		//std::cout << "ratio " << ratio << std::endl;
		int start_i = (ratio * size() - 20)>0 ? (ratio * size() - 20):0;
		int end_i = (ratio * size() + 20) > size() ? size() : (ratio * size() + 20);
		//std::cout << "ratio number:" << start_i << " to " << end_i << std::endl;

		if (flag == 1)
		{
			start_i = (ratio * size() - 150)>0 ? (ratio * size() - 150) : 0;
			end_i = (ratio * size() + 150) > size() ? size() : (ratio * size() + 150);
			start_i = 0;
			end_i = lines.size();
		}
		else if (flag == 3)
		{
			start_i = (ratio * size() - 20)>0 ? (ratio * size() - 20) : 0;
			end_i = (ratio * size() + 20) > size() ? size() : (ratio * size() + 20);
		}
		else
		{
			start_i = 0;
			end_i = this->lines.size();
		}
		for (int i = start_i; i < end_i; i++) //this->lines.size()
		{
			if (lines[i].isIntersect(y))
			{
				arr.push_back(i);
				//std::cout << i << " ";
			}
			else
			{
				continue;
			}
		}
		//std::cout <<  std::endl;;
		if (arr.size() == 0)
		{
			//std::cout << "y = " << y << std::endl;
		}
		
		return arr;
	}
	double getMaxY()//得到line的最大Y值
	{
		if (lines[0].getFormerPoint().y > lines[lines.size()-1].getLatterPoint().y)
		{
			return lines[0].getFormerPoint().y;
		}
		else 
		{
			return lines[lines.size() - 1].getLatterPoint().y;
		}
	}
	double getLineLengthY()//得到line在Y轴上的分量的长度
	{
		return fabs(lines[lines.size() - 1].getLatterPoint().y - lines[0].getFormerPoint().y);
	}
	int size()
	{
		return lines.size();
	}
protected:
private:
	std::vector<LineSegment> lines;
};