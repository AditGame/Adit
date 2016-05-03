#pragma once

#include <ostream>

#include <osg/Vec3i>

#include <PolyVox\Vector.h>

//osg Vec2i class with additions
class Coords
{
public:
	Coords(int x, int y, int z = 0) : _x(x), _y(y), _z(z) {}

	Coords(const Coords &v) : _x(v.x()), _y(v.y()), _z(v.z()) {}

	int x() const { return _x; }
	int y() const { return _y; }
	int z() const { return _z; }

	bool operator<(const Coords &v) const;
	bool operator<=(const Coords &v) const;
	bool operator>(const Coords &v) const;
	bool operator>=(const Coords &v) const;
	bool operator==(const Coords &v) const;
	Coords operator+=(const Coords &v) const;
	Coords operator+(const Coords & v) const;
	Coords operator-=(const Coords &v) const;
	Coords operator*(const int &v) const;

	int dist_squared(const Coords &v) const;

	int dist_squared_2D(const Coords & v) const;

	operator PolyVox::Vector3DInt32() const;
private:
	int _x;
	int _y;
	int _z;
};

std::ostream& operator<<(std::ostream& os, const Coords& c);

