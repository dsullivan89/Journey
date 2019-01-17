#include "cMap.h"
#include<memory.h>

void cMap::Create(int layerCount)
{
	m_LayerCount = layerCount;

	m_InfoLayers = new char*[layerCount];
	m_Widths = new unsigned[layerCount];
	m_Heights = new unsigned[layerCount];

}

bool cMap::AddLayer(unsigned layerIndex, char * infoLayer, int width, int height)
{
	int area = width * height;
	//infoLayer = new char[area];
	m_InfoLayers[layerIndex] = new char[area];

	memcpy(m_InfoLayers[layerIndex], infoLayer, area);
	m_Heights[layerIndex] = height;
	m_Widths[layerIndex] = width;


	return true;
}

void cMap::GetLayerInfo(unsigned layerIndex, char * info)
{
	int width = m_Widths[layerIndex];
	int height = m_Heights[layerIndex];
	int area = width * height;

	info = new char[area];
	memcpy(info, m_InfoLayers[layerIndex], area);

	return;
}

unsigned cMap::GetLayerWidth(unsigned layerIndex)
{
	return m_Widths[layerIndex];
}

unsigned cMap::GetLayerHeight(unsigned layerIndex)
{
	return m_Heights[layerIndex];
}

void cMap::Destroy()
{
	for (unsigned i = 0; i < m_LayerCount; i++)
		delete[] m_InfoLayers[i];

	delete[] m_InfoLayers;
}
