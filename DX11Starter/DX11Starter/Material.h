#pragma once
#include <DirectXMath.h>
class Material {

public:
	Material();
	~Material();
	enum MatType {
		Opaque,
		Transulcent
	}materialType;
	DirectX::XMFLOAT4 surfaceColor;
	unsigned int translucentID;

private:

};
