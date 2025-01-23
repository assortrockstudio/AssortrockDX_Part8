#pragma once
#include "CRenderComponent.h"

enum SKYBOX_TYPE
{
    SPHERE,
    CUBE,
};

class CSkyBox :
    public CRenderComponent
{
private:
    SKYBOX_TYPE     m_Type;


public:
    void SetSkyBoxType(SKYBOX_TYPE _Type);
    


public:
    virtual void finaltick() override;
    virtual void render() override;

public:
    CLONE(CSkyBox);
    CSkyBox();
    ~CSkyBox();
};

