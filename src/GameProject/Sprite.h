#pragma once
#include "Texture.h"
#include <unordered_map>
#include <vector>

class Sprite
{
public:
  Sprite(const Point2f& position, const Point2f& size, const Point2f& frameSize, float msPerFrame, const std::string& resource);
  virtual ~Sprite() = default;

  void Draw(bool debug = false) const;
  void Update(float elapsedSec); // Update isn't strictly required for every sprite, this is mostly for animaed sprites

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

  void SetFrame(int frame);

  struct StateInfo
  {
    int id;
    const Texture* texture; // No rule of 3 required despite having a pointer here, this because the manager takes care of cleaning up textures & the pointer here is const
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