#include "pch.h"
#include "Hair.h"
#include "GameDefines.h"
#include "TextureManager.h"

Hair::Hair(const Point2f& position, int parts, float startingSize)
  : m_Goal(position)
{
  m_HairTexture = TextureManager::Instance()->GetTexture(HAIR_RESOURCE);
  float length{ startingSize / 2.f }; // We can safely asume that the hair is the same size in both width and height
  m_HairLimb = new Limb(position, 1, length);

  for (size_t i = 1; i < parts; ++i)
  {
    m_HairLimb->AddJoint(new Joint(position, length * (1 - i / float(parts))));
  }

  m_Bangs = new Sprite(Point2f{ m_Goal.x - startingSize / 2.f, m_Goal.y - startingSize / 2.f }, Point2f(startingSize, startingSize), Point2f{ BANGS_FRAME_SIZE, BANGS_FRAME_SIZE }, 0, BANGS_RESOURCE);
}

Hair::~Hair()
{
  delete m_HairLimb;
  delete m_Bangs;
}

void Hair::Draw(bool debug) const
{
  const int joints{ (int)m_HairLimb->Joints()->size()}; // Used to calculate the size of each hair piece
  
  for (const Joint* joint : *m_HairLimb->Joints()) {
    const Point2f middle{ joint->GetMiddle() };
    const float length{ joint->GetLength() * 2.f };

    const Rectf dstRect{
      middle.x - length / 2.f,
      middle.y - length / 2.f,
      length,
      length
    };

    m_HairTexture->Draw(dstRect);
  }

  m_Bangs->Draw();

  // Debug symbols should be drawn before the actual hair is drawn (obviously :) )
  if (debug) {
    m_HairLimb->Draw(); // This will draw a bunch of mysterious debug symbols
  }
}

void Hair::Update(float elapsedSec)
{
  const Point2f start{ m_HairLimb->GetAnchor() };

  const float distance{ SetEnd(start) }; // Get the distance from the end

  // If the distance is too far, recalculate the goal
  if (distance > 0.f) {
    // Calculate the direction to move to
    const Point2f end{ m_HairLimb->GetEnd() };
    const Vector2f direction{ m_Goal - start };
    const Point2f newPos{ start + direction.Normalized() * distance };

    m_HairLimb->Set(newPos, m_Goal);
  }
}

Point2f Hair::GetPosition() const
{
    return m_HairLimb->GetAnchor(); // The position is the anchor point of the hair limb
}

float Hair::SetEnd(const Point2f& position)
{
  return m_HairLimb->Set(position, m_Goal);
}

void Hair::SetGoal(const Point2f& goal)
{
  m_Goal = goal;

  const Point2f bangsSize{ m_Bangs->GetSize() };
  m_Bangs->SetPosition(Point2f{ m_Goal.x - bangsSize.x / 2.f, m_Goal.y - bangsSize.y / 2.f }); // Update the bangs position
}
