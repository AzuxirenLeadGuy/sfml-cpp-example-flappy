#include "sfmlgameclass.hpp"
#include "../scene/scene.hpp"

#include <memory>
#include <thread>

using namespace core;

SfmlGameClass::SfmlGameClass(const SfmlGameSettings &init_settings)
	: _window(
		  sf::VideoMode(init_settings.Width, init_settings.Height),
		  init_settings.Title,
		  init_settings.WindowStyle,
		  init_settings.Context)
	, SharedSettings(init_settings)
{
	_exit_called = false;
	_is_loading = false;
	_window.setPosition(sf::Vector2i(0, 0));
	if (init_settings.VerticalSyncEnabled)
	{
		_window.setVerticalSyncEnabled(init_settings.VerticalSyncEnabled);
	}
	else
	{
		_window.setFramerateLimit(init_settings.FPS);
	}
	_main_error = 0;
	_thread_err = 0;
	_time_taken = 0;
}

SfmlGameClass::~SfmlGameClass() = default;

void SfmlGameClass::ExitCall()
{
	_exit_called = true;
}

auto SfmlGameClass::Initialize() -> int
{
	main = scenes::GetStarter();
	back = scenes::GetLoader();
	return 0;
}

auto SfmlGameClass::ShutDown() -> int
{
	int err1;
	int err2;
	if ((err1 = main->Destroy(*this)) != 0)
	{
		_main_error = err1;
	}
	if ((err2 = back->Destroy(*this)) != 0)
	{
		_main_error = err2;
	}
	main.reset();
	back.reset();
	return err1 + err2;
}

auto SfmlGameClass::RunFrame() -> int
{
	auto &scene = _is_loading ? back : main;
	sf::Event event;
	int err = 0;
	while (_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			ExitCall();
		}
		else if ((err = scene->PollEvent(*this, event)) != 0)
		{
			break;
		}
	}
	if (err > 0)
	{
		return err;
	}

	_window.clear(SharedSettings.ClearColor);

	auto up_result = scene->Update(*this, _time_taken);
	if (up_result.Valid != 0)
	{
		return up_result.Valid;
	}
	if ((err = scene->Draw(*this, _time_taken)) != 0)
	{
		return err;
	}
	if (_is_loading)
	{
		goto stop;
	}
	else if (_thread_err != 0)
	{
		return _thread_err;
	}
	else if (up_result.Next != nullptr)
	{
		_is_loading = true;
		std::thread(
			[&]()
			{
				_thread_err = main->Destroy(*this);
				if (_thread_err != 0)
				{
					return;
				}
				main.reset(up_result.Next.release());
				_thread_err = main->Load(*this);
				_is_loading = false;
			});
	}

stop:
	_window.display();
	_time_taken = _clock.restart().asMicroseconds();
	return _main_error;
}

auto SfmlGameClass::RunBareGame(SfmlGameClass &game) -> int
{
	if ((game._main_error = game.Initialize()) != 0)
	{
		return game._main_error;
	}
	if ((game._main_error = game.main->Load(game)) != 0)
	{
		return game._main_error;
	}
	if ((game._main_error = game.back->Load(game)) != 0)
	{
		return game._main_error;
	}
	do
	{
		game._main_error = game.RunFrame();
	} while (game._main_error == 0 && !game._exit_called);
	return game._main_error != 0 ? game._main_error : game.ShutDown();
}
