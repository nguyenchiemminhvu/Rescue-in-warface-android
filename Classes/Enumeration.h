#pragma once

////////////////////////////////////////
// Transition time

#define TRANSITION_FADE_TIME 2.0F
#define TRANSITION_PROGRESS_IN_OUT 1.0F
#define AVERAGE_DELTA_TIME 0.0165F

////////////////////////////////////////
// Schedule time

#define SCHEDULE_TRANSITION_TIME 3.0F
#define HUD_WARNING_DELAY_TIME 0.5F

////////////////////////////////////////
// Enemy

#define TOWER_SPAWN_DURATION 15.0F
#define ANTI_AIRCRAFT_SPAWN_DURATION 8.0F
#define ANTI_AIRCRAFT_RELOAD_DURATION 3.0F
#define RANGE_MISSILE_SPAWN_DURATION 4.0F
#define MISSILE_FLYING_DURATION 2.0F
#define TOWER_BULLET_DURATION 3.0F
#define TOWER_RELOAD_DURATION 2.5F

///////////////////////////////////////
// Platform thickness

#define PLATFORM_THICKNESS 168.0F

///////////////////////////////////////
// Introduction scene durations

#define HELICOPTER_COMING_DURATION 7
#define HELICOPTER_LANDING_DURATION 5
#define SOLDIER_MOVING_DURATION 3
#define HELICOPTER_LEAVING_DURATION 6

///////////////////////////////////////
// Game scene distance

#define BACKGROUND_SHIFT_DISTANCE 100.0F
#define GAME_SCENE_TOTAL_DISTANCE 50
#define GASCAN_FALLING_DURATION 7.0F

///////////////////////////////////////
// Player

#define PLAYER_MAX_FUEL 20
#define GASCAN_SPAWN_DURATION 20.0F

///////////////////////////////////////
// Boss

#define MINI_BOSS_PREPARE_DURATION 3.0F
#define BIG_BOSS_PREPARE_DURATION 3.0F

////////////////////////////////////////
// Collision bitmask

enum class CollisionBitmask
{
	PLAYER_COLLISION_BITMASK,
	PLAYER_BULLET_BITMASK,
	OBSTACLE_COLLISION_BITMASK,
	ENEMY_BULLET_BISMASK,
	ENEMY_COLLISION_BITMASK,
	GASCAN_COLLISION_BITMASK,
	BOSS_COLLISION_BITMASK
};
