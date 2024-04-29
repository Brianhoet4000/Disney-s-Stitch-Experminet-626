#pragma once


class PickupBase : public GameObject
{

public:
	PickupBase() = default;

};

class HealthPickUp final : public PickupBase
{

public:
	HealthPickUp() = default;
	void Initialize(const SceneContext&) override;
};

class BlueDnaPickUp final : public PickupBase
{

public:
	BlueDnaPickUp() = default;
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext& sceneContext) override;

private:
	float m_Rot;

};

class RedDnaPickUp final : public PickupBase
{

public:
	RedDnaPickUp() = default;
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext& sceneContext) override;

private:
	float m_Rot;
};