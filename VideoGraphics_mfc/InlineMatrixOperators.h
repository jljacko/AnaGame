#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#ifndef _INLINE_MATRIX_OPERATORS
#define _INLINE_MATRIX_OPERATORS


inline DirectX::XMFLOAT4 operator*(DirectX::XMFLOAT4X4& mat, DirectX::XMFLOAT4 f)
{
	DirectX::XMFLOAT4 ret{ 0,0,0,0 };

	for (int c = 0; c < 4; c++)
	{
		ret.x += mat(0,c) * f.x;
		ret.y += mat(1, c) * f.y;
		ret.z += mat(2, c) * f.z;
		ret.w += mat(3, c) * f.w;
	}
	return ret;
}

#endif