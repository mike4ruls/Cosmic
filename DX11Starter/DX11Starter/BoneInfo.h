#pragma once
#include <DirectXMath.h>

struct BoneInfo {
	aiMatrix4x4 BoneOffset;
	aiMatrix4x4 FinalTransformation;

	DirectX::XMFLOAT4X4 ConvertToDX(aiMatrix4x4 mat)
	{
		DirectX::XMFLOAT4X4 newMat = {mat.a1, mat.a2, mat.a3, mat.a4,
									  mat.b1, mat.b2, mat.b3, mat.b4,
									  mat.c1, mat.c2, mat.c3, mat.c4,
									  mat.d1, mat.d2, mat.d3, mat.d4,};

		return newMat;
	}
};