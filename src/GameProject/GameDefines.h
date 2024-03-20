#pragma once
#include "pch.h"

static const std::string PROJECT_NAME{ "GameProject - Huybreghs, balder - 1DAE14" };

static const char FS{ '\\' };

// File extensions
static const std::string TEXTURE_EXTENSION{ ".png" };
static const std::string SOUND_EXTENSION{ ".mp3" };

// cev, a combination of celeste and level, very original, I know
static const std::string LEVEL_EXTENSION{ ".cev" };

// Game folders
static const std::string TEXTURE_FOLDER{ "texture" };
static const std::string SOUND_FOLDER{ "sound" };
static const std::string MUSIC_FOLDER{ "music" };
static const std::string LEVEL_FOLDER{ "level" };\
static const std::string MISSING_TEXTURE_TEXTURE{ TEXTURE_FOLDER + FS + "missing_texture" + TEXTURE_EXTENSION };

static const std::string GAMEPLAY_RESOURCE{ "gameplay" };
static const std::string GUI_FOLDER{ "gui" };
static const std::string CHARACTER_RESOURCE{ GAMEPLAY_RESOURCE + FS + "character" };
static const std::string PLAYER_RESOURCE{ CHARACTER_RESOURCE + FS + "player" };

static const std::string TILEMAP_FOLDER{ "tilesets" };
static const std::string TILEMAP_BG_PREFIX{ "bg_" };

static const std::string PLAYER_IDLE_RESOURCE{ PLAYER_RESOURCE + FS + "idle" };
static const std::string PLAYER_FALL_RESOURCE{ PLAYER_RESOURCE + FS + "fall" };
static const std::string PLAYER_EDGE_RESOURCE{ PLAYER_RESOURCE + FS + "edge" };
static const std::string PLAYER_DANGLING_RESOURCE{ PLAYER_RESOURCE + FS + "dangling" };
static const std::string PLAYER_DUCK_RESOURCE{ PLAYER_RESOURCE + FS + "duck" };
static const std::string PLAYER_JUMP_RESOURCE{ PLAYER_RESOURCE + FS + "jump_slow" };
static const std::string PLAYER_CLIMB_RESOURCE{ PLAYER_RESOURCE + FS + "climb" };
static const std::string PLAYER_RUN_RESOURCE{ PLAYER_RESOURCE + FS + "run_slow" };
static const std::string PLAYER_WALK_RESOURCE{ PLAYER_RESOURCE + FS + "walk" };

static const std::string LOADING_RESOURCE{ "loading" };
static const std::string LOGO_RESOURCE{ GUI_FOLDER + FS + "logo" };

static const std::string LEVEL1_NAME{ "forsaken_city" };

// Screen info
static const float WINDOW_WIDTH{ 1280.f };
static const float WINDOW_HEIGHT{ 800.f };

// Texture information
static const int TILE_SIZE{ 8 };

// Editor related settings
static const float MOUSE_DRAG_BORDER_MARGIN_HORIZONTAL{ WINDOW_WIDTH * 0.2f };
static const float MOUSE_DRAG_BORDER_MARGIN_VERTICAL{ WINDOW_HEIGHT * 0.2f };
static const float SCROLL_ZOOM_FACTOR{ 0.05f };
static const float CAMERA_DRAG_SPEED{ 1000 };

// Animation information
static const int FRAME_SIZE{ 32 };
static const float FRAMES_PER_SECOND{ 1 / 10.f };