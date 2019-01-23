#include "cTiles.h"

cTiles::cTiles()
{
}

cTiles::~cTiles()
{
}

bool cTiles::Create(cGraphics* graphics, long textureCount)
{
	m_Graphics = graphics;
	m_TextureCount = textureCount;

	m_Textures = new cTexture[textureCount];
	m_Widths = new short[textureCount];
	m_Heights = new short[textureCount];
	m_Columns = new short[textureCount];
	m_ScaleX = new float[textureCount];
	m_ScaleY = new float[textureCount];

	return false;
}

bool cTiles::Destroy()
{
	for (int i = 0; i < m_TextureCount; i++)
		m_Textures[i].Free();

	delete[] m_Textures;
	delete[] m_Widths;
	delete[] m_Heights;
	delete[] m_Columns;
	delete[] m_ScaleX;
	delete[] m_ScaleY;

	return false;
}

bool cTiles::Load(long textureIndex, std::wstring fileName, short TileWidth, short TileHeight)
{
	m_Textures[textureIndex].Load(*m_Graphics, fileName);
	m_Widths[textureIndex] = TileWidth;
	m_Heights[textureIndex] = TileHeight;
	m_Columns[textureIndex] = m_Textures[textureIndex].GetWidth() / TileWidth;
	m_ScaleX[textureIndex] = 1;
	m_ScaleY[textureIndex] = 1;


	return true;
}

void cTiles::SetTileset(int textureIndex)
{
	m_Textures[textureIndex].Set();
}

bool cTiles::Free(long textureIndex)
{
	if(m_Textures[textureIndex].IsLoaded())
		m_Textures[textureIndex].Free();
	m_Widths[textureIndex] = -1;
	m_Heights[textureIndex] = -1;
	m_Columns[textureIndex] = -1;

	return 1;
}

void cTiles::SetScaleX(long textureIndex, float scaleX)
{
	m_ScaleX[textureIndex] = scaleX;
}

void cTiles::SetScaleY(long textureIndex, float scaleY)
{
	m_ScaleY[textureIndex] = scaleY;
}

int cTiles::GetWidth(long textureIndex)
{
	return m_Widths[textureIndex];
}

int cTiles::GetHeight(long textureIndex)
{
	return m_Heights[textureIndex];
}

DRECT cTiles::GetTile(int textureIndex, int tileIndex)
{
	DRECT rect;

	float texWidth = (float)m_Textures[textureIndex].GetWidth();
	float texHeight = (float)m_Textures[textureIndex].GetHeight();
	float tileWidth = (float)m_Widths[textureIndex];
	float tileHeight = (float)m_Heights[textureIndex];
	int columns = m_Columns[textureIndex];
	//int rows = texHeight / tileHeight;

	rect.top = ((tileIndex / columns) * tileHeight) / texHeight;
	rect.left = ((tileIndex % columns) * tileWidth) / texWidth;
	rect.bottom = rect.top + (tileHeight / texHeight);
	rect.right = rect.left + (tileWidth / texWidth);


	return rect;
}
