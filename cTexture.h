#pragma once

#include"cGraphics.h"

#include"WICTextureLoader.h"
#include<wincodec.h>
#pragma comment(lib, "windowscodecs.lib")

class cTexture
{
protected:
	cGraphics * m_Graphics; // Parent cGraphics
	ID3D11Resource *m_Resource; // Texture COM
	ID3D11ShaderResourceView *m_SRV;
	ID3D11SamplerState* m_SamplerState;
	unsigned long m_Width, m_Height; // Dimensions of texture image.
	bool m_IsLoaded;
public:
	cTexture(); // Constructor
	~cTexture(); // Destructor
	 // Return texture COM
										// Load a texture from file

										// DWORD Transparent = 0

	BOOL Load(cGraphics &Graphics, std::wstring fileName);
	void Set();
	void Unset();

	void Free(); // Free texture object
	BOOL IsLoaded() { return m_IsLoaded; } // Returns TRUE if texture is loaded

	ID3D11ShaderResourceView *GetSRV() { return m_SRV; }
	unsigned GetWidth() { return m_Width; } // Return width (pitch) of texture
	unsigned GetHeight() { return m_Height; }; // Return height of texture


					  // D3DFORMAT GetFormat(); // Return texture storage format

					  // Draw a 2-D portion of texture to device
					  /*BOOL Blit(long DestX, long DestY, \
					  long SrcX = 0, long SrcY = 0, \
					  long Width = 0, long Height = 0, \
					  float XScale = 1.0f, float YScale = 1.0f, \
					  D3DCOLOR Color = 0xFFFFFFFF);*/
};
