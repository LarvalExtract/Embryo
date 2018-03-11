#include "vec3.h"

vec3::vec3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

vec3::vec3(const float& x, const float& y, const float& z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

vec3& vec3::Add(const vec3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

vec3& vec3::Subtract(const vec3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

vec3& vec3::Multiply(const vec3& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;

	return *this;
}

vec3& vec3::Divide(const vec3& other)
{
	x /= other.x;
	y /= other.y;
	z -= other.z;

	return *this;
}



std::ostream& operator<<(std::ostream& output, const vec3& vector)
{
	return output << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
}

vec3& operator+(vec3 left, const vec3& right)
{
	return left.Add(right);
}

vec3& operator+(vec3 left, const float& right)
{
	return vec3(left.x + right, left.y + right, left.z + right);
}

vec3& operator-(vec3 left, const vec3& right)
{
	return left.Subtract(right);
}

vec3& operator-(vec3 left, const float& right)
{
	return vec3(left.x - right, left.y - right, left.z - right);
}

vec3& operator*(vec3 left, const vec3& right)
{
	return left.Multiply(right);
}

vec3& operator*(vec3 left, const float& right)
{
	return vec3(left.x * right, left.y * right, left.z * right);
}

vec3& operator/(vec3 left, const vec3& right)
{
	return left.Divide(right);
}

vec3& operator/(vec3 left, const float& right)
{
	return vec3(left.x / right, left.y / right, left.z / right);
}


void vec3::operator+=(vec3 right)
{
	this->Add(right);
}

void vec3::operator-=(vec3 right)
{
	this->Subtract(right);
}

void vec3::operator*=(const vec3& right)
{
	this->Multiply(right);
}

void vec3::operator/=(const vec3& right)
{
	this->Divide(right);
}