#include "stdafx.h"
#include "MainScreen.h"

void MainScreen::Initialize()
{
	

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");

	auto GameObjMS = new GameObject();
	m_pMainScreen = new SpriteComponent(L"GameResources/UI/Mainscreen.png");
	GameObjMS->AddComponent(m_pMainScreen);
	AddChild(GameObjMS);

	m_MaxTimer = 0.7f;
	m_Timer = m_MaxTimer;

	static FMOD::Sound* pSound = nullptr;
	if (!pSound)
	{
		SoundManager::Get()->GetSystem()->createStream("Resources/GameResources/Audio/MainMenu.wav", FMOD_DEFAULT, nullptr, &pSound);
		
		pSound->setMode(FMOD_LOOP_NORMAL);
	}
	SoundManager::Get()->GetSystem()->playSound(pSound, nullptr, true, &m_pMainMenuChannel);
	m_pMainMenuChannel->setVolume(0.2f);


}

void MainScreen::Update()
{
	TextRenderer::Get()->DrawText(m_pFont, L"Press Space/A to start the game", XMFLOAT2{100,
		GetSceneContext().windowHeight - 100 }, m_TextColor);
	TextRenderer::Get()->DrawText(m_pFont, L"Press esc/X to exit the game", XMFLOAT2{ GetSceneContext().windowWidth/2 + 10,
		GetSceneContext().windowHeight - 100 }, m_TextColor);

	if (m_Timer > 0.f && !m_HittedZero)
	{
		m_TextColor = XMFLOAT4{ 0,0,0,0 };
		m_Timer -= 1.f * GetSceneContext().pGameTime->GetElapsed();

		if(m_Timer < 0.f) m_HittedZero = true;
	}
	if(m_Timer < m_MaxTimer && m_HittedZero)
	{
		m_Timer += 1.f * GetSceneContext().pGameTime->GetElapsed();
		m_TextColor = XMFLOAT4{ 0,0,0,1 };

		if(m_Timer > m_MaxTimer) m_HittedZero = false;
	}

	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, VK_SPACE) || m_SceneContext.pInput->IsGamepadButton(InputState::down, XINPUT_GAMEPAD_A))
	{
		SceneManager::Get()->SetActiveGameScene(L"Stitch_Experiment");
	}
	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, VK_ESCAPE) || m_SceneContext.pInput->IsGamepadButton(InputState::down, XINPUT_GAMEPAD_X))
	{
		std::exit(0);
	}
}

void MainScreen::OnSceneActivated()
{
	bool isPaused{};
	m_pMainMenuChannel->getPaused(&isPaused);

	if (isPaused)
		m_pMainMenuChannel->setPaused(false);
}

void MainScreen::OnSceneDeactivated()
{
	bool isPaused{};
	m_pMainMenuChannel->getPaused(&isPaused);

	if (!isPaused)
		m_pMainMenuChannel->setPaused(true);
}
