#pragma once

namespace util {

template <typename T>
class Point {
private:
	Point& operator=(const Point&);

public:
	Point()
	: x(), y() {}
	Point(const T& x, const T& y)
	: x(x), y(y) {}
	Point(const Point& rhs)
	: x(rhs.x), y(rhs.y) {}

	Point operator+(const Point& rhs) {
		return Point(x + rhs.x, y + rhs.y);
	}

	Point operator-(const Point& rhs) {
		return Point(x - rhs.x, y - rhs.y);
	}

	const T x;
	const T y;
};

}
