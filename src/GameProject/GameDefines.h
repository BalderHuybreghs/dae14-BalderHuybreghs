#pragma once
#include "pch.h"
#include "Vector2f.h"
#include "ParticleEmitter.h"

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
static const std::string CHARACTER_FOLDER{ GAMEPLAY_RESOURCE + FS + "character" };
static const std::string PLAYER_FOLDER{ CHARACTER_FOLDER + FS + "player" };
static const std::string HAIR_FOLDER{ PLAYER_FOLDER + FS + "hair" };

static const std::string TILEMAP_FOLDER{ "tilesets" };
static const std::string TILEMAP_BG_PREFIX{ "bg_" };

static const std::string PLAYER_IDLE_RESOURCE{ PLAYER_FOLDER + FS + "idle" };
static const std::string PLAYER_FALL_RESOURCE{ PLAYER_FOLDER + FS + "fall" };
static const std::string PLAYER_SLIDE_RESOURCE{ PLAYER_FOLDER + FS + "hold" };
static const std::string PLAYER_EDGE_RESOURCE{ PLAYER_FOLDER + FS + "edge" };
static const std::string PLAYER_DANGLING_RESOURCE{ PLAYER_FOLDER + FS + "dangling" };
static const std::string PLAYER_DUCK_RESOURCE{ PLAYER_FOLDER + FS + "duck" };
static const std::string PLAYER_JUMP_RESOURCE{ PLAYER_FOLDER + FS + "jump_fast" };
static const std::string PLAYER_CLIMB_RESOURCE{ PLAYER_FOLDER + FS + "climb" };
static const std::string PLAYER_RUN_RESOURCE{ PLAYER_FOLDER + FS + "run_fast" };
static const std::string PLAYER_WALK_RESOURCE{ PLAYER_FOLDER + FS + "walk" };
static const std::string PLAYER_PUSH_RESOURCE{ PLAYER_FOLDER + FS + "push" };

static const std::string BANGS_RESOURCE{ HAIR_FOLDER + FS + "bangs" };
static const std::string HAIR_RESOURCE{ HAIR_FOLDER + FS + "hair" };

static const std::string SYNE_FONT{ "syne" };

static const std::string LOADING_RESOURCE{ "loading" };
static const std::string LOGO_RESOURCE{ GUI_FOLDER + FS + "logo" };

static const std::string LEVEL1_NAME{ "forsaken_city" };

// Screen info, Celestes aspect ratio would be 16:9
static const float WINDOW_WIDTH{ 1280.f };
static const float WINDOW_HEIGHT{ 720.f };

// The scale of pixels on the screen, source https://aran.ink/posts/celeste-tilesets for more information on aspect ratio and size etc
static const float PIXEL_SCALE{ WINDOW_WIDTH / 320.f }; 

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
  "bg_temple_b", 
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
static const int PLAYER_FRAME_SIZE{ 32 };
static const int HAIR_FRAME_SIZE{ 10 };
static const int BANGS_FRAME_SIZE{ 10 };
static const float FRAMES_PER_SECOND{ 1 / 8.f };

// Playing information
static const Vector2f GRAVITY{0.f, -2000.f};
static const Vector2f TERMINAL_VELOCITY{ 0.f, 0.f }; // TODO: Determine terminal velocity
static const Color4f BACKGROUND_COLOR{ 11 / 255.f, 16 / 255.f, 19 / 255.f, 1.f }; // The absolute background color is always the same single color in Celeste
static const float PLAYER_ACCELERATION{ 500.f }; // The rate at which the player accelerates when moving left / right
static const float PLAYER_JUMP_POTENTIAL{ 500.f }; // The maximu  m potential jump force the player may reach
static const float PLAYER_JUMP_FORCE{ 100.f }; // The jump force applied to the player until it reaches max potential
static const float PLAYER_DASH_FORCE{ 1500.f }; // The force behind a player dash
static const float PLAYER_SCALE{ PIXEL_SCALE * PLAYER_FRAME_SIZE };
static const float HAIR_SCALE{ PIXEL_SCALE * HAIR_FRAME_SIZE };
static const float PLAYER_HEIGHT{ 11 * PIXEL_SCALE };
static const float PLAYER_LOW{ 10 * PIXEL_SCALE };
static const float PLAYER_HAIR_START{ 16 * PIXEL_SCALE };
static const float PLAYER_BODY_WIDTH{ 8 * PIXEL_SCALE };
static const float PLAYER_BODY_HEIGHT{ 12 * PIXEL_SCALE };

// The following stamine values are based on https://celeste.ink/wiki/Stamina, they're not numbers I pulled out my thumb
static const float PLAYER_BASE_STAMINA{ 110.f };
static const float STAMINA_FLASH_RED{ 20.f };
static const float HOLDING_STAMINA_COST{ 10.f };
static const float CLIMBING_STAMINA_COST{ 45.45f };
static const float CLIMB_JUMP_STAMINA_COST{ 27.5f };

// Particle information
static const float SCREEN_EMISSION_ZONE_WIDTH{ 100.f };
static const float SCREEN_EMISSION_ZONE_HEIGHT{ WINDOW_HEIGHT };

static const float SNOW_PARTICLE_SIZE{ PIXEL_SCALE };

static const Color4f SNOW_PARTICLE_COLOR1{ 1.f, 1.f, 1.f, 1.f };
static const Color4f SNOW_PARTICLE_COLOR2{ 0.f, 0.f, 0.2f, 1.f };
static const Color4f SNOW_PARTICLE_COLOR3{ 0.5f, 0.5f, 0.5f, 1.f };

static const EmitterSpawnInfo SNOW_PARTICLE_INFO{
    600.f,     // The minimum force on a particle
    1000.f,    // The maximum force on a particle
    3.f, // The minimum rotation in radians
    5.f, // The maximum rotation in radians
    10.f,     // The minimum lifetime for a particle in seconds
    10.f,     // The maximum lifetime for a particle in seconds
    5,        // The minimum amount of particles to spawn in a run
    10,        // The maximum amount of particles to spawn in a run
    500,      // The maximum amount of particles allowed at a time
    .1f,      // The minimum spawning delay in seconds
    .5f       // The maximum spawning delay in seconds
};