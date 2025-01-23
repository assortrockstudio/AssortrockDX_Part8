#ifndef _LIGHT
#define _LIGHT

#include "value.fx"
#include "func.fx"

// ======================
// DirLightShader
// Domain : DOMAIN_LIGHTING
// MRT    : LIGHT
// Mesh   : RectMesh
#define LightIdx        g_int_0
#define PositionTarget  g_tex_0
#define NormalTarget    g_tex_1
// ======================
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

VS_OUT VS_DirLight(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // Directional Light �� ȭ���� ��� �ȼ��� ���ؼ� �ȼ����̴��� ȣ��ǵ��� ��
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

struct PS_OUT
{
    float4 vDiffuse     : SV_Target;
    float4 vSpecular    : SV_Target1;
};

PS_OUT PS_DirLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
        
    // ȣ��� �ȼ��� ������ ��ġ�� ��ϵ� Position ���� �����´�.
    float4 vPosition = PositionTarget.Sample(g_sam_0, _in.vUV);
    
    // Position ���� �������� ������, ���� ���� ��ü�� �������� �ʴ´�.
    if (0.f == vPosition.a)
    {
        discard;
    }
    
    float4 vNormal = NormalTarget.Sample(g_sam_0, _in.vUV);
    
    tLight light = (tLight) 0.f;
    
    CalLight3D(LightIdx, vPosition.xyz, vNormal.xyz, light);
        
    output.vDiffuse = light.vDiffuse + light.vAmbient;
    output.vSpecular = light.vMaxSpecular;
        
    output.vDiffuse.a = vPosition.z;
    output.vSpecular.a = vPosition.z;
    
    return output;
}

#endif