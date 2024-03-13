#include "pch.h"
#include "ResourceUtils.h"
#include "GameDefines.h"

std::string ResourceUtils::ResourceToImagePath(const std::string& resource)
{
  return TEXTURE_FOLDER + '\\' + resource + TEXTURE_EXTENSION;
}

std::string ResourceUtils::ResourceToSoundPath(const std::string& resource)
{
  return SOUND_FOLDER + '\\' + resource + SOUND_EXTENSION;
}

std::string ResourceUtils::ResourceToMusicPath(const std::string& resource)
{
  return MUSIC_FOLDER + '\\' + resource + SOUND_EXTENSION;
}