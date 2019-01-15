#include "cGraphics.h"
#include <sstream>
#include<vector>


bool cGraphics::InitializeInputLayout()
{
	HRESULT hr;

	D3D11_INPUT_ELEMENT_DESC texturedVertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT nDescriptors = ARRAYSIZE(texturedVertexLayout);



	hr = m_Device->CreateInputLayout(texturedVertexLayout, nDescriptors, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &m_TexturedVertexLayout);
	if (FAILED(hr))
	{
		std::ostringstream os;
		os << std::hex << (LONG)hr;
		MessageBox(NULL, os.str().c_str(), "Input Layout Result Code:", MB_SETFOREGROUND);
		return false;
	}

	m_Context->IASetInputLayout(m_TexturedVertexLayout);
	m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

bool cGraphics::InitializeShaders()
{
	HRESULT hr;

	if (FAILED(hr = D3DReadFileToBlob(L"VertexShader.cso", &VS_Buffer)))
	{
		std::ostringstream os;
		os << (LONG)hr;
		MessageBox(NULL, os.str().c_str(), "Vertex Shader Result:", MB_SETFOREGROUND);
		return false;
	}
	if (FAILED(hr = m_Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &m_VertexShader)))
	{
		return false;
	}

	if (FAILED(hr = D3DReadFileToBlob(L"GeometryShader.cso", &GS_Buffer)))
	{
		std::ostringstream os;
		os << (LONG)hr;
		MessageBox(NULL, os.str().c_str(), "Geometry Shader Result:", MB_SETFOREGROUND);
		return false;
	}
	if (FAILED(hr = m_Device->CreateGeometryShader(GS_Buffer->GetBufferPointer(), GS_Buffer->GetBufferSize(), NULL, &m_GeometryShader)))
	{
		return false;
	}

	if (FAILED(hr = D3DReadFileToBlob(L"PixelShader.cso", &PS_Buffer)))
	{
		std::ostringstream os;
		os << (LONG)hr;
		MessageBox(NULL, os.str().c_str(), "Pixel Shader Result:", MB_SETFOREGROUND);
		return false;
	}
	if (FAILED(hr = m_Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &m_PixelShader)))
	{
		return false;
	}

	m_Context->VSSetShader(m_VertexShader, 0, 0);
	// m_Context->GSSetShader(m_GeometryShader, 0, 0);
	m_Context->PSSetShader(m_PixelShader, 0, 0);

	return true;
}

//bool cGraphics::InitializeTexure()
//{
//	HRESULT hr;
//
//	std::wstring fileName = L"braynzar.jpg";
//
//	hr = CreateWICTextureFromFile(m_Device, m_Context, fileName.c_str(), &m_QuadTexture,
//		&m_QuadTextureSRV);
//	
//	if(FAILED(hr))
//		return false;
//
//	// Describe the Sample State
//	D3D11_SAMPLER_DESC sampDesc;
//	ZeroMemory(&sampDesc, sizeof(sampDesc));
//	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	sampDesc.MinLOD = 0;
//	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
//
//	//Create the Sample State
//	hr = m_Device->CreateSamplerState(&sampDesc, &m_QuadTextureSamplerState);
//	if (FAILED(hr))
//		MessageBox(m_hWnd, "Graphics::InitializeTexture()", "Error", MB_OK);
//
//	m_Context->PSSetSamplers(0, 1, &m_QuadTextureSamplerState);
//	m_Context->PSSetShaderResources(0, 1, &m_QuadTextureSRV);
//
//	return true;
//}

bool cGraphics::InitializeConstantBuffer()
{
	cbPerObject obj;
	obj.Dimensions = { 1.0f, 1.0f };
	obj.WVP = XMMatrixIdentity();
	obj.HasTexture = false;
	obj.Color = { 1.0f, 0.0f, 0.0f, 1.0f };

	HRESULT hr;
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.MiscFlags = 0;
	cbbd.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &obj;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// This validates our function input.
	hr = m_Device->CreateBuffer(&cbbd, &InitData, NULL);
	if (hr != S_FALSE)
		return false;

	// if we got this far then we know we passed the right arguments.
	hr = m_Device->CreateBuffer(&cbbd, &InitData, &m_ConstantBufferPerObject);
	if (FAILED(hr))
		return false;

	return true;
}

void cGraphics::UpdateConstantBuffer(cbPerObject obj)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//  Disable GPU access to the vertex buffer data.
	hr = m_Context->Map(m_ConstantBufferPerObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//  Update the vertex buffer here.
	memcpy(mappedResource.pData, &obj, sizeof(obj));
	//  Reenable GPU access to the vertex buffer data.
	m_Context->Unmap(m_ConstantBufferPerObject, 0);

	m_Context->GSSetConstantBuffers(0, 1, &m_ConstantBufferPerObject);
}

