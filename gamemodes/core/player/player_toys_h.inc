enum Player_Toys_Info
{
	player_toy_VALID,
	
	player_toy_ID,
	player_toy_NAME[24],
	player_toy_ATTACHED,
	player_toy_MODELID,
	player_toy_BONE,
	Float:player_toy_OFFSET_X,
	Float:player_toy_OFFSET_Y,
	Float:player_toy_OFFSET_Z,
	Float:player_toy_ROT_X,
	Float:player_toy_ROT_Y,
	Float:player_toy_ROT_Z,
	Float:player_toy_SCALE_X,
	Float:player_toy_SCALE_Y,
	Float:player_toy_SCALE_Z,
	player_toy_COLOR_1,
	player_toy_COLOR_2,
}
new PLAYER_TOYS[MAX_PLAYERS][MAX_SU_TOYS][Player_Toys_Info]; //PTOYS = PlayerToys

