#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial() :
    Material<UberMaterial>(L"Effects/UberShader.fx")
{
}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
    m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
    InitializeEffectVariables();
}

void UberMaterial::SetSpecularTexture()//const std::wstring& assetFile)
{
    
}

void UberMaterial::SetNormalTexture()//const std::wstring& assetFile)
{
    
}

void UberMaterial::InitializeEffectVariables()
{
    SetVariable_Scalar(L"gUseTextureDiffuse", true);
    SetVariable_Vector(L"gColorDiffuse", { 1,0,0 });
    SetVariable_Texture(L"gTextureDiffuse", m_pDiffuseTexture->GetShaderResourceView());
}