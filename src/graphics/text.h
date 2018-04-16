#pragma once

#include "renderable/renderable.h"
#include "texture2d.h"
#include "renderable/Sprite.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <maths/vec2.h>
#include <map>
#include <vector>

struct Character
{
	Character() {};

	Sprite sprite;
	Vec2<int> size;
	Vec2<int> bearing;
	unsigned int advance;
};

class Text : public Renderable
{
public:
	Text(const std::string &text);
	~Text();

	void SetFont(const std::string &fontPath);
	void SetFontSize(const unsigned char fontSize);

	void Draw(Camera &camera, Mat4 &cameraMatrix) override;

private:
	static bool bInitialised;
	static const std::string defaultFont;

	static bool InitialiseFreeType();
	static void LoadGlyphs();
	
	static FT_Library ftl;
	static FT_Face fontFace;
	static std::map<unsigned char, Character*> characterMap;

	std::vector<Sprite*> text;
};