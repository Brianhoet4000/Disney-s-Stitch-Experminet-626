#pragma once

class EnemyBuzz : public GameObject
{
public:

	EnemyBuzz() = default;
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;


private:
	ModelAnimator* pAnimator{ nullptr };
	GameObject* m_pEnemyObject{ nullptr };
	ModelComponent* m_pModel{ nullptr };

};

