#include "stdafx.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(const std::wstring& spriteAsset, const XMFLOAT2& pivot, const XMFLOAT4& color):
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color)
{}

void SpriteComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

XMFLOAT2 SpriteComponent::GetDimension()
{
	return m_pTexture->GetDimension();
}

void SpriteComponent::Draw(const SceneContext& sceneContext)
{
	if (!m_pTexture)
		return;

	const auto transform = m_pGameObject->GetTransform();
	const XMFLOAT2 position = XMFLOAT2{ transform->GetPosition().x,  transform->GetPosition().y };
	const XMFLOAT2 scale = XMFLOAT2{ transform->GetScale().x, transform->GetScale().y };
	const float rotZ = MathHelper::QuaternionToEuler(transform->GetRotation()).z;

	SpriteRenderer::Get()->AppendSprite(m_pTexture, position, XMFLOAT4{ Colors::White }, m_Pivot, scale, rotZ, transform->GetPosition().z);
	SpriteRenderer::Get()->Draw(sceneContext);
}