void cGraphics::SetConstantBuffer(int startSlot)
{
	m_Context->PSSetConstantBuffers(0, 1, &m_ConstantBufferPerObject);
}

bool cGraphics::InitializeDynamicVertexBuffer()
{
	HRESULT hr;

	VertexPositionTexture vertices[1000];

	m_CurrentVertexCount = 6;

	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VertexPositionTexture) * 1000;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	hr = m_Device->CreateBuffer(&bufferDesc, &InitData, &m_DynamicVB);
	if (FAILED(hr))
		return false;

	return true;
}

void cGraphics::UpdateDynamicVertexBuffer(VertexPositionTexture* verts, int count)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//  Disable GPU access to the vertex buffer data.
	m_Context->Map(m_DynamicVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//  Update the vertex buffer here.
	memcpy(mappedResource.pData, verts, sizeof(VertexPositionTexture) * 1000);
	//  Reenable GPU access to the vertex buffer data.
	m_Context->Unmap(m_DynamicVB, 0);
}

void cGraphics::SetVertexBuffer(int startSlot)
{
	UINT stride = sizeof(VertexPositionTexture);
	UINT offset = 0;
	m_Context->IASetVertexBuffers(startSlot, 1, &m_DynamicVB, &stride, &offset);
}

cGraphics::cGraphics()
{
	m_CurrentVertexCount = 0;
}


cGraphics::~cGraphics()
{
}

bool cGraphics::Initialize(HWND hWnd, int width, int height, bool windowed)
{
	CoInitialize(NULL);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = windowed;

	const D3D_FEATURE_LEVEL lvl[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1 };
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


	m_Device = nullptr;
	m_Context = nullptr;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, lvl, _countof(lvl), D3D11_SDK_VERSION, &sd, &m_SwapChain, &m_Device, &FeatureLevelsSupported, &m_Context);
	// Can we use the selected DirectX version?
	if (hr == E_INVALIDARG)
	{
		hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, &lvl[1], _countof(lvl) - 1, D3D11_SDK_VERSION, &sd, &m_SwapChain, &m_Device, &FeatureLevelsSupported, &m_Context);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Failed to create the device", "Device Creation Error", MB_OK);
			return false;
		}

	}

	m_hWnd = hWnd;


	ID3D11Texture2D* pBackBuffer;
	// Get a pointer to the back buffer
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "SwapChain::GetBuffer() failed", "Graphics::Initialize()", MB_OK);
	}

	// Create a render-target view
	hr = m_Device->CreateRenderTargetView(pBackBuffer, NULL,
		&m_RenderTargetView);
	if (FAILED(hr))
		MessageBox(hWnd, "Device::CreateRenderTargetView()", "Graphics::Initialize()", MB_OK);

	pBackBuffer->Release();


	// Bind the view
	m_Context->OMSetRenderTargets(1, &m_RenderTargetView, nullptr);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_Context->RSSetViewports(1, &vp);

	bool result;
	result = InitializeShaders();
	if (!result)
		return false;

	result = InitializeInputLayout();
	if (!result)
		return false;

	result = InitializeDynamicVertexBuffer();
	if (!result)
		return false;

	result = InitializeConstantBuffer();
	if (!result)
		return false;

	/*result = InitializeTexure();
	if (!result)
		return false;*/

	SetVertexBuffer(0);
	SetConstantBuffer(0);

	m_Width = width;
	m_Height = height;

	return true;
}

void cGraphics::Clear(float color[4])
{
	m_Context->ClearRenderTargetView(m_RenderTargetView, color);
}

void cGraphics::Update(float deltaTime)
{
}

