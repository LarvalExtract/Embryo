#pragma once

#include <iostream>

struct vec3
{
	float x, y, z;

	vec3();
	vec3(const float& x, const float& y, const float& z);

	friend std::ostream& operator<<(std::ostream& output, const vec3& vector);

	friend vec3& operator+(vec3 left, const vec3& right);
	friend vec3& operator+(vec3 left, const float& right);

	friend vec3& operator-(vec3 left, const vec3& right);
	friend vec3& operator-(vec3 left, const float& right);

	friend vec3& operator*(vec3 left, const vec3& right);
	friend vec3& operator*(vec3 left, const float& right);

	friend vec3& operator/(vec3 left, const vec3& right);
	friend vec3& operator/(vec3 left, const float& right);

	void operator+=(vec3 right);// Fix these
	void operator-=(vec3 right);// Fix these
	void operator*=(const vec3& right);
	void operator/=(const vec3& right);

private:
	vec3& Add(const vec3& other);
	vec3& Subtract(const vec3& other);
	vec3& Multiply(const vec3& other);
	vec3& Divide(const vec3& other);

};
