#pragma once

////////////////////////////////////////
// Transition time

const float	TRANSITION_FADE_TIME = 2.0F;
const float TRANSITION_PROGRESS_IN_OUT = 1.0F;
const float AVERAGE_DELTA_TIME = 0.0165F;

////////////////////////////////////////
// Schedule time

const float SCHEDULE_TRANSITION_TIME = 3.0F;
const float HUD_WARNING_DELAY_TIME = 0.5F;

////////////////////////////////////////
// Enemy

const float TOWER_SPAWN_DURATION = 15.0F;
const float ANTI_AIRCRAFT_SPAWN_DURATION = 8.0F;
const float ANTI_AIRCRAFT_RELOAD_DURATION = 3.0F;
const float RANGE_MISSILE_SPAWN_DURATION = 4.0F;
const float MISSILE_FLYING_DURATION = 1.5F;
const float TOWER_BULLET_DURATION = 3.0F;
const float TOWER_RELOAD_DURATION = 2.5F;

///////////////////////////////////////
// Thickness

const float PLATFORM_THICKNESS = 168.0F;

///////////////////////////////////////
// Introduction scene durations

const float HELICOPTER_COMING_DURATION = 7;
const float HELICOPTER_LANDING_DURATION = 5;
const float SOLDIER_MOVING_DURATION = 3;
const float HELICOPTER_LEAVING_DURATION = 6;

///////////////////////////////////////
// Game scene distance

const float BACKGROUND_SHIFT_DISTANCE = 100.0F;
const int GAME_SCENE_TOTAL_DISTANCE = 50;
const float GASCAN_FALLING_DURATION = 7.0F;

///////////////////////////////////////
// Player

const int PLAYER_MAX_FUEL = 20;
const float GASCAN_SPAWN_DURATION = 20.0F;

///////////////////////////////////////
// Boss

const float MINI_BOSS_PREPARE_DURATION = 3.0F;
const float BIG_BOSS_PREPARE_DURATION = 3.0F;
const int MINI_BOSS_MAX_HEALTH = 50;

////////////////////////////////////////
// Collision bitmask

enum class CollisionBitmask
{
	PLAYER_COLLISTION_BITMASK,
	PLAYER_BULLET_BITMASK,
	OBSTACLE_COLLISION_BITMASK,
	ENEMY_BULLET_BISMASK,
	ENEMY_COLLISION_BITMASK,
	GASCAN_COLLISION_BITMASK,
	BOSS_COLLISION_BITMASK
};
