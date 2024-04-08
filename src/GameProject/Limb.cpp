#include "pch.h"
#include "Limb.h"

Limb::Limb(const Point2f& anchor, const Point2f& goal, int joints, float length)
  : m_Anchor(anchor)
{
  // Calculates the size each joint needs to be to cover the total length
  float sizePerJoint{ length / joints };

  // Reserve space
  m_Joints.reserve(joints);

  // Create all the joints on the anchor
  for (size_t jointIndex = 0; jointIndex < joints; jointIndex++)
  {
    m_Joints.push_back(new Joint(m_Anchor, sizePerJoint));
  }

  // Makes the joint face the correct way
  Follow(goal);
}

Limb::Limb(const Point2f& anchor, int joints, float length)
  : Limb(anchor, Point2f(0, 0), joints, length)
{
}

Limb::Limb(const Limb& other)
{
  m_Anchor = other.GetAnchor();

  // Copy the joints over
  const std::vector<Joint*>* joints{ other.Joints() };
  
  for (const Joint* joint : *joints) {
    m_Joints.push_back(new Joint(*joint));
  }
}

Limb& Limb::operator=(const Limb& other)
{
  if (this == &other)
    return *this;

  // Copy anchor
  m_Anchor = other.GetAnchor();

  // Delete existing joints
  for (Joint* joint : m_Joints)
  {
    delete joint;
  }
  m_Joints.clear();

  // Copy new joints
  const std::vector<Joint*>* otherJoints = other.Joints();
  for (const Joint* joint : *otherJoints) {
    m_Joints.push_back(new Joint(*joint));
  }

  return *this;
}

Limb::~Limb()
{
  for (Joint* joint : m_Joints)
  {
    delete joint;
  }
}

void Limb::Draw()
{
  for (Joint* joint : m_Joints)
  {
    joint->Draw();
  }

  SetColor(Color4f(0.f, 0.f, 1.f, 1.f));
  FillEllipse(m_Anchor, 5.f, 5.f);

  SetColor(Color4f(1.f, 0.f, 1.f, 1.f));
  FillEllipse(GetEnd(), 5.f, 5.f);

  SetColor(Color4f(1.f, 1.f, 0.f, 1.f));
  DrawLine(m_Anchor, GetEnd());
}

float Limb::Set(const Point2f& anchor, const Point2f& goal)
{
  m_Anchor = anchor;

  // Set is basically update, but only needs to be called when the limb values are changed
  return Follow(goal);
}

float Limb::Follow(const Point2f& goal)
{
  // Make all joints follow the heading joint
  for (size_t jointIndex = 0; jointIndex < m_Joints.size(); jointIndex++)
  {
    // Incase the limb has no joints (for some vague reason)
    if (jointIndex == 0) {
      m_Joints.at(jointIndex)->Follow(goal);
      continue;
    }

    m_Joints.at(jointIndex)->Follow(m_Joints.at(jointIndex - 1)->GetPosition());
  }

  // Move the arm to it's anchoring position
  const size_t lastIndex{ m_Joints.size() - 1 };
  Joint* last{ m_Joints.at(lastIndex) };
  last->SetPosition(m_Anchor);
  last->Calculate();

  for (int jointIndex = lastIndex -1; jointIndex >= 0; jointIndex--)
  {
    Joint* joint{ m_Joints.at(jointIndex) };
    Joint* next{ m_Joints.at(size_t(jointIndex + 1)) };

    joint->SetPosition(next->GetEnd());
    joint->Calculate();
  }

  return goal.Distance(GetEnd());
}

Point2f Limb::GetAnchor() const
{
  return this->m_Anchor;
}

Point2f Limb::GetEnd() const
{
  return m_Joints.front()->GetEnd();
}

void Limb::AddJoint(Joint* joint)
{
  m_Joints.push_back(joint);
}

const std::vector<Joint*>* Limb::Joints() const
{
  return &this->m_Joints;
}
