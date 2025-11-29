#pragma once

struct Point
{
	float x, y, z;

	Point();
	Point(float same);
	Point(float x, float y, float z);

	Point operator-  (Point& other) const;
	bool  operator== (Point& other) const;
};