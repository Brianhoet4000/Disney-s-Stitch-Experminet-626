#include "stdafx.h"
#include "DiffuseMaterial_Shadow.h"

DiffuseMaterial_Shadow::DiffuseMaterial_Shadow():
	Material(L"Effects/Shadow/PosNormTex3D_Shadow.fx")
{}

void DiffuseMaterial_Shadow::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Shadow::InitializeEffectVariables()
{
}

void DiffuseMaterial_Shadow::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
    XMMATRIX lightWorldViewPointmatrix = (XMLoadFloat4x4(&pModel->GetGameObject()->GetTransform()->GetWorld()) * XMLoadFloat4x4(&ShadowMapRenderer::Get()->GetLightVP()));
    XMFLOAT4X4 lightWorldViewPoint{};

    XMStoreFloat4x4(&lightWorldViewPoint, lightWorldViewPointmatrix);

    SetVariable_Matrix(L"gWorldViewProj_Light", (float*)&lightWorldViewPoint);

    SetVariable_Texture(L"gShadowMap", ShadowMapRenderer::Get()->GetShadowMap());

    SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
}
