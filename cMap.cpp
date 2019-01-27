#include "cMap.h"
#include<memory.h>

void cMap::Create(int layerCount)
{
	m_LayerCount = layerCount;
	m_InfoLayers = new char*[layerCount];
	m_LayerWidths = new unsigned[layerCount];
	m_LayerHeights = new unsigned[layerCount];

}

bool cMap::AddLayer(unsigned layerIndex, char * infoLayer, int width, int height)
{
	int area = width * height;
	m_InfoLayers[layerIndex] = new char[area];

	memcpy(m_InfoLayers[layerIndex], infoLayer, area);
	m_LayerHeights[layerIndex] = height;
	m_LayerWidths[layerIndex] = width;


	return true;
}

void cMap::GetLayerInfo(unsigned layerIndex, char * info)
{
	int width = m_LayerWidths[layerIndex];
	int height = m_LayerHeights[layerIndex];
	int area = width * height;

	memcpy(info, m_InfoLayers[layerIndex], area);

	return;
}

unsigned cMap::GetLayerWidth(unsigned layerIndex)
{
	return m_LayerWidths[layerIndex];
}

unsigned cMap::GetLayerHeight(unsigned layerIndex)
{
	return m_LayerHeights[layerIndex];
}

unsigned cMap::GetEmptySpacesInLayer(unsigned layerIndex)
{
	unsigned emptySpaceCount = 0;
	int area = m_LayerWidths[layerIndex] * m_LayerHeights[layerIndex];

	for (int i = 0; i < area; i++)
		if (m_InfoLayers[layerIndex][i] == -1)
			emptySpaceCount++;

	return emptySpaceCount;
}

void cMap::Destroy()
{
	for (unsigned i = 0; i < m_LayerCount; i++)
		delete[] m_InfoLayers[i];

	delete[] m_InfoLayers;
}