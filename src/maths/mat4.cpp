#include "mat4.h"
#include "math.h"

// New matrix stuff
Mat4::Mat4(float scalar) :
	elements{ 0 }
{
	for (char i = 0; i < columnSize; i++)
		elements[i][i] = scalar;
}

void Mat4::Identity()
{
	for (char i = 0; i < columnSize; i++)
		elements[i][i] = 1.0f;
}


void Mat4::Translate(const Vec3<float> &translate)
{
	elements[3][0] = translate.x;
	elements[3][1] = translate.y;
	elements[3][2] = translate.z;
}


void Mat4::Rotate(float angle, const Vec3<float> &axis)
{
	float R = Maths::Radians(angle);
	float cosR = cos(R);
	float sinR = sin(R);
	float omc = 1.0f - cosR;	// (o)ne (m)inus (c)osine

	elements[0][0] = axis.x * omc + cosR;
	elements[0][1] = axis.y * axis.x * omc + axis.z * sinR;
	elements[0][2] = axis.x * axis.z * omc - axis.y * sinR;

	elements[1][0] = axis.x * axis.y * omc - axis.z * sinR;
	elements[1][1] = axis.y * omc + cosR;
	elements[1][2] = axis.y * axis.z * omc + axis.x * sinR;

	elements[2][0] = axis.z * axis.x * omc + axis.y * sinR;
	elements[2][1] = axis.y * axis.z * omc - axis.x * sinR;
	elements[2][2] = axis.z * omc + cosR;
}


void Mat4::Scale(const Vec3<float> &scale)
{
	elements[0][0] = scale.x;

	elements[1][1] = scale.y;

	elements[2][2] = scale.z;
}


void Mat4::LookAt(Vec3<float> pos, const Vec3<float> &target, const Vec3<float> &up)
{
	Vec3<float> zaxis = Maths::Normalise(pos - target);
	Vec3<float> xaxis = Maths::Normalise(Maths::Cross(up, zaxis));
	Vec3<float> yaxis = Maths::Cross(zaxis, xaxis);

	elements[0][0] = xaxis.x;
	elements[0][1] = yaxis.x;
	elements[0][2] = zaxis.x;
	
	elements[1][0] = xaxis.y;
	elements[1][1] = yaxis.y;
	elements[1][2] = zaxis.y;
	
	elements[2][0] = xaxis.z;
	elements[2][1] = yaxis.z;
	elements[2][2] = zaxis.z;
	
	elements[3][0] = -Maths::Dot(xaxis, pos);
	elements[3][1] = -Maths::Dot(yaxis, pos);
	elements[3][2] = -Maths::Dot(zaxis, pos);
}


void Mat4::Orthographic(float left, float right, float bottom, float top, float near, float far)
{
	float rml = right - left;	// (r)ight (m)inus (l)eft
	float tmb = top - bottom;	// (t)op (m)inus (b)ottom
	float fmn = far - near;		// (f)ar (m)inus (n)ear

	float rpl = right + left;	// (r)ight (p)lus (l)eft
	float tpb = top + bottom;	// (t)op (p)lus (b)ottom
	float fpn = far + near;		// (f)ar (p)lus (n)ear

	// See: www.safaribooksonline.com/library/view/opengl-es-2/9781941222560/images/AspectRatio/OrthographicMatrixDefinition.png
	elements[0][0] = 2.0f / rml;

	elements[1][1] = 2.0f / tmb;

	elements[2][2] = -2.0f / fmn;

	elements[3][0] = -(rpl / rml);
	elements[3][1] = -(tpb / tmb);
	elements[3][2] = -(fpn / fmn);
}


void Mat4::Perspective(float fov, float aspectRatio, float near, float far)
{
	float q = 1.0f / tanf(Maths::Radians(0.5f * fov));
	float zRange = near - far;

	// See: i.stack.imgur.com/oesw9.jpg
	elements[0][0] = q / aspectRatio;

	elements[1][1] = q;

	elements[2][2] = (far + near) / zRange;
	elements[2][3] = -1.0f;

	elements[3][2] = 2 * (far * near) / zRange;
}

void Mat4::NullTranslate()
{
	elements[3][0] = 0.0f;
	elements[3][1] = 0.0f;
	elements[3][2] = 0.0f;
}

// Operator overloads
Mat4& Mat4::operator=(const Mat4 &rhs)
{
	for (char c = 0; c < columnSize; c++)
		for (char r = 0; r < rowSize; r++)
			elements[c][r] = rhs.elements[c][r];
	return *this;
}

Mat4& Mat4::operator+=(const Mat4 &rhs)
{
	for (char c = 0; c < columnSize; c++)
		for (char r = 0; r < rowSize; r++)
			elements[c][r] += rhs.elements[c][r];
	return *this;
}

Mat4& Mat4::operator-=(const Mat4 &rhs)
{
	for (char c = 0; c < columnSize; c++)
		for (char r = 0; r < rowSize; r++)
			elements[c][r] -= rhs.elements[c][r];
	return *this;
}

Mat4& Mat4::operator*=(const Mat4 &rhs)
{
	for (char m = 0; m < columnSize; m++)
		for (char c = 0; c < columnSize; c++)
			for (char r = 0; r < rowSize; r++)
				elements[m][c] += elements[m][r] * rhs.elements[r][c];
	return *this;
}

// Non-member overloads
Mat4 operator+(Mat4 lhs, const Mat4 &rhs)
{
	for (char c = 0; c < 4; c++)
		for (char r = 0; r < 4; r++)
			lhs.elements[c][r] += rhs.elements[c][r];

	return lhs;
}

Mat4 operator-(Mat4 lhs, const Mat4 &rhs)
{
	for (char c = 0; c < 4; c++)
		for (char r = 0; r < 4; r++)
			lhs.elements[c][r] -= rhs.elements[c][r];

	return lhs;
}

Mat4 operator*(Mat4 lhs, const Mat4 &rhs)
{
	Mat4 result(0.0f);

	for (char m = 0; m < 4; m++)
		for (char c = 0; c < 4; c++)
			for (char r = 0; r < 4; r++)
				result.elements[m][c] += lhs.elements[m][r] * rhs.elements[r][c];

	return result;
}