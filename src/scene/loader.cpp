#include "../core/sfmlgameclass.hpp"
#include "scene.hpp"
#include <memory>
using namespace core;

namespace scenes
{
class Loader : public SfmlGameClass::AbstractScene
{
  public:
	auto Load(SfmlGameClass &game) -> int override;
	auto PollEvent(SfmlGameClass &game, sf::Event event) -> int override;
	auto Update(SfmlGameClass &game, long delta) -> UpdateResult override;
	auto Draw(SfmlGameClass &game, long delta) -> int override;
	auto Destroy(SfmlGameClass &game) -> int override;
};
} // namespace scenes

using namespace scenes;
using namespace core;

auto Loader::Load(SfmlGameClass & /*unused*/) -> int
{
	return 0;
}

auto Loader::PollEvent(SfmlGameClass & /*unused*/, sf::Event /*unused*/) -> int
{
	return 0;
}

auto Loader::Update(SfmlGameClass & /*unused*/, const long /*unused*/)
	-> Loader::UpdateResult
{
	return Loader::UpdateResult{0, nullptr};
}

auto Loader::Draw(SfmlGameClass & /*unused*/, const long /*unused*/) -> int
{
	return 0;
}

auto Loader::Destroy(SfmlGameClass & /*unused*/) -> int
{
	return 0;
}

auto scenes::GetLoader() -> std::unique_ptr<SfmlGameClass::AbstractScene>
{
	return std::make_unique<Loader>();
}