#pragma once

#include "lightsource.h"

class LightOmni : public LightSource
{
public:
	LightOmni();

	void Attenuate(Shader &shader, const unsigned char index) override;

private:
	
};