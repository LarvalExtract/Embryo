#include "mat4.h"
#include "math.h"

mat4::mat4()
{
	for (int column = 0; column < MATRIX_SIZE; column++)
		for (int row = 0; row < MATRIX_SIZE; row++)
			elements[column][row] = 0;
}

mat4::mat4(float diagonal)
{
	for (int column = 0; column < MATRIX_SIZE; column++)
		for (int row = 0; row < MATRIX_SIZE; row++)
			elements[column][row] = 0;

	for (int i = 0; i < MATRIX_SIZE; i++)
		elements[i][i] = diagonal;
}

mat4::mat4(float a0, float a1, float a2, float a3,
		   float b0, float b1, float b2, float b3,
		   float c0, float c1, float c2, float c3,
		   float d0, float d1, float d2, float d3)
{
	elements[0][0] = a0;
	elements[0][1] = a1;
	elements[0][2] = a2;
	elements[0][3] = a3;

	elements[1][0] = b0;
	elements[1][1] = b1;
	elements[1][2] = b2;
	elements[1][3] = b3;

	elements[2][0] = c0;
	elements[2][1] = c1;
	elements[2][2] = c2;
	elements[2][3] = c3;

	elements[3][0] = d0;
	elements[3][1] = d1;
	elements[3][2] = d2;
	elements[3][3] = d3;
}

//mat4::mat4(const mat3& matrix3)
//{
//	for (char column = 0; column < 3; column++)
//		for (char row = 0; row < 3; row++)
//			elements[column][row] = matrix3.elements[column][row];
//
//	for (char i = 0; i < MATRIX_SIZE; i++)
//		elements[3][i] = 0.0f;
//
//	for (char i = 0; i < MATRIX_SIZE; i++)
//		elements[i][3] = 0.0f;
//}

mat4 mat4::Identity()
{
	return mat4(1.0f);
}

// Sets the translation column to 0, removing any translations from the matrix
void mat4::NullTranslate()
{
	for (char i = 0; i < MATRIX_SIZE; i++)
		elements[3][i] = 0.0f;

	elements[3][3] = 1.0f;
}

// ._.
mat4& mat4::Multiply(const mat4& other)
{
	mat4 result;

	for (int matrix = 0; matrix < MATRIX_SIZE; matrix++)
		for (int column = 0; column < MATRIX_SIZE; column++)
			for (int row = 0; row < MATRIX_SIZE; row++)
				result.elements[matrix][column] += elements[matrix][row] * other.elements[row][column];

	return result;
}

std::ostream& operator<<(std::ostream& output, const mat4& matrix)
{
	return output 
		<< "[" << matrix.elements[0][0] << "\t" << matrix.elements[1][0] << "\t" << matrix.elements[2][0] << "\t" << matrix.elements[3][0] << "]" << "\n"
		<< "[" << matrix.elements[0][1] << "\t" << matrix.elements[1][1] << "\t" << matrix.elements[2][1] << "\t" << matrix.elements[3][1] << "]" << "\n"
		<< "[" << matrix.elements[0][2] << "\t" << matrix.elements[1][2] << "\t" << matrix.elements[2][2] << "\t" << matrix.elements[3][2] << "]" << "\n"
		<< "[" << matrix.elements[0][3] << "\t" << matrix.elements[1][3] << "\t" << matrix.elements[2][3] << "\t" << matrix.elements[3][3] << "]";
}

mat4 operator*(mat4 left, const mat4& right)
{
	return left.Multiply(right);
}

mat4& mat4::operator*=(const mat4& right)
{
	return Multiply(right);
}

