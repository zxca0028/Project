#pragma once

BEGIN(Client)

typedef signed char		    _byte;
typedef unsigned char		_ubyte;

typedef bool				_bool;

typedef wchar_t				_tchar;

typedef signed short        _short;
typedef unsigned short      _ushort;

typedef signed int			_int;
typedef unsigned int		_uint;

typedef signed long			_long;
typedef unsigned long		_ulong;

typedef float				_float;
typedef double				_double;

typedef D3DXVECTOR2		    _float2;
typedef D3DXVECTOR3		    _float3;
typedef D3DXVECTOR4		    _float4;

typedef D3DXMATRIX		    _matrix;

typedef struct tagVertexTexture
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR2 vTexUV;

}VTXTEX;

typedef struct tagVertexPointTexture
{
	D3DXVECTOR3		vPosition;
	float			fSize;
	D3DXVECTOR2		vTexUV;

}VTXPOINT;

typedef struct tagVertexMatrix
{
	D3DXVECTOR4		vRight;
	D3DXVECTOR4		vUp;
	D3DXVECTOR4		vLook;
	D3DXVECTOR4		vPosition;

}VTXMATRIX;

END