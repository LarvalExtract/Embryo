#include "text.h"
#include <utilities/console.h>

// Initialise static members
const std::string	Text::defaultFont = "calibri.ttf";
bool				Text::bInitialised = false;
FT_Library			Text::ftl;
FT_Face				Text::fontFace;
std::map<unsigned char, Character*> Text::characterMap;

Text::Text(const std::string &text) :
	Renderable(GL_QUADS)
{
	// Only initialise freetype once
	if (bInitialised != true)
		InitialiseFreeType();

	float xpos = 0;
	float ypos = 0;

	this->text.reserve(text.length());

	Shader *pTextShader = new Shader("basicText");

	SetFont(defaultFont);
	SetFontSize(48);
	LoadGlyphs();

	for (unsigned short i = 0; i < text.length(); i++)
	{
		Sprite *pSprite;
		pSprite = &characterMap[text[i]]->sprite;
		pSprite->SetShader(pTextShader);
		pSprite->position.x = xpos + characterMap[text[i]]->bearing.x;
		pSprite->position.y = ypos - (characterMap[text[i]]->size.y - characterMap[text[i]]->bearing.y);

		this->text.push_back(pSprite);

		xpos += (characterMap[text[i]]->advance);
	}
}

Text::~Text()
{
	for (unsigned int i = 0; i < text.size(); i++)
		if (text[i] != nullptr)
			delete text[i];
}

void Text::SetFont(const std::string &fontPath)
{
	if (FT_New_Face(ftl, ("res/fonts/" + fontPath).c_str(), 0, &fontFace) != 0)
	{
		Console::Log(LogType::Error) << "Couldn't find " << fontPath << "\n";
		return;
	}
}

void Text::SetFontSize(const unsigned char fontSize)
{
	FT_Set_Pixel_Sizes(fontFace, 0, fontSize);
}

void Text::Draw(Camera &camera, Mat4 &cameraMatrix)
{
	for (unsigned int i = 0; i < text.size(); i++)
	{
		text[i]->Draw(camera, cameraMatrix);
	}
}

bool Text::InitialiseFreeType()
{
	if (FT_Init_FreeType(&ftl) != 0)
	{
		Console::Log(LogType::Error) << "Couldn't initialise FreeType.\n";
		return false;
	}

	else
	{
		return Text::bInitialised = true;
	}
}

void Text::LoadGlyphs()
{
	characterMap.clear();

	// Generate sprites for every character in the specified font
	for (unsigned char character = 0; character < 255; character++)
	{
		if (FT_Load_Char(fontFace, character, FT_LOAD_RENDER) != 0)
		{
			Console::Log(LogType::Error) << "Couldn't load glyph " << character << "\n";
			continue;
		}

		Character *pCharSprite = new Character;
		pCharSprite->sprite = Sprite(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows, fontFace->glyph->bitmap.buffer);
		pCharSprite->size = Vec2<int>(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows);
		pCharSprite->bearing = Vec2<int>(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top);
		pCharSprite->advance = fontFace->glyph->advance.x;

		characterMap.insert(std::pair<unsigned char, Character*>(character, pCharSprite));
	}
}
