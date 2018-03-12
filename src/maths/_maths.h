#pragma once

#include "Vec3.h"

namespace Maths
{
	extern const float Pi;

	// TODO: Implement overloads for vector functions

	// Converts degrees to radians
	float Radians(float degrees);

	// Get the lowest value of 2 values
	float Min(float left, float right);

	// Get the highest value of 2 values
	float Max(float left, float right);

	// Clamp a value within a range
	float Clamp(float value, float min, float max);

	// Returns the dot product of 2 vectors
	float Dot(const Vec3<float> &left, const Vec3<float> &right);

	// Returns the cross product of 2 vectors
	Vec3<float> Cross(const Vec3<float> &left, const Vec3<float> &right);

	// Returns the magnitude (length) of a vector, using pythagorean theorum
	float Magnitude(const Vec3<float> &vector);

	// Returns the distance between 2 vectors
	float Distance(const Vec3<float> &left, const Vec3<float> &right);

	// Returns a normalised magnitude (length) of a vector
	Vec3<float> Normalise(const Vec3<float> &vector);
}