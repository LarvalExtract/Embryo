#pragma once

#include <maths/Vec3.h>
#include <maths/Vec4.h>

class LightSource
{
public:
	Vec3<float> GetPosition();
	Vec3<float> GetColour();
	float GetPower();

	std::string GetName();

	void SetPosition(float x, float y, float z);
	void SetColour(float r, float g, float b);
	void SetPower(float value);

	void SetName(const std::string& name);

protected:
	LightSource(const Vec3<float> &position, const Vec3<float> &colour, float brightness);

	Vec3<float> position;
	Vec3<float> colour;

	std::string name;

	float power;
};

class LightOmni : public LightSource
{
public:
	LightOmni(const Vec3<float> &position, const Vec3<float> &colour, float brightness);

private:

};