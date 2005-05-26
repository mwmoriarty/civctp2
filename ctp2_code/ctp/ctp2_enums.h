//
// C++ Interface: ctp2_enums
//
// Description: 
//
//
// Author:  <ctp2source@apolyton.net>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif

#ifndef __ctp__ctp2_enums_h__
#define __ctp__ctp2_enums_h__ 1

#include "AgreementTypes.h"

enum CONVERTED_BY {
	CONVERTED_BY_CLERIC,
	CONVERTED_BY_TELEVANGELIST,
	CONVERTED_BY_NOTHING
};

enum DATABASE {
	DATABASE_DEFAULT,
	DATABASE_UNITS,
	DATABASE_BUILDINGS,
	DATABASE_WONDERS,
	DATABASE_ADVANCES,
	DATABASE_TERRAIN,
	DATABASE_CONCEPTS,
	DATABASE_GOVERNMENTS,
	DATABASE_TILE_IMPROVEMENTS,
	DATABASE_RESOURCE,
	DATABASE_ORDERS,
	DATABASE_SEARCH,

	DATABASE_MAX
};

#include "GameObj_types.h"
#include "GameOver.h"

enum HAPPY_REASON {
	HAPPY_REASON_SMOKING_CRACK = -1,

	
	HAPPY_REASON_CITY_SIZE,
	HAPPY_REASON_DISTANCE_FROM_CAPITOL,
	HAPPY_REASON_ENEMY_ACTION,
	HAPPY_REASON_POLLUTION,
	HAPPY_REASON_STARVATION,
	HAPPY_REASON_PEACE_MOVEMENT,
	HAPPY_REASON_WORKDAY,
	HAPPY_REASON_WAGES,
	HAPPY_REASON_RATIONS,
	HAPPY_REASON_MARTIAL_LAW,
	HAPPY_REASON_ENTERTAINERS,
	HAPPY_REASON_BUILDINGS,
	HAPPY_REASON_WONDERS,
	HAPPY_REASON_CRIME,
	HAPPY_REASON_CONQUEST_DISTRESS,
	HAPPY_REASON_NUM_CITIES,
	HAPPY_REASON_FEATS,
	
	HAPPY_REASON_HAPPINESS_ATTACK,
	HAPPY_REASON_SLAVES_TAKEN,
	HAPPY_REASON_INDULGENCES,
	HAPPY_REASON_BIO_INFECTION,
	HAPPY_REASON_REFORMATION,
	HAPPY_REASON_POP_TYPE_REMOVED,
	HAPPY_REASON_POP_TYPE_ADDED,
	HAPPY_REASON_SOOTHSAY,
	HAPPY_REASON_EMANCIPATION,
	HAPPY_REASON_ATTACKED_CONVERTER,

	HAPPY_REASON_POST_REVOULTION_BLISS,  
	HAPPY_REASON_WONDER_TURNS_REMAINING,
	HAPPY_REASON_ASSASSINATION,
	HAPPY_REASON_MAX
};

enum INSTALLATION_TYPES { 
    INSTALLATION_LISTENING_POSTS=0, 

    INSTALLATION_RADAR_STATIONS, 	
    INSTALLATION_AIR_BASES,
    INSTALLATION_FORTIFICATIONS, 
    INSTALLATION_SONAR_BUOYS,    
};


enum ORDER_RESULT {
	ORDER_RESULT_ILLEGAL,
	ORDER_RESULT_FAILED,
	ORDER_RESULT_SUCCEEDED,
	ORDER_RESULT_INCOMPLETE,
	ORDER_RESULT_SUCCEEDED_INCOMPLETE,
};

enum POP_TYPE {
	POP_WORKER,
	POP_SCIENTIST,
	POP_ENTERTAINER,
	POP_FARMER,
	POP_LABORER,
	POP_MERCHANT,
	POP_SLAVE,
	POP_MAX
};

enum READINESS_LEVEL {
	READINESS_LEVEL_PEACE,
	READINESS_LEVEL_ALERT,
	READINESS_LEVEL_WAR
};

enum REGARD_TYPE
	{
    REGARD_TYPE_INSANE_HATRED,
	REGARD_TYPE_HOTWAR,
    REGARD_TYPE_COLDWAR, 
	REGARD_TYPE_NEUTRAL,
	REGARD_TYPE_FRIENDLY,
	REGARD_TYPE_LOVE
};

enum ROUTE_TYPE {
	ROUTE_TYPE_RESOURCE,
	ROUTE_TYPE_FOOD,
	ROUTE_TYPE_GOLD,
	ROUTE_TYPE_SLAVE, 
};

enum SPECATTACK {
	SPECATTACK_NONE = -1,

	SPECATTACK_NUKE,
	SPECATTACK_HEARGOSSIP,
	SPECATTACK_THROWPARTY,
	SPECATTACK_ESTABLISHEMBASSY,
	SPECATTACK_INCITEREVOLUTION,
	SPECATTACK_BOMBCABINET,
	SPECATTACK_CREATEFRANCHISE,
	SPECATTACK_CAUSEUNHAPPINESS,
	SPECATTACK_CONDUCTHIT,
	SPECATTACK_BIOTERROR,
	SPECATTACK_NANOTERROR,
	SPECATTACK_SLAVERAID,
	SPECATTACK_ENSLAVESETTLER,
	SPECATTACK_SLAVEUPRISING,
	SPECATTACK_FREESLAVES,
	SPECATTACK_SELLINDULGENCE,
	SPECATTACK_CONVERTCITY,
	SPECATTACK_PLANTNUKE,
	SPECATTACK_SOOTHSAY,
	SPECATTACK_CREATEPARK,
	SPECATTACK_INJOIN,
	SPECATTACK_SPY,
	SPECATTACK_STEALTECH,
	SPECATTACK_REVOLUTION,
	SPECATTACK_REFORMCITY,

