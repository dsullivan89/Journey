#include "cTexture.h"



cTexture::cTexture()
{
}


cTexture::~cTexture()
{
}

BOOL cTexture::Load(cGraphics &Graphics, std::wstring fileName)
{
	m_Graphics = &Graphics;
	HRESULT hr;

	// std::wstring fileName = L"braynzar.jpg";

	hr = CreateWICTextureFromFile(m_Graphics->GetDevice(), 
								m_Graphics->GetContext(), 
								fileName.c_str(), 
								&m_Resource,
								&m_SRV);

	if (FAILED(hr))
		return false;

	m_IsLoaded = true;

	ID3D11Texture2D *pTextureInterface = 0;
	m_Resource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
	D3D11_TEXTURE2D_DESC desc;
	pTextureInterface->GetDesc(&desc);

	m_Width = desc.Width;
	m_Height = desc.Height;

	pTextureInterface->Release();

	// Describe the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	hr = m_Graphics->GetDevice()->CreateSamplerState(&sampDesc, &m_SamplerState);
	if (FAILED(hr))
		MessageBox(NULL, "cTexture::Load()", "Error", MB_OK);

	return true;
}

void cTexture::Set()
{
	m_Graphics->GetContext()->PSSetSamplers(0, 1, &m_SamplerState);
	m_Graphics->GetContext()->PSSetShaderResources(0, 1, &m_SRV);
}

void cTexture::Unset()
{
	m_Graphics->GetContext()->PSSetSamplers(0, 0, nullptr);
	m_Graphics->GetContext()->PSSetShaderResources(0, 0, nullptr);
}

void cTexture::Free()
{
	if(m_Resource)
		m_Resource->Release();
	if (m_SRV)
		m_SRV->Release();
	if (m_SamplerState)
		m_SamplerState->Release();

	m_IsLoaded = false;

	return;
}
