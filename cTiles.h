#pragma once

//#include"cTexture.h"


struct sTileInfo
{
	short width;
	short height;
	short columns;
};

class cTiles
{
private:
	long m_NumTextures; // # of textures
	//cTexture *m_Textures; // cTexture array
	short *m_Widths; // Tile widths array
	short *m_Heights; // Tile heights array
	short *m_Columns; // # columns in texture
public:
	cTiles();
	~cTiles();
	// Functions to create and free the tile interface
	//BOOL Create(long NumTextures);
	//BOOL Free();
	// Functions to load and free a single texture
	//BOOL Load(long TextureNum, char *Filename,
		short TileWidth = 0, short TileHeight = 0);
	//BOOL Free(long TextureNum = -1);
	// Functions to retrieve tile dimensions and
	// # of tiles in a texture.
	long GetWidth(long TextureNum);
	long GetHeight(long TextureNum);
	long GetNum(long TextureNum);
	// Draw a single tile to location
};
