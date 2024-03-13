#pragma once
#include "GameObject.h"
#include "Texture.h"
#include <unordered_map>
#include <vector>

class Sprite : public GameObject
{
public:
  Sprite(const Point2f& position, const Point2f& size, const Point2f& frameSize, float msPerFrame, const std::string& resource);

  void Draw(bool debug = false) const override;
  void Update(float elapsedSec) override;

  // Resources are states in this case, since resources will be added 
  // within code, states should remain relatively unchanged
  void SetResource(int state);

  // Adds a resource to this sprite and returns its id
  int AddResource(const std::string& resource);

  Point2f GetPosition() const;
  Point2f GetSize() const;

  void SetPosition(const Point2f& position);
  void SetSize(const Point2f& size);

  struct StateInfo
  {
    const Texture* texture;
    int frames;
  };
protected:
  Point2f m_Position;
  Point2f m_Size;
  Point2f m_FrameSize;
  float m_MsPerFrame;

  // Internal management
  std::vector<StateInfo> m_States;
  StateInfo m_State;
  float m_Time;
  int m_Frame;
};