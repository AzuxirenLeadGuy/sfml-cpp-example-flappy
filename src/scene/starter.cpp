#include "../core/sfmlgameclass.hpp"
#include "../utils/utils.hpp"
#include "scene.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <cstdint>
#include <memory>
#include <random>

struct PhyObj
{
	sf::Vector2f Pos, Vel;
	std::unique_ptr<sf::Shape> _shape;
	PhyObj(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f vel, sf::Color color)
		: Pos(pos)
		, Vel(vel)
		, _shape(new sf::RectangleShape(size))
	{
		SetColor(color);
	}
	PhyObj(sf::Vector2f pos, float radius, sf::Vector2f vel, sf::Color color)
		: Pos(pos)
		, Vel(vel)
		, _shape(new sf::CircleShape(radius, 50))
	{
		SetColor(color);
	}
	void SetColor(sf::Color color)
	{
		_shape->setFillColor(color);
	}
	void Update(sf::Vector2f acc, float friction, const float time)
	{
		acc += -friction * Vel;
		Vel += acc;
		Pos += Vel * time;
		_shape->setPosition(Pos);
	}
	void Draw(sf::RenderWindow &window) const
	{
		window.draw(*_shape.get());
	}
};

struct Obstacle
{
	sf::RectangleShape lower, upper;
	float Position;

  public:
	Obstacle() = default;
	Obstacle(float unit, sf::Vector2f pos, float size)
	{
		sf::Vector2f sizevec(unit, size);
		lower.setSize(sizevec);
		upper.setSize(sizevec);
		Position = pos.x;
		auto y = pos.y;
		// unit *= 4;
		pos.y = y + unit;
		lower.setPosition(pos);
		pos.y = y - unit - size;
		upper.setPosition(pos);
	}
	void Update(float vel, const float time)
	{
		Position -= vel * time;
		auto pos_vec = upper.getPosition();
		upper.setPosition(Position, pos_vec.y);
		pos_vec = lower.getPosition();
		lower.setPosition(Position, pos_vec.y);
	}
	void SetColor(sf::Color color)
	{
		upper.setFillColor(color);
		lower.setFillColor(color);
	}
	void Draw(sf::RenderWindow &window) const
	{
		window.draw(lower);
		window.draw(upper);
	}
	bool Intersect(sf::FloatRect rect) const
	{
		auto urect = upper.getGlobalBounds();
		auto lrect = lower.getGlobalBounds();
		return rect.intersects(urect) || rect.intersects(lrect);
	}
};

using namespace core;
using namespace utils;
using KlPtr = std::unique_ptr<KeyListener>;
using PhyPtr = std::unique_ptr<PhyObj>;
using ObsPtr = std::unique_ptr<Obstacle>;

enum class State
{
	Playing,
	Paused,
	GameOver
};

namespace scenes
{
class Starter : public SfmlGameClass::AbstractScene
{
	KlPtr _listener;
	std::array<Obstacle, 8> _obstacle_list;
	PhyPtr Bird;
	sf::IntRect _boundary;
	State _state;
	uint64_t _score;
	uint8_t _activeObstacles;
	int64_t _timer = 0;
	unsigned int unit_size;
	std::uniform_real_distribution<> _dist;
	std::default_random_engine engine;

  public:
	Starter() = default;
	auto Load(SfmlGameClass &game) -> int override;
	auto PollEvent(SfmlGameClass &game, sf::Event event) -> int override;
	auto Update(SfmlGameClass &game, long time) -> UpdateResult override;
	auto Draw(SfmlGameClass &game, long delta) -> int override;
	auto Destroy(SfmlGameClass &game) -> int override;
};
} // namespace scenes

using namespace scenes;

auto Starter::Load(SfmlGameClass &game) -> int
{
	auto init = {
		sf::Keyboard::Key::Escape,
		sf::Keyboard::Key::Up,
		sf::Keyboard::Key::Space,
	};
	_listener = std::make_unique<KeyListener>(init);
	auto size = game._window.getSize();
	_boundary = sf::IntRect(0, 0, size.x, size.y);
	unit_size = size.x / game.SharedSettings.UnitDivisor;
	Bird = std::make_unique<PhyObj>(
		sf::Vector2f((size.x >> 2) - (unit_size >> 1), (size.y >> 1) - (unit_size >> 1)),
		unit_size,
		sf::Vector2f(),
		sf::Color::White);
	_score = 0;
	_state = State::Playing;
	_activeObstacles = 0;
	_timer = game.SharedSettings.LoadTime;
	Bird->SetColor(sf::Color::Yellow);

	_dist = std::uniform_real_distribution<>(0, 1);

	return 0;
}

auto Starter::PollEvent(SfmlGameClass & /*unused*/, sf::Event event) -> int
{
	if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
	{
		_listener->Update(event.key.code, event.type == sf::Event::KeyPressed);
	}
	return 0;
}

auto Starter::Update(SfmlGameClass &game, const long time) -> Starter::UpdateResult
{
	bool esc_press = _listener->ViewKey(sf::Keyboard::Escape) == KeyState::Just_Release;
	bool up_press = _listener->ViewKey(sf::Keyboard::Up) == KeyState::Just_Release;
	bool spc_press = _listener->ViewKey(sf::Keyboard::Space) == KeyState::Just_Release;
	if (esc_press)
	{
		_state = State::GameOver;
	}
	sf::Vector2f acc;
	acc.y = game.SharedSettings.Gravity;
	sf::FloatRect brect;
	float delta = time / 1000000.0;
	switch (_state)
	{
	case State::Playing:
		if (_timer > 0 && time > 0)
		{
			_timer -= time;
		}
		if (_timer < 0)
		{
			float value = _dist(engine);
			float height = unit_size + (value * (_boundary.height - unit_size));
			auto obs = Obstacle(
				unit_size * 4,
				sf::Vector2f(_boundary.left + _boundary.width, height), // RANDOM
				_boundary.height);
			obs.SetColor(sf::Color::Red);
			_obstacle_list[_activeObstacles] = obs;
			_activeObstacles++;
			_timer = game.SharedSettings.LoadTime;
		}
		if (spc_press)
		{
			_state = State::Paused;
		}
		if (up_press)
		{
			Bird->Vel.y = -game.SharedSettings.JumpSpeed;
		}
		Bird->Update(acc, 0, delta);
		if (Bird->Pos.y > _boundary.height || Bird->Pos.y < 0)
		{
			_state = State::GameOver;
		}
		brect = Bird->_shape->getGlobalBounds();
		for (int i = _activeObstacles - 1; i >= 0; i--)
		{
			_obstacle_list[i].Update(game.SharedSettings.ObstacleSpeed, delta);
			if (_obstacle_list[i].Intersect(brect))
			{
				_state = State::GameOver;
			}
			else if (_obstacle_list[i].Position < 0)
			{
				_score++;
				_activeObstacles--;
				if (_activeObstacles <= 0)
				{
					break;
				}
				_obstacle_list[i] = _obstacle_list[_activeObstacles];
			}
		}
		break;
	case State::Paused:
		if (spc_press)
		{
			_state = State::Playing;
		}
		break;
	case State::GameOver:
		printf("Your score is %ld\n\n", _score);
		game.ExitCall();
		break;
	}
	_listener->FrameEnd();
	return Starter::UpdateResult{0, nullptr};
}

auto Starter::Draw(SfmlGameClass &game /*unused*/, const long /*unused*/) -> int
{
	switch (_state)
	{
	case State::GameOver: break;
	default:
		Bird->Draw(game._window);
		for (int i = 0; i < _activeObstacles; i++)
		{
			_obstacle_list[i].Draw(game._window);
		}
	}
	return 0;
}

auto Starter::Destroy(SfmlGameClass & /*unused*/) -> int
{
	Bird.reset();
	_listener.reset();
	return 0;
}

auto scenes::GetStarter() -> std::unique_ptr<SfmlGameClass::AbstractScene>
{
	return std::make_unique<Starter>();
}