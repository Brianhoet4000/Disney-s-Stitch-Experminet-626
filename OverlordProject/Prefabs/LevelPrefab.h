#pragma once

class LevelPrefab final : public GameObject
{
public:

    struct Vertex
	{
        float x, y, z;  // Position
        float u, v;     // Texture coordinates
        float nx, ny, nz; // Normal vectors
    };

    LevelPrefab(const float scale);


private:
    std::vector<std::string> ParserMTL(const std::string& filename);
    std::vector<float> parseOBJ(const std::string& filename);

    const float m_Scale{};
};