mat4 mat4::Orthographic(float left, float right, float bottom, float top, float near, float far)
{
	mat4 result;

	float rml = right - left;	// (r)ight (m)inus (l)eft
	float tmb = top - bottom;	// (t)op (m)inus (b)ottom
	float fmn = far - near;		// (f)ar (m)inus (n)ear

	float rpl = right + left;	// (l)eft (p)lus (r)ight
	float tpb = top + bottom;	// (t)op (p)lus (b)ottom
	float fpn = far + near;		// (f)ar (p)lus (n)ear

	// See: www.safaribooksonline.com/library/view/opengl-es-2/9781941222560/images/AspectRatio/OrthographicMatrixDefinition.png
	result.elements[0][0] =  2.0f / rml;
	
	result.elements[1][1] =  2.0f / tmb;

	result.elements[2][2] = -2.0f / fmn;

	result.elements[3][0] = -(rpl / rml);
	result.elements[3][1] = -(tpb / tmb);
	result.elements[3][2] = -(fpn / fmn);
	result.elements[3][3] =  1.0f;

	return result;
}

mat4 mat4::Perspective(float fov, float aspectRatio, float near, float far)
{
	mat4 result;

	float q = 1.0f / tanf(Maths::Radians(0.5f * fov));
	float zRange = near - far;
	//float fmn = far - near;		// (f)ar (m)inus (n)ear
	//float fpn = far + near;		// (f)ar (p)lus (n)ear

	// See: i.stack.imgur.com/oesw9.jpg
	result.elements[0][0] = q / aspectRatio;

	result.elements[1][1] = q;

	result.elements[2][2] = (far + near) / zRange;
	result.elements[2][3] = -1.0f;

	result.elements[3][2] = 2 * (far * near) / zRange;

	return result;
}

mat4 mat4::LookAt(Vec3<float> eye, const Vec3<float> &target, const Vec3<float> &up)
{
	Vec3<float> zaxis = Maths::Normalise(eye - target);
	Vec3<float> xaxis = Maths::Normalise(Maths::Cross(up, zaxis));
	Vec3<float> yaxis = Maths::Cross(zaxis, xaxis);

	mat4 result(1.0f);

	result.elements[0][0] = xaxis.x;
	result.elements[0][1] = yaxis.x;
	result.elements[0][2] = zaxis.x;

	result.elements[1][0] = xaxis.y;
	result.elements[1][1] = yaxis.y;
	result.elements[1][2] = zaxis.y;

	result.elements[2][0] = xaxis.z;
	result.elements[2][1] = yaxis.z;
	result.elements[2][2] = zaxis.z;

	result.elements[3][0] = -Maths::Dot(xaxis, eye);
	result.elements[3][1] = -Maths::Dot(yaxis, eye);
	result.elements[3][2] = -Maths::Dot(zaxis, eye);

	return result;
}

mat4 mat4::Translation(const Vec3<float> &translate)
{
	mat4 result(1.0f);

	result.elements[3][0] = translate.x;
	result.elements[3][1] = translate.y;
	result.elements[3][2] = translate.z;

	return result;
}

mat4 mat4::Rotation(float angle, const Vec3<float> &axis)
{
	mat4 result(1.0f);

	float r = Maths::Radians(angle);
	float c = cos(r);
	float s = sin(r);
	float omc = 1.0f - c;	// (o)ne (m)inus (c)osine

	result.elements[0][0] = axis.x * omc + c;
	result.elements[0][1] = axis.y * axis.x * omc + axis.z * s;
	result.elements[0][2] = axis.x * axis.z * omc - axis.y * s;

	result.elements[1][0] = axis.x * axis.y * omc - axis.z * s;
	result.elements[1][1] = axis.y * omc + c;
	result.elements[1][2] = axis.y * axis.z * omc + axis.x * s;

	result.elements[2][0] = axis.z * axis.x * omc + axis.y * s;
	result.elements[2][1] = axis.y * axis.z * omc - axis.x * s;
	result.elements[2][2] = axis.z * omc + c;

	//result.elements[3][0] = -32.0f;
	//result.elements[3][1] = -32.0f;
	//result.elements[3][2] = 0.0f;

	return result;
}

mat4 mat4::Scale(const Vec3<float> &scale)
{
	mat4 result(1.0f);

	result.elements[0][0] = scale.x;

	result.elements[1][1] = scale.y;

	result.elements[2][2] = scale.z;

	return result;
}