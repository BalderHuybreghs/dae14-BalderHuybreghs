#pragma once
#include "Texture.h"
#include <unordered_map>
#include <vector>

class Sprite
{
public:
  Sprite(const Point2f& position, const Point2f& size, const Point2f& frameSize, float msPerFrame, const std::string& resource);

  void Draw(bool debug = false) const;
  void Update(float elapsedSec);

  // Resources are states in this case, since resources will be added 
  // within code, states should remain relatively unchanged
  void SetState(int state, bool reset = false);

  // Adds a resource to this sprite and returns its id
  size_t AddResource(const std::string& resource);

  Point2f GetPosition() const;
  Point2f GetSize() const;

  void SetPosition(const Point2f& position);
  void SetSize(const Point2f& size);

  void SetMirror(bool mirror);

  struct StateInfo
  {
    int id;
    const Texture* texture;
    int frames;
  };
protected:
  Point2f m_Position;
  Point2f m_Size;
  Point2f m_FrameSize;
  float m_MsPerFrame;
  bool m_Mirror;

  // Internal management
  std::vector<StateInfo> m_States;
  StateInfo m_State;
  float m_Time;
  int m_Frame;
};