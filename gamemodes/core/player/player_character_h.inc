#if defined player_character_included 
	#endinput
#endif
#define player_character_included

#define MAX_PLAYER_GPS_SAVES	(10)
enum Player_GPS_Enum
{
	player_gps_VALID,
	player_gps_ID,
	player_gps_NAME[24],
	Float:player_gps_X,
	Float:player_gps_Y,
	Float:player_gps_Z,
	player_gps_WORLD,
	player_gps_INTERIOR,
}
new PLAYER_GPS[MAX_PLAYERS][MAX_PLAYER_GPS_SAVES][Player_GPS_Enum];