	SPECATTACK_MAX
};

enum STRENGTH_CAT {
	STRENGTH_CAT_NONE = -1,
	STRENGTH_CAT_KNOWLEDGE,
	STRENGTH_CAT_MILITARY,
	STRENGTH_CAT_POLLUTION,
	STRENGTH_CAT_TRADE,
	STRENGTH_CAT_GOLD,
	STRENGTH_CAT_POPULATION,
	STRENGTH_CAT_CITIES,
	STRENGTH_CAT_GEOGRAPHICAL,
	STRENGTH_CAT_SPACE,
	STRENGTH_CAT_UNDERSEA,
	STRENGTH_CAT_UNITS,
	STRENGTH_CAT_BUILDINGS,
	STRENGTH_CAT_WONDERS,
	STRENGTH_CAT_PRODUCTION,
	STRENGTH_CAT_MAX
};

enum UNIT_COMMAND {
	UNIT_COMMAND_NEAREST_CITY,
	UNIT_COMMAND_NEAREST_FORT,
	UNIT_COMMAND_NEAREST_AIRFIELD,
	UNIT_COMMAND_ENTRENCH,
	UNIT_COMMAND_SLEEP
};

enum UNIT_ORDER_TYPE {
	UNIT_ORDER_NONE,
	UNIT_ORDER_MOVE,
	UNIT_ORDER_PATROL,
	UNIT_ORDER_CIRCULAR_PATROL,
	UNIT_ORDER_ENTRENCH,
	UNIT_ORDER_SLEEP,
	UNIT_ORDER_DETRENCH,
	UNIT_ORDER_UNLOAD,
	UNIT_ORDER_MOVE_TO,
	UNIT_ORDER_TELEPORT_TO,

	
	UNIT_ORDER_EXPEL_TO,
	UNIT_ORDER_GROUP,
	UNIT_ORDER_UNGROUP,
	UNIT_ORDER_INVESTIGATE_CITY,
	UNIT_ORDER_NULLIFY_WALLS,
	UNIT_ORDER_STEAL_TECHNOLOGY,
	UNIT_ORDER_INCITE_REVOLUTION,
	UNIT_ORDER_ASSASSINATE,
	UNIT_ORDER_INVESTIGATE_READINESS,
	UNIT_ORDER_BOMBARD,

	
	UNIT_ORDER_SUE,
	UNIT_ORDER_FRANCHISE,
	UNIT_ORDER_SUE_FRANCHISE,
	UNIT_ORDER_EXPEL,
	UNIT_ORDER_ESTABLISH_EMBASSY,
	UNIT_ORDER_THROW_PARTY,
	UNIT_ORDER_CAUSE_UNHAPPINESS,
	UNIT_ORDER_PLANT_NUKE,
	UNIT_ORDER_SLAVE_RAID,
	UNIT_ORDER_ENSLAVE_SETTLER,

	
	UNIT_ORDER_UNDERGROUND_RAILWAY,
	UNIT_ORDER_INCITE_UPRISING,
	UNIT_ORDER_BIO_INFECT,
	UNIT_ORDER_NANO_INFECT,
	UNIT_ORDER_CONVERT,
	UNIT_ORDER_REFORM,
	UNIT_ORDER_INDULGENCE,
	UNIT_ORDER_SOOTHSAY,
	UNIT_ORDER_CREATE_PARK,
	UNIT_ORDER_PILLAGE,

	
	UNIT_ORDER_INJOIN,
	UNIT_ORDER_INTERCEPT_TRADE,
	UNIT_ORDER_PARADROP_MOVE,
	UNIT_ORDER_SET_UNLOAD_MOVEMENT_POINTS,
	UNIT_ORDER_GROUP_UNIT,
	UNIT_ORDER_DISBAND,
	UNIT_ORDER_FINISH_ATTACK,
	UNIT_ORDER_UNLOAD_ONE_UNIT,
	UNIT_ORDER_BOARD_TRANSPORT,
	UNIT_ORDER_WAKE_UP,

	
	UNIT_ORDER_PILLAGE_UNCONDITIONALLY,
	UNIT_ORDER_MOVE_THEN_UNLOAD,
	UNIT_ORDER_ADVERTISE,
    UNIT_ORDER_INFORM_AI_CAPTURE_CITY, 
	UNIT_ORDER_UNLOAD_SELECTED_STACK,
	UNIT_ORDER_ADD_EVENT,
	UNIT_ORDER_SETTLE, 
	UNIT_ORDER_LAUNCH,
	UNIT_ORDER_TARGET,
	UNIT_ORDER_CLEAR_TARGET,

	
	UNIT_ORDER_PLAGUE,
	UNIT_ORDER_VICTORY_MOVE,

	UNIT_ORDER_MAX
};

enum UPRISING_CAUSE {
	UPRISING_CAUSE_NONE,
	UPRISING_CAUSE_SLAVE_STARVED,
	UPRISING_CAUSE_UNGUARDED_SLAVES,
	UPRISING_CAUSE_INCITED,
	UPRISING_CAUSE_INTERNAL, 
};

#endif
