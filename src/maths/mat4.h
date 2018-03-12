#pragma once
#include <maths/_maths.h>
#include <iostream>

struct mat4
{
	static const int MATRIX_SIZE = 4;
	float elements[MATRIX_SIZE][MATRIX_SIZE];		// Column major ordering: matrix[column][row]

	mat4();
	mat4(float diagonal);
	mat4(float a0, float a1, float a2, float a3, 
		 float b0, float b1, float b2, float b3,
		 float c0, float c1, float c2, float c3, 
		 float d0, float d1, float d2, float d3 );
	
	static mat4 Identity();

	void NullTranslate();

	static mat4 Orthographic(float left, float right, float bottom, float top, float near, float far);
	static mat4 Perspective(float fov, float aspectRatio, float near, float far);

	static mat4 LookAt(Vec3<float> pos, const Vec3<float> &target, const Vec3<float> &up);

	static mat4 Translation(const Vec3<float> &translate);
	static mat4 Rotation(float angle, const Vec3<float> &axis);
	static mat4 Scale(const Vec3<float> &scale);

	friend std::ostream& operator<<(std::ostream &output, const mat4 &matrix);

	friend mat4 operator*(mat4 left, const mat4 &right);
	mat4& operator*=(const mat4& right);

private:
	mat4& Multiply(const mat4 &other);
};
