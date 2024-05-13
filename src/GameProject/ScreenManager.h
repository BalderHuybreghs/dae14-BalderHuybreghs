#pragma once
#include "pch.h"
#include "InputManager.h"

class GameScreen; // Forward declaration

// Manages the current screen, can have nested screens to save resources across screens
class ScreenManager final
{
public:
  ScreenManager();

  // Prevent making copies of this class, application of rule of 3
  // I can't imagine a scenario where you would need to copy a screen manager
  ScreenManager(const ScreenManager& other) = delete;
  ScreenManager& operator=(const ScreenManager& other) = delete;
  ~ScreenManager();

  ScreenManager(const ScreenManager&& other) = delete;
  ScreenManager& operator=(const ScreenManager&& other) = delete;

  void        SetScreen(GameScreen* newScreenPtr);
  void        SetParent(ScreenManager* parentPtr);

  GameScreen* GetScreen();
  ScreenManager* GetParent(); // In case you have screenception

private:
  ScreenManager* m_ParentPtr;
  GameScreen* m_PreviousScreenPtr;
  GameScreen* m_CurrentScreenPtr;
};

// An abstract GameScreen class that all sceens can implement
class GameScreen
{
public:
  // Again, no copying of the GameScreen class
  GameScreen(const GameScreen& other) = delete;
  GameScreen& operator=(const GameScreen& other) = delete;

  void SetScreenManager(ScreenManager* screenManagerPtr);

  // Gets called when the screen resources should be initialized
  virtual void Initialize()                                      {};

  // The screenManagerPtr should not be deleted because it
  // is part of another class
  virtual ~GameScreen();

  // General game functions
  virtual void Draw()                                            {};
  virtual void Update(float elapsedSec) {};

  // OnInputEvent only called to update t he InputManager
  void OnInputEvent(const SDL_Event& e);

  virtual void OnKeyDownEvent(const SDL_KeyboardEvent& key)      {};
  virtual void OnKeyUpEvent(const SDL_KeyboardEvent& key)        {};
  virtual void OnMouseMotionEvent(const SDL_MouseMotionEvent& e) {};
  virtual void OnMouseDownEvent(const SDL_MouseButtonEvent& e)   {};
  virtual void OnMouseUpEvent(const SDL_MouseButtonEvent& e)     {};
  virtual void OnMouseWheelEvent(const SDL_MouseWheelEvent& e)   {};

protected:
  GameScreen();

  ScreenManager* m_ScreenManagerPtr;
  InputManager* m_InputManagerPtr;
};