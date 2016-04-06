#pragma once

#include <ostream>

class Coords
{
public:
	Coords(int x, int y, int z=0) : x(x), y(y), z(z) {}

	Coords(const Coords& v) : x(v.getX()), y(v.getY()), z(v.getZ()) {}

	int getX() const { return x; }
	int getY() const { return y; }
	int getZ() const { return z; }

	bool operator<(const Coords &v) const;
	bool operator<=(const Coords &v) const;
	bool operator>(const Coords &v) const;
	bool operator>=(const Coords &v) const;
	bool operator==(const Coords &v) const;
	Coords operator+=(const Coords &v) const;
	Coords operator-=(const Coords &v) const;
	Coords operator*(const int &v) const;

private:
	int x;
	int y;
	int z;
};

std::ostream& operator<<(std::ostream& os, const Coords& c);

