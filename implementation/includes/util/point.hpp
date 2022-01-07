#pragma once

#include <iostream>

namespace util {

template <typename T>
class Point {
public:
	Point()
	: x(), y() {}
	Point(const T& x, const T& y)
	: x(x), y(y) {}

	Point operator+(const Point& rhs) {
		return Point(x + rhs.x, y + rhs.y);
	}

	Point operator-(const Point& rhs) {
		return Point(x - rhs.x, y - rhs.y);
	}

	T x;
	T y;
};

template <typename T>
bool operator==(const Point<T>& a, const Point<T>& b) {
	return a.x == b.x && a.y == b.y;
}

template <typename T>
bool operator!=(const Point<T>& a, const Point<T>& b) {
	return !(a == b);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Point<T>& rhs) {
	out << "(" << rhs.x << ", " << rhs.y << ")";
	return out;
}

}
