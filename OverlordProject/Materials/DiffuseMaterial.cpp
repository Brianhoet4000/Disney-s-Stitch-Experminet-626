#include "stdafx.h"
#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial() :
    Material<DiffuseMaterial>(L"Effects/PosNormTex3D.fx")
{

}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
    m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
    InitializeEffectVariables();
}

void DiffuseMaterial::InitializeEffectVariables()
{
    SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture->GetShaderResourceView());
}