#pragma once

#include <iostream>

struct vec4
{
public:
	float x, y, z, w;

	vec4();
	vec4(const float& x, const float& y, const float& z, const float& w);

	friend std::ostream& operator<<(std::ostream& output, const vec4& vector);

	friend vec4& operator+(vec4& left, const vec4& right);
	friend vec4& operator-(vec4& left, const vec4& right);
	friend vec4& operator*(vec4& left, const vec4& right);
	friend vec4& operator/(vec4& left, const vec4& right);

	void operator+=(const vec4& right);
	void operator-=(const vec4& right);
	void operator*=(const vec4& right);
	void operator/=(const vec4& right);

private:
	vec4& Add(const vec4& other);
	vec4& Subtract(const vec4& other);
	vec4& Multiply(const vec4& other);
	vec4& Divide(const vec4& other);

};
