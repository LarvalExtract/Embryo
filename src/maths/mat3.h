#pragma once
#include <maths/_maths.h>
#include "vec2.h"
#include <iostream>

struct mat3
{
	static const int MATRIX_SIZE = 3;
	float elements[MATRIX_SIZE][MATRIX_SIZE];		// Column major ordering: matrix[column][row]

	mat3();
	mat3(float diagonal);
	mat3(float a0, float a1, float a2,
		 float b0, float b1, float b2,
		 float c0, float c1, float c2);
	
	static mat3 Identity();

	static mat3 Translation(const vec2& translate);
	static mat3 Rotation(const float& angle);
	static mat3 Scale(const vec2& scale);

	friend std::ostream& operator<<(std::ostream& output, const mat3& matrix);

	//mat3 operator=(const mat4& matrix4);

	friend mat3 operator*(mat3 left, const mat3& right);
	mat3& operator*=(const mat3& right);

private:
	mat3& Multiply(const mat3& other);
};
