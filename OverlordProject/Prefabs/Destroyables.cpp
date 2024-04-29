#include "stdafx.h"
#include "Destroyables.h"

#include "Prefabs/BulletPrefab.h"
#include "ExplosionParticle.h"
#include "HUD.h"
#include "StitchPrefab.h"
#include "Materials/DiffuseMaterial.h"

void Destroyables::SpawnParticles()
{
	auto pExplosion = new ExplosionParticle(0.8f);
	pExplosion->GetTransform()->Translate(GetTransform()->GetPosition());
	GetScene()->AddChild(pExplosion);
}

void Destroyables::PlaySoundExplosion()
{
	static FMOD::Sound* pSound = nullptr;
	if (!pSound)
	{
		SoundManager::Get()->GetSystem()->createStream("Resources/GameResources/Audio/BoxExplosion.wav", FMOD_DEFAULT, nullptr, &pSound);
		pSound->setMode(FMOD_LOOP_OFF);
	}
	SoundManager::Get()->GetSystem()->playSound(pSound, nullptr, false, &m_pExplosionChannel);
	m_pExplosionChannel->setVolume(7.0f);
}

void DestroyableBox::Initialize(const SceneContext&)
{
	auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMat->SetDiffuseTexture(L"GameResources/Textures/Destroyables/OJCRATE.png");

	auto pObject = new GameObject();
	ModelComponent* pModel = new ModelComponent(L"GameResources/Models/Destroyables/Crate.ovm");
	pModel->SetMaterial(pMat);
	pObject->AddComponent<ModelComponent>(pModel);
	AddChild(pObject);
	pModel->GetTransform()->Translate(0, 0, 0);

	//Collision
	auto& phys = PxGetPhysics();
	auto pBouncyMaterial = phys.createMaterial(0, 0, 1.f);

	//Trigger
	const auto pRigidBody = AddComponent(new RigidBodyComponent(true));
	const auto pConvexMesh = ContentManager::Load<PxConvexMesh>(L"GameResources/Models/Destroyables/Crate.ovpc");
	pRigidBody->AddCollider(PxConvexMeshGeometry(pConvexMesh, PxMeshScale(1.5f)), *pBouncyMaterial, true);

	//Collision
	pRigidBody->AddCollider(PxConvexMeshGeometry(pConvexMesh), *pBouncyMaterial, false);

	auto onTrigger = [&](GameObject*, GameObject* other, PxTriggerAction action)
	{
		if (other->GetTag() != L"Bullet") return;

		if (action != PxTriggerAction::ENTER) return;

		for (const auto& child : GetScene()->GetChilderen())
		{
			auto hud = dynamic_cast<HUD*>(child);

			if (hud != nullptr)
			{
				
				hud->AddScore(25);
			};
		}

		//auto bullet = dynamic_cast<BulletPrefab*>(other);
		//bullet->MarkTrueForDeleting();

		SpawnParticles();
		PlaySoundExplosion();
		MarkTrueForDeleting();
	};

	SetOnTriggerCallBack(onTrigger);
}

void DestroyableTiki1::Initialize(const SceneContext&)
{
	const float scale{ 2.5f };
	auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMat->SetDiffuseTexture(L"GameResources/Textures/Destroyables/TIKI1.png");

	auto pCrateObject = new GameObject();
	ModelComponent* pModel = new ModelComponent(L"GameResources/Models/Destroyables/Tiki1.ovm");
	pModel->SetMaterial(pMat);
	pCrateObject->AddComponent<ModelComponent>(pModel);
	AddChild(pCrateObject);
	pModel->GetTransform()->Translate(0, 0, 0);

	//Collision
	auto& phys = PxGetPhysics();
	auto pBouncyMaterial = phys.createMaterial(0, 0, 1.f);

	//Trigger
	const auto pRigidBody = AddComponent(new RigidBodyComponent(true));
	const auto pConvexMesh = ContentManager::Load<PxConvexMesh>(L"GameResources/Models/Destroyables/Tiki1.ovpc");
	pRigidBody->AddCollider(PxConvexMeshGeometry(pConvexMesh, PxMeshScale( scale + 0.5f )), *pBouncyMaterial, true);
	pRigidBody->GetTransform()->Scale(scale);

	//Collision
	pRigidBody->AddCollider(PxConvexMeshGeometry(pConvexMesh, PxMeshScale(scale)), *pBouncyMaterial, false);

	auto onTrigger = [&](GameObject*, GameObject* other, PxTriggerAction action)
	{
		if (other->GetTag() != L"Bullet") return;

		if (action != PxTriggerAction::ENTER) return;

		//auto bullet = dynamic_cast<BulletPrefab*>(other);
		//bullet->MarkTrueForDeleting();

		for (const auto& child : GetScene()->GetChilderen())
		{
			auto hud = dynamic_cast<HUD*>(child);

			if (hud != nullptr)
			{
				
				hud->AddScore(25);
			};
		}

		SpawnParticles();
		PlaySoundExplosion();
		MarkTrueForDeleting();
	};

	SetOnTriggerCallBack(onTrigger);
}

void DestroyableTiki2::Initialize(const SceneContext&)
{
	const float scale{ 2.f };
	auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMat->SetDiffuseTexture(L"GameResources/Textures/Destroyables/TIKI2.png");

	auto pCrateObject = new GameObject();
	ModelComponent* pModel = new ModelComponent(L"GameResources/Models/Destroyables/Tiki2.ovm");
	pModel->SetMaterial(pMat);
	pCrateObject->AddComponent<ModelComponent>(pModel);
	AddChild(pCrateObject);
	pModel->GetTransform()->Translate(0, 0, 0);

	//Collision
	auto& phys = PxGetPhysics();
	auto pBouncyMaterial = phys.createMaterial(0, 0, 1.f);

	//Trigger
	const auto pRigidBody = AddComponent(new RigidBodyComponent(true));
	const auto pConvexMesh = ContentManager::Load<PxConvexMesh>(L"GameResources/Models/Destroyables/Tiki2.ovpc");
	pRigidBody->AddCollider(PxConvexMeshGeometry(pConvexMesh, PxMeshScale( scale + 0.5f )), *pBouncyMaterial, true);
	pRigidBody->GetTransform()->Scale(scale);

	//Collision
	pRigidBody->AddCollider(PxConvexMeshGeometry(pConvexMesh, PxMeshScale(scale)), *pBouncyMaterial, false);

	auto onTrigger = [&](GameObject*, GameObject* other, PxTriggerAction action)
	{
		if (other->GetTag() != L"Bullet") return;

		if (action != PxTriggerAction::ENTER) return;

		//auto bullet = dynamic_cast<BulletPrefab*>(other);
		//bullet->MarkTrueForDeleting();

		for (const auto& child : GetScene()->GetChilderen())
		{
			auto hud = dynamic_cast<HUD*>(child);

			if (hud != nullptr)
			{
				hud->AddScore(25);
			};
		}

		SpawnParticles();
		PlaySoundExplosion();
		MarkTrueForDeleting();
	};

	SetOnTriggerCallBack(onTrigger);
}

void DestroyableCrystal::Initialize(const SceneContext&)
{
	const float scale{ 1.f };
	auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMat->SetDiffuseTexture(L"GameResources/Textures/Destroyables/CRYREF.png");

	auto pCrateObject = new GameObject();
	ModelComponent* pModel = new ModelComponent(L"GameResources/Models/Destroyables/Crystal.ovm");
	pModel->SetMaterial(pMat);
	pCrateObject->AddComponent<ModelComponent>(pModel);
	AddChild(pCrateObject);
	pModel->GetTransform()->Translate(0, 0, 0);

	//Collision
	auto& phys = PxGetPhysics();
	auto pBouncyMaterial = phys.createMaterial(0, 0, 1.f);

	//Trigger
	const auto pRigidBody = AddComponent(new RigidBodyComponent(true));
	const auto pConvexMesh = ContentManager::Load<PxConvexMesh>(L"GameResources/Models/Destroyables/Crystal.ovpc");
	pRigidBody->AddCollider(PxConvexMeshGeometry(pConvexMesh, PxMeshScale(scale)), *pBouncyMaterial, true);
	pRigidBody->GetTransform()->Scale(scale);

	//Collision
	pRigidBody->AddCollider(PxConvexMeshGeometry(pConvexMesh, PxMeshScale(scale)), *pBouncyMaterial, false);

	auto onTrigger = [&](GameObject*, GameObject* other, PxTriggerAction action)
	{
		if (other->GetTag() != L"Bullet") return;

		if (action != PxTriggerAction::ENTER) return;

		//auto bullet = dynamic_cast<BulletPrefab*>(other);
		//bullet->MarkTrueForDeleting();

		for (const auto& child : GetScene()->GetChilderen())
		{
			auto hud = dynamic_cast<HUD*>(child);

			if (hud != nullptr)
			{
				hud->AddScore(25);
			};
		}

		SpawnParticles();
		PlaySoundExplosion();
		MarkTrueForDeleting();
	};

	SetOnTriggerCallBack(onTrigger);
}
