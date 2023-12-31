#ifndef SCENE
#define SCENE
#include "../core/sfmlgameclass.hpp"
#include <memory>

using namespace core;

namespace scenes
{
auto GetStarter() -> std::unique_ptr<SfmlGameClass::AbstractScene>;
auto GetLoader() -> std::unique_ptr<SfmlGameClass::AbstractScene>;
} // namespace scenes

#endif