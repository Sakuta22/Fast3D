#pragma once

namespace Console3D {

struct Point {
	float x, y, z;

	Point();
	Point(const float same);
	Point(const float x, const float y, const float z);

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

}