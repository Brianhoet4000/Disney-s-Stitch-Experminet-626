#pragma once
class StitchPrefab;

class HUD final : public GameObject
{
public:
	HUD(XMFLOAT2 position, float scale, int score);
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	void AddScore(int score);
	int GetScore();

	void SetTextPosition(XMFLOAT2 pos);

	void GetPlayer(StitchPrefab* player);
private:
	StitchPrefab* m_pStitch;
	int OldHealth{};

	SpriteFont* m_pFont{};

	int m_Score{ 0 };
	float m_Scale;
	XMFLOAT2 m_TextPosition{};
	SpriteComponent* m_pDnaUI{};

	GameObject* m_pHealthObj{ nullptr };
	SpriteComponent* m_pHpIndicator{};

	XMFLOAT4 m_TextColor{ 1.f,1.f,1.f,1.f };

	bool m_DrawText = true;
};

