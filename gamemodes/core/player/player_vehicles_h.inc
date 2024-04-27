enum Player_Vehicles_Info
{
	bool:player_vehicle_VALID,
	player_vehicle_ID,
	player_vehicle_OWNER_ID,
	bool:player_vehicle_ACCESSIBLE,
	player_vehicle_CLAMP
};
new PLAYER_VEHICLES[MAX_VEHICLES][Player_Vehicles_Info];