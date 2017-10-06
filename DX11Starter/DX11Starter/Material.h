#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

struct Material {

public:
	enum MatType {
		Opaque,
		Transulcent
	}materialType;
	DirectX::XMFLOAT4 surfaceColor;
	unsigned int translucentID;
	bool hasSurText;
	bool hasNorText;

	void LoadSurfaceTexture(ID3D11ShaderResourceView* text)
	{
		surfaceTexture = text;
		hasSurText = true;
	}
	void LoadNormalTexture(ID3D11ShaderResourceView* text)
	{
		normalTexture = text;
		hasNorText = true;
	}

	ID3D11ShaderResourceView* GetSurfaceTexture()
	{
		return surfaceTexture;
	}
	ID3D11ShaderResourceView* GetNormalTexture()
	{
		return normalTexture;
	}

	void FlushSurfaceTexture() 
	{
		hasSurText = false;
	}
	void FlushNormalTexture()
	{
		hasNorText = false;
	}

private:
	ID3D11ShaderResourceView* surfaceTexture = nullptr;
	ID3D11ShaderResourceView* normalTexture = nullptr;
};
