#include "vec2.h"

vec2::vec2()
{
	x = 0.0f;
	y = 0.0f;
}

vec2::vec2(const float& x, const float& y)
{
	this->x = x;
	this->y = y;
}

vec2& vec2::Add(const vec2& other)
{
	x += other.x;
	y += other.y;

	return *this;
}

vec2& vec2::Subtract(const vec2& other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

vec2& vec2::Multiply(const vec2& other)
{
	x *= other.x;
	y *= other.y;

	return *this;
}

vec2& vec2::Divide(const vec2& other)
{
	x /= other.x;
	y /= other.y;

	return *this;
}



std::ostream& operator<<(std::ostream& output, const vec2& vector)
{
	return output << "(" << vector.x << ", " << vector.y << ")";
}

vec2& operator+(vec2& left, const vec2& right)
{
	return left.Add(right);
}

vec2& operator-(vec2& left, const vec2& right)
{
	return left.Subtract(right);
}

vec2& operator*(vec2& left, const vec2& right)
{
	return left.Multiply(right);
}

vec2& operator/(vec2& left, const vec2& right)
{
	return left.Divide(right);
}


void vec2::operator+=(const vec2& right)
{
	this->Add(right);
}

void vec2::operator-=(const vec2& right)
{
	this->Subtract(right);
}

void vec2::operator*=(const vec2& right)
{
	this->Multiply(right);
}

void vec2::operator/=(const vec2& right)
{
	this->Divide(right);
}