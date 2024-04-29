#pragma once
class MainScreen : public GameScene
{
public:
	MainScreen() :GameScene(L"Main_Scene") {}
	~MainScreen() override = default;
	MainScreen(const MainScreen& other) = delete;
	MainScreen(MainScreen&& other) noexcept = delete;
	MainScreen& operator=(const MainScreen& other) = delete;
	MainScreen& operator=(MainScreen&& other) noexcept = delete;

	void Initialize() override;
	void Update() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	SpriteFont* m_pFont{};
	SpriteComponent* m_pMainScreen{};
	XMFLOAT2 m_TextPosition{};
	XMFLOAT4 m_TextColor{ 0.f,0.f,0.f,1.f };
	float m_Timer{};
	float m_MaxTimer{};
	bool m_HittedZero{ false };

	FMOD::Channel* m_pMainMenuChannel = nullptr;
};

