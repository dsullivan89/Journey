#pragma once

#include"cTiles.h"

enum eLayers
{
	Terrain,
	Objects,
	Walls
};

// This map holds a list of layers and data can be retrieved from a specified layer.
class cMap
{
private:
	char**    m_InfoLayers;
	unsigned* m_LayerWidths;
	unsigned* m_LayerHeights;
	unsigned* m_LayerTileWidth;
	unsigned* m_LayerTileHeight;
	unsigned m_LayerCount;

public:
	void GetTileClicked(int mouseX, int mouseY, float& gridX, float& gridY, int halfTileWidth, int halfTileHeight);
	void Create(int layerCount);
	bool AddLayer(unsigned layerIndex, char* infoLayer, int width, int height);
	void GetLayerInfo(unsigned layerIndex, char* info);
	unsigned GetLayerWidth(unsigned layerIndex);
	unsigned GetLayerHeight(unsigned layerIndex);
	unsigned GetLayerCount() { return m_LayerCount; }
	unsigned GetEmptySpacesInLayer(unsigned layerIndex);
	void Destroy();

	cMap() {};
	~cMap() {};
};