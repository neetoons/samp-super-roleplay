#include <YSI-Includes\YSI\y_hooks>

new
    pLastFuelSubtraction[MAX_PLAYERS],
	bool:pSpeedoShown[MAX_PLAYERS],
    pSpeedoTimer[MAX_PLAYERS] = {-1, ...},
    PlayerText:pSpeedoTd[MAX_PLAYERS][6];

hook OnPlayerConnect(playerid) {
    pSpeedoTd[playerid][0] = CreatePlayerTextDraw(playerid, 552.500000, 344.000000, "box");
	PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][0], 0.000000, 8.133330);
	PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][0], 0.000000, 108.000000);
	PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][0], 2);
	PlayerTextDrawColor(playerid, pSpeedoTd[playerid][0], -1);
	PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][0], 150);
	PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][0], 0);
	PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][0], 0);
	PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][0], 255);
	PlayerTextDrawFont(playerid, pSpeedoTd[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][0], 0);

	pSpeedoTd[playerid][1] = CreatePlayerTextDraw(playerid, 552.500000, 344.000000, "Veloc¢metro");
	PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][1], 0.198999, 1.015113);
	PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][1], 0.000000, 108.000000);
	PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][1], 2);
	PlayerTextDrawColor(playerid, pSpeedoTd[playerid][1], -1);
	PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][1], 1);
	PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][1], 0x6e657cff);
	PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][1], 0);
	PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][1], 0);
	PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][1], 255);
	PlayerTextDrawFont(playerid, pSpeedoTd[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][1], 0);

	pSpeedoTd[playerid][2] = CreatePlayerTextDraw(playerid, 503.000000, 359.000000, "Vehiculo:_");
	PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][2], 0.244666, 1.234962);
	PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][2], 1);
	PlayerTextDrawColor(playerid, pSpeedoTd[playerid][2], -1);
	PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][2], 0);
	PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][2], 1);
	PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][2], 255);
	PlayerTextDrawFont(playerid, pSpeedoTd[playerid][2], 1);
	PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][2], 0);

	pSpeedoTd[playerid][3] = CreatePlayerTextDraw(playerid, 503.000000, 369.000000, "Gasolina:_0.0_Litros");
	PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][3], 0.244666, 1.234962);
	PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][3], 1);
	PlayerTextDrawColor(playerid, pSpeedoTd[playerid][3], -1);
	PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][3], 0);
	PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][3], 1);
	PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][3], 255);
	PlayerTextDrawFont(playerid, pSpeedoTd[playerid][3], 1);
	PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][3], 0);

	pSpeedoTd[playerid][4] = CreatePlayerTextDraw(playerid, 503.000000, 390.000000, "Velocidad:_0_Km/h");
	PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][4], 0.244666, 1.234962);
	PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][4], 1);
	PlayerTextDrawColor(playerid, pSpeedoTd[playerid][4], -1);
	PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][4], 0);
	PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][4], 1);
	PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][4], 255);
	PlayerTextDrawFont(playerid, pSpeedoTd[playerid][4], 1);
	PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][4], 1);
	PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][4], 0);

	pSpeedoTd[playerid][5] = CreatePlayerTextDraw(playerid, 503.000000, 401.000000, "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
	PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][5], 0.244666, 1.234962);
	PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][5], 1);
	PlayerTextDrawColor(playerid, pSpeedoTd[playerid][5], -1);
	PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][5], 0);
	PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][5], 1);
	PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][5], 255);
	PlayerTextDrawFont(playerid, pSpeedoTd[playerid][5], 1);
	PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][5], 1);
	PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][5], 0);
}

hook OnPlayerDisconnect(playerid, reason) {
    for(new i = 0; i < sizeof pSpeedoTd[]; i ++) {
        PlayerTextDrawDestroy(playerid, pSpeedoTd[playerid][i]);
    }

	pSpeedoShown[playerid] = false;
    pLastFuelSubtraction[playerid] = 0;
    if(pSpeedoTimer[playerid] != -1) {
	    KillTimer(pSpeedoTimer[playerid]);
        pSpeedoTimer[playerid] = -1;
    }
}

hook OnPlayerStateChange(playerid, newstate, oldstate) {
    if(newstate == PLAYER_STATE_DRIVER) {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(VEHICLE_INFO[GetVehicleModel(vehicleid) - 400][vehicle_info_NORMAL_SPEEDO] && !PLAYER_TEMP[playerid][pt_IN_TUNING_GARAGE]) ShowPlayerSpeedoMeter(playerid);
    }
    else if(oldstate == PLAYER_STATE_DRIVER) {
        HidePlayerSpeedoMeter(playerid);
    }
}

ShowPlayerSpeedoMeter(playerid)
{
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 0;
	new vehicleid = GetPlayerVehicleID(playerid), modelid = GetVehicleModel(vehicleid);
	if(!VEHICLE_INFO[modelid - 400][vehicle_info_NORMAL_SPEEDO]) return 0;
    if(pSpeedoTimer[playerid] != -1) {
	    KillTimer(pSpeedoTimer[playerid]);
        pSpeedoTimer[playerid] = -1;
    }
	
	new td_str[64];
	format(td_str, sizeof td_str, "Veh¢culo:~y~_%s", VEHICLE_INFO[modelid - 400][vehicle_info_NAME]);
	PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][2], td_str);
	
	format(td_str, sizeof td_str, "Gasolina:~y~_%.1f_Litros", GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS]);
	PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][3], td_str);
	
	PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][4], "Velocidad:~y~_0_Km/h");
	PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][5], "~w~IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
	
	if(!IsInventoryOpened(playerid)) {
		for(new i = 0; i < sizeof pSpeedoTd[]; i ++) {
			PlayerTextDrawShow(playerid, pSpeedoTd[playerid][i]);
		}
		pSpeedoShown[playerid] = true;
	}
	else pSpeedoShown[playerid] = false;

	pLastFuelSubtraction[playerid] = gettime();
	pSpeedoTimer[playerid] = SetTimerEx("UpdatePlayerSpeedo", 250, true, "iif", playerid, vehicleid, VEHICLE_INFO[modelid - 400][vehicle_info_MAX_VEL]);
	return 1;
}

HidePlayerSpeedoMeter(playerid)
{
    pLastFuelSubtraction[playerid] = 0;
	pSpeedoShown[playerid] = false;
	if(pSpeedoTimer[playerid] != -1) {
	    KillTimer(pSpeedoTimer[playerid]);
        pSpeedoTimer[playerid] = -1;
    }
	for(new i = 0; i < sizeof pSpeedoTd[]; i ++) {
        PlayerTextDrawHide(playerid, pSpeedoTd[playerid][i]);
    }
	return 1;
}

forward UpdatePlayerSpeedo(playerid, vehicleid, Float:maxvel);
public UpdatePlayerSpeedo(playerid, vehicleid, Float:maxvel)
{
	if(!IsInventoryOpened(playerid) && !pSpeedoShown[playerid]) {
		for(new i = 0; i < sizeof pSpeedoTd[]; i ++) {
			PlayerTextDrawShow(playerid, pSpeedoTd[playerid][i]);
		}
		pSpeedoShown[playerid] = true;
	}
	else if(IsInventoryOpened(playerid) && pSpeedoShown[playerid]) {
		for(new i = 0; i < sizeof pSpeedoTd[]; i ++) {
			PlayerTextDrawHide(playerid, pSpeedoTd[playerid][i]);
		}
		pSpeedoShown[playerid] = false;
	}

	if(vehicleid != GetPlayerVehicleID(playerid))
	{
		HidePlayerSpeedoMeter(playerid);
		ShowPlayerSpeedoMeter(playerid);
		
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] = INVALID_PLAYER_ID;
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_DRIVER] = playerid;
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_OCCUPIED] = false;
		PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] = GetPlayerVehicleID(playerid);
		GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_OCCUPIED] = true;
		return 0;
	}
	
	new Float:vel = GetVehicleSpeed(vehicleid);
	
	if(ac_Info[CHEAT_VEHICLE_SPEED_HACK][ac_Enabled])
	{
		if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_IMMUNITY])
		{
			if(vel > maxvel + 100.0)
			{
				if(!ac_Info[CHEAT_VEHICLE_SPEED_HACK][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_VEHICLE_SPEED_HACK);
				else
				{
					if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_VEHICLE_SPEED_HACK][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_DETECTIONS] = 0;
					else PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_DETECTIONS] ++;
					
					PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_LAST_DETECTION] = gettime();
					if(PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_VEHICLE_SPEED_HACK][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_VEHICLE_SPEED_HACK);
				}
			}
		}
	}
	
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] == VEHICLE_STATE_NORMAL)
	{
		GetVehicleHealth(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH]);
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] < MIN_VEHICLE_HEALTH)
		{	
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] = VEHICLE_STATE_DAMAGED;
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] = MIN_VEHICLE_HEALTH;
			SetVehicleHealthEx(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH], playerid);
				
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
			UpdateVehicleParams(vehicleid);
			SendClientMessage(playerid, -1, "{CCCCCC}El motor del vehículo está demasiado dañado.");
		}
	}
	
	if(gettime() > pLastFuelSubtraction[playerid] + 5)
	{
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE])
		{
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] -= floatmul(floatdiv(vel, maxvel), 0.1);
			
			if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] <= 0.1)
			{
				PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_IMMUNITY] = gettime() + 15;
				GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] = 0.0;
				GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
				UpdateVehicleParams(vehicleid);
				
				SendClientMessage(playerid, -1, "{999999}El vehículo se ha quedado sin gasolina...");
			}
		}
		pLastFuelSubtraction[playerid] = gettime();
	}
	
	new td_str[64];
	format(td_str, 64, "Gasolina:~y~_%.1f_Litros", GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS]);
	PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][3], td_str);
	
	format(td_str, 64, "Velocidad:~y~_%d_Km/h", floatround(vel));
	PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][4], td_str);

    new start = floatround( floatdiv(vel, floatdiv(maxvel, 33.0)) );
    format(td_str, 64, "~y~IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
    if(start < 33) strins(td_str, "~w~", 3 + start);
	PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][5], td_str);
	
	GetVehiclePos(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][0], GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][1], GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][2]);
	return 1;
}

stock Float:GetVehicleSpeed(vehicleid)
{
    new Float:vx, Float:vy, Float:vz;
    GetVehicleVelocity(vehicleid, vx, vy, vz);
	new Float:vel = floatmul(floatsqroot(floatadd(floatadd(floatpower(vx, 2), floatpower(vy, 2)),  floatpower(vz, 2))), 181.5);
	return vel;
}