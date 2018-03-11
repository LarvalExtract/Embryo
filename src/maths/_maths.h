#pragma once

#include "vec3.h"

#define PI 3.14159f

////TODO: Implement overloads for vector functions

// Converts degrees to radians
inline float radians(float degrees)
{
	return degrees * (PI / 180.0f);
}

inline float min(const float& left, const float& right)
{
	if (left < right)
		return left;

	return right;
}

inline float max(const float& left, const float& right)
{
	if (left > right)
		return left;

	return right;
}

inline void clamp(float& value, const float& min, const float& max)
{
	if (value < min)
		value = min;

	if (value > max)
		value = max;
}

// Returns the dot product of 2 vectors
inline float dot(const vec3& left, const vec3& right) 
{
	return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}

// Returns the cross product of 2 vectors
inline vec3 cross(const vec3& left, const vec3& right)
{
	return vec3((left.y * right.z) - (left.z * right.y), (left.z * right.x) - (left.x * right.z), (left.x * right.y) - (left.y * right.x));
}

// Returns the magnitude (length) of a vector, using pythagorean theorum
inline float magnitude(const vec3& vector)
{
	return sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}

// Returns the distance between 2 vectors
inline float distance(const vec3& left, const vec3& right)
{
	return sqrt(((left.x - right.x) * (left.x - right.x)) + ((left.y - right.y) * (left.y - right.y)) + ((left.z - right.z) * (left.z - right.z)));
}

// Returns a normalised magnitude (length) of a vector
inline vec3 normalise(const vec3& vector)
{
	float m = magnitude(vector);

	if (m == 0)
		return vec3(0.0f, 0.0f, 0.0f);

	return vec3(vector.x / m, vector.y / m, vector.z / m);
}