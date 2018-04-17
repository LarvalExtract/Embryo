#include "texture.h"
#include <utilities/console.h>

Texture::Texture(unsigned int textureTarget) :
	hTexture(0),
	target(textureTarget),
	width(0),
	height(0),
	bitsPerPixel(0)
{

}

Texture::~Texture()
{
	glBindTexture(target, 0);
	glDeleteTextures(1, &hTexture);
}

void Texture::Bind(unsigned int unit)
{
	if (unit < 0 || unit > 31)
		unit = 0;

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(target, hTexture);
}

void Texture::SetParameterInt(unsigned int parameter, int value)
{
	glTexParameteri(target, parameter, value);
}

void Texture::SetParameterFloat(unsigned int parameter, float value)
{
	glTexParameterf(target, parameter, value);
}