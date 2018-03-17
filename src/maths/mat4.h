#pragma once
#include <maths/maths.h>

class Mat4
{
public:
	Mat4() : elements{ 0 } {};
	Mat4(float scalar);

	Mat4 Identity();
	void Translate(const Vec3<float> &translate);
	void Rotate(float angle, const Vec3<float> &axis);
	void Scale(const Vec3<float> &scale);
	void LookAt(Vec3<float> pos, const Vec3<float> &target, const Vec3<float> &up);
	void Orthographic(float left, float right, float bottom, float top, float near, float far);
	void Perspective(float fov, float aspectRatio, float near, float far);

	void NullTranslate();

	Mat4& operator= (const Mat4 &rhs);
	Mat4& operator+=(const Mat4 &rhs);
	Mat4& operator-=(const Mat4 &rhs);
	Mat4& operator*=(const Mat4 &rhs);

	float elements[4][4];

private:
	const char columnSize = 4;
	const char rowSize = 4;
};

Mat4 operator+(Mat4 lhs, const Mat4 &rhs);
Mat4 operator-(Mat4 lhs, const Mat4 &rhs);
Mat4 operator*(Mat4 lhs, const Mat4 &rhs);