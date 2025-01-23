#ifndef _STD3D_DEFERRED
#define _STD3D_DEFERRED

#include "value.fx"


struct VS_IN
{
	float3 vPos : POSITION;
	float2 vUV  : TEXCOORD;	
};

struct VS_OUT
{
	float4 vPosition : SV_Position;
	float2 vUV : TEXCOOR;
};


VS_OUT VS_Std3D_Deferred(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;
	
	output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
	output.vUV = _in.vUV;
	
	return output;
}


struct PS_OUT
{
	float4 vColor		: SV_Target0;
	float4 vNormal		: SV_Target1;
	float4 vPostion		: SV_Target2;
	float4 vEmissive	: SV_Target3;
	float4 vData		: SV_Target4;
};


PS_OUT PS_Std3D_Deferred(VS_OUT _in)
{
	PS_OUT output = (PS_OUT) 0.f;

	output.vColor		= float4(1.f, 0.f, 0.f, 1.f);
	output.vNormal		= float4(0.f, 1.f, 0.f, 1.f);
	output.vPostion		= float4(0.f, 0.f, 1.f, 1.f);
	output.vEmissive	= float4(1.f, 1.f, 0.f, 1.f);
	output.vData		= float4(1.f, 0.f, 1.f, 1.f);
		
	return output;
}

#endif