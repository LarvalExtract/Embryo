#pragma once
#include <string>
#include <utilities/import_bitmap.h>

class Texture
{
public:
	void Bind(unsigned int unit);

	inline unsigned short GetWidth() { return width; }
	inline unsigned short GetHeight() { return height; }
	inline unsigned char GetBitDepth() { return bpp; }

	void SetSpecularity(float value);
	void SetGlossiness(float value);

	float GetSpecularity();
	float GetGlossiness();

protected:
	Texture() : specularity(1), glossiness(10) {};

	unsigned int hTexture;
	unsigned int target;

	unsigned short width;
	unsigned short height;
	unsigned char  bpp;

	float specularity;
	float glossiness;
};

class Texture2D : public Texture
{
public:
	Texture2D();
	Texture2D(const std::string& filePath);
	~Texture2D();

	void Init(const std::string& filePath);
private:

};

class Cubemap : public Texture
{
public:
	Cubemap();
	Cubemap(const std::string& filePath);
	~Cubemap();

	void Init(const std::string& filePath);

	void SetCubemapFace(unsigned int face, const char*& imageData);
private:

};