#pragma once

/**
VoxTreeCoord.h
Header for class VoxTreeCoord
*/
class VoxTreeCoord
{
public:
	VoxTreeCoord(int x, int y, int z) : x(x), y(y), z(z) {}
	virtual ~VoxTreeCoord() {}

	VoxTreeCoord operator+(const VoxTreeCoord & co) { return VoxTreeCoord(x + co.x, y + co.y, z + co.z); }
	VoxTreeCoord operator-(const VoxTreeCoord & co) { return VoxTreeCoord(x - co.x, y - co.y, z - co.z); }
	VoxTreeCoord operator+=(const VoxTreeCoord & co) { x += co.x; y += co.y; z += co.z; return *this; }
	VoxTreeCoord operator-=(const VoxTreeCoord & co) { x -= co.x; y -= co.y; z -= co.z; return *this; }

public:
	union
	{
		struct
		{
			int x;
			int y;
			int z;
		};
		int pos[3];
	};
};