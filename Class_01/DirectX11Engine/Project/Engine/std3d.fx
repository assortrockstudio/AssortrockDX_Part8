#ifndef _STD3D
#define _STD3D

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;        
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
        
    vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    return vOutColor;
}






#endif