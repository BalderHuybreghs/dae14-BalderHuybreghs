#pragma once
#include "pch.h"
#include "Vector2f.h"

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
static const std::string PLAYER_JUMP_RESOURCE{ PLAYER_RESOURCE + FS + "jump_fast" };
static const std::string PLAYER_CLIMB_RESOURCE{ PLAYER_RESOURCE + FS + "climb" };
static const std::string PLAYER_RUN_RESOURCE{ PLAYER_RESOURCE + FS + "run_fast" };
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

// Tilemap information
// How many side tiles per row
static const int TILE_SIDE_SIZE{ 4 };

// How many inside tiles per row
static const int TILE_INSIDE_SIZE{ 2 };

// The rows for each side of the tile
static const int TILE_TOP_ROW{ 0 };
static const int TLE_BOTTOM_ROW{ 1 };
static const int TILE_LEFT_ROW{ 2 };
static const int TILE_RIGHT_ROW{ 3 };
static const int TILE_TOP_BOTTOM_ROW{ 4 };
static const int TILE_LEFT_RIGHT_ROW{ 5 };
static const int TILE_LEFT_TOP_RIGHT_ROW{ 6 };
static const int TILE_LEFT_BOTTOM_RIGHT_ROW{ 7 };
static const int TILE_TOP_LEFT_BOTTOM_ROW{ 8 };
static const int TILE_TOP_RIGHT_BOTTOM_ROW{ 9 };
static const int TILE_CENTER{ 10 };
static const int TILE_TOP_CORNER_LEFT{ 11 };
static const int TILE_TOP_CORNER_RIGHT{ 11 };
static const int TILE_BOTTOM_CORNER_LEFT{ 13 };
static const int TILE_BOTTOM_CORNER_RIGHT{ 14 };

// Animation information
static const int FRAME_SIZE{ 32 };
static const float FRAMES_PER_SECOND{ 1 / 8.f };

// Playing information
static const Vector2f GRAVITY{0.f, -500.f};
static const Vector2f TERMINAL_VELOCITY{ 0.f, 0.f }; // TODO: Determine terminal velocity