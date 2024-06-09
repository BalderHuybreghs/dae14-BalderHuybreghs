#pragma once
#include "Texture.h"
#include <unordered_map>

class Sprite
{
public:
  struct StateInfo
  {
    const Texture* texturePtr; // No rule of 3 required despite having a pointer here, this because the manager takes care of cleaning up textures & the pointer here is const
    int frames;
  };

  // Since each animation in Celeste has a variable amount of culumns, instead of asking for the amount of columns,
  // we ask for the size of a single frame
  Sprite(const Point2f& frameSize, float msPerFrame, const std::string& state, const std::string& resource);
  virtual ~Sprite() = default;

  void DrawColor(const Rectf& dstRect, const Color4f& color, bool flipped = false, bool debug = false) const;
  void Draw(const Rectf& dstRect, bool flipped = false, bool debug = false) const;
  void Draw(const Point2f& position, float scale, bool flipped = false, bool debug = false) const;
  void Draw(const Point2f& position, bool flipped = false, bool debug = false) const;
  void Update(float elapsedSec); // Update isn't strictly required for every sprite, this is mostly for animaed sprites

  // Resources are states in this case, since resources will be added 
  // within code, states should remain relatively unchanged
  void SetState(const std::string& state, bool reset = false);

  // Adds a resource to this sprite and returns its id
  void AddState(const std::string& id, const std::string& resource);
  StateInfo GetStateInfo(const std::string& id);

  void SetFrame(int frame);

  bool IsAnimationDone() const;
protected:
  Point2f m_FrameSize;
  float m_MsPerFrame;

  // Internal management
  std::unordered_map<std::string, const StateInfo> m_States;

  std::string m_State;
  float m_Time;
  int m_Frame;
};