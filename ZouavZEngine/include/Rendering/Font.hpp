#pragma once

#include "Resource.hpp"
#include "Rendering/Texture.hpp"

#include <string>
#include <unordered_map>

class Character
{
private:
	int id;
	float xTextureCoord;
	float yTextureCoord;
	float xMaxTextureCoord;
	float yMaxTextureCoord;
	float xOffset;
	float yOffset;
	float sizeX;
	float sizeY;
	float xAdvance;

public:
	Character(int _id, float _xTextureCoord, float _yTextureCoord, float _xTexSize, float _yTexSize,
		float _xOffset, float _yOffset, float _sizeX, float _sizeY, float _xAdvance);

	int GetId() const { return id; }
	float GetxTextureCoord() const { return xTextureCoord; }
	float GetyTextureCoord() const { return yTextureCoord; }
	float GetXMaxTextureCoord() const { return xMaxTextureCoord; }
	float GetYMaxTextureCoord() const { return yMaxTextureCoord; }
	float GetxOffset() const { return xOffset; }
	float GetyOffset() const { return yOffset; }
	float GetSizeX() const { return sizeX; }
	float GetSizeY() const { return sizeY; }
	float GetxAdvance() const { return xAdvance; }
};

class MetaData
{
private:
	static int PAD_TOP;
	static int PAD_LEFT;
	static int PAD_BOTTOM;
	static int PAD_RIGHT;

	static int DESIRED_PADDING;

	float verticalPerPixelSize;
	float horizontalPerPixelSize;
	float spaceWidth;
	int padding[4];
	int paddingWidth;
	int paddingHeight;

	std::unordered_map<int, Character> metaData;

	bool processNextLine();
	void loadCharacter(const std::string& line, int _imageSize);

public:
	MetaData(const char* _path);
	
	float GetSpaceWidth() const { return spaceWidth; }

	const Character* GetCharacter(int _ascii) const;
};

class Font : public Resource
{
	friend class Engine;
private:
	MetaData metaData;
	Texture texture;
public:
	Font(const std::string& _name, const char* _path);
	~Font() = default;

	void RemoveFromResourcesManager() override;

	const MetaData& GetMetaData() const { return metaData; }
	const Texture& GetTexture() const { return texture; }
};

