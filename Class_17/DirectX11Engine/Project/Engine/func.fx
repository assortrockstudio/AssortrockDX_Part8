#ifndef _FUNC
#define _FUNC

#include "value.fx"


float3 CalLight2D(int _LightIdx, float3 _vWorldPos)
{    
    tLightInfo info = g_Light2D[_LightIdx];
  
    // 광원 처리
    float3 vLightPow = (float3) 0.f;
        
    // Directional Light
    if (0 == info.LightType)
    {
        vLightPow = info.Light.vDiffuse.rgb + info.Light.vAmbient.rgb;
    }
    
    // PointLight
    else if (1 == info.LightType)
    {
        // 픽셀의 월드스페이스
        float fDist = distance(info.WorldPos.xy, _vWorldPos.xy);
        
        float fRatio = 0.f;
       
        //fRatio = 1.f - saturate(fDist / info.Range);
        fRatio = cos((fDist / info.Range) * (PI / 2.f));        
        
        if (fDist < info.Range)
        {
            vLightPow = info.Light.vDiffuse.xyz * fRatio;
        }
    }
    
    // SpotLight
    else
    {
        
    }
        
    return vLightPow;
}

void CalLight3D(int _LightIdx, float3 _ViewPos, float3 _ViewNormal, inout tLight _Light)
{    
    // 광원의 정보
    tLightInfo LightInfo = g_Light3D[_LightIdx];
        
    // 빛의 세기
    float LightPow = (float) 0.f;
    
    // 빛의 방향
    float3 vLight = (float3) 0.f;
    
    // 빛을 적용받을 픽셀(물체) 에서의 법선벡터
    _ViewNormal = normalize(_ViewNormal);
    
    // 거리에 따른 빛의 세기
    float DistancePow = 1.f;
    
    // Directional
    if (0 == LightInfo.LightType)
    {
        // View 스페이스에서 광원의 방향
        vLight = normalize(mul(float4(LightInfo.WorldDir, 0.f), g_matView).xyz);        
        
        // 램버트 코사인법칙
        LightPow = saturate(dot(_ViewNormal, -vLight));
    }
    
    // Point
    else if(1 == LightInfo.LightType)
    {
        // View 스페이스에서 광원의 방향
        // 물체(픽셀) 의 위치 - 광원의 위치
        float3 vLightPos = mul(float4(LightInfo.WorldPos.xyz, 1.f), g_matView).xyz;       
        vLight = normalize(_ViewPos - vLightPos);
        
        // 램버트 코사인법칙
        LightPow = saturate(dot(_ViewNormal, -vLight));
        
        // 광원과 물체 사이의 거리를 구한다.
        float Distance = distance(_ViewPos, vLightPos);
        
        // 거리에 따른 빛의 세기 감소율
        //DistancePow =  saturate(1.f - Distance / LightInfo.Range);
        DistancePow = saturate(cos(saturate(Distance / LightInfo.Range) * (PI / 2.f)));
    }
    
    // Spot
    else
    {
        
    } 
    
    // 반사
    // Reflect
    float3 vReflect = normalize(vLight + dot(-vLight, _ViewNormal) * 2.f * _ViewNormal);
        
    // 시선벡터
    float3 vEye = normalize(_ViewPos);
    
    // 반사된 빛이 카메라로 들어오는 각도를 cos0 로 환산
    float ReflectPow = saturate(dot(-vEye, vReflect));
    ReflectPow = pow(ReflectPow, 20);
         
    // 빛의 크기를 계산
    _Light.vDiffuse.rgb     += LightInfo.Light.vDiffuse.rgb * LightPow * DistancePow;
    _Light.vAmbient.rgb     += LightInfo.Light.vAmbient.rgb;
    _Light.vMaxSpecular.rgb += LightInfo.Light.vDiffuse.rgb * LightInfo.Light.vMaxSpecular.rgb * ReflectPow * DistancePow;
}


int IsBinding(in Texture2D _tex)
{
    uint width = 0;
    uint height = 0;
    _tex.GetDimensions(width, height);
    
    if (width && height)
        return 1;
    else
        return 0;
}



float3 GetRandom(in Texture2D _Noise, float _NormalizedThreadID)
{
    // 파티클의 포지션을 랜덤한 위치로 지정하기
    float2 vUV = (float2) 0.f;      
    
    vUV.x = _NormalizedThreadID + (Time * 0.1f);
    vUV.y = (sin((vUV.x - Time) * PI * 20.f) * 0.5f) + (Time * 0.2f);
                
    float3 vNoise = _Noise.SampleLevel(g_sam_0, vUV, 0).xyz;
    
    return vNoise;
}



#endif