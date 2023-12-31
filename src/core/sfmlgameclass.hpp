#ifndef CORE
#define CORE

#include <SFML/Config.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <cstdint>
#include <memory>

namespace core
{

/**
 * @brief The settings to create a SFML Game class object
 *
 */
struct SfmlGameSettings
{
	/**
	 * @brief The frame per second count. It is
	 * ignored if `VerticalSyncEnabled` is set to
	 * true
	 *
	 */
	const int FPS;
	/**
	 * @brief Sets Vertical Sychronizatoin
	 *
	 */
	const bool VerticalSyncEnabled;
	/**
	 * @brief The dimensions of the window
	 *
	 */
	unsigned int Width, Height;
	/**
	 * @brief The title of the window
	 *
	 */
	const sf::String Title;
	/**
	 * @brief The color to clear the screen with
	 *
	 */
	sf::Color ClearColor;
	/**
	 * @brief A valid sf::WindowStyle option
	 *
	 */
	const int WindowStyle;
	/**
	 * @brief ContextSettings for the window
	 *
	 */
	const sf::ContextSettings Context;
	const sf::Uint8 UnitDivisor;
	const uint32_t LoadTime;
	const float Gravity, ObstacleSpeed, JumpSpeed;
};

/**
 * @brief An abstract game class that works on
 * a foreground and a background scene. The object
 * of this class also acts as a shared resources
 * between all scenes of the game
 *
 */
class SfmlGameClass
{
	/**
	 * @brief Represents a scene of a game
	 *
	 */

  public:
	class AbstractScene
	{

	  public:
		/**
		 * @brief The structure that represents the data
		 * returned by the Update() function
		 *
		 */
		struct UpdateResult
		{
			/**
			 * @brief Valid = 0 implies that no errors were
			 * encountered. Valid != 0 implies an exception
			 *
			 */
			int Valid;
			/**
			 * @brief If not null, it means that a new scene
			 * is being invoked for swapping with the
			 * foreground scene
			 *
			 */
			std::unique_ptr<AbstractScene> Next;
		};

		/**
		 * @brief Loads or Initializes the game, using the
		 * values from the BareGame object
		 * @param game The shared game object active in this
		 * instance
		 * @return int 0 if successful, otherwise a non-zero
		 * number
		 */
		virtual auto Load(SfmlGameClass &) -> int = 0;

		/**
		 * @brief Recives an event, and takes the required
		 * action
		 *
		 * @param e The event
		 * @return int 0 if successful, otherwise a non-zero
		 * number
		 */
		virtual auto PollEvent(SfmlGameClass &, sf::Event) -> int = 0;

		/**
		 * @brief Updates the game logic, takes game input,
		 * plays audio and sounds effects
		 * @param game The shared game object active in this
		 * instance
		 * @param delta The time in milliseconds taken by
		 * the previous frame
		 * @return UpdateResult The result of the updatation
		 * of this scene
		 */
		virtual auto Update(SfmlGameClass &, long) -> UpdateResult = 0;

		/**
		 * @brief Draws the textures/sprites on the screen
		 *
		 * @param game The shared game object active in this
		 * instance
		 * @param delta The time in milliseconds taken by
		 * the previous frame
		 * @return int 0 if successful, otherwise a non-zero
		 * number
		 */
		virtual auto Draw(SfmlGameClass &, long) -> int = 0;

		/**
		 * @brief Destroys or releases all resources held by
		 * this scene
		 *
		 * @param game The shared game object active in this
		 * instance
		 * @return int 0 if successful, otherwise a non-zero
		 * number
		 */
		virtual auto Destroy(SfmlGameClass &) -> int = 0;

		/**
		 * @brief Destroy the Abstract Scene object
		 *
		 */
		virtual ~AbstractScene() = default;
	};

  protected:
	sf::Clock _clock;
	bool _exit_called, _is_loading;
	std::unique_ptr<AbstractScene> main, back;
	long _time_taken;
	int _main_error, _thread_err;
	/**
	 * @brief Initializes the game and returns the created
	 * scenes that act as the start of the game
	 *
	 * @return StartScenes* A struct containing the two
	 * created scenes
	 */
	auto Initialize() -> int;

	/**
	 * @brief Runs one frame of the initialized game object
	 *
	 * @return int
	 */
	auto RunFrame() -> int;

	/**
	 * @brief Stops the running game instance
	 *
	 * @return int 0 if successful, otherwise a non-zero
	 * number
	 */
	auto ShutDown() -> int;

  public:
	sf::RenderWindow _window;
	/**
	 * @brief The shared settings (shared among/between the scenes) of the game
	 *
	 */
	SfmlGameSettings SharedSettings;

	/**
	 * @brief Construct a new Sfml Game Class object
	 *
	 */
	SfmlGameClass(const SfmlGameSettings &);

	/**
	 * @brief Call for exit. A screen can invoke this
	 *
	 */
	void ExitCall();

	/**
	 * @brief Destroy the Bare Game object
	 *
	 */
	~SfmlGameClass();
	static auto RunBareGame(SfmlGameClass &game) -> int;
};
} // namespace core

#endif
