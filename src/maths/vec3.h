#pragma once

#include <iostream>

template <class T>
class Vec3
{
public:
	Vec3() : x(0), y(0), z(0) {}
	Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

	Vec3& operator+=(const Vec3 &rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	Vec3& operator-=(const Vec3 &rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
	Vec3& operator*=(const Vec3 &rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
	Vec3& operator/=(const Vec3 &rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

	T x;
	T y;
	T z;
};

template <class T>
std::ostream& operator<<(std::ostream &output, const Vec3<T> &vector) { return output << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")"; }

template <class T> Vec3<T> operator+(Vec3<T> lhs, const Vec3<T> &rhs) { return lhs += rhs; }
template <class T> Vec3<T> operator-(Vec3<T> lhs, const Vec3<T> &rhs) { return lhs -= rhs; }
template <class T> Vec3<T> operator*(Vec3<T> lhs, const Vec3<T> &rhs) { return lhs *= rhs; }
template <class T> Vec3<T> operator/(Vec3<T> lhs, const Vec3<T> &rhs) { return lhs /= rhs; }

template <class T> Vec3<T> operator+(Vec3<T> lhs, const T &rhs) { lhs.x += rhs; lhs.y += rhs; lhs.z += rhs; return lhs; }
template <class T> Vec3<T> operator-(Vec3<T> lhs, const T &rhs) { lhs.x -= rhs; lhs.y -= rhs; lhs.z -= rhs; return lhs; }
template <class T> Vec3<T> operator*(Vec3<T> lhs, const T &rhs) { lhs.x *= rhs; lhs.y *= rhs; lhs.z *= rhs; return lhs; }
template <class T> Vec3<T> operator/(Vec3<T> lhs, const T &rhs) { lhs.x /= rhs; lhs.y /= rhs; lhs.z /= rhs; return lhs; }
