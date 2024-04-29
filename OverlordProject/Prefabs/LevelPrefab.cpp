#include "stdafx.h"
#include "LevelPrefab.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

#include "Materials/DiffuseMaterial.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"


LevelPrefab::LevelPrefab(const float scale)
	:m_Scale{ scale }
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	
	const auto pLevelObject = AddChild(new GameObject());
	//Level
	{
		const auto pLevelMesh = pLevelObject->AddComponent(new ModelComponent(L"GameResources/Models/Level/Level.ovm")); //L"GameResources/Models/New/Level5.ovm"


		auto pathTexture = ContentManager::GetFullAssetPath(L"GameResources/Models/FullTexturedMap/LevelMapTextured.mtl");
		auto mtl_contents = ParserMTL(pathTexture.string());

		for (UINT8 i{}; i < mtl_contents.size(); ++i)
		{
			int wstr_size = MultiByteToWideChar(CP_UTF8, 0, mtl_contents[i].c_str(), -1, NULL, 0);
			std::wstring wstr(wstr_size, 0);
			MultiByteToWideChar(CP_UTF8, 0, mtl_contents[i].c_str(), -1, &wstr[0], wstr_size);

			auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
			pMat->SetDiffuseTexture(L"GameResources/Textures/Map/" + wstr);
			pLevelMesh->SetMaterial(pMat, i);
		}

		const auto pLevelActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
		const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"GameResources/Models/Level/Level.ovpt");
		pLevelActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ m_Scale })), *pDefaultMaterial);
		pLevelObject->GetTransform()->Scale(m_Scale);
	}

	//LightShaft
	{
		//const auto pLightsGO = AddChild(new GameObject());
		const auto pLightShafts = pLevelObject->AddComponent(new ModelComponent(L"GameResources/Models/FullTexturedMap/LightShafts/LightShaftovm.ovm"));
		auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
		pMat->SetDiffuseTexture(L"GameResources/Textures/Map/LIGHT.png");
		pLightShafts->SetMaterial(pMat);
	}

	//Trees
	{
		const auto pTrees = pLevelObject->AddComponent(new ModelComponent(L"GameResources/Models/Trees/Trees.ovm"));
		auto pMatTree = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		pMatTree->SetDiffuseTexture(L"GameResources/Textures/Map/FEBL.png");
		pTrees->SetMaterial(pMatTree);

		const auto pTreeActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
		const auto pPxTreeTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"GameResources/Models/Trees/Trees.ovpt");
		pTreeActor->AddCollider(PxTriangleMeshGeometry(pPxTreeTriangleMesh, PxMeshScale({ m_Scale })), *pDefaultMaterial);


		const auto pTreeLeaves = pLevelObject->AddComponent(new ModelComponent(L"GameResources/Models/Trees/TreeLeaves.ovm"));
		auto pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
		pMat->SetDiffuseTexture(L"GameResources/Textures/Map/OJTRE4.png");
		pTreeLeaves->SetMaterial(pMat);
	}

	//LilyPads
	{
		const auto pLilyPads = pLevelObject->AddComponent(new ModelComponent(L"GameResources/Models/LilyPads/LilyPads.ovm"));
		auto pMatLily = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		pMatLily->SetDiffuseTexture(L"GameResources/Textures/Map/OJLILY.png");
		pLilyPads->SetMaterial(pMatLily);

		const auto pLilyActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
		const auto pPxLilyTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"GameResources/Models/LilyPads/LilyPads.ovpt");
		pLilyActor->AddCollider(PxTriangleMeshGeometry(pPxLilyTriangleMesh, PxMeshScale({ m_Scale })), *pDefaultMaterial);
	}
}

std::vector<std::string> LevelPrefab::ParserMTL(const std::string& filename)
{
	std::ifstream file(filename);
	std::vector<std::string> map_kds;
	std::string line;
	while (std::getline(file, line))
	{
		size_t map_kd_pos = line.find("map_Kd ");
		if (map_kd_pos != std::string::npos)
		{
			std::string map_kd_value = line.substr(map_kd_pos + 7);
			map_kds.push_back(map_kd_value);
		}

	}
	return map_kds;
}

std::vector<float> LevelPrefab::parseOBJ(const std::string& filename)
{
	std::ifstream file( "../OverlordProject/Resources/" + filename);
	if (!file)
	{
		std::cout << "Failed to open OBJ file: " << filename << std::endl;
		throw std::runtime_error("Failed to open file");
	}

	std::vector<float> normals;

	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;


		if (prefix == "f")
		{
			int vertexIndex[3], texcoordIndex[3], normalIndex[3];
			char slash;

			for (int i = 0; i < 3; ++i)
			{
				iss >> vertexIndex[i] >> slash >> texcoordIndex[i] >> slash >> normalIndex[i];
				// OBJ indices are 1-based, so subtract 1 to make them 0-based
				--vertexIndex[i];
				--texcoordIndex[i];
				--normalIndex[i];
			}
		}
	}

	return normals;
}