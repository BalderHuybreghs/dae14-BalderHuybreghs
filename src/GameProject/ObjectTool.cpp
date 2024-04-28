#include "pch.h"
#include "ObjectTool.h"
#include "ObjectManager.h"

ObjectTool::ObjectTool(Level* levelPtr, const InputManager* inputManagerPtr)
  : EditTool("Object", inputManagerPtr), m_LevelPtr(levelPtr), m_CurrentObjectPtr(nullptr)
{
  if (ObjectManager::GetInstance()->GetObjectCount() > 0) {
    m_CurrentObjectPtr = new ObjectBlueprint(0, Point2f{});
  }
}

void ObjectTool::Draw(const Camera* cameraPtr) const
{
  EditTool::Draw(cameraPtr);

  // Man I should really start implementing some UI drawing features
  // for some neater controls etc.. Soon :)
  if (m_CurrentObjectPtr != nullptr) {
    m_CurrentObjectPtr->Draw();
  }
}

void ObjectTool::Update(float elapsedSec, const Rectf& hoveringTile)
{
  if (m_CurrentObjectPtr != nullptr) {
    m_CurrentObjectPtr->SetPosition(Point2f{ hoveringTile.left, hoveringTile.bottom });
  }

  if (m_InputManagerPtr->IsMouseDown(SDL_BUTTON_LEFT) && m_CurrentObjectPtr != nullptr) {
    m_LevelPtr->AddBlueprint(ObjectBlueprint(*m_CurrentObjectPtr));
  }
}

void ObjectTool::OnMouseWheelEvent(const SDL_MouseWheelEvent& e)
{
  // In the case no objects are registered
  if (m_CurrentObjectPtr == nullptr) {
    return;
  }

  const int maxId{ ObjectManager::GetInstance()->GetObjectCount() };
  int id{ m_CurrentObjectPtr->GetObjectId() };

  if (e.y > 0) // Scroll up
  {
    id = id > 0 ? id - 1 : 0;
  } else if (e.y < 0) // Scroll down
  {
    id = id < maxId - 1 ? id + 1 : id;
  }

  if (ObjectManager::GetInstance()->ObjectWithIdExists(id)) {
    m_CurrentObjectPtr->SetObjectId(id);
  }
}
