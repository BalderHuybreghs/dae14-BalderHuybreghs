#pragma once
class Resource
{
public:
  enum Type
  {
    TEXTURE,
    SOUND,
    MUSIC
  };

  Resource(Type type, std::string value);

  std::string ToPath() const;
  const std::string& GetValue() const;
  Type GetType() const;

  static Resource Texture(std::string value);
  static Resource Sound(std::string value);
  static Resource Music(std::string value);

protected:
  Type m_Type;
  std::string m_Value;
};