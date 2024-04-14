#pragma once
#include "BaseGame.h"
#include "InputManager.h"
#include "ScreenManager.h"

class Game : public BaseGame
{
public:
	explicit Game(const Window& window, GameScreen* initialScreenPtr);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw() const override;

	// Event handling
  void ProcessInputEvent(const SDL_Event& e) override;
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;
  void ProcessMouseWheelEvent(const SDL_MouseWheelEvent& e) override;

private:
	// Functions
	void Initialize(GameScreen* initialScreenPtr);
	void Cleanup( );
  void ClearBackground(Color4f color = Color4f{ 0.f, 0.f, 0.f, 1.f}) const;

  // Variables
  ScreenManager* m_ScreenManagerPtr;
};