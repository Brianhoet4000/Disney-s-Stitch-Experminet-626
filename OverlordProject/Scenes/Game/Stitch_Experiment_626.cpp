#include "stdafx.h"
#include "Stitch_Experiment_626.h"
#include "Scenegraph/GameObject.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Materials/SkyBoxMaterial.h"
#include "Prefabs/Button.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/Destroyables.h"
#include "Prefabs/EnemyBuzz.h"
#include "Prefabs/HUD.h"
#include "Prefabs/JumpPad.h"
#include "Prefabs/PickupBase.h"
#include "Prefabs/StitchPrefab.h"

void Stitch_Experiment_626::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;
	GetSceneSettings().drawPhysXDebug = false;

	m_SceneContext.pLights->SetDirectionalLight({ -0,10,-0 }, { -1, -1, 0.309117377f });

	m_pStitch = new StitchPrefab();
	AddChild(m_pStitch);

	auto pLevel = new LevelPrefab(2.f);
	AddChild(pLevel);

	m_pHud = new HUD(XMFLOAT2{ 1020, 660}, 0.6f, 0);
	m_pHud->GetPlayer(m_pStitch);
	AddChild(m_pHud);

	//auto buzzer = new EnemyBuzz();
	//buzzer->GetTransform()->Scale(0.01f);
	//AddChild(buzzer);

	//SkyBox
	{
		auto m_SkyBox = new GameObject();
		const auto pSkyBoxMaterial = MaterialManager::Get()->CreateMaterial<SkyBoxMaterial>();
		pSkyBoxMaterial->SetSkyBoxTexture(L"GameResources/Skybox2.dds");

		m_SkyBox->AddComponent(new ModelComponent(L"GameResources/Models/Box.ovm"));
		m_SkyBox->GetComponent<ModelComponent>()->SetMaterial(pSkyBoxMaterial);
		m_SkyBox->GetTransform()->Rotate(0.f, 0.f, 90.f);
		AddChild(m_SkyBox);
	}

	//Wall
	{
		XMFLOAT3 size{ 11, 6, 1 };
		auto Wall = new CubePrefab(size.x, size.y, size.z);
		Wall->GetTransform()->Translate(45.79f, -11.64f + size.y/2, -114.07f);
		Wall->GetTransform()->Rotate(0, 90, 0, true);

		auto& phys = PxGetPhysics();
		auto pBouncyMaterial = phys.createMaterial(0, 0, 1.f);

		auto pRigidBodyCp = Wall->AddComponent(new RigidBodyComponent(true));
		pRigidBodyCp->AddCollider(PxBoxGeometry(size.x /2, size.y /2, size.z/2), *pBouncyMaterial, false);
		AddChild(Wall);
	}

	//GameSound
	{
		static FMOD::Sound* pSound = nullptr;
		if (!pSound)
		{
			SoundManager::Get()->GetSystem()->createStream("Resources/GameResources/Audio/GreemaFalls.wav", FMOD_DEFAULT, nullptr, &pSound);
			pSound->setMode(FMOD_LOOP_NORMAL);
		}
		SoundManager::Get()->GetSystem()->playSound(pSound, nullptr, true, &m_pGameChannel);
		m_pGameChannel->setVolume(0.2f);
	}

	//Load Objects
	{
		auto jumppad = new JumpPad(3.f);
		jumppad->GetTransform()->Translate(30.f, -23.f, 0.6f);
		AddChild(jumppad);

		LoadPickUps();
		LoadDestroyables();
	}

	//postprocessing
	{
		m_pPostGrayscale = MaterialManager::Get()->CreateMaterial<PostGrayscale>();
		m_pPostBlur = MaterialManager::Get()->CreateMaterial<PostBlur>();
		m_pFilmGrain = MaterialManager::Get()->CreateMaterial<FilmGrain>();
		m_pInvert = MaterialManager::Get()->CreateMaterial<Invert>();

		AddPostProcessingEffect(m_pPostGrayscale);
		m_pPostGrayscale->SetIsEnabled(false);
		AddPostProcessingEffect(m_pPostBlur);
		m_pPostBlur->SetIsEnabled(false);
		AddPostProcessingEffect(m_pFilmGrain);
		m_pFilmGrain->SetIsEnabled(false);
		AddPostProcessingEffect(m_pInvert);
		m_pInvert->SetIsEnabled(false);
	}
}

void Stitch_Experiment_626::Update()
{
	if (GetSceneContext().pInput->IsKeyboardKey(InputState::pressed, VK_ESCAPE))
	{
		if (m_Paused)
		{
			m_Paused = false;
			RemoveMenu();
			m_pHud->SetTextPosition(XMFLOAT2{ 1020, 660 });
			m_SceneContext.pGameTime->Start();
		}
		else
		{
			m_Paused = true;
			CreateMenu();
			m_pHud->SetTextPosition(XMFLOAT2{ 955, 603 });
			m_SceneContext.pGameTime->Stop();
		}
	}
	

	CheckDeleting();
}

void Stitch_Experiment_626::OnSceneActivated()
{
	bool isPaused{};
	m_pGameChannel->getPaused(&isPaused);

	if (isPaused)
		m_pGameChannel->setPaused(false);
}
void Stitch_Experiment_626::OnSceneDeactivated()
{
	bool isPaused{};
	m_pGameChannel->getPaused(&isPaused);

	if (!isPaused)
		m_pGameChannel->setPaused(true);

	Reset();
}

void Stitch_Experiment_626::OnGUI()
{
	bool isEnabled = m_pPostGrayscale->IsEnabled();
	ImGui::Checkbox("Grayscale PP", &isEnabled);
	m_pPostGrayscale->SetIsEnabled(isEnabled);

	
	isEnabled = m_pPostBlur->IsEnabled();
	ImGui::Checkbox("Blur PP", &isEnabled);
	m_pPostBlur->SetIsEnabled(isEnabled);
	
	isEnabled = m_pFilmGrain->IsEnabled();
	ImGui::Checkbox("FilmGrain", &isEnabled);
	m_pFilmGrain->SetIsEnabled(isEnabled);

	isEnabled = m_pInvert->IsEnabled();
	ImGui::Checkbox("Invert", &isEnabled);
	m_pInvert->SetIsEnabled(isEnabled);
}

void Stitch_Experiment_626::CheckDeleting()
{
	for(auto child : GetChilderen())
	{
		if(child->ReturnDeleting())
		{
			this->RemoveChild(child, true);
		}
	}
}
void Stitch_Experiment_626::LoadDestroyables()
{
	m_DestroyPos.push_back(XMFLOAT3{ 30.47f, -10.01f, 30.75f });
	m_DestroyPos.push_back(XMFLOAT3{ 54.51f, -8.74f, -5.78f });
	m_DestroyPos.push_back(XMFLOAT3{ 36.51f, -23.88f, -12.90f });
	m_DestroyPos.push_back(XMFLOAT3{ 29.04f, -23.56f, -13.86f });
	m_DestroyPos.push_back(XMFLOAT3{ -33.62f, -11.49f, 18.00f });
	m_DestroyPos.push_back(XMFLOAT3{ -28.47f, -12.48f, 25.32f });

	m_DestroyRot.push_back(XMFLOAT3{ 0, 0, 0 });
	m_DestroyRot.push_back(XMFLOAT3{ 0, 90, 0 });
	m_DestroyRot.push_back(XMFLOAT3{ 0, -30, 0 });
	m_DestroyRot.push_back(XMFLOAT3{ 0, 20, 0 });
	m_DestroyRot.push_back(XMFLOAT3{ 0, 110, 0 });
	m_DestroyRot.push_back(XMFLOAT3{ 0, 110, 0 });


	for (int i = 0; i < m_DestroyPos.size(); ++i)
	{
		auto newBox = new DestroyableBox();

		newBox->GetTransform()->Translate(m_DestroyPos[i]);
		newBox->GetTransform()->Rotate(m_DestroyRot[i], true);

		AddChild(newBox);
	}
	


	auto Tiki = new DestroyableTiki1();
	Tiki->GetTransform()->Translate(-18.67f, -15.16f, 6.89f);
	Tiki->GetTransform()->Rotate(0, 67, 0, true);
	AddChild(Tiki);

	auto Tiki2 = new DestroyableTiki2();
	Tiki2->GetTransform()->Translate(11.55f, -13.86f, -16.68f);
	Tiki2->GetTransform()->Rotate(0, 123, 23, true);
	AddChild(Tiki2);

	auto Tiki3 = new DestroyableTiki2();
	Tiki3->GetTransform()->Translate(-5.53f, -11.82f, -26.71f);
	Tiki3->GetTransform()->Rotate(0, 180, 0, true);
	AddChild(Tiki3);

	auto Tiki4 = new DestroyableTiki2();
	Tiki4->GetTransform()->Translate(-25.46f, -14.89f, -13.77f);
	Tiki4->GetTransform()->Rotate(56, 164, 11, true);
	AddChild(Tiki4);
}
void Stitch_Experiment_626::LoadPickUps()
{
	m_pHealthPickUp = new HealthPickUp();
	m_pHealthPickUp->GetTransform()->Translate(2, -12, 10);
	AddChild(m_pHealthPickUp);

	auto pBlueDnaPickUp = new BlueDnaPickUp();
	pBlueDnaPickUp->GetTransform()->Translate(7, -12, 10);
	AddChild(pBlueDnaPickUp);


	pBlueDnaPickUp = new BlueDnaPickUp();
	pBlueDnaPickUp->GetTransform()->Translate(3.61f, -14.78f, -66.48f);
	AddChild(pBlueDnaPickUp);

	pBlueDnaPickUp = new BlueDnaPickUp();
	pBlueDnaPickUp->GetTransform()->Translate(-13.16f, -11.26f, -95.71f);
	AddChild(pBlueDnaPickUp);

	pBlueDnaPickUp = new BlueDnaPickUp();
	pBlueDnaPickUp->GetTransform()->Translate(40.29f, -14.87f, -76.66f);
	AddChild(pBlueDnaPickUp);

	auto pRedDnaPickup = new RedDnaPickUp();
	pRedDnaPickup->GetTransform()->Translate(28.46f, 0.71f, 1.20f);
	AddChild(pRedDnaPickup);

	pRedDnaPickup = new RedDnaPickUp();
	pRedDnaPickup->GetTransform()->Translate(29.11f, -15.08f, -66.38f);
	AddChild(pRedDnaPickup);
}
void Stitch_Experiment_626::Reset()
{
	m_pStitch->SetSpawnPos(XMFLOAT3{ 0,1,0 });

	for (auto child : GetChilderen())
	{
		if(dynamic_cast<DestroyableBox*>(child) || dynamic_cast<DestroyableTiki1*>(child)
			|| dynamic_cast<DestroyableTiki2*>(child))
		{
			RemoveChild(child, true);
		}
	}
	LoadDestroyables();

	for (auto child : GetChilderen())
	{
		if (dynamic_cast<BlueDnaPickUp*>(child) || dynamic_cast<RedDnaPickUp*>(child)
			|| dynamic_cast<HealthPickUp*>(child))
		{
			RemoveChild(child, true);
		}
	}
	LoadPickUps();

	RemoveMenu();
}

void Stitch_Experiment_626::CreateMenu()
{
	m_pMenu = AddChild(new GameObject());

	m_pMenu->AddComponent(new SpriteComponent(L"GameResources/UI/Options.png", XMFLOAT2{ 0.5f,0.5f }));
	m_pMenu->GetTransform()->Translate(XMFLOAT3{ m_SceneContext.windowWidth / 2.f ,m_SceneContext.windowHeight / 2.f ,0.1f });

	Button* pButton = m_pMenu->AddChild(new Button(L"Main Menu", XMFLOAT2{ (m_SceneContext.windowWidth / 2.f) - 52.f,(m_SceneContext.windowHeight / 2.f) - 120.f }, XMFLOAT4{ Colors::Yellow }, { 200.f,50.f }));
	pButton->SetOnClickFunction([&]()
		{
			SceneManager::Get()->SetActiveGameScene(L"Main_Scene");
		});

	pButton = m_pMenu->AddChild(new Button(L"Quit", XMFLOAT2{ (m_SceneContext.windowWidth / 2.f) - 52.f,(m_SceneContext.windowHeight / 2.f) - 60.f }, XMFLOAT4{ Colors::Yellow }, { 200.f,50.f }));
	pButton->SetOnClickFunction([&]()
		{
			std::exit(0);
		});

	pButton = m_pMenu->AddChild(new Button(L"Continue", XMFLOAT2{ (m_SceneContext.windowWidth / 2.f) - 52.f,(m_SceneContext.windowHeight / 2.f) - 180.f }, XMFLOAT4{ Colors::Yellow }, { 200.f,50.f }));
	pButton->SetOnClickFunction([&]()
		{
			m_Paused = false;
			m_pHud->SetTextPosition(XMFLOAT2{ 1020, 660 });
			m_SceneContext.pGameTime->Start();
			RemoveMenu();
		});
}
void Stitch_Experiment_626::RemoveMenu()
{
	if (m_pMenu)
	{
		for (auto& child : m_pMenu->GetChildren<GameObject>())
		{
			m_pMenu->RemoveChild(child, true);
		}

		RemoveChild(m_pMenu, true);

		m_pMenu = nullptr;
	}

	
}