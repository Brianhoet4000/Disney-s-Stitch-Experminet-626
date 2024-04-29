#include "stdafx.h"
#include "HUD.h"

#include "CubePrefab.h"
#include "StitchPrefab.h"

HUD::HUD(XMFLOAT2 position, float scale, int score)
	:m_TextPosition{ position },
	m_Scale{scale},
	m_Score{ score }
{
}

void HUD::Initialize(const SceneContext& sceneContext)
{
	OldHealth = m_pStitch->GetHealth();

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");

	auto DNAObj = new GameObject();
	m_pDnaUI = new SpriteComponent(L"GameResources/UI/DnaCounter.png");
	DNAObj->AddComponent(m_pDnaUI);
	AddChild(DNAObj);
	DNAObj->GetTransform()->Scale(m_Scale);
	DNAObj->GetTransform()->Translate(XMFLOAT3{ sceneContext.windowWidth - (m_pDnaUI->GetDimension().x * m_Scale),
		sceneContext.windowHeight - (m_pDnaUI->GetDimension().y * m_Scale),0.4f });

	m_pHealthObj = new GameObject();
	m_pHpIndicator = new SpriteComponent(L"GameResources/UI/Health.png");
	m_pHealthObj->AddComponent(m_pHpIndicator);
	AddChild(m_pHealthObj);
	m_pHealthObj->GetTransform()->Scale(m_Scale);
	m_pHealthObj->GetTransform()->Translate(XMFLOAT3{ 150 ,
		sceneContext.windowHeight - (m_pHpIndicator->GetDimension().y * m_Scale) - 10, 0.4f });

	auto HealthUI = new GameObject();
	auto HpUI = new SpriteComponent(L"GameResources/UI/HealthCounter.png");
	HealthUI->AddComponent(HpUI);
	AddChild(HealthUI);
	HealthUI->GetTransform()->Scale(m_Scale);
	HealthUI->GetTransform()->Translate(XMFLOAT3{0,
		sceneContext.windowHeight - (HpUI->GetDimension().y * m_Scale), 0.3f });

}

void HUD::Update(const SceneContext&)
{
	if(m_DrawText)
	TextRenderer::Get()->DrawText(m_pFont, std::to_wstring(m_Score), XMFLOAT2{ m_TextPosition.x,
		m_TextPosition.y }, m_TextColor);

	if(OldHealth != m_pStitch->GetHealth())
	{
		m_pHealthObj->GetTransform()->Scale( float(m_pStitch->GetHealth()/ (m_pHpIndicator->GetDimension().x * m_Scale)), m_Scale, m_Scale);
		OldHealth = m_pStitch->GetHealth();
	}
}

void HUD::AddScore(int score)
{
	m_Score += score;
}

int HUD::GetScore()
{
	return m_Score;
}

void HUD::SetTextPosition(XMFLOAT2 pos)
{
	m_TextPosition = pos;
}

void HUD::GetPlayer(StitchPrefab* player)
{
	m_pStitch = player;
}

