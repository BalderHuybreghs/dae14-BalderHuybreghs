#pragma once
#include "BaseGame.h"
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
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;
  void ProcessMouseWheelEvent(const SDL_MouseWheelEvent& e) override;

private:
  // Variables
  ScreenManager* m_ScreenManagerPtr;

	// FUNCTIONS
	void Initialize(GameScreen* initialScreenPtr);
	void Cleanup( );
	void ClearBackground( ) const;
};