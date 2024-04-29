#include "stdafx.h"
#include "ExplosionParticle.h"

ExplosionParticle::ExplosionParticle(float lifeTime)
	:m_LifeTime(lifeTime)
{
	m_Timer = m_LifeTime;
}

void ExplosionParticle::Initialize(const SceneContext&)
{
	//Particle System
	ParticleEmitterSettings settings{};
	settings.velocity = { 0.f,3.f,0.f };
	settings.minSize = 1.f;
	settings.maxSize = 1.6f;
	settings.minEnergy = 1.f;
	settings.maxEnergy = 2.f;
	settings.minScale = 3.5f;
	settings.maxScale = 5.5f;
	settings.minEmitterRadius = .5f;
	settings.maxEmitterRadius = 1.4f;
	settings.color = { 1.f,1.0f,1.0f, .6f };

	m_pEmitter = AddComponent(new ParticleEmitterComponent(L"Textures/Smoke_explosion.png", settings, 150));
	m_pEmitter->SetDrawParticles(true);
}

void ExplosionParticle::Update(const SceneContext& sceneContext)
{
	m_Timer -= 1.0f * sceneContext.pGameTime->GetElapsed();

	if(m_Timer <= 0.f)
	{
		MarkTrueForDeleting();
	}
}
