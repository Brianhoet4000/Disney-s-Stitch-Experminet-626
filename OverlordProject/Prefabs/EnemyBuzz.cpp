#include "stdafx.h"
#include "EnemyBuzz.h"

#include "Materials/DiffuseMaterial.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

void EnemyBuzz::Initialize(const SceneContext&)
{
	m_pEnemyObject = new GameObject();
	auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pMat->SetDiffuseTexture(L"GameResources/Textures/Enemy/BUZZERout.png");

	m_pModel = new ModelComponent(L"GameResources/Models/Enemy/EnemyBuzz_Wings.ovm");
	m_pEnemyObject->AddComponent<ModelComponent>(m_pModel);
	m_pModel->SetMaterial(pMat);

	std::cout << m_pModel->GetTransform()->GetPosition().x << ", " << m_pModel->GetTransform()->GetPosition().y << ", " << m_pModel->GetTransform()->GetPosition().z << '\n';
	std::cout << GetTransform()->GetPosition().x << ", " << GetTransform()->GetPosition().y << ", " << GetTransform()->GetPosition().z << '\n';
	std::cout << m_pEnemyObject->GetTransform()->GetPosition().x << ", " << m_pEnemyObject->GetTransform()->GetPosition().y << ", " << m_pEnemyObject->GetTransform()->GetPosition().z << '\n';

	auto& phys = PxGetPhysics();
	auto pBouncyMaterial = phys.createMaterial(0, 0, 1.f);
	auto pRigidBodyCp = AddComponent(new RigidBodyComponent(true));
	pRigidBodyCp->AddCollider(PxSphereGeometry(1.f), *pBouncyMaterial, false);

	AddChild(m_pEnemyObject);

	pAnimator = m_pModel->GetAnimator();
	pAnimator->SetAnimation(0);
	pAnimator->Play();

	
}

void EnemyBuzz::Update(const SceneContext&)
{
}
