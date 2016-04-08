#include "Coords.h"


bool Coords::operator==(const Coords &v) const
{
	return ((x() == v.x()) && (y() == v.y()) && (z() == v.z()));
}

Coords Coords::operator+=(const Coords & v) const
{
	return Coords(x() + v.x(), y() + v.y(), z() + v.z());
}

Coords Coords::operator+(const Coords & v) const
{
	return Coords(x() + v.x(), y() + v.y(), z() + v.z());
}

Coords Coords::operator-=(const Coords & v) const
{
	return Coords(x() - v.x(), y() - v.y(), z() - v.z());
}

Coords Coords::operator*(const int & v) const
{
	return Coords(x()*v, y()*v, z()*v);
}

Coords::operator PolyVox::Vector3DInt32() const
{
	PolyVox::Vector3DInt32 vec;
	vec.setElements(x(), y(), z());
	return vec;
}

bool Coords::operator>(const Coords &v) const
{
	return (x()*x() + y()*y() + z()*z()) > (v.x()*v.x() + v.y()*v.y() + v.z()*v.z());
}

bool Coords::operator<(const Coords &v) const
{
	if (x() < v.x()) return true;
	if (x() > v.x()) return false;
	//x == coord.x
	if (y() < v.y()) return true;
	if (y() > v.y()) return false;
	//x == coord.x && y == coord.y
	if (z() < v.z()) return true;
	if (z() > v.z()) return false;
	//*this == coord
	return false;
}

bool Coords::operator>=(const Coords &v) const
{
	return (x()*x() + y()*y() + z()*z()) > (v.x()*v.x() + v.y()*v.y() + v.z()*v.z()) ||
		(x()*x() + y()*y() + z()*z()) == (v.x()*v.x() + v.y()*v.y() + v.z()*v.z());
}

bool Coords::operator<=(const Coords &v) const
{
	return (x()*x() + y()*y() + z()*z()) < (v.x()*v.x() + v.y()*v.y() + v.z()*v.z()) ||
		(x()*x() + y()*y() + z()*z()) == (v.x()*v.x() + v.y()*v.y() + v.z()*v.z());
}

std::ostream& operator<<(std::ostream& os, const Coords& c)
{
	os << "(" << c.x() << ',' << c.y() << ',' << c.z() << ")";
	return os;
}