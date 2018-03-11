#include "vec4.h"

vec4::vec4()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

vec4::vec4(const float& x, const float& y, const float& z, const float& w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

vec4& vec4::Add(const vec4& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;

	return *this;
}

vec4& vec4::Subtract(const vec4& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;

	return *this;
}

vec4& vec4::Multiply(const vec4& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;

	return *this;
}

vec4& vec4::Divide(const vec4& other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
	w /= other.w;

	return *this;
}



std::ostream& operator<<(std::ostream& output, const vec4& vector)
{
	return output << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
}

vec4& operator+(vec4& left, const vec4& right)
{
	return left.Add(right);
}

vec4& operator-(vec4& left, const vec4& right)
{
	return left.Subtract(right);
}

vec4& operator*(vec4& left, const vec4& right)
{
	return left.Multiply(right);
}

vec4& operator/(vec4& left, const vec4& right)
{
	return left.Divide(right);
}


void vec4::operator+=(const vec4& right)
{
	this->Add(right);
}

void vec4::operator-=(const vec4& right)
{
	this->Subtract(right);
}

void vec4::operator*=(const vec4& right)
{
	this->Multiply(right);
}

void vec4::operator/=(const vec4& right)
{
	this->Divide(right);
}