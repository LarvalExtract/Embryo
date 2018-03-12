#pragma once

#include <iostream>

template <class T>
class Vec4
{
public:
	Vec4() : x(0), y(0), z(0), w(0) {}
	Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(0) {}

	Vec4& operator+=(const Vec4 &rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
	Vec4& operator-=(const Vec4 &rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
	Vec4& operator*=(const Vec4 &rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this; }
	Vec4& operator/=(const Vec4 &rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w; return *this; }

	T x;
	T y;
	T z;
	T w;
};

template <class T>
std::ostream& operator<<(std::ostream &output, const Vec4<T> &vector) { return output << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")"; }

template <class T> Vec4<T> operator+(Vec4<T> lhs, const Vec4<T> &rhs) { return lhs += rhs; }
template <class T> Vec4<T> operator-(Vec4<T> lhs, const Vec4<T> &rhs) { return lhs -= rhs; }
template <class T> Vec4<T> operator*(Vec4<T> lhs, const Vec4<T> &rhs) { return lhs *= rhs; }
template <class T> Vec4<T> operator/(Vec4<T> lhs, const Vec4<T> &rhs) { return lhs /= rhs; }

template <class T> Vec4<T> operator+(Vec4<T> lhs, const T &rhs) { lhs.x += rhs; lhs.y += rhs; lhs.z += rhs; lhs.w += rhs; return lhs; }
template <class T> Vec4<T> operator-(Vec4<T> lhs, const T &rhs) { lhs.x -= rhs; lhs.y -= rhs; lhs.z -= rhs; lhs.w -= rhs; return lhs; }
template <class T> Vec4<T> operator*(Vec4<T> lhs, const T &rhs) { lhs.x *= rhs; lhs.y *= rhs; lhs.z *= rhs; lhs.w *= rhs; return lhs; }
template <class T> Vec4<T> operator/(Vec4<T> lhs, const T &rhs) { lhs.x /= rhs; lhs.y /= rhs; lhs.z /= rhs; lhs.w /= rhs; return lhs; }
