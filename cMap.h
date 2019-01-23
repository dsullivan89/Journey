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
	unsigned* m_Widths;
	unsigned* m_Heights;
	unsigned m_LayerCount;

	cTiles m_Tiles;

	long m_ObjectCount; // current number of objects that need to be drawn.
	SpriteData m_Objects[1024];

public:
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