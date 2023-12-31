#ifndef UTILS
#define UTILS

#include <SFML/Window/Keyboard.hpp>
#include <initializer_list>
#include <vector>
namespace utils
{

/**
 * @brief A given key can be in any of the following states
 *
 */
enum class KeyState
{
	/**
	 * @brief This key is in 'Released' state
	 * in this frame and the previous frame
	 *
	 */
	Long_Release,
	/**
	 * @brief This key is in 'Pressed' state
	 * in this frame, and has just changed from
	 * 'Released' state since previous frame
	 *
	 */
	Just_Pressed,
	/**
	 * @brief This key is in 'Released' state
	 * in this frame, and has just changed from
	 * 'Pressed' state since previous frame
	 *
	 */
	Just_Release,
	/**
	 * @brief This key is in 'Pressed' state
	 * in this frame and the previous frame
	 *
	 */
	Long_Pressed,
	/**
	 * @brief Invalid state, represents any
	 * excpetion caught by the system
	 *
	 */
	Invalid,
};
/**
 * @brief An object that tracks for the state of keyboard keys
 * and updates them for each frame
 *
 */
class KeyListener
{
	auto Find(int code) -> int;
	std::vector<int> keys;
	std::vector<KeyState> values;

  public:
	KeyListener(std::initializer_list<sf::Keyboard::Key> arg_keys);
	auto Update(int code, bool press) -> int;
	auto ViewKey(int code) -> KeyState;
	auto FrameEnd() -> int;
};
} // namespace utils

#endif