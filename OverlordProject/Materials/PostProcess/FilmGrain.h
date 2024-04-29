#pragma once
class FilmGrain : public PostProcessingMaterial
{
public:
	FilmGrain();
	~FilmGrain() override = default;
	FilmGrain(const FilmGrain& other) = delete;
	FilmGrain(FilmGrain&& other) noexcept = delete;
	FilmGrain& operator=(const FilmGrain& other) = delete;
	FilmGrain& operator=(FilmGrain&& other) noexcept = delete;

protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
};