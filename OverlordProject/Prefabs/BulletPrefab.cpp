#include "stdafx.h"
#include "BulletPrefab.h"

#include "SpherePrefab.h"

BulletPrefab::BulletPrefab(const float bulletspeed, XMFLOAT3 forwardDir,ModelComponent* model)
	:m_Speed{ bulletspeed },
	m_ForwardDirCharacter{ forwardDir },
	m_pModel{model}
{
	m_Timer = 6;
}

void BulletPrefab::Initialize(const SceneContext&)
{
	auto pBulletObject = new GameObject();

	pBulletObject->SetTag(L"Bullet");

	auto& phys = PxGetPhysics();
	auto pBouncyMaterial = phys.createMaterial(0, 0, 1.f);
	const float scale{ 0.3f };
	m_pBullet = new SpherePrefab(scale, 10, XMFLOAT4(Colors::Green));
	pBulletObject->AddChild(m_pBullet);

	m_pBulletCollision = pBulletObject->AddComponent(new RigidBodyComponent(false));
	m_pBulletCollision->AddCollider(PxSphereGeometry(scale), *pBouncyMaterial, false);
	m_pBulletCollision->SetConstraint(RigidBodyConstraint::TransY, false);
	
	m_pBulletCollision->GetTransform()->Translate( m_pModel->GetTransform()->GetPosition().x + m_ForwardDirCharacter.x + 0.5f,
		m_pModel->GetTransform()->GetPosition().y + 1,m_pModel->GetTransform()->GetPosition().z + m_ForwardDirCharacter.z);

	AddChild(pBulletObject);

	ShouldUpdate = true;
}

void BulletPrefab::Update(const SceneContext& sceneContext)
{
	if (ShouldUpdate)
	{
		m_Timer -= 1.f * sceneContext.pGameTime->GetElapsed();

		const float speed{ 50 };

		m_DirectionX = speed * m_ForwardDirCharacter.x * sceneContext.pGameTime->GetElapsed();
		m_DirectionY = speed * m_ForwardDirCharacter.y * sceneContext.pGameTime->GetElapsed();
		m_DirectionZ = speed * m_ForwardDirCharacter.z * sceneContext.pGameTime->GetElapsed();

		m_pBulletCollision->AddForce(XMFLOAT3{ m_DirectionX, m_DirectionY, m_DirectionZ }, PxForceMode::eIMPULSE);
	
		if(m_Timer < 0.2f)
		{
			ShouldUpdate = false;
			GetParent()->RemoveChild(this, true);
		}
	}
}
