#pragma once

class ExplosionParticle final : public GameObject
{

public:

	ExplosionParticle(float lifeTime);

	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	ParticleEmitterComponent* m_pEmitter{};
	float m_LifeTime;
	float m_Timer;
};

