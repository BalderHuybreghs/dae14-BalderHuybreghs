#pragma once
#include "pch.h"

class GameScreen; // Forward declaration

// Manages the current screen, can have nested screens to save resources across screens
class ScreenManager
{
public:
  ScreenManager();
  ~ScreenManager();

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
  void SetScreenManager(ScreenManager* screenManagerPtr);

  // Gets called when the screen resources should be initialized
  virtual void Initialize()                                      {};

  // The screenManagerPtr should not be deleted because it
  // is part of another class
  virtual ~GameScreen()                                          {};

  // General game functions
  virtual void Draw()                                            {};
  virtual void Update(float elapsedSec)                          {};

  // Key events
  virtual void OnKeyDownEvent(const SDL_KeyboardEvent& key)      {};
  virtual void OnKeyUpEvent(const SDL_KeyboardEvent& key)        {};
  virtual void OnMouseMotionEvent(const SDL_MouseMotionEvent& e) {};
  virtual void OnMouseDownEvent(const SDL_MouseButtonEvent& e)   {};
  virtual void OnMouseUpEvent(const SDL_MouseButtonEvent& e)     {};
  virtual void OnMouseWheelEvent(const SDL_MouseWheelEvent& e)   {};

protected:
  GameScreen();

  ScreenManager* m_ScreenManagerPtr;
};