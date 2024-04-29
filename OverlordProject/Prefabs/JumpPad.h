#pragma once
class JumpPad : public GameObject
{
public:
	JumpPad(float scale);

	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
protected:
	float m_Scale;
	FMOD::Channel* m_pExplosionChannel = nullptr;
};

