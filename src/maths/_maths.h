#pragma once

#include "Vec3.h"

#define PI 3.14159f

////TODO: Implement overloads for vector functions

// Converts degrees to radians
inline float radians(float degrees)
{
	return degrees * (PI / 180.0f);
}

inline float min(float left, float right)
{
	if (left < right)
		return left;

	return right;
}

inline float max(float left, float right)
{
	if (left > right)
		return left;

	return right;
}

inline void clamp(float& value, float min, float max)
{
	if (value < min)
		value = min;

	if (value > max)
		value = max;
}

// Returns the dot product of 2 vectors
inline float dot(const Vec3<float> &left, const Vec3<float> &right)
{
	return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}

// Returns the cross product of 2 vectors
inline Vec3<float> cross(const Vec3<float> &left, const Vec3<float> &right)
{
	return Vec3<float>((left.y * right.z) - (left.z * right.y), (left.z * right.x) - (left.x * right.z), (left.x * right.y) - (left.y * right.x));
}

// Returns the magnitude (length) of a vector, using pythagorean theorum
inline float magnitude(const Vec3<float> &vector)
{
	return sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}

// Returns the distance between 2 vectors
inline float distance(const Vec3<float> &left, const Vec3<float> &right)
{
	return sqrt(((left.x - right.x) * (left.x - right.x)) + ((left.y - right.y) * (left.y - right.y)) + ((left.z - right.z) * (left.z - right.z)));
}

// Returns a normalised magnitude (length) of a vector
inline Vec3<float> normalise(const Vec3<float> &vector)
{
	float m = magnitude(vector);

	if (m == 0)
		return Vec3<float>(0.0f, 0.0f, 0.0f);

	return Vec3<float>(vector.x / m, vector.y / m, vector.z / m);
}