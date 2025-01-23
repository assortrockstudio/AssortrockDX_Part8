#ifndef _STD3D
#define _STD3D

#include "value.fx"
#include "func.fx"

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
    
    float3 vViewPos : POSITION;
    float3 vViewNormal : NORMAL;    
};

VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    // ������ ����
    tLightInfo LightInfo = g_Light3D[0];
            
    // ������ ������� �븻������ �ȼ����̴��� �����ؼ� �ѱ��.
    output.vViewPos     = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vViewNormal  = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
    
    return output;
}

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float3 vObjectColor = float3(1.f, 0.0f, 1.f);
    float Alpha = 1.f;
    
    if(g_btex_0)
    {
        float4 vColor   = g_tex_0.Sample(g_sam_0, _in.vUV);
        vObjectColor    = vColor.rgb;
        Alpha           = vColor.a;
    }
    
    // ������ ���� ���
    tLight Light = (tLight) 0.f;
    
    for (int i = 0; i < Light3DCount; ++i)
    {
        CalLight3D(i, _in.vViewPos, _in.vViewNormal, Light);
    }
        
    // ��ü�� ������ ����
    float4 vOut = float4(0.f, 0.f, 0.f, 1.f);
    
    vOut.rgb = vObjectColor.rgb * Light.vDiffuse.rgb
               + vObjectColor.rgb * Light.vAmbient.rgb
               + Light.vMaxSpecular.rgb;
    
    return vOut;
}

#endif