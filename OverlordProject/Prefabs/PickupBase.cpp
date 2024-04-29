#include "stdafx.h"
#include "PickupBase.h"

#include "Prefabs/Character.h"
#include "HUD.h"
#include "StitchPrefab.h"
#include "Materials/DiffuseMaterial.h"

void HealthPickUp::Initialize(const SceneContext& /*sceneContext*/)
{
	auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMat->SetDiffuseTexture(L"GameResources/Textures/OAHLTHout.png");

	auto pModelObject = new GameObject();
	ModelComponent* pModel = new ModelComponent(L"GameResources/Models/PickUps/AlienToe.ovm");
	pModel->SetMaterial(pMat);
	pModelObject->AddComponent<ModelComponent>(pModel);
	AddChild(pModelObject);
	pModel->GetTransform()->Translate(0, -0.5f, 0);

	//Collision
	auto& phys = PxGetPhysics();
	auto pBouncyMaterial = phys.createMaterial(0, 0, 1.f);

	auto pRigidBodyCp = AddComponent(new RigidBodyComponent(true));
	const XMFLOAT3 size{ 1, 1 ,1 };
	pRigidBodyCp->AddCollider(PxSphereGeometry(1.f), *pBouncyMaterial, true);

	auto onTrigger = [&](GameObject*, GameObject* other, PxTriggerAction action)
	{
		if (other->GetTag() != L"Player") return;

		if (action != PxTriggerAction::ENTER) return;

		for (const auto& child : GetScene()->GetChilderen())
		{
			auto Player = dynamic_cast<StitchPrefab*>(child);

			if (Player != nullptr)
			{
				Player->IncreaseHealth(25);
			};
		}

		MarkTrueForDeleting();
	};

	SetOnTriggerCallBack(onTrigger);
}

void BlueDnaPickUp::Initialize(const SceneContext& /*sceneContext*/)
{
	auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMat->SetDiffuseTexture(L"GameResources/Textures/OADNAout.png");

	auto pModelObject = new GameObject();
	ModelComponent* pModel = new ModelComponent(L"GameResources/Models/PickUps/Dna.ovm");
	pModel->SetMaterial(pMat);
	pModelObject->AddComponent<ModelComponent>(pModel);
	AddChild(pModelObject);
	pModel->GetTransform()->Translate(0, -0.5f, 0);
	pModel->GetTransform()->Scale(1.3f);

	//Collision
	auto& phys = PxGetPhysics();
	auto pBouncyMaterial = phys.createMaterial(0, 0, 1.f);

	auto pRigidBodyCp = AddComponent(new RigidBodyComponent(false));
	pRigidBodyCp->SetConstraint(RigidBodyConstraint::AllTrans, false);
	
	pRigidBodyCp->AddCollider(PxSphereGeometry(1.f), *pBouncyMaterial, true);

	auto onTrigger = [&](GameObject*, GameObject* other, PxTriggerAction action)
	{
		if (other->GetTag() != L"Player") return;

		if (action != PxTriggerAction::ENTER) return;

		for(const auto& child : GetScene()->GetChilderen())
		{
			auto hud = dynamic_cast<HUD*>(child);

			if(hud != nullptr)
			{
				hud->AddScore(10);
		  	}
		}
		  
		MarkTrueForDeleting();
	};

	SetOnTriggerCallBack(onTrigger);
}

void BlueDnaPickUp::Update(const SceneContext& sceneContext)
{
	m_Rot += sceneContext.pGameTime->GetElapsed() * 180;
	GetTransform()->Rotate(0, m_Rot, 0, true);
}

void RedDnaPickUp::Initialize(const SceneContext&)
{
	auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMat->SetDiffuseTexture(L"GameResources/Textures/OADNARout.png");

	auto pModelObject = new GameObject();
	ModelComponent* pModel = new ModelComponent(L"GameResources/Models/PickUps/Dna.ovm");
	pModel->SetMaterial(pMat);
	pModelObject->AddComponent<ModelComponent>(pModel);
	AddChild(pModelObject);
	pModel->GetTransform()->Translate(0, -0.5f, 0);
	pModel->GetTransform()->Scale(1.3f);

	//Collision
	auto& phys = PxGetPhysics();
	auto pBouncyMaterial = phys.createMaterial(0, 0, 1.f);

	auto pRigidBodyCp = AddComponent(new RigidBodyComponent(true));
	const XMFLOAT3 size{ 1, 1 ,1 };
	pRigidBodyCp->AddCollider(PxSphereGeometry(1.f), *pBouncyMaterial, true);

	auto onTrigger = [&](GameObject*, GameObject* other, PxTriggerAction action)
	{
		if (other->GetTag() != L"Player") return;

		if (action != PxTriggerAction::ENTER) return;

		for (const auto& child : GetScene()->GetChilderen())
		{
			auto hud = dynamic_cast<HUD*>(child);

			if (hud != nullptr)
			{
				hud->AddScore(50);
			};
		}
		MarkTrueForDeleting();
	};
	SetOnTriggerCallBack(onTrigger);
}

void RedDnaPickUp::Update(const SceneContext& sceneContext)
{
	m_Rot += sceneContext.pGameTime->GetElapsed() * 180;
	GetTransform()->Rotate(0, m_Rot, 0, true);
}
