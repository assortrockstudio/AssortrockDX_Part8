#ifndef _STD3D
#define _STD3D

#include "value.fx"

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
    float3 vWorldNormal : NORMAL;    
};

VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    // ������ ����
    tLightInfo LightInfo = g_Light3D[0];
            
    // ������ ������� �븻������ �ȼ����̴��� �����ؼ� �ѱ��.
    output.vWorldNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWorld));
    
    return output;
}

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float3 vObjectColor = float3(0.7f, 0.7f, 0.7f);
            
    // ������ ����
    tLightInfo LightInfo = g_Light3D[0];
        
    // ����Ʈ �ڻ��ι�Ģ
    float LightPow = saturate(dot(normalize(_in.vWorldNormal), -normalize(LightInfo.WorldDir.xyz)));
        
    // �ݻ�
    // Reflect
    float3 vReflect = LightInfo.WorldDir.xyz + dot(-LightInfo.WorldDir.xyz, _in.vWorldNormal) * 2.f * _in.vWorldNormal;
       
    
    
    float3 vOut = vObjectColor * LightInfo.Light.vDiffuse.rgb * LightPow
                  + (vObjectColor * LightInfo.Light.vAmbient.rgb);
    
    
    
    
    
    
    return float4(vOut, 1.f);
   
}

#endif