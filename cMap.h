#pragma once

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

public:
	void Create(int layerCount);
	bool AddLayer(unsigned layerIndex, char* infoLayer, int width, int height);
	void GetLayerInfo(unsigned layerIndex, char* info);
	unsigned GetLayerWidth(unsigned layerIndex);
	unsigned GetLayerHeight(unsigned layerIndex);
	void Destroy();

	cMap() {};
	~cMap() {};
};