#pragma once

#include<d3d11.h>
#include<d3dcompiler.h>
#include<DirectXMath.h>



#include<string>

using namespace DirectX;

// #pragma comment(lib, "DirectXTK.lib")

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include"WICTextureLoader.h"
#include<wincodec.h>
#pragma comment(lib, "windowscodecs.lib")

#define MAX_VERTICES 6

struct DRECT
{
	double top;
	double bottom;
	double right;
	double left;
};

struct SpriteData
{
	int Width; // Width of sprite in pixels
	int Height; // Height of sprite in pixels
	float X; // location
	float Y;
	float Scale; // <1 smaller, >1 bigger
	float Angle; // Rotation angle in radians
	DRECT Rect; // Used to select an image from a larger texture
	unsigned TextureID; // Pointer to texture
	bool FlipHorizontal;// True to flip sprite horizontally (mirror)
	bool FlipVertical; // True to flip sprite vertically
	bool ClipPixels;

	SpriteData() {};
	SpriteData(int width, int height,
		float x, float y, float scale, float angle,
		DRECT rect, unsigned texID,
		bool flipH, bool flipV, unsigned textureID, bool shouldClip)
		: Width(width), Height(height), X(x), Y(y),
		Scale(scale), Angle(angle),
		Rect(rect), TextureID(texID),
		FlipHorizontal(flipH), FlipVertical(flipV), ClipPixels(shouldClip)
	{};
};


struct VertexPositionTexture    //Overloaded Vertex Structure
{
	VertexPositionTexture() {}
	VertexPositionTexture(float x, float y, float z,
		float u, float v)
		: pos(x, y, z), texCoord(u, v) {}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
};

struct cbPerObject
{
	XMMATRIX WVP;
	XMFLOAT2 Dimensions;
	bool HasTexture;
	bool ShouldClip;
	XMFLOAT4 Color;
};

class cGraphics
{
private:
	HWND m_hWnd;
	unsigned m_Width;
	unsigned m_Height;

	ID3D11Device * m_Device;
	ID3D11DeviceContext* m_Context;
	IDXGISwapChain* m_SwapChain;
	ID3D11RenderTargetView* m_RenderTargetView;

	// Rasterizer States.
	ID3D11RasterizerState* m_RSWireFrame;
	ID3D11RasterizerState* m_RSSolid;

	unsigned m_CurrentVertexCount;
	ID3D11Buffer* m_DynamicVB;
	ID3D11Buffer* m_ConstantBufferPerObject;

	// SHADERS
	ID3D11VertexShader	 *m_VertexShader;
	ID3D11GeometryShader *m_GeometryShader;
	ID3D11PixelShader	 *m_PixelShader;
	ID3DBlob* VS_Buffer;
	ID3DBlob* GS_Buffer;
	ID3DBlob* PS_Buffer;

	ID3D11InputLayout *m_TexturedVertexLayout;

	ID3D11Resource* m_QuadTexture;
	ID3D11ShaderResourceView* m_QuadTextureSRV;
	ID3D11SamplerState* m_QuadTextureSamplerState;

	bool InitializeInputLayout();
	bool InitializeShaders();

	// bool InitializeTexure();

	bool InitializeConstantBuffer();
	void UpdateConstantBuffer(cbPerObject obj);
	void SetConstantBuffer(int startSlot);

	bool InitializeDynamicVertexBuffer();
	void UpdateDynamicVertexBuffer(VertexPositionTexture* verts, int count);
	void SetVertexBuffer(int startSlot);
	
	bool InitScene();

public:
	cGraphics();
	~cGraphics();

	ID3D11Device* GetDevice() { return m_Device; }
	ID3D11DeviceContext* GetContext() { return m_Context; }

	bool Initialize(HWND hWnd, int width, int height, bool windowed);
	void Clear(float color[4]);
	void Update(float deltaTime);
	void Render(SpriteData* sd, const int count);
	void Present();
	void Shutdown();

	void RSWireFrameMode() { m_Context->RSSetState(m_RSWireFrame); }
	void RSSolidMode() { m_Context->RSSetState(nullptr); }

	void OnResize(int newWidth, int newHeight);
};

