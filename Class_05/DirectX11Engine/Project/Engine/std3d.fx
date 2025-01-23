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
    
    float3 vViewPos : POSITION;
    float3 vViewNormal : NORMAL;    
};

VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    // 광원의 정보
    tLightInfo LightInfo = g_Light3D[0];
            
    // 정점의 월드상의 노말방향을 픽셀쉐이더에 보간해서 넘긴다.
    output.vViewPos     = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vViewNormal  = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
    
    return output;
}

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float3 vObjectColor = float3(0.7f, 0.7f, 0.7f);
            
    // 광원의 정보
    tLightInfo LightInfo = g_Light3D[0];
    
    // View 스페이스에서 광원의 방향
    float3 vLight = mul(float4(LightInfo.WorldDir, 0.f), g_matWV).xyz;
        
    // 램버트 코사인법칙
    float LightPow = saturate(dot(normalize(_in.vViewNormal), -normalize(vLight)));
        
    // 반사
    // Reflect
    float3 vReflect = vLight + dot(-vLight, _in.vViewNormal) * 2.f * _in.vViewNormal;
        
    // 시선벡터
    float3 vEye = _in.vViewPos;
    
    // 반사된 빛이 카메라로 들어오는 각도를 cos0 로 환산
    float ReflectPow = saturate(dot(-normalize(vEye), normalize(vReflect)));        
    ReflectPow = pow(ReflectPow, 20);
        
    // 물체의 색상에 빛을 적용
    float3 vOut = (float3) 0.f;
    
    if(g_int_0)
    {
        vOut = vObjectColor * LightInfo.Light.vDiffuse.rgb * LightPow
               + (vObjectColor * LightInfo.Light.vAmbient.rgb)
               + LightInfo.Light.vDiffuse.rgb * LightInfo.Light.vMaxSpecular.rgb * ReflectPow;
    }
    else
    {
        vOut = vObjectColor * LightInfo.Light.vDiffuse.rgb * LightPow
               + (vObjectColor * LightInfo.Light.vAmbient.rgb);
    }
    
    return float4(vOut, 1.f);
}

#endif