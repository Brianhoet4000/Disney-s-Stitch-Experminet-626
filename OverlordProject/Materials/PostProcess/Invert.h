#pragma once
class Invert : public PostProcessingMaterial
{
public:
	Invert();
	~Invert() override = default;
	Invert(const Invert& other) = delete;
	Invert(Invert&& other) noexcept = delete;
	Invert& operator=(const Invert& other) = delete;
	Invert& operator=(Invert&& other) noexcept = delete;

protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
};

