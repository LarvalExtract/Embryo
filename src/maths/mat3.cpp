#include "mat3.h"
#include "math.h"

mat3::mat3()
{
	for (int column = 0; column < MATRIX_SIZE; column++)
		for (int row = 0; row < MATRIX_SIZE; row++)
			elements[column][row] = 0;
}

mat3::mat3(float diagonal)
{
	for (int column = 0; column < MATRIX_SIZE; column++)
		for (int row = 0; row < MATRIX_SIZE; row++)
			elements[column][row] = 0;

	for (int i = 0; i < MATRIX_SIZE; i++)
		elements[i][i] = diagonal;
}

mat3::mat3(float a0, float a1, float a2,
		   float b0, float b1, float b2,
		   float c0, float c1, float c2)
{
	elements[0][0] = a0;
	elements[0][1] = a1;
	elements[0][2] = a2;

	elements[1][0] = b0;
	elements[1][1] = b1;
	elements[1][2] = b2;

	elements[2][0] = c0;
	elements[2][1] = c1;
	elements[2][2] = c2;
}

mat3 mat3::Identity()
{
	return mat3(1.0f);
}

// ._.
mat3& mat3::Multiply(const mat3& other)
{
	mat3 result;

	for (int matrix = 0; matrix < MATRIX_SIZE; matrix++)
		for (int column = 0; column < MATRIX_SIZE; column++)
			for (int row = 0; row < MATRIX_SIZE; row++)
				result.elements[matrix][column] += elements[matrix][row] * other.elements[row][column];

	return result;
}

std::ostream& operator<<(std::ostream& output, const mat3& matrix)
{
	return output 
		<< "[" << matrix.elements[0][0] << "\t" << matrix.elements[1][0] << "\t" << matrix.elements[2][0] << "]" << "\n"
		<< "[" << matrix.elements[0][1] << "\t" << matrix.elements[1][1] << "\t" << matrix.elements[2][1] << "]" << "\n"
		<< "[" << matrix.elements[0][2] << "\t" << matrix.elements[1][2] << "\t" << matrix.elements[2][2] << "]";				  
}

//mat3 mat3::operator=(const mat4& matrix4)
//{
//	return mat3
//		(matrix4.elements[0][0], matrix4.elements[0][1], matrix4.elements[0][2],
//		 matrix4.elements[1][0], matrix4.elements[1][1], matrix4.elements[1][2],
//		 matrix4.elements[2][0], matrix4.elements[2][1], matrix4.elements[2][2]);
//}

mat3 operator*(mat3 left, const mat3& right)
{
	return left.Multiply(right);
}

mat3& mat3::operator*=(const mat3& right)
{
	return Multiply(right);
}

mat3 mat3::Translation(const Vec2<float> &translate)
{
	mat3 result(1.0f);

	result.elements[2][0] = translate.x;
	result.elements[2][1] = translate.y;

	return result;
}

mat3 mat3::Rotation(float angle)
{
	mat3 result(1.0f);

	float r = Maths::Radians(angle);

	result.elements[0][0] = cos(r);
	result.elements[0][1] = sin(r);

	result.elements[1][0] = sin(r);
	result.elements[1][1] = cos(r);

	return result;
}

mat3 mat3::Scale(const Vec2<float> &scale)
{
	mat3 result(1.0f);

	result.elements[0][0] = scale.x;

	result.elements[1][1] = scale.y;

	return result;
}