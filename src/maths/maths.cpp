#include "maths.h"

namespace Maths
{
	const float HalfPi = 1.570796f;
	const float Pi = 3.1415927f;
	const float DoublePi = 6.283185f;

	// Converts degrees to radians
	float Radians(float degrees)
	{
		return degrees * (Pi / 180.0f);
	}

	float Min(float left, float right)
	{
		return left < right ? left : right;
	}

	float Max(float left, float right)
	{
		return left > right ? left : right;
	}

	float Clamp(float value, float min, float max)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;

		return value;
	}

	float Dot(const Vec3<float> &left, const Vec3<float> &right)
	{
		return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
	}

	Vec3<float> Cross(const Vec3<float> &left, const Vec3<float> &right)
	{
		return Vec3<float>((left.y * right.z) - (left.z * right.y), (left.z * right.x) - (left.x * right.z), (left.x * right.y) - (left.y * right.x));
	}

	float Magnitude(const Vec3<float> &vector)
	{
		return sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	}

	float Distance(const Vec3<float> &left, const Vec3<float> &right)
	{
		return sqrt(((left.x - right.x) * (left.x - right.x)) + ((left.y - right.y) * (left.y - right.y)) + ((left.z - right.z) * (left.z - right.z)));
	}

	Vec3<float> Normalise(const Vec3<float> &vector)
	{
		float m = Magnitude(vector);

		if (m == 0)
			return Vec3<float>(0.0f, 0.0f, 0.0f);

		return Vec3<float>(vector.x / m, vector.y / m, vector.z / m);
	}
}