#pragma once

#include "CTexture.h"

enum class RENDER_MODE
{
    PLAY,
    EDITOR,
};

class CCamera;
class CLight2D;
class CLight3D;
class CStructuredBuffer;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    vector<CCamera*>    m_vecCam;
    CCamera*            m_EditorCam;

    void(CRenderMgr::* Render_Func)(void);

    vector<CLight2D*>   m_vecLight2D;
    CStructuredBuffer* m_Light2DBuffer;

    vector<CLight3D*>   m_vecLight3D;    
    CStructuredBuffer*  m_Light3DBuffer;

    Ptr<CTexture>       m_RenderTargetCopyTex;

    class CMRT*         m_MRT[(UINT)MRT_TYPE::END];


public:
    void RegisterCamera(CCamera* _Cam, int _Priority);
    void ClearRegisteredCamera() { m_vecCam.clear(); }
    void RegisterEditorCamera(CCamera* _EditorCam) { m_EditorCam = _EditorCam; }

    vector<CCamera*>& GetRegisteredCamera() { return m_vecCam; }

    int RegisterLight2D(CLight2D* _Light2D)
    {
        m_vecLight2D.push_back(_Light2D);
        return (int)m_vecLight2D.size() - 1;
    }

    int RegisterLight3D(CLight3D* _Light3D)
    {
        m_vecLight3D.push_back(_Light3D);
        return (int)m_vecLight3D.size() - 1;
    }

    void CopyRenderTarget();


public:
    void init();
    void tick();
    void render();

    void ChangeRenderMode(RENDER_MODE _Mode)
    {
        RENDER_MODE::PLAY == _Mode ? Render_Func = &CRenderMgr::render_play : Render_Func = &CRenderMgr::render_editor;
    }

private:
    void render_play();
    void render_editor();

private:
    void DataBinding();
    void DataClear();
    void CreateMRT();
    void ClearMRT();
};

 