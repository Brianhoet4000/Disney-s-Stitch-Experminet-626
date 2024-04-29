#include "stdafx.h"
#include "JumpPad.h"

#include "StitchPrefab.h"
#include "Materials/DiffuseMaterial.h"

JumpPad::JumpPad(float scale)
	:m_Scale{scale}
{
}

void JumpPad::Initialize(const SceneContext&)
{
	auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMat->SetDiffuseTexture(L"GameResources/Textures/JUMPAH2.png");

	auto pCrateObject = new GameObject();
	ModelComponent* pModel = new ModelComponent(L"GameResources/Models/JumpPad/JumpPad.ovm");
	pModel->SetMaterial(pMat);
	pCrateObject->AddComponent<ModelComponent>(pModel);
	AddChild(pCrateObject);
	pModel->GetTransform()->Scale(m_Scale);
	pModel->GetTransform()->Translate(0, 0, 0);

	
	auto& phys = PxGetPhysics();
	auto pBouncyMaterial = phys.createMaterial(0, 0, 1.f);

	//Trigger
	const auto pRigidBody = AddComponent(new RigidBodyComponent(true));
	const auto pConvexMesh = ContentManager::Load<PxConvexMesh>(L"GameResources/Models/JumpPad/JumpPad.ovpc");
	pRigidBody->AddCollider(PxConvexMeshGeometry(pConvexMesh, PxMeshScale(m_Scale)), *pBouncyMaterial, true);

	auto onTrigger = [&](GameObject*, GameObject* other, PxTriggerAction action)
	{
		if (other->GetTag() != L"Player") return;

		if (action != PxTriggerAction::ENTER) return;

		for (const auto& child : GetScene()->GetChilderen())
		{
			auto stitch = dynamic_cast<StitchPrefab*>(child);

			if (stitch != nullptr)
			{
				for (auto& stitchchild  : stitch->GetChildren<Character>())
				{
					auto character = dynamic_cast<Character*>(stitchchild);

					character->SetVelocity(100);
				}
			};
		}
	};
	SetOnTriggerCallBack(onTrigger);
}

void JumpPad::Update(const SceneContext&)
{
}
