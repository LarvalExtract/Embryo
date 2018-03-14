#pragma once

#include <iostream>

// Generic 2D vector
template <class T>
class Vec2
{
public:
	Vec2() : x(0), y(0) {}
	Vec2(T x, T y) : x(x), y(y) {}

	Vec2& operator+=(const Vec2 &rhs) { x += rhs.x; y += rhs.y; return *this; }
	Vec2& operator-=(const Vec2 &rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	Vec2& operator*=(const Vec2 &rhs) { x *= rhs.x; y *= rhs.y; return *this; }
	Vec2& operator/=(const Vec2 &rhs) { x /= rhs.x; y /= rhs.y; return *this; }

	T x;
	T y;
};

template <class T>
std::ostream& operator<<(std::ostream &output, const Vec2<T> &vector) { return output << "(" << vector.x << ", " << vector.y << ")"; }

template <class T> Vec2<T> operator+(Vec2<T> lhs, const Vec2<T> &rhs) { return lhs += rhs; }
template <class T> Vec2<T> operator-(Vec2<T> lhs, const Vec2<T> &rhs) { return lhs -= rhs; }
template <class T> Vec2<T> operator*(Vec2<T> lhs, const Vec2<T> &rhs) { return lhs *= rhs; }
template <class T> Vec2<T> operator/(Vec2<T> lhs, const Vec2<T> &rhs) { return lhs /= rhs; }

template <class T> Vec2<T> operator+(Vec2<T> lhs, const T &rhs) { lhs.x += rhs; lhs.y += rhs; return lhs; } 
template <class T> Vec2<T> operator-(Vec2<T> lhs, const T &rhs) { lhs.x -= rhs; lhs.y -= rhs; return lhs; }
template <class T> Vec2<T> operator*(Vec2<T> lhs, const T &rhs) { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
template <class T> Vec2<T> operator/(Vec2<T> lhs, const T &rhs) { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
