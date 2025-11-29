#pragma once

struct Point {
	float x, y, z;

	Point();
	Point(float same);
	Point(float x, float y, float z);

	Point& operator=  (const Point& other);
	bool   operator== (const Point& other) const;
	bool   operator!= (const Point& other) const;
	Point  operator-  () const;
	
	Point  operator+  (const Point& other) const;
	Point& operator+= (const Point& other);
	Point  operator-  (const Point& other) const;
	Point& operator-= (const Point& other);	
	
	Point  operator+  (const float& scalar) const;
	Point& operator+= (const float& scalar);
	Point  operator-  (const float& scalar) const;
	Point& operator-= (const float& scalar);
	Point  operator*  (const float& scalar) const;
	Point& operator*= (const float& scalar);
	Point  operator/  (const float& scalar) const;
	Point& operator/= (const float& scalar);
};