#pragma once
#include <cmath>

struct vector2
{
	float x, y;

	vector2 operator+(const vector2& v) const {
		return { x + v.x, y + v.y };
	}

	vector2 operator-(const vector2& v) const {
		return { x - v.x, y - v.y };
	}

	vector2 operator*(const float& n) const {
		return { x * n , y * n };
	}

	vector2 operator/(const float& n) const {
		return { x / n, y / n };
	}

	float operator>>(const vector2& v) const {
		return std::sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
	}
};

struct vector3
{
	float x, y, z;

	vector3 operator+(const vector3& v) const {
		return { x + v.x, y + v.y, z + v.z };
	}

	vector3 operator-(const vector3& v) const {
		return { x - v.x, y - v.y, z - v.z };
	}

	vector3 operator*(const float& n) const {
		return { x * n , y * n, z * n };
	}

	vector3 operator/(const float& n) const {
		return { x / n, y / n, z / n };
	}
};