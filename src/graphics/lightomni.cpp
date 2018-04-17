#include "lightomni.h"

LightOmni::LightOmni() :
	LightSource()
{
	name = "light_omni";
}

void LightOmni::Attenuate(Shader &shader, const unsigned char index)
{
	shader.SetUniformVec3f("u_lightPosition[" + std::to_string(index) + ']', position);
	shader.SetUniformVec3f("u_lightColour[" + std::to_string(index) + ']', colour);
	shader.SetUniformFloat("u_lightPower[" + std::to_string(index) + ']', power);
}