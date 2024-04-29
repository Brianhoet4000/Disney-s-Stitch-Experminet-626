#pragma once
#include "Materials/PostProcess/FilmGrain.h"
#include "Materials/PostProcess/Invert.h"
#include "Materials/PostProcess/PostBlur.h"
#include "Materials/PostProcess/PostGrayscale.h"
#include "Prefabs/BulletPrefab.h"
#include "Prefabs/Destroyables.h"
#include "Prefabs/HUD.h"
#include "Prefabs/LevelPrefab.h"
#include "Prefabs/PickupBase.h"
#include "Prefabs/StitchPrefab.h"
class Character;

class Stitch_Experiment_626 : public GameScene
{
public:
	Stitch_Experiment_626() :GameScene(L"Stitch_Experiment") {}
	~Stitch_Experiment_626() override = default;
	Stitch_Experiment_626(const Stitch_Experiment_626& other) = delete;
	Stitch_Experiment_626(Stitch_Experiment_626&& other) noexcept = delete;
	Stitch_Experiment_626& operator=(const Stitch_Experiment_626& other) = delete;
	Stitch_Experiment_626& operator=(Stitch_Experiment_626&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
	void OnGUI() override;

	void CheckDeleting();
	void LoadDestroyables();
	void LoadPickUps();
	void Reset();

private:

	GameObject* m_pHealthPickUp{};
	FMOD::Channel* m_pGameChannel = nullptr;
	FMOD::Channel* m_pPauseChannel = nullptr;
	StitchPrefab* m_pStitch{};

	void CreateMenu();
	void RemoveMenu();

	GameObject* m_pMenu{ nullptr };
	HUD* m_pHud{ nullptr };
	bool m_Paused{ false };

	//PostProcessing
	PostBlur* m_pPostBlur{};
	PostGrayscale* m_pPostGrayscale{};
	FilmGrain* m_pFilmGrain{};
	Invert* m_pInvert{};

	XMFLOAT3 m_StartPos{};
	std::vector<XMFLOAT3> m_DestroyPos{};
	std::vector<XMFLOAT3> m_DestroyRot{};
};

