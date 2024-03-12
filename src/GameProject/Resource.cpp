#include "pch.h"
#include "Resource.h"
#include "GameDefines.h"

Resource::Resource(Type type, std::string value)
  : m_Type(type), m_Value(value)
{
}

std::string Resource::ToPath() const
{
  std::string extension;
  std::string folder;

  switch (m_Type) {
    case Type::TEXTURE:
      extension = TEXTURE_EXTENSION;
      folder = TEXTURE_FOLDER;
      break;
    case Type::SOUND:
      extension = SOUND_EXTENSION;
      folder = SOUND_FOLDER;
    case Type::MUSIC:
      extension = SOUND_EXTENSION;
      folder = MUSIC_FOLDER;
  }

  return TEXTURE_FOLDER + m_Value + TEXTURE_EXTENSION;
}

const std::string& Resource::GetValue() const
{
  return m_Value;
}

Resource::Type Resource::GetType() const
{
    return m_Type;
}

Resource Resource::Texture(std::string value)
{
    return Resource(Type::TEXTURE, value);
}

Resource Resource::Sound(std::string value)
{
    return Resource(Type::SOUND, value);
}

Resource Resource::Music(std::string value)
{
  return Resource(Type::MUSIC, value);
}
