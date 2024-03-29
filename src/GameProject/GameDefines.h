#pragma once
#include "pch.h"
#include "Vector2f.h"

static const std::string PROJECT_NAME{ "GameProject - Huybreghs, balder - 1DAE14" };

static const char FS{ '\\' };

// File extensions
static const std::string TEXTURE_EXTENSION{ ".png" };
static const std::string SOUND_EXTENSION{ ".mp3" };
static const std::string FONT_EXTENSION{ ".ttf" };

// cev, a combination of celeste and level, very original, I know
static const std::string LEVEL_EXTENSION{ ".cev" };

// Game folders
static const std::string TEXTURE_FOLDER{ "texture" };
static const std::string SOUND_FOLDER{ "sound" };
static const std::string MUSIC_FOLDER{ "music" };
static const std::string LEVEL_FOLDER{ "level" };
static const std::string FONT_FOLDER{ "font" };

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

static const std::string SYNE_FONT{ "syne" };

static const std::string LOADING_RESOURCE{ "loading" };
static const std::string LOGO_RESOURCE{ GUI_FOLDER + FS + "logo" };

static const std::string LEVEL1_NAME{ "forsaken_city" };

// Screen info
static const float WINDOW_WIDTH{ 1280.f };
static const float WINDOW_HEIGHT{ 800.f };

// Editor related settings
static const float MOUSE_DRAG_BORDER_MARGIN_HORIZONTAL{ WINDOW_WIDTH * 0.2f };
static const float MOUSE_DRAG_BORDER_MARGIN_VERTICAL{ WINDOW_HEIGHT * 0.2f };
static const float SCROLL_ZOOM_FACTOR{ 0.05f };
static const float CAMERA_DRAG_SPEED{ 1000 };

// Tilemap information
// How big a single tile is
static const int TILE_SIZE{ 8 };

// How many side tiles per row
static const int TILE_SIDE_SIZE{ 4 };
static const int TILE_COLUMN_SIZE{ 6 };

static const int TILE_BOTTOM_CORNER_RIGHT{ 14 };

static const int BACKGROUND_TILES_SIZE{ 12 };
static const int FOREGROUND_TILES_SIZE{ 23 };

static const std::string BACKGROUND_TILES[]{ 
  "bg_brick", 
  "bg_brick_ruined", 
  "bg_cliffside", 
  "bg_core", 
  "bg_dirt", 
  "bg_lost_levels", 
  "bg_reflection", 
  "bg_snow", 
  "bg_summit", 
  "bg_temple_a", 
  "bg_temple", 
  "bg_wood"
};

static const std::string FOREGROUND_TILES[]{
  "cement",
  "cliffside",
  "core",
  "deadgrass",
  "dirt",
  "girder",
  "grass",
  "lostlevels",
  "pool_edges",
  "reflection",
  "reflection_alt",
  "rock",
  "scifi",
  "snow",
  "star_jump",
  "stone",
  "summit",
  "summit_no_snow",
  "template",
  "temple_a",
  "temple_b",
  "tower",
  "wood",
  "wood_stone_edges"
};

// Animation information
static const int FRAME_SIZE{ 32 };
static const float FRAMES_PER_SECOND{ 1 / 8.f };

// Playing information
static const Vector2f GRAVITY{0.f, -2000.f};
static const Vector2f TERMINAL_VELOCITY{ 0.f, 0.f }; // TODO: Determine terminal velocity
static const Color4f BACKGROUND_COLOR{ 11 / 255.f, 16 / 255.f, 19 / 255.f, 1.f }; // The absolute background color is always the same single color in Celeste