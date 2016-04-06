#include "Coords.h"


bool Coords::operator==(const Coords &v) const
{
	return ((x == v.x) && (y == v.y) && (z == v.z));
}

Coords Coords::operator+=(const Coords & v) const
{
	return Coords(x + v.x, y + v.y, z + v.z);
}

Coords Coords::operator-=(const Coords & v) const
{
	return Coords(x - v.x, y - v.y, z - v.z);
}

Coords Coords::operator*(const int & v) const
{
	return Coords(x*v, y*v, z*v);
}

bool Coords::operator>(const Coords &v) const
{
	return (x*x + y*y + z*z) > (v.x*v.x + v.y*v.y + v.z*v.z);
}

bool Coords::operator<(const Coords &v) const
{
	return (x*x + y*y + z*z) < (v.x*v.x + v.y*v.y + v.z*v.z);
}

bool Coords::operator>=(const Coords &v) const
{
	return (x*x + y*y + z*z) > (v.x*v.x + v.y*v.y + v.z*v.z) ||
		(x*x + y*y + z*z) == (v.x*v.x + v.y*v.y + v.z*v.z);
}

bool Coords::operator<=(const Coords &v) const
{
	return (x*x + y*y + z*z) < (v.x*v.x + v.y*v.y + v.z*v.z) ||
		(x*x + y*y + z*z) == (v.x*v.x + v.y*v.y + v.z*v.z);
}

std::ostream& operator<<(std::ostream& os, const Coords& c)
{
	os << "(" << c.getX() << ',' << c.getY() << ',' << c.getZ() << ")";
	return os;
}