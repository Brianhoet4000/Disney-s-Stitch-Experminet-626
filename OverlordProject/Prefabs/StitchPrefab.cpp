#include "stdafx.h"
#include "StitchPrefab.h"

#include <corecrt_math_defines.h>

#include "Prefabs/BulletPrefab.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

void StitchPrefab::Initialize(const SceneContext& sceneContext)
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	SetTag(L"Player");

	//Character
	CharacterDesc characterDesc{ pDefaultMaterial, 0.5f, 1.f };
	characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;
	characterDesc.actionId_Jump = CharacterJump;

	m_pCharacter = AddChild(new Character(characterDesc));
	m_pCharacter->GetTransform()->Translate(0.f, 1.f, 0.f);

	//Input
	{
		auto inputAction = InputAction(CharacterMoveLeft, InputState::down, 'A', -1 , XINPUT_GAMEPAD_DPAD_LEFT);
		sceneContext.pInput->AddInputAction(inputAction);

		inputAction = InputAction(CharacterMoveRight, InputState::down, 'D', -1, XINPUT_GAMEPAD_DPAD_RIGHT);
		sceneContext.pInput->AddInputAction(inputAction);

		inputAction = InputAction(CharacterMoveForward, InputState::down, 'W', -1 , XINPUT_GAMEPAD_DPAD_UP);
		sceneContext.pInput->AddInputAction(inputAction);

		inputAction = InputAction(CharacterMoveBackward, InputState::down, 'S', -1, XINPUT_GAMEPAD_DPAD_DOWN);
		sceneContext.pInput->AddInputAction(inputAction);

		inputAction = InputAction(CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
		sceneContext.pInput->AddInputAction(inputAction);
	}

	//Character
	{
		m_pStitchObject = new GameObject();
		auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
		pMat->SetDiffuseTexture(L"GameResources/Textures/STITCH.png");
		auto pMat2 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
		pMat2->SetDiffuseTexture(L"GameResources/Textures/STITCH2.png");

		m_pModel = new ModelComponent(L"GameResources/Models/Stitch/StitchModel3.ovm");
		m_pStitchObject->AddComponent<ModelComponent>(m_pModel);
		m_pModel->SetMaterial(pMat);
		m_pModel->SetMaterial(pMat2);
		m_pStitchObject->GetTransform()->Scale(.02f);

		AddChild(m_pStitchObject);


		pAnimator = m_pModel->GetAnimator();
		pAnimator->SetAnimation(CharacterState::Idle);
	}
}

void StitchPrefab::Update(const SceneContext& sceneContext)
{
	if (sceneContext.pInput->IsKeyboardKey(InputState::down, VK_BACK))
	{
		DecreaseHealth(25);
	}

	auto pos = m_pCharacter->GetTransform()->GetPosition();
	pos.y -= 1.0f;
	m_pStitchObject->GetTransform()->Translate(pos);

	if ((abs(m_pCharacter->GetVelocity().x) != 0 && abs(m_pCharacter->GetVelocity().z) != 0))
	{
		auto rot = std::atan2f(m_pCharacter->GetVelocity().z, -m_pCharacter->GetVelocity().x) * float(180 / M_PI) + 90;
		m_pStitchObject->GetTransform()->Rotate(0, rot, 0);
	}

	if (m_Shot)
	{
		m_Trigger -= 1.f * sceneContext.pGameTime->GetElapsed();
	}

	if (sceneContext.pInput->IsMouseButton(InputState::down, 1) || sceneContext.pInput->IsGamepadButton(InputState::down, XINPUT_GAMEPAD_X))
	{
		m_Shot = true;
		
		
		if (m_Trigger <= 0.f)
		{
			std::cout << m_pStitchObject->GetTransform()->GetPosition().x << ", " << m_pStitchObject->GetTransform()->GetPosition().y << ", " << m_pStitchObject->GetTransform()->GetPosition().z << '\n';
			const float speed{ 1000.f };
			auto bullet = new BulletPrefab(speed, m_pCharacter->GetDirection(), m_pModel);
			AddChild(bullet);
			m_Trigger = 1.f;
			m_Shot = false;

			static FMOD::Sound* pSound = nullptr;
			if (!pSound)
			{
				SoundManager::Get()->GetSystem()->createStream("Resources/GameResources/Audio/Shoot.wav", FMOD_DEFAULT, nullptr, &pSound);
				pSound->setMode(FMOD_LOOP_OFF);
				
			}
			SoundManager::Get()->GetSystem()->playSound(pSound, nullptr, false, &m_pShootingChannel);
			m_pShootingChannel->setVolume(2.f);

		}
	}
	else
	{
		m_Shot = false;
	}
	
	m_IsGrounded = IsGrounded();

	if (!m_IsGrounded && m_State != CharacterState::Jump)
	{
		pAnimator->Pause();
		m_State = CharacterState::Jump;
	}
	else if(m_Shot && m_State != CharacterState::Shoot && m_IsGrounded)
	{
		pAnimator->Pause();
		m_State = CharacterState::Shoot;
	}
	else if ((abs(m_pCharacter->GetVelocity().x) > 0 && abs(m_pCharacter->GetVelocity().z) > 0) && m_State != CharacterState::Running && m_IsGrounded && !m_Shot)
	{
		pAnimator->Pause();
		m_State = CharacterState::Running;
	}
	else if ((abs(m_pCharacter->GetVelocity().x) == 0 && abs(m_pCharacter->GetVelocity().z) == 0) && m_State != CharacterState::Idle && m_IsGrounded && !m_Shot)
	{
		pAnimator->Pause();
		m_State = CharacterState::Idle;
	}

	if (!pAnimator->IsPlaying())
	{
		pAnimator->SetAnimation(m_State);
		pAnimator->Play();
	}

}

void StitchPrefab::SetSpawnPos(XMFLOAT3 pos)
{
	m_pCharacter->GetTransform()->Translate(pos);
}

void StitchPrefab::SetHealth(int newhealth)
{
	m_health = newhealth;
}

void StitchPrefab::DecreaseHealth(int Damage)
{
	static FMOD::Sound* pSound = nullptr;
	if (!pSound)
	{
		SoundManager::Get()->GetSystem()->createStream("Resources/GameResources/Audio/Damage.wav", FMOD_DEFAULT, nullptr, &pSound);
		pSound->setMode(FMOD_LOOP_OFF);
	}
	SoundManager::Get()->GetSystem()->playSound(pSound, nullptr, false, &m_pHealthChannel);
	m_pHealthChannel->setVolume(2.f);

	m_health = m_health - Damage;

	if (m_health < 0)
		m_health = 0;
}

void StitchPrefab::IncreaseHealth(int Addhealth)
{
	static FMOD::Sound* pSound = nullptr;
	if (!pSound)
	{
		SoundManager::Get()->GetSystem()->createStream("Resources/GameResources/Audio/Health.wav", FMOD_DEFAULT, nullptr, &pSound);
		pSound->setMode(FMOD_LOOP_OFF);
	}
	SoundManager::Get()->GetSystem()->playSound(pSound, nullptr, false, &m_pHealthChannel);
	m_pHealthChannel->setVolume(2.f);


	m_health = m_health + Addhealth;

	if (m_health > 100)
		m_health = 100;
}

bool StitchPrefab::IsGrounded()
{
	PxVec3 rayStart = PxVec3{ m_pModel->GetTransform()->GetPosition().x, m_pModel->GetTransform()->GetPosition().y ,m_pModel->GetTransform()->GetPosition().z };
	PxVec3 rayDir{ 0, -1, 0 };
	float distance{ 0.4f };
	
	PxRaycastBuffer hit{};

	//DebugRenderer::DrawLine(m_pModel->GetTransform()->GetPosition(), XMFLOAT3{ rayStart.x, rayStart.y - distance, rayStart.z}, XMFLOAT4{ Colors::Red });

	return GetScene()->GetPhysxProxy()->Raycast(rayStart, rayDir, distance, hit);
}
