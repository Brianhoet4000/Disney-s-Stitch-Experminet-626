#pragma once
class Destroyables : public GameObject
{
public:
	Destroyables() = default;
	void SpawnParticles();
	void PlaySoundExplosion();
protected:
	FMOD::Channel* m_pExplosionChannel = nullptr;
};

class DestroyableBox final : public Destroyables
{
public:
	DestroyableBox() = default;
	void Initialize(const SceneContext&) override;
	
};

class DestroyableTiki1 final : public Destroyables
{
public:
	DestroyableTiki1() = default;
	void Initialize(const SceneContext&) override;
};

class DestroyableTiki2 final : public Destroyables
{
public:
	DestroyableTiki2() = default;
	void Initialize(const SceneContext&) override;
};

class DestroyableCrystal final : public Destroyables
{
public:
	DestroyableCrystal() = default;
	void Initialize(const SceneContext&) override;
};