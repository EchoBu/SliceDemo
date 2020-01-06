#pragma once
#include <iostream>
#include <vector>
#include "clipper.hpp"


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
public:
	double x;
	double y;
	double z;
};

class Point2D
{
public:
	Point2D() {};
	Point2D(double a, double b) :x(a), y(b) {};
	Point2D operator-(Point2D a)
	{
		return Point2D(x - a.x, y - a.y);
	}
	double distance2Point(Point2D a)
	{
		return sqrt((this->x - a.x)*(this->x - a.x) + (this->y - a.y)*(this->y - a.y));
	}
public:
	double x;
	double y;
	//double z;
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