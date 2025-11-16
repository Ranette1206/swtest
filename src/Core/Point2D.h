#pragma once

#include <cmath>

namespace sw::core
{

template <typename T>
class Point2D
{
public:
	T x;
	T y;

	bool operator==(const Point2D& other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const Point2D& other) const
	{
		return !(*this == other);
	}

	Point2D operator+(const Point2D& other) const
	{
		return Point2D(x + other.x, y + other.y);
	}

	Point2D operator-(const Point2D& other) const
	{
		return Point2D(x - other.x, y - other.y);
	}

	Point2D& operator+=(const Point2D& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Point2D& operator-=(const Point2D& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	T distance(const Point2D& other) const
	{
		T dx = x - other.x;
		T dy = y - other.y;
		return static_cast<T>(std::sqrt(static_cast<double>(dx * dx + dy * dy)));
	}

	bool isInRange(const Point2D& other, T range) const
	{
		return distance(other) <= range;
	}

	T length() const
	{
		return static_cast<T>(std::sqrt(static_cast<double>(x * x + y * y)));
	}

	Point2D normalized() const
	{
		T len = length();
		return Point2D(x / len, y / len);
	}
};

using Point2DInt = Point2D<int32_t>;
using Direction2D = Point2D<int32_t>;

}
