#ifndef _SKYBOX
#define _SKYBOX

#include "value.fx"
#include "func.fx"

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

VS_OUT VS_SkyBox(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // SphereMesh ÀÇ Local ÁÂÇ¥       
    float4 vViewPos = mul(float4(_in.vPos, 0.f), g_matView);        
    output.vPosition = mul(vViewPos, g_matProj);
    output.vPosition.z = output.vPosition.w;
    
    output.vUV = _in.vUV;    
    
    return output;
}

float4 PS_SkyBox(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }        
    
    return vOutColor;
}


#endif