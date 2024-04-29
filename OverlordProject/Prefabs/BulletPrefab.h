#pragma once
class BulletPrefab : public GameObject
{
public:
	BulletPrefab(const float bulletspeed, XMFLOAT3 forwardDir,ModelComponent* model);


protected:

	void Initialize(const SceneContext&) override;
	void Update(const SceneContext& sceneContext) override;

private:
	const float m_Speed;
	GameObject* m_pBullet;
	XMFLOAT3 m_ForwardDirCharacter;
	RigidBodyComponent* m_pBulletCollision{};
	ModelComponent* m_pModel;

	float m_Timer;
	bool ShouldUpdate = false;
	
	float m_DirectionX;
	float m_DirectionY;
	float m_DirectionZ;
};

