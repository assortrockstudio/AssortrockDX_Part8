#ifndef _STD3D
#define _STD3D

#include "value.fx"

static float3 LightPos = float3(0.f, 0.f, 0.f);
static float3 LightDir = float3(1.f, -1.f, 0.f);
static float3 LightColor = float3(0.78f, 0.23f, 0.31f);

static float3 Ambient = float3(0.15f, 0.06f, 0.07f);


struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;    
    float3 vNormal : NORMAL;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;    
    float LightPow : FOG;
};

VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    
    // 물체의 월드상에서의 표면의 수직방향
    float3 vWorldNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWorld));
    float3 vLightDir = normalize(LightDir);
    
    // 광원의 방향과, 표면의 법선벡터가 이루는 각도로 Cos 값을 구한다.
    output.LightPow = saturate(dot(vWorldNormal, -vLightDir));
 
    return output;
}

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float3 vObjectColor = float3(0.7f, 0.7f, 0.7f);
    
    float3 vOut = vObjectColor * LightColor * _in.LightPow 
                  + (vObjectColor * Ambient);
    
    return float4(vOut, 1.f);
}






#endif