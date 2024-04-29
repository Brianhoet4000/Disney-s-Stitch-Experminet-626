#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	const char identifier0{ pReader->Read<char>() };
	const char identifier1{ pReader->Read<char>() };
	const char identifier2{ pReader->Read<char>() };
	if (identifier0 != 'B' && identifier1 != 'M' && identifier2 != 'F')
	{
		Logger::LogError(LogString{ L"SpriteFontLoader::LoadContent > Not a valid .fnt font" });
		return nullptr;
	}

	const char identifier3{ pReader->Read<char>() };
	constexpr char curVersion{ 3 };
	if (identifier3 != curVersion)
	{
		Logger::LogError(LogString{ L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported" });
		return nullptr;
	}
	
	SpriteFontDesc fontDesc{};
	
	char blockId{ pReader->Read<char>() };
	int blockSize{ pReader->Read<int>() };

	fontDesc.fontSize = pReader->Read<short>();
	
	constexpr int bytesUntilFontName{ 12 };
	pReader->MoveBufferPosition(bytesUntilFontName);

	fontDesc.fontName = pReader->ReadNullString();
	
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();

	constexpr int bytesUntilTextureSize{ 4 };
	pReader->MoveBufferPosition(bytesUntilTextureSize);

	fontDesc.textureWidth = pReader->Read<unsigned short>();
	fontDesc.textureHeight = pReader->Read<unsigned short>();

	const unsigned short pageCount{ pReader->Read<unsigned short>() };
	if (pageCount > 1)
	{
		Logger::LogError(LogString{ L"SpriteFontLoader::LoadContent > Only one texture per font is allowed" });
		return nullptr;
	}

	constexpr int bytesUntilBlock2{ 5 };
	pReader->MoveBufferPosition(bytesUntilBlock2);

	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();

	const std::wstring pageName{ pReader->ReadNullString() };

	fontDesc.pTexture = ContentManager::Load<TextureData>(loadInfo.assetFullPath.parent_path().append(pageName).wstring());

	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();

	constexpr int charInfoSize{ 20 };
	const int numChars{ blockSize / charInfoSize };

	for (int i{}; i < numChars; ++i)
	{
		const unsigned int charId{ pReader->Read<unsigned int>() };

		FontMetric character{};

		character.character = static_cast<wchar_t>(charId);

		const unsigned short xPos{ pReader->Read<unsigned short>() };
		const unsigned short yPos{ pReader->Read<unsigned short>() };

		character.width = pReader->Read<unsigned short>();
		character.height = pReader->Read<unsigned short>();
		character.offsetX = pReader->Read<unsigned short>();
		character.offsetY = pReader->Read<unsigned short>();
		character.advanceX = pReader->Read<unsigned short>();
		character.page = pReader->Read<unsigned char>();

		const unsigned char channelBitField{ pReader->Read<unsigned char>() };
		switch (channelBitField)
		{
		case 0x1:
			character.channel = 2;
			break;
		case 0x2:
			character.channel = 1;
			break;
		case 0x4:
			character.channel = 0;
			break;
		case 0x8:
			character.channel = 4;
			break;
		case 0xF:
		default:
			character.channel = 0;
		}
		//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
		character.texCoord = XMFLOAT2
		{
			static_cast<float>(xPos) / fontDesc.textureWidth,
			static_cast<float>(yPos) / fontDesc.textureWidth
		};

		//> Insert new FontMetric to the metrics [font.metrics] map
		//	> key = (wchar_t) charId
		//	> value = new FontMetric
		fontDesc.metrics[character.character] = character;
	}

	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
