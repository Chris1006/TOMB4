#pragma once
#include "../global/vars.h"

#define SetD3DViewMatrix	( (void(__cdecl*)()) 0x00490B30  )
#define	D3DTransform	( (void(__cdecl*)(D3DVECTOR*, D3DMATRIX*)) 0x00490E10 )
#define D3DNormalise	( (void(__cdecl*)(D3DVECTOR*)) 0x00490D50 )
#define SetD3DMatrix	( (void(__cdecl*)(D3DMATRIX*, long*)) 0x00490C30 )
#define S_InitD3DMatrix	( (void(__cdecl*)()) 0x00490CF0 )