void cGraphics::Render(SpriteData* sd, int count)
{
	m_CurrentVertexCount = count * 6;
	//VertexPositionTexture* vertices = new VertexPositionTexture[m_CurrentVertexCount]; // count = vertex count.
	float ar = (float)m_Width / (float)m_Height; // use later... maybe.
	//int vIndex = 0; // index of the vertex array.
	std::vector<VertexPositionTexture> vertList;

	for (int sprite_i = 0; sprite_i < count; sprite_i++)
	{
		// Vertex positions:
		//		Normalized dimensions and position.

		float nWidth = (float)sd[sprite_i].Width / float(m_Width / 2);     // n implies normalized to the [-1,1] range.
		float nHeight = (float)sd[sprite_i].Height / float(m_Height / 2);
		float nPosX = sd[sprite_i].X / float(m_Width / 2);        // position in [-1,1] range
		float nPosY = sd[sprite_i].Y / float(m_Height / 2);

		float Ax = nPosX - (nWidth / 2);
		float Ay = nPosY + (nHeight / 2);

		float Bx = nPosX + (nWidth / 2);
		float By = nPosY + (nHeight / 2);

		float Cx = nPosX - (nWidth / 2);
		float Cy = nPosY - (nHeight / 2);

		float Dx = nPosX + (nWidth / 2);
		float Dy = nPosY - (nHeight / 2);

		bool flipH = sd[sprite_i].FlipHorizontal;

		// Texture coords:
		float x1 = flipH ? (float)sd[sprite_i].Rect.right : (float)sd[sprite_i].Rect.left;
		float x2 = flipH ? (float)sd[sprite_i].Rect.left : (float)sd[sprite_i].Rect.right;
		float y1 = (float)sd[sprite_i].Rect.top;
		float y2 = (float)sd[sprite_i].Rect.bottom;

		float z = 0.0f;

		VertexPositionTexture vertA = { Ax, Ay, z, x1, y1 };
		VertexPositionTexture vertB = { Bx, By, z, x2, y1 };
		VertexPositionTexture vertC = { Cx, Cy, z, x1, y2 };
		VertexPositionTexture vertD = { Dx, Dy, z, x2, y2 };
		//VertexPositionTexture vertC = { Cx, Cy, z, x1, y2 };
		//VertexPositionTexture vertB = { Bx, By, z, x2, y1 };

		vertList.push_back(vertA);
		vertList.push_back(vertB);
		vertList.push_back(vertC);
		vertList.push_back(vertD);
		vertList.push_back(vertC);
		vertList.push_back(vertB);

		//VertexPositionTexture vertices[] =
		//{
		//	{ Ax, Ay, z, x1, y1 }, // A
		//	{ Bx, By, z, x2, y1 }, // B
		//	{ Cx, Cy, z, x1, y2 }, // C
		//	{ Dx, Dy, z, x2, y2 }, // D
		//	{ Cx, Cy, z, x1, y2 }, // C
		//	{ Bx, By, z, x2, y1 }  // B 
		//};

		
	}

	UpdateDynamicVertexBuffer(&vertList[0], vertList.size());
	SetVertexBuffer(0);

	m_Context->Draw(vertList.size(), 0);
	

	//VertexPositionTexture vertices[] =
	//{
	//	{ -1.0f,  1.0f, 0.0f, 0.0f, 0.0f }, // A
	//	{ 0.0f,  1.0f, 0.0f, 1.0f, 0.0f }, // B
	//	{ -1.0f,  0.0f, 0.0f, 0.0f, 1.0f }, // C
	//	{ 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }, // D
	//	{ -1.0f,  0.0f, 0.0f, 1.0f, 0.0f }, // C
	//	{ 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }  // B 
	//};

	// m_Context->Draw(m_CurrentVertexCount, 0);
}

void cGraphics::Present()
{
	m_SwapChain->Present(0, 0);
}

void cGraphics::Shutdown()
{
	if (m_Device)
		m_Device->Release();
	if (m_Context)
		m_Context->Release();
	if (m_SwapChain)
		m_SwapChain->Release();
	if (m_RenderTargetView)
		m_RenderTargetView->Release();
	if (m_DynamicVB)
		m_DynamicVB->Release();
	if (m_ConstantBufferPerObject)
		m_ConstantBufferPerObject->Release();
	if (m_GeometryShader)
		m_GeometryShader->Release();
	if (m_PixelShader)
		m_PixelShader->Release();
	if (m_VertexShader)
		m_VertexShader->Release();
	if (PS_Buffer)
		PS_Buffer->Release();
	if (GS_Buffer)
		GS_Buffer->Release();
	if (VS_Buffer)
		VS_Buffer->Release();
	if (m_TexturedVertexLayout)
		m_TexturedVertexLayout->Release();
	if (m_QuadTexture)
		m_QuadTexture->Release();
	if (m_QuadTextureSRV)
		m_QuadTextureSRV->Release();
	if (m_QuadTextureSamplerState)
		m_QuadTextureSamplerState->Release();

	CoUninitialize();

	return;
}

void cGraphics::OnResize(int newWidth, int newHeight)
{
	if (m_Context) // not null
	{
		m_Context->OMSetRenderTargets(0, 0, 0);

		// Release all outstanding references to the swap chain's buffers.
		m_RenderTargetView->Release();

		HRESULT hr;
		// Preserve the existing buffer count and format.
		// Automatically choose the width and height to match the client rect for HWNDs.
		hr = m_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0); // DXGI_FORMAT_UNKNOWN

																				 // Perform error handling here!

																				 // Get buffer and create a render-target-view.
		ID3D11Texture2D* pBuffer;
		hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			(void**)&pBuffer);
		// Perform error handling here!

		hr = m_Device->CreateRenderTargetView(pBuffer, NULL,
			&m_RenderTargetView);
		// Perform error handling here!
		pBuffer->Release();

		m_Context->OMSetRenderTargets(1, &m_RenderTargetView, NULL);

		// Set up the viewport.
		D3D11_VIEWPORT vp;
		vp.Width = (float)newWidth;
		vp.Height = (float)newHeight;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_Context->RSSetViewports(1, &vp);
	}

	m_Width = newWidth;
	m_Height = newHeight;

}
