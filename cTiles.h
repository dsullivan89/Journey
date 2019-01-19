#pragma once

#include"cTexture.h"


struct sTileInfo
{
	short width;
	short height;
	short columns;
};

class cTiles
{
private:
	cGraphics* m_Graphics;
	long m_TextureCount; // # of textures
	cTexture *m_Textures; // cTexture array
	short *m_Widths; // Tile widths array
	short *m_Heights; // Tile heights array
	short *m_Columns; // # columns in texture
public:
	cTiles();
	~cTiles();
	// Functions to create and free the tile interface
	bool Create(cGraphics* graphics, long NumTextures);
	bool Destroy();
	// Functions to load and free a single texture
	bool Load(long textureIndex, std::wstring fileName,
		short TileWidth = 0, short TileHeight = 0);
	void SetTileset(int tileIndex);
	// free an individual texture.
	bool Free(long textureIndex = -1);
	// Functions to retrieve tile dimensions and
	// # of tiles in a texture.
	int GetWidth(long textureIndex);
	int GetHeight(long textureIndex);
	DRECT GetTile(int textureIndex, int tileIndex);
	// Draw a single tile to location
};
