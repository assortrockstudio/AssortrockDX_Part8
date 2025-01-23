#include "pch.h"
#include "SkyBoxUI.h"

#include <Engine/CSkyBox.h>

SkyBoxUI::SkyBoxUI()
	: ComponentUI("SkyBox", "##SkyBoxUI", COMPONENT_TYPE::SKYBOX)
{
}

SkyBoxUI::~SkyBoxUI()
{
}

void SkyBoxUI::render_tick()
{
	render_title();

	CSkyBox* pSkyBox = GetTarget()->SkyBox();
	
	SKYBOX_TYPE Type = pSkyBox->GetSkyBoxType();

    const char* items[2] = {};
    items[0] = "SPHERE";
    items[1] = "CUBE";

    if (ImGui::BeginCombo("##SkyBoxType", items[(UINT)Type], 0))
    {
        for (int i = 0; i < 2; ++i)
        {
            const bool is_selected = ((UINT)Type == i);
            if (ImGui::Selectable(items[i], is_selected))
            {
                pSkyBox->SetSkyBoxType((SKYBOX_TYPE)i);
            }
        }
        ImGui::EndCombo();
    }


}