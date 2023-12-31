#include "utils.hpp"

utils::KeyListener::KeyListener(std::initializer_list<sf::Keyboard::Key> arg_keys)
	: keys(arg_keys.size())
	, values(arg_keys.size())
{
	for (auto key : arg_keys)
	{
		keys.push_back(key);
		values.push_back(KeyState::Long_Release);
	}
}

auto utils::KeyListener::Find(int code) -> int
{
	int idx = 0;
	const int limit = keys.size();
	while (keys[idx] != code && idx < limit)
	{
		idx++;
	}
	return idx >= limit ? -1 : idx;
}

auto utils::KeyListener::Update(int code, bool press) -> int
{
	int idx = Find(code);
	if (idx == -1)
	{
		return idx;
	}
	auto state = values[idx];
	if (state == KeyState::Invalid)
	{
		return 1;
	}
	if (state == KeyState::Long_Release || state == KeyState::Just_Release)
	{
		state = press ? KeyState::Just_Pressed : KeyState::Long_Release;
	}
	else
	{
		state = press ? KeyState::Long_Pressed : KeyState::Just_Release;
	}
	values[idx] = state;
	return 0;
}

auto utils::KeyListener::ViewKey(int code) -> KeyState
{
	int idx = Find(code);
	return idx >= 0 ? values[idx] : KeyState::Invalid;
}

auto utils::KeyListener::FrameEnd() -> int
{
	for (int i = 0, limit = keys.size(); i < limit; i++)
	{
		auto state = values[i];
		if (state == KeyState::Just_Pressed)
		{
			state = KeyState::Long_Pressed;
		}
		if (state == KeyState::Just_Release)
		{
			state = KeyState::Long_Release;
		}
		values[i] = state;
	}
	return 0;
}
