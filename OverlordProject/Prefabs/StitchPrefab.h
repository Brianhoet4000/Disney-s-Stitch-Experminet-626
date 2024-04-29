#pragma once
#include "Prefabs/Character.h"

class StitchPrefab final : public GameObject
{
public:
	StitchPrefab() = default;

	int GetHealth() { return m_health; }
	void SetHealth(int newhealth);
	void DecreaseHealth(int Damage);
	void IncreaseHealth(int Addhealth);
	void SetSpawnPos(XMFLOAT3 pos);

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	

private:
	bool IsGrounded();
	bool m_IsGrounded = false;

	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump,
		CharacterShoot
	};

	enum CharacterState : int
	{
		Idle,
		Running,
		Jump,
		Shoot,
		Crawl
	};

	float m_Trigger = 1.f;
	bool m_Shot = false;

	int m_health{ 100 };

	CharacterState m_State{};

	ModelAnimator* pAnimator{ nullptr };

	Character* m_pCharacter{ nullptr };
	GameObject* m_pStitchObject{ nullptr };
	ModelComponent* m_pModel{ nullptr };

	FMOD::Channel* m_pShootingChannel = nullptr;
	FMOD::Channel* m_pHealthChannel = nullptr;
	FMOD::Channel* m_pWalkingChannel = nullptr;
};

