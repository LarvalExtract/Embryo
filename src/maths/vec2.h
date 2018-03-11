#pragma once

#include <iostream>

struct vec2
{
public:
	float x, y;

	vec2();
	vec2(const float& x, const float& y);

	friend std::ostream& operator<<(std::ostream& output, const vec2& vector);

	friend vec2& operator+(vec2& left, const vec2& right);
	friend vec2& operator-(vec2& left, const vec2& right);
	friend vec2& operator*(vec2& left, const vec2& right);
	friend vec2& operator/(vec2& left, const vec2& right);

	void operator+=(const vec2& right);
	void operator-=(const vec2& right);
	void operator*=(const vec2& right);
	void operator/=(const vec2& right);

private:
	vec2& Add(const vec2& other);
	vec2& Subtract(const vec2& other);
	vec2& Multiply(const vec2& other);
	vec2& Divide(const vec2& other);

};
