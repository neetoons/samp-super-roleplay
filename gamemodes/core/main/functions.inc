/*
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

	The Initial Developer of the Original Code was adri1.
	Code originally released from: https://sampforum.blast.hk/showthread.php?tid=639431

*/

#if defined functions_included 
	#endinput
#endif
#define functions_included


#define MIN_SECONDS_BETWEEN_COMMANDS 1 // Deben pasar al menos 1 segundos entre comando y comando.

#define PlayerTask_Interval_Seconds 10

//ANTI AIMBOT no funciona bn, se atraviesan los players
//const PLAYER_SYNC = 207;
//IPacket:PLAYER_SYNC(playerid, BitStream:bs)
//{
//    new onFootData[PR_OnFootSync];
//
//    BS_IgnoreBits(bs, 8); 
//    BS_ReadOnFootSync(bs, onFootData);
//
//    onFootData[PR_specialAction] = SPECIAL_ACTION_ENTER_VEHICLE;
//
//    BS_SetWriteOffset(bs, 8);
//    BS_WriteOnFootSync(bs, onFootData);
//    return 1;
//}

ShowPlayerMessage(playerid, message[],tiempo = 5000)
{
	KillTimer(TimerMensaje[playerid]);
	for(new len = strlen(message), pos; pos < len; pos ++)
	{
		switch(message[pos])
		{
			case 'à': message[pos] = 151;
			case 'á': message[pos] = 152;
			case 'â': message[pos] = 153;
			case 'ä': message[pos] = 154;
			case 'À': message[pos] = 128;
			case 'Á': message[pos] = 129;
			case 'Â': message[pos] = 130;
			case 'Ä': message[pos] = 131;
			case 'è': message[pos] = 157;
			case 'é': message[pos] = 158;
			case 'ê': message[pos] = 159;
			case 'ë': message[pos] = 160;
			case 'È': message[pos] = 134;
			case 'É': message[pos] = 135;
			case 'Ê': message[pos] = 136;
			case 'Ë': message[pos] = 137;
			case 'ì': message[pos] = 161;
			case 'í': message[pos] = 162;
			case 'î': message[pos] = 163;
			case 'ï': message[pos] = 164;
			case 'Ì': message[pos] = 138;
			case 'Í': message[pos] = 139;
			case 'Î': message[pos] = 140;
			case 'Ï': message[pos] = 141;
			case 'ò': message[pos] = 165;
			case 'ó': message[pos] = 166;
			case 'ô': message[pos] = 167;
			case 'ö': message[pos] = 168;
			case 'Ò': message[pos] = 142;
			case 'Ó': message[pos] = 143;
			case 'Ô': message[pos] = 144;
			case 'Ö': message[pos] = 145;
			case 'ù': message[pos] = 169;
			case 'ú': message[pos] = 170;
			case 'û': message[pos] = 171;
			case 'ü': message[pos] = 172;
			case 'Ù': message[pos] = 146;
			case 'Ú': message[pos] = 147;
			case 'Û': message[pos] = 148;
			case 'Ü': message[pos] = 149;
			case 'ñ': message[pos] = 174;
			case 'Ñ': message[pos] = 173;
			case '¡': message[pos] = 64;
			case '¿': message[pos] = 175;
			case '`': message[pos] = 177;
			case '&': message[pos] = 38;
			default:  message[pos] = message[pos];
		}
	}
	PlayerTextDrawSetString(playerid, MensajeTD[playerid], message);
	PlayerTextDrawShow(playerid, MensajeTD[playerid]);
	TimerMensaje[playerid] = SetTimerEx("HidePlayerMessage", tiempo, false, "i", playerid);
	return 1;
}

UpdatePlayerHud(playerid)
{
	if(!PLAYER_TEMP[playerid][pt_HUD_TEXTDRAWS]) return 0;
	
	new Float:size;
	size = 500.699981 + (PI[playerid][ac_HUNGRY] * 0.46030121);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1], size, 0.000000);
	
	size = 557.699981 + (PI[playerid][ac_THIRST] * 0.46030121);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2], size, 0.000000);
	
	if(PI[playerid][ac_CONFIG_HUD])
	{
		PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1]);
		PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2]);
	}
	return 1;
}
UpdateVehicleParams(vehicleid)
{
	return SetVehicleParamsEx(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE], GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_LIGHTS], GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ALARM], GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS], GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_BONNET], GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_BOOT], GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_OBJECTIVE]);
}

SetVehicleHealthEx(vehicleid, Float:health, forplayerid = INVALID_PLAYER_ID)
{
	if(forplayerid != INVALID_PLAYER_ID) PLAYER_AC_INFO[forplayerid][CHEAT_VEHICLE_HEALTH][p_ac_info_IMMUNITY] = gettime() + 3;
	
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] = health;
	SetVehicleHealth(vehicleid, health);
	return 1;
}

RepairVehicleEx(vehicleid, forplayerid = INVALID_PLAYER_ID, Float:health = 1000.0)
{
	if(forplayerid != INVALID_PLAYER_ID) PLAYER_AC_INFO[forplayerid][CHEAT_VEHICLE_HEALTH][p_ac_info_IMMUNITY] = gettime() + 3;
	
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_REPAIR_TIME] = gettime();
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] = VEHICLE_STATE_NORMAL;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] = health;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_PANELS] = 0;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_DOORS] = 0;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_LIGHTS] = 0;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_TIRES] = 0;
	SetVehicleHealthEx(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH]);
	if(health >= 1000.0) RepairVehicle(vehicleid);
	return 1;
}

SetVehiclePosEx(vehicleid, Float:x, Float:y, Float:z)
{
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_TP_IMMUNITY] = gettime() + 5;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][0] = x;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][1] = y;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][2] = z;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_ATTACHED_TO] = INVALID_VEHICLE_ID;
	return SetVehiclePos(vehicleid, x, y, z);
}

LinkVehicleToInteriorEx(vehicleid, interior)
{
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_TP_IMMUNITY] = gettime() + 5;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_INTERIOR] = interior;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_ATTACHED_TO] = INVALID_VEHICLE_ID;
	return LinkVehicleToInterior(vehicleid, interior);
}

SetVehicleToRespawnEx(vehicleid)
{
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWNED] = false;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_ATTACHED_TO] = INVALID_VEHICLE_ID;
	return SetVehicleToRespawn(vehicleid);
}
UpdatePlayerLoadingTruckSize(playerid)
{
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3], 253.0 + ( PLAYER_TEMP[playerid][pt_TRUCK_LOADING_VALUE] * 1.34), 0.000000);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3]);
	return 1;
}
CreatePropertyInfo(i, pid, pname[], cid, cname[])
{
	if(PROPERTY_INFO[i][property_EXT_LABEL_ID] != Text3D:INVALID_STREAMER_ID)
	{
		DestroyDynamic3DTextLabel(PROPERTY_INFO[i][property_EXT_LABEL_ID]);
		PROPERTY_INFO[i][property_EXT_LABEL_ID] = Text3D:INVALID_STREAMER_ID;
	}
	if(PROPERTY_INFO[i][property_INT_LABEL_ID] != Text3D:INVALID_STREAMER_ID)
	{
		DestroyDynamic3DTextLabel(PROPERTY_INFO[i][property_INT_LABEL_ID]);
		PROPERTY_INFO[i][property_INT_LABEL_ID] = Text3D:INVALID_STREAMER_ID;
	}
	if(PROPERTY_INFO[i][property_EXT_PICKUP_ID] != INVALID_STREAMER_ID)
	{
		DestroyDynamicPickup(PROPERTY_INFO[i][property_EXT_PICKUP_ID]);
		PROPERTY_INFO[i][property_EXT_PICKUP_ID] = INVALID_STREAMER_ID;
	}
	if(PROPERTY_INFO[i][property_INT_PICKUP_ID] != INVALID_STREAMER_ID)
	{
		DestroyDynamicPickup(PROPERTY_INFO[i][property_INT_PICKUP_ID]);
		PROPERTY_INFO[i][property_INT_PICKUP_ID] = INVALID_STREAMER_ID;
	}

	new info[3], label_str[256], pickup_modelid;
	if(pid)
	{
		pickup_modelid = 0;
		PROPERTY_INFO[i][property_SOLD] = true;
		PROPERTY_INFO[i][property_CREW] = false;
		PROPERTY_INFO[i][property_OWNER_ID] = pid;
		PROPERTY_INFO[i][property_CREW_ID] = 0;
		format(label_str, sizeof label_str, "Propiedad {"#PRIMARY_COLOR"}#%d\n\n{FFFFFF}Propietario: {"#PRIMARY_COLOR"}%s\n{FFFFFF}Presiona {"#PRIMARY_COLOR"}~k~~CONVERSATION_YES~ {FFFFFF}para entrar.", PROPERTY_INFO[i][property_ID], pname);
	}
	else if(cid)
	{
		pickup_modelid = 0;
		PROPERTY_INFO[i][property_SOLD] = true;
		PROPERTY_INFO[i][property_CREW] = true;
		PROPERTY_INFO[i][property_CREW_ID] = cid;
		PROPERTY_INFO[i][property_OWNER_ID] = 0;
		format(PROPERTY_INFO[i][property_NAME], 24, "Propiedad %d", PROPERTY_INFO[i][property_ID]);
		format(label_str, sizeof label_str, "Propiedad {"#PRIMARY_COLOR"}#%d\n\n{FFFFFF}Banda: {"#PRIMARY_COLOR"}%s\n{FFFFFF}Presiona {"#PRIMARY_COLOR"}~k~~CONVERSATION_YES~ {FFFFFF}para entrar.", PROPERTY_INFO[i][property_ID], cname);
	}
	else
	{
		pickup_modelid = 1273;
		PROPERTY_INFO[i][property_SOLD] = false;
		PROPERTY_INFO[i][property_OWNER_ID] = 0;
		format(PROPERTY_INFO[i][property_NAME], 24, "Propiedad %d", PROPERTY_INFO[i][property_ID]);
		if(PROPERTY_INFO[i][property_VIP_LEVEL])
		{
			if(PROPERTY_INFO[i][property_EXTRA]) format(label_str, sizeof label_str, "{"#PRIMARY_COLOR"}Membresía VIP requerida\n\n{FFFFFF}Propiedad {"#PRIMARY_COLOR"}#%d {FFFFFF}está en venta\n\nInterior: {"#PRIMARY_COLOR"}%d\n{FFFFFF}Coste: {"#PRIMARY_COLOR"}%d "SERVER_COIN"\n{FFFFFF}Nivel necesario: {"#PRIMARY_COLOR"}%d\n{FFFFFF}Usa {"#PRIMARY_COLOR"}/ayuda {FFFFFF}para ver como adquirirla.", PROPERTY_INFO[i][property_ID], PROPERTY_INFO[i][property_ID_INTERIOR], PROPERTY_INFO[i][property_EXTRA], PROPERTY_INFO[i][property_LEVEL]);
			else format(label_str, sizeof label_str, "{"#PRIMARY_COLOR"}Membresía VIP requerida\n\n{FFFFFF}Propiedad {"#PRIMARY_COLOR"}#%d {FFFFFF}está en venta\n\nInterior: {"#PRIMARY_COLOR"}%d\n{FFFFFF}Precio: {"#PRIMARY_COLOR"}%s$\n{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\n{FFFFFF}Usa {"#PRIMARY_COLOR"}/ayuda {FFFFFF}para ver como adquirirla.", PROPERTY_INFO[i][property_ID], PROPERTY_INFO[i][property_ID_INTERIOR], number_format_thousand(PROPERTY_INFO[i][property_PRICE]), PROPERTY_INFO[i][property_LEVEL]);
		}
		else
		{
			if(PROPERTY_INFO[i][property_EXTRA]) format(label_str, sizeof label_str, "{FFFFFF}Propiedad {"#PRIMARY_COLOR"}#%d {FFFFFF}está en venta\n\nInterior: {"#PRIMARY_COLOR"}%d\n{FFFFFF}Coste: {"#PRIMARY_COLOR"}%d "SERVER_COIN"\n{FFFFFF}Nivel necesario: {"#PRIMARY_COLOR"}%d\n{FFFFFF}Usa {"#PRIMARY_COLOR"}/ayuda {FFFFFF}para ver como adquirirla.", PROPERTY_INFO[i][property_ID], PROPERTY_INFO[i][property_ID_INTERIOR], PROPERTY_INFO[i][property_EXTRA], PROPERTY_INFO[i][property_LEVEL]);
			else format(label_str, sizeof label_str, "{FFFFFF}Propiedad {"#PRIMARY_COLOR"}#%d {FFFFFF}está en venta\n\nInterior: {"#PRIMARY_COLOR"}%d\n{FFFFFF}Precio: {"#PRIMARY_COLOR"}%s$\n{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\n{FFFFFF}Usa {"#PRIMARY_COLOR"}/ayuda {FFFFFF}para ver como adquirirla.", PROPERTY_INFO[i][property_ID], PROPERTY_INFO[i][property_ID_INTERIOR], number_format_thousand(PROPERTY_INFO[i][property_PRICE]), PROPERTY_INFO[i][property_LEVEL]);
		}
	}
	
	PROPERTY_INFO[i][property_EXT_LABEL_ID] = CreateDynamic3DTextLabel(label_str, 0xFFFFFFFF, PROPERTY_INFO[i][property_EXT_X], PROPERTY_INFO[i][property_EXT_Y], PROPERTY_INFO[i][property_EXT_Z] + 0.25, 5.0, .testlos = true, .worldid = 0, .interiorid = PROPERTY_INFO[i][property_EXT_INTERIOR]);

	new Float:z_pos = PROPERTY_INTERIORS[ PROPERTY_INFO[i][property_ID_INTERIOR] ][property_INT_Z];
	if(PROPERTY_INFO[i][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
	PROPERTY_INFO[i][property_INT_LABEL_ID] = CreateDynamic3DTextLabel("Presiona {"#PRIMARY_COLOR"}~k~~CONVERSATION_YES~ {FFFFFF}para salir\n\nEscribe {"#PRIMARY_COLOR"}/casa {FFFFFF}para más opciones", 0xFFFFFFFF, PROPERTY_INTERIORS[ PROPERTY_INFO[i][property_ID_INTERIOR] ][property_INT_X], PROPERTY_INTERIORS[ PROPERTY_INFO[i][property_ID_INTERIOR] ][property_INT_Y], z_pos + 0.25, 3.0, .testlos = true, .worldid = PROPERTY_INFO[i][property_ID], .interiorid = PROPERTY_INTERIORS[ PROPERTY_INFO[i][property_ID_INTERIOR] ][property_INT_INTERIOR]);

	PROPERTY_INFO[i][property_EXT_PICKUP_ID] = CreateDynamicPickup(pickup_modelid, 1, PROPERTY_INFO[i][property_EXT_X], PROPERTY_INFO[i][property_EXT_Y], PROPERTY_INFO[i][property_EXT_Z], 0, PROPERTY_INFO[i][property_EXT_INTERIOR]);
	PROPERTY_INFO[i][property_INT_PICKUP_ID] = CreateDynamicPickup(0, 1, PROPERTY_INTERIORS[ PROPERTY_INFO[i][property_ID_INTERIOR] ][property_INT_X], PROPERTY_INTERIORS[ PROPERTY_INFO[i][property_ID_INTERIOR] ][property_INT_Y], z_pos, PROPERTY_INFO[i][property_ID], PROPERTY_INTERIORS[ PROPERTY_INFO[i][property_ID_INTERIOR] ][property_INT_INTERIOR]);
	info[0] = PICKUP_TYPE_PROPERTY;
	info[1] = i; // Index
	info[2] = 1; // Pickup Interior
	Streamer_SetArrayData(STREAMER_TYPE_PICKUP, PROPERTY_INFO[i][property_INT_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
	info[0] = PICKUP_TYPE_PROPERTY;
	info[1] = i; // Index
	info[2] = 2; // Pickup Exterior
	Streamer_SetArrayData(STREAMER_TYPE_PICKUP, PROPERTY_INFO[i][property_EXT_PICKUP_ID], E_STREAMER_EXTRA_ID, info);

	PROPERTY_INFO[i][property_POLICE_FORCING] = false;
	PROPERTY_INFO[i][property_POLICE_FORCED] = false;
	PROPERTY_INFO[i][property_POLICE_FORCE_TIMER] = -1;
	return 1;
}
ExitPlayerPropertyConstructor(playerid)
{
	CancelSelectTextDrawEx(playerid);
	PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_ENABLED] = false;
	
	SetPlayerPosEx
	(
		playerid, 
		PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_X],
		PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_Y],
		PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_Z],
		PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_ANG],
		PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_INT],
		0, false, true
	);
	
	for(new i = 0; i != 6; i ++) PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][i]);
	return 1;
}

Create_PlayerPropertyConstructo(playerid)
{
	new slot = GetEmptyPropertySlot();
	if(slot == -1)
	{
		SendNotification(playerid, "No hay mas slots.");
		ExitPlayerPropertyConstructor(playerid);
		return 1;
	}
	
	PROPERTY_INFO[slot][property_VALID] = true;
	PROPERTY_INFO[slot][property_EXT_X] = PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_X];
	PROPERTY_INFO[slot][property_EXT_Y] = PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_Y];
	PROPERTY_INFO[slot][property_EXT_Z] = PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_Z];
	PROPERTY_INFO[slot][property_EXT_ANGLE] = PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_ANG];
	PROPERTY_INFO[slot][property_EXT_INTERIOR] = PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_INT];
	PROPERTY_INFO[slot][property_EXT_FREEZE] = false;
	PROPERTY_INFO[slot][property_ID_INTERIOR] = PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO];
	
	PROPERTY_INFO[slot][property_SOLD] = false;
	PROPERTY_INFO[slot][property_OWNER_ID] = 0;
	PROPERTY_INFO[slot][property_PRICE] = PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][1];
	PROPERTY_INFO[slot][property_LEVEL] = PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][0];
	PROPERTY_INFO[slot][property_EXTRA] = PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][3];
	PROPERTY_INFO[slot][property_VIP_LEVEL] = PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][2];
	PROPERTY_INFO[slot][property_PRICE_BASE] = floatround(floatdiv(PROPERTY_INFO[slot][property_PRICE], MULTIPLIER_PROPERITES));
	
	if(PROPERTY_INFO[slot][property_EXTRA]) PROPERTY_INFO[slot][property_PRICE] = 0;
	if(PROPERTY_INFO[slot][property_VIP_LEVEL]) PROPERTY_INFO[slot][property_LEVEL] = 1;
	
	inline OnPropertyInserted()
	{
		PROPERTY_INFO[slot][property_ID] = cache_insert_id();
		CreatePropertyInfo(slot, 0, "", 0, "");

		SendFormatNotification(playerid, "Propiedad creada, id: %d (%d/%d).", PROPERTY_INFO[slot][property_ID], slot, MAX_PROPERTIES);
		ExitPlayerPropertyConstructor(playerid);
	}
	mysql_format
	(
		srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER,
		"\
			INSERT INTO properties (ext_x, ext_y, ext_z, ext_angle, ext_interior, ext_freeze, id_interior, price, level, extra, vip_level)\
			VALUES (%f, %f, %f, %f, %d, %d, %d, %d, %d, %d, %d);\
		",
			PROPERTY_INFO[slot][property_EXT_X], PROPERTY_INFO[slot][property_EXT_Y], PROPERTY_INFO[slot][property_EXT_Z], PROPERTY_INFO[slot][property_EXT_ANGLE], 
			PROPERTY_INFO[slot][property_EXT_INTERIOR], PROPERTY_INFO[slot][property_EXT_FREEZE], PROPERTY_INFO[slot][property_ID_INTERIOR],
			PROPERTY_INFO[slot][property_PRICE_BASE], PROPERTY_INFO[slot][property_LEVEL], PROPERTY_INFO[slot][property_EXTRA], PROPERTY_INFO[slot][property_VIP_LEVEL]
	);
	MySQL_TQueryInline(srp_db, using inline OnPropertyInserted, QUERY_BUFFER);
	return 1;
}

GetEmptyPropertySlot()
{
	for(new i = 0; i != MAX_PROPERTIES; i ++)
	{
		if(!PROPERTY_INFO[i][property_VALID]) return i;
	}
	return -1;
}

SpaceFix(text[])
{
	new str[100 + 1]; format(str, sizeof str, "%s", text);
    for(new i = 0; i < strlen(str); i++) 
	{
		if(str[i] == ' ') str[i] = '+';
	}
	return str;
}
UpdatePlayerToy(playerid, index)
{
	RemovePlayerAttachedObject(playerid, index);
	SetPlayerAttachedObject
	(
		playerid, 
		index,
		PLAYER_TOYS[playerid][ index ][player_toy_MODELID],
		PLAYER_TOYS[playerid][ index ][player_toy_BONE],
		PLAYER_TOYS[playerid][ index ][player_toy_OFFSET_X],
		PLAYER_TOYS[playerid][ index ][player_toy_OFFSET_Y],
		PLAYER_TOYS[playerid][ index ][player_toy_OFFSET_Z],
		PLAYER_TOYS[playerid][ index ][player_toy_ROT_X],
		PLAYER_TOYS[playerid][ index ][player_toy_ROT_Y],
		PLAYER_TOYS[playerid][ index ][player_toy_ROT_Z],
		PLAYER_TOYS[playerid][ index ][player_toy_SCALE_X],
		PLAYER_TOYS[playerid][ index ][player_toy_SCALE_Y],
		PLAYER_TOYS[playerid][ index ][player_toy_SCALE_Z],
		PLAYER_TOYS[playerid][ index ][player_toy_COLOR_1],
		PLAYER_TOYS[playerid][ index ][player_toy_COLOR_2]
	);
	return 1;
}

EditPlayerToy(playerid)
{
	if(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED]) RemovePlayerAttachedObject(playerid, PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT]);
				
	SetPlayerAttachedObject
	(
		playerid, 
		PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT],
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_MODELID],
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_BONE],
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_OFFSET_X],
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_OFFSET_Y],
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_OFFSET_Z],
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ROT_X],
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ROT_Y],
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ROT_Z],
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_SCALE_X],
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_SCALE_Y],
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_SCALE_Z],
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_COLOR_1],
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_COLOR_2]
	);
	
	SendNotification(playerid, "Ajusta el accesorio y haz clic en el icono de guardar, si pulsas ~r~'ESCAPE' ~w~no se guardará la configuración.");
	SendNotification(playerid, "~w~Mantén presionado ~r~'ESPACIO' ~w~para mover la cámara.");
	EditAttachedObject(playerid, PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT]);
	return 1;
}
GetCrewIndexById(id)
{
	for(new x = 0; x != MAX_CREWS; x ++)
	{
		if(!CREW_INFO[x][crew_VALID]) continue;
		
		if(CREW_INFO[x][crew_ID] == id)
		{
			return x;
		}
	}
	return -1;
}
stock Float:frandom(Float:max, Float:min, dp)
{
	new
		Float:mul = floatpower(10.0, dp),
		imin = floatround(min * mul),
		imax = floatround(max * mul);
	return float(random(imax - imin) + imin) / mul;
}


SetTaxiPlayerMarkers(playerid)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
			{
				if(PLAYER_TEMP[i][pt_WANT_TAXI])
				{
					SetPlayerMarkerForPlayer(playerid, i, 0xf2f224FF);
				}
			}
		}
	}
	return 1;
}

SetNormalPlayerMarkers(playerid)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			SetPlayerMarkerForPlayer(playerid, i, PLAYER_TEMP[i][pt_PLAYER_COLOR]);
		}
	}
	return 1;
}

LoadPlayerCrewInfo(playerid)
{
	if(ACCOUNT_INFO[playerid][ac_ID] == 0) return 0;
	
	if(PI[playerid][ac_CREW])
	{
		new crew_index = GetCrewIndexById(PI[playerid][ac_CREW]);
		if(crew_index == -1)
		{
			PI[playerid][ac_CREW] = 0;
			PI[playerid][ac_CREW_RANK] = 0;
			PLAYER_TEMP[playerid][pt_CREW_INDEX] = 0;
			SendNotification(playerid, "La banda a la que pertenecías ya no existe.");

			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET crew = NULL WHERE id = %d;", ACCOUNT_INFO[playerid][ac_ID]);
			mysql_tquery(srp_db, QUERY_BUFFER);
		}
		else
		{
			PLAYER_TEMP[playerid][pt_CREW_INDEX] = crew_index;
			CREW_INFO[crew_index][crew_ONLINE_MEMBERS] ++;
		}
	}
	return 1;
}

LoadPlayerWorks(playerid)
{
	if(ACCOUNT_INFO[playerid][ac_ID] == 0) return 0;
	
	inline OnPworksLoad()
	{
		new rows, count;
		if(cache_get_row_count(rows))
		{
			for(new i = 0; i != rows; i ++)
			{
				new work;
				cache_get_value_name_int(i, "id_work", work);
				cache_get_value_name_int(i, "set", PLAYER_WORKS[playerid][work][pwork_SET]);
				cache_get_value_name_int(i, "level", PLAYER_WORKS[playerid][work][pwork_LEVEL]);

				if(PLAYER_WORKS[playerid][work][pwork_SET] && !ACCOUNT_INFO[playerid][ac_VIP])
				{
					if(count >= MAX_NU_WORKS)
					{
						PLAYER_WORKS[playerid][work][pwork_SET] = false;
						if(work == WORK_POLICE)
						{
							mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pworks SET `set` = 0, level = 0 WHERE id_player = %d AND id_work = %d;", ACCOUNT_INFO[playerid][ac_ID], work);
							mysql_tquery(srp_db, QUERY_BUFFER);
							PLAYER_WORKS[playerid][work][pwork_LEVEL] = 0;
						}
						else
						{
							mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pworks SET `set` = 0 WHERE id_player = %d AND id_work = %d;", ACCOUNT_INFO[playerid][ac_ID], work);
							mysql_tquery(srp_db, QUERY_BUFFER);
						}
					}
					count ++;
				}
				
			}
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM pworks WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
	MySQL_TQueryInline(srp_db, using inline OnPworksLoad, QUERY_BUFFER);
	return 1;
}

ResetTruckInfo(vehicleid)
{
	new tmp_TRUCK_VEHICLE[TRUCK_VEHICLE_Info]; TRUCK_VEHICLE[vehicleid] = tmp_TRUCK_VEHICLE;
	return 1;
}

ResetPizzaInfo(vehicleid)
{
	new tmp_PIZZA_VEHICLE[PIZZA_VEHICLE_Info]; PIZZA_VEHICLE[vehicleid] = tmp_PIZZA_VEHICLE;
	return 1;
}

ResetTrashInfo(vehicleid)
{
	new tmp_TRASH_VEHICLES[TRASH_VEHICLES_INFO]; TRASH_VEHICLES[vehicleid] = tmp_TRASH_VEHICLES;
	
	TRASH_VEHICLES[vehicleid][trash_vehicle_DRIVER_ID] = INVALID_PLAYER_ID;
	TRASH_VEHICLES[vehicleid][trash_vehicle_PASSENGER_ID] = INVALID_PLAYER_ID;
	return 1;
}

forward TruckLoadUp(playerid, vehicleid);
public TruckLoadUp(playerid, vehicleid)
{
	if(PLAYER_TEMP[playerid][pt_TRUCK_LOADING_VALUE] < 100.0)
	{
		PLAYER_TEMP[playerid][pt_TRUCK_LOADING_VALUE] += 10.0;
		if(PLAYER_TEMP[playerid][pt_TRUCK_LOADING_VALUE] > 100.0) PLAYER_TEMP[playerid][pt_TRUCK_LOADING_VALUE] = 100.0;
		
		UpdatePlayerLoadingTruckSize(playerid);
		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][9]);
		PLAYER_TEMP[playerid][pt_TIMERS][9] = SetTimerEx("TruckLoadUp", 1000, false, "ii", playerid, vehicleid);
		return 1;
	}
	
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3]);
	
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 1;
	UpdateVehicleParams(vehicleid);
	
	TRUCK_VEHICLE[vehicleid][truck_vehicle_LOADED] = true;
	TRUCK_VEHICLE[vehicleid][truck_vehicle_LOADING] = false;
	TRUCK_VEHICLE[vehicleid][truck_vehicle_DELIVERED] = false;
	
	SendClientMessage(playerid,-1, "{"#SILVER_COLOR"}El camión ha sido cargado, dirígite al checkpoint para entregar la mercancía.");
	SetPlayerTruckCheckpoint(playerid, vehicleid);
	return 1;
}
ExitPlayerWardrobe(playerid)
{
	SetPlayerPosEx(playerid, PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z], PI[playerid][ac_ANGLE], ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INT_INTERIOR], ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INT_WORLD], false, true);
	return 1;
}
LoadPlayerPocketData(playerid)
{
	if(ACCOUNT_INFO[playerid][ac_ID] == 0) return 0;
	
	inline OnPfoodsLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			for(new i = 0; i != rows; i ++)
			{
				PLAYER_POCKET[playerid][i][player_pocket_VALID] = true;
				cache_get_value_name_int(i, "id", PLAYER_POCKET[playerid][i][player_pocket_object_ID]);
				cache_get_value_name(i, "name", PLAYER_POCKET[playerid][i][player_pocket_object_NAME]);
				cache_get_value_name_float(i, "hungry", PLAYER_POCKET[playerid][i][player_pocket_object_HUNGRY]);
				cache_get_value_name_float(i, "thirst", PLAYER_POCKET[playerid][i][player_pocket_object_THIRST]);
				cache_get_value_name_int(i, "drunk", PLAYER_POCKET[playerid][i][player_pocket_object_DRUNK]);
			}
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM pfoods WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
	MySQL_TQueryInline(srp_db,  using inline OnPfoodsLoad, QUERY_BUFFER);
	return 1;
}


LoadPlayerGPSData(playerid)
{
	if(ACCOUNT_INFO[playerid][ac_ID] == 0) return 0;
	
	inline OnPgpsLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			for(new i = 0; i != rows; i ++)
			{
				PLAYER_GPS[playerid][i][player_gps_VALID] = true;
				cache_get_value_name_int(i, "id", PLAYER_GPS[playerid][i][player_gps_ID]);
				cache_get_value_name(i, "name", PLAYER_GPS[playerid][i][player_gps_NAME]);
				cache_get_value_name_float(i, "x", PLAYER_GPS[playerid][i][player_gps_X]);
				cache_get_value_name_float(i, "y", PLAYER_GPS[playerid][i][player_gps_Y]);
				cache_get_value_name_float(i, "z", PLAYER_GPS[playerid][i][player_gps_Z]);
				cache_get_value_name_int(i, "world", PLAYER_GPS[playerid][i][player_gps_WORLD]);
				cache_get_value_name_int(i, "interior", PLAYER_GPS[playerid][i][player_gps_INTERIOR]);
				
			}
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM pgps WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
	MySQL_TQueryInline(srp_db,  using inline OnPgpsLoad, QUERY_BUFFER);
	return 1;
}





GetEmptyPlayerToySlot(playerid)
{
	for(new i = 0; i != MAX_SU_TOYS; i ++)
	{
		if(!PLAYER_TOYS[playerid][i][player_toy_VALID]) return i;
	}
	return -1;
}



stock ConvertTime_SecondsToHours(sec, &hours, &minutes, &seconds)
{
	hours = floatround(sec / 3600);
	minutes = floatround((sec - (hours * 3600)) / 60);
	seconds = sec % 60;
   	return 1;
}





public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(clickedid == Text:INVALID_TEXT_DRAW)
    {
		PLAYER_TEMP[playerid][pt_SELECT_TEXTDRAW] = false;
	
		if(PLAYER_TEMP[playerid][pt_READING_GUIDE])
		{
			SendNotification(playerid, "Antes de continuar debes leer el tutorial de conceptos.");
			SelectTextDrawEx(playerid, 0xe8d08fFF);
			return 1;
		}
		
		if(PLAYER_TEMP[playerid][pt_REGISTER_SELECTING_SKIN])
		{
			SendNotification(playerid, "Antes de continuar debes elegir tu sexo y tu ropa.");
			SelectTextDrawEx(playerid, 0xe8d08fFF);
			return 1;
		}	

		if(PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_ENABLED])
		{
			ExitPlayerPropertyConstructor(playerid);
			return 1;
		}
		
		if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] != -1) // Tiene abierto el menú de ropa
		{
			ClosePlayerClothingMenu(playerid);
			ExitPlayerWardrobe(playerid);
		}
		
		if(PLAYER_TEMP[playerid][pt_TOYS_SHOP]) ClosePlayerToysMenu(playerid);
		
		if(PLAYER_TEMP[playerid][pt_SHOP]) HidePlayerShop(playerid);
		
		if(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_SHOP])
		{
			CancelPlayerTuningShop(playerid);
			ShowDialog(playerid, DIALOG_VOBJECT_MENU);
		}
		return 1;
	}

	if(PLAYER_TEMP[playerid][pt_SHOP] && (PLAYER_TEMP[playerid][pt_SHOP_STATE] == PLAYER_SHOP_STATE_ALL || PLAYER_TEMP[playerid][pt_SHOP_STATE] == PLAYER_SHOP_STATE_MY_ADS))
	{
		for(new i = 0; i != 6; i ++)
		{
			if(clickedid == Textdraws[textdraw_SHOP_ARTICLES_BOX][i])
			{
				if(!PLAYER_TEMP[playerid][pt_SHOP_ARTICLE_ID][i]) return SendNotification(playerid, "Este anuncio ya no está disponible.");
				
				inline OnShopChecked()
				{
					new rows;
					if(cache_get_row_count(rows))
					{
						if(rows)
						{
							PLAYER_TEMP[playerid][pt_SHOP_SELECTED_ARTICLE_ID] = PLAYER_TEMP[playerid][pt_SHOP_ARTICLE_ID][i];
							PLAYER_TEMP[playerid][pt_SHOP_STATE] = PLAYER_SHOP_STATE_AD;
							UpdatePlayerShop(playerid);
						}
						else SendNotification(playerid, "Este anuncio ya no está disponible.");
					}
					else SendNotification(playerid, "Este anuncio ya no está disponible.");
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM shop WHERE id = %d;", PLAYER_TEMP[playerid][pt_SHOP_ARTICLE_ID][i]);
				MySQL_TQueryInline(srp_db,  using inline OnShopChecked, QUERY_BUFFER);
				break;
			}
		}
		return 1;
	}
	
	if(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_SHOP])
	{
		if(clickedid == Textdraws[textdraw_SHOP_TUNING][2]) //Comprar
		{
			new slot = GetVehicleFreeObjectSlot(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID]);
			if(!ACCOUNT_INFO[playerid][ac_VIP] && slot >= MAX_NU_VOBJECTS)
			{
				PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
				SendFormatNotification(playerid, "¡Los jugadores ~y~VIP ~w~pueden tener hasta %d objetos en vehículos! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP.", MAX_SU_VOBJECTS);
				return 1;
			}
			if(slot == -1)
			{
				SendNotification(playerid, "No hay más espacio para objetos en este vehículo.");
				return 1;
			}
			
			if(Tuning_Shop_Objects[ PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ][tuning_object_PRICE] > PI[playerid][ac_CASH])
			{
				PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
				SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar este objeto.", number_format_thousand(Tuning_Shop_Objects[ PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ][tuning_object_PRICE] - PI[playerid][ac_CASH]));
				return 1;
			}
			
			if(GivePlayerCash(playerid, -Tuning_Shop_Objects[ PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ][tuning_object_PRICE], true, true)) {
				VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_VALID] = true;
				VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_TYPE] = VOBJECT_TYPE_OBJECT;
				VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_ID] = 0;
				VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_MODELID] = Tuning_Shop_Objects[ PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ][tuning_object_MODELID];
				format(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_NAME], 32, "%s", Tuning_Shop_Objects[ PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ][tuning_object_NAME]);
				VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_ATTACHED] = true;
				UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], slot);
				
				RegisterNewVehicleObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], slot);
				CancelPlayerTuningShop(playerid);
				
				SendNotification(playerid, "Ajusta el objeto y haz clic en el icono de guardar, si pulsas ~r~'ESCAPE' ~w~no se guardará la configuración.");
				SendNotification(playerid, "Mantén presionado ~r~'W' ~w~para mover la cámara.");
				EditVehicleObject(playerid, PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], slot);
			}
			return 1;
		}
		else if(clickedid == Textdraws[textdraw_SHOP_TUNING][3]) //Siguiente
		{
			if(PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] == (sizeof(Tuning_Shop_Objects) - 1)) PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] = 0;			
			else PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ++;
			UpdateTuningShop(playerid);
			return 1;
		}
		else if(clickedid == Textdraws[textdraw_SHOP_TUNING][4]) //Anterior
		{
			if(PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] == 0) PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] = sizeof(Tuning_Shop_Objects) - 1;
			else PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] --;
			UpdateTuningShop(playerid);
			return 1;
		}
		return 1;
	}
	
	return 1;
}
LoadPlayerVehicles(playerid)
{
	if(ACCOUNT_INFO[playerid][ac_ID] == 0) return 0;
	
	inline OnPlayerVehiclesLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			for(new i = 0; i != rows; i ++)
			{
				if(i >= MAX_SU_VEHICLES)
				{
					printf("---> MAX_SU_VEHICLES superado al cargar de base de datos.");
					break;
				}

				new id, plate[32], modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:spawn_angle,
					Float:health, damage_panels, damage_doors, damage_lights, damage_tires, color1, color2,
					paintjob, Float:gas, Float:max_gas, closed, interior, eworld, veh_state,
					components[MAX_VEHICLE_COMPONENTS], vClamp,
					bool:isnull_plate;

				cache_get_value_name_int(i, "id", id);
				cache_is_value_name_null(i, "plate", isnull_plate);
				if(!isnull_plate) cache_get_value_name(i, "plate", plate, 32);
				cache_get_value_name_int(i, "modelid", modelid);
				cache_get_value_name_float(i, "spawn_x", spawn_x);
				cache_get_value_name_float(i, "spawn_y", spawn_y);
				cache_get_value_name_float(i, "spawn_z", spawn_z);
				cache_get_value_name_float(i, "spawn_angle", spawn_angle);
				cache_get_value_name_float(i, "health", health);
				cache_get_value_name_int(i, "damage_panels", damage_panels);
				cache_get_value_name_int(i, "damage_doors", damage_doors);
				cache_get_value_name_int(i, "damage_lights", damage_lights);
				cache_get_value_name_int(i, "damage_tires", damage_tires);
				cache_get_value_name_int(i, "color1", color1);
				cache_get_value_name_int(i, "color2", color2);
				cache_get_value_name_int(i, "paintjob", paintjob);
				cache_get_value_name_float(i, "gas", gas);
				cache_get_value_name_float(i, "max_gas", max_gas);
				cache_get_value_name_int(i, "closed", closed);
				cache_get_value_name_int(i, "interior", interior);
				cache_get_value_name_int(i, "world", eworld);
				cache_get_value_name_int(i, "state", veh_state);
				cache_get_value_name_int(i, "slot0", components[0]);
				cache_get_value_name_int(i, "slot1", components[1]);
				cache_get_value_name_int(i, "slot2", components[2]);
				cache_get_value_name_int(i, "slot3", components[3]);
				cache_get_value_name_int(i, "slot4", components[4]);
				cache_get_value_name_int(i, "slot5", components[5]);
				cache_get_value_name_int(i, "slot6", components[6]);
				cache_get_value_name_int(i, "slot7", components[7]);
				cache_get_value_name_int(i, "slot8", components[8]);
				cache_get_value_name_int(i, "slot9", components[9]);
				cache_get_value_name_int(i, "slot10", components[10]);
				cache_get_value_name_int(i, "slot11", components[11]);
				cache_get_value_name_int(i, "slot12", components[12]);
				cache_get_value_name_int(i, "slot13", components[13]);
				cache_get_value_name_int(i, "clamp", vClamp);

				new vehicle_id = CreateVehicle(modelid, spawn_x, spawn_y, spawn_z, spawn_angle, color1, color2, -1);
				if(vehicle_id != INVALID_VEHICLE_ID)
				{
					PLAYER_VEHICLES[vehicle_id][player_vehicle_VALID] = true;
					PLAYER_VEHICLES[vehicle_id][player_vehicle_OWNER_ID] = ACCOUNT_INFO[playerid][ac_ID];
					PLAYER_VEHICLES[vehicle_id][player_vehicle_ID] = id;
					PLAYER_VEHICLES[vehicle_id][player_vehicle_ACCESSIBLE] = true;
					PLAYER_VEHICLES[vehicle_id][player_vehicle_CLAMP] = vClamp;

					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_VALID] = true;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_TYPE] = VEHICLE_TYPE_PERSONAL;
					format(GLOBAL_VEHICLES[vehicle_id][gb_vehicle_NUMBER_PLATE], 32, "%s", plate);
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] = modelid;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X] = spawn_x;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y] = spawn_y;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z] = spawn_z;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_ANGLE] = spawn_angle;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][0] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X];
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][1] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y];
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][2] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z];
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_HEALTH] = health;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_PANELS] = damage_panels;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_DOORS] = damage_doors;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_LIGHTS] = damage_lights;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_TIRES] = damage_tires;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_1] = color1;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_2] = color2;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PAINTJOB] = paintjob;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS] = max_gas;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_GAS] = gas;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PARAMS_DOORS] = closed;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_STATE] = veh_state;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_INTERIOR] = interior;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_WORLD] = 0;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COMPONENTS] = components;
					GLOBAL_VEHICLES[vehicle_id][gb_vehicle_LAST_REPAIR_TIME] = gettime();

					if(!ACCOUNT_INFO[playerid][ac_VIP]) 
					{
						if(i >= MAX_NU_VEHICLES)
						{
							GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PARAMS_DOORS] = 1;
							PLAYER_VEHICLES[vehicle_id][player_vehicle_ACCESSIBLE] = false;
						}
					}

					//vboot
					inline OnVbootLoad()
					{
						new vb_rows;
						if(cache_get_row_count(vb_rows))
						{
							for(new x = 0; x != vb_rows; x ++)
							{
								new vboot_id, type, int, int_extra;
								cache_get_value_name_int(x, "id", vboot_id);
								cache_get_value_name_int(x, "type", type);
								cache_get_value_name_int(x, "int", int);
								cache_get_value_name_int(x, "int_extra", int_extra);

								VEHICLE_BOOT[vehicle_id][x][vehicle_boot_VALID] = true;
								VEHICLE_BOOT[vehicle_id][x][vehicle_boot_OBJECT_ID] = vboot_id;
								VEHICLE_BOOT[vehicle_id][x][vehicle_boot_TYPE] = type;
								VEHICLE_BOOT[vehicle_id][x][vehicle_boot_INT] = int;
								VEHICLE_BOOT[vehicle_id][x][vehicle_boot_INT_EXTRA] = int_extra;
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM vboot WHERE id_vehicle = %d LIMIT %d;", PLAYER_VEHICLES[vehicle_id][player_vehicle_ID], VEHICLE_INFO[GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] - 400][vehicle_info_BOOT_SLOTS]);
					MySQL_TQueryInline(srp_db, using inline OnVbootLoad, QUERY_BUFFER);

					//vobjects
					inline OnVobjectsLoad()
					{
						new vo_rows;
						if(cache_get_row_count(vo_rows))
						{
							for(new x = 0; x != vo_rows; x ++)
							{
								new vobject_id, type, name[32], vobject_modelid, Float:off_x, Float:off_y, Float:off_z,
									Float:rot_x, Float:rot_y, Float:rot_z, attached, vobject_color0, vobject_color1,
									vobject_color2, vobject_color3, vobject_color4, text[32], font[24], fontsize, fontbold, fontcolor,
									bool:isnull_text, bool:isnull_font;

								cache_get_value_name_int(x, "id", vobject_id);
								cache_get_value_name_int(x, "type", type);
								cache_get_value_name(x, "name", name);
								cache_get_value_name_int(x, "modelid", vobject_modelid);
								cache_get_value_name_float(x, "off_x", off_x);
								cache_get_value_name_float(x, "off_y", off_y);
								cache_get_value_name_float(x, "off_z", off_z);
								cache_get_value_name_float(x, "rot_x", rot_x);
								cache_get_value_name_float(x, "rot_y", rot_y);
								cache_get_value_name_float(x, "rot_z", rot_z);
								cache_get_value_name_int(x, "attached", attached);
								cache_get_value_name_int(x, "color0", vobject_color0);
								cache_get_value_name_int(x, "color1", vobject_color1);
								cache_get_value_name_int(x, "color2", vobject_color2);
								cache_get_value_name_int(x, "color3", vobject_color3);
								cache_get_value_name_int(x, "color4", vobject_color4);
								cache_is_value_name_null(x, "text", isnull_text);
								if(!isnull_text) cache_get_value_name(x, "text", text);
								cache_is_value_name_null(x, "font", isnull_font);
								if(!isnull_font) cache_get_value_name(x, "font", font);
								cache_get_value_name_int(x, "fontsize", fontsize);
								cache_get_value_name_int(x, "fontbold", fontbold);
								cache_get_value_name_int(x, "fontcolor", fontcolor);

								//ok
								VEHICLE_OBJECTS[vehicle_id][x][vobject_VALID] = true;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_ID] = vobject_id;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_TYPE] = type;
								format(VEHICLE_OBJECTS[vehicle_id][x][vobject_NAME], 32, "%s", name);
								VEHICLE_OBJECTS[vehicle_id][x][vobject_MODELID] = vobject_modelid;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_OFFSET][0] = off_x;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_OFFSET][1] = off_y;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_OFFSET][2] = off_z;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_ROT][0] = rot_x;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_ROT][1] = rot_y;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_ROT][2] = rot_z;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_ATTACHED] = attached;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_COLORS][0] = vobject_color0;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_COLORS][1] = vobject_color1;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_COLORS][2] = vobject_color2;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_COLORS][3] = vobject_color3;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_COLORS][4] = vobject_color4;
								format(VEHICLE_OBJECTS[vehicle_id][x][vobject_text_TEXT], 32, "%s", text);
								format(VEHICLE_OBJECTS[vehicle_id][x][vobject_text_FONT], 24, "%s", font);
								VEHICLE_OBJECTS[vehicle_id][x][vobject_text_FONT_SIZE] = fontsize;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_text_BOLD] = fontbold;
								VEHICLE_OBJECTS[vehicle_id][x][vobject_text_FONT_COLOR] = fontcolor;
								UpdateVehicleAttachedObject(vehicle_id, x, true);
							}
							//loaded
							SetVehicleToRespawnEx(vehicle_id);
							//loaded
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM vobjects WHERE id_vehicle = %d LIMIT %d;", PLAYER_VEHICLES[vehicle_id][player_vehicle_ID], MAX_SU_VOBJECTS);
					MySQL_TQueryInline(srp_db, using inline OnVobjectsLoad, QUERY_BUFFER);
				}
			}
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM pvehicles WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
	MySQL_TQueryInline(srp_db, using inline OnPlayerVehiclesLoad, QUERY_BUFFER);
	return 1;
}
LoadPlayerToys(playerid)
{
	if(ACCOUNT_INFO[playerid][ac_ID] == 0) return 0;
	
	inline OnPlayerToysLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			for(new i = 0; i != rows; i ++)
			{
				PLAYER_TOYS[playerid][i][player_toy_VALID] = true;
				cache_get_value_name_int(i, "id", PLAYER_TOYS[playerid][i][player_toy_ID]);
				cache_get_value_name(i, "name", PLAYER_TOYS[playerid][i][player_toy_NAME]);
				cache_get_value_name_int(i, "attached", PLAYER_TOYS[playerid][i][player_toy_ATTACHED]);
				cache_get_value_name_int(i, "modelid", PLAYER_TOYS[playerid][i][player_toy_MODELID]);
				cache_get_value_name_int(i, "bone", PLAYER_TOYS[playerid][i][player_toy_BONE]);
				cache_get_value_name_float(i, "offset_x", PLAYER_TOYS[playerid][i][player_toy_OFFSET_X]);
				cache_get_value_name_float(i, "offset_y", PLAYER_TOYS[playerid][i][player_toy_OFFSET_Y]);
				cache_get_value_name_float(i, "offset_z", PLAYER_TOYS[playerid][i][player_toy_OFFSET_Z]);
				cache_get_value_name_float(i, "rot_x", PLAYER_TOYS[playerid][i][player_toy_ROT_X]);
				cache_get_value_name_float(i, "rot_y", PLAYER_TOYS[playerid][i][player_toy_ROT_Y]);
				cache_get_value_name_float(i, "rot_z", PLAYER_TOYS[playerid][i][player_toy_ROT_Z]);
				cache_get_value_name_float(i, "scale_x", PLAYER_TOYS[playerid][i][player_toy_SCALE_X]);
				cache_get_value_name_float(i, "scale_y", PLAYER_TOYS[playerid][i][player_toy_SCALE_Y]);
				cache_get_value_name_float(i, "scale_z", PLAYER_TOYS[playerid][i][player_toy_SCALE_Z]);
				cache_get_value_name_int(i, "color1", PLAYER_TOYS[playerid][i][player_toy_COLOR_1]);
				cache_get_value_name_int(i, "color2", PLAYER_TOYS[playerid][i][player_toy_COLOR_2]);
			}
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM ptoys WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
	MySQL_TQueryInline(srp_db,  using inline OnPlayerToysLoad, QUERY_BUFFER);
	return 1;
}
LoadPlayerPhoneBook(playerid)
{
	if(ACCOUNT_INFO[playerid][ac_ID] == 0) return 0;
	
	inline OnPhoneBookLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			for(new i = 0; i != rows; i ++)
			{
				new id, name[24], number;
				cache_get_value_name_int(i, "id", id);
				cache_get_value_name(i, "name", name);
				cache_get_value_name_int(i, "number", number);
				
				PLAYER_PHONE_BOOK[playerid][i][phone_book_contact_VALID] = true;
				PLAYER_PHONE_BOOK[playerid][i][phone_book_contact_ID] = id;
				format(PLAYER_PHONE_BOOK[playerid][i][phone_book_contact_NAME], 24, "%s", name);
				PLAYER_PHONE_BOOK[playerid][i][phone_book_contact_PHONE_NUMBER] = number;
			}
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM pbook WHERE id_player = %d LIMIT %d;", ACCOUNT_INFO[playerid][ac_ID], MAX_PHONE_CONTACTS);
	MySQL_TQueryInline(srp_db,  using inline OnPhoneBookLoad, QUERY_BUFFER);
	return 1;
}
Auto_SendPlayerAction(playerid, action[])
{
	new str_text[145];
	format(str_text, 145, "* %s %s", PLAYER_TEMP[playerid][pt_RP_NAME], action);
	ProxDetector(playerid, 15.0, str_text, 0xC2A2DAFF, 0xC2A2DAFF, 0xC2A2DAFF, 0xC2A2DAFF, 0xC2A2DAFF);
	return 1;
}
RemovePlayerSlotWeapon(playerid, slot, bool:db_delete = false)
{
	if(db_delete)
	{
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM pweapons WHERE id = %d;", PLAYER_WEAPONS[playerid][slot][player_weapon_DB_ID]);
		mysql_tquery(srp_db, QUERY_BUFFER);
	}
	
	new current_weapon = GetPlayerWeapon(playerid);
	if(current_weapon == PLAYER_WEAPONS[playerid][slot][player_weapon_ID]) current_weapon = 0;
	
	PLAYER_WEAPONS[playerid][slot][player_weapon_VALID] = false;
	PLAYER_WEAPONS[playerid][slot][player_weapon_DB_ID] = 0;
	PLAYER_WEAPONS[playerid][slot][player_weapon_ID] = 0;
	PLAYER_WEAPONS[playerid][slot][player_weapon_AMMO] = 0;
	
	SetWeaponsForPlayer(playerid);
	SetPlayerArmedWeapon(playerid, current_weapon);
	return 1;
}
SetPlayerPosEx(playerid, Float:x, Float:y, Float:z, Float:angle, interior, world, freeze = 0, addoffset = 0)
{
	PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_UNOCCUPIED_VEHICLE_TP][p_ac_info_IMMUNITY] = gettime() + 5;
	
	if(addoffset)
	{
		x += (1.5 * floatsin(-angle, degrees));
		y += (1.5 * floatcos(-angle, degrees));
	}
	
	PI[playerid][ac_POS_X] = x;
	PI[playerid][ac_POS_Y] = y;
	PI[playerid][ac_POS_Z] = z;
	PI[playerid][ac_ANGLE] = angle;
	
	SetPlayerPos(playerid, x, y, z);
	SetPlayerFacingAngle(playerid, angle);
	SetPlayerInterior(playerid, interior);
	SetPlayerVirtualWorld(playerid, world);
	SetCameraBehindPlayer(playerid);
	
	if(freeze)
	{
		TogglePlayerControllableEx(playerid, false);
		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][3]);
		PLAYER_TEMP[playerid][pt_TIMERS][3] = SetTimerEx("TogglePlayerControl", 2000, false, "ib", playerid, true);
	}
	return 1;
}
const RPC_VehicleDestroy = 136;


SetPropertyDefaultFurniture(index, bool:toggle) {
	if(PROPERTY_INFO[index][property_DIS_DEFAULT_INTERIOR] != !toggle) {
		PROPERTY_INFO[index][property_DIS_DEFAULT_INTERIOR] = !toggle;
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE properties SET dis_default_interior = %d WHERE id = %d;", PROPERTY_INFO[index][property_DIS_DEFAULT_INTERIOR], PROPERTY_INFO[index][property_ID]);
		mysql_tquery(srp_db, QUERY_BUFFER);

		new Float:z_pos = PROPERTY_INTERIORS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_INT_Z];
		if(PROPERTY_INFO[index][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
		Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, PROPERTY_INFO[index][property_INT_LABEL_ID], E_STREAMER_Z, z_pos + 0.25);
		Streamer_SetFloatData(STREAMER_TYPE_PICKUP, PROPERTY_INFO[index][property_INT_PICKUP_ID], E_STREAMER_Z, z_pos);

		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(IsPlayerConnected(i) && (ACCOUNT_INFO[i][ac_STATE] == ROLEPLAY_STATE_OWN_PROPERTY || ACCOUNT_INFO[i][ac_STATE] == ROLEPLAY_STATE_GUEST_PROPERTY) && PI[i][ac_LOCAL_INTERIOR] == PROPERTY_INFO[index][property_ID])
			{
				new Float:pos[3], Float:angle;
				GetPlayerPos(i, pos[0], pos[1], pos[2]);
				GetPlayerFacingAngle(i, angle);

				SetPlayerPosEx(i, pos[0], pos[1], pos[2] + (PROPERTY_INFO[index][property_DIS_DEFAULT_INTERIOR] ? PROPERTY_EMPTY_INTERIOR_Z_OFFSET : -PROPERTY_EMPTY_INTERIOR_Z_OFFSET), angle, GetPlayerInterior(i), GetPlayerVirtualWorld(i), 1);
			}
		}

		CallLocalFunction("OnDefaultPFurChanged", "i", index);
	}
}
SetPlayerHealthEx(playerid, Float:health)
{
	PLAYER_AC_INFO[playerid][CHEAT_PLAYER_HEALTH][p_ac_info_IMMUNITY] = gettime() + 3;
	
	PI[playerid][ac_HEALTH] = health;
	SetPlayerHealth(playerid, PI[playerid][ac_HEALTH]);
	return 1;
}

SetPlayerArmourEx(playerid, Float:armour)
{
	PLAYER_AC_INFO[playerid][CHEAT_PLAYER_ARMOUR][p_ac_info_IMMUNITY] = gettime() + 3;
	
	PI[playerid][ac_ARMOUR] = armour;
	SetPlayerArmour(playerid, PI[playerid][ac_ARMOUR]);
	return 1;
}

GivePlayerHealthEx(playerid, Float:health)
{
	PLAYER_AC_INFO[playerid][CHEAT_PLAYER_HEALTH][p_ac_info_IMMUNITY] = gettime() + 3;
	
	PI[playerid][ac_HEALTH] += health;
	if(PI[playerid][ac_HEALTH] > 100.0) PI[playerid][ac_HEALTH] = 100.0;
	SetPlayerHealth(playerid, PI[playerid][ac_HEALTH]);
	return 1;
}

GivePlayerArmourEx(playerid, Float:armour)
{
	PLAYER_AC_INFO[playerid][CHEAT_PLAYER_ARMOUR][p_ac_info_IMMUNITY] = gettime() + 3;
	
	PI[playerid][ac_ARMOUR] += armour;
	if(PI[playerid][ac_ARMOUR] > 100.0) PI[playerid][ac_ARMOUR] = 100.0;
	SetPlayerArmour(playerid, PI[playerid][ac_ARMOUR]);
	return 1;
}

TransferPlayerWeapon(from_playerid, slot, to_playerid)
{
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pweapons SET id_player = %d WHERE id = %d;", ACCOUNT_INFO[to_playerid][ac_ID], PLAYER_WEAPONS[from_playerid][slot][player_weapon_DB_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	
	PLAYER_WEAPONS[to_playerid][slot][player_weapon_VALID] = true;
	PLAYER_WEAPONS[to_playerid][slot][player_weapon_DB_ID] = PLAYER_WEAPONS[from_playerid][slot][player_weapon_DB_ID];
	PLAYER_WEAPONS[to_playerid][slot][player_weapon_ID] = PLAYER_WEAPONS[from_playerid][slot][player_weapon_ID];
	PLAYER_WEAPONS[to_playerid][slot][player_weapon_AMMO] = PLAYER_WEAPONS[from_playerid][slot][player_weapon_AMMO];
	SetWeaponsForPlayer(to_playerid);
	
	RemovePlayerSlotWeapon(from_playerid, slot);
	return 1;
}

PlayerPayday(playerid)
{
	new 
		dialog_body[80],
		dialog_header[630],
		dialog_string[140],
		date[24],
		money = 300 * ACCOUNT_INFO[playerid][ac_LEVEL];

	if(money > 4500) money = 4500;
	money += minrand(100, 200);
	
	getDateTime(date);

	format(dialog_body, sizeof dialog_body, "{ABD9C6}Pago diario (%s)", date);

	strcat(dialog_header, "\n{FF6746}____ [ PAGA DEL GOBIERNO ] ____\n");

	format(dialog_string, sizeof dialog_string, "{FFFFFF}Paga del gobierno: {FFFF7F}%s dólares\n", number_format_thousand(money));
	strcat(dialog_header, dialog_string);
	
	if(!ACCOUNT_INFO[playerid][ac_VIP])
	{
		strcat(dialog_header, "\n\n{FF6746}____ [ VEHÍCULOS ] ____\n");

		for(new i = 0, j = GetVehiclePoolSize(); i <= j; i ++)
		{
			if(!PLAYER_VEHICLES[i][player_vehicle_VALID]) continue;
			if(PLAYER_VEHICLES[i][player_vehicle_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) continue;
			
			new veh_money = VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_PRICE] / 100;
			money -= veh_money;
			
			format(dialog_string, sizeof dialog_string, "{FFFFFF}Seguro del vehículo %s: {FF6746}-%s dólares\n", VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_NAME], number_format_thousand(veh_money));
			strcat(dialog_header, dialog_string);
		}

		strcat(dialog_header, "\n\n{FF6746}____ [ PROPIEDADES ] ____\n");
		
		for(new i = 0; i != MAX_PROPERTIES; i ++)
		{
			if(!PROPERTY_INFO[i][property_VALID]) continue;
			if(PROPERTY_INFO[i][property_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) continue;
		
			if(PROPERTY_INFO[i][property_VIP_LEVEL]) format(dialog_string, sizeof dialog_string, "{FFFFFF}Seguro de la propiedad %s: {FFFF7F}PROPIEDAD VIP, NO SE PAGA SEGURO.\n", PROPERTY_INFO[i][property_NAME]);
			else
			{
				new property_money = PROPERTY_INFO[i][property_PRICE_BASE] / 200;
				money -= property_money;
				
				format(dialog_string, sizeof dialog_string, "{FFFFFF}Seguro de la propiedad %s: {FF6746}-%s dólares\n", PROPERTY_INFO[i][property_NAME], number_format_thousand(property_money));
			}
			strcat(dialog_header, dialog_string);
		}
	}
	
	if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
	{
		new work_payment;
		if(work_info[WORK_POLICE][work_info_EXTRA_PAY] > 0 && work_info[WORK_POLICE][work_info_EXTRA_PAY_EXP] > 0)
		{
			work_payment = (work_info[WORK_POLICE][work_info_EXTRA_PAY] * floatround(floatdiv(PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL], work_info[WORK_POLICE][work_info_EXTRA_PAY_EXP])));
			if(work_info[WORK_POLICE][work_info_EXTRA_PAY_LIMIT] != 0) if(work_payment > work_info[WORK_POLICE][work_info_EXTRA_PAY_LIMIT]) work_payment = work_info[WORK_POLICE][work_info_EXTRA_PAY_LIMIT];
		}
		
		money += work_payment;

		strcat(dialog_header, "\n\n{FF6746}____ [ TRABAJOS ] ____\n");
		
		format(dialog_string, sizeof dialog_string, "{FFFFFF}Paga por ser policía: {FFFF7F}%s dólares\n", number_format_thousand(work_payment));
		strcat(dialog_header, dialog_string);
	}

	if(PI[playerid][ac_CREW])
	{
		new 
			territories = GetCrewTerritories(PI[playerid][ac_CREW]),
			graffitis = CountCrewGraffitis(PI[playerid][ac_CREW]);

		if(territories > 0 || graffitis > 0)
		{
			money += territories * 70;
			money += graffitis * 40;

			strcat(dialog_header, "\n\n{FF6746}____ [ TERRITORIOS ] ____\n");

			if(territories > 0) format(dialog_string, sizeof dialog_string, "{FFFFFF}Paga por territorios en posesión (%d): {FFFF7F}%d dólares\n", territories, territories * 70);
			if(graffitis > 0) format(dialog_string, sizeof dialog_string, "{FFFFFF}Paga por graffitis (%d): {FFFF7F}%d dólares\n", graffitis, graffitis * 40);
			strcat(dialog_header, dialog_string);
		}
	}
	
	if(money + PI[playerid][ac_CASH] <= 0)
	{
		money = 0;
		PI[playerid][ac_CASH] = 0;
	}

	strcat(dialog_header, "\n\n{FF6746}____ [ RESUMEN ] ____\n");

	if(money > 0) format(dialog_string, sizeof dialog_string, "{FFFFFF}Has ganado: {FFFF7F}%s dólares", number_format_thousand(money));
	else format(dialog_string, sizeof dialog_string, "{FFFFFF}Has perdido: {FF6746}%s dólares", number_format_thousand(money));
	
	strcat(dialog_header, dialog_string);
	
	ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, dialog_body, dialog_header, "Aceptar", "");
	
	GivePlayerCash(playerid, money, true, money < 0 ? true : false);
	return 1;
}
PlayerWantedColor(playerid)
{
	if(PI[playerid][ac_WANTED_LEVEL] > 0)
	{
		if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return 0x009900ff;
		else
		{
			switch(PI[playerid][ac_WANTED_LEVEL])
			{
				case 1: return 0xfffdbaff;
				case 2: return 0xf7f254ff;
				case 3: return 0xd98900ff;
				case 4: return 0xce4202ff;
				case 5: return 0xff00ffff;
				case 6: return 0x9900ccff;
				default: return 0x9900ccff;
			}
		}
	}
	return PLAYER_COLOR;
}

KickEx(playerid, time = 0)
{
	PLAYER_TEMP[playerid][pt_KICKED] = true;
	if(!time) Kick(playerid);
	else
	{
		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][0]);
		PLAYER_TEMP[playerid][pt_TIMERS][0] = SetTimerEx("KickPlayer", time, false, "i", playerid);
	}
	return 1;
}
DisablePlayerPoliceMark(playerid)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
			{
				if(i == playerid) continue;
				if(!PLAYER_WORKS[i][WORK_POLICE][pwork_SET]) continue;
				if(PLAYER_TEMP[i][pt_WORKING_IN] != WORK_POLICE) continue;
				
				SetPlayerMarkerForPlayer(i, playerid, PLAYER_COLOR);
			}
		}
	}
	return 1;
}

Crew_RequestHelp(playerid, crew_id)
{
	new
		city[45],
		zone[45],
		crew_message[128];

	GetPlayerZones(playerid, city, zone);

	if(!PLAYER_TEMP[playerid][pt_CREW_HELP])
	{
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(IsPlayerConnected(i))
			{
				if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
				{
					if(i == playerid) continue;
					if(PI[i][ac_CREW] != crew_id) continue;
					
					SetPlayerMarkerForPlayer(i, playerid, 0xDD4C4FFF);
				}
			}
		}
		format(crew_message, sizeof crew_message, "{%06x}[Banda] {FFFFFF}%s está pidiendo refuerzos en %s, %s.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, PLAYER_TEMP[playerid][pt_RP_NAME], city, zone);
		SendMessageToCrewMembers(crew_id, -1, crew_message);

		PLAYER_TEMP[playerid][pt_CREW_HELP] = true;
	}
	else
	{
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(IsPlayerConnected(i))
			{
				if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
				{
					if(i == playerid) continue;
					if(PI[i][ac_CREW] != crew_id) continue;
					
					SetPlayerMarkerForPlayer(i, playerid, PLAYER_COLOR);
				}
			}
		}
		format(crew_message, sizeof crew_message, "{%06x}[Banda] {FFFFFF}%s ya no necesita refuerzos.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, PLAYER_TEMP[playerid][pt_RP_NAME]);
		SendMessageToCrewMembers(crew_id, -1, crew_message);

		PLAYER_TEMP[playerid][pt_CREW_HELP] = false;
	}
	return 1;
}

SetWantedMarkerToPolice(playerid)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
			{
				if(i == playerid) continue;
				if(!PLAYER_WORKS[i][WORK_POLICE][pwork_SET]) continue;
				if(PLAYER_TEMP[i][pt_WORKING_IN] != WORK_POLICE) continue;
				
				SetPlayerMarkerForPlayer(i, playerid, PlayerWantedColor(playerid));
			}
		}
	}
	return 1;
}

ShowPlayerStats(playerid, pid)
{
	if(!IsPlayerConnected(pid)) return 0;
	if(ACCOUNT_INFO[pid][ac_ID] == 0) return 0;
	
	new Float:hours;
	ConvertTime_SecondsToHoursFloat(ACCOUNT_INFO[pid][ac_TIME_PLAYING], hours);

	new next_rep[24], neccessary_rep = PI[pid][ac_LEVEL] * REP_MULTIPLIER;
	if(PI[pid][ac_REP] < neccessary_rep)
	{
		new time = (ACCOUNT_INFO[pid][ac_TIME_FOR_REP] - (gettime() * 1000 - PLAYER_TEMP[pid][pt_TIME_PASSED_LAST_REP])) / 1000;
		if(time < 3) time = 3;
		
		format(next_rep, sizeof next_rep, "%sm", TimeConvert(time));
	}
	else format(next_rep, sizeof next_rep, "/comprarnivel");
	
	new caption[48], dialog[445 + (27 * sizeof(work_info))];
	format(caption, sizeof caption, "Estadísticas de %s", PLAYER_TEMP[pid][pt_RP_NAME]);
	
	format(dialog, sizeof dialog,
		
		"\
			ID: %d\n\
			Nombre: %s\n\
			Correo electrónico: %s\n\
			Fecha registro: %s\n\
			Últ. conexión: %s\n\
			Tiempo jugando: %.1f horas\n\
			Nivel: %d\n\
			Reputación: %d/%d\n\
			Siguiente reputación: %s\n\
			Trabajo(s): %s\n\
			Banda: %s\n\
			Dinero: %s$\n\
			Dinero en banco: %s$\n\
			Monedas "SERVER_COIN": %d\n\
			VIP: %s\n\
			\n\
			Para estadísticas de tus trabajos utiliza /exp.\n\
			Para ver tus pertenencias usa /inv o pulsa N.\n\
			\n\
		",
			
			ACCOUNT_INFO[pid][ac_ID],
			ACCOUNT_INFO[pid][ac_NAME],
			ACCOUNT_INFO[pid][ac_EMAIL],
			ACCOUNT_INFO[pid][ac_REG_DATE],
			ACCOUNT_INFO[pid][ac_LAST_CONNECTION],
			hours,
			ACCOUNT_INFO[pid][ac_LEVEL],
			ACCOUNT_INFO[pid][ac_REP],
			neccessary_rep,
			next_rep,
			getPlayerWorks(pid),
			getPlayerCrew(pid),
			number_format_thousand(PI[pid][ac_CASH]),
			number_format_thousand(PI[pid][ac_BANK_MONEY]),
			PI[pid][ac_COINS],
			getPlayerVip(pid)
			
	);
	
	ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, caption, dialog, "Cerrar", "");	
	return 1;
}

getPlayerCrew(playerid)
{
	new crew[32];
	if(!PI[playerid][ac_CREW]) crew = "ninguna";
	else format(crew, sizeof crew, "%s", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_NAME]);
	return crew;
}

getPlayerVip(playerid)
{
	new vip[16];
	if(!ACCOUNT_INFO[playerid][ac_VIP]) vip = "No";
	else format(vip, sizeof vip, "Sí, VIP (%d)", ACCOUNT_INFO[playerid][ac_VIP]);
	return vip;
}

AddPlayerBadHistory(account_id, by_account_id, type, text[])
{
	new date[24];
	getDateTime(date);
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO bad_history (id_player, type, `by`, `text`, `date`) VALUES (%d, %d, %d, '%e', '%e');", account_id, type, by_account_id, text, date);
	mysql_tquery(srp_db, QUERY_BUFFER);
	return 1;
}

AddPlayerBan(account_id, account_name[], account_ip[], by_account_id, type, text[], days = 0)
{
	new date[24], account_name_fix[24], account_ip_fix[24];
	getDateTime(date);
	format(account_name_fix, 24, "%s", account_name);
	format(account_ip_fix, 16, "%s", account_ip);

	if(!days)
	{
		inline OnBadHistoryInserted1()
		{
			new id = cache_insert_id();
			if(id)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO bans (name, ip, id_history) VALUES ('%e', '%e', %d);", account_name_fix, account_ip_fix, id);
				mysql_tquery(srp_db, QUERY_BUFFER);
			}
		}
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO bad_history (id_player, type, `by`, `text`, `date`) VALUES (%d, %d, %d, '%e', '%e');", account_id, type, by_account_id, text, date);
		MySQL_TQueryInline(srp_db, using inline OnBadHistoryInserted1, QUERY_BUFFER);
	}
	else
	{
		inline OnBadHistoryInserted2()
		{
			new id = cache_insert_id();
			if(id)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO bans (name, ip, id_history, expire_date) VALUES ('%e', '%e', %d, DATE_ADD(NOW(), INTERVAL %d DAY));", account_name_fix, account_ip_fix, id, days);
				mysql_tquery(srp_db, QUERY_BUFFER);
			}
		}
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO bad_history (id_player, type, `by`, `text`, `date`) VALUES (%d, %d, %d, '%e', '%e');", account_id, type, by_account_id, text, date);
		MySQL_TQueryInline(srp_db, using inline OnBadHistoryInserted2, QUERY_BUFFER);
	}
	return 1;
}



RegisterVehicleBootObject(vehicleid, boot_slot, type, int, extra)
{
	if(!PLAYER_VEHICLES[vehicleid][player_vehicle_ID]) return 0;

	VEHICLE_BOOT[vehicleid][boot_slot][vehicle_boot_VALID] = false;
	VEHICLE_BOOT[vehicleid][boot_slot][vehicle_boot_TYPE] = type;
	VEHICLE_BOOT[vehicleid][boot_slot][vehicle_boot_INT] = int;
	VEHICLE_BOOT[vehicleid][boot_slot][vehicle_boot_INT_EXTRA] = extra;
	
	inline OnVbootInserted()
	{
		VEHICLE_BOOT[vehicleid][boot_slot][vehicle_boot_OBJECT_ID] = cache_insert_id();
		if(VEHICLE_BOOT[vehicleid][boot_slot][vehicle_boot_OBJECT_ID]) VEHICLE_BOOT[vehicleid][boot_slot][vehicle_boot_VALID] = true;
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO vboot (id_vehicle, type, `int`, int_extra) VALUES (%d, %d, %d, %d);", PLAYER_VEHICLES[vehicleid][player_vehicle_ID], VEHICLE_BOOT[vehicleid][boot_slot][vehicle_boot_TYPE], VEHICLE_BOOT[vehicleid][boot_slot][vehicle_boot_INT], VEHICLE_BOOT[vehicleid][boot_slot][vehicle_boot_INT_EXTRA]);
	MySQL_TQueryInline(srp_db, using inline OnVbootInserted, QUERY_BUFFER);
	return 1;
}

SavePlayerWeaponInVehicleBoot(playerid, weapon_slot, vehicleid, boot_slot)
{
	RegisterVehicleBootObject(vehicleid, boot_slot, BOOT_TYPE_WEAPON, PLAYER_WEAPONS[playerid][weapon_slot][player_weapon_ID], PLAYER_WEAPONS[playerid][weapon_slot][player_weapon_AMMO]);
	RemovePlayerSlotWeapon(playerid, weapon_slot, true);
	return 1;
}

GetPropertyAvaibleClosetSlot(index)
{
	new max_slots = MAX_CLOSET_SLOTS;
	for(new i = 0; i != max_slots; i ++)
	{
		if(!PROPERTY_CLOSET[index][i][property_closet_VALID]) return i;
	}
	return -1;
}

RegisterPropertyClosetObject(index, closet_slot, type, int, extra)
{
	PROPERTY_CLOSET[index][closet_slot][property_closet_VALID] = false;
	PROPERTY_CLOSET[index][closet_slot][property_closet_TYPE] = type;
	PROPERTY_CLOSET[index][closet_slot][property_closet_INT] = int;
	PROPERTY_CLOSET[index][closet_slot][property_closet_INT_EXTRA] = extra;
	
	inline OnPropertyClosetInserted()
	{
		PROPERTY_CLOSET[index][closet_slot][property_closet_OBJECT_ID] = cache_insert_id();
		if(PROPERTY_CLOSET[index][closet_slot][property_closet_OBJECT_ID]) PROPERTY_CLOSET[index][closet_slot][property_closet_VALID] = true;
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO property_closet (id_property, type, `int`, int_extra) VALUES (%d, %d, %d, %d);", PROPERTY_INFO[index][property_ID], type, int, extra);
	MySQL_TQueryInline(srp_db, using inline OnPropertyClosetInserted, QUERY_BUFFER);
	return 1;
}

SaveWeaponInPropertyCloset(playerid, weapon_slot, index, closet_slot)
{
	RegisterPropertyClosetObject(index, closet_slot, CLOSET_TYPE_WEAPON, PLAYER_WEAPONS[playerid][weapon_slot][player_weapon_ID], PLAYER_WEAPONS[playerid][weapon_slot][player_weapon_AMMO]);
	RemovePlayerSlotWeapon(playerid, weapon_slot, true);
	return 1;
}

SetCorrectObjectPos(objectid)
{
	new Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz;
	GetDynamicObjectPos(objectid, x, y, z);
	GetDynamicObjectRot(objectid, rx, ry, rz);

	SetDynamicObjectPos(objectid, x, y, z);
	SetDynamicObjectRot(objectid, rx, ry, rz);
}

public OnPlayerEditDynamicObject(playerid, STREAMER_TAG_OBJECT objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	switch(response)
	{
		case EDIT_RESPONSE_CANCEL:
		{
			new info[3];
			Streamer_GetArrayData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_EXTRA_ID, info);
			switch(info[0])
			{
				case OBJECT_TYPE_POLICE:
				{
					if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
					{
						CancelEdit(playerid);
						SendNotification(playerid, "No eres policía.");
						return 1;
					}
					if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE)
					{
						CancelEdit(playerid);
						SendNotification(playerid, "No estás de servicio como policía.");
						return 1;
					}
					if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] < 8)
					{
						CancelEdit(playerid);
						SendFormatNotification(playerid, "Debes ser al menos rango ~r~%s~w~ para colocar objetos policiales.", POLICE_RANKS[8]);
						return 1;
					}
					
					DestroyDynamicObject(POLICE_OBJECTS[ info[1] ][police_object_OBJECT_ID]);
					POLICE_OBJECTS[ info[1] ][police_object_VALID] = false;
					POLICE_OBJECTS[ info[1] ][police_object_USER][0] = EOS;
					POLICE_OBJECTS[ info[1] ][police_object_OBJECT_ID] = INVALID_STREAMER_ID;
					SendNotification(playerid, "Objeto policial eliminado.");
				}
			}
		}
		case EDIT_RESPONSE_FINAL:
		{
			new info[3];
			Streamer_GetArrayData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_EXTRA_ID, info);
			
			switch(info[0])
			{
				case OBJECT_TYPE_POLICE:
				{
					if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
					{
						CancelEdit(playerid);
						SendNotification(playerid, "No eres policía.");
						SetCorrectObjectPos(objectid);
						return 1;
					}
					if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE)
					{
						CancelEdit(playerid);
						SendNotification(playerid, "No estás de servicio como policía.");
						SetCorrectObjectPos(objectid);
						return 1;
					}
					if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] < 8)
					{
						CancelEdit(playerid);
						SendFormatNotification(playerid, "Debes ser al menos rango ~r~%s~w~ para colocar objetos policiales.", POLICE_RANKS[8]);
						SetCorrectObjectPos(objectid);
						return 1;
					}

					SetDynamicObjectPos(objectid, x, y, z);
					SetDynamicObjectRot(objectid, rx, ry, rz);

					SendNotification(playerid, "Objeto movido correctamente");

					format(POLICE_OBJECTS[ info[1] ][police_object_USER], 24, "%s", ACCOUNT_INFO[playerid][ac_NAME]);

				}
				case OBJECT_TYPE_TEST: {
					printf("CreateDynamicObject(%d, %f, %f, %f, %f, %f, %f);", Streamer_GetIntData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_MODEL_ID), x, y, z, rx, ry, rz);
				}
			}
		}
	}
	return 1;
}

IPacket:CRASHER_PACKET(playerid, BitStream:bs)
{
	printf("[ADVERTENCIA] Packet 228 recibido!");
	KickEx(playerid);
}

CountPoliceRadioPlayers(radio)
{
	new total;
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
			{
				if(PLAYER_WORKS[i][WORK_POLICE][pwork_SET])
				{
					if(PLAYER_TEMP[i][pt_WORKING_IN] == WORK_POLICE)
					{
						if(PLAYER_TEMP[i][pt_POLICE_RADIO] == radio)
						{
							total ++;
						}
					}
				}
			}
		}
	}
	return total;
}

GetPlayerZones(playerid, city[], zone[])
{
	new Float:f_Pos[3];
	
	switch(ACCOUNT_INFO[playerid][ac_STATE])
	{
		case ROLEPLAY_STATE_INTERIOR:
		{
			f_Pos[0] = ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_EXT_X];
			f_Pos[1] = ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_EXT_Y];
			f_Pos[2] = ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_EXT_Z];
		}
		case ROLEPLAY_STATE_OWN_PROPERTY, ROLEPLAY_STATE_GUEST_PROPERTY:
		{
			f_Pos[0] = PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] ][property_EXT_X];
			f_Pos[1] = PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] ][property_EXT_Y];
			f_Pos[2] = PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] ][property_EXT_Z];
		}
		default: GetPlayerPos(playerid, f_Pos[0], f_Pos[1], f_Pos[2]);
	}

    format(city, 45, "San Andreas");
    format(zone, 45, "Desconocida");

	for(new i = 0; i < sizeof(g_Cities); i++)
	{
	    if(	(f_Pos[0] >= g_Cities[i][z_MinX] && f_Pos[0] < g_Cities[i][z_MaxX]) && (f_Pos[1] >= g_Cities[i][z_MinY] && f_Pos[1] < g_Cities[i][z_MaxY]) )
		{
			format(city, 45, g_Cities[i][z_Name]);
		    break;
		}
	}

	for(new i = 0; i < sizeof(g_Zones); i++)
	{
	    if(	(f_Pos[0] >= g_Zones[i][z_MinX] && f_Pos[0] < g_Zones[i][z_MaxX]) && (f_Pos[1] >= g_Zones[i][z_MinY] && f_Pos[1] < g_Zones[i][z_MaxY]) )
		{
			format(zone, 45, g_Zones[i][z_Name]);
		    break;
		}
	}
	
	return 1;
}

SetPlayerWantedLevelEx(playerid, level)
{
	PI[playerid][ac_WANTED_LEVEL] = level;

	if(PI[playerid][ac_WANTED_LEVEL] == 0)
	{
		DisablePlayerPoliceMark(playerid);
		PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0]);
		PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1]);
	}
	else
	{
		new td_str[24];
		for(new i = 0; i != PI[playerid][ac_WANTED_LEVEL]; i ++) strcat(td_str, "]");
		PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1], td_str);

		PLAYER_TEMP[playerid][pt_LAST_SET_WANTED_LEVEL] = gettime();
	
		PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0]);
		PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1]);
		SetWantedMarkerToPolice(playerid);
	}
	return 1;
}

JailPlayer(playerid, time = 0)
{
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1]);
		
	ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_JAIL;

	if(time)
		PI[playerid][ac_POLICE_JAIL_TIME] = time;
	else
	{	
		if(ACCOUNT_INFO[playerid][ac_VIP]) PI[playerid][ac_POLICE_JAIL_TIME] = 150 * PI[playerid][ac_WANTED_LEVEL];
		else PI[playerid][ac_POLICE_JAIL_TIME] = 300 * PI[playerid][ac_WANTED_LEVEL];
	}

	SetPlayerWantedLevelEx(playerid, 0);
	PLAYER_TEMP[playerid][pt_ENTER_JAIL_TIME] = gettime();
	SetPlayerPosEx(playerid, JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID] ][jail_X], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_Y], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_Z], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_ANGLE], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_INTERIOR], 0, true);
	
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][15]);
	PLAYER_TEMP[playerid][pt_TIMERS][15] = SetTimerEx("UnjailPlayer", PI[playerid][ac_POLICE_JAIL_TIME] * 1000, false, "i", playerid);
	
	SendFormatNotification(playerid, "Te quedan %s minutos de condena.", TimeConvert( PI[playerid][ac_POLICE_JAIL_TIME] - (gettime() - PLAYER_TEMP[playerid][pt_ENTER_JAIL_TIME]) ));
	ResetPlayerWeaponsEx(playerid);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	DisablePlayerPoliceMark(playerid);
	DeleteIlegalInv(playerid);
	return 1;
}

SendChatMessageToAdmins(color, const message[])
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++) 
	    if(IsPlayerConnected(i) && PLAYER_TEMP[i][pt_USER_LOGGED])
	        if(ACCOUNT_INFO[i][ac_ADMIN_LEVEL] > 1 && PLAYER_TEMP[i][pt_SEE_ADM_CHAT] /*&& PLAYER_TEMP[i][pt_ADMIN_SERVICE]*/)
	            SendClientMessage(i, color, message);
}

SendMessageToAdmins(color, const message[], level = 1)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++) 
	    if(IsPlayerConnected(i) && PLAYER_TEMP[i][pt_USER_LOGGED])
	        if(ACCOUNT_INFO[i][ac_ADMIN_LEVEL] > level && PLAYER_TEMP[i][pt_SEE_ADM_LOG] /*&& PLAYER_TEMP[i][pt_ADMIN_SERVICE]*/)
	            SendClientMessage(i, color, message);

	return 1;
}

SendCmdLogToAdmins(playerid, cmdtext[], params[])
{
	new message[145];
	if(isnull(params)) format(message, sizeof message, "El administrador %s (%d) ha utilizado el comando /%s", ACCOUNT_INFO[playerid][ac_NAME], playerid, cmdtext);
	else format(message, sizeof message, "El administrador %s (%d) ha utilizado el comando /%s %s", ACCOUNT_INFO[playerid][ac_NAME], playerid, cmdtext, params);
	
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++) 
	    if(IsPlayerConnected(i) && PLAYER_TEMP[i][pt_USER_LOGGED])
	        if(ACCOUNT_INFO[i][ac_ADMIN_LEVEL] >= ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] && PLAYER_TEMP[i][pt_SEE_ACMD_LOG] /*&& PLAYER_TEMP[i][pt_ADMIN_SERVICE]*/)
	            SendClientMessage(i, 0xA9C4E4FF, message);
	
	return 1;
}

SendMessageToAdminsAC(color, const message[])
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++) 
	    if(IsPlayerConnected(i) && PLAYER_TEMP[i][pt_USER_LOGGED])
	        if(ACCOUNT_INFO[i][ac_ADMIN_LEVEL] > 1 && PLAYER_TEMP[i][pt_SEE_AC_LOG] /*&& PLAYER_TEMP[i][pt_ADMIN_SERVICE]*/)
	            SendClientMessage(i, color, message);
	
	return 1;
}

RespawnAllUnoccupiedVehicles()
{
	for(new i = 0, j = GetVehiclePoolSize(); i <= j; i ++)
	{
		if(!GLOBAL_VEHICLES[i][gb_vehicle_VALID]) continue;
		if(GLOBAL_VEHICLES[i][gb_vehicle_OCCUPIED]) continue;
		if(GLOBAL_VEHICLES[i][gb_vehicle_TYPE] == VEHICLE_TYPE_TEST)
		{
			DestroyVehicleEx(i);
			continue;
		}
		
		SetVehicleToRespawnEx(i);
	}
	return 1;
}

RespawnGlobalUnoccupiedVehicles()
{
	for(new i = 0, j = GetVehiclePoolSize(); i <= j; i ++)
	{
		if(!GLOBAL_VEHICLES[i][gb_vehicle_VALID]) continue;
		if(GLOBAL_VEHICLES[i][gb_vehicle_TYPE] == VEHICLE_TYPE_PERSONAL) continue;
		if(GLOBAL_VEHICLES[i][gb_vehicle_OCCUPIED]) continue;
	
		SetVehicleToRespawnEx(i);
	}
	return 1;
}



CMD:kill(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK) {
		new now = gettime();
		if(now < PLAYER_TEMP[playerid][pt_DEATH_TIME] + 30)
		{
			new time = 30 - (now - PLAYER_TEMP[playerid][pt_DEATH_TIME]);
			SendFormatNotification(playerid, "Tienes que esperar %s minutos para poder usar este comando.", TimeConvert(time));
			return 1;
		}
	}

	SetPlayerHealthEx(playerid, 0.0);
	return 1;
}

GetNumberOfPlayersOnThisIP(const test_ip[])
{
	new against_ip[16], ip_count = 0;
	for(new x = 0, j = GetPlayerPoolSize(); x <= j; x++) 
	{
		if(IsPlayerConnected(x)) 
		{
		    GetPlayerIp(x, against_ip, 16);
		    if(!strcmp(against_ip, test_ip)) ip_count++;
		}
	}
	return ip_count;
}

getRandomSalt(salt[], length = sizeof salt)
{
    for(new i = 0; i != length; i ++)
	{
		salt[i] = random(2) ? (random(26) + (random(2) ? 'a' : 'A')) : (random(10) + '0');
	}
	return true;
}

GetTickDiff(newtick, oldtick)
{
	if (oldtick < 0 && newtick >= 0) {
		return newtick - oldtick;
	} else if (oldtick >= 0 && newtick < 0 || oldtick > newtick) {
		return (cellmax - oldtick + 1) - (cellmin - newtick);
	}
	return newtick - oldtick;
}

GetPlayerSpeed(vehicleid)
{
	new Float:xPos[3];
	GetPlayerVelocity(vehicleid, xPos[0], xPos[1], xPos[2]);
	return floatround(floatsqroot(xPos[0] * xPos[0] + xPos[1] * xPos[1] + xPos[2] * xPos[2]) * 170.00);
}

ShowPlayerShop(playerid)
{
	TextDrawShowForPlayer(playerid, Textdraws[textdraw_SHOP][0]);
	TextDrawShowForPlayer(playerid, Textdraws[textdraw_SHOP][1]);
	TextDrawShowForPlayer(playerid, Textdraws[textdraw_SHOP][2]);
	TextDrawShowForPlayer(playerid, Textdraws[textdraw_SHOP][3]);
	TextDrawShowForPlayer(playerid, Textdraws[textdraw_SHOP][4]);
	
	PLAYER_TEMP[playerid][pt_SHOP_COME_FROM_MY_ADS] = false;
	PLAYER_TEMP[playerid][pt_SHOP] = true;
	PLAYER_TEMP[playerid][pt_SHOP_STATE] = PLAYER_SHOP_STATE_ALL;
	
	PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] = 6;
	PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
	
	UpdatePlayerShop(playerid);
	SelectTextDrawEx(playerid, 0x00a39bFF);
	return 1;
}

UpdatePlayerShop(playerid)
{
	switch(PLAYER_TEMP[playerid][pt_SHOP_STATE])
	{
		case PLAYER_SHOP_STATE_ALL:
		{
			if(PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID] != PlayerText:INVALID_TEXT_DRAW)
			{
				PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID]);
				PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID] = PlayerText:INVALID_TEXT_DRAW;
			}
			TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP_ARTICLE_BOX]);
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER]);
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO]);
			
			for(new i = 0; i != 6; i ++)
			{
				PLAYER_TEMP[playerid][pt_SHOP_ARTICLE_ID][i] = 0;
				TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP_ARTICLES_BOX][i]);
				if(i <= 4) PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][i]);
			}
			
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], "+_Publicar_anuncio");
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], "-_Mis_anuncios");
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], "Cerrar");
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], "Siguiente");
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], "Anterior");
			
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4]);
			
			
			DestroyArticleShopTextDraws(playerid);
			
			//
			inline OnShopLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					for(new i = 0; i != rows; i ++)
					{
						new connected, id_ad, price, modelid, Float:rx, Float:ry, Float:rz, Float:zoom, vcol1, vcol2;
						cache_get_value_name_int(i, "connected", connected);
						cache_get_value_name_int(i, "id_ad", id_ad);
						cache_get_value_name_int(i, "price", price);
						cache_get_value_name_int(i, "modelid", modelid);
						cache_get_value_name_float(i, "rx", rx);
						cache_get_value_name_float(i, "ry", ry);
						cache_get_value_name_float(i, "rz", rz);
						cache_get_value_name_float(i, "zoom", zoom);
						cache_get_value_name_int(i, "vcol1", vcol1);
						cache_get_value_name_int(i, "vcol2", vcol2);
						PLAYER_TEMP[playerid][pt_SHOP_ARTICLE_ID][i] = id_ad;

						new Float:Td_Pos[2], td_str[64];
						switch(i)
						{
							case 0: Td_Pos = Float:{212.000000, 152.000000};
							case 1: Td_Pos = Float:{289.000000, 152.000000};
							case 2: Td_Pos = Float:{366.000000, 152.000000};
							case 3: Td_Pos = Float:{212.000000, 229.000000};
							case 4: Td_Pos = Float:{289.000000, 229.000000};
							case 5: Td_Pos = Float:{366.000000, 229.000000};
						}

						PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i] = CreatePlayerTextDraw(playerid, Td_Pos[0], Td_Pos[1], "");
						PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], 62.000000, 55.000000);
						PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], 1);
						PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], -1);
						if(connected) PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], 0xCCCCCCFF);
						else PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], 0x666666FF);
						PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], 5);
						PlayerTextDrawSetPreviewModel(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], modelid);
						PlayerTextDrawSetPreviewRot(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], rx, ry, rz, zoom);
						PlayerTextDrawSetPreviewVehCol(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], vcol1, vcol2);
						PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i]);
						
						format(td_str, sizeof td_str, "%s$", number_format_thousand(price));
						PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i] = CreatePlayerTextDraw(playerid, Td_Pos[0] + 31.0, Td_Pos[1] + 55.0, td_str);
						PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 0.229333, 0.957037);
						PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 2);
						PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], -1);
						PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 0);
						PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 0);
						PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 255);
						PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 1);
						PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 1);
						PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 0);
						PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i]);

						TextDrawShowForPlayer(playerid, Textdraws[textdraw_SHOP_ARTICLES_BOX][i]);

						inline OnCountQueryLoad()
						{
							new crows;
							if(cache_get_row_count(crows))
							{
								if(crows)
								{
									new count, pages;
									cache_get_value_index_int(0, 0, count);
									pages = GetDatabasePages(count, PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);

									format(td_str, sizeof td_str, "-_%d/%d_-", PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] + 1, pages);
									PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE], td_str);
								}
							}
						}
						MySQL_TQueryInline(srp_db,  using inline OnCountQueryLoad, "SELECT COUNT(id) FROM shop;");
						PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE]);
					}
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT player.connected, shop.*, shop.id AS id_ad FROM shop INNER JOIN player ON shop.id_player = player.id ORDER BY player.connected DESC, shop.date DESC LIMIT %d, %d;", PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT], PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);
			MySQL_TQueryInline(srp_db, using inline OnShopLoad, QUERY_BUFFER);
			return 1;
		}
		case PLAYER_SHOP_STATE_MY_ADS:
		{
			if(PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID] != PlayerText:INVALID_TEXT_DRAW)
			{
				PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID]);
				PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID] = PlayerText:INVALID_TEXT_DRAW;
			}
			TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP_ARTICLE_BOX]);
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER]);
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO]);
			
			for(new i = 0; i != 6; i ++)
			{
				PLAYER_TEMP[playerid][pt_SHOP_ARTICLE_ID][i] = 0;
				TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP_ARTICLES_BOX][i]);
				if(i <= 4) PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][i]);
			}
			
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], "_");
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], "_");
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], "Atr˜s");
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], "Siguiente");
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], "Anterior");

			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4]);
			
			
			DestroyArticleShopTextDraws(playerid);
			
			//
			inline OnShopLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new td_str[64];
					for(new i = 0; i != rows; i ++)
					{
						new connected, ad_id, price, modelid, Float:rx, Float:ry, Float:rz, Float:zoom, vcol1, vcol2;
						cache_get_value_name_int(i, "connected", connected);
						cache_get_value_name_int(i, "ad_id", ad_id);
						cache_get_value_name_int(i, "price", price);
						cache_get_value_name_int(i, "modelid", modelid);
						cache_get_value_name_float(i, "rx", rx);
						cache_get_value_name_float(i, "ry", ry);
						cache_get_value_name_float(i, "rz", rz);
						cache_get_value_name_float(i, "zoom", zoom);
						cache_get_value_name_int(i, "vcol1", vcol1);
						cache_get_value_name_int(i, "vcol2", vcol2);
						PLAYER_TEMP[playerid][pt_SHOP_ARTICLE_ID][i] = ad_id;

						new Float:Td_Pos[2];
						switch(i)
						{
							case 0: Td_Pos = Float:{212.000000, 152.000000};
							case 1: Td_Pos = Float:{289.000000, 152.000000};
							case 2: Td_Pos = Float:{366.000000, 152.000000};
							case 3: Td_Pos = Float:{212.000000, 229.000000};
							case 4: Td_Pos = Float:{289.000000, 229.000000};
							case 5: Td_Pos = Float:{366.000000, 229.000000};
						}

						PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i] = CreatePlayerTextDraw(playerid, Td_Pos[0], Td_Pos[1], "");
						PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], 62.000000, 55.000000);
						PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], 1);
						PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], -1);
						if(connected) PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], 0xCCCCCCFF);
						else PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], 0x666666FF);
						PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], 5);
						PlayerTextDrawSetPreviewModel(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], modelid);
						PlayerTextDrawSetPreviewRot(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], rx, ry, rz, zoom);
						PlayerTextDrawSetPreviewVehCol(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i], vcol1, vcol2);
						PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i]);
						
						format(td_str, sizeof td_str, "%s$", number_format_thousand(price));
						PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i] = CreatePlayerTextDraw(playerid, Td_Pos[0] + 31.0, Td_Pos[1] + 55.0, td_str);
						PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 0.229333, 0.957037);
						PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 2);
						PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], -1);
						PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 0);
						PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 0);
						PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 255);
						PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 1);
						PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 1);
						PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i], 0);
						PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i]);

						TextDrawShowForPlayer(playerid, Textdraws[textdraw_SHOP_ARTICLES_BOX][i]);

						inline OnCountQueryLoad()
						{
							new crows;
							if(cache_get_row_count(crows))
							{
								if(crows)
								{
									new count, pages;
									cache_get_value_index_int(0, 0, count);
									pages = GetDatabasePages(count, PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);

									format(td_str, sizeof td_str, "-_%d/%d_-", PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] + 1, pages);
									PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE], td_str);
								}
							}
						}
						mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM shop WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
						MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
						PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE]);
					}
				}
			}
			new limit1 = PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT];
			if(limit1 < 0) limit1 = 0;
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT player.connected, shop.*, shop.id AS ad_id FROM shop INNER JOIN player ON shop.id_player = player.id WHERE shop.id_player = %d ORDER BY player.connected DESC, shop.date DESC LIMIT %d, %d;", ACCOUNT_INFO[playerid][ac_ID], limit1, PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);
			MySQL_TQueryInline(srp_db, using inline OnShopLoad, QUERY_BUFFER);
			return 1;
		}
		case PLAYER_SHOP_STATE_AD:
		{
			inline OnShopAdLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new td_str[190];
					if(rows)
					{
						new id, id_player, type, price, text[24], modelid, Float:rx, Float:ry, Float:rz, Float:zoom, vcol1, vcol2, date[24], connected, name[24], phone_number;
						cache_get_value_name_int(0, "id", id);
						cache_get_value_name_int(0, "id_player", id_player);
						cache_get_value_name_int(0, "type", type);
						cache_get_value_name_int(0, "price", price);
						cache_get_value_name(0, "text", text);
						cache_get_value_name_int(0, "modelid", modelid);
						cache_get_value_name_float(0, "rx", rx);
						cache_get_value_name_float(0, "ry", ry);
						cache_get_value_name_float(0, "rz", rz);
						cache_get_value_name_float(0, "zoom", zoom);
						cache_get_value_name_int(0, "vcol1", vcol1);
						cache_get_value_name_int(0, "vcol2", vcol2);
						cache_get_value_name(0, "date", date);
						cache_get_value_name_int(0, "connected", connected);
						cache_get_value_name(0, "name", name);
						cache_get_value_name_int(0, "phone_number", phone_number);

						if(PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID] != PlayerText:INVALID_TEXT_DRAW)
						{
							PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID]);
							PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID] = PlayerText:INVALID_TEXT_DRAW;
						}
						PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID] = CreatePlayerTextDraw(playerid, 212.000000, 179.000000, "");
						PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID], 0.000000, 0.000000);
						PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID], 92.000000, 91.000000);
						PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID], 1);
						PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID], -1);
						PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID], 0);
						PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID], 0);
						PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID], 44284927);
						PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID], 5);
						PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID], 0);
						PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID], 0);
						PlayerTextDrawSetPreviewModel(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID], modelid);
						PlayerTextDrawSetPreviewRot(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID], rx, ry, rz, zoom);
						PlayerTextDrawSetPreviewVehCol(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID], vcol1, vcol2);
						PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID]);
						TextDrawShowForPlayer(playerid, Textdraws[textdraw_SHOP_ARTICLE_BOX]);
						
						format(td_str, sizeof td_str, "Art¢culo_n|_%d (%s)", id, date);
						PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER], td_str);
						PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER]);
						
						switch(type)
						{
							case SHOP_ARTICLE_TYPE_PROPERTY: format(td_str, sizeof td_str, "Producto:_Propiedad~n~Precio:_%s$~n~~n~Descripci¦n:~n~%s~n~~n~Tlf:_%d_(%s)~n~%s", number_format_thousand(price), text, phone_number, connected ? "online" : "offline", name);
							case SHOP_ARTICLE_TYPE_VEHICLE: format(td_str, sizeof td_str, "Producto:_Veh¢culo~n~Precio:_%s$~n~~n~Descripci¦n:~n~%s~n~~n~Tlf:_%d_(%s)~n~%s", number_format_thousand(price), text, phone_number, connected ? "online" : "offline", name);
							case SHOP_ARTICLE_TYPE_OTHER: format(td_str, sizeof td_str, "Producto:_Otros~n~Precio:_%s$~n~~n~Descripci¦n:~n~%s~n~~n~Tlf:_%d_(%s)~n~%s", number_format_thousand(price), text, phone_number, connected ? "online" : "offline", name);
						}
						PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO], td_str);
						PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO]);
						
						for(new i = 0; i != 4; i ++) PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][i]);
						PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], "_");
						PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], "_");
						PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], "_");
						PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], "_");
						PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], "Atr˜s");
						PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4]);
						
						if(ACCOUNT_INFO[playerid][ac_ID] == id_player)
						{
							PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], "Modificar");
							PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], "Eliminar");
							PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2]);
							PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3]);
						}

						for(new i = 0; i != 6; i ++)
						{
							PLAYER_TEMP[playerid][pt_SHOP_ARTICLE_ID][i] = 0;
							TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP_ARTICLES_BOX][i]);
						}
						
						PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE]);
						DestroyArticleShopTextDraws(playerid);
					}
					else
					{
						SendNotification(playerid, "Este anuncio ya no está disponible.");
						PLAYER_TEMP[playerid][pt_SHOP_STATE] = PLAYER_SHOP_STATE_ALL;
					}
				}
				else
				{
					SendNotification(playerid, "Este anuncio ya no está disponible.");
					PLAYER_TEMP[playerid][pt_SHOP_STATE] = PLAYER_SHOP_STATE_ALL;
				}

			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT shop.*, player.connected, player.name, player.phone_number FROM shop INNER JOIN player ON shop.id_player = player.id WHERE shop.id = %d;", PLAYER_TEMP[playerid][pt_SHOP_SELECTED_ARTICLE_ID]);
			MySQL_TQueryInline(srp_db, using inline OnShopAdLoad, QUERY_BUFFER);
		}
	}
	return 1;
}

HidePlayerShop(playerid)
{
	HidePlayerDialog(playerid);
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP][0]);
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP][1]);
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP][2]);
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP][3]);
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP][4]);
	
	PLAYER_TEMP[playerid][pt_SHOP_COME_FROM_MY_ADS] = false;
	PLAYER_TEMP[playerid][pt_SHOP] = false;
	PLAYER_TEMP[playerid][pt_SHOP_STATE] = 0;
	
	PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] = 0;
	PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
	
	if(PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID]);
		PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_MODELID] = PlayerText:INVALID_TEXT_DRAW;
	}
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP_ARTICLE_BOX]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE]);
	
	for(new i = 0; i != 6; i ++)
	{
		PLAYER_TEMP[playerid][pt_SHOP_ARTICLE_ID][i] = 0;
		TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP_ARTICLES_BOX][i]);
		if(i <= 4) PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][i]);
	}
	DestroyArticleShopTextDraws(playerid);
	
	CancelSelectTextDrawEx(playerid);
	return 1;
}

DestroyArticleShopTextDraws(playerid)
{
	for(new i; i < 6; i++)
	{
		if(PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i] != PlayerText:INVALID_TEXT_DRAW)
		{
			PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i]);
			PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_MODEL][i] = PlayerText:INVALID_TEXT_DRAW;
		}
		if(PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i] != PlayerText:INVALID_TEXT_DRAW)
		{
			PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i]);
			PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PRICE][i] = PlayerText:INVALID_TEXT_DRAW;
		}
	}
	return 1;
}

td_str_legal(const text[])
{
	for(new i = 0; i != strlen(text); i ++)
	{
		if((text[i] >= 91 && text[i] <= 96) || (text[i] >= 58 && text[i] <= 64) || (text[i] < 48 && text[i] != 32) || text[i] > 122)
		{
			return false;
		}
	}
	return true;
}



CountPlayerJobs(playerid)
{
	new count;
	for(new i = 1; i != sizeof(work_info); i ++)
	{
		if(PLAYER_WORKS[playerid][i][pwork_SET]) count ++;
	}
	return count;
}

getPlayerWorks(playerid)
{
	new works[27 * sizeof(work_info)], count;
	for(new i = 1; i != sizeof(work_info); i ++)
	{
		if(PLAYER_WORKS[playerid][i][pwork_SET])
		{
			if(count > 0) strcat(works, ", ");
			strcat(works, work_info[i][work_info_NAME]);
			
			count ++;
		}
	}
	
	if(!count) works = "ninguno";
	return works;
}

SetPlayer_Pizza_Checkpoint(playerid, point)
{
	if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]))
	{
		DestroyDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT] = INVALID_STREAMER_ID;
	}
	if(IsValidDynamicActor(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR]))
	{
		DestroyDynamicActor(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR]);
		PLAYER_TEMP[playerid][pt_PIZZA_ACTOR] = INVALID_STREAMER_ID;
	}
	
	if(point == -1)
	{
		PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT] = CreateDynamicCP(2096.279785, -1797.436279, 12.982056, 3.0, 0, 0, playerid, 9999999999.0);
		
		new info[1];
		info[0] = CHECKPOINT_TYPE_PIZZA_FINISH;
		Streamer_SetArrayData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT], E_STREAMER_EXTRA_ID, info);
	}
	else
	{
		PLAYER_TEMP[playerid][pt_PIZZA_ACTOR] = CreateDynamicActor(minrand(0, 311), PIZZA_DELIVERY_POINT[point][0], PIZZA_DELIVERY_POINT[point][1], PIZZA_DELIVERY_POINT[point][2], PIZZA_DELIVERY_POINT[point][3], .playerid = playerid);
		
		new Float:pos[3];
		pos[0] = PIZZA_DELIVERY_POINT[point][0] + (1.5 * floatsin(-PIZZA_DELIVERY_POINT[point][3], degrees));
		pos[1] = PIZZA_DELIVERY_POINT[point][1] + (1.5 * floatcos(-PIZZA_DELIVERY_POINT[point][3], degrees));
		pos[2] = PIZZA_DELIVERY_POINT[point][2];
	
		PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT] = CreateDynamicCP(pos[0], pos[1], pos[2], 1.0, 0, 0, playerid, 9999999999.0);
		
		new info[1];
		info[0] = CHECKPOINT_TYPE_PIZZA_DEL;
		Streamer_SetArrayData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT], E_STREAMER_EXTRA_ID, info);
	}
	return 1;
}

randomEx(max, exception)
{
	new rand = random(max);
	while(rand == exception)
	{
		rand = random(max);
	}
	return rand;
}

SetPlayerSkillLevels(playerid)
{	
	if(ACCOUNT_INFO[playerid][ac_LEVEL] >= 10)
	{
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN,		200);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, 				40);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL_SILENCED, 		500);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_DESERT_EAGLE, 		999);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SHOTGUN, 				999);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SPAS12_SHOTGUN, 		999);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, 			50);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_MP5, 					999);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_AK47, 				999);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_M4, 					999);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SNIPERRIFLE, 			999);
	}
	else
	{
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL_SILENCED, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_DESERT_EAGLE, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SHOTGUN, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SPAS12_SHOTGUN, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_MP5, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_AK47, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_M4, 0);
		SetPlayerSkillLevel(playerid, WEAPONSKILL_SNIPERRIFLE, 0);
	}
	return 1;
}

DeleteIlegalInv(playerid)
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
	{
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET seed_cannabis = 0, seed_crack = 0, cannabis = 0, crack = 0 WHERE id = %d;", ACCOUNT_INFO[playerid][ac_ID]);
		mysql_tquery(srp_db, QUERY_BUFFER);

		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM pweapons WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
		mysql_tquery(srp_db, QUERY_BUFFER);
		ResetPlayerWeaponsEx(playerid);
	}
	
	PI[playerid][ac_SEED_CANNABIS] = 0;
	PI[playerid][ac_SEED_CRACK] = 0;
	PI[playerid][ac_CANNABIS] = 0;
	PI[playerid][ac_CRACK] = 0;
	return 1;
}

SetInjuredPlayerMarkers(playerid)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
			{
				if(ACCOUNT_INFO[i][ac_STATE] == ROLEPLAY_STATE_CRACK)
				{
					SetPlayerMarkerForPlayer(playerid, i, 0x4aba00FF);
				}
			}
		}
	}
	return 1;
}

SendAlertToMedics(playerid)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
			{
				if(i == playerid) continue;
				if(!PLAYER_WORKS[i][WORK_MEDIC][pwork_SET]) continue;
				if(PLAYER_TEMP[i][pt_WORKING_IN] != WORK_MEDIC) continue;

				SetPlayerMarkerForPlayer(i, playerid, 0x4aba00FF);
			}
		}
	}
	return 1;
}

DisablePlayerInjuredMark(playerid)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
			{
				if(i == playerid) continue;
				if(!PLAYER_WORKS[i][WORK_MEDIC][pwork_SET]) continue;
				if(PLAYER_TEMP[i][pt_WORKING_IN] != WORK_MEDIC) continue;

				SetPlayerMarkerForPlayer(i, playerid, PLAYER_COLOR);
			}
		}
	}
	return 1;
}

GetAvaibleCrewIndex()
{
	for(new i = 0; i != MAX_CREWS; i ++)
	{
		if(!CREW_INFO[i][crew_VALID]) return i;
	}
	return -1;
}


NewCrewRegister(index, playerid)
{
	if(!CREW_INFO[index][crew_VALID]) return 0;
	
	inline OnCrewInserted()
	{
		CREW_INFO[index][crew_ID] = cache_insert_id();
		if(CREW_INFO[index][crew_ID])
		{
			PI[playerid][ac_CREW] = CREW_INFO[index][crew_ID];
			PI[playerid][ac_CREW_RANK] = 0;
			PLAYER_TEMP[playerid][pt_CREW_INDEX] = index;
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET crew = %d, crew_rank = %d WHERE id = %d;", PI[playerid][ac_CREW], PI[playerid][ac_CREW_RANK], ACCOUNT_INFO[playerid][ac_ID]);
			mysql_tquery(srp_db, QUERY_BUFFER);
			RegisterNewCrewRank(index, 0);
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO crews (name, color) VALUES ('%e', %d);", CREW_INFO[index][crew_NAME], CREW_INFO[index][crew_COLOR]);
	MySQL_TQueryInline(srp_db, using inline OnCrewInserted, QUERY_BUFFER);
	return 1;
}

SendMessageToCrewMembers(crew_id, color, const message[])
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i) && PLAYER_TEMP[i][pt_USER_LOGGED])
		{
			if(PI[i][ac_CREW] == crew_id)
			{
				SendClientMessage(i, color, message);
			}
		}
	}
	return 1;
}

RegisterNewCrewRank(index, rank)
{
	inline OnCrewRankInserted()
	{
		CREW_RANK_INFO[index][rank][crew_rank_ID] = cache_insert_id();
	}
	mysql_format
	(
		srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, 
		"\
			INSERT INTO crew_ranks (id_crew, rank_pos, rank_name, permission0, permission1, permission2, permission3, permission4, permission5, permission6, permission7, permission8, permission9, permission10)\
			VALUES (%d, %d, '%e', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d);\
		",
			CREW_INFO[index][crew_ID], rank, CREW_RANK_INFO[index][rank][crew_rank_NAME], CREW_RANK_INFO[index][rank][crew_rank_PERMISSION][0], CREW_RANK_INFO[index][rank][crew_rank_PERMISSION][1],
			CREW_RANK_INFO[index][rank][crew_rank_PERMISSION][2], CREW_RANK_INFO[index][rank][crew_rank_PERMISSION][3], CREW_RANK_INFO[index][rank][crew_rank_PERMISSION][4],
			CREW_RANK_INFO[index][rank][crew_rank_PERMISSION][5], CREW_RANK_INFO[index][rank][crew_rank_PERMISSION][6], CREW_RANK_INFO[index][rank][crew_rank_PERMISSION][7],
			CREW_RANK_INFO[index][rank][crew_rank_PERMISSION][8], CREW_RANK_INFO[index][rank][crew_rank_PERMISSION][9], CREW_RANK_INFO[index][rank][crew_rank_PERMISSION][10]
	);
	MySQL_TQueryInline(srp_db, using inline OnCrewRankInserted, QUERY_BUFFER);
	return 1;
}

SetPlayerGangZones(playerid)
{
	for(new i = 0; i != MAX_TERRITORIES; i ++)
	{
		if(!TERRITORIES[i][territory_VALID]) continue;
		
		if(TERRITORIES[i][territory_WAR])
		{
			new flash_color, r, g, b, a;
			HexToRGBA(CREW_INFO[ TERRITORIES[i][territory_ATTACKER_CREW_INDEX] ][crew_COLOR], r, g, b, a);
			flash_color = RGBAToHex(r, g, b, 135);
			
			GangZoneFlashForPlayer(playerid, TERRITORIES[i][territory_GANG_ZONE], flash_color);
		}
		else GangZoneShowForPlayer(playerid, TERRITORIES[i][territory_GANG_ZONE], TERRITORIES[i][territory_COLOR]);
	}
	return 1;
}

HidePlayerGangZones(playerid)
{
	for(new i = 0; i != MAX_TERRITORIES; i ++)
	{
		if(!TERRITORIES[i][territory_VALID]) continue;
		
		GangZoneHideForPlayer(playerid, TERRITORIES[i][territory_GANG_ZONE]);
	}
	return 1;
}


UpdateGangZoneColor(index)
{
	if(TERRITORIES[index][territory_WAR])
	{
		new flash_color, r, g, b, a;
		HexToRGBA(CREW_INFO[ TERRITORIES[index][territory_ATTACKER_CREW_INDEX] ][crew_COLOR], r, g, b, a);
		flash_color = RGBAToHex(r, g, b, 135);
		
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(IsPlayerConnected(i))
			{
				if(PI[i][ac_CREW])
				{
					GangZoneFlashForPlayer(i, TERRITORIES[index][territory_GANG_ZONE], flash_color);
				}
			}
		}
	}
	else
	{
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(IsPlayerConnected(i))
			{
				if(PI[i][ac_CREW])
				{
					GangZoneShowForPlayer(i, TERRITORIES[index][territory_GANG_ZONE], TERRITORIES[index][territory_COLOR]);
				}
			}
		}
	}
	return 1;
}

StartTerritoryAttack(crew_index, territory_index, time)
{
	CREW_INFO[crew_index][crew_FIGHTING] = true;
	CREW_INFO[crew_index][crew_LAST_ATTACK] = gettime();
	TERRITORIES[territory_index][territory_WAR] = true;
	TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX] = crew_index;
	TERRITORIES[territory_index][territory_WAR_TIME_LEFT] = time;
	TERRITORIES[territory_index][territory_LAST_ATTACK] = gettime();
	UpdateGangZoneColor(territory_index);
	
	new message[145];
	format(message, sizeof message, "Conquista:_%s", TimeConvert(TERRITORIES[territory_index][territory_WAR_TIME_LEFT]));
	TextDrawSetString(TERRITORIES[territory_index][territory_TEXTDRAW], message);
	
	KillTimer(TERRITORIES[territory_index][territory_TIMER]);
	TERRITORIES[territory_index][territory_TIMER] = SetTimerEx("UpdateTerritoryAttack", 1000, false, "i", territory_index);
	
	if(TERRITORIES[territory_index][territory_OCCUPIED])
	{
		CREW_INFO[ TERRITORIES[territory_index][territory_CREW_INDEX] ][crew_FIGHTING] = true;
		format(message, sizeof message, "{"#PRIMARY_COLOR"}[BANDAS] {FFFFFF}La banda '%s' está atacando un territorio de la banda '%s' en %s.", CREW_INFO[crew_index][crew_NAME], CREW_INFO[ TERRITORIES[territory_index][territory_CREW_INDEX] ][crew_NAME], TERRITORIES[territory_index][territory_NAME]);
	}
	else
	{
		format(message, sizeof message, "{"#PRIMARY_COLOR"}[BANDAS] {FFFFFF}La banda '%s' está atacando un territorio en %s.", CREW_INFO[crew_index][crew_NAME], TERRITORIES[territory_index][territory_NAME]);
	}

	new message_police[145];
	format(message_police, sizeof message_police, "{"#PRIMARY_COLOR"}[Central policía] {FFFFFF}La banda '%s' está atacando un territorio en %s.", CREW_INFO[crew_index][crew_NAME], TERRITORIES[territory_index][territory_NAME]);
	
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(PI[i][ac_CREW])
			{
				SendClientMessage(i, -1, message);
				if(IsPlayerInDynamicArea(i, TERRITORIES[territory_index][territory_AREA]))
				{	
					if(PI[i][ac_WANTED_LEVEL] < 3)
						SetPlayerWantedLevelEx(i, 3);

					new r, g, b, a;
					HexToRGBA(CREW_INFO[ PLAYER_TEMP[i][pt_CREW_INDEX] ][crew_COLOR], r, g, b, a);
					SetPlayerColorEx(i, RGBAToHex(r, g, b, 0));
					
					TextDrawShowForPlayer(i, Textdraws[textdraw_TERRITORY_BOX]);
					TextDrawShowForPlayer(i, TERRITORIES[territory_index][territory_TEXTDRAW]);
				}
			}
			
			if(PLAYER_WORKS[i][WORK_POLICE][pwork_SET])
			{
				if(PLAYER_TEMP[i][pt_WORKING_IN] == WORK_POLICE)
				{
					SendClientMessage(i, -1, message_police);
				}
			}
		}
	}
	return 1;
}

CountCrewPlayersInTerritory(crew_index, territory_index)
{
	new count;
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(PI[i][ac_CREW])
			{
				if(PLAYER_TEMP[i][pt_CREW_INDEX] == crew_index)
				{
					if(ACCOUNT_INFO[i][ac_STATE] == ROLEPLAY_STATE_NORMAL && PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
					{
						if(IsPlayerInDynamicArea(i, TERRITORIES[territory_index][territory_AREA]))
						{
							count ++;
						}
					}
				}
			}
		}
	}
	return count;
}

SetPlayerColorEx(playerid, color)
{
	PLAYER_TEMP[playerid][pt_PLAYER_COLOR] = color;
	return SetPlayerColor(playerid, color);
}
	
LoadServerInfo()
{
	new File:ECO = fopen("SERVER/ECO.txt", io_read), eco_str[256];
	if(ECO)
	{
		fread(ECO, eco_str);
		sscanf(eco_str, "p<,>fffffddd", MULTIPLIER_PROPERITES, MULTIPLIER_VEHICLES, MULTIPLIER_TOYS, MULTIPLIER_SKINS, MULTIPLIER_WEAPONS, SU_SD_PRICE, SU_WORK_EXTRA_PAY, CHANGE_NAME_SD_PRICE);
		fclose(ECO);
	}
	
	
	new File:AC = fopen("SERVER/AC.txt", io_read), ac_str[128], ac_num;
	if(AC)
	{
		while(fread(AC, ac_str))
		{
			if(ac_num >= sizeof(ac_Info)) break;
			sscanf(ac_str, "p<,>bbdd", ac_Info[ac_num][ac_Enabled], ac_Info[ac_num][ac_Kick], ac_Info[ac_num][ac_Detections], ac_Info[ac_num][ac_Interval]);
			ac_num ++;
		}
		fclose(AC);
	}
	return 1;
}

WriteAnticheatRules()
{
	new File:AC = fopen("SERVER/AC.txt", io_write), ac_str[128];
	if(AC)
	{
		for(new i = 0; i != sizeof(ac_Info); i ++)
		{
			format(ac_str, sizeof ac_str, "%d,%d,%d,%d\r\n", ac_Info[i][ac_Enabled], ac_Info[i][ac_Kick], ac_Info[i][ac_Detections], ac_Info[i][ac_Interval]);
			fwrite(AC, ac_str);
		}
		fclose(AC);
	}
	return 1;
}

WriteEconomyMultipliers()
{
	new File:ECO = fopen("SERVER/ECO.txt", io_write), eco_str[256];
	if(ECO)
	{
		format(eco_str, sizeof eco_str, "%f,%f,%f,%f,%f,%d,%d,%d", MULTIPLIER_PROPERITES, MULTIPLIER_VEHICLES, MULTIPLIER_TOYS, MULTIPLIER_SKINS, MULTIPLIER_WEAPONS, SU_SD_PRICE, SU_WORK_EXTRA_PAY, CHANGE_NAME_SD_PRICE);
		fwrite(ECO, eco_str);
		fclose(ECO);
	}
	return 1;
}

UpdateEconomy(type = -1)
{
	new label_str[256];
	
	if(type == -1)
	{
		for(new i = 0; i != MAX_PROPERTIES; i ++)
		{
			if(!PROPERTY_INFO[i][property_VALID]) continue;
			PROPERTY_INFO[i][property_PRICE] = floatround(PROPERTY_INFO[i][property_PRICE_BASE] * MULTIPLIER_PROPERITES, floatround_ceil);
			
			if(!PROPERTY_INFO[i][property_SOLD])
			{
				UpdateUnnocupiedPropertyLabel(i);
			}
		}
		
		for(new i = 0, j = GetVehiclePoolSize(); i <= j; i ++)
		{
			if(!SELL_VEHICLES[i][sell_vehicle_VALID]) continue;
			SELL_VEHICLES[i][sell_vehicle_PRICE] = floatround(VEHICLE_INFO[ GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400 ][vehicle_info_PRICE] * MULTIPLIER_VEHICLES, floatround_ceil);		
		
			if(SELL_VEHICLES[i][sell_vehicle_VIP_LEVEL]) SELL_VEHICLES[i][sell_vehicle_LEVEL] = 1;
			if(SELL_VEHICLES[i][sell_vehicle_EXTRA]) SELL_VEHICLES[i][sell_vehicle_PRICE] = 0;
			
			if(SELL_VEHICLES[i][sell_vehicle_VIP_LEVEL])
			{
				if(SELL_VEHICLES[i][sell_vehicle_EXTRA])
				{
					format
					(
						label_str, 
							sizeof label_str, 
							"\
								{"#PRIMARY_COLOR"}Membresía VIP requerida\n\
								\n\
								{"#PRIMARY_COLOR"}%s\n\n\
								{FFFFFF}Coste: {"#PRIMARY_COLOR"}%d "SERVER_COIN"\n\
								{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\
							", VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_NAME], SELL_VEHICLES[i][sell_vehicle_EXTRA], SELL_VEHICLES[i][sell_vehicle_LEVEL]
					);
				}
				else
				{
					format
					(
						label_str, 
							sizeof label_str, 
							"\
								{"#PRIMARY_COLOR"}Membresía VIP requerida\n\
								\n\
								{"#PRIMARY_COLOR"}%s\n\n\
								{FFFFFF}Precio: {"#PRIMARY_COLOR"}%s$\n\
								{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\
							", VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_NAME], number_format_thousand(SELL_VEHICLES[i][sell_vehicle_PRICE]), SELL_VEHICLES[i][sell_vehicle_LEVEL]
					);
				}
			}
			else
			{
				if(SELL_VEHICLES[i][sell_vehicle_EXTRA])
				{
					format
					(
						label_str, 
							sizeof label_str, 
							"\
								{"#PRIMARY_COLOR"}%s\n\n\
								{FFFFFF}Coste: {"#PRIMARY_COLOR"}%d "SERVER_COIN"\n\
								{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\
							", VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_NAME], SELL_VEHICLES[i][sell_vehicle_EXTRA], SELL_VEHICLES[i][sell_vehicle_LEVEL]
					);
				}
				else
				{
					format
					(
						label_str, 
							sizeof label_str, 
							"\
								{"#PRIMARY_COLOR"}%s\n\n\
								{FFFFFF}Precio: {"#PRIMARY_COLOR"}%s$\n\
								{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\
							", VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_NAME], number_format_thousand(SELL_VEHICLES[i][sell_vehicle_PRICE]), SELL_VEHICLES[i][sell_vehicle_LEVEL]
					);
				}
			}
			UpdateDynamic3DTextLabelText(SELL_VEHICLES[i][sell_vehicle_LABEL_ID], 0xFFFFFFFF, label_str);
		}
		
		for(new i = 0; i != sizeof(Toys_Shop); i ++) Toys_Shop[i][shop_toy_price] = floatround(Toys_Shop[i][shop_toy_price_base] * MULTIPLIER_TOYS, floatround_ceil);		
		for(new i = 0; i != sizeof(Binco_Shop_Male_Skins); i ++) Binco_Shop_Male_Skins[i][2] = floatround(Binco_Shop_Male_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
		for(new i = 0; i != sizeof(Binco_Shop_Female_Skins); i ++) Binco_Shop_Female_Skins[i][2] = floatround(Binco_Shop_Female_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
		for(new i = 0; i != sizeof(Suburban_Shop_Male_Skins); i ++) Suburban_Shop_Male_Skins[i][2] = floatround(Suburban_Shop_Male_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
		for(new i = 0; i != sizeof(Suburban_Shop_Female_Skins); i ++) Suburban_Shop_Female_Skins[i][2] = floatround(Suburban_Shop_Female_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
		for(new i = 0; i != sizeof(Prolaps_Shop_Male_Skins); i ++) Prolaps_Shop_Male_Skins[i][2] = floatround(Prolaps_Shop_Male_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
		for(new i = 0; i != sizeof(Prolaps_Shop_Female_Skins); i ++) Prolaps_Shop_Female_Skins[i][2] = floatround(Prolaps_Shop_Female_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
		for(new i = 0; i != sizeof(Didier_Shop_Male_Skins); i ++) Didier_Shop_Male_Skins[i][2] = floatround(Didier_Shop_Male_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
		for(new i = 0; i != sizeof(Didier_Shop_Female_Skins); i ++) Didier_Shop_Female_Skins[i][2] = floatround(Didier_Shop_Female_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
		for(new i = 0; i != sizeof(Victim_Shop_Male_Skins); i ++) Victim_Shop_Male_Skins[i][2] = floatround(Victim_Shop_Male_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
		for(new i = 0; i != sizeof(Victim_Shop_Female_Skins); i ++) Victim_Shop_Female_Skins[i][2] = floatround(Victim_Shop_Female_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
		for(new i = 0; i != sizeof(Zip_Shop_Male_Skins); i ++) Zip_Shop_Male_Skins[i][2] = floatround(Zip_Shop_Male_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
		for(new i = 0; i != sizeof(Zip_Shop_Female_Skins); i ++) Zip_Shop_Female_Skins[i][2] = floatround(Zip_Shop_Female_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
		
		for(new i = 0; i != sizeof(BLACK_MARKT_WEAPONS); i ++) BLACK_MARKT_WEAPONS[i][black_market_WEAPON_PRICE] = floatround(BLACK_MARKT_WEAPONS[i][black_market_WEAPON_PRICE_BASE] * MULTIPLIER_WEAPONS, floatround_ceil);
	}
	else
	{
		switch(type)
		{
			case 0:
			{
				for(new i = 0; i != MAX_PROPERTIES; i ++)
				{
					if(!PROPERTY_INFO[i][property_VALID]) continue;
					PROPERTY_INFO[i][property_PRICE] = floatround(PROPERTY_INFO[i][property_PRICE_BASE] * MULTIPLIER_PROPERITES, floatround_ceil);
					
					if(!PROPERTY_INFO[i][property_SOLD])
					{
						UpdateUnnocupiedPropertyLabel(i);
					}
				}
			}
			case 1:
			{
				for(new i = 0, j = GetVehiclePoolSize(); i <= j; i ++)
				{
					if(!SELL_VEHICLES[i][sell_vehicle_VALID]) continue;
					SELL_VEHICLES[i][sell_vehicle_PRICE] = floatround(VEHICLE_INFO[ GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400 ][vehicle_info_PRICE] * MULTIPLIER_VEHICLES, floatround_ceil);		
					
					if(SELL_VEHICLES[i][sell_vehicle_VIP_LEVEL]) SELL_VEHICLES[i][sell_vehicle_LEVEL] = 1;
					if(SELL_VEHICLES[i][sell_vehicle_EXTRA]) SELL_VEHICLES[i][sell_vehicle_PRICE] = 0;
					
					if(SELL_VEHICLES[i][sell_vehicle_VIP_LEVEL])
					{
						if(SELL_VEHICLES[i][sell_vehicle_EXTRA])
						{
							format
							(
								label_str, 
									sizeof label_str, 
									"\
										{"#PRIMARY_COLOR"}Membresía VIP requerida\n\
										\n\
										{"#PRIMARY_COLOR"}%s\n\n\
										{FFFFFF}Coste: {"#PRIMARY_COLOR"}%d "SERVER_COIN"\n\
										{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\
									", VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_NAME], SELL_VEHICLES[i][sell_vehicle_EXTRA], SELL_VEHICLES[i][sell_vehicle_LEVEL]
							);
						}
						else
						{
							format
							(
								label_str, 
									sizeof label_str, 
									"\
										{"#PRIMARY_COLOR"}Membresía VIP requerida\n\
										\n\
										{"#PRIMARY_COLOR"}%s\n\n\
										{FFFFFF}Precio: {"#PRIMARY_COLOR"}%s$\n\
										{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\
									", VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_NAME], number_format_thousand(SELL_VEHICLES[i][sell_vehicle_PRICE]), SELL_VEHICLES[i][sell_vehicle_LEVEL]
							);
						}
					}
					else
					{
						if(SELL_VEHICLES[i][sell_vehicle_EXTRA])
						{
							format
							(
								label_str, 
									sizeof label_str, 
									"\
										{"#PRIMARY_COLOR"}%s\n\n\
										{FFFFFF}Coste: {"#PRIMARY_COLOR"}%d "SERVER_COIN"\n\
										{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\
									", VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_NAME], SELL_VEHICLES[i][sell_vehicle_EXTRA], SELL_VEHICLES[i][sell_vehicle_LEVEL]
							);
						}
						else
						{
							format
							(
								label_str, 
									sizeof label_str, 
									"\
										{"#PRIMARY_COLOR"}%s\n\n\
										{FFFFFF}Precio: {"#PRIMARY_COLOR"}%s$\n\
										{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\
									", VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_NAME], number_format_thousand(SELL_VEHICLES[i][sell_vehicle_PRICE]), SELL_VEHICLES[i][sell_vehicle_LEVEL]
							);
						}
					}
					UpdateDynamic3DTextLabelText(SELL_VEHICLES[i][sell_vehicle_LABEL_ID], 0xFFFFFFFF, label_str);
				}
			}
			case 2:
			{
				for(new i = 0; i != sizeof(Toys_Shop); i ++) Toys_Shop[i][shop_toy_price] = floatround(Toys_Shop[i][shop_toy_price_base] * MULTIPLIER_TOYS, floatround_ceil);		
			}
			case 3:
			{
				for(new i = 0; i != sizeof(Binco_Shop_Male_Skins); i ++) Binco_Shop_Male_Skins[i][2] = floatround(Binco_Shop_Male_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
				for(new i = 0; i != sizeof(Binco_Shop_Female_Skins); i ++) Binco_Shop_Female_Skins[i][2] = floatround(Binco_Shop_Female_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
				for(new i = 0; i != sizeof(Suburban_Shop_Male_Skins); i ++) Suburban_Shop_Male_Skins[i][2] = floatround(Suburban_Shop_Male_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
				for(new i = 0; i != sizeof(Suburban_Shop_Female_Skins); i ++) Suburban_Shop_Female_Skins[i][2] = floatround(Suburban_Shop_Female_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
				for(new i = 0; i != sizeof(Prolaps_Shop_Male_Skins); i ++) Prolaps_Shop_Male_Skins[i][2] = floatround(Prolaps_Shop_Male_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
				for(new i = 0; i != sizeof(Prolaps_Shop_Female_Skins); i ++) Prolaps_Shop_Female_Skins[i][2] = floatround(Prolaps_Shop_Female_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
				for(new i = 0; i != sizeof(Didier_Shop_Male_Skins); i ++) Didier_Shop_Male_Skins[i][2] = floatround(Didier_Shop_Male_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
				for(new i = 0; i != sizeof(Didier_Shop_Female_Skins); i ++) Didier_Shop_Female_Skins[i][2] = floatround(Didier_Shop_Female_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
				for(new i = 0; i != sizeof(Victim_Shop_Male_Skins); i ++) Victim_Shop_Male_Skins[i][2] = floatround(Victim_Shop_Male_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
				for(new i = 0; i != sizeof(Victim_Shop_Female_Skins); i ++) Victim_Shop_Female_Skins[i][2] = floatround(Victim_Shop_Female_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
				for(new i = 0; i != sizeof(Zip_Shop_Male_Skins); i ++) Zip_Shop_Male_Skins[i][2] = floatround(Zip_Shop_Male_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
				for(new i = 0; i != sizeof(Zip_Shop_Female_Skins); i ++) Zip_Shop_Female_Skins[i][2] = floatround(Zip_Shop_Female_Skins[i][1] * MULTIPLIER_SKINS, floatround_ceil);
			}
			case 4:
			{
				for(new i = 0; i != sizeof(BLACK_MARKT_WEAPONS); i ++) BLACK_MARKT_WEAPONS[i][black_market_WEAPON_PRICE] = floatround(BLACK_MARKT_WEAPONS[i][black_market_WEAPON_PRICE_BASE] * MULTIPLIER_WEAPONS, floatround_ceil);
			}
		}
	}
	return 1;
}

ReLockPlayerVehicles(playerid, bool:remove = false)
{
	if(ACCOUNT_INFO[playerid][ac_ID] == 0) return 0;
	if(ACCOUNT_INFO[playerid][ac_VIP]) return 0;
	
	inline OnQueryLoadedInline()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			for(new i = 0; i != rows; i ++)
			{
				if(i >= MAX_SU_VEHICLES)
				{
					printf("---> MAX_SU_VEHICLES superado al cargar de base de datos.");
					break;
				}

				new veh_did;
				cache_get_value_index_int(i, 0, veh_did);

				//
				new vehicle_id = GetPlayerVehicleIdFromDbId(veh_did);
				if(vehicle_id != INVALID_VEHICLE_ID)
				{
					PLAYER_VEHICLES[vehicle_id][player_vehicle_ACCESSIBLE] = true;
					if(i >= MAX_NU_VEHICLES)
					{
						if(remove)
						{
							if(GLOBAL_VEHICLES[vehicle_id][gb_vehicle_OCCUPIED])
							{
								SetVehicleVelocity(vehicle_id, 0.0, 0.0, 0.0);
								if(GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DRIVER] != INVALID_PLAYER_ID) RemovePlayerFromVehicle(playerid);
							}
						}
						GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PARAMS_ENGINE] = 0;
						GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PARAMS_LIGHTS] = 0;
						GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PARAMS_DOORS] = 1;
						PLAYER_VEHICLES[vehicle_id][player_vehicle_ACCESSIBLE] = false;
						UpdateVehicleParams(vehicle_id);
					}
				}
			}
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM pvehicles WHERE id_player = %d ORDER BY id DESC LIMIT %d;", ACCOUNT_INFO[playerid][ac_ID], MAX_SU_VEHICLES);
	MySQL_TQueryInline(srp_db, using inline OnQueryLoadedInline, QUERY_BUFFER);
	return 1;
}

UnlockPlayerVehicles(playerid)
{
	new total;
	for(new i = 0, j = GetVehiclePoolSize(); i <= j; i ++)
	{
		if(!GLOBAL_VEHICLES[i][gb_vehicle_VALID]) continue;
		if(!PLAYER_VEHICLES[i][player_vehicle_VALID]) continue;
		
		if(PLAYER_VEHICLES[i][player_vehicle_ID] == ACCOUNT_INFO[playerid][ac_ID])
		{
			if(total >= MAX_SU_VEHICLES) break;
			
			PLAYER_VEHICLES[i][player_vehicle_ACCESSIBLE] = true;
		}
	}
	return total;
}

GetPlayerVehicleIdFromDbId(veh_did)
{
	for(new i = 0, j = GetVehiclePoolSize(); i <= j; i ++)
	{
		if(!GLOBAL_VEHICLES[i][gb_vehicle_VALID]) continue;
		if(!PLAYER_VEHICLES[i][player_vehicle_VALID]) continue;
		
		if(PLAYER_VEHICLES[i][player_vehicle_ID] == veh_did) return i;
	}
	return INVALID_VEHICLE_ID;
}

stock GetPlayerCameraLookAt(playerid, &Float:X, &Float:Y, &Float:Z)
{
    new Float:pos[6];
    GetPlayerCameraPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerCameraFrontVector(playerid, pos[3], pos[4], pos[5]);
    X = floatadd(pos[0], pos[3]);
    Y = floatadd(pos[1], pos[4]);
    Z = floatadd(pos[2], pos[5]);
    return 1;
}

UpdateTuningShop(playerid)
{
	if(PLAYER_TEMP[playerid][pt_TUNING_PICKUP] != INVALID_STREAMER_ID)
	{
		DestroyDynamicPickup(PLAYER_TEMP[playerid][pt_TUNING_PICKUP]);
		PLAYER_TEMP[playerid][pt_TUNING_PICKUP] = INVALID_STREAMER_ID;
	}
	PLAYER_TEMP[playerid][pt_TUNING_PICKUP] = CreateDynamicPickup(Tuning_Shop_Objects[ PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ][tuning_object_MODELID], 1, 268.2404, 11.4559, 2.4222, playerid + MAX_PLAYERS, -1, playerid);
	
	new td_str[128];
	format(td_str, sizeof td_str, "Objeto:_%d/%d~n~Precio:_%s$", PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] + 1, sizeof Tuning_Shop_Objects, number_format_thousand( Tuning_Shop_Objects[ PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] ][tuning_object_PRICE] ) );
	PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_TUNING_SHOP], td_str);
	return 1;
}

CancelPlayerTuningShop(playerid)
{	
	SetCameraBehindPlayer(playerid);
	
	if(PLAYER_TEMP[playerid][pt_TUNING_PICKUP] != INVALID_STREAMER_ID)
	{
		DestroyDynamicPickup(PLAYER_TEMP[playerid][pt_TUNING_PICKUP]);
		PLAYER_TEMP[playerid][pt_TUNING_PICKUP] = INVALID_STREAMER_ID;
	}
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP_TUNING][0]);
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP_TUNING][1]);
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP_TUNING][2]);
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP_TUNING][3]);
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_SHOP_TUNING][4]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TUNING_SHOP]);
	CancelSelectTextDrawEx(playerid);
	
	PLAYER_TEMP[playerid][pt_TUNING_GARAGE_SHOP] = false;
	return 1;
}

GetVehicleFreeObjectSlot(vehicleid)
{
	for(new i = 0; i != MAX_SU_VOBJECTS; i ++)
	{
		if(!VEHICLE_OBJECTS[vehicleid][i][vobject_VALID]) return i;
	}
	return -1;
}

UpdateVehicleAttachedObject(vehicleid, slot, material = false)
{
	if(VEHICLE_OBJECTS[vehicleid][slot][vobject_ATTACHED])
	{
		if(VEHICLE_OBJECTS[vehicleid][slot][vobject_OBJECT_ID] == INVALID_STREAMER_ID)
		{
			VEHICLE_OBJECTS[vehicleid][slot][vobject_OBJECT_ID] = 	CreateDynamicObject
																	(
																		VEHICLE_OBJECTS[vehicleid][slot][vobject_MODELID],
																		0.0,
																		0.0,
																		0.0,
																		0.0,
																		0.0,
																		0.0
																	);
		}
		
		if(material)
		{
			switch(VEHICLE_OBJECTS[vehicleid][slot][vobject_TYPE])
			{
				case VOBJECT_TYPE_OBJECT:
				{
					for(new i = 0; i != MAX_VEHICLE_OBJECTS_INDEXS; i ++)
					{
						SetDynamicObjectMaterial(VEHICLE_OBJECTS[vehicleid][slot][vobject_OBJECT_ID], i, -1, "none", "none", VEHICLE_OBJECTS[vehicleid][slot][vobject_COLORS][i]);
					}
				}
				case VOBJECT_TYPE_TEXT:
				{
					SetDynamicObjectMaterialText
					(
						VEHICLE_OBJECTS[vehicleid][slot][vobject_OBJECT_ID],
						0,
						VEHICLE_OBJECTS[vehicleid][slot][vobject_text_TEXT],
						130,
						VEHICLE_OBJECTS[vehicleid][slot][vobject_text_FONT],
						VEHICLE_OBJECTS[vehicleid][slot][vobject_text_FONT_SIZE],
						VEHICLE_OBJECTS[vehicleid][slot][vobject_text_BOLD],
						VEHICLE_OBJECTS[vehicleid][slot][vobject_text_FONT_COLOR],
						0,
						OBJECT_MATERIAL_TEXT_ALIGN_CENTER
					);
				}
				case VOBJECT_TYPE_COUNTRY_FLAG: TextureCountryFlag(VEHICLE_OBJECTS[vehicleid][slot][vobject_OBJECT_ID], 1, VEHICLE_OBJECTS[vehicleid][slot][vobject_COLORS][0]);
				case VOBJECT_TYPE_COUNTRY_PANEL: TextureCountryFlag(VEHICLE_OBJECTS[vehicleid][slot][vobject_OBJECT_ID], 0, VEHICLE_OBJECTS[vehicleid][slot][vobject_COLORS][0]);
			}
		}
		
		AttachDynamicObjectToVehicle
		(
			VEHICLE_OBJECTS[vehicleid][slot][vobject_OBJECT_ID],
			vehicleid,
			VEHICLE_OBJECTS[vehicleid][slot][vobject_OFFSET][0],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_OFFSET][1],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_OFFSET][2],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_ROT][0],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_ROT][1],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_ROT][2]
		);
	}
	else
	{
		if(VEHICLE_OBJECTS[vehicleid][slot][vobject_OBJECT_ID] != INVALID_STREAMER_ID)
		{
			DestroyDynamicObject(VEHICLE_OBJECTS[vehicleid][slot][vobject_OBJECT_ID]);
			VEHICLE_OBJECTS[vehicleid][slot][vobject_OBJECT_ID] = INVALID_STREAMER_ID;
		}
	}
	return 1;
}

EditVehicleObject(playerid, vehicleid, slot)
{
	if(PLAYER_TEMP[playerid][pt_PIVOT_OBJECT] != INVALID_OBJECT_ID)
	{
		DestroyPlayerObject(playerid, PLAYER_TEMP[playerid][pt_PIVOT_OBJECT]);
		PLAYER_TEMP[playerid][pt_PIVOT_OBJECT] = INVALID_OBJECT_ID;
	}
	
	PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] = slot;
	GetVehiclePos(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][0], GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][1], GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][2]);
	
	new Float:pos[3];
	pos[0] = GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][0] + VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][0];
	pos[1] = GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][1] + VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][1];
	pos[2] = GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][2] + VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][2];
	
	PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][0] = VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][0];
	PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][1] = VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][1];
	PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][2] = VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][2];
	PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][3] = VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][0];
	PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][4] = VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][1];
	PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][5] = VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][2];
	
	PLAYER_TEMP[playerid][pt_PIVOT_OBJECT] = CreatePlayerObject(playerid, 999, pos[0], pos[1], pos[2], VEHICLE_OBJECTS[vehicleid][slot][vobject_ROT][0], VEHICLE_OBJECTS[vehicleid][slot][vobject_ROT][1], VEHICLE_OBJECTS[vehicleid][slot][vobject_ROT][2]);
	SetPlayerObjectMaterial(playerid, PLAYER_TEMP[playerid][pt_PIVOT_OBJECT], 0, 0, "null", "null");
	SetPlayerObjectMaterial(playerid, PLAYER_TEMP[playerid][pt_PIVOT_OBJECT], 1, 0, "null", "null");
	
	Streamer_Update(playerid);
	EditPlayerObject(playerid, PLAYER_TEMP[playerid][pt_PIVOT_OBJECT]);
	return 1;
}

SetVehicleVirtualWorldEx(vehicleid, worldid)
{
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_TP_IMMUNITY] = gettime() + 5;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_WORLD] = worldid;
	
	for(new i = 0; i != MAX_SU_VOBJECTS; i ++)
	{
		if(!VEHICLE_OBJECTS[vehicleid][i][vobject_VALID]) continue;
		if(VEHICLE_OBJECTS[vehicleid][i][vobject_OBJECT_ID] == INVALID_STREAMER_ID) continue;
		
		Streamer_SetIntData(STREAMER_TYPE_OBJECT, VEHICLE_OBJECTS[vehicleid][i][vobject_OBJECT_ID], E_STREAMER_WORLD_ID, GLOBAL_VEHICLES[vehicleid][gb_vehicle_WORLD]);
	}
	
	SetVehicleVirtualWorld(vehicleid, worldid);
	return 1;
}

TextureCountryFlag(objectid, index, country)
{
	new text_flag[128], material_size, font[24], font_size, text_alignment;
	switch(country)
	{
		case 0: // Alemania
		{
			text_flag		= "{231f20}ggg\n{e01f26}ggg\n{e3bc22}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 1: // Argentina
		{
			text_flag		= "{1FBEF2}ggg\n{ffffff}ggg\n{1FBEF2}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 2: // Austria
		{
			text_flag		= "{d81e05}ggg\n{ffffff}ggg\n{d81e05}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 3: // Belgica
		{
			text_flag		= "{000000}g{ffd600}g{e80000}g\n{000000}g{ffd600}g{e80000}g\n{000000}g{ffd600}g{e80000}g";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 4: // Bolivia
		{
			text_flag		= "{d52b1e}ggg\n{f9e300}ggg\n{007934}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 5: // Bulgaria
		{
			text_flag		= "{ffffff}ggg\n{00966e}ggg\n{d62612}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 6: // Chile
		{
			text_flag		= "{04019a}g{ffffff}gg\n{fe0002}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 7: // Colombia
		{
			text_flag		= "{fcd116}ggg\n{003893}ggg\n{ce1126}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 0;
		}
		case 8: // El Salvador
		{
			text_flag		= "{0f47af}ggg\n{ffffff}ggg\n{0f47af}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 9: // España
		{
			text_flag		= "{c60b1e}ggg\n{ffc400}ggg\n{c60b1e}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 10: // Estonia
		{
			text_flag		= "{3a75c4}ggg\n{000000}ggg\n{ffffff}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 11: // Francia
		{
			text_flag		= "{00209f}g{ffffff}g{f42a41}g\n{00209f}g{ffffff}g{f42a41}g\n{00209f}g{ffffff}g{f42a41}g";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 12: // Guatemala
		{
			text_flag		= "{4997d0}g{ffffff}g{4997d0}g\n{4997d0}g{ffffff}g{4997d0}g\n{4997d0}g{ffffff}g{4997d0}g";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 13: // Holanda
		{
			text_flag		= "{ad1b28}ggg\n{fefefe}ggg\n{204589}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 14: // Hungria
		{
			text_flag		= "{d2232c}ggg\n{ffffff}ggg\n{008d46}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 15: // India
		{
			text_flag		= "{ff9933}ggg\n{ffffff}ggg\n{138808}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 16: // Indonesia
		{
			text_flag		= "{ce1126}ggg\n{ffffff}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 60;
			text_alignment	= 1;
		}
		case 17: // Irlanda
		{
			text_flag		= "{009e60}g{ffffff}g{f77f00}g\n{009e60}g{ffffff}g{f77f00}g\n{009e60}g{ffffff}g{f77f00}g";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 18: // Italia
		{
			text_flag		= "{009246}g{ffffff}g{ce2b37}g\n{009246}g{ffffff}g{ce2b37}g\n{009246}g{ffffff}g{ce2b37}g";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 19: // Lituania
		{
			text_flag		= "{fdb913}ggg\n{006a44}ggg\n{c1272d}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 20: // Luxemburgo
		{
			text_flag		= "{ef2b2d}ggg\n{ffffff}ggg\n{00a3dd}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 21: // Madagascar
		{
			text_flag		= "{ffffff}g{d11f33}gg\n{ffffff}g{00a651}gg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 60;
			text_alignment	= 1;
		}
		case 22: // Malta
		{
			text_flag		= "{ffffff}g{d11f33}g\n{ffffff}g{d11f33}g\n{ffffff}g{d11f33}g";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 60;
			text_alignment	= 1;
		}
		case 23: // Mali
		{
			text_flag		= "{14b53a}g{fcd116}g{ce1126}g\n{14b53a}g{fcd116}g{ce1126}g\n{14b53a}g{fcd116}g{ce1126}g";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 24: // México
		{
			text_flag		= "{006847}g{ffffff}g{ce1126}g\n{006847}g{ffffff}g{ce1126}g\n{006847}g{ffffff}g{ce1126}g";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 25: // Nigeria
		{
			text_flag		= "{008751}g{ffffff}g{008751}g\n{008751}g{ffffff}g{008751}g\n{008751}g{ffffff}g{008751}g";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 26: // Panamá
		{
			text_flag		= "{ffffff}g{d21034}g\n{0067c6}g{ffffff}g";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 60;
			text_alignment	= 0;
		}
		case 27: // Paraguay
		{
			text_flag		= "{d52b1e}ggg\n{ffffff}ggg\n{0038a8}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 28: // Perú
		{
			text_flag		= "{e30000}g{ffffff}g{e30000}g\n{e30000}g{ffffff}g{e30000}g\n{e30000}g{ffffff}g{e30000}g";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 29: // Polonia
		{
			text_flag		= "{ffffff}ggg\n{dc143c}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 60;
			text_alignment	= 1;
		}
		case 30: // Rumania
		{
			text_flag		= "{002b7f}g{fcd116}g{ce1126}g\n{002b7f}g{fcd116}g{ce1126}g\n{002b7f}g{fcd116}g{ce1126}g";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 31: // Rusia
		{
			text_flag		= "{ffffff}ggg\n{054da2}ggg\n{e01f26}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
		case 32: // Ucrania
		{
			text_flag		= "{3a75c4}ggg\n{f9dd16}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 60;
			text_alignment	= 1;
		}
		case 33: // Venezuela
		{
			text_flag		= "{f7d117}ggg\n{0033ab}ggg\n{cf142b}ggg";
			material_size	= 60;
			font			= "Webdings";
			font_size		= 55;
			text_alignment	= 1;
		}
	}
	
	SetDynamicObjectMaterialText
	(
		objectid,
		index,
		text_flag,
		material_size,
		font,
		font_size,
		0, 
		0xFFFFFFFF,
		0,
		text_alignment
	);
	return 1;
}

RegisterNewVehicleObject(vehicleid, slot)
{
	inline OnVobjectInserted()
	{
		VEHICLE_OBJECTS[vehicleid][slot][vobject_ID] = cache_insert_id();
	}

	mysql_format
	(
		srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER,
		"\
			INSERT INTO vobjects (id_vehicle, type, name, modelid, off_x, off_y, off_z, rot_x, rot_y, rot_z, attached, color0, color1, color2, color3, color4, text, font, fontsize, fontbold, fontcolor)\
			VALUES(%d, %d, '%e', %d, %f, %f, %f, %f, %f, %f, %d, %d, %d, %d, %d, %d, '%e', '%e', %d, %d, %d);\
		",
			PLAYER_VEHICLES[vehicleid][player_vehicle_ID], VEHICLE_OBJECTS[vehicleid][slot][vobject_TYPE], VEHICLE_OBJECTS[vehicleid][slot][vobject_NAME], VEHICLE_OBJECTS[vehicleid][slot][vobject_MODELID],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_OFFSET][0], VEHICLE_OBJECTS[vehicleid][slot][vobject_OFFSET][1], VEHICLE_OBJECTS[vehicleid][slot][vobject_OFFSET][2],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_ROT][0], VEHICLE_OBJECTS[vehicleid][slot][vobject_ROT][1], VEHICLE_OBJECTS[vehicleid][slot][vobject_ROT][2],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_ATTACHED], VEHICLE_OBJECTS[vehicleid][slot][vobject_COLORS][0], VEHICLE_OBJECTS[vehicleid][slot][vobject_COLORS][1], VEHICLE_OBJECTS[vehicleid][slot][vobject_COLORS][2],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_COLORS][3], VEHICLE_OBJECTS[vehicleid][slot][vobject_COLORS][4], VEHICLE_OBJECTS[vehicleid][slot][vobject_text_TEXT], VEHICLE_OBJECTS[vehicleid][slot][vobject_text_FONT],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_text_FONT_SIZE], VEHICLE_OBJECTS[vehicleid][slot][vobject_text_BOLD], VEHICLE_OBJECTS[vehicleid][slot][vobject_text_FONT_COLOR]
	);
	MySQL_TQueryInline(srp_db, using inline OnVobjectInserted, QUERY_BUFFER);
	return 1;
}

UpdateVehicleObject(vehicleid, slot)
{
	mysql_format
	(
		srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER,
		"\
			UPDATE vobjects SET \
				id_vehicle = %d,\
				type = %d,\
				name = '%e',\
				modelid = %d,\
				off_x = %f,\
				off_y = %f,\
				off_z = %f,\
				rot_x = %f,\
				rot_y = %f,\
				rot_z = %f,\
				attached = %d,\
				color0 = %d,\
				color1 = %d,\
				color2 = %d,\
				color3 = %d,\
				color4 = %d,\
				text = '%e',\
				font = '%e',\
				fontsize = %d,\
				fontbold = %d,\
				fontcolor = %d \
			WHERE id = %d;\
		",
			PLAYER_VEHICLES[vehicleid][player_vehicle_ID], VEHICLE_OBJECTS[vehicleid][slot][vobject_TYPE], VEHICLE_OBJECTS[vehicleid][slot][vobject_NAME], VEHICLE_OBJECTS[vehicleid][slot][vobject_MODELID],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_OFFSET][0], VEHICLE_OBJECTS[vehicleid][slot][vobject_OFFSET][1], VEHICLE_OBJECTS[vehicleid][slot][vobject_OFFSET][2],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_ROT][0], VEHICLE_OBJECTS[vehicleid][slot][vobject_ROT][1], VEHICLE_OBJECTS[vehicleid][slot][vobject_ROT][2],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_ATTACHED], VEHICLE_OBJECTS[vehicleid][slot][vobject_COLORS][0], VEHICLE_OBJECTS[vehicleid][slot][vobject_COLORS][1],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_COLORS][2], VEHICLE_OBJECTS[vehicleid][slot][vobject_COLORS][3], VEHICLE_OBJECTS[vehicleid][slot][vobject_COLORS][4],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_text_TEXT], VEHICLE_OBJECTS[vehicleid][slot][vobject_text_FONT], VEHICLE_OBJECTS[vehicleid][slot][vobject_text_FONT_SIZE],
			VEHICLE_OBJECTS[vehicleid][slot][vobject_text_BOLD], VEHICLE_OBJECTS[vehicleid][slot][vobject_text_FONT_COLOR],

			VEHICLE_OBJECTS[vehicleid][slot][vobject_ID]
	);
	mysql_tquery(srp_db, QUERY_BUFFER);
	return 1;
}



GetRulePagePositions(lines, &Float:pos_descripcion, &Float:pos_pages, &Float:pos_title)
{
	new Float:val[3];
	val[0] = 215.0 - (7.0 * (lines - 1) );
	val[1] = val[0] + (13.0 * (lines -1) ) + 20.0;
	val[2] = val[0] - 25.0;
	
	pos_descripcion = val[0];
	pos_pages = val[1];
	pos_title = val[2];
	return 1;
}

SetPlayerRulesTextDraw(playerid, const title[], info[], info_lines, page, total_pages)
{
	if(PlayerTextdraws[playerid][ptextdraw_GUIDE][0] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][0]); PlayerTextdraws[playerid][ptextdraw_GUIDE][0] = PlayerText:INVALID_TEXT_DRAW;
		PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][1]); PlayerTextdraws[playerid][ptextdraw_GUIDE][1] = PlayerText:INVALID_TEXT_DRAW;
		PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2]); PlayerTextdraws[playerid][ptextdraw_GUIDE][2] = PlayerText:INVALID_TEXT_DRAW;
		PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3]); PlayerTextdraws[playerid][ptextdraw_GUIDE][3] = PlayerText:INVALID_TEXT_DRAW;
		PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][4]); PlayerTextdraws[playerid][ptextdraw_GUIDE][4] = PlayerText:INVALID_TEXT_DRAW;
	}
	
	FixTextDrawString(info);
	
	new Float:pos[3];
	GetRulePagePositions(info_lines, pos[0], pos[1], pos[2]);
	PlayerTextdraws[playerid][ptextdraw_GUIDE][0] = CreatePlayerTextDraw(playerid, 320.000000, pos[2], title);
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][0], 0.386999, 1.861333);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][0], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][0], -76);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][0], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][0], 1);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][0], 5272575);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][0], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][0], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][0], 0);

	PlayerTextdraws[playerid][ptextdraw_GUIDE][1] = CreatePlayerTextDraw(playerid, 320.000000, pos[0], info);
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][1], 0.314666, 1.442370);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][1], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][1], -76);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][1], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][1], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][1], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][1], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][1], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][1], 0);

	PlayerTextdraws[playerid][ptextdraw_GUIDE][2] = CreatePlayerTextDraw(playerid, 437.000000, pos[1], "Siguiente");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2], 0.386999, 1.861333);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2], 3);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2], -76);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2], 1);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2], 5272575);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2], 0);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2], 500.0, 20.0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2], true);

	PlayerTextdraws[playerid][ptextdraw_GUIDE][3] = CreatePlayerTextDraw(playerid, 203.000000, pos[1], "Anterior");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3], 0.386999, 1.861333);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3], -76);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3], 1);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3], 5272575);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3], 0);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3], 266.0, 20.0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3], true);
	
	if(page == total_pages) PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2], "Finalizar");
	new str[12]; format(str, sizeof str, "-_%d/%d_-", page, total_pages);
	PlayerTextdraws[playerid][ptextdraw_GUIDE][4] = CreatePlayerTextDraw(playerid, 320.000000, pos[1] + 3.5, str);
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][4], 0.257998, 1.172740);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][4], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][4], -76);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][4], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][4], 1);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][4], 5272575);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][4], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][4], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][4], 0);
	
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][0]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][1]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][4]);
	return 1;
}

DestroyPlayerRulesTextDraw(playerid)
{
	if(PlayerTextdraws[playerid][ptextdraw_GUIDE][0] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][0]); PlayerTextdraws[playerid][ptextdraw_GUIDE][0] = PlayerText:INVALID_TEXT_DRAW;
		PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][1]); PlayerTextdraws[playerid][ptextdraw_GUIDE][1] = PlayerText:INVALID_TEXT_DRAW;
		PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][2]); PlayerTextdraws[playerid][ptextdraw_GUIDE][2] = PlayerText:INVALID_TEXT_DRAW;
		PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][3]); PlayerTextdraws[playerid][ptextdraw_GUIDE][3] = PlayerText:INVALID_TEXT_DRAW;
		PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_GUIDE][4]); PlayerTextdraws[playerid][ptextdraw_GUIDE][4] = PlayerText:INVALID_TEXT_DRAW;
	}
	return 1;
}

FixTextDrawString(message[], bool:space = false) {
	for(new len = strlen(message), pos; pos < len; pos ++)
	{
		switch(message[pos])
		{
			case 'à': message[pos] = 151;
			case 'á': message[pos] = 152;
			case 'â': message[pos] = 153;
			case 'ä': message[pos] = 154;
			case 'À': message[pos] = 128;
			case 'Á': message[pos] = 129;
			case 'Â': message[pos] = 130;
			case 'Ä': message[pos] = 131;
			case 'è': message[pos] = 157;
			case 'é': message[pos] = 158;
			case 'ê': message[pos] = 159;
			case 'ë': message[pos] = 160;
			case 'È': message[pos] = 134;
			case 'É': message[pos] = 135;
			case 'Ê': message[pos] = 136;
			case 'Ë': message[pos] = 137;
			case 'ì': message[pos] = 161;
			case 'í': message[pos] = 162;
			case 'î': message[pos] = 163;
			case 'ï': message[pos] = 164;
			case 'Ì': message[pos] = 138;
			case 'Í': message[pos] = 139;
			case 'Î': message[pos] = 140;
			case 'Ï': message[pos] = 141;
			case 'ò': message[pos] = 165;
			case 'ó': message[pos] = 166;
			case 'ô': message[pos] = 167;
			case 'ö': message[pos] = 168;
			case 'Ò': message[pos] = 142;
			case 'Ó': message[pos] = 143;
			case 'Ô': message[pos] = 144;
			case 'Ö': message[pos] = 145;
			case 'ù': message[pos] = 169;
			case 'ú': message[pos] = 170;
			case 'û': message[pos] = 171;
			case 'ü': message[pos] = 172;
			case 'Ù': message[pos] = 146;
			case 'Ú': message[pos] = 147;
			case 'Û': message[pos] = 148;
			case 'Ü': message[pos] = 149;
			case 'ñ': message[pos] = 174;
			case 'Ñ': message[pos] = 173;
			case '¡': message[pos] = 64;
			case '¿': message[pos] = 175;
			case '`': message[pos] = 177;
			case '&': message[pos] = 38;
			case ' ': {
				if(space) {
					message[pos] = '_';
				}
			}
			default:  message[pos] = message[pos];
		}
	}
}

PlayerPlaySoundEx(playerid, sound, Float:X, Float:Y, Float:Z)
{
	if(PI[playerid][ac_CONFIG_SOUNDS]) PlayerPlaySound(playerid, sound, X, Y, Z);
	return 1;
}

PlayAudioStreamForPlayerEx(playerid, url[], Float:posX = 0.0, Float:posY = 0.0, Float:posZ = 0.0, Float:distance = 50.0, usepos = 0)
{
	if(PI[playerid][ac_CONFIG_AUDIO]) PlayAudioStreamForPlayer(playerid, url, posX, posY, posZ, distance, usepos);
	return 1;
}

TogglePlayerControllableEx(playerid, bool:controllable)
{
	PLAYER_TEMP[playerid][pt_CONTROL] = controllable;
	return TogglePlayerControllable(playerid, controllable);
}

IsPlayerInSafeZone(playerid)
{
	for(new i = 0; i != sizeof SAFE_ZONES; i ++)
	{
		if(IsPlayerInDynamicArea(playerid, SAFE_ZONES[i][safe_zone_AREA_ID]))
		{
			return true;
		}
	}
	return false;
}

SetPlayerVip(playerid, vip_level, price_coin = 0, days = 30)
{
	if(vip_level < 1) return 0;
	
	vip_level = 2;//ok
	PI[playerid][ac_COINS] -= price_coin;
	ACCOUNT_INFO[playerid][ac_VIP] = vip_level;
	
	inline OnInfoQueryLoad()
	{
		inline OnCountQueryLoad()
		{
			new rows;
			if(cache_get_row_count(rows))
			{
				if(rows)
				{
					cache_get_value_index(0, 0, ACCOUNT_INFO[playerid][ac_VIP_EXPIRE_DATE]);
					SendClientMessageEx(playerid, -1, "{"#PRIMARY_COLOR"}¡Felicidades! {FFFFFF}Has comprado VIP (%d) por %d días, fecha de caducidad: %s.", ACCOUNT_INFO[playerid][ac_VIP], days, ACCOUNT_INFO[playerid][ac_VIP_EXPIRE_DATE]);
					SendClientMessage(playerid, -1, "Puedes utilizar {"#PRIMARY_COLOR"}/vip {FFFFFF}para ver el tiempo restante o renovar.");
					UnlockPlayerVehicles(playerid);
					if(GetPlayerSkin(playerid) == PI[playerid][ac_SKIN]) SetPlayerToys(playerid);
				}
			}
		}
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT vip_expire_date FROM player WHERE id = %d;", ACCOUNT_INFO[playerid][ac_ID]);
		MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET coins = %d, vip = %d, vip_expire_date = DATE_ADD(NOW(), INTERVAL %d DAY) WHERE id = %d;", PI[playerid][ac_COINS], ACCOUNT_INFO[playerid][ac_VIP], days, ACCOUNT_INFO[playerid][ac_ID]);
	MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	return 1;
}

UpdateUnnocupiedPropertyLabel(index)
{
	if(!PROPERTY_INFO[index][property_VALID]) return 0;
	if(PROPERTY_INFO[index][property_SOLD]) return 0;
	
	new label_str[256];
	if(PROPERTY_INFO[index][property_VIP_LEVEL])
	{
		if(PROPERTY_INFO[index][property_EXTRA])
		{
			format
			(
				label_str, 
				sizeof label_str, 
				"\
					{"#PRIMARY_COLOR"}Membresía VIP requerida\n\
					\n\
					{FFFFFF}Propiedad {"#PRIMARY_COLOR"}#%d {FFFFFF}está en venta\n\n\
					Interior: {"#PRIMARY_COLOR"}%d\n\
					{FFFFFF}Coste: {"#PRIMARY_COLOR"}%d "SERVER_COIN"\n\
					{FFFFFF}Nivel necesario: {"#PRIMARY_COLOR"}%d\n\
					{FFFFFF}Usa {"#PRIMARY_COLOR"}/ayuda {FFFFFF}para ver como adquirirla.\
				", PROPERTY_INFO[index][property_ID], PROPERTY_INFO[index][property_ID_INTERIOR], PROPERTY_INFO[index][property_EXTRA], PROPERTY_INFO[index][property_LEVEL]
			);
		}
		else
		{
			format
			(
				label_str, 
				sizeof label_str, 
				"\
					{"#PRIMARY_COLOR"}Membresía VIP requerida\n\
					\n\
					{FFFFFF}Propiedad {"#PRIMARY_COLOR"}#%d {FFFFFF}está en venta\n\n\
					Interior: {"#PRIMARY_COLOR"}%d\n\
					{FFFFFF}Precio: {"#PRIMARY_COLOR"}%s$\n\
					{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\n\
					{FFFFFF}Usa {"#PRIMARY_COLOR"}/ayuda {FFFFFF}para ver como adquirirla.\
				", PROPERTY_INFO[index][property_ID], PROPERTY_INFO[index][property_ID_INTERIOR], number_format_thousand(PROPERTY_INFO[index][property_PRICE]), PROPERTY_INFO[index][property_LEVEL]
			);
		}
	}
	else
	{
		if(PROPERTY_INFO[index][property_EXTRA])
		{
			format
			(
				label_str, 
				sizeof label_str, 
				"\
					{FFFFFF}Propiedad {"#PRIMARY_COLOR"}#%d {FFFFFF}está en venta\n\n\
					Interior: {"#PRIMARY_COLOR"}%d\n\
					{FFFFFF}Coste: {"#PRIMARY_COLOR"}%d "SERVER_COIN"\n\
					{FFFFFF}Nivel necesario: {"#PRIMARY_COLOR"}%d\n\
					{FFFFFF}Usa {"#PRIMARY_COLOR"}/ayuda {FFFFFF}para ver como adquirirla.\
				", PROPERTY_INFO[index][property_ID], PROPERTY_INFO[index][property_ID_INTERIOR], PROPERTY_INFO[index][property_EXTRA], PROPERTY_INFO[index][property_LEVEL]
			);
		}
		else
		{
			format
			(
				label_str, 
				sizeof label_str, 
				"\
					{FFFFFF}Propiedad {"#PRIMARY_COLOR"}#%d {FFFFFF}está en venta\n\n\
					Interior: {"#PRIMARY_COLOR"}%d\n\
					{FFFFFF}Precio: {"#PRIMARY_COLOR"}%s$\n\
					{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\n\
					{FFFFFF}Usa {"#PRIMARY_COLOR"}/ayuda {FFFFFF}para ver como adquirirla.\
				", PROPERTY_INFO[index][property_ID], PROPERTY_INFO[index][property_ID_INTERIOR], number_format_thousand(PROPERTY_INFO[index][property_PRICE]), PROPERTY_INFO[index][property_LEVEL]
			);
		}
	}
	UpdateDynamic3DTextLabelText(PROPERTY_INFO[index][property_EXT_LABEL_ID], 0xFFFFFFFF, label_str);
	return 1;
}


FreezePlayer(playerid, ms = 2000)
{
	TogglePlayerControllableEx(playerid, false);
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][3]);
	PLAYER_TEMP[playerid][pt_TIMERS][3] = SetTimerEx("TogglePlayerControl", ms, false, "ib", playerid, true);
	return 1;
}

SetPlayerNormalColor(playerid)
{
	///*if(ACCOUNT_INFO[playerid][ac_VIP]) return SetPlayerColorEx(playerid, 0x{"#PRIMARY_COLOR"}00);
	//else
	//{
	//	if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] && PLAYER_TEMP[playerid][pt_ADMIN_SERVICE] == true) SetPlayerColorEx(playerid, 0x8000FF00);
	//	else SetPlayerColorEx(playerid, PLAYER_COLOR);
	//}*/
	SetPlayerColorEx(playerid, PLAYER_COLOR);
	return 1;
}



SendAdminAd(color, const text[])
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if((PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL || PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_DEAD) && PI[i][ac_CONFIG_ADMIN])
			{
				SendClientMessage(i, color, text);
			}
		}
	}
	return 1;
}
GetFreePoliceObjectSlot()
{
	for(new i = 0; i != MAX_POLICE_OBJECTS; i ++)
	{
		if(!POLICE_OBJECTS[i][police_object_VALID]) return i;
	}
	return -1;
}

AddPlayerPoliceHistory(playerid, by_id, reason[])
{
	if(!ACCOUNT_INFO[playerid][ac_ID]) return 0;
	
	new date[24];
	getDateTime(date);
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO police_history (id_player, by_id, text, date) VALUES (%d, %d, '%e', '%e');", ACCOUNT_INFO[playerid][ac_ID], by_id, reason, date);
	mysql_tquery(srp_db, QUERY_BUFFER);
	return 1;
}

PlayMp3(playerid, url[], song_name[])
{
	if(PLAYER_TEMP[playerid][pt_MUSIC_FOR_PROPERTY])
	{
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(IsPlayerConnected(i))
			{
				if( (ACCOUNT_INFO[i][ac_STATE] == ROLEPLAY_STATE_OWN_PROPERTY || ACCOUNT_INFO[i][ac_STATE] == ROLEPLAY_STATE_GUEST_PROPERTY) && PI[i][ac_LOCAL_INTERIOR] == PI[playerid][ac_LOCAL_INTERIOR])
				{
					PlayAudioStreamForPlayerEx(i, url);
					SendFormatNotification(i, "~b~Reproduciendo '%s'~n~~n~~w~Utiliza ~r~/stop ~w~para parar la música.", song_name);
				}
			}
		}
		PLAYER_TEMP[playerid][pt_MUSIC_FOR_PROPERTY] = false;
		Auto_SendPlayerAction(playerid, "pone música en su propiedad.");
	}
	else if(PLAYER_TEMP[playerid][pt_MUSIC_FOR_VEHICLE])
	{
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(IsPlayerConnected(i))
			{
				if(IsPlayerInAnyVehicle(i))
				{
					if(GetPlayerVehicleID(playerid) == GetPlayerVehicleID(i))
					{
						PlayAudioStreamForPlayerEx(i, url);
						SendFormatNotification(i, "~b~Reproduciendo '%s'~n~~n~~w~Utiliza ~r~/stop ~w~para parar la música.", song_name);
					}
				}
			}
		}
		PLAYER_TEMP[playerid][pt_MUSIC_FOR_VEHICLE] = false;
		if(PLAYER_VEHICLES[ GetPlayerVehicleID(playerid) ][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID]) Auto_SendPlayerAction(playerid, "pone música en su vehículo.");
		else Auto_SendPlayerAction(playerid, "pone música en el vehículo.");
	}
	else if(PLAYER_TEMP[playerid][pt_MUSIC_FOR_SPEAKERS])
	{
		new Float:pos[3];
		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(IsPlayerConnected(i))
			{
				if((PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL || PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_DEAD) && PI[i][ac_CONFIG_AUDIO])
				{
					if(GetPlayerDistanceFromPoint(i, pos[0], pos[1], pos[2]) <= 30.0)
					{
						PlayAudioStreamForPlayerEx(i, url, pos[0], pos[1], pos[2], 50.0, true);
						SendClientMessageEx(i, 0xf796bdFF, "* Se escuchan unos altavoces...");
					}
				}
			}
		}
		PLAYER_TEMP[playerid][pt_SPEAKERS_TIME] = gettime();
		PLAYER_TEMP[playerid][pt_MUSIC_FOR_SPEAKERS] = false;
		Auto_SendPlayerAction(playerid, "pone música de su MP3 en los altavoces.");
	}
	else
	{
		PlayAudioStreamForPlayerEx(playerid, url);
		SendFormatNotification(playerid, "Reproduciendo ~g~'%s' ~w~usa ~g~/stop ~w~para parar la música.", song_name);
		Auto_SendPlayerAction(playerid, "escucha música de su MP3 en sus auriculares.");
	}
	return 1;
}

Set_FUMIGATOR_Checkpoint(playerid)
{
	if(IsValidDynamicRaceCP(PLAYER_TEMP[playerid][pt_FUMIGATOR_CHECKPOINT]))
	{
		DestroyDynamicRaceCP(PLAYER_TEMP[playerid][pt_FUMIGATOR_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_FUMIGATOR_CHECKPOINT] = INVALID_STREAMER_ID;
	}
	
	if(PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] == sizeof(FUMIGATOR_CHECKPOINTS) - 1)
	{
		PLAYER_TEMP[playerid][pt_FUMIGATOR_CHECKPOINT] =	CreateDynamicRaceCP
														(
															4,
															FUMIGATOR_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] ][0],
															FUMIGATOR_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] ][1],
															FUMIGATOR_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] ][2],
															0.0,
															0.0,
															0.0,
															10.0, 
															0,
															0,
															playerid,
															999999.0
														);
	}
	else
	{
		PLAYER_TEMP[playerid][pt_FUMIGATOR_CHECKPOINT] =	CreateDynamicRaceCP
														(
															3,
															FUMIGATOR_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] ][0],
															FUMIGATOR_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] ][1],
															FUMIGATOR_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] ][2],
															FUMIGATOR_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] + 1 ][0],
															FUMIGATOR_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] + 1 ][1],
															FUMIGATOR_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] + 1 ][2],
															10.0, 
															0,
															0,
															playerid,
															999999.0
														);
	}
	
	new info[1];
	info[0] = CHECKPOINT_TYPE_FUMIGATOR;
	Streamer_SetArrayData(STREAMER_TYPE_RACE_CP, PLAYER_TEMP[playerid][pt_FUMIGATOR_CHECKPOINT], E_STREAMER_EXTRA_ID, info);
	
	return 1;
}

SetPlayerDrivingSchoolCP(playerid)
{
	if(IsValidDynamicRaceCP(PLAYER_TEMP[playerid][pt_DL_EXAM_CP]))
	{
		DestroyDynamicRaceCP(PLAYER_TEMP[playerid][pt_DL_EXAM_CP]);
		PLAYER_TEMP[playerid][pt_DL_EXAM_CP] = INVALID_STREAMER_ID;
	}

	if(PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] == sizeof(Driving_School_Points) - 1)
	{
		PLAYER_TEMP[playerid][pt_DL_EXAM_CP] =	CreateDynamicRaceCP
												(
													1,
													Driving_School_Points[ PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] ][0],
													Driving_School_Points[ PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] ][1],
													Driving_School_Points[ PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] ][2],
													0.0,
													0.0,
													0.0,
													5.0, 
													0,
													0,
													playerid
												);
	}
	else
	{
		PLAYER_TEMP[playerid][pt_DL_EXAM_CP] =	CreateDynamicRaceCP
												(
													0,
													Driving_School_Points[ PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] ][0],
													Driving_School_Points[ PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] ][1],
													Driving_School_Points[ PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] ][2],
													Driving_School_Points[ PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] + 1][0],
													Driving_School_Points[ PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] + 1][1],
													Driving_School_Points[ PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] + 1][2],
													5.0, 
													0,
													0,
													playerid
												);
	}

	new info[1];
	info[0] = CHECKPOINT_TYPE_DRIVING_SCHOOL;
	Streamer_SetArrayData(STREAMER_TYPE_RACE_CP, PLAYER_TEMP[playerid][pt_DL_EXAM_CP], E_STREAMER_EXTRA_ID, info);
	return 1;
}

getPhoneNumber(dbid)
{
	new number_str[24];
	format(number_str, sizeof number_str, "%d%04d", random(999), dbid);
	return strval(number_str);
}

getBankAccountNumber(dbid)
{
	new number_str[24];
	format(number_str, sizeof number_str, "%d%04d", random(999), dbid);
	return strval(number_str);
}

SetPiDefaultValues(playerid)
{
	new tmp_PI[enum_PI];
	PI[playerid] = tmp_PI;

	new date[24];
	getDateTime(date);

	GetPlayerIp(playerid, ACCOUNT_INFO[playerid][ac_IP], 16);
	format(ACCOUNT_INFO[playerid][ac_NAME], 24, "%s", PLAYER_TEMP[playerid][pt_NAME]);
	format(ACCOUNT_INFO[playerid][ac_LAST_CONNECTION], 24, "%s", date);
	format(ACCOUNT_INFO[playerid][ac_REG_DATE], 24, "%s", date);
	ACCOUNT_INFO[playerid][ac_LAST_CONNECTION_TIMESTAMP] = gettime();
	ACCOUNT_INFO[playerid][ac_LEVEL] = 1;
	ACCOUNT_INFO[playerid][ac_REP] = 1;
	ACCOUNT_INFO[playerid][ac_TIME_FOR_REP] = TIME_FOR_REP;
	ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_NORMAL;
	ACCOUNT_INFO[playerid][ac_DOUBT_CHANNEL] = 1;
	PI[playerid][ac_CONNECTED] = 1;
	PI[playerid][ac_PLAYERID] = playerid;
	PI[playerid][ac_SKIN] = Intro_Skins[0][0];
	PI[playerid][ac_CASH] = minrand(5000, 7000);
	PI[playerid][ac_POS_X] = New_User_Pos[0];
	PI[playerid][ac_POS_Y] = New_User_Pos[1];
	PI[playerid][ac_POS_Z] = New_User_Pos[2];
	PI[playerid][ac_ANGLE] = New_User_Pos[3];
	PI[playerid][ac_FIGHT_STYLE] = 4;
	PI[playerid][ac_HEALTH] = 100.0;
	PI[playerid][ac_HUNGRY] = 100.0;
	PI[playerid][ac_THIRST] = 100.0;
	PI[playerid][ac_CONFIG_SOUNDS] = 1;
	PI[playerid][ac_CONFIG_AUDIO] = 1;
	PI[playerid][ac_CONFIG_TIME] = 1;
	PI[playerid][ac_CONFIG_HUD] = 1;
	PI[playerid][ac_CONFIG_ADMIN] = 1;
	PI[playerid][ac_CONFIG_SECURE_LOGIN] = 0;
	PI[playerid][ac_PHONE_VISIBLE_NUMBER] = 1;
	return 1;
}

getDateTime(string[])
{
	new 
		Year, Month, Day,
		Hour, Minute, Second
	;
	getdate(Year, Month, Day);
	gettime(Hour, Minute, Second);
	
	format(string, 24, "%04d-%02d-%02d %02d:%02d:%02d", Year, Month, Day, Hour, Minute, Second);
	return 1;
}

LoginPlayer(playerid) {
	PLAYER_TEMP[playerid][pt_USER_LOGGED] = true;

	LoadPlayerVehicles(playerid);
	LoadPlayerPhoneBook(playerid);
	LoadPlayerToys(playerid);
	LoadPlayerGPSData(playerid);
	LoadPlayerPocketData(playerid);
	LoadPlayerWeaponsData(playerid);
	LoadPlayerWorks(playerid);
	LoadPlayerCrewInfo(playerid);

	SetPlayerScore(playerid, ACCOUNT_INFO[playerid][ac_LEVEL]);
	PLAYER_TEMP[playerid][pt_DOUBT_CHANNEL_TIME] = gettime();
	ResetPlayerWeapons(playerid);
	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, PI[playerid][ac_CASH]);
	SetPlayerFightingStyle(playerid, PI[playerid][ac_FIGHT_STYLE]);
	SetPlayerHealthEx(playerid, PI[playerid][ac_HEALTH]);
	SetPlayerArmourEx(playerid, PI[playerid][ac_ARMOUR]);
	SetPlayerVirtualWorld(playerid, 0);
	SetPlayerNormalColor(playerid);
	StopAudioStreamForPlayer(playerid);
	SetSpawnInfo(playerid, NO_TEAM, PI[playerid][ac_SKIN], PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z], PI[playerid][ac_ANGLE], 0, 0, 0, 0, 0, 0);
	SetPlayerInterior(playerid, PI[playerid][ac_INTERIOR]);
	
	PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] = 0;
	SendClientMessageEx(playerid, 0xB09CA9FF, "> Bienvenido de vuelta, %s. Tu última conexión fue el %s.", PLAYER_TEMP[playerid][pt_RP_NAME], ACCOUNT_INFO[playerid][ac_LAST_CONNECTION]);
    TogglePlayerSpectatingEx(playerid, false);
	TogglePlayerControllableEx(playerid, true);
}

UpdatePlayerWorldInfo(playerid)
{
	if(PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_DEAD || PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_NORMAL)
	{
		//health armour
		GetPlayerHealth(playerid, PI[playerid][ac_HEALTH]);
		GetPlayerArmour(playerid, PI[playerid][ac_ARMOUR]);
		if(PI[playerid][ac_HEALTH] <= 0.0) PI[playerid][ac_HEALTH] = 1.0;
		if(PI[playerid][ac_HEALTH] > 100.0) PI[playerid][ac_HEALTH] = 100.0;
		if(PI[playerid][ac_ARMOUR] > 100.0) PI[playerid][ac_ARMOUR] = 100.0;

		//world
		switch(ACCOUNT_INFO[playerid][ac_STATE])
		{
			case ROLEPLAY_STATE_INTERIOR:
			{
				new index = GetEnterExitIndexById(PI[playerid][ac_LOCAL_INTERIOR]);
				if(index == -1)
				{
					ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_NORMAL;
					PI[playerid][ac_LOCAL_INTERIOR] = 0;
					PI[playerid][ac_POS_X] = New_User_Pos[0];
					PI[playerid][ac_POS_Y] = New_User_Pos[1];
					PI[playerid][ac_POS_Z] = New_User_Pos[2];
					PI[playerid][ac_ANGLE] = New_User_Pos[3];
					PI[playerid][ac_INTERIOR] = 0;
				}
				else
				{
					ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_NORMAL;
					PI[playerid][ac_LOCAL_INTERIOR] = 0;
					PI[playerid][ac_POS_X] = ENTER_EXIT[index][ee_EXT_X];
					PI[playerid][ac_POS_Y] = ENTER_EXIT[index][ee_EXT_Y];
					PI[playerid][ac_POS_Z] = ENTER_EXIT[index][ee_EXT_Z];
					PI[playerid][ac_ANGLE] = ENTER_EXIT[index][ee_EXT_ANGLE];
					PI[playerid][ac_INTERIOR] = ENTER_EXIT[index][ee_EXT_INTERIOR];
				}
			}
			case ROLEPLAY_STATE_OWN_PROPERTY:
			{
				new index = GetPropertyIndexByID(PI[playerid][ac_LOCAL_INTERIOR]);
				if(index == -1)
				{
					ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_NORMAL;
					PI[playerid][ac_LOCAL_INTERIOR] = 0;
					PI[playerid][ac_POS_X] = New_User_Pos[0];
					PI[playerid][ac_POS_Y] = New_User_Pos[1];
					PI[playerid][ac_POS_Z] = New_User_Pos[2];
					PI[playerid][ac_ANGLE] = New_User_Pos[3];
					PI[playerid][ac_INTERIOR] = 0;
				}
				else
				{
					if(PROPERTY_INFO[index][property_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
					{
						PI[playerid][ac_POS_X] = PROPERTY_INTERIORS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_INT_X];
						PI[playerid][ac_POS_Y] = PROPERTY_INTERIORS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_INT_Y];

						new Float:z_pos = PROPERTY_INTERIORS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_INT_Z];
						if(PROPERTY_INFO[index][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
						PI[playerid][ac_POS_Z] = z_pos;

						PI[playerid][ac_ANGLE] = PROPERTY_INTERIORS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_INT_ANGLE];
						PI[playerid][ac_INTERIOR] = PROPERTY_INTERIORS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_INT_INTERIOR];
					}
					else
					{
						ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_NORMAL;
						PI[playerid][ac_LOCAL_INTERIOR] = 0;
						PI[playerid][ac_POS_X] = PROPERTY_INFO[index][property_EXT_X];
						PI[playerid][ac_POS_Y] = PROPERTY_INFO[index][property_EXT_Y];
						PI[playerid][ac_POS_Z] = PROPERTY_INFO[index][property_EXT_Z];
						PI[playerid][ac_ANGLE] = PROPERTY_INFO[index][property_EXT_ANGLE];
						PI[playerid][ac_INTERIOR] = PROPERTY_INFO[index][property_EXT_INTERIOR];
					}
				}
			}
			case ROLEPLAY_STATE_GUEST_PROPERTY:
			{
				new index = GetPropertyIndexByID(PI[playerid][ac_LOCAL_INTERIOR]);
				if(index == -1)
				{
					ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_NORMAL;
					PI[playerid][ac_LOCAL_INTERIOR] = 0;
					PI[playerid][ac_POS_X] = New_User_Pos[0];
					PI[playerid][ac_POS_Y] = New_User_Pos[1];
					PI[playerid][ac_POS_Z] = New_User_Pos[2];
					PI[playerid][ac_ANGLE] = New_User_Pos[3];
					PI[playerid][ac_INTERIOR] = 0;
				}
				else
				{
					ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_NORMAL;
					PI[playerid][ac_LOCAL_INTERIOR] = 0;
					PI[playerid][ac_POS_X] = PROPERTY_INFO[index][property_EXT_X];
					PI[playerid][ac_POS_Y] = PROPERTY_INFO[index][property_EXT_Y];
					PI[playerid][ac_POS_Z] = PROPERTY_INFO[index][property_EXT_Z];
					PI[playerid][ac_ANGLE] = PROPERTY_INFO[index][property_EXT_ANGLE];
					PI[playerid][ac_INTERIOR] = PROPERTY_INFO[index][property_EXT_INTERIOR];
				}
			}
			case ROLEPLAY_STATE_JAIL:
			{
				PI[playerid][ac_POS_X] = JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID] ][jail_X];
				PI[playerid][ac_POS_Y] = JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID] ][jail_Y];
				PI[playerid][ac_POS_Z] = JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID] ][jail_Z];
				PI[playerid][ac_ANGLE] = JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID] ][jail_ANGLE];
				PI[playerid][ac_INTERIOR] = JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_INTERIOR];
			}
			default:
			{
				GetPlayerPos(playerid, PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z]);
				GetPlayerFacingAngle(playerid, PI[playerid][ac_ANGLE]);
				PI[playerid][ac_INTERIOR] = GetPlayerInterior(playerid);
			}
		}
		return 1;
	}
	return 0;
}

SavePlayerData(playerid)
{
	if(ACCOUNT_INFO[playerid][ac_ID])
	{
		new tmp_crew[24], tmp_vip_expire_date[24];
		
		tmp_crew = "NULL";
		if(PI[playerid][ac_CREW]) format(tmp_crew, sizeof tmp_crew, "%d", PI[playerid][ac_CREW]);

		tmp_vip_expire_date = "'0000-00-00 00:00:00'";
		if(strlen(ACCOUNT_INFO[playerid][ac_VIP_EXPIRE_DATE]) > 3) format(tmp_vip_expire_date, sizeof tmp_vip_expire_date, "'%s'", ACCOUNT_INFO[playerid][ac_VIP_EXPIRE_DATE]);

		mysql_format
		(
			srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, 
			"\
				UPDATE player SET \
					name = '%e',\
					ip = '%e',\
					email = '%e',\
					salt = '%e',\
					pass = '%e',\
					last_connection = '%e',\
					last_connection_timestamp = %d,\
					state = %d,\
					time_playing = %d,\
					level = %d,\
					rep = %d,\
					connected = %d,\
					playerid = %d,\
					doubt_channel = %d,\
					time_for_rep = %d,\
					admin_level = %d,\
					payday_rep = %d,\
					vip = %d,\
					vip_expire_date = %s,\
					skin = %d,\
					cash = %d,\
					pos_x = %f,\
					pos_y = %f,\
					pos_z = %f,\
					angle = %f,\
					interior = %d,\
					local_interior = %d,\
					fight_style = %d,\
					health = %f,\
					armour = %f,\
					gender = %d,\
					hungry = %f,\
					thirst = %f,\
					black_market_level = %d,\
					wanted_level = %d,\
					police_jail_time = %d,\
					police_duty = %d,\
					police_jail_id = %d,\
					drive_license_points = %d,\
					bank_account = %d,\
					bank_money = %d,\
					phone_number = %d,\
					phone_state = %d,\
					phone_visible_number = %d,\
					gps = %d,\
					maso = %d,\
					mp3 = %d,\
					phone_resolver = %d,\
					speakers = %d,\
					mechanic_pieces = %d,\
					fuel_drum = %d,\
					seed_medicine = %d,\
					seed_cannabis = %d,\
					seed_crack = %d,\
					medicine = %d,\
					cannabis = %d,\
					crack = %d,\
					config_sounds = %d,\
					config_audio = %d,\
					config_time = %d,\
					config_hud = %d,\
					config_admin = %d,\
					config_secure_login = %d,\
					mute = %d,\
					placa_pd = %d,\
					can_buy_bm = %d,\
					crew = %s,\
					crew_rank = %d,\
					mechanic_kits = %d,\
					medical_kits = %d \
				WHERE id = %d;\
			",
				ACCOUNT_INFO[playerid][ac_NAME],
				ACCOUNT_INFO[playerid][ac_IP],
				ACCOUNT_INFO[playerid][ac_EMAIL],
				ACCOUNT_INFO[playerid][ac_SALT],
				ACCOUNT_INFO[playerid][ac_PASS],
				ACCOUNT_INFO[playerid][ac_LAST_CONNECTION],
				ACCOUNT_INFO[playerid][ac_LAST_CONNECTION_TIMESTAMP],
				ACCOUNT_INFO[playerid][ac_STATE],
				ACCOUNT_INFO[playerid][ac_TIME_PLAYING],
				ACCOUNT_INFO[playerid][ac_LEVEL],
				ACCOUNT_INFO[playerid][ac_REP],
				PI[playerid][ac_CONNECTED],
				PI[playerid][ac_PLAYERID],
				ACCOUNT_INFO[playerid][ac_DOUBT_CHANNEL],
				ACCOUNT_INFO[playerid][ac_TIME_FOR_REP],
				ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL],
				ACCOUNT_INFO[playerid][ac_PAYDAY_REP],
				ACCOUNT_INFO[playerid][ac_VIP],
				tmp_vip_expire_date,
				PI[playerid][ac_SKIN],
				PI[playerid][ac_CASH],
				PI[playerid][ac_POS_X],
				PI[playerid][ac_POS_Y],
				PI[playerid][ac_POS_Z],
				PI[playerid][ac_ANGLE],
				PI[playerid][ac_INTERIOR],
				PI[playerid][ac_LOCAL_INTERIOR],
				PI[playerid][ac_FIGHT_STYLE],
				PI[playerid][ac_HEALTH],
				PI[playerid][ac_ARMOUR],
				PI[playerid][ac_GENDER],
				PI[playerid][ac_HUNGRY],
				PI[playerid][ac_THIRST],
				PI[playerid][ac_BLACK_MARKET_LEVEL],
				PI[playerid][ac_WANTED_LEVEL],
				PI[playerid][ac_POLICE_JAIL_TIME],
				PI[playerid][ac_POLICE_DUTY],
				PI[playerid][ac_POLICE_JAIL_ID],
				PI[playerid][ac_DRIVE_LICENSE_POINTS],
				PI[playerid][ac_BANK_ACCOUNT],
				PI[playerid][ac_BANK_MONEY],
				PI[playerid][ac_PHONE_NUMBER],
				PI[playerid][ac_PHONE_STATE],
				PI[playerid][ac_PHONE_VISIBLE_NUMBER],
				PI[playerid][ac_GPS],
				PI[playerid][ac_Maso],
				PI[playerid][ac_MP3],
				PI[playerid][ac_PHONE_RESOLVER],
				PI[playerid][ac_SPEAKERS],
				PI[playerid][ac_MECHANIC_PIECES],
				PI[playerid][ac_FUEL_DRUM],
				PI[playerid][ac_SEED_MEDICINE],
				PI[playerid][ac_SEED_CANNABIS],
				PI[playerid][ac_SEED_CRACK],
				PI[playerid][ac_MEDICINE],
				PI[playerid][ac_CANNABIS],
				PI[playerid][ac_CRACK],
				PI[playerid][ac_CONFIG_SOUNDS],
				PI[playerid][ac_CONFIG_AUDIO],
				PI[playerid][ac_CONFIG_TIME],
				PI[playerid][ac_CONFIG_HUD],
				PI[playerid][ac_CONFIG_ADMIN],
				PI[playerid][ac_CONFIG_SECURE_LOGIN],
				PI[playerid][ac_MUTE],
				PI[playerid][ac_PLACA_PD],
				PI[playerid][ac_CAN_BUY_BM],
				tmp_crew,
				PI[playerid][ac_CREW_RANK],
				PI[playerid][ac_MECHANIC_KITS],
				PI[playerid][ac_MEDICAL_KITS],
				ACCOUNT_INFO[playerid][ac_ID]
		);
		Logger_Log(QUERY_BUFFER);
		mysql_tquery(srp_db, QUERY_BUFFER);
		return 1;
	}
	return 0;
}

CountPlayerVehicles(playerid)
{
	new count;
	if(ACCOUNT_INFO[playerid][ac_ID])
	{
		for(new i = 0; i < MAX_VEHICLES; i ++)
		{
			if(PLAYER_VEHICLES[i][player_vehicle_VALID])
			{
				if(PLAYER_VEHICLES[i][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
				{
					count ++;
					if(count >= MAX_SU_VEHICLES)
					{
						break;
					}
				}
			}
		}
	}
	return count;
}

CountPlayerProperties(playerid)
{
	new count;
	if(ACCOUNT_INFO[playerid][ac_ID])
	{
		for(new i = 0; i < MAX_PROPERTIES; i ++)
		{
			if(PROPERTY_INFO[i][property_VALID])
			{
				if(PROPERTY_INFO[i][property_SOLD] && PROPERTY_INFO[i][property_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
				{
					count ++;
					if(count >= MAX_SU_PROPERTIES)
					{
						break;
					}
				}
			}
		}
	}
	return count;
}

SavePlayerWorkLevels(playerid)
{
	if(!ACCOUNT_INFO[playerid][ac_ID]) return 0;
	
	for(new i = 0; i != sizeof work_info; i ++)
	{
		if(!PLAYER_WORKS[playerid][i][pwork_SET] || !i) continue;
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pworks SET level = %d WHERE id_player = %d AND id_work = %d;", PLAYER_WORKS[playerid][i][pwork_LEVEL], ACCOUNT_INFO[playerid][ac_ID], i);
		mysql_tquery(srp_db, QUERY_BUFFER);
	}
	return 1;
}

ptask PlayerTask[PlayerTask_Interval_Seconds * 1000](playerid)
{
	ACCOUNT_INFO[playerid][ac_TIME_PLAYING] += PlayerTask_Interval_Seconds;
}

native gpci(playerid, serial[], len);
CheckPlayerFakeClient(playerid) //experimental
{  
    if(IsPlayerNPC(playerid)) return 0;
    new sr[80],  fcharcount;  
    gpci(playerid, sr, sizeof(sr));  
    for(new i = 0; i < strlen(sr); i++)  
    {  
        if(sr[i] >= '0' && sr[i] <= '9')  fcharcount++;  
    }  
    return (fcharcount>= 30 || strlen(sr) <= 30) ? true : false;
}

GetBootTypeName(type) {
	new name[24];
	switch(type) {
		case BOOT_TYPE_WEAPON: name = "Arma";
		case BOOT_TYPE_MEDICINES: name = "Medicamentos";
		case BOOT_TYPE_CANNABIS: name = "Marihuana";
		case BOOT_TYPE_CRACK: name = "Crack";
	}
	return name;
}

GetClosetTypeName(type) {
	new name[24];
	switch(type) {
		case CLOSET_TYPE_WEAPON: name = "Arma";
		case CLOSET_TYPE_MEDICINES: name = "Medicamentos";
		case CLOSET_TYPE_CANNABIS: name = "Marihuana";
		case CLOSET_TYPE_CRACK: name = "Crack";
	}
	return name;
}

GetVehicleRotation(vehicleid, &Float:rx, &Float:ry, &Float:rz) {
	new Float:qw, Float:qx, Float:qy, Float:qz;
	GetVehicleRotationQuat(vehicleid, qw, qx, qy, qz);
	rx = asin(2 * qy * qz - 2 * qx * qw);
	ry = -atan2(qx * qz + qy * qw, 0.5 - qx * qx - qy * qy);
	rz = -atan2(qx * qy + qz * qw, 0.5 - qx * qx - qz * qz);
}

stock HexToInt(const string[])//DracoBlue
{
   if (string[0] == 0) return 0;
   new i, cur=1, res = 0;
   for (i=strlen(string);i>0;i--) {
     if (string[i-1]<58) res=res+cur*(string[i-1]-48); else res=res+cur*(string[i-1]-65+10);
     cur=cur*16;
   }
   return res;
}

GetCrewTerritories(id)
{
	new
		count;

	for(new i = 0; i != MAX_TERRITORIES; i ++)
	{
		if(TERRITORIES[i][territory_CREW_ID] == id) count++;
	}
	return count;
}

LoadIntroData()
{
	// INTRO 1
	INTRO_1[0] = CreateDynamicActor(50, 47.1593, -239.3631, 1.5781, 205.1492);
	INTRO_1[1] = CreateDynamicActor(59, 48.4112, -236.3764, 1.5781, 106.4268);
	INTRO_1[2] = CreateDynamicActor(98, 48.0544, -235.0872, 1.5781, 153.4899);
	INTRO_1[3] = CreateVehicle(579, 45.6427, -235.7737, 1.5537, 202.5125, 3, 3, -1);

	for(new e = 0; e < 3; e++) SetDynamicActorVirtualWorld(INTRO_1[e], WORLD_INTRO);
	SetVehicleVirtualWorld(INTRO_1[3], WORLD_INTRO);

	ApplyDynamicActorAnimation(INTRO_1[2], "PED", "IDLE_chat", 4.1, 1, 0, 0, 1, 0);
	ApplyDynamicActorAnimation(INTRO_1[1], "COP_AMBIENT", "Coplook_loop", 4.1, 1, 0, 0, 1, 0);
	ApplyDynamicActorAnimation(INTRO_1[0], "CAR", "Fixn_Car_Loop", 4.1, 1, 0, 0, 1, 0);

	// INTRO 2
	INTRO_2[0] = CreateVehicle(560, 415.1954, 2511.5398, 16.1897, 157.1949, 0, 0, -1);
	INTRO_2[1] = CreateVehicle(482, 415.9417, 2505.1563, 16.5969, 41.3965, 0, 0, -1);
	INTRO_2[2] = CreateDynamicActor(112, 417.3246, 2510.1504, 16.4844, 110.3539);
	INTRO_2[3] = CreateDynamicActor(111, 417.5173, 2508.6733, 16.4844, 93.1204);
	INTRO_2[4] = CreateDynamicActor(113, 417.3795, 2507.4189, 16.4844, 65.4215);
	INTRO_2[5] = CreateDynamicActor(116, 412.8536, 2508.1931, 16.4844, 275.7721);

	for(new i = 2; i < 6; i++) SetDynamicActorVirtualWorld(INTRO_2[i], WORLD_INTRO);
	for(new i = 0; i < 2; i++) SetVehicleVirtualWorld(INTRO_2[i], 555);

	ApplyDynamicActorAnimation(INTRO_2[2], "COP_AMBIENT", "Coplook_loop", 4.1, 1, 0, 0, 1, 0);
	ApplyDynamicActorAnimation(INTRO_2[4], "COP_AMBIENT", "Coplook_loop", 4.1, 1, 0, 0, 1, 0);
	ApplyDynamicActorAnimation(INTRO_2[5], "PED", "HANDSUP", 4.1, 0, 0, 0, 1, 0);
	ApplyDynamicActorAnimation(INTRO_2[3], "PED", "ARRESTGUN", 4.1, 0, 0, 0, 1, 0);

	// INTRO 3
	INTRO_3[0] = CreateVehicle(531, 314.8449, 1133.0660, 8.8792, 51.2440, 149, 149, -1);
	INTRO_3[1] = CreateVehicle(478, 306.9597, 1138.1361, 8.5805, 358.9598, 0, 0, -1);
	INTRO_3[2] = CreateDynamicActor(161, 313.3412, 1131.9296, 9.0343, 34.2756);

	for(new i = 0; i < 2; i++) SetVehicleVirtualWorld(INTRO_3[i], 555);
	SetDynamicActorVirtualWorld(INTRO_3[2], 555);

	ApplyDynamicActorAnimation(INTRO_3[2], "BOMBER", "BOMB_PLANT", 4.1, 0, 0, 0, 1, 0);
	return 1;
}

stock CountPolicesOnDuty() {
	new count = 0;
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
			{
				if(PLAYER_WORKS[i][WORK_POLICE][pwork_SET] && PLAYER_TEMP[i][pt_WORKING_IN] == WORK_POLICE) {
					count ++;
				}
			}
		}
	}
	return count;
}

stock GetPlayerCameraRotation(playerid,&Float:rx,&Float:rz){
	new Float:mx,Float:my,Float:mz;
	GetPlayerCameraFrontVector(playerid,mx,my,mz);
	rx = CompressRotation(-(acos(mz)-90.0));
	rz = CompressRotation((atan2(my,mx)-90.0));
}

stock Float:CompressRotation(Float:rotation){
	return (rotation - floatround(rotation/360.0,floatround_floor)*360.0);
}

CountCrewPlayers(id_crew) {
    mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d;", id_crew);
    new count, Cache:result = mysql_query(srp_db, QUERY_BUFFER);
    cache_get_value_int(0, 0, count);
    cache_delete(result);
    return count;
}

GetRandomPropertyIndex() {
	new index = -1;
	while(index == -1) {
		new tmpIndex = random(MAX_PROPERTIES);
		if(PROPERTY_INFO[tmpIndex][property_VALID]) index = tmpIndex;
	}
	return index;
}

IRPC:RPC_VehicleDestroy(playerid, BitStream:bs)
{
	if(ac_Info[CHEAT_DESTROY_VEHICLE][ac_Enabled]) {
		new vehicleid;
		BS_ReadUint16(bs, vehicleid);

		if(vehicleid <= 0 || vehicleid >= MAX_VEHICLES)
		{
			return 0;
		}

		if(ACCOUNT_INFO[playerid][ac_LEVEL] < 3)
		{
			if( (vehicleid == GetPlayerVehicleID(playerid) && GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] == playerid) || (PLAYER_VEHICLES[vehicleid][player_vehicle_VALID] && PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID]) )
			{
				return 1;
			}
			return 0;
		}

		if(!IsVehicleStreamedIn(vehicleid, playerid))
		{
			return 0;
		}

		new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_LAST_VEHICLE_DESTROY]);
		if(interval < 500)
		{
			new current_gettime = gettime();
			if(current_gettime > PLAYER_AC_INFO[playerid][CHEAT_DESTROY_VEHICLE][p_ac_info_IMMUNITY])
			{
				if(!ac_Info[CHEAT_DESTROY_VEHICLE][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_DESTROY_VEHICLE);
				else
				{
					if(current_gettime - PLAYER_AC_INFO[playerid][CHEAT_DESTROY_VEHICLE][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_DESTROY_VEHICLE][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_DESTROY_VEHICLE][p_ac_info_DETECTIONS] = 0;
					else PLAYER_AC_INFO[playerid][CHEAT_DESTROY_VEHICLE][p_ac_info_DETECTIONS] ++;
					
					PLAYER_AC_INFO[playerid][CHEAT_DESTROY_VEHICLE][p_ac_info_LAST_DETECTION] = current_gettime;
					if(PLAYER_AC_INFO[playerid][CHEAT_DESTROY_VEHICLE][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_DESTROY_VEHICLE][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_DESTROY_VEHICLE);
				}
			}
			return 0;
		}
		PLAYER_TEMP[playerid][pt_LAST_VEHICLE_DESTROY] = GetTickCount();
	}
    return 1;
}



GetVehicleComponentIndexFromId(vehicleid, id) {
	for(new i = 0; i != MAX_VEHICLE_COMPONENTS; i ++)
	{
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_COMPONENTS][i] && GLOBAL_VEHICLES[vehicleid][gb_vehicle_COMPONENTS][i] == id) {
			return i;
		}
	}
	return -1;
}

GetTerritoryIndexById(id) {
	for(new i = 0; i != MAX_TERRITORIES; i ++) {
		if(TERRITORIES[i][territory_VALID] && TERRITORIES[i][territory_ID] == id) return i;
	}
	return -1;
}

SendClientMessageEx(playerid, colour, const message[], {Float, _}:...)
{
	new out[145];
	format(out, sizeof(out), message, ___(3));
	return SendClientMessage(playerid, colour, out);
}

stock TimeConvert(seconds)
{
	new tmp[16];
 	new minutes = floatround(seconds/60);
  	seconds -= minutes*60;
   	format(tmp, sizeof(tmp), "%d:%02d", minutes, seconds);
   	return tmp;
}

stock TimeConvertEx(sec, &days, &hours, &minutes, &seconds)
{
	days = floatround(sec / 86400);
	hours = floatround((sec - (days * 86400)) / 3600);
	minutes = floatround((sec - (days * 86400) - (hours * 3600)) / 60);
	seconds = sec % 60;
   	return 1;
}

stock TimeConvertExAsText(sec, bool:lastMinute = false)
{
	new string[128] = "", auxString[32];
	
	if(lastMinute && sec < 60) {
		string = "menos de 1 minuto";
		return string;
	}

	if(sec <= 0) {
		string = "nada";
		return string;
	}

	new days, hours, minutes, seconds;
	TimeConvertEx(sec, days, hours, minutes, seconds);

	if(days) {
		format(auxString, sizeof auxString, "%d días", days);
		strcat(string, auxString);
	}

	if(hours) {
		if(strlen(string) > 0) strcat(string, " ");
		format(auxString, sizeof auxString, "%d horas", hours);
		strcat(string, auxString);
	}

	if(minutes) {
		if(strlen(string) > 0) strcat(string, " ");
		format(auxString, sizeof auxString, "%d minutos", minutes);
		strcat(string, auxString);
	}

	if(seconds) {
		if(strlen(string) > 0) strcat(string, " ");
		format(auxString, sizeof auxString, "%d segundos", seconds);
		strcat(string, auxString);
	}
   	return string;
}

ProxDetector(playerid, Float:radi, string[], col1, col2, col3, col4, col5, div = 0)
{
	new line1_str[165], line2_str[165], bool:line2_used;
	
	if(div)
	{
		format(line1_str, div + 1, "%s", string);
		if(strlen(string) > div)
		{
			format(line2_str, sizeof line2_str, "— %s", string[div]);
			line2_used = true;
		}
	}
	else format(line1_str, sizeof line1_str, "%s", string);
	
	new Float:oldposx, Float:oldposy, Float:oldposz, current_vw = GetPlayerVirtualWorld(playerid), current_int = GetPlayerInterior(playerid);
	GetPlayerPos(playerid, oldposx, oldposy, oldposz);
	
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++) 					
	{
		if(!IsPlayerConnected(i)) continue;
		if(!PLAYER_TEMP[i][pt_USER_LOGGED]) continue;
		if(GetPlayerVirtualWorld(i) != current_vw) continue;
		if(GetPlayerInterior(i) != current_int) continue;
		
		if(IsPlayerInRangeOfPoint(i, radi/16, oldposx, oldposy, oldposz))
		{
			SendClientMessage(i, col1, line1_str);
			if(div && line2_used) SendClientMessage(i, col1, line2_str);
		}
		else if(IsPlayerInRangeOfPoint(i, radi/8, oldposx, oldposy, oldposz))
		{
			SendClientMessage(i, col2, line1_str);
			if(div && line2_used) SendClientMessage(i, col2, line2_str);
		}
		else if(IsPlayerInRangeOfPoint(i, radi/4, oldposx, oldposy, oldposz))
		{
			SendClientMessage(i, col3, line1_str);			
			if(div && line2_used) SendClientMessage(i, col3, line2_str);
		}
		else if(IsPlayerInRangeOfPoint(i, radi/2, oldposx, oldposy, oldposz))
		{
			SendClientMessage(i, col4, line1_str);	
			if(div && line2_used) SendClientMessage(i, col4, line2_str);
		}
		else if(IsPlayerInRangeOfPoint(i, radi, oldposx, oldposy, oldposz))
		{
			SendClientMessage(i, col5, line1_str);
			if(div && line2_used) SendClientMessage(i, col5, line2_str);
		}
	}
	return 1;
}

NearbyMessage(Float:pos_x, Float:pos_y, Float:pos_z, current_int, current_vw, Float:radi, col, const string[])
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++) 					
	{
		if(!IsPlayerConnected(i)) continue;
		if(!PLAYER_TEMP[i][pt_USER_LOGGED]) continue;
		if(GetPlayerVirtualWorld(i) != current_vw) continue;
		if(GetPlayerInterior(i) != current_int) continue;
		
		if(IsPlayerInRangeOfPoint(i, radi, pos_x, pos_y, pos_z)) SendClientMessage(i, col, string);
	}
	return 1;
}

IsValidRPName(const string[])
{
	new Regex:reg_exp = Regex_New("[A-Z][a-z]+_[A-Z][a-z]{1,3}[A-Z]?[a-z]*");
	new result = Regex_Check(string, reg_exp);
	Regex_Delete(reg_exp);
	return result;
}

IsValidEmail(const string[])
{
	new Regex:reg_exp = Regex_New("[a-zA-Z0-9_\\.]+@([a-zA-Z0-9\\-]+\\.)+[a-zA-Z]{2,4}"); 
	new result = Regex_Check(string, reg_exp); 
	Regex_Delete(reg_exp); 
	return result; 
}

/*
IsValidName(name[])
{
	for(new c = 0; c < strlen(name); c++)
	{
	    switch(name[c])
	    {
	        case 'A' .. 'Z', 'a' .. 'z', '_': continue;
	        default: return false;
	    }
	}
	return true;
}
*/

OnPlayerCheatDetected(playerid, cheat, Float:extra = 0.0)
{
	if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] >= ADMIN_LEVEL_AC_IMMUNITY) return 1;
	if(PLAYER_TEMP[playerid][pt_KICKED]) return 1;
	

	if(gettime() < PLAYER_TEMP[playerid][pt_LAST_CHEAT_DETECTED_TIME] + 5) return 1;

	new ac_message[145], player_state = GetPlayerState(playerid);
	
	if(ac_Info[cheat][ac_Kick])
	{	
		new bad_history[24];
		format(bad_history, sizeof bad_history, "ac, cheat (%02d)", cheat);
		AddPlayerBadHistory(ACCOUNT_INFO[playerid][ac_ID], -1, TYPE_KICK, bad_history);
		
		if(extra != 0.0) format(ac_message, sizeof ac_message, "{"#RED_COLOR"}[AC-KICK] {FFFFFF}%s (%d): %s (cd: %02d, ps: %02d, ping: %d, dec: %d:%d, extra: %.1f)", ACCOUNT_INFO[playerid][ac_NAME], playerid, ac_Info[cheat][ac_Name], cheat, player_state, GetPlayerPing(playerid), PLAYER_AC_INFO[playerid][cheat][p_ac_info_DETECTIONS], ac_Info[cheat][ac_Interval], extra);
		else format(ac_message, sizeof ac_message, "{"#RED_COLOR"}[AC-KICK] {FFFFFF}%s (%d): %s (cd: %02d, ps: %02d, ping: %d, dec: %d:%d)", ACCOUNT_INFO[playerid][ac_NAME], playerid, ac_Info[cheat][ac_Name], cheat, player_state, GetPlayerPing(playerid), PLAYER_AC_INFO[playerid][cheat][p_ac_info_DETECTIONS], ac_Info[cheat][ac_Interval]);
		
		KickEx(playerid);
		
		if(cheat == CHEAT_PLAYER_HEALTH) PI[playerid][ac_HEALTH] = 20.0;
		if(cheat == CHEAT_PLAYER_ARMOUR) PI[playerid][ac_ARMOUR] = 0.0;
	}
	else
	{
		if(extra != 0.0) format(ac_message, sizeof ac_message, "{"#RED_COLOR"}[AC-AVISO] {FFFFFF}%s (%d): %s (cd: %02d, ps: %02d, ping: %d, dec: %d:%d, extra: %.1f)", ACCOUNT_INFO[playerid][ac_NAME], playerid, ac_Info[cheat][ac_Name], cheat, player_state, GetPlayerPing(playerid), PLAYER_AC_INFO[playerid][cheat][p_ac_info_DETECTIONS], ac_Info[cheat][ac_Interval], extra);
		else format(ac_message, sizeof ac_message, "{"#RED_COLOR"}[AC-AVISO] {FFFFFF}%s (%d): %s (cd: %02d, ps: %02d, ping: %d, dec: %d:%d)", ACCOUNT_INFO[playerid][ac_NAME], playerid, ac_Info[cheat][ac_Name], cheat, player_state, GetPlayerPing(playerid), PLAYER_AC_INFO[playerid][cheat][p_ac_info_DETECTIONS], ac_Info[cheat][ac_Interval]);
	}
	
	SendMessageToAdminsAC(-1, ac_message);
	PLAYER_TEMP[playerid][pt_LAST_CHEAT_DETECTED_TIME] = gettime();

	return 1;
}

stock ConvertTime_SecondsToHoursFloat(sec, &Float:hours)
{
	hours = floatdiv(float(sec), 3600);
   	return 1;
}
stock number_format_thousand(e_number) // By adri1 - SFR3 4.3 
{
	new f_string[24], bool:negative;
	format(f_string, sizeof f_string, "%d", e_number);
	if(e_number < 0)
	{
		negative = true;
		strdel(f_string, 0, 1);
	}
	
	new numbers = strlen(f_string);
	while(numbers > 3)
	{
		numbers -= 3;
		strins(f_string, ".", numbers);
	}
	
	if(negative) strins(f_string, "-", 0);
	return f_string;
}

SetWeaponsForPlayer(playerid)
{
	PLAYER_AC_INFO[playerid][CHEAT_WEAPON][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_AMMO][p_ac_info_IMMUNITY] = gettime() + 3;

	ResetPlayerWeapons(playerid);

	for(new i = 0; i != sizeof PLAYER_WEAPONS[]; i ++)
	{
		if(PLAYER_WEAPONS[playerid][i][player_weapon_ID] != 0)
		{
			SetPlayerWeapon(playerid, i);
		}
	}
	return 1;
}

stock minrand(min, max) //By Alex "Y_Less" Cole
{
	return random(max - min) + min;
}







ResetPlayerWeaponsEx(playerid)
{
	PLAYER_AC_INFO[playerid][CHEAT_WEAPON][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_AMMO][p_ac_info_IMMUNITY] = gettime() + 3;

	new tmp_PLAYER_WEAPONS[enum_PLAYER_WEAPONS];
	for(new i = 0; i != sizeof PLAYER_WEAPONS[]; i ++) PLAYER_WEAPONS[playerid][i] = tmp_PLAYER_WEAPONS;
	
	ResetPlayerWeapons(playerid);
	return 1;
}

DestroyVehicleEx(vehicleid)
{
	if(!GLOBAL_VEHICLES[vehicleid][gb_vehicle_VALID]) return false;
	
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] != INVALID_PLAYER_ID)
	{
		PLAYER_AC_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] ][CHEAT_VEHICLE_HEALTH][p_ac_info_IMMUNITY] = gettime() + 3;
		PLAYER_AC_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] ][CHEAT_STATE_SPAMMER][p_ac_info_IMMUNITY] = gettime() + 3;
	}
	
	if(DestroyVehicle(vehicleid))
	{
		if(IsValidDynamic3DTextLabel(GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL]))
		{
			DestroyDynamic3DTextLabel(GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL]);
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL] = Text3D:INVALID_STREAMER_ID;
		}

		new temp_GLOBAL_VEHICLES[Global_Vehicles_Info]; GLOBAL_VEHICLES[vehicleid] = temp_GLOBAL_VEHICLES;
		new temp_PLAYER_VEHICLES[Player_Vehicles_Info]; PLAYER_VEHICLES[vehicleid] = temp_PLAYER_VEHICLES;
		new temp_SELL_VEHICLES[SELL_VEHICLES_Enum]; SELL_VEHICLES[vehicleid] = temp_SELL_VEHICLES;
		
		new temp_VEHICLE_BOOT[enum_VEHICLE_BOOT];
		for(new i = 0; i != MAX_BOOT_SLOTS; i ++) VEHICLE_BOOT[vehicleid][i] = temp_VEHICLE_BOOT;

		new temp_VEHICLE_OBJECTS[e_VEHICLE_OBJECTS];
		for(new i = 0; i != MAX_SU_VOBJECTS; i ++)
		{
			if(VEHICLE_OBJECTS[vehicleid][i][vobject_OBJECT_ID] != INVALID_STREAMER_ID) DestroyDynamicObject(VEHICLE_OBJECTS[vehicleid][i][vobject_OBJECT_ID]);
			VEHICLE_OBJECTS[vehicleid][i] = temp_VEHICLE_OBJECTS;
		}
		
		return true;
	}
	return false;
}



SelectTextDrawEx(playerid, color)
{
	PLAYER_TEMP[playerid][pt_SELECT_TEXTDRAW] = true;
	return SelectTextDraw(playerid, color);
}

GetDatabasePages(count, limit)
{
	return floatround(floatdiv(count, limit), floatround_ceil);
}



SetPlayerWeapon(playerid, slot) {
	if(WEAPON_INFO[ PLAYER_WEAPONS[playerid][slot][player_weapon_ID] ][weapon_info_SPECIAL]) {
		switch(WEAPON_INFO[ PLAYER_WEAPONS[playerid][slot][player_weapon_ID] ][weapon_info_TYPE]) {
			case WEAPON_TASER_GUN: {
				GivePlayerWeapon(playerid, 23, PLAYER_WEAPONS[playerid][slot][player_weapon_AMMO]);
			}
		}
	}
	else {
		if(PLAYER_WEAPONS[playerid][slot][player_weapon_AMMO] > 0) GivePlayerWeapon(playerid, PLAYER_WEAPONS[playerid][slot][player_weapon_ID], PLAYER_WEAPONS[playerid][slot][player_weapon_AMMO]);
	}
}



GetPropertyIndexByID(id)
{
	for(new i = 0; i != MAX_PROPERTIES; i ++)
	{
		if(!PROPERTY_INFO[i][property_VALID]) continue;
		if(PROPERTY_INFO[i][property_ID] == id) return i;
	}
	return -1;
}

convertPhoneNumber(playerid, number)
{
	new f_string[24], bool:exists;
	for(new i = 0; i != MAX_PHONE_CONTACTS; i ++)
	{
		if(!PLAYER_PHONE_BOOK[playerid][i][phone_book_contact_VALID]) continue;
		if(number == PLAYER_PHONE_BOOK[playerid][i][phone_book_contact_PHONE_NUMBER])
		{
			exists = true;
			format(f_string, sizeof f_string, "%s", PLAYER_PHONE_BOOK[playerid][i][phone_book_contact_NAME]);
			break;
		}
	}

	if(!exists) format(f_string, sizeof f_string, "%d", number);
	return f_string;
}



SendMessageToDoubtChannel(playerid, message[])
{
	new str[145];
	if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) format(str, 145, "{"#PRIMARY_COLOR"}[Dudas] {"#SILVER_COLOR"}%s (%d) [%s]: %s", PLAYER_TEMP[playerid][pt_RP_NAME], playerid, ADMIN_LEVELS[ ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] ], message);
	else format(str, 145, "{"#PRIMARY_COLOR"}[Dudas] {"#SILVER_COLOR"}%s (%d) [Nivel %d]: %s", PLAYER_TEMP[playerid][pt_RP_NAME], playerid, ACCOUNT_INFO[playerid][ac_LEVEL], message);

	PLAYER_TEMP[playerid][pt_DOUBT_CHANNEL_TIME] = gettime();
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if((PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL || PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_DEAD) && PI[i][ac_DOUBT_CHANNEL])
			{
				SendClientMessage(i, 0x77c7ffFF, str);
			}
		}
	}
	return 1;
}

ShowPlayerSkills(playerid, pid)
{
	if(!IsPlayerConnected(pid)) return 0;
	if(ACCOUNT_INFO[pid][ac_ID] == 0) return 0;
	
	new caption[48], line_str[80], dialog[sizeof line_str * (sizeof(work_info) + 5)];
	format(caption, sizeof caption, "Experiencia de %s", PLAYER_TEMP[pid][pt_RP_NAME]);
	
	strcat(dialog, "{"#BLUE_COLOR"}CUENTA\n");
	format(line_str, sizeof line_str, "{"#SILVER_COLOR"}Nivel: {"#PRIMARY_COLOR"}%d\n", PI[pid][ac_LEVEL]);
	strcat(dialog, line_str);
	
	new neccessary_rep = PI[pid][ac_LEVEL] * REP_MULTIPLIER;
	format(line_str, sizeof line_str, "{"#SILVER_COLOR"}Reputación: {"#PRIMARY_COLOR"}%d/%d\n", PI[pid][ac_REP], neccessary_rep);
	strcat(dialog, line_str);
	
	new Float:hours;
	ConvertTime_SecondsToHoursFloat(ACCOUNT_INFO[pid][ac_TIME_PLAYING], hours);
	format(line_str, sizeof line_str, "{"#SILVER_COLOR"}Horas de juego: {"#PRIMARY_COLOR"}%.1f horas\n", hours);
	strcat(dialog, line_str);
	
	strcat(dialog, "\n{"#BLUE_COLOR"}TRABAJOS\n");
	
	for(new i = 1; i != sizeof work_info; i ++)
	{
		if(i == WORK_POLICE)
		{
			if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
			{
				format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%c%s: {"#PRIMARY_COLOR"}%s\n", toupper(work_info[i][work_info_NAME][0]), work_info[i][work_info_NAME][1], POLICE_RANKS[PLAYER_WORKS[pid][i][pwork_LEVEL]]);
				strcat(dialog, line_str);
			}
		}
		else
		{
			format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%c%s: {"#PRIMARY_COLOR"}%s\n", toupper(work_info[i][work_info_NAME][0]), work_info[i][work_info_NAME][1], number_format_thousand(PLAYER_WORKS[pid][i][pwork_LEVEL]));
			strcat(dialog, line_str);
		}
	}
	
	ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, caption, dialog, "Cerrar", "");	
	return 1;
}

SavePlayerVehicles(playerid, destroy = false)
{
	if(ACCOUNT_INFO[playerid][ac_ID] == 0) return 0;
	
	for(new i = 0, j = GetVehiclePoolSize(); i <= j; i ++)
	{
		if(!PLAYER_VEHICLES[i][player_vehicle_VALID]) continue;
		if(PLAYER_VEHICLES[i][player_vehicle_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) continue;
		
		GetVehiclePos(i, GLOBAL_VEHICLES[i][gb_vehicle_SPAWN_X], GLOBAL_VEHICLES[i][gb_vehicle_SPAWN_Y], GLOBAL_VEHICLES[i][gb_vehicle_SPAWN_Z]);
		GetVehicleZAngle(i, GLOBAL_VEHICLES[i][gb_vehicle_SPAWN_ANGLE]);
		GetVehicleHealth(i, GLOBAL_VEHICLES[i][gb_vehicle_HEALTH]);
		if(GLOBAL_VEHICLES[i][gb_vehicle_HEALTH] > 1000.0) GLOBAL_VEHICLES[i][gb_vehicle_HEALTH] = 1000.0;
		if(GLOBAL_VEHICLES[i][gb_vehicle_HEALTH] < MIN_VEHICLE_HEALTH) GLOBAL_VEHICLES[i][gb_vehicle_HEALTH] = MIN_VEHICLE_HEALTH;
		GetVehicleDamageStatus(i, GLOBAL_VEHICLES[i][gb_vehicle_DAMAGE_PANELS], GLOBAL_VEHICLES[i][gb_vehicle_DAMAGE_DOORS], GLOBAL_VEHICLES[i][gb_vehicle_DAMAGE_LIGHTS], GLOBAL_VEHICLES[i][gb_vehicle_DAMAGE_TIRES]);
		GLOBAL_VEHICLES[i][gb_vehicle_WORLD] = 0;//GetVehicleVirtualWorld(i);
		
		mysql_format
		(
			srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER,
			"\
				UPDATE pvehicles SET \
					id_player = %d,\
					plate = '%e',\
					modelid = %d,\
					spawn_x = %f,\
					spawn_y = %f,\
					spawn_z = %f,\
					spawn_angle = %f,\
					health = %f,\
					damage_panels = %d,\
					damage_doors = %d,\
					damage_lights = %d,\
					damage_tires = %d,\
					color1 = %d,\
					color2 = %d,\
					paintjob = %d,\
					gas = %f,\
					max_gas = %f,\
					closed = %d,\
					interior = %d,\
					world = %d,\
					state = %d,\
					slot0 = %d,\
					slot1 = %d,\
					slot2 = %d,\
					slot3 = %d,\
					slot4 = %d,\
					slot5 = %d,\
					slot6 = %d,\
					slot7 = %d,\
					slot8 = %d,\
					slot9 = %d,\
					slot10 = %d,\
					slot11 = %d,\
					slot12 = %d,\
					slot13 = %d,\
					clamp = %d \
				WHERE id = %d;\
			",
				PLAYER_VEHICLES[i][player_vehicle_OWNER_ID], GLOBAL_VEHICLES[i][gb_vehicle_NUMBER_PLATE], GLOBAL_VEHICLES[i][gb_vehicle_MODELID], GLOBAL_VEHICLES[i][gb_vehicle_SPAWN_X],
				GLOBAL_VEHICLES[i][gb_vehicle_SPAWN_Y], GLOBAL_VEHICLES[i][gb_vehicle_SPAWN_Z], GLOBAL_VEHICLES[i][gb_vehicle_SPAWN_ANGLE], GLOBAL_VEHICLES[i][gb_vehicle_HEALTH],
				GLOBAL_VEHICLES[i][gb_vehicle_DAMAGE_PANELS], GLOBAL_VEHICLES[i][gb_vehicle_DAMAGE_DOORS], GLOBAL_VEHICLES[i][gb_vehicle_DAMAGE_LIGHTS], GLOBAL_VEHICLES[i][gb_vehicle_DAMAGE_TIRES],
				GLOBAL_VEHICLES[i][gb_vehicle_COLOR_1], GLOBAL_VEHICLES[i][gb_vehicle_COLOR_2], GLOBAL_VEHICLES[i][gb_vehicle_PAINTJOB], GLOBAL_VEHICLES[i][gb_vehicle_GAS], GLOBAL_VEHICLES[i][gb_vehicle_MAX_GAS],
				GLOBAL_VEHICLES[i][gb_vehicle_PARAMS_DOORS], GLOBAL_VEHICLES[i][gb_vehicle_INTERIOR], GLOBAL_VEHICLES[i][gb_vehicle_WORLD], GLOBAL_VEHICLES[i][gb_vehicle_STATE],
				GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][0], GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][1], GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][2], GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][3],
				GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][4], GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][5], GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][6], GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][7],
				GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][8], GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][9], GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][10], GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][11],
				GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][12], GLOBAL_VEHICLES[i][gb_vehicle_COMPONENTS][13], PLAYER_VEHICLES[i][player_vehicle_CLAMP],

				PLAYER_VEHICLES[i][player_vehicle_ID]
		);
		mysql_tquery(srp_db, QUERY_BUFFER);

		if(destroy) DestroyVehicleEx(i);
	}
	return 1;
}



SavePlayerToys(playerid)
{
	if(!ACCOUNT_INFO[playerid][ac_ID]) return 0;
	
	for(new i = 0; i != MAX_SU_TOYS; i ++)
	{
		if(!PLAYER_TOYS[playerid][i][player_toy_VALID]) continue;
		mysql_format
		(
			srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, 
			"\
				UPDATE ptoys SET \
					name = '%e',\
					attached = %d,\
					modelid = %d,\
					bone = %d,\
					offset_x = %f,\
					offset_y = %f,\
					offset_z = %f,\
					rot_x = %f,\
					rot_y = %f,\
					rot_z = %f,\
					scale_x = %f,\
					scale_y = %f,\
					scale_z = %f,\
					color1 = %d,\
					color2 = %d \
				WHERE id = %d;\
			",
			PLAYER_TOYS[playerid][i][player_toy_NAME], PLAYER_TOYS[playerid][i][player_toy_ATTACHED], PLAYER_TOYS[playerid][i][player_toy_MODELID],
			PLAYER_TOYS[playerid][i][player_toy_BONE], PLAYER_TOYS[playerid][i][player_toy_OFFSET_X], PLAYER_TOYS[playerid][i][player_toy_OFFSET_Y],
			PLAYER_TOYS[playerid][i][player_toy_OFFSET_Z], PLAYER_TOYS[playerid][i][player_toy_ROT_X], PLAYER_TOYS[playerid][i][player_toy_ROT_Y],
			PLAYER_TOYS[playerid][i][player_toy_ROT_Z], PLAYER_TOYS[playerid][i][player_toy_SCALE_X], PLAYER_TOYS[playerid][i][player_toy_SCALE_Y],
			PLAYER_TOYS[playerid][i][player_toy_SCALE_Z], PLAYER_TOYS[playerid][i][player_toy_COLOR_1], PLAYER_TOYS[playerid][i][player_toy_COLOR_2],
			PLAYER_TOYS[playerid][i][player_toy_ID]
		);
		mysql_tquery(srp_db, QUERY_BUFFER);
	}
	return 1;
}

GetClothingShopIndexByIntType(interior_type)
{
	for(new i = 0; i != sizeof Clothing_Shop_Positions; i ++)
	{
		if(Clothing_Shop_Positions[i][clothing_shop_INTERIOR_TYPE] == interior_type) return i;
	}
	return -1;
}

ShowToysShopTextdraws(playerid)
{
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][4]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][8]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9]);
	return 1;
}

GetPlayerIdByBankAccountId(account_id)
{
	for(new i = 0; i != sizeof(PI); i ++)
	{
		if(PI[i][ac_BANK_ACCOUNT] == 0) continue;
		if(PI[i][ac_BANK_ACCOUNT] == account_id) return i;
	}
	return -1;
}

forward UnjailPlayer(playerid);
public UnjailPlayer(playerid)
{
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][15]);
	PI[playerid][ac_POLICE_JAIL_TIME] = 0;
	SetPlayerWantedLevelEx(playerid, 0);
	ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_INTERIOR;
	SetWeaponsForPlayer(playerid);
	SetPlayerArmedWeapon(playerid, 0);
	
	for(new i = 0; i != sizeof ENTER_EXIT; i ++)
	{
		if(ENTER_EXIT[i][ee_INTERIOR_TYPE] == JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID] ][jail_INT])
		{
			PI[playerid][ac_LOCAL_INTERIOR] = ENTER_EXIT[i][ee_ID];
			PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] = -1;
			SetPlayerPosEx(playerid, ENTER_EXIT[i][ee_INT_X], ENTER_EXIT[i][ee_INT_Y], ENTER_EXIT[i][ee_INT_Z], ENTER_EXIT[i][ee_INT_ANGLE] + 180.0, ENTER_EXIT[i][ee_INT_INTERIOR], ENTER_EXIT[i][ee_INT_WORLD], true);
			StopAudioStreamForPlayer(playerid);
			break;
		}
	}
	
	SendFormatNotification(playerid, "Has cumplido tu condena.");
	return 1;
}

HidePlayerGpsMap(playerid)
{
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][13]);
	PLAYER_TEMP[playerid][pt_GPS_MAP] = false;
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_GPS_MAP][0]);
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_GPS_MAP][1]);
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_GPS_MAP][2]);
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_GPS_MAP][3]);
	
	for(new i = 0; i != MAX_PLAYER_MAP_GPS_POINTS; i ++)
	{
		if(PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][i] != PlayerText:INVALID_TEXT_DRAW)
		{
			PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][i]); 
			PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][i] = PlayerText:INVALID_TEXT_DRAW;
		}
	}
	return 1;
}

ShowPlayerGpsMap(playerid)
{
	PLAYER_TEMP[playerid][pt_GPS_MAP] = true;
	TextDrawShowForPlayer(playerid, Textdraws[textdraw_GPS_MAP][0]);
	TextDrawShowForPlayer(playerid, Textdraws[textdraw_GPS_MAP][2]);
	TextDrawShowForPlayer(playerid, Textdraws[textdraw_GPS_MAP][1]);
	
	UpdatePlayer_GPS_Map(playerid);
	
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][13]);
	PLAYER_TEMP[playerid][pt_TIMERS][13] = SetTimerEx("UpdatePlayer_GPS_Map", 1000, true, "i", playerid);
	return 1;
}

RegisterPhoneMessage(from_id, to_id, message[], offline = 0)
{
	new date[24], from_id_insert[24] = "NULL", to_id_insert[24] = "NULL";
	getDateTime(date);
	if(from_id) format(from_id_insert, 24, "%d", from_id);
	if(to_id) format(to_id_insert, 24, "%d", to_id);

	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO pmessages (from_id, to_id, message, date, offline) VALUES (%s, %s, '%e', '%e', %d);", from_id_insert, to_id_insert, message, date, offline);
	mysql_tquery(srp_db, QUERY_BUFFER);
	return 1;
}

DisablePlayerTaxiMark(playerid)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
			{
				if(i == playerid) continue;
				if(GetPlayerState(i) != PLAYER_STATE_DRIVER) continue;
				if(!PLAYER_WORKS[i][WORK_TAXI][pwork_SET]) continue;
				if(PLAYER_TEMP[i][pt_WORKING_IN] != WORK_TAXI) continue;
				
				SetPlayerMarkerForPlayer(i, playerid, PLAYER_COLOR);
			}
		}
	}
	return 1;
}

EndPhoneCall(playerid)
{
	if(!PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL]) return 0;
	
	if(PLAYER_TEMP[playerid][pt_POLICE_CALL_NAME] || PLAYER_TEMP[playerid][pt_POLICE_CALL_DESCRIPTION])
	{
		PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL] = false;
		PLAYER_TEMP[playerid][pt_POLICE_CALL_NAME] = false;
		PLAYER_TEMP[playerid][pt_POLICE_CALL_DESCRIPTION] = false;
		return 1;
	}
	
	if(PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID] == INVALID_PLAYER_ID) return 0;
	if(PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_STATE] == CALL_GAME_STATE_NONE) return 0;
	
	new to_playerid = PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID];
	switch(PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_STATE])
	{
		case CALL_STATE_WAITING_RESPONSE:
		{
			KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][6]);
			
			PLAYER_TEMP[to_playerid][pt_PLAYER_IN_CALL] = false;
			PLAYER_TEMP[to_playerid][pt_PLAYER_PHONE_CALL_STATE] = CALL_GAME_STATE_NONE;
			PLAYER_TEMP[to_playerid][pt_PLAYER_PHONE_CALL_PLAYERID] = INVALID_PLAYER_ID;
		}
		case CALL_STATE_INCOMING_CALL:
		{
			SendClientMessage(to_playerid, -1, "{"#SILVER_COLOR"}No respondieron el teléfono...");
			KillTimer(PLAYER_TEMP[to_playerid][pt_TIMERS][6]);
			
			PLAYER_TEMP[to_playerid][pt_PLAYER_IN_CALL] = false;
			PLAYER_TEMP[to_playerid][pt_PLAYER_PHONE_CALL_STATE] = CALL_GAME_STATE_NONE;
			PLAYER_TEMP[to_playerid][pt_PLAYER_PHONE_CALL_PLAYERID] = INVALID_PLAYER_ID;
		}
		case CALL_STATE_ESTABLISHED:
		{
			SendClientMessage(to_playerid, -1, "{"#PRIMARY_COLOR"}Llamada finalizada.");
			PLAYER_TEMP[to_playerid][pt_PLAYER_IN_CALL] = false;
			PLAYER_TEMP[to_playerid][pt_PLAYER_PHONE_CALL_STATE] = CALL_GAME_STATE_NONE;
			PLAYER_TEMP[to_playerid][pt_PLAYER_PHONE_CALL_PLAYERID] = INVALID_PLAYER_ID;
			
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
			SetPlayerSpecialAction(to_playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
		}
	}
	
	PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL] = false;
	PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_STATE] = CALL_GAME_STATE_NONE;
	PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID] = INVALID_PLAYER_ID;
	return 1;
}

GetBuyPropertySiteIndexByIntTyp(interior_type)
{
	for(new i = 0; i != sizeof BUY_PROPERTIES_SITES; i ++)
	{
		if(BUY_PROPERTIES_SITES[i][site_INTERIOR_TYPE] == interior_type) return i;
	}
	return -1;
}

GetBankLocalIndexByIntType(interior_type)
{
	for(new i = 0; i != sizeof Bank_Interior_Positions; i ++)
	{
		if(Bank_Interior_Positions[i][bank_INTERIOR_TYPE] == interior_type) return i;
	}
	return -1;
}

forward UpdatePlayer_GPS_Map(playerid);
public UpdatePlayer_GPS_Map(playerid)
{
	///
	//	0 - playerpos
	//	1 - cp gps
	//	2 - cp works
	//	3 > cp works
	//	
	//
	if(!PLAYER_TEMP[playerid][pt_GPS_MAP]) return KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][13]);
	
	new Float:pos[3];
	
	switch(ACCOUNT_INFO[playerid][ac_STATE])
	{
		case ROLEPLAY_STATE_INTERIOR:
		{
			pos[0] = ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_EXT_X];
			pos[1] = ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_EXT_Y];
		}
		case ROLEPLAY_STATE_OWN_PROPERTY, ROLEPLAY_STATE_GUEST_PROPERTY:
		{
			pos[0] = PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] ][property_EXT_X];
			pos[1] = PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] ][property_EXT_Y];
		}
		default: GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	}
	SetPlayerPoint_GPS_Map(0, playerid, "hud:radar_waypoint", -1, 5.0, 5.0, pos[0], pos[1]);
	
	if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT]))
	{
		new Float:gps_cp_pos[2]; 
		Streamer_GetFloatData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT], E_STREAMER_X, gps_cp_pos[0]);
		Streamer_GetFloatData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT], E_STREAMER_Y, gps_cp_pos[1]);
		SetPlayerPoint_GPS_Map(1, playerid, "LD_SPAC:white", 0xaa0000ff, 3.0, 3.0, gps_cp_pos[0], gps_cp_pos[1]);
	}
	else 
	{
		if(PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][1] != PlayerText:INVALID_TEXT_DRAW)
		{
			PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][1]); 
			PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][1] = PlayerText:INVALID_TEXT_DRAW;
		}
	}
	
	switch(PLAYER_TEMP[playerid][pt_WORKING_IN])
	{
		case WORK_NONE:
		{
			if(PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] != PlayerText:INVALID_TEXT_DRAW)
			{
				PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2]); 
				PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] = PlayerText:INVALID_TEXT_DRAW;
			}
		}
		case WORK_TRUCK, WORK_DELIVERYMAN:
		{
			if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT]))
			{
				new Float:gps_cp_pos[2]; 
				Streamer_GetFloatData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT], E_STREAMER_X, gps_cp_pos[0]);
				Streamer_GetFloatData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT], E_STREAMER_Y, gps_cp_pos[1]);
				SetPlayerPoint_GPS_Map(2, playerid, "LD_SPAC:white", 0xaa0000ff, 3.0, 3.0, gps_cp_pos[0], gps_cp_pos[1]);
			}
			else 
			{
				if(PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] != PlayerText:INVALID_TEXT_DRAW)
				{
					PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2]); 
					PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] = PlayerText:INVALID_TEXT_DRAW;
				}
			}
		}
		case WORK_HARVESTER:
		{
			if(IsValidDynamicRaceCP(PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT]))
			{
				new Float:gps_cp_pos[2]; 
				Streamer_GetFloatData(STREAMER_TYPE_RACE_CP, PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT], E_STREAMER_X, gps_cp_pos[0]);
				Streamer_GetFloatData(STREAMER_TYPE_RACE_CP, PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT], E_STREAMER_Y, gps_cp_pos[1]);
				SetPlayerPoint_GPS_Map(2, playerid, "LD_SPAC:white", 0xaa0000ff, 3.0, 3.0, gps_cp_pos[0], gps_cp_pos[1]);
			}
			else 
			{
				if(PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] != PlayerText:INVALID_TEXT_DRAW)
				{
					PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2]); 
					PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] = PlayerText:INVALID_TEXT_DRAW;
				}
			}
		}
		case WORK_FUMIGATOR:
		{
			if(IsValidDynamicRaceCP(PLAYER_TEMP[playerid][pt_FUMIGATOR_CHECKPOINT]))
			{
				new Float:gps_cp_pos[2]; 
				Streamer_GetFloatData(STREAMER_TYPE_RACE_CP, PLAYER_TEMP[playerid][pt_FUMIGATOR_CHECKPOINT], E_STREAMER_X, gps_cp_pos[0]);
				Streamer_GetFloatData(STREAMER_TYPE_RACE_CP, PLAYER_TEMP[playerid][pt_FUMIGATOR_CHECKPOINT], E_STREAMER_Y, gps_cp_pos[1]);
				SetPlayerPoint_GPS_Map(2, playerid, "LD_SPAC:white", 0xaa0000ff, 3.0, 3.0, gps_cp_pos[0], gps_cp_pos[1]);
			}
			else 
			{
				if(PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] != PlayerText:INVALID_TEXT_DRAW)
				{
					PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2]); 
					PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] = PlayerText:INVALID_TEXT_DRAW;
				}
			}
		}
		case WORK_TRASH:
		{
			if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT]))
			{
				new Float:gps_cp_pos[2]; 
				Streamer_GetFloatData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT], E_STREAMER_X, gps_cp_pos[0]);
				Streamer_GetFloatData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT], E_STREAMER_Y, gps_cp_pos[1]);
				SetPlayerPoint_GPS_Map(2, playerid, "LD_SPAC:white", 0xaa0000ff, 3.0, 3.0, gps_cp_pos[0], gps_cp_pos[1]);
			}
			else 
			{
				if(PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] != PlayerText:INVALID_TEXT_DRAW)
				{
					PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2]); 
					PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] = PlayerText:INVALID_TEXT_DRAW;
				}
			}
		}
		case WORK_LUMBERJACK:
		{
			if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT]))
			{
				new Float:gps_cp_pos[2]; 
				Streamer_GetFloatData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT], E_STREAMER_X, gps_cp_pos[0]);
				Streamer_GetFloatData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT], E_STREAMER_Y, gps_cp_pos[1]);
				SetPlayerPoint_GPS_Map(2, playerid, "LD_SPAC:white", 0xaa0000ff, 3.0, 3.0, gps_cp_pos[0], gps_cp_pos[1]);
			}
			else 
			{
				if(PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] != PlayerText:INVALID_TEXT_DRAW)
				{
					PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2]); 
					PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] = PlayerText:INVALID_TEXT_DRAW;
				}
			}
		}
		case WORK_PIZZA:
		{
			if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]))
			{
				new Float:gps_cp_pos[2]; 
				Streamer_GetFloatData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT], E_STREAMER_X, gps_cp_pos[0]);
				Streamer_GetFloatData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT], E_STREAMER_Y, gps_cp_pos[1]);
				SetPlayerPoint_GPS_Map(2, playerid, "LD_SPAC:white", 0xaa0000ff, 3.0, 3.0, gps_cp_pos[0], gps_cp_pos[1]);
			}
			else 
			{
				if(PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] != PlayerText:INVALID_TEXT_DRAW)
				{
					PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2]); 
					PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][2] = PlayerText:INVALID_TEXT_DRAW;
				}
			}
		}
	}
	
	new cp_counts = 3;
	for(new d = cp_counts; d != MAX_PLAYER_MAP_GPS_POINTS; d ++)
	{
		if(PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][d] != PlayerText:INVALID_TEXT_DRAW)
		{
			PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][d]); 
			PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][d] = PlayerText:INVALID_TEXT_DRAW;
		}
	}
	
	if(cp_counts >= MAX_PLAYER_MAP_GPS_POINTS) return 1;
	switch(PLAYER_TEMP[playerid][pt_WORKING_IN])
	{
		case WORK_TAXI:
		{
			for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
			{
				if(IsPlayerConnected(i))
				{
					if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
					{
						if(PLAYER_TEMP[i][pt_WANT_TAXI])
						{
							if(cp_counts >= MAX_PLAYER_MAP_GPS_POINTS) break;
							
							new Float:player_pos[3];
							switch(ACCOUNT_INFO[i][ac_STATE])
							{
								case ROLEPLAY_STATE_INTERIOR:
								{
									player_pos[0] = ENTER_EXIT[ PLAYER_TEMP[i][pt_INTERIOR_INDEX] ][ee_EXT_X];
									player_pos[1] = ENTER_EXIT[ PLAYER_TEMP[i][pt_INTERIOR_INDEX] ][ee_EXT_Y];
								}
								case ROLEPLAY_STATE_OWN_PROPERTY, ROLEPLAY_STATE_GUEST_PROPERTY:
								{
									player_pos[0] = PROPERTY_INFO[ PLAYER_TEMP[i][pt_PROPERTY_INDEX] ][property_EXT_X];
									player_pos[1] = PROPERTY_INFO[ PLAYER_TEMP[i][pt_PROPERTY_INDEX] ][property_EXT_Y];
								}
								default: GetPlayerPos(i, player_pos[0], player_pos[1], player_pos[2]);
							}
							SetPlayerPoint_GPS_Map(cp_counts, playerid, "LD_SPAC:white", 0xf2f224FF, 3.0, 3.0, player_pos[0], player_pos[1]);
							cp_counts ++;
						}
					}
				}
			}
		}
		case WORK_POLICE:
		{
			for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
			{
				if(IsPlayerConnected(i))
				{
					if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
					{
						if(PI[i][ac_WANTED_LEVEL] > 0)
						{
							if(cp_counts >= MAX_PLAYER_MAP_GPS_POINTS) break;

							new Float:player_pos[3];
							switch(ACCOUNT_INFO[i][ac_STATE])
							{
								case ROLEPLAY_STATE_INTERIOR:
								{
									if(PLAYER_TEMP[i][pt_INTERIOR_INDEX] == -1)
									{
										printf("[debug] Player id %d tiene interior index -1 estando en ROLEPLAY_STATE_INTERIOR");
										PrintBacktrace();
										continue;
									}
									player_pos[0] = ENTER_EXIT[ PLAYER_TEMP[i][pt_INTERIOR_INDEX] ][ee_EXT_X];
									player_pos[1] = ENTER_EXIT[ PLAYER_TEMP[i][pt_INTERIOR_INDEX] ][ee_EXT_Y];
								}
								case ROLEPLAY_STATE_OWN_PROPERTY, ROLEPLAY_STATE_GUEST_PROPERTY:
								{
									if(PLAYER_TEMP[i][pt_PROPERTY_INDEX] == -1)
									{
										printf("[debug] Player id %d tiene property index -1 estando en pt_PROPERTY_INDEX");
										PrintBacktrace();
										continue;
									}
									player_pos[0] = PROPERTY_INFO[ PLAYER_TEMP[i][pt_PROPERTY_INDEX] ][property_EXT_X];
									player_pos[1] = PROPERTY_INFO[ PLAYER_TEMP[i][pt_PROPERTY_INDEX] ][property_EXT_Y];
								}
								default: GetPlayerPos(i, player_pos[0], player_pos[1], player_pos[2]);
							}
							SetPlayerPoint_GPS_Map(cp_counts, playerid, "LD_SPAC:white", PlayerWantedColor(i), 3.0, 3.0, player_pos[0], player_pos[1]);
							cp_counts ++;
						}
					}
				}
			}
		}
		case WORK_MEDIC:
		{
			for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
			{
				if(IsPlayerConnected(i))
				{
					if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
					{
						if(ACCOUNT_INFO[i][ac_STATE] == ROLEPLAY_STATE_CRACK)
						{
							if(cp_counts >= MAX_PLAYER_MAP_GPS_POINTS) break;

							new Float:player_pos[3];
							GetPlayerPos(i, player_pos[0], player_pos[1], player_pos[2]);
							SetPlayerPoint_GPS_Map(cp_counts, playerid, "LD_SPAC:white", 0x4aba00FF, 3.0, 3.0, player_pos[0], player_pos[1]);
							cp_counts ++;
						}
					}
				}
			}
		}
	}
	return 1;
}

UpdateClothingShop(playerid)
{
	if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] == -1) return 0;
	
	new cost, textdraw_str[128];
	PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][1], Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_NAME]);
	
	switch(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE])
	{
		case INTERIOR_BINCO:
		{
			switch(PI[playerid][ac_GENDER])
			{
				case SEX_MALE:
				{
					SetPlayerSkin(playerid, Binco_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][0]);
					cost = Binco_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][2];
					
					if(PI[playerid][ac_CASH] < cost) PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -1459617611);
					else PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 9306312);
					
					format(textdraw_str, sizeof textdraw_str, "Precio:_%s$~n~Ropa:_%d/%d", number_format_thousand(cost), PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] + 1, sizeof(Binco_Shop_Male_Skins));
					PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], textdraw_str);
				}
				case SEX_FEMALE:
				{
					SetPlayerSkin(playerid, Binco_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][0]);
					cost = Binco_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][2];
					
					if(PI[playerid][ac_CASH] < cost) PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -1459617611);
					else PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 9306312);
					
					format(textdraw_str, sizeof textdraw_str, "Precio:_%s$~n~Ropa:_%d/%d", number_format_thousand(cost), PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] + 1, sizeof(Binco_Shop_Female_Skins));
					PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], textdraw_str);
				}
			}
		}
		case INTERIOR_SUBURBAN:
		{
			switch(PI[playerid][ac_GENDER])
			{
				case SEX_MALE:
				{
					SetPlayerSkin(playerid, Suburban_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][0]);
					cost = Suburban_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][2];
					
					if(PI[playerid][ac_CASH] < cost) PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -1459617611);
					else PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 9306312);
					
					format(textdraw_str, sizeof textdraw_str, "Precio:_%s$~n~Ropa:_%d/%d", number_format_thousand(cost), PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] + 1, sizeof(Suburban_Shop_Male_Skins));
					PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], textdraw_str);
				}
				case SEX_FEMALE:
				{
					SetPlayerSkin(playerid, Suburban_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][0]);
					cost = Suburban_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][2];
					
					if(PI[playerid][ac_CASH] < cost) PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -1459617611);
					else PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 9306312);
					
					format(textdraw_str, sizeof textdraw_str, "Precio:_%s$~n~Ropa:_%d/%d", number_format_thousand(cost), PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] + 1, sizeof(Suburban_Shop_Female_Skins));
					PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], textdraw_str);
				}
			}
		}
		case INTERIOR_PROLAPS:
		{
			switch(PI[playerid][ac_GENDER])
			{
				case SEX_MALE:
				{
					SetPlayerSkin(playerid, Prolaps_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][0]);
					cost = Prolaps_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][2];
					
					if(PI[playerid][ac_CASH] < cost) PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -1459617611);
					else PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 9306312);
					
					format(textdraw_str, sizeof textdraw_str, "Precio:_%s$~n~Ropa:_%d/%d", number_format_thousand(cost), PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] + 1, sizeof(Prolaps_Shop_Male_Skins));
					PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], textdraw_str);
				}
				case SEX_FEMALE:
				{
					SetPlayerSkin(playerid, Prolaps_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][0]);
					cost = Prolaps_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][2];
					
					if(PI[playerid][ac_CASH] < cost) PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -1459617611);
					else PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 9306312);
					
					format(textdraw_str, sizeof textdraw_str, "Precio:_%s$~n~Ropa:_%d/%d", number_format_thousand(cost), PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] + 1, sizeof(Prolaps_Shop_Female_Skins));
					PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], textdraw_str);
				}
			}
		}
		case INTERIOR_DIDIER_SACHS:
		{
			switch(PI[playerid][ac_GENDER])
			{
				case SEX_MALE:
				{
					SetPlayerSkin(playerid, Didier_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][0]);
					cost = Didier_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][2];
					
					if(PI[playerid][ac_CASH] < cost) PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -1459617611);
					else PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 9306312);
					
					format(textdraw_str, sizeof textdraw_str, "Precio:_%s$~n~Ropa:_%d/%d", number_format_thousand(cost), PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] + 1, sizeof(Didier_Shop_Male_Skins));
					PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], textdraw_str);
				}
				case SEX_FEMALE:
				{
					SetPlayerSkin(playerid, Didier_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][0]);
					cost = Didier_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][2];
					
					if(PI[playerid][ac_CASH] < cost) PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -1459617611);
					else PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 9306312);
					
					format(textdraw_str, sizeof textdraw_str, "Precio:_%s$~n~Ropa:_%d/%d", number_format_thousand(cost), PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] + 1, sizeof(Didier_Shop_Female_Skins));
					PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], textdraw_str);
				}
			}
		}
		case INTERIOR_VICTIM:
		{
			switch(PI[playerid][ac_GENDER])
			{
				case SEX_MALE:
				{
					SetPlayerSkin(playerid, Victim_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][0]);
					cost = Victim_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][2];
					
					if(PI[playerid][ac_CASH] < cost) PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -1459617611);
					else PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 9306312);
					
					format(textdraw_str, sizeof textdraw_str, "Precio:_%s$~n~Ropa:_%d/%d", number_format_thousand(cost), PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] + 1, sizeof(Victim_Shop_Male_Skins));
					PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], textdraw_str);
				}
				case SEX_FEMALE:
				{
					SetPlayerSkin(playerid, Victim_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][0]);
					cost = Victim_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][2];
					
					if(PI[playerid][ac_CASH] < cost) PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -1459617611);
					else PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 9306312);
					
					format(textdraw_str, sizeof textdraw_str, "Precio:_%s$~n~Ropa:_%d/%d", number_format_thousand(cost), PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] + 1, sizeof(Victim_Shop_Female_Skins));
					PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], textdraw_str);
				}
			}
		}
		case INTERIOR_ZIP:
		{
			switch(PI[playerid][ac_GENDER])
			{
				case SEX_MALE:
				{
					SetPlayerSkin(playerid, Zip_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][0]);
					cost = Zip_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][2];
					
					if(PI[playerid][ac_CASH] < cost) PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -1459617611);
					else PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 9306312);
					
					format(textdraw_str, sizeof textdraw_str, "Precio:_%s$~n~Ropa:_%d/%d", number_format_thousand(cost), PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] + 1, sizeof(Zip_Shop_Male_Skins));
					PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], textdraw_str);
				}
				case SEX_FEMALE:
				{
					SetPlayerSkin(playerid, Zip_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][0]);
					cost = Zip_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]][2];
					
					if(PI[playerid][ac_CASH] < cost) PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -1459617611);
					else PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 9306312);
					
					format(textdraw_str, sizeof textdraw_str, "Precio:_%s$~n~Ropa:_%d/%d", number_format_thousand(cost), PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] + 1, sizeof(Zip_Shop_Female_Skins));
					PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], textdraw_str);
				}
			}
		}
	}
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2]);
	return 1;
}

UpdateToysShop(playerid)
{
	if(!PLAYER_TEMP[playerid][pt_TOYS_SHOP]) return 0;
	
	PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1], ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_NAME]);
	PlayerTextDrawSetPreviewModel(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3], Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_modelid]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3]);
	
	new td_str[128]; format(td_str, sizeof td_str, "Modelo:_%s~n~Precio:_%s$", Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_model], number_format_thousand( Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_price] ));
	PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][4], td_str);
	
	
	if(PI[playerid][ac_CASH] < Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_price]) PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], -900267777);
	else PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], 1855979775);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5]);
	
	format(td_str, sizeof td_str, "%d/%d", PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] + 1, sizeof(Toys_Shop));
	PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][8], td_str);
	return 1;
}

GetPlayerFastFoodIndex(playerid)
{
	for(new i = 0; i != sizeof Fast_Food_Positions; i ++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 1.0, Fast_Food_Positions[i][fast_food_X], Fast_Food_Positions[i][fast_food_Y], Fast_Food_Positions[i][fast_food_Z]))
		{
			return i;
		}
	}
	return -10;
}

SendAlertToTaxiDrivers(playerid)
{
	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
			{
				if(i == playerid) continue;
				if(GetPlayerState(i) != PLAYER_STATE_DRIVER) continue;
				if(!PLAYER_WORKS[i][WORK_TAXI][pwork_SET]) continue;
				if(PLAYER_TEMP[i][pt_WORKING_IN] != WORK_TAXI) continue;
				
				SetPlayerMarkerForPlayer(i, playerid, 0xf2f224FF);
				SendClientMessageEx(i, -1, "Hay un nuevo cliente solicitando un taxi, distancia: {"#PRIMARY_COLOR"}%.2f Km.", (GetPlayerDistanceFromPoint(i, pos[0], pos[1], pos[2]) * 0.01));
			}
		}
	}
	return 1;
}

TransferPlayerPocketObject(from_playerid, from_slot, to_playerid, to_slot)
{
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pfoods SET id_player = %d WHERE id = %d;", ACCOUNT_INFO[to_playerid][ac_ID], PLAYER_POCKET[from_playerid][from_slot][player_pocket_object_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	
	PLAYER_POCKET[to_playerid][to_slot][player_pocket_VALID] = true;
	PLAYER_POCKET[to_playerid][to_slot][player_pocket_object_ID] = PLAYER_POCKET[from_playerid][from_slot][player_pocket_object_ID];
	format(PLAYER_POCKET[to_playerid][to_slot][player_pocket_object_NAME], 24, "%s", PLAYER_POCKET[from_playerid][from_slot][player_pocket_object_NAME]);
	PLAYER_POCKET[to_playerid][to_slot][player_pocket_object_HUNGRY] = PLAYER_POCKET[from_playerid][from_slot][player_pocket_object_HUNGRY];
	PLAYER_POCKET[to_playerid][to_slot][player_pocket_object_THIRST] = PLAYER_POCKET[from_playerid][from_slot][player_pocket_object_THIRST];
	PLAYER_POCKET[to_playerid][to_slot][player_pocket_object_DRUNK] = PLAYER_POCKET[from_playerid][from_slot][player_pocket_object_DRUNK];
	
	PLAYER_POCKET[from_playerid][from_slot][player_pocket_VALID] = false;
	PLAYER_POCKET[from_playerid][from_slot][player_pocket_object_ID] = 0;
	format(PLAYER_POCKET[from_playerid][from_slot][player_pocket_object_NAME], 24, "");
	PLAYER_POCKET[from_playerid][from_slot][player_pocket_object_HUNGRY] = 0.0;
	PLAYER_POCKET[from_playerid][from_slot][player_pocket_object_THIRST] = 0.0;
	PLAYER_POCKET[from_playerid][from_slot][player_pocket_object_DRUNK] = 0;
	return 1;
}

SetPlayerPoint_GPS_Map(index, playerid, const icon[], color, Float:icon_size_X, Float:icon_size_Y, Float:x, Float:y)
{
	new Float:td_X, Float:td_Y,
		Float:converted_MAP_SIZE_X = floatdiv(map_td_SIZE_X, 2),
		Float:converted_MAP_SIZE_Y = floatdiv(map_td_SIZE_Y, 2),
		Float:converted_ICON_SIZE_X = floatdiv(icon_size_X, 2),
		Float:converted_ICON_SIZE_Y = floatdiv(icon_size_Y, 2);
	
	if(x > 3000.0) x = 3000.0;
	else if(x < -3000.0) x = -3000.0;
	
	if(y > 3000.0) y = 3000.0;
	else if(y < -3000.0) y = -3000.0;

	td_X = map_td_X + floatmul(floatdiv(converted_MAP_SIZE_X, 3000), x) + converted_MAP_SIZE_X - converted_ICON_SIZE_X;
	td_Y = map_td_Y + floatmul(floatdiv(-converted_MAP_SIZE_Y, 3000), y) + converted_MAP_SIZE_Y- converted_ICON_SIZE_Y;

	if(PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index]);
		PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index] = PlayerText:INVALID_TEXT_DRAW;
	}

	PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index] = CreatePlayerTextDraw(playerid, td_X, td_Y, icon);
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index], 0.160333, 1.280592);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index], icon_size_X, icon_size_Y);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index], color);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index], 4);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index], 0);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index], 0);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_PLAYER_GPS_MAP][index]);
	return 1;
}

ShowClothingShopTextdraws(playerid)
{
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][0]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][1]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4]);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5]);
	return 1;
}

GetEmptyPlayerPocketSlot(playerid)
{
	for(new i = 0; i != MAX_PLAYER_POCKET_OBJECTS; i ++)
	{
		if(!PLAYER_POCKET[playerid][i][player_pocket_VALID]) return i;
	}
	return -1;
}

Add_Hungry_Thirst(playerid, Float:hungry, Float:thirst)
{
	PI[playerid][ac_HUNGRY] += hungry;
	if(PI[playerid][ac_HUNGRY] > 30.0) PLAYER_TEMP[playerid][pt_HUNGRY_MESSAGE] = false;
	if(PI[playerid][ac_HUNGRY] > 100.0) PI[playerid][ac_HUNGRY] = 100.0;
	
	PI[playerid][ac_THIRST] += thirst;
	if(PI[playerid][ac_THIRST] > 30.0) PLAYER_TEMP[playerid][pt_THIRST_MESSAGE] = false;
	if(PI[playerid][ac_THIRST] > 100.0) PI[playerid][ac_THIRST] = 100.0;
	
	UpdatePlayerHud(playerid);
	return 1;
}

GivePlayerCash(playerid, amount, bool:update = true, bool:negative = false)
{
	if(amount == 0) return 1;
	if(!negative && amount < 0) return 0;
	if(negative && amount > 0) return 0;
	if(negative && (PI[playerid][ac_CASH] + amount) < 0) return 0;

	PI[playerid][ac_CASH] += amount;
	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, PI[playerid][ac_CASH]);
	
	if(update)
	{
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET cash = %d WHERE id = %d;", PI[playerid][ac_CASH], ACCOUNT_INFO[playerid][ac_ID]);
		mysql_tquery(srp_db, QUERY_BUFFER);
	}
	return 1;
}

DeletePlayerPocketObject(playerid, slot)
{
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM pfoods WHERE id = %d;", PLAYER_POCKET[playerid][slot][player_pocket_object_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	
	PLAYER_POCKET[playerid][slot][player_pocket_VALID] = false;
	PLAYER_POCKET[playerid][slot][player_pocket_object_ID] = 0;
	format(PLAYER_POCKET[playerid][slot][player_pocket_object_NAME], 24, "");
	PLAYER_POCKET[playerid][slot][player_pocket_object_HUNGRY] = 0.0;
	PLAYER_POCKET[playerid][slot][player_pocket_object_THIRST] = 0.0;
	PLAYER_POCKET[playerid][slot][player_pocket_object_DRUNK] = 0;
	return 1;
}

GivePlayerDrunkLevel(playerid, amount)
{
	return SetPlayerDrunkLevel(playerid, GetPlayerDrunkLevel(playerid) + amount);
}

CancelSelectTextDrawEx(playerid)
{
	PLAYER_TEMP[playerid][pt_SELECT_TEXTDRAW] = false;
	return CancelSelectTextDraw(playerid);
}

SetRolePlayNames(playerid)
{
	if(isnull(PLAYER_TEMP[playerid][pt_NAME])) return 0;
	
	new name[24], bool:underscore;
	format(name, 24, "%s", PLAYER_TEMP[playerid][pt_NAME]);
	format(PLAYER_TEMP[playerid][pt_RP_NAME], 24, "%s", name);
	for(new i = 0; i < 24; i++) 
	{
		if(name[i] == '_')
		{
			PLAYER_TEMP[playerid][pt_RP_NAME][i] = ' ';
			if(!underscore)
			{
				strmid(PLAYER_TEMP[playerid][pt_FIRST_NAME], name, 0, i);
				strmid(PLAYER_TEMP[playerid][pt_SUB_NAME], name, i + 1, 24);
				underscore = true;
			}
		}
	}
	return 1;
}

CreatePlayerTextDraws(playerid)
{
	for(new i; i < sizeof(PlayerTextdraws[]); i++) PlayerTextdraws[playerid][Player_Textdraws_Enum:i] = INVALID_TEXT_DRAW;

 	MensajeTD[playerid] = CreatePlayerTextDraw(playerid, 320.000000, 365.000000, "_");
	PlayerTextDrawLetterSize(playerid, MensajeTD[playerid], 0.298333, 1.976000);
	PlayerTextDrawAlignment(playerid, MensajeTD[playerid], 2);
	PlayerTextDrawColor(playerid,MensajeTD[playerid], -1);
	PlayerTextDrawSetShadow(playerid, MensajeTD[playerid], 0);
	PlayerTextDrawSetOutline(playerid, MensajeTD[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, MensajeTD[playerid], 255);
	PlayerTextDrawFont(playerid, MensajeTD[playerid], 1);
	PlayerTextDrawSetProportional(playerid, MensajeTD[playerid], 1);
	PlayerTextDrawSetShadow(playerid, MensajeTD[playerid], 1);

	PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0] = CreatePlayerTextDraw(playerid, 320.000000, 380.000000, "_");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0], 0.400000, 1.600000);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0], 0.000000, 100.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0], 255);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0], 0);

	PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1] = CreatePlayerTextDraw(playerid, 272.000000, 382.000000, "_");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1], 0.400000, 1.127111);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1], 368.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1], -16777123);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1], 0);
	
	PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2] = CreatePlayerTextDraw(playerid, 272.000000, 382.000000, "_");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2], 0.400000, 1.127111);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2], 270.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2], -16777123);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2], 0);
	
	
	PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][0] = CreatePlayerTextDraw(playerid, 320.000000, 350.000000, "I");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][0], 27.410259, 5.897183);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][0], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][0], 145);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][0], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][0], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][0], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][0], 2);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][0], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][0], 0);

	PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][1] = CreatePlayerTextDraw(playerid, 320.000000, 358.000000, "Tienda");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][1], 0.361666, 1.400889);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][1], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][1], -1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][1], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][1], 1);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][1], 90);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][1], 0);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][1], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][1], 0);

	PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2] = CreatePlayerTextDraw(playerid, 320.000000, 377.000000, "COMPRAR");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 0.193666, 0.915556);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 10.000000, 34.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], -201); // Money = 9306312   ,  -1459617611  No money
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2], true);

	PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3] = CreatePlayerTextDraw(playerid, 363.000000, 377.000000, "SIGUIENTE");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3], 0.193666, 0.915556);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3], 10.000000, 34.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3], -132);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3], true);

	PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4] = CreatePlayerTextDraw(playerid, 277.000000, 377.000000, "ANTERIOR");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4], 0.193666, 0.915556);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4], 10.000000, 34.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4], -132);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4], true);

	PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5] = CreatePlayerTextDraw(playerid, 320.000000, 398.000000, "Precio:_0$~n~Ropa:_0/0");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], 0.171666, 0.782815);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], -86);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5], 0);
	
	PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0] = CreatePlayerTextDraw(playerid, 320.000000, 298.000000, "l");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0], 33.579376, 12.750882);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0], 172);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0], 0);

	PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1] = CreatePlayerTextDraw(playerid, 320.000000, 316.000000, "Ropa_del_personaje");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1], 0.400000, 1.600000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1], -81);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1], 0);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1], 0);

	PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2] = CreatePlayerTextDraw(playerid, 290.000000, 340.000000, "_");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], 0.221999, 1.558518);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], 0.000000, 50.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], -256); // seleccionado -94);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], 2);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], 0);

	PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3] = CreatePlayerTextDraw(playerid, 290.000000, 342.000000, "hombre");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3], 0.221999, 1.147852);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3], 13.000000, 47.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3], -1936945921);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3], 2);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3], true);

	PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4] = CreatePlayerTextDraw(playerid, 350.000000, 340.000000, "_");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], 0.221999, 1.558518);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], 0.000000, 50.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], -256);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], 2);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], 0);

	PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5] = CreatePlayerTextDraw(playerid, 350.000000, 342.000000, "mujer");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5], 0.221999, 1.147852);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5], 13.000000, 47.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5], -1936945921);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5], 2);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5], true);
	
	PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6] = CreatePlayerTextDraw(playerid, 320.000000, 365.000000, "continuar");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6], 0.221999, 1.147852);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6], 13.000000, 50.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6], -1936945921);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6], 2);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6], true);

	PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7] = CreatePlayerTextDraw(playerid, 362.000000, 365.000000, ">>");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7], 0.221999, 1.147852);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7], 13.000000, 23.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7], -1936945921);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7], 2);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7], true);

	PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8] = CreatePlayerTextDraw(playerid, 278.000000, 365.000000, "<<");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8], 0.221999, 1.147852);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8], 13.000000, 23.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8], -1936945921);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8], 2);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8], true);

	PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9] = CreatePlayerTextDraw(playerid, 320.000000, 380.000000, "0/0");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], 0.158000, 0.770370);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], -171);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], 0);

	
	PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0] = CreatePlayerTextDraw(playerid, 320.000000, 159.000000, "box");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0], 0.000000, 15.033352);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0], 0.000000, 240.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0], 1869574143);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0], 0);

	PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1] = CreatePlayerTextDraw(playerid, 320.000000, 156.000000, "Tienda");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1], 0.400000, 1.600000);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1], 0.000000, 240.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1], -1633916417);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1], 0);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1], 0);

	PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2] = CreatePlayerTextDraw(playerid, 215.000000, 184.000000, "");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2], 80.000000, 85.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2], -1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2], -19201);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2], 5);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2], 0);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2], 0);
	PlayerTextDrawSetPreviewModel(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2], 18864);
	PlayerTextDrawSetPreviewRot(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2], 344.000000, 0.000000, 109.000000, 1.000000);

	PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3] = CreatePlayerTextDraw(playerid, 217.000000, 186.000000, "");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3], 76.000000, 81.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3], -1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3], -926365441);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3], 5);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3], 0);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3], 0);
	PlayerTextDrawSetPreviewModel(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3], Toys_Shop[0][shop_toy_model]);
	PlayerTextDrawSetPreviewRot(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3], 344.000000, 0.000000, 109.000000, 1.000000);

	PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][4] = CreatePlayerTextDraw(playerid, 311.000000, 207.000000, "Modelo:__~n~Precio:__$");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][4], 0.219000, 1.081481);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][4], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][4], -1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][4], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][4], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][4], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][4], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][4], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][4], 0);

	PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5] = CreatePlayerTextDraw(playerid, 363.000000, 233.000000, "Comprar");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], 0.188333, 1.189333);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], 13.000000, 50.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], 1855979775); // sin dinero color = -900267777
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], 2);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5], true);

	PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6] = CreatePlayerTextDraw(playerid, 404.000000, 233.000000, ">>");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6], 0.188333, 1.189333);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6], 13.000000, 20.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6], -1094795521);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6], 2);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6], true);

	PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7] = CreatePlayerTextDraw(playerid, 322.000000, 233.000000, "<<");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7], 0.188333, 1.189333);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7], 13.000000, 20.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7], -1094795521);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7], 2);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7], true);

	PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][8] = CreatePlayerTextDraw(playerid, 255.000000, 271.000000, "0/0");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][8], 0.163666, 0.832593);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][8], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][8], -66);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][8], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][8], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][8], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][8], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][8], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][8], 0);

	PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9] = CreatePlayerTextDraw(playerid, 320.000000, 284.000000, "Accesorios");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9], 0.400000, 1.600000);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9], 0.000000, 240.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9], -1633916417);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9], 0);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9], 0);
	
	
	PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][0] = CreatePlayerTextDraw(playerid, 553.000000, 176.000000, "box");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][0], 0.000000, 8.766664);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][0], 0.000000, 108.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][0], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][0], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][0], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][0], 255);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][0], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][0], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][0], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][0], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][0], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][0], 0);

	PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][1] = CreatePlayerTextDraw(playerid, 505.000000, 181.000000, "Interior:_0/0");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][1], 0.300000, 1.438221);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][1], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][1], -1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][1], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][1], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][1], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][1], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][1], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][1], 0);

	PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2] = CreatePlayerTextDraw(playerid, 505.000000, 206.000000, "Anterior");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2], 0.300000, 1.438221);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2], 546.000000, 15.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2], 1684301055);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2], true);

	PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3] = CreatePlayerTextDraw(playerid, 555.000000, 206.000000, "Siguiente");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3], 0.300000, 1.438221);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3], 600.000000, 15.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3], 1684301055);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3], true);

	PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4] = CreatePlayerTextDraw(playerid, 505.000000, 230.000000, "Crear");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4], 0.300000, 1.438221);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4], 533.000000, 15.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4], 1684301055);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4], true);

	PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5] = CreatePlayerTextDraw(playerid, 542.000000, 230.000000, "Cancelar");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5], 0.300000, 1.438221);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5], 583.000000, 15.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5], 1684301055);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5], true);
	
	PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0] = CreatePlayerTextDraw(playerid, 320.000000, 317.000000, "Cargando_camión...");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0], 0.255666, 1.176888);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0], -1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0], 0);

	PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1] = CreatePlayerTextDraw(playerid, 250.000000, 332.000000, "Borde");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1], 0.000000, 3.766666);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1], 390.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1], 255);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1], 0);

	PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2] = CreatePlayerTextDraw(playerid, 253.000000, 336.000000, "Abajo");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2], 0.000000, 2.866667);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2], 387.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2], -594542436);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2], 0);

	PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3] = CreatePlayerTextDraw(playerid, 253.000000, 336.000000, "Sellena");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3], 0.000000, 2.866667);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3], 250.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3], -594542357);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3], 0);
	
	
	PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0] = CreatePlayerTextDraw(playerid, 320.000000, 315.000000, "");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0], 0.298999, 1.483852);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0], -161);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0], 0);

	PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1] = CreatePlayerTextDraw(playerid, 320.000000, 333.000000, "box");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1], 0.000000, 0.766669);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1], 0.000000, 130.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1], 2021161125);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1], 0);

	PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2] = CreatePlayerTextDraw(playerid, 320.000000, 333.000000, "box");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2], 0.000000, 0.766669);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2], 0.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2], 1988896255);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2], 0);

	PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0] = CreatePlayerTextDraw(playerid, 604.000000, 126.000000, "]]]]]]");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0], 0.491999, 1.960886);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0], 3);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0], 95);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0], 0);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0], 0);

	PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1] = CreatePlayerTextDraw(playerid, 604.000000, 126.000000, "_");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1], 0.491999, 1.960886);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1], 3);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1], -336860171);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1], 0);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1], 0);

	PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0] = CreatePlayerTextDraw(playerid, 275.000000, 325.000000, "+_Publicar_anuncio");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], 0.223666, 1.139557);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], 10.000000, 80.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], 190);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0], true);

	PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1] = CreatePlayerTextDraw(playerid, 365.000000, 325.000000, "-_Mis_anuncios");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], 0.223666, 1.139557);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], 10.000000, 80.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], 190);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1], true);

	PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2] = CreatePlayerTextDraw(playerid, 320.000000, 307.000000, "Cerrar");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], 0.223666, 1.139557);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], 10.000000, 50.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], 190);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2], true);

	PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3] = CreatePlayerTextDraw(playerid, 380.000000, 307.000000, "Siguiente");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], 0.223666, 1.139557);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], 10.000000, 50.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], 190);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3], true);

	PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4] = CreatePlayerTextDraw(playerid, 260.000000, 307.000000, "Anterior");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], 0.223666, 1.139557);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], 10.000000, 50.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], 190);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], 0);
	PlayerTextDrawSetSelectable(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4], true);
	
	PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE] = CreatePlayerTextDraw(playerid, 320.000000, 134.000000, "-_0/0_-");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE], 0.169000, 0.766222);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE], -1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLES_PAGE], 0);
	
	PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER] = CreatePlayerTextDraw(playerid, 320.000000, 152.000000, "");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER], 0.214666, 0.936296);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER], -1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_NUMBER], 0);

	PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO] = CreatePlayerTextDraw(playerid, 321.000000, 181.000000, "");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO], 0.259333, 1.181036);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO], -36);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_SHOP_ARTICLE_INFO], 0);
	
	PlayerTextdraws[playerid][ptextdraw_TUNING_SHOP] = CreatePlayerTextDraw(playerid, 320.000000, 350.000000, "Objeto:_0/0~n~Precio:_0$");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_TUNING_SHOP], 0.200333, 1.098074);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_TUNING_SHOP], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_TUNING_SHOP], -121);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TUNING_SHOP], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_TUNING_SHOP], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_TUNING_SHOP], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_TUNING_SHOP], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_TUNING_SHOP], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_TUNING_SHOP], 0);
	
	
	PlayerTextdraws[playerid][ptextdraw_MESSAGE] = CreatePlayerTextDraw(playerid, 320.000000, 365.000000, "_");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_MESSAGE], 0.298333, 1.376000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_MESSAGE], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_MESSAGE], -1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_MESSAGE], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_MESSAGE], 1);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_MESSAGE], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_MESSAGE], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_MESSAGE], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_MESSAGE], 0);
	
	
	PlayerTextdraws[playerid][ptextdraw_HUD][0] = CreatePlayerTextDraw(playerid, 0.000000, 446.000000, "barraReputacion");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], 0.000000, 0.500000);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], 0.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], 0xfce679FF);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], 0);

	PlayerTextdraws[playerid][ptextdraw_HUD][1] = CreatePlayerTextDraw(playerid, 500.699981, 104.000000, "barraHambre");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1], 0.000000, 0.238333);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1], 500.699981, 0.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1], 1923088639);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][1], 0);

	PlayerTextdraws[playerid][ptextdraw_HUD][2] = CreatePlayerTextDraw(playerid, 557.699981, 104.000000, "barraSed");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2], 0.000000, 0.238333);
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2], 557.699981, 0.000000);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2], 1);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2], -1);
	PlayerTextDrawUseBox(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2], 1);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2], 12124080);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2], 0);
	PlayerTextDrawSetOutline(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2], 1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][2], 0);

	PlayerTextdraws[playerid][ptextdraw_HUD][3] = CreatePlayerTextDraw(playerid, 320.000000, 433.000000, "Siguiente_nivel:");
	PlayerTextDrawLetterSize(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][3], 0.169666, 0.832592);
	PlayerTextDrawAlignment(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][3], 2);
	PlayerTextDrawColor(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][3], -1);
	PlayerTextDrawSetShadow(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][3], 0);
	PlayerTextDrawBackgroundColor(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][3], 255);
	PlayerTextDrawFont(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][3], 1);
	PlayerTextDrawSetProportional(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][3], 1);
	return 1;
}

DestroyPlayerCheckpoints(playerid)
{
	if(IsValidDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_POLICE_LABEL]))
	{
		DestroyDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_POLICE_LABEL]);
		PLAYER_TEMP[playerid][pt_POLICE_LABEL] = Text3D:INVALID_STREAMER_ID;
	}
	if(IsValidDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_ADMIN_LABEL]))
	{
		DestroyDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_ADMIN_LABEL]);
		PLAYER_TEMP[playerid][pt_ADMIN_LABEL] = Text3D:INVALID_STREAMER_ID;
	}
	if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT]))
	{
		DestroyDynamicCP(PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT] = INVALID_STREAMER_ID;
	}
	if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT]))
	{
		DestroyDynamicCP(PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT] = INVALID_STREAMER_ID;
	}
	if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT]))
	{
		DestroyDynamicCP(PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT] = INVALID_STREAMER_ID;
	}
	if(IsValidDynamicRaceCP(PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT]))
	{
		DestroyDynamicRaceCP(PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT] = INVALID_STREAMER_ID;
	}
	if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT]))
	{
		DestroyDynamicCP(PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT] = INVALID_STREAMER_ID;
	}
	if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]))
	{
		DestroyDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT] = INVALID_STREAMER_ID;
	}
	if(IsValidDynamicActor(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR]))
	{
		DestroyDynamicActor(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR]);
		PLAYER_TEMP[playerid][pt_PIZZA_ACTOR] = INVALID_STREAMER_ID;
	}
	if(IsValidDynamicRaceCP(PLAYER_TEMP[playerid][pt_DL_EXAM_CP]))
	{
		DestroyDynamicRaceCP(PLAYER_TEMP[playerid][pt_DL_EXAM_CP]);
		PLAYER_TEMP[playerid][pt_DL_EXAM_CP] = INVALID_STREAMER_ID;
	}
	return 1;
}

SetPlayerHud(playerid)
{
	PLAYER_TEMP[playerid][pt_HUD_TEXTDRAWS] = true;
	UpdatePlayerHud(playerid);
	UpdateReputationTextDraws(playerid);
	
	if(PI[playerid][ac_CONFIG_TIME]) TextDrawShowForPlayer(playerid, Textdraws[textdraw_SERVER_TIME]);
	
	if(PI[playerid][ac_CONFIG_HUD])
	{
		for(new i = 0; i < HUD_TEXTDRAWS_SIZE; i ++) {
			TextDrawShowForPlayer(playerid, Textdraws[textdraw_HUD][i]);	
		}
		
		for(new i = 0; i < HUD_PTEXTDRAWS_SIZE; i ++) {
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][i]);
		}
	}
	
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][5]);
	PLAYER_TEMP[playerid][pt_TIMERS][5] = SetTimerEx("HungryThirstDown", INTERVAL_HUNGRY_THIRST_DOWN * 1000, true, "i", playerid);
	return 1;
}

GetNearestHospitalForPlayer(playerid)
{
	new Float:distance = 99999.0,
        Float:tmp_distance,
        closest = -1; 

    for(new i = 0; i < sizeof Hospital_Spawn_Positions; i++) 
	{
		tmp_distance = GetPlayerDistanceFromPoint(playerid, Hospital_Spawn_Positions[i][hspi_X], Hospital_Spawn_Positions[i][hspi_Y], Hospital_Spawn_Positions[i][hspi_Z]); 
		if (tmp_distance < distance) 
		{ 
			distance = tmp_distance; 
			closest = i;
		}
	}

	if(closest == -1) closest = 1;
	return closest;
}

TogglePlayerSpectatingEx(playerid, bool:spectate)
{
	PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_SPECTATE][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_TEMP[playerid][pt_PLAYER_SPECTATE] = spectate;
	return TogglePlayerSpectating(playerid, spectate);
}
DestroyPlayerTextDraws(playerid)
{
	for(new i; i < sizeof(PlayerTextdraws[]); i++)
	{
		if(PlayerTextdraws[playerid][Player_Textdraws_Enum:i] == INVALID_TEXT_DRAW) continue;
		PlayerTextDrawDestroy(playerid, PlayerText:PlayerTextdraws[playerid][Player_Textdraws_Enum:i]);
		PlayerTextdraws[playerid][Player_Textdraws_Enum:i] = INVALID_TEXT_DRAW;
	}
	return 1;
}
////////////////////impls
GetPlayerVehicleAvaibleBootSlot(vehicleid)
{
	new max_slots = VEHICLE_INFO[GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_BOOT_SLOTS];
	if(max_slots > MAX_BOOT_SLOTS) max_slots = MAX_BOOT_SLOTS;
	
	for(new i = 0; i != max_slots; i ++)
	{
		if(!VEHICLE_BOOT[vehicleid][i][vehicle_boot_VALID]) return i;
	}
	return -1;
}
Set_HARVEST_Checkpoint(playerid)
{
	if(IsValidDynamicRaceCP(PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT]))
	{
		DestroyDynamicRaceCP(PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT] = INVALID_STREAMER_ID;
	}
	
	if(PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] == sizeof(HARVEST_CHECKPOINTS) - 1)
	{
		PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT] =	CreateDynamicRaceCP
														(
															1,
															HARVEST_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] ][0],
															HARVEST_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] ][1],
															HARVEST_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] ][2],
															0.0,
															0.0,
															0.0,
															5.0, 
															0,
															0,
															playerid
														);
	}
	else
	{
		PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT] =	CreateDynamicRaceCP
														(
															0,
															HARVEST_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] ][0],
															HARVEST_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] ][1],
															HARVEST_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] ][2],
															HARVEST_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] + 1 ][0],
															HARVEST_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] + 1 ][1],
															HARVEST_CHECKPOINTS[ PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] + 1 ][2],
															5.0, 
															0,
															0,
															playerid
														);
	}
	
	new info[1];
	info[0] = CHECKPOINT_TYPE_HARVEST;
	Streamer_SetArrayData(STREAMER_TYPE_RACE_CP, PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT], E_STREAMER_EXTRA_ID, info);
	
	return 1;
}
AddVehicleComponents(vehicleid)
{
	for(new i = 0; i != MAX_VEHICLE_COMPONENTS; i ++)
	{
		RemoveVehicleComponent(vehicleid, i);
		
		if(!GLOBAL_VEHICLES[vehicleid][gb_vehicle_COMPONENTS][i]) continue;
		AddVehicleComponent(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_COMPONENTS][i]);
	}
	return 1;
}


stock getRandomLetter() 
{
    return 65 + random(25);
}

UpdatePlayerPropertyContructor(playerid)
{
	SetPlayerPosEx
	(
		playerid, 
		PROPERTY_INTERIORS[ PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] ][property_INT_X],
		PROPERTY_INTERIORS[ PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] ][property_INT_Y],
		PROPERTY_INTERIORS[ PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] ][property_INT_Z],
		PROPERTY_INTERIORS[ PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] ][property_INT_ANGLE],
		PROPERTY_INTERIORS[ PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] ][property_INT_INTERIOR],
		0, false, true
	);
	
	new td_str[45]; format(td_str, sizeof td_str, "Interior:_%d/%d", PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO], sizeof PROPERTY_INTERIORS - 1);
	PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][1], td_str);
	SelectTextDrawEx(playerid, 0xe8d08fFF);
	return 1;
}
SendPlayerAction(playerid, action[])
{
	new str_text[190];
	format(str_text, sizeof str_text, "* %s %s", PLAYER_TEMP[playerid][pt_RP_NAME], action);
	ProxDetector(playerid, 15.0, str_text, 0xC2A2DAFF, 0xC2A2DAFF, 0xC2A2DAFF, 0xC2A2DAFF, 0xC2A2DAFF, 85);
	return 1;
}

public OnIncomingConnection(playerid, ip_address[], port)
{
	if(!server_loaded) BlockIpAddress(ip_address, 20 * 1000);
	return 1;
}

RegisterNewPlayer(playerid)
{
	if(PLAYER_TEMP[playerid][pt_USER_EXIST]) return 0;
	
	inline OnPlayerInserted()
	{
		ACCOUNT_INFO[playerid][ac_ID] = cache_insert_id();
		if(ACCOUNT_INFO[playerid][ac_ID])
		{
			PLAYER_TEMP[playerid][pt_USER_EXIST] = true;
			PI[playerid][ac_PHONE_NUMBER] = getPhoneNumber(ACCOUNT_INFO[playerid][ac_ID]);
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET phone_number = %d WHERE id = %d;", PI[playerid][ac_PHONE_NUMBER], ACCOUNT_INFO[playerid][ac_ID]);
			mysql_tquery(srp_db, QUERY_BUFFER);
			CreateDefaultContacts(playerid);
			CallLocalFunction("OnPlayerRegister", "i", playerid);
		}
		else Kick(playerid);
		return 1;
	}

	mysql_format
	(
		srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, 
		"\
			INSERT INTO player\
			(\
				name,\
				ip,\
				email,\
				salt,\
				pass,\
				reg_date,\
				last_connection_timestamp,\
				level,\
				rep,\
				connected,\
				playerid,\
				time_for_rep,\
				skin,\
				cash,\
				pos_x,\
				pos_y,\
				pos_z,\
				angle,\
				state,\
				fight_style,\
				health,\
				hungry,\
				thirst,\
				config_sounds,\
				config_audio,\
				config_time,\
				config_hud,\
				config_admin,\
				config_secure_login,\
				phone_visible_number,\
				doubt_channel\
			) \
			VALUES\
			(\
				'%e', '%e', '%e', '%e', '%e', '%e', %d, %d, %d, %d, %d, %d, %d, %d, %d, %f, %f, %f, %f, %d, %f, %f, %f, %d, %d, %d, %d, %d, %d, %d, %d\
			);\
		",
			ACCOUNT_INFO[playerid][ac_NAME],
			ACCOUNT_INFO[playerid][ac_IP],
			ACCOUNT_INFO[playerid][ac_EMAIL],
			ACCOUNT_INFO[playerid][ac_SALT],
			ACCOUNT_INFO[playerid][ac_PASS],
			ACCOUNT_INFO[playerid][ac_REG_DATE],
			ACCOUNT_INFO[playerid][ac_LAST_CONNECTION_TIMESTAMP],
			ACCOUNT_INFO[playerid][ac_STATE],
			ACCOUNT_INFO[playerid][ac_LEVEL],
			ACCOUNT_INFO[playerid][ac_REP],
			PI[playerid][ac_CONNECTED],
			PI[playerid][ac_PLAYERID],
			ACCOUNT_INFO[playerid][ac_TIME_FOR_REP],
			PI[playerid][ac_SKIN],
			PI[playerid][ac_CASH],
			PI[playerid][ac_POS_X],
			PI[playerid][ac_POS_Y],
			PI[playerid][ac_POS_Z],
			PI[playerid][ac_ANGLE],
			PI[playerid][ac_FIGHT_STYLE],
			PI[playerid][ac_HEALTH],
			PI[playerid][ac_HUNGRY],
			PI[playerid][ac_THIRST],
			PI[playerid][ac_CONFIG_SOUNDS],
			PI[playerid][ac_CONFIG_AUDIO],
			PI[playerid][ac_CONFIG_TIME],
			PI[playerid][ac_CONFIG_HUD],
			PI[playerid][ac_CONFIG_ADMIN],
			PI[playerid][ac_CONFIG_SECURE_LOGIN],
			PI[playerid][ac_PHONE_VISIBLE_NUMBER],
			ACCOUNT_INFO[playerid][ac_DOUBT_CHANNEL]
	);
	printf("%s", QUERY_BUFFER);
	MySQL_TQueryInline(srp_db, using inline OnPlayerInserted, QUERY_BUFFER );
	return 1;
}

RegisterNewPlayer_GPS_Site(playerid, slot)
{
	inline OnPgpsInserted()
	{
		PLAYER_GPS[playerid][slot][player_gps_ID] = cache_insert_id();
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO pgps (id_player, name, x, y, z, world, interior) VALUES(%d, '%e', %f, %f, %f, %d, %d);", ACCOUNT_INFO[playerid][ac_ID], PLAYER_GPS[playerid][slot][player_gps_NAME], PLAYER_GPS[playerid][slot][player_gps_X], PLAYER_GPS[playerid][slot][player_gps_Y], PLAYER_GPS[playerid][slot][player_gps_Z], PLAYER_GPS[playerid][slot][player_gps_WORLD], PLAYER_GPS[playerid][slot][player_gps_INTERIOR]);
	MySQL_TQueryInline(srp_db, using inline OnPgpsInserted,QUERY_BUFFER );
	return 1;
}



ResetPlayerVariables(playerid)
{
	new tmp_PI[enum_PI]; PI[playerid] = tmp_PI;

	new temp_PLAYER_TEMP[Temp_Enum]; PLAYER_TEMP[playerid] = temp_PLAYER_TEMP;
	
	new temp_PLAYER_TOYS[Player_Toys_Info];
	for(new i = 0; i != MAX_SU_TOYS; i ++) PLAYER_TOYS[playerid][i] = temp_PLAYER_TOYS;
	
	new temp_PLAYER_POCKET[Player_Pocket_Enum];
	for(new i = 0; i != MAX_PLAYER_POCKET_OBJECTS; i ++) PLAYER_POCKET[playerid][i] = temp_PLAYER_POCKET;
	
	
	new temp_PLAYER_PHONE_BOOK[Phone_Book_Enum]; 
	for(new i = 0; i != MAX_PHONE_CONTACTS; i ++) PLAYER_PHONE_BOOK[playerid][i] = temp_PLAYER_PHONE_BOOK;
	
	new temp_PLAYER_GPS[Player_GPS_Enum]; 
	for(new i = 0; i != MAX_PLAYER_GPS_SAVES; i ++) PLAYER_GPS[playerid][i] = temp_PLAYER_GPS;
	
	new tmp_PLAYER_WORKS[enum_PLAYER_WORKS];
	for(new i = 0; i != sizeof(PLAYER_WORKS[]); i ++) PLAYER_WORKS[playerid][i] = tmp_PLAYER_WORKS;
	
	new temp_PLAYER_PROPERTY_CONSTRUCTO[PLAYER_PROPERTY_CONSTRUCTOR_ENU]; PLAYER_PROPERTY_CONSTRUCTOR[playerid] = temp_PLAYER_PROPERTY_CONSTRUCTO;
	
	PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] = INVALID_PLAYER_ID;
	PLAYER_TEMP[playerid][pt_ADMIN_SERVICE] = false;

	new tmp_PLAYER_WEAPONS[enum_PLAYER_WEAPONS];
	for(new i = 0; i != sizeof PLAYER_WEAPONS[]; i ++) PLAYER_WEAPONS[playerid][i] = tmp_PLAYER_WEAPONS;
	
	new tmp_PLAYER_AC_INFO[e_PLAYER_AC_INFO];
	for(new i = 0; i != sizeof(ac_Info); i ++) PLAYER_AC_INFO[playerid][i] = tmp_PLAYER_AC_INFO;
	return 1;
}



GetEnterExitIndexById(id)
{
	for(new i = 0; i != sizeof ENTER_EXIT; i ++)
	{
		if(ENTER_EXIT[i][ee_ID] == 0) continue;
		if(ENTER_EXIT[i][ee_ID] == id) return i;
	}
	return -1;
}





CMD:cachear(playerid, params[])
{
	if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No formas parte de una banda.");
	if(sscanf(params, "u", params[0])) return SendNotification(playerid, "Error: /cachear [playerid/nombre]");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás de pie.");
	
	if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "El jugador no está conectado.");
	new Float:pos[3]; GetPlayerPos(params[0], pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");
	if(ACCOUNT_INFO[params[0]][ac_STATE] != ROLEPLAY_STATE_CRACK) return SendNotification(playerid, "El jugador no está abatido.");
	if(GetPlayerState(params[0]) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "Para revisar a esta persona tiene que estar de pie.");
	
	new dialog[95 * 15], line_str[95];
	format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Arma\t{"#BLUE_COLOR"}Munición\t{"#SILVER_COLOR"}Slot\n");
	
	for(new i = 0; i < sizeof PLAYER_WEAPONS[]; i ++)
	{
		if(!PLAYER_WEAPONS[params[0]][i][player_weapon_VALID]) continue;
		
		format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}%s\t{"#SILVER_COLOR"}%d\n", WEAPON_INFO[ PLAYER_WEAPONS[params[0]][i][player_weapon_ID] ][weapon_info_NAME], number_format_thousand(PLAYER_WEAPONS[params[0]][i][player_weapon_AMMO]), i);
		strcat(dialog, line_str);
	}

	format(line_str, sizeof line_str, "Armas de %s", PLAYER_TEMP[playerid][pt_RP_NAME]);
	
	ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_TABLIST_HEADERS, line_str, dialog, "Continuar", "Cerrar");
	
	new action[64];
	format(action, sizeof action, "cachea rápidamente a %s.", PLAYER_TEMP[params[0]][pt_RP_NAME]);
	Auto_SendPlayerAction(playerid, action);
	return 1;
}

CMD:morir(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] != ROLEPLAY_STATE_CRACK) return SendNotification(playerid, "No estás malherido");
	
	new now = gettime();
	if(now < PLAYER_TEMP[playerid][pt_DEATH_TIME] + 30)
	{
		new time = 30 - (now - PLAYER_TEMP[playerid][pt_DEATH_TIME]);
		SendFormatNotification(playerid, "Tienes que esperar %s minutos para poder usar este comando.", TimeConvert(time));
		return 1;
	}

	SetPlayerHealthEx(playerid, 0.0);
	Auto_SendPlayerAction(playerid, "ha elegido la salida rápida.");
	return 1;
}

CMD:gpos(playerid, params[])
{
	new Float:pos[3], Float:angle;
	if(IsPlayerInAnyVehicle(playerid)) {
		new vehicleId = GetPlayerVehicleID(playerid);
		GetVehiclePos(vehicleId, pos[0], pos[1], pos[2]);
		GetVehicleZAngle(vehicleId, angle);
	}
	else {
		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
		GetPlayerFacingAngle(playerid, angle);
	}

	if(isnull(params)) printf("%f, %f, %f, %f, %d", pos[0], pos[1], pos[2], angle, GetPlayerInterior(playerid));
	else printf("%f, %f, %f, %f, %d //%s", pos[0], pos[1], pos[2], angle, GetPlayerInterior(playerid), params);
	return 1;
}

CMD:cpos(playerid, params[])
{
	new Float:pos[3], Float:at[3];
	GetPlayerCameraPos(playerid, pos[0], pos[1], pos[2]);
	GetPlayerCameraLookAt(playerid, at[0], at[1], at[2]);

	printf("{%f, %f, %f, %f, %f, %f}", pos[0], pos[1], pos[2], at[0], at[1], at[2]);
	return 1;
}













FormatDialogStrings()
{
	new Dialog_Lines_String[256];
	
		/// PIZZA FOOD
	format(DIALOG_FOOD_PIZZA_String, sizeof DIALOG_FOOD_PIZZA_String, "Producto\tPrecio\n");
	for(new i; i != sizeof PIZZA_FOOD; i ++)
	{
		format(Dialog_Lines_String, sizeof Dialog_Lines_String, "{FFFFFF}%d. %s\t{"#BLUE_COLOR"}%d$\n", i + 1, PIZZA_FOOD[i][food_NAME], PIZZA_FOOD[i][food_PRICE]);
		strcat(DIALOG_FOOD_PIZZA_String, Dialog_Lines_String);
	}
	
		// CLUCKIN FOOD 
	format(DIALOG_FOOD_CLUCKIN_String, sizeof DIALOG_FOOD_CLUCKIN_String, "Producto\tPrecio\n");
	for(new i; i != sizeof CLUCKIN_BELL_FOOD; i ++)
	{
		format(Dialog_Lines_String, sizeof Dialog_Lines_String, "{FFFFFF}%d. %s\t{"#BLUE_COLOR"}%d$\n", i + 1, CLUCKIN_BELL_FOOD[i][food_NAME], CLUCKIN_BELL_FOOD[i][food_PRICE]);
		strcat(DIALOG_FOOD_CLUCKIN_String, Dialog_Lines_String);
	}
	
		/// BURGER_SHOT FOOD 
	format(DIALOG_FOOD_BURGER_String, sizeof DIALOG_FOOD_BURGER_String, "Producto\tPrecio\n");
	for(new i; i != sizeof BURGER_SHOT_FOOD; i ++)
	{
		format(Dialog_Lines_String, sizeof Dialog_Lines_String, "{FFFFFF}%d. %s\t{"#BLUE_COLOR"}%d$\n", i + 1, BURGER_SHOT_FOOD[i][food_NAME], BURGER_SHOT_FOOD[i][food_PRICE]);
		strcat(DIALOG_FOOD_BURGER_String, Dialog_Lines_String);
	}

		// 24/7 LIST 
	format(DIALOG_247_LIST_String, sizeof DIALOG_247_LIST_String, "Producto\tPrecio\n");
	for(new i = 0; i != sizeof Supermarket_Product_List; i ++)
	{
		format(Dialog_Lines_String, sizeof(Dialog_Lines_String), "{FFFFFF}%d. %s\t{"#BLUE_COLOR"}%d$\n", i + 1, Supermarket_Product_List[i][product_NAME], Supermarket_Product_List[i][product_PRICE]);
		strcat(DIALOG_247_LIST_String, Dialog_Lines_String);
	}
	
	//GPS
	new city[45], zone[45];

	//GPS CONCES
	format(DIALOG_PLAYER_GPS_SITE_7_String, sizeof DIALOG_PLAYER_GPS_SITE_7_String, "{"#SILVER_COLOR"}Lugar\t{"#BLUE_COLOR"}Zona\n");
	for(new i = 0; i != sizeof(SELL_VEHICLES_SHOPS_SPAWN); i++ )
	{
		GetPointZone(SELL_VEHICLES_SHOPS_SPAWN[i][0], SELL_VEHICLES_SHOPS_SPAWN[i][1], city, zone);
		format(Dialog_Lines_String, sizeof Dialog_Lines_String, "{FFFFFF}Concesionario %d\t{"#BLUE_COLOR"}%s, %s\n", i + 1, city, zone);
		strcat(DIALOG_PLAYER_GPS_SITE_7_String, Dialog_Lines_String);
	}
	
	//Colores bandas
	format(DIALOG_CREATE_CREW_COLOR_String, sizeof DIALOG_CREATE_CREW_COLOR_String, "");
	for(new i = 0; i != sizeof(RandomColors); i++)
	{
		format(Dialog_Lines_String, sizeof(Dialog_Lines_String), "{%06x}color %d\n", RandomColors[i] >>> 8, i + 1);
		strcat(DIALOG_CREATE_CREW_COLOR_String, Dialog_Lines_String);
	}

	format(DIALOG_CREDIT_String, sizeof DIALOG_CREDIT_String, 
		"\
			Servidor: "SERVER_NAME"\n\
			Web/foro: "SERVER_WEBSITE"\n\
			Versión: "SERVER_VERSION"\n\
			Fecha versión: "VERSION_DATE"\n\
			\n\
			"SERVER_NAME" es un servidor Roleplay nuevo por lo que actualmente\n\
			se encuentra desarrollo, puedes dejar tus sugerencias en el foro.\n\
			Actualmente al ser una versión inicial puede contener fallos.\n\
			\n\
			\n\
			Programación:\n\
			adri1\n\
		"
	);
	
	//Animaciones
	format(DIALOG_ANIMS_String, sizeof DIALOG_ANIMS_String, "");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /rendirse\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /blowjob\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /rodar\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /borracho\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /bomba\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /apuntar\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /reir\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /amenazar\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /paja\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /herido\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /cruzarbrazos\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /recostarse\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /cubrirse\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /vomitar\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /comer\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /despedir\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /nalgada\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /agonizar\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /besar\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /crack\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /mear\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /sentarse\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /fucku\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /llamar\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /taichi\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /beber\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /boxear\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /saludar\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /llorar\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /dormir\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /rapear\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /astrip\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /bailar\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /alentar\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /hablar\n");
	strcat(DIALOG_ANIMS_String, "{"#SILVER_COLOR"}>> /asiento\n");
			
	return 1;
}

	
SanAndreas()
{
	CreateTextDraws();
	LoadEnterExits();
	
	//db
	LoadProperties();
	
	//HABITACION INTRO PARA REGISTRARSE
	CreateDynamicObject(19624, 446.51517, 514.81940, 1001.63550,   0.00000, 0.00000, 29.44436); //MALETA ROCKSTAR DE LA HABITACION DE LA INTRO
	SetDynamicObjectMaterial(CreateDynamicObject(19893, 444.1881, 514.5712, 1001.3964,   0.0000, 0.0000, 0.0), 1, 18646, "matcolours", "grey-50-percent", 0xFFFFFFFF); //LAPTOP DE LA HABITACION DE LA INTRO
	SetDynamicObjectMaterial(CreateDynamicObject(18981, 444.61780, 510.80801, 1004.14160,   0.00000, 90.00000, 0.00000), 0, 18646, "matcolours", "black", 0xFFFFFFFF); //TECHO NEGRO PRIMERA HABITACION DE LA INTRO
	CreateDynamicObject(2102, 443.90033, 514.92853, 1001.66467,   0.00000, 0.00000, 38.35522); //RADIO DE LA HABITACION DE LA INTRO

	LoadIntroData();
	
	//Mina
	CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}Herramientas de minería\n{FFFFFF}3 productos disponibles.\nPresiona H para comprar.", 0xFFFFFFAA, 626.2347,116.3636,8.0031, 10);
	CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}Procesadora\n{FFFFFF}Presiona ALT para soltar los minerales.", 0xFFFFFFAA, 594.7598,-41.0603,791.2190-2.0, 15);
	CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}Procesadora\n{FFFFFF}Presiona ALT para soltar los minerales.", 0xFFFFFFAA, 566.8312,-41.2121,791.2190-2.0, 15);
	CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}Procesadora\n{FFFFFF}Presiona ALT para soltar los minerales.", 0xFFFFFFAA, 509.5341,-36.8747,790.7587-2.0, 15);
	CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}Procesadora\n{FFFFFF}Presiona ALT para soltar los minerales.", 0xFFFFFFAA, 454.7086,-36.1840,790.3587-2.0, 15);
	CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}Procesadora\n{FFFFFF}Presiona ALT para soltar los minerales.", 0xFFFFFFAA, 378.3867,-37.9046,790.3587-2.0, 15);
	CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}Procesadora\n{FFFFFF}Presiona ALT para soltar los minerales.", 0xFFFFFFAA, 268.0978,-37.8627,790.3587-2.0, 15);
	CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}Procesadora\n{FFFFFF}Presiona ALT para soltar los minerales.", 0xFFFFFFAA, 234.2289,-37.7960,790.3587-2.0, 15);

	
	
	//auto
	CreateDynamic3DTextLabel("Usa {"#PRIMARY_COLOR"}/examen {FFFFFF}para realizar el examen por 500$.", 0xFFFFFFFF, 1063.718994, -343.093566, 2797.699951, 5.0, .testlos = true, .worldid = -1, .interiorid = -1);

	//Putis
    CreateDynamic3DTextLabel("Pulsa {"#PRIMARY_COLOR"}'F' {FFFFFF}para soltar unos cuantos billetes.", 0xFFFFFFFF, 1208.744140, -34.261119, 1000.953125, 5.0, .testlos = true, .worldid = 0, .interiorid = 3);
    CreateDynamic3DTextLabel("Pulsa {"#PRIMARY_COLOR"}'F' {FFFFFF}para soltar unos cuantos billetes.", 0xFFFFFFFF, 1213.738037, -33.667881, 1000.953125, 5.0, .testlos = true, .worldid = 0, .interiorid = 3);
	CreateDynamic3DTextLabel("Escribe {"#PRIMARY_COLOR"}/bebida {FFFFFF}para comprar bebida alcohólica.", 0xFFFFFFFF, 1207.234375, -29.231435, 1000.953125, 5.0, .testlos = true, .worldid = 0, .interiorid = 3);

	Club_Strippers_Actor[0] = CreateDynamicActor(246, 1209.416503, -36.272216, 1001.484375, 17.0, .worldid = 0, .interiorid = 3);
    Club_Strippers_Actor[1] = CreateDynamicActor(87, 1215.355712, -33.439422, 1001.382812, 85.3, .worldid = 0, .interiorid = 3);
	CreateDynamic3DTextLabel("Osvalda", 0xFFFFFFAA, 1209.416503, -36.272216, 1001.484375 + 0.3, 5.0, .testlos = false, .worldid = 0, .interiorid = 3);
	CreateDynamic3DTextLabel("Tamara", 0xFFFFFFAA, 1215.355712, -33.439422, 1001.382812 + 0.3, 5.0, .testlos = false, .worldid = 0, .interiorid = 3);
    Club_Bar_Actor = CreateDynamicActor(195, 1206.232788, -29.325387, 1000.953125, 270.0, .worldid = 0, .interiorid = 3);
	Club_Actors[0] = CreateDynamicActor(1, 1209.846191, -33.879096, 1000.953125, 180.626205, .worldid = 0, .interiorid = 3);
	Club_Actors[1] = CreateDynamicActor(15, 1207.873535, -34.441837, 1000.953125, 213.213790, .worldid = 0, .interiorid = 3);
	Club_Actors[2] = CreateDynamicActor(21, 1215.694702, -31.700962, 1000.953125, 158.379913, .worldid = 0, .interiorid = 3);
	Club_Actors[3] = CreateDynamicActor(73, 1214.740356, -35.461227, 1000.953125, 337.608306, .worldid = 0, .interiorid = 3);
	Club_Actors[4] = CreateDynamicActor(78, 1213.713012, -34.496170, 1000.953125, 293.427886, .worldid = 0, .interiorid = 3);
	Club_Actors[5] = CreateDynamicActor(127, 1211.037963, -36.378440, 1000.960571, 74.092475, .worldid = 0, .interiorid = 3);
	Club_Actors[6] = CreateDynamicActor(131, 1207.190307, -36.132881, 1000.953125, 277.761077, .worldid = 0, .interiorid = 3);
	UpdateThePutis();
	SetTimer("UpdateThePutis", 3000, true);

	// Map Icons Hospitales
	//for(new i = 0; i < sizeof Hospital_Spawn_Positions; i++) CreateDynamicMapIcon(Hospital_Spawn_Positions[i][hspi_X], Hospital_Spawn_Positions[i][hspi_Y], Hospital_Spawn_Positions[i][hspi_Z], 22, -1, 0, 0);
	
	//Tuning
	CreateDynamicMapIcon(263.592102, 22.960014, 2.170951, 63, -1, 0, 0);
	CreateDynamic3DTextLabel("Usa {"#PRIMARY_COLOR"}/tuning {FFFFFF}para tunear tu vehículo", 0xFFFFFFFF, 263.592102, 22.960014, 2.170951, 10.0, .testlos = true, .worldid = 0, .interiorid = 0);
	
	//3d texts armarios
	for(new i = 0; i < sizeof PROPERTY_CLOSET_POS; i++)
	{
		CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}Armario\n\n{FFFFFF}Escribe {"#PRIMARY_COLOR"}/armario {FFFFFF}para verlo", 0xFFFFFFFF, PROPERTY_CLOSET_POS[i][property_closet_X], PROPERTY_CLOSET_POS[i][property_closet_Y], PROPERTY_CLOSET_POS[i][property_closet_Z] + 0.25, 10.0, .testlos = true, .interiorid = PROPERTY_INTERIORS[i][property_INT_INTERIOR]);
		if(PROPERTY_INTERIORS[i][property_EMPTY_INTERIOR]) CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}Armario\n\n{FFFFFF}Escribe {"#PRIMARY_COLOR"}/armario {FFFFFF}para verlo", 0xFFFFFFFF, PROPERTY_CLOSET_POS[i][property_closet_X], PROPERTY_CLOSET_POS[i][property_closet_Y], PROPERTY_CLOSET_POS[i][property_closet_Z] + 0.25 + PROPERTY_EMPTY_INTERIOR_Z_OFFSET, 10.0, .testlos = true, .interiorid = PROPERTY_INTERIORS[i][property_INT_INTERIOR]);
	}

	// 3D Texts Ropas
	for(new i = 0; i < sizeof Clothing_Shop_Positions; i++)
	{
		new label_str[256];
		format(label_str, sizeof label_str, "{FF7373}%s\n{FFFFFF}Presiona {FF7373}[ H ] {FFFFFF}para ver las opciones disponibles", Clothing_Shop_Positions[i][clothing_shop_NAME]);
		CreateDynamic3DTextLabel(label_str, 0xFFFFFFFF, Clothing_Shop_Positions[i][clothing_shop_X], Clothing_Shop_Positions[i][clothing_shop_Y], Clothing_Shop_Positions[i][clothing_shop_Z] + 0.25, 10.0, .testlos = true, .interiorid = Clothing_Shop_Positions[i][clothing_shop_INTERIOR]);
	}
	
	// 3D Texts Pedir comida
	for(new i = 0; i < sizeof Fast_Food_Positions; i++)
	{
		new label_str[128];
		format(label_str, sizeof label_str, "{FF7373}%s\n{FFFFFF}Presiona {FF7373}[ H ] {FFFFFF}para alimentarte", Fast_Food_Positions[i][fast_food_NAME]);
		CreateDynamic3DTextLabel(label_str, 0xFFFFFFFF, Fast_Food_Positions[i][fast_food_X], Fast_Food_Positions[i][fast_food_Y], Fast_Food_Positions[i][fast_food_Z] + 0.25, 10.0, .testlos = true, .interiorid = Fast_Food_Positions[i][fast_food_INTERIOR]);
	}
	
	// 3D Texts Bank
	for(new i = 0; i < sizeof Bank_Interior_Positions; i++) CreateDynamic3DTextLabel("Banco\nPresiona {FF7373} [ H ] {FFFFFF}para realizar una operación", 0xFFFFFFFF, Bank_Interior_Positions[i][bank_X], Bank_Interior_Positions[i][bank_Y], Bank_Interior_Positions[i][bank_Z] + 0.25, 10.0, .testlos = true, .worldid = Bank_Interior_Positions[i][bank_WORLD], .interiorid = Bank_Interior_Positions[i][bank_INTERIOR]);
	for(new i = 0; i < sizeof BUY_PROPERTIES_SITES; i++) CreateDynamic3DTextLabel("Escribe {"#PRIMARY_COLOR"}/comprarcasa [ID] {FFFFFF}para comprar una propiedad", 0xFFFFFFFF, BUY_PROPERTIES_SITES[i][site_X], BUY_PROPERTIES_SITES[i][site_Y], BUY_PROPERTIES_SITES[i][site_Z] + 0.25, 10.0, .testlos = true, .worldid = BUY_PROPERTIES_SITES[i][site_WORLD], .interiorid = BUY_PROPERTIES_SITES[i][site_INTERIOR]);
	
	//Cajeros
	for(new i = 0; i < sizeof ATM_BANK; i ++)
	{
		CreateDynamicObject(ATM_BANK[i][atm_modelid], ATM_BANK[i][atm_X], ATM_BANK[i][atm_Y], ATM_BANK[i][atm_Z], ATM_BANK[i][atm_RX], ATM_BANK[i][atm_RY], ATM_BANK[i][atm_RZ], 0, 0);
		
		ATM_BANK[i][atm_X] += (-0.5 * floatsin(-(ATM_BANK[i][atm_RZ] - 90.0), degrees));
		ATM_BANK[i][atm_Y] += (-0.5 * floatcos(-(ATM_BANK[i][atm_RZ] - 90.0), degrees));
		CreateDynamicMapIcon(ATM_BANK[i][atm_X], ATM_BANK[i][atm_Y], ATM_BANK[i][atm_Z], 52, -1, 0, 0);
		CreateDynamic3DTextLabel("Cajero automático\nPresiona {FF7373} [ H ] {FFFFFF}para realizar operaciones", 0xFFFFFFFF, ATM_BANK[i][atm_X], ATM_BANK[i][atm_Y], ATM_BANK[i][atm_Z] + 0.25, 10.0, .testlos = true, .worldid = 0, .interiorid = 0);
	
		new atm_pickup_id = CreateDynamicPickup(0, 1, ATM_BANK[i][atm_X], ATM_BANK[i][atm_Y], ATM_BANK[i][atm_Z] + 0.10, 0, 0), info[3];
		info[0] = PICKUP_TYPE_ATM;
		info[1] = i; // Index
		info[2] = 0; // Nada
		Streamer_SetArrayData(STREAMER_TYPE_PICKUP, atm_pickup_id, E_STREAMER_EXTRA_ID, info);
	}
	
	// 3D Texts Gasolinera
	for(new i = 0; i < sizeof Fuel_Stations; i++)
	{
		CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}Gasolinera\n\n{FFFFFF}Precio: {"#PRIMARY_COLOR"}9$/Litro\n{FFFFFF}Escribe {"#PRIMARY_COLOR"}/gasolina [litros / lleno] {FFFFFF}para repostar\n\nCompra {"#PRIMARY_COLOR"}/bidon {FFFFFF}de 20 litros por 250$", 0xFFFFFFFF, Fuel_Stations[i][fs_X], Fuel_Stations[i][fs_Y], Fuel_Stations[i][fs_Z] + 0.25, 10.0, .testlos = true, .worldid = 0, .interiorid = 0);
	}

	//24/7 Int
	CreateDynamic3DTextLabel("Presiona {FF7373}[ H ] {FFFFFF}para ver los productos disponibles", 0xFFFFFFFF, -27.964675, -89.948631, 1003.546875 + 0.25, 10.0, .testlos = true, .interiorid = 18);

	//Mercado negro
	CreateDynamic3DTextLabel("{FF7373}Mercado negro\n{FFFFFF}Presiona {FF7373}[ H ] {FFFFFF}para comprar", 0xFFFFFFFF, 2119.059814, -2001.701904, 7.984375, 10.0, .testlos = true, .interiorid = 0, .worldid = 0);

	//Vehs venta
	for(new i = 0; i != sizeof SELL_INFO_VEHICLES; i ++)
	{
		if(SELL_INFO_VEHICLES[i][sell_info_COLOR_1] == -1) SELL_INFO_VEHICLES[i][sell_info_COLOR_1] = valid_work_vehicle_colors[random(sizeof(valid_work_vehicle_colors))];
		if(SELL_INFO_VEHICLES[i][sell_info_COLOR_2] == -1) SELL_INFO_VEHICLES[i][sell_info_COLOR_2] = valid_work_vehicle_colors[random(sizeof(valid_work_vehicle_colors))];
		
		new vehicle_id = INVALID_VEHICLE_ID;
		vehicle_id = CreateVehicle(SELL_INFO_VEHICLES[i][sel_info_vehicle_MODELID], SELL_INFO_VEHICLES[i][sell_info_SPAWN_X], SELL_INFO_VEHICLES[i][sell_info_SPAWN_Y], SELL_INFO_VEHICLES[i][sell_info_SPAWN_Z], SELL_INFO_VEHICLES[i][sell_info_SPAWN_ANGLE], SELL_INFO_VEHICLES[i][sell_info_COLOR_1], SELL_INFO_VEHICLES[i][sell_info_COLOR_2], -1, false);
		
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_VALID] = true;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_TYPE] = VEHICLE_TYPE_SELL;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] = SELL_INFO_VEHICLES[i][sel_info_vehicle_MODELID];
		format(GLOBAL_VEHICLES[vehicle_id][gb_vehicle_NUMBER_PLATE], 32, "EN VENTA");
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X] = SELL_INFO_VEHICLES[i][sell_info_SPAWN_X];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y] = SELL_INFO_VEHICLES[i][sell_info_SPAWN_Y];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z] = SELL_INFO_VEHICLES[i][sell_info_SPAWN_Z];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_ANGLE] = SELL_INFO_VEHICLES[i][sell_info_SPAWN_ANGLE];
		
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][0] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][1] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][2] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z];
		
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_HEALTH] = 1000.0;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_PANELS] = 0;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_DOORS] = 0;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_LIGHTS] = 0;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_TIRES] = 0;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_1] = SELL_INFO_VEHICLES[i][sell_info_COLOR_1];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_2] = SELL_INFO_VEHICLES[i][sell_info_COLOR_2];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PAINTJOB] = 3; // No paintjob
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS] = 0.0;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_GAS] = 0.0;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_STATE] = VEHICLE_STATE_NORMAL;

		SELL_VEHICLES[vehicle_id][sell_vehicle_VALID] = true;
		SELL_VEHICLES[vehicle_id][sell_vehicle_SHOP] = SELL_INFO_VEHICLES[i][sell_info_SHOP];
		SELL_VEHICLES[vehicle_id][sell_vehicle_PRICE] = VEHICLE_INFO[GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] - 400][vehicle_info_PRICE];
		SELL_VEHICLES[vehicle_id][sell_vehicle_LEVEL] = 1; // VEHICLE_INFO[GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] - 400][vehicle_info_LEVEL];
		SELL_VEHICLES[vehicle_id][sell_vehicle_EXTRA] = VEHICLE_INFO[GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] - 400][vehicle_info_COINS];
		SELL_VEHICLES[vehicle_id][sell_vehicle_VIP_LEVEL] = VEHICLE_INFO[GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] - 400][vehicle_info_VIP_LEVEL];
		
		if(SELL_VEHICLES[vehicle_id][sell_vehicle_VIP_LEVEL]) SELL_VEHICLES[vehicle_id][sell_vehicle_LEVEL] = 1;
		if(SELL_VEHICLES[vehicle_id][sell_vehicle_EXTRA]) SELL_VEHICLES[vehicle_id][sell_vehicle_PRICE] = 0;
		
		SetVehicleToRespawnEx(vehicle_id);
		
		
		new label_str[256];
		
		if(SELL_VEHICLES[vehicle_id][sell_vehicle_VIP_LEVEL])
		{
			if(SELL_VEHICLES[vehicle_id][sell_vehicle_EXTRA])
			{
				format
				(
					label_str, 
						sizeof label_str, 
						"\
							{"#PRIMARY_COLOR"}Membresía VIP requerida\n\
							\n\
							{"#PRIMARY_COLOR"}%s\n\n\
							{FFFFFF}Coste: {"#PRIMARY_COLOR"}%d "SERVER_COIN"\n\
							{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\
						", VEHICLE_INFO[GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] - 400][vehicle_info_NAME], SELL_VEHICLES[vehicle_id][sell_vehicle_EXTRA], SELL_VEHICLES[vehicle_id][sell_vehicle_LEVEL]
				);
			}
			else
			{
				format
				(
					label_str, 
						sizeof label_str, 
						"\
							{"#PRIMARY_COLOR"}Membresía VIP requerida\n\
							\n\
							{"#PRIMARY_COLOR"}%s\n\n\
							{FFFFFF}Precio: {"#PRIMARY_COLOR"}%s$\n\
							{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\
						", VEHICLE_INFO[GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] - 400][vehicle_info_NAME], number_format_thousand(SELL_VEHICLES[vehicle_id][sell_vehicle_PRICE]), SELL_VEHICLES[vehicle_id][sell_vehicle_LEVEL]
				);
			}
		}
		else
		{
			if(SELL_VEHICLES[vehicle_id][sell_vehicle_EXTRA])
			{
				format
				(
					label_str, 
						sizeof label_str, 
						"\
							{"#PRIMARY_COLOR"}%s\n\n\
							{FFFFFF}Coste: {"#PRIMARY_COLOR"}%d "SERVER_COIN"\n\
							{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\
						", VEHICLE_INFO[GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] - 400][vehicle_info_NAME], SELL_VEHICLES[vehicle_id][sell_vehicle_EXTRA], SELL_VEHICLES[vehicle_id][sell_vehicle_LEVEL]
				);
			}
			else
			{
				format
				(
					label_str, 
						sizeof label_str, 
						"\
							{"#PRIMARY_COLOR"}%s\n\n\
							{FFFFFF}Precio: {"#PRIMARY_COLOR"}%s$\n\
							{FFFFFF}Nivel: {"#PRIMARY_COLOR"}%d\
						", VEHICLE_INFO[GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] - 400][vehicle_info_NAME], number_format_thousand(SELL_VEHICLES[vehicle_id][sell_vehicle_PRICE]), SELL_VEHICLES[vehicle_id][sell_vehicle_LEVEL]
				);
			}
		}
		
		
		SELL_VEHICLES[vehicle_id][sell_vehicle_LABEL_ID] = CreateDynamic3DTextLabel(label_str, 0xFFFFFFFF, 0.0, 0.0, 1.5, 10.0, .attachedvehicle = vehicle_id, .testlos = true, .worldid = 0, .interiorid = 0);
	}
	
	//Concesionarios
	for(new i = 0; i < sizeof SELL_VEHICLES_SHOPS_SPAWN; i++)
	{
		CreateDynamicMapIcon(SELL_VEHICLES_SHOPS_SPAWN[i][0], SELL_VEHICLES_SHOPS_SPAWN[i][1], SELL_VEHICLES_SHOPS_SPAWN[i][2], 55, -1, 0, 0);
		CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}SALIDA DE VEHICULOS", 0xFFFFFFFF, SELL_VEHICLES_SHOPS_SPAWN[i][0], SELL_VEHICLES_SHOPS_SPAWN[i][1], SELL_VEHICLES_SHOPS_SPAWN[i][2], 10.0, .testlos = true, .worldid = 0, .interiorid = 0);
	}
	
	//Notario
	CreateDynamic3DTextLabel("Presiona {FF7373}[ H ] {FFFFFF}para vender\nalguna propiedad o vehículo", 0xFFFFFFFF, -474.596282, 289.679107, 2004.584960, 10.0, .testlos = true, .interiorid = 20);
	//Grua
	CreateDynamic3DTextLabel("Presiona {FF7373}[ H ] {FFFFFF}para solicitar\nel servicio de grúa a un vehículo", 0xFFFFFFFF, -508.645385, 322.147644, 2004.585937, 10.0, .testlos = true, .interiorid = 20);
	
	
	//San Andreas Vehicles
	for(new i = 0; i != sizeof San_Andreas_Vehicles; i ++)
	{
		
		if(San_Andreas_Vehicles[i][san_andreas_vehicle_COLOR_1] == -1) San_Andreas_Vehicles[i][san_andreas_vehicle_COLOR_1] = valid_work_vehicle_colors[random(sizeof(valid_work_vehicle_colors))];
		if(San_Andreas_Vehicles[i][san_andreas_vehicle_COLOR_2] == -1) San_Andreas_Vehicles[i][san_andreas_vehicle_COLOR_2] = valid_work_vehicle_colors[random(sizeof(valid_work_vehicle_colors))];
		
		new vehicle_id = INVALID_VEHICLE_ID, respawn_time = 300;
		
		if(San_Andreas_Vehicles[i][san_andreas_vehicle_TYPE] == VEHICLE_TYPE_WORK)
		{
			if(work_info[ San_Andreas_Vehicles[i][san_andreas_vehicle_TYPE_IN] ][work_info_TYPE] == WORK_TYPE_FAMILY)
			{
				respawn_time = 2700;
			}
		}
		
		vehicle_id = CreateVehicle(San_Andreas_Vehicles[i][san_andreas_vehicle_MODELID], San_Andreas_Vehicles[i][san_andreas_vehicle_SPAWN_X], San_Andreas_Vehicles[i][san_andreas_vehicle_SPAWN_Y], San_Andreas_Vehicles[i][san_andreas_vehicle_SPAWN_Z], San_Andreas_Vehicles[i][san_andreas_vehicle_SPAWN_ANGLE], San_Andreas_Vehicles[i][san_andreas_vehicle_COLOR_1], San_Andreas_Vehicles[i][san_andreas_vehicle_COLOR_2], respawn_time, false);
		if(vehicle_id == INVALID_VEHICLE_ID) continue;
		
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_VALID] = true;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_TYPE] = San_Andreas_Vehicles[i][san_andreas_vehicle_TYPE];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] = San_Andreas_Vehicles[i][san_andreas_vehicle_MODELID];
		format(GLOBAL_VEHICLES[vehicle_id][gb_vehicle_NUMBER_PLATE], 32, "%c%c%c-%04d", getRandomLetter(), getRandomLetter(), getRandomLetter(), random(9999));
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X] = San_Andreas_Vehicles[i][san_andreas_vehicle_SPAWN_X];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y] = San_Andreas_Vehicles[i][san_andreas_vehicle_SPAWN_Y];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z] = San_Andreas_Vehicles[i][san_andreas_vehicle_SPAWN_Z];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_ANGLE] = San_Andreas_Vehicles[i][san_andreas_vehicle_SPAWN_ANGLE];
		
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][0] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][1] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][2] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z];
		
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_HEALTH] = 1000.0;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_PANELS] = 0;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_DOORS] = 0;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_LIGHTS] = 0;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_TIRES] = 0;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_1] = San_Andreas_Vehicles[i][san_andreas_vehicle_COLOR_1];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_2] = San_Andreas_Vehicles[i][san_andreas_vehicle_COLOR_2];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PAINTJOB] = 3; // No paintjob
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS] = VEHICLE_INFO[ GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] - 400][vehicle_info_MAX_GAS];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_GAS] = frandom(GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS], GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS] / 3, 2);
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_STATE] = VEHICLE_STATE_NORMAL;
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_VIP] = San_Andreas_Vehicles[i][san_andreas_vehicle_vip];
		GLOBAL_VEHICLES[vehicle_id][gb_vehicle_WORLD] = San_Andreas_Vehicles[i][san_andreas_vehicle_WORLD];
		
		if(GLOBAL_VEHICLES[vehicle_id][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK)
		{
			WORK_VEHICLES[vehicle_id][work_vehicle_VALID] = true;
			WORK_VEHICLES[vehicle_id][work_vehicle_WORK] = San_Andreas_Vehicles[i][san_andreas_vehicle_TYPE_IN];
			WORK_VEHICLES[vehicle_id][work_vehicle_EXP] = San_Andreas_Vehicles[i][san_andreas_vehicle_TYPE_IN_EXP];
			WORK_VEHICLES[vehicle_id][work_vehicle_NEED_DUTY] = work_info[ WORK_VEHICLES[vehicle_id][work_vehicle_WORK] ][work_info_NEED_DUTY];
		}
		
		SetVehicleToRespawnEx(vehicle_id);
		CallLocalFunction("OnSanAndreasVehicleLoad", "i", vehicle_id);
	}
	CallLocalFunction("OnSanAndreasVehiclesLoad", "");
	
	//Barreras
	for(new i = 0; i != sizeof San_Andreas_Barriers; i ++)
	{
		new tmpobjid = CreateDynamicObject(966, San_Andreas_Barriers[i][barrier_X], San_Andreas_Barriers[i][barrier_Y], San_Andreas_Barriers[i][barrier_Z], 0.0, 0.0, San_Andreas_Barriers[i][barrier_ROTATION], San_Andreas_Barriers[i][barrier_WORLD], San_Andreas_Barriers[i][barrier_INTERIOR], .streamdistance = 600.0, .drawdistance = 600.0);
		SetDynamicObjectMaterial(tmpobjid, 0, 9514, "711_sfw", "ws_carpark2", 0xFFFFFFFF);
		SetDynamicObjectMaterial(tmpobjid, 1, 9514, "711_sfw", "ws_carpark2", 0x00000000);
		tmpobjid = CreateDynamicObject(968, San_Andreas_Barriers[i][barrier_X], San_Andreas_Barriers[i][barrier_Y], San_Andreas_Barriers[i][barrier_Z] + 0.72967, 0.0, -90.0, San_Andreas_Barriers[i][barrier_ROTATION], San_Andreas_Barriers[i][barrier_WORLD], San_Andreas_Barriers[i][barrier_INTERIOR], .streamdistance = 600.0, .drawdistance = 600.0);
		SetDynamicObjectMaterial(tmpobjid, 0, 16640, "a51", "ws_carparkwall2", 0xFFFFFFFF);
		SetDynamicObjectMaterial(tmpobjid, 1, 16640, "a51", "ws_carparkwall2", 0x00000000);
		San_Andreas_Barriers[i][barrier_OBJECT_ID] = tmpobjid;

		new label_str[256];
		
		if(San_Andreas_Barriers[i][barrier_PRICE] > 0) format(label_str, sizeof label_str, "{FF7373}Peaje\n\n{FFFFFF}Toca el {FF7373}claxón {FFFFFF}para pagar {FF7373}%s$", number_format_thousand(San_Andreas_Barriers[i][barrier_PRICE]));
		else {
			if(San_Andreas_Barriers[i][barrier_VEHICLE_TYPE]) format(label_str, sizeof label_str, "{FF7373}%c%s\n\n{FFFFFF}Toca el {FF7373}claxón {FFFFFF}para que te abran", toupper(work_info[ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN] ][work_info_NAME][0]), work_info[ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN] ][work_info_NAME][1]);
			else format(label_str, sizeof label_str, "{FFFFFF}Toca el {FF7373}claxón {FFFFFF}para que te abran");
		}

		CreateDynamic3DTextLabel(label_str, 0xFFFFFFFF, San_Andreas_Barriers[i][barrier_X], San_Andreas_Barriers[i][barrier_Y], San_Andreas_Barriers[i][barrier_Z] + 1.5, 15.0, .worldid = San_Andreas_Barriers[i][barrier_WORLD], .interiorid = San_Andreas_Barriers[i][barrier_INTERIOR], .testlos = true);

		San_Andreas_Barriers[i][barrier_PLAYER_X] = San_Andreas_Barriers[i][barrier_X] + (3.4 * floatsin(-(San_Andreas_Barriers[i][barrier_ROTATION] + 90.0), degrees));
		San_Andreas_Barriers[i][barrier_PLAYER_Y] = San_Andreas_Barriers[i][barrier_Y] + (3.4 * floatcos(-(San_Andreas_Barriers[i][barrier_ROTATION] + 90.0), degrees));
		San_Andreas_Barriers[i][barrier_PLAYER_Z] = San_Andreas_Barriers[i][barrier_Z];
	}
	
	//polciais
	for(new i = 0; i != sizeof POLICE_GARAGE_DOORS; i ++)
	{
		POLICE_GARAGE_DOORS[i][police_gdoor_OBJECT_ID] = CreateDynamicObject(POLICE_GARAGE_DOORS[i][police_gdoor_MODELID], POLICE_GARAGE_DOORS[i][police_gdoor_CLOSED_X], POLICE_GARAGE_DOORS[i][police_gdoor_CLOSED_Y], POLICE_GARAGE_DOORS[i][police_gdoor_CLOSED_Z], POLICE_GARAGE_DOORS[i][police_gdoor_CLOSED_RX], POLICE_GARAGE_DOORS[i][police_gdoor_CLOSED_RY], POLICE_GARAGE_DOORS[i][police_gdoor_CLOSED_RZ], 0, 0);
	}
	for(new i = 0; i != sizeof POLICE_DOORS; i ++)
	{
		POLICE_DOORS[i][police_door_OBJECT_ID] = CreateDynamicObject(POLICE_DOORS[i][police_door_MODELID], POLICE_DOORS[i][police_door_X], POLICE_DOORS[i][police_door_Y], POLICE_DOORS[i][police_door_Z], 0.0, 0.0, POLICE_DOORS[i][police_door_RZ], POLICE_DOORS[i][police_door_WORLD], POLICE_DOORS[i][police_door_INTERIOR]);
		CreateDynamic3DTextLabel("Pulsa {FF7373} [ ENTER ] {FFFFFF}para abrir la puerta", 0xFFFFFFFF, POLICE_DOORS[i][police_door_X], POLICE_DOORS[i][police_door_Y], POLICE_DOORS[i][police_door_Z] + 1.25, 5.0, .testlos = false, .interiorid = POLICE_DOORS[i][police_door_INTERIOR], .worldid = POLICE_DOORS[i][police_door_WORLD]);
	}
	CreateDynamic3DTextLabel("Presiona {FF7373}[ H ] {FFFFFF}para empezar a trabajar\no para dejar de trabajar", 0xFFFFFFFF, 253.280288, 76.219017, 1003.640625, 5.0, .testlos = true, .interiorid = 6);
	CreateDynamic3DTextLabel("Presiona {FF7373}[ H ] {FFFFFF}para empezar a trabajar\no para dejar de trabajar", 0xFFFFFFFF, 266.547180, 119.652816, 1004.617187, 5.0, .testlos = true, .interiorid = 10);
	CreateDynamic3DTextLabel("Presiona {FF7373}[ H ] {FFFFFF}para empezar a trabajar\no para dejar de trabajar", 0xFFFFFFFF, 217.816406, 187.667922, 1003.031250, 5.0, .testlos = true, .interiorid = 3);

	CreateDynamic3DTextLabel("Presiona {FF7373}[ H ] {FFFFFF}para equiparte", 0xFFFFFFFF, 219.244018, 69.982215, 1005.039062, 5.0, .testlos = true, .interiorid = 6);
	CreateDynamic3DTextLabel("Presiona {FF7373}[ H ] {FFFFFF}para equiparte", 0xFFFFFFFF, 262.945587, 109.785270, 1004.617187, 5.0, .testlos = true, .interiorid = 10);
	CreateDynamic3DTextLabel("Presiona {FF7373}[ H ] {FFFFFF}para equiparte", 0xFFFFFFFF, 222.683914, 186.956054, 1003.031250, 5.0, .testlos = true, .interiorid = 3);
	
	Jail_Areas[0] = CreateDynamicRectangle(258.236938, 72.319061, 273.117279, 95.841438, -1, 6);
	Jail_Areas[1] = CreateDynamicRectangle(211.001739, 117.171920, 236.010986, 104.004219, -1, 10);
	Jail_Areas[2] = CreateDynamicRectangle(204.957885, 180.868392, 183.743148, 168.624618, -1, 3);
	Jail_Areas[3] = CreateDynamicRectangle(366.3146, 1951.8367, 17.6406, 78.4408, -1, 13);

	//Safe Zones
	for(new i = 0; i != sizeof SAFE_ZONES; i ++)
	{
		SAFE_ZONES[i][safe_zone_AREA_ID] = CreateDynamicRectangle(SAFE_ZONES[i][safe_zone_MIN_X], SAFE_ZONES[i][safe_zone_MIN_Y], SAFE_ZONES[i][safe_zone_MAX_X], SAFE_ZONES[i][safe_zone_MAX_Y], SAFE_ZONES[i][safe_zone_WORLD], SAFE_ZONES[i][safe_zone_INTERIOR]);
	}

	//Trabajos
	for(new i = 1; i < E_WORKS; i ++)
	{
		if(!obtain_work_coords[i][obtain_work_AVAILABLE]) continue;
		switch(work_info[i][work_info_TYPE])
		{
			case WORK_TYPE_NORMAL:
			{
				if(obtain_work_coords[i][obtain_work_LABELS])
				{
					new label_str[256];

					format(label_str, sizeof label_str, "{FF7373}Puesto de trabajo (%s)\n\n{FFFFFF}Nivel requerido: {FF7373}%d\n{FFFFFF}Presiona {FF7373} [ H ]{FFFFFF} para conseguirlo", work_info[i][work_info_NAME], work_info[i][work_info_LEVEL]);
					CreateDynamic3DTextLabel(label_str, 0xFFFFFFFF, obtain_work_coords[i][obtain_work_X], obtain_work_coords[i][obtain_work_Y], obtain_work_coords[i][obtain_work_Z], 10.0, .testlos = true, .interiorid = obtain_work_coords[i][obtain_work_INTERIOR]);
					CreateDynamicPickup(1239, 1, obtain_work_coords[i][obtain_work_X], obtain_work_coords[i][obtain_work_Y], obtain_work_coords[i][obtain_work_Z], -1, obtain_work_coords[i][obtain_work_INTERIOR]);
				}
				if(obtain_work_coords[i][obtain_work_MAP_ICON]) CreateDynamicMapIcon(obtain_work_coords[i][obtain_work_MAP_ICON_X], obtain_work_coords[i][obtain_work_MAP_ICON_Y], obtain_work_coords[i][obtain_work_MAP_ICON_Z], obtain_work_coords[i][obtain_work_MAP_ICON_ID], -1, 0, 0);
			}
		}
	}

	//Random taximeters
	for(new i = 0, j = GetVehiclePoolSize(); i <= j; i ++) TAXI_METER_VEHICLE[i][veh_taxi_meter_PRICE] = minrand(2, 8);

	//Carga camioneros
	for(new i = 0; i != sizeof LoadTrucksPoints; i ++)
	{
		CreateDynamic3DTextLabel("Usa {"#PRIMARY_COLOR"}/cargar {FFFFFF}para cargar el camión", 0xFFFFFFFF, LoadTrucksPoints[i][0], LoadTrucksPoints[i][1], LoadTrucksPoints[i][2], 10.0, .testlos = true, .interiorid = 0);
		CreateDynamicPickup(19135, 1, LoadTrucksPoints[i][0], LoadTrucksPoints[i][1], LoadTrucksPoints[i][2], -1, 0);
	}

	//Mecánico
	Mechanic_Areas[0] = CreateDynamicRectangle(1808.799194, -1450.352661, 1837.703979, -1414.697753, 0, 0);
	Streamer_SetArrayData(STREAMER_TYPE_AREA, Mechanic_Areas[0], E_STREAMER_EXTRA_ID, { AREA_TYPE_MECHANIC });

	for(new i; i != sizeof MechanicStartWorkingCoords; i++)
		CreateDynamic3DTextLabel("Usa {"#PRIMARY_COLOR"}/mecanico {FFFFFF}para empezar a trabajar\no para dejar de trabajar", 0xFFFFFFFF, MechanicStartWorkingCoords[i][0], MechanicStartWorkingCoords[i][1], MechanicStartWorkingCoords[i][2], 5.0, .testlos = true, .interiorid = 0);

	for(new i; i != sizeof MechanicBuyPiecesCoords; i++)
		CreateDynamic3DTextLabel("Piezas de mecánico\nEscribe {"#PRIMARY_COLOR"}/piezas [cantidad] {FFFFFF}para comprar piezas\nPrecio de pieza: {"#PRIMARY_COLOR"}50$", 0xFFFFFFFF, MechanicBuyPiecesCoords[i][0], MechanicBuyPiecesCoords[i][1], MechanicBuyPiecesCoords[i][2], 5.0, .testlos = true, .interiorid = 0);

	for(new i; i != sizeof MechanicBuyKitsCoords; i++)
		CreateDynamic3DTextLabel("Kits de reparación\n\nEscribe {"#PRIMARY_COLOR"}/kit {FFFFFF}para comprar un kit de reparación\nPrecio del kit: {"#PRIMARY_COLOR"}1.000$", 0xFFFFFFFF, MechanicBuyKitsCoords[i][0], MechanicBuyKitsCoords[i][1], MechanicBuyKitsCoords[i][2], 5.0, .testlos = true, .interiorid = 0);

	//Leñador
	CreateDynamicPickup(19793, 1, -527.670349, -97.338562, 63.176174, 0, 0);
	CreateDynamic3DTextLabel("Usa {"#PRIMARY_COLOR"}/talar {FFFFFF}para empezar a trabajar\no para dejar de trabajar", 0xFFFFFFFF, -527.670349, -97.338562, 63.176174, 5.0, .testlos = true, .worldid = 0, .interiorid = 0);

	//Pizzero
	CreateDynamicPickup(1582, 1, 2097.355712, -1818.040771, 13.382812, 0, 0);
	CreateDynamic3DTextLabel("Usa {"#PRIMARY_COLOR"}/pizzero {FFFFFF}para empezar a trabajar\no para dejar de trabajar", 0xFFFFFFFF, 2097.355712, -1818.040771, 13.382812, 5.0, .testlos = true, .worldid = 0, .interiorid = 0);

	//Medico
	CreateDynamicPickup(1275, 1, -2029.751342, -114.503044, 1035.171875, -1, 3);
	CreateDynamic3DTextLabel("Usa {"#PRIMARY_COLOR"}/medico {FFFFFF}para empezar a trabajar\no para dejar de trabajar", 0xFFFFFFFF, -2029.751342, -114.503044, 1035.171875, 5.0, .testlos = true, .interiorid = 3);

	for(new i; i != sizeof MedicalBuyKitsCoords; i++)
		CreateDynamic3DTextLabel("Botiquines\n\nEscribe {"#PRIMARY_COLOR"}/botiquin {FFFFFF}para comprar un botiquín\nPrecio del botiquín: {"#PRIMARY_COLOR"}5.000$", 0xFFFFFFFF, MedicalBuyKitsCoords[i][0], MedicalBuyKitsCoords[i][1], MedicalBuyKitsCoords[i][2], 5.0, .testlos = true);

	//mercado
	CreateDynamicActor(179, 2120.724853, -2000.400268, 7.984375, 130.0, true, 100.0, 0, 0);

	//Trash
	CreateDynamic3DTextLabel("Usa {"#PRIMARY_COLOR"}/basurero {FFFFFF}para empezar a trabajar\no para dejar de trabajar", 0xFFFFFFFF, -1906.577514, -1756.457519, 22.079319, 5.0, .testlos = true, .worldid = 0, .interiorid = 0);

	//Ruta 0
	CreateTrashCheckpoint(0, 1440, -1812.46228, -558.37744, 15.73948,   0.00000, 0.00000, 274.24359);
	CreateTrashCheckpoint(0, 1440, -1793.84106, -312.50452, 24.46057,   0.00000, 0.00000, 268.49271);
	CreateTrashCheckpoint(0, 1440, -1801.20825, 115.39587, 14.55550,   0.00000, 0.00000, 280.09882);
	CreateTrashCheckpoint(0, 1440, -1750.40051, 323.11810, 6.61720,   0.00000, 0.00000, 319.06424);
	CreateTrashCheckpoint(0, 1440, -1814.43311, 511.15622, 28.66113,   0.00000, 9.00000, 304.31201);
	CreateTrashCheckpoint(0, 1440, -1982.79871, 614.85687, 34.61121,   0.00000, 0.00000, 0.00000);
	CreateTrashCheckpoint(0, 1440, -2229.42896, 575.27448, 34.58952,   0.00000, 0.00000, 0.00000);
	CreateTrashCheckpoint(0, 1440, -2385.20020, 513.64594, 28.11606,   0.00000, 0.00000, 282.48959);
	CreateTrashCheckpoint(0, 1440, -2258.51782, 221.79150, 34.74878,   0.00000, 0.00000, 90.00000);
	CreateTrashCheckpoint(0, 1440, -2266.30273, -206.86926, 34.86986,   0.00000, 0.00000, 90.00000);
	CreateTrashCheckpoint(0, 1440, -1995.48499, -771.90076, 31.56746,   0.00000, 0.00000, 90.00000);
		//Ruta 1
	CreateTrashCheckpoint(1, 1440, -2211.24414, -964.98248, 38.68247,   0.00000, 2.50000, 286.39740);
	CreateTrashCheckpoint(1, 1440, -2424.54443, -595.71997, 131.88649,   0.00000, -4.50000, 303.65921);
	CreateTrashCheckpoint(1, 1440, -2602.82544, -478.18484, 68.15035,   0.00000, 0.00000, 21.71607);
	CreateTrashCheckpoint(1, 1440, -2353.62256, -396.48764, 78.42006,   0.00000, -3.50000, 305.07410);
	CreateTrashCheckpoint(1, 1440, -2756.29199, -500.66721, 6.77589,   0.00000, 0.00000, 316.44635);
	CreateTrashCheckpoint(1, 1440, -2811.07056, -346.85269, 6.66456,   0.00000, 0.00000, 259.86459);
	CreateTrashCheckpoint(1, 1440, -2415.51563, -128.34013, 34.82273,   0.00000, 0.00000, 250.60507);
	CreateTrashCheckpoint(1, 1440, -2028.09985, -76.42242, 34.70956,   0.00000, 0.00000, 0.00000);
		//Ruta 2
	CreateTrashCheckpoint(2, 1440, -2193.88306, -2156.55347, 46.46848,   0.00000, -10.00000, 46.48320);
	CreateTrashCheckpoint(2, 1440, -2208.01904, -2280.10645, 30.12751,   0.00000, 0.00000, 331.91882);
	CreateTrashCheckpoint(2, 1440, -2125.33594, -2256.24902, 30.10599,   0.00000, 0.00000, 326.47290);
	CreateTrashCheckpoint(2, 1440, -2101.86353, -2313.20093, 30.14946,   0.00000, 0.00000, 42.68287);
	CreateTrashCheckpoint(2, 1440, -2183.69775, -2449.03003, 30.01783,   0.00000, 0.00000, 51.16318);
	CreateTrashCheckpoint(2, 1440, -2328.98047, -2704.17358, 44.41791,   6.17998, 5.88000, 340.20822);
	CreateTrashCheckpoint(2, 1440, -2601.78345, -2334.39746, 10.50614,   0.00000, 0.00000, 41.64481);
	
	//Leñador arboles
	Lumberjack_Area = CreateDynamicRectangle(-613.953796, -209.777252, -381.961181, -6.355076, 0, 0);
	for(new i = 0; i != sizeof LUMBER_TREES; i ++)
	{
		LUMBER_TREES[i][lumber_tree_OBJECT_ID] = CreateDynamicObject(LUMBER_TREES[i][lumber_tree_MODELID], LUMBER_TREES[i][lumber_tree_X], LUMBER_TREES[i][lumber_tree_Y], LUMBER_TREES[i][lumber_tree_Z], LUMBER_TREES[i][lumber_tree_RX], LUMBER_TREES[i][lumber_tree_RY], LUMBER_TREES[i][lumber_tree_RZ], 0, 0);
		
		new label_str[256];
		format(label_str, sizeof label_str, "{f46530}Árbol (%d)\n\n{FFFFFF}Pulsa {f9d834}~k~~CONVERSATION_NO~ {FFFFFF}para talar el árbol.", i);
		LUMBER_TREES[i][lumber_tree_LABEL] = CreateDynamic3DTextLabel(label_str, 0xFFFFFFFF, LUMBER_TREES[i][lumber_tree_X], LUMBER_TREES[i][lumber_tree_Y], LUMBER_TREES[i][lumber_tree_Z] + 1.5, 5.0, .testlos = false, .worldid = 0, .interiorid = 0);
	}
	
	//Agricultor
	Farmer_Area = CreateDynamicCircle(1461.8894, -83.2621, 65.0, 0, 0);
	CreateDynamicMapIcon(1461.8894, -83.2621, 65.0, 53, -1, 0, 0);
	CreateDynamic3DTextLabel("Usa {"#PRIMARY_COLOR"}/semillas {FFFFFF}para comprar semillas", 0xFFFFFFFF, 1566.521606, 31.370532, 24.16406, 5.0, .testlos = true, .worldid = 0, .interiorid = 0);
	
	//Cosechador
	Harvest_Area = CreateDynamicRectangle(-428.336059, -1667.658569, -116.565414, -1220.122070, 0, 0);
	return 1;
}



stock Float:GetSpeed(Float:vx, Float:vy, Float:vz)
{
	new Float:vel = floatmul(floatsqroot(floatadd(floatadd(floatpower(vx, 2), floatpower(vy, 2)),  floatpower(vz, 2))), 181.5);
	return vel;
}


//By Kar
stock IsVehicleDrivingForward(vehicleid)
{
    new Float:Float[3];
    if(GetVehicleVelocity(vehicleid, Float[1], Float[2], Float[0]))
    {
        GetVehicleZAngle(vehicleid, Float[0]);
        if(Float[0] < 90)
        {
            if(Float[1] < 0 && Float[2] > 0) return true;
        }
        else if(Float[0] < 180)
        {
            if(Float[1] < 0 && Float[2] < 0) return true;
        }
        else if(Float[0] < 270)
        {
            if(Float[1] > 0 && Float[2] < 0) return true;
        }
        else if(Float[1] > 0 && Float[2] > 0) return true;
    }
    return false;
}

CreateTrashCheckpoint(route, modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	if(route >= MAX_ROUTES) return print("---> Límite superado en array 'TRASH_OBJECTS, route' en la función 'CreateTrashCheckpoint'.");
	if(!TRASH_OBJECTS[route][trash_route_VALID]) TRASH_OBJECTS[route][trash_route_VALID] = true;
	if(TRASH_OBJECTS[route][trash_route_OBJECTS] >= MAX_OBJECTS_PER_ROUTE) return print("---> Límite superado en array 'TRASH_OBJECTS, objects' en la función 'CreateTrashCheckpoint'.");

	if(TRASH_OBJECTS[route][trash_route_OBJECTS] == 0) TRASH_OBJECTS[route][trash_route_DISTANCE] = GetDistanceBetweenPoints3D(x, y, z, obtain_work_coords[WORK_TRASH][obtain_work_X], obtain_work_coords[WORK_TRASH][obtain_work_Y], obtain_work_coords[WORK_TRASH][obtain_work_Z]);
	else
	{
		TRASH_OBJECTS[route][trash_route_DISTANCE] -= GetDistanceBetweenPoints3D(TRASH_OBJECTS[route][trash_object_X][ TRASH_OBJECTS[route][trash_route_OBJECTS] - 1], TRASH_OBJECTS[route][trash_object_Y][ TRASH_OBJECTS[route][trash_route_OBJECTS] - 1 ], TRASH_OBJECTS[route][trash_object_Z][ TRASH_OBJECTS[route][trash_route_OBJECTS] - 1 ], obtain_work_coords[WORK_TRASH][obtain_work_X], obtain_work_coords[WORK_TRASH][obtain_work_Y], obtain_work_coords[WORK_TRASH][obtain_work_Z]);
		TRASH_OBJECTS[route][trash_route_DISTANCE] += GetDistanceBetweenPoints3D(x, y, z, TRASH_OBJECTS[route][trash_object_X][ TRASH_OBJECTS[route][trash_route_OBJECTS] - 1], TRASH_OBJECTS[route][trash_object_Y][ TRASH_OBJECTS[route][trash_route_OBJECTS] - 1 ], TRASH_OBJECTS[route][trash_object_Z][ TRASH_OBJECTS[route][trash_route_OBJECTS] - 1 ]);
	}
	
	TRASH_OBJECTS[route][trash_object_MODELID][ TRASH_OBJECTS[route][trash_route_OBJECTS] ] = modelid;
	TRASH_OBJECTS[route][trash_object_X][ TRASH_OBJECTS[route][trash_route_OBJECTS] ] = x;
	TRASH_OBJECTS[route][trash_object_Y][ TRASH_OBJECTS[route][trash_route_OBJECTS] ] = y;
	TRASH_OBJECTS[route][trash_object_Z][ TRASH_OBJECTS[route][trash_route_OBJECTS] ] = z;
	TRASH_OBJECTS[route][trash_object_RX][ TRASH_OBJECTS[route][trash_route_OBJECTS] ] = rx;
	TRASH_OBJECTS[route][trash_object_RY][ TRASH_OBJECTS[route][trash_route_OBJECTS] ] = ry;
	TRASH_OBJECTS[route][trash_object_RZ][ TRASH_OBJECTS[route][trash_route_OBJECTS] ] = rz;
	//CreateDynamicObject(modelid, x, y, z, rx, ry, rz, 0, 0);
	
	TRASH_OBJECTS[route][trash_route_DISTANCE] += GetDistanceBetweenPoints3D(x, y, z, obtain_work_coords[WORK_TRASH][obtain_work_X], obtain_work_coords[WORK_TRASH][obtain_work_Y], obtain_work_coords[WORK_TRASH][obtain_work_Z]);
	TRASH_OBJECTS[route][trash_route_MONEY] = floatround(0.3 * TRASH_OBJECTS[route][trash_route_DISTANCE]);
	
	TRASH_OBJECTS[route][trash_route_OBJECTS] ++;
	return 1;
}
	
stock Float:GetDistanceBetweenPoints3D(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2)
{
    return VectorSize(x1-x2,y1-y2,z1-z2);
}

CreateTextDraws()
{
	for(new i; i < sizeof(Textdraws); i++) Textdraws[Textdraws_Enum:i] = INVALID_TEXT_DRAW;
	
	Textdraws[textdraw_COMBAT_MODE] = TextDrawCreate(87.000000, 315.000000, "En_combate");
	TextDrawLetterSize(Textdraws[textdraw_COMBAT_MODE], 0.267000, 1.521185);
	TextDrawTextSize(Textdraws[textdraw_COMBAT_MODE], 0.000000, 98.000000);
	TextDrawAlignment(Textdraws[textdraw_COMBAT_MODE], 2);
	TextDrawColor(Textdraws[textdraw_COMBAT_MODE], -16776961);
	TextDrawSetShadow(Textdraws[textdraw_COMBAT_MODE], 0);
	TextDrawSetOutline(Textdraws[textdraw_COMBAT_MODE], 1);
	TextDrawBackgroundColor(Textdraws[textdraw_COMBAT_MODE], 255);
	TextDrawFont(Textdraws[textdraw_COMBAT_MODE], 2);
	TextDrawSetProportional(Textdraws[textdraw_COMBAT_MODE], 1);

	Textdraws[textdraw_HUD][0] = TextDrawCreate(-15.000000, 446.000000, "barraReputacion");
	TextDrawLetterSize(Textdraws[textdraw_HUD][0], 0.000000, 0.500000);
	TextDrawTextSize(Textdraws[textdraw_HUD][0], 650.000000, 0.000000);
	TextDrawAlignment(Textdraws[textdraw_HUD][0], 1);
	TextDrawColor(Textdraws[textdraw_HUD][0], -1);
	TextDrawUseBox(Textdraws[textdraw_HUD][0], 1);
	TextDrawBoxColor(Textdraws[textdraw_HUD][0], 140);
	TextDrawSetShadow(Textdraws[textdraw_HUD][0], 0);
	TextDrawSetOutline(Textdraws[textdraw_HUD][0], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_HUD][0], 255);
	TextDrawFont(Textdraws[textdraw_HUD][0], 1);
	TextDrawSetProportional(Textdraws[textdraw_HUD][0], 1);
	TextDrawSetShadow(Textdraws[textdraw_HUD][0], 0);

	Textdraws[textdraw_HUD][1] = TextDrawCreate(497.000000, 112.000000, "hud:radar_datefood"); //Icono comida 
	TextDrawLetterSize(Textdraws[textdraw_HUD][1], 0.000000, 0.000000);
	TextDrawTextSize(Textdraws[textdraw_HUD][1], 12.000000, 10.000000);
	TextDrawAlignment(Textdraws[textdraw_HUD][1], 1);
	TextDrawColor(Textdraws[textdraw_HUD][1], -1);
	TextDrawSetShadow(Textdraws[textdraw_HUD][1], 0);
	TextDrawSetOutline(Textdraws[textdraw_HUD][1], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_HUD][1], 255);
	TextDrawFont(Textdraws[textdraw_HUD][1], 4);
	TextDrawSetProportional(Textdraws[textdraw_HUD][1], 0);
	TextDrawSetShadow(Textdraws[textdraw_HUD][1], 0);

	Textdraws[textdraw_HUD][2] = TextDrawCreate(499.000000, 102.000000, "bordebarraHambre");
	TextDrawLetterSize(Textdraws[textdraw_HUD][2], 0.000000, 0.699999);
	TextDrawTextSize(Textdraws[textdraw_HUD][2], 549.000000, 0.000000);
	TextDrawAlignment(Textdraws[textdraw_HUD][2], 1);
	TextDrawColor(Textdraws[textdraw_HUD][2], -1);
	TextDrawUseBox(Textdraws[textdraw_HUD][2], 1);
	TextDrawBoxColor(Textdraws[textdraw_HUD][2], 255);
	TextDrawSetShadow(Textdraws[textdraw_HUD][2], 0);
	TextDrawSetOutline(Textdraws[textdraw_HUD][2], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_HUD][2], 255);
	TextDrawFont(Textdraws[textdraw_HUD][2], 1);
	TextDrawSetProportional(Textdraws[textdraw_HUD][2], 1);
	TextDrawSetShadow(Textdraws[textdraw_HUD][2], 0);

	Textdraws[textdraw_HUD][3] = TextDrawCreate(500.699981, 104.000000, "barraHambre");
	TextDrawLetterSize(Textdraws[textdraw_HUD][3], 0.000000, 0.238333);
	TextDrawTextSize(Textdraws[textdraw_HUD][3], 546.730102, 0.000000);
	TextDrawAlignment(Textdraws[textdraw_HUD][3], 1);
	TextDrawColor(Textdraws[textdraw_HUD][3], -1);
	TextDrawUseBox(Textdraws[textdraw_HUD][3], 1);
	TextDrawBoxColor(Textdraws[textdraw_HUD][3], 1414856922);
	TextDrawSetShadow(Textdraws[textdraw_HUD][3], 0);
	TextDrawSetOutline(Textdraws[textdraw_HUD][3], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_HUD][3], 255);
	TextDrawFont(Textdraws[textdraw_HUD][3], 1);
	TextDrawSetProportional(Textdraws[textdraw_HUD][3], 1);
	TextDrawSetShadow(Textdraws[textdraw_HUD][3], 0);

	Textdraws[textdraw_HUD][4] = TextDrawCreate(556.000000, 112.000000, "hud:radar_datedrink"); //Icono bebida
	TextDrawLetterSize(Textdraws[textdraw_HUD][4], 0.000000, 0.000000);
	TextDrawTextSize(Textdraws[textdraw_HUD][4], 9.000000, 10.000000);
	TextDrawAlignment(Textdraws[textdraw_HUD][4], 1);
	TextDrawColor(Textdraws[textdraw_HUD][4], -1);
	TextDrawSetShadow(Textdraws[textdraw_HUD][4], 0);
	TextDrawSetOutline(Textdraws[textdraw_HUD][4], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_HUD][4], 255);
	TextDrawFont(Textdraws[textdraw_HUD][4], 4);
	TextDrawSetProportional(Textdraws[textdraw_HUD][4], 0);
	TextDrawSetShadow(Textdraws[textdraw_HUD][4], 0);

	Textdraws[textdraw_HUD][5] = TextDrawCreate(556.000000, 102.000000, "bordeBarraSed");
	TextDrawLetterSize(Textdraws[textdraw_HUD][5], 0.000000, 0.699999);
	TextDrawTextSize(Textdraws[textdraw_HUD][5], 606.000000, 0.000000);
	TextDrawAlignment(Textdraws[textdraw_HUD][5], 1);
	TextDrawColor(Textdraws[textdraw_HUD][5], -1);
	TextDrawUseBox(Textdraws[textdraw_HUD][5], 1);
	TextDrawBoxColor(Textdraws[textdraw_HUD][5], 255);
	TextDrawSetShadow(Textdraws[textdraw_HUD][5], 0);
	TextDrawSetOutline(Textdraws[textdraw_HUD][5], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_HUD][5], 255);
	TextDrawFont(Textdraws[textdraw_HUD][5], 1);
	TextDrawSetProportional(Textdraws[textdraw_HUD][5], 1);
	TextDrawSetShadow(Textdraws[textdraw_HUD][5], 0);

	Textdraws[textdraw_HUD][6] = TextDrawCreate(557.699981, 104.000000, "barraSed");
	TextDrawLetterSize(Textdraws[textdraw_HUD][6], 0.000000, 0.238333);
	TextDrawTextSize(Textdraws[textdraw_HUD][6], 603.730102, 0.000000);
	TextDrawAlignment(Textdraws[textdraw_HUD][6], 1);
	TextDrawColor(Textdraws[textdraw_HUD][6], -1);
	TextDrawUseBox(Textdraws[textdraw_HUD][6], 1);
	TextDrawBoxColor(Textdraws[textdraw_HUD][6], 8173202);
	TextDrawSetShadow(Textdraws[textdraw_HUD][6], 0);
	TextDrawSetOutline(Textdraws[textdraw_HUD][6], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_HUD][6], 255);
	TextDrawFont(Textdraws[textdraw_HUD][6], 1);
	TextDrawSetProportional(Textdraws[textdraw_HUD][6], 1);
	TextDrawSetShadow(Textdraws[textdraw_HUD][6], 0);
	
	Textdraws[textdraw_GPS_MAP][0] = TextDrawCreate(-5.000000, 132.000000, "box");
	TextDrawLetterSize(Textdraws[textdraw_GPS_MAP][0], 0.000000, 22.498472);
	TextDrawTextSize(Textdraws[textdraw_GPS_MAP][0], 180.130477, 0.000000);
	TextDrawAlignment(Textdraws[textdraw_GPS_MAP][0], 1);
	TextDrawColor(Textdraws[textdraw_GPS_MAP][0], -1);
	TextDrawUseBox(Textdraws[textdraw_GPS_MAP][0], 1);
	TextDrawBoxColor(Textdraws[textdraw_GPS_MAP][0], 120);
	TextDrawSetShadow(Textdraws[textdraw_GPS_MAP][0], 0);
	TextDrawSetOutline(Textdraws[textdraw_GPS_MAP][0], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_GPS_MAP][0], 255);
	TextDrawFont(Textdraws[textdraw_GPS_MAP][0], 1);
	TextDrawSetProportional(Textdraws[textdraw_GPS_MAP][0], 1);
	TextDrawSetShadow(Textdraws[textdraw_GPS_MAP][0], 0);

	Textdraws[textdraw_GPS_MAP][1] = TextDrawCreate(map_td_X, map_td_Y, "samaps:map");
	TextDrawLetterSize(Textdraws[textdraw_GPS_MAP][1], 0.000000, 0.000000);
	TextDrawTextSize(Textdraws[textdraw_GPS_MAP][1], map_td_SIZE_X, map_td_SIZE_Y);
	TextDrawAlignment(Textdraws[textdraw_GPS_MAP][1], 1);
	TextDrawColor(Textdraws[textdraw_GPS_MAP][1], -1);
	TextDrawSetShadow(Textdraws[textdraw_GPS_MAP][1], 0);
	TextDrawSetOutline(Textdraws[textdraw_GPS_MAP][1], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_GPS_MAP][1], 255);
	TextDrawFont(Textdraws[textdraw_GPS_MAP][1], 4);
	TextDrawSetProportional(Textdraws[textdraw_GPS_MAP][1], 0);
	TextDrawSetShadow(Textdraws[textdraw_GPS_MAP][1], 0);

	Textdraws[textdraw_GPS_MAP][2] = TextDrawCreate(90.000000, 324.000000, "GPS_-_/mapa");
	TextDrawLetterSize(Textdraws[textdraw_GPS_MAP][2], 0.197330, 1.027553);
	TextDrawAlignment(Textdraws[textdraw_GPS_MAP][2], 2);
	TextDrawColor(Textdraws[textdraw_GPS_MAP][2], -186);
	TextDrawSetShadow(Textdraws[textdraw_GPS_MAP][2], 0);
	TextDrawSetOutline(Textdraws[textdraw_GPS_MAP][2], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_GPS_MAP][2], 255);
	TextDrawFont(Textdraws[textdraw_GPS_MAP][2], 1);
	TextDrawSetProportional(Textdraws[textdraw_GPS_MAP][2], 1);
	TextDrawSetShadow(Textdraws[textdraw_GPS_MAP][2], 0);

	Textdraws[textdraw_GPS_MAP][3] = TextDrawCreate(map_td_X, map_td_Y, "mdl-1000:lc_map");
	TextDrawLetterSize(Textdraws[textdraw_GPS_MAP][3], 0.000000, 0.000000);
	TextDrawTextSize(Textdraws[textdraw_GPS_MAP][3], map_td_SIZE_X, map_td_SIZE_Y);
	TextDrawAlignment(Textdraws[textdraw_GPS_MAP][3], 1);
	TextDrawColor(Textdraws[textdraw_GPS_MAP][3], -1);
	TextDrawSetShadow(Textdraws[textdraw_GPS_MAP][3], 0);
	TextDrawSetOutline(Textdraws[textdraw_GPS_MAP][3], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_GPS_MAP][3], 255);
	TextDrawFont(Textdraws[textdraw_GPS_MAP][3], 4);
	TextDrawSetProportional(Textdraws[textdraw_GPS_MAP][3], 0);
	TextDrawSetShadow(Textdraws[textdraw_GPS_MAP][3], 0);
	
	
	Textdraws[textdraw_PLAYER_REP_BACK][0] = TextDrawCreate(500.000000, 13.000000, "box");
	TextDrawLetterSize(Textdraws[textdraw_PLAYER_REP_BACK][0], 0.000000, 0.633333);
	TextDrawTextSize(Textdraws[textdraw_PLAYER_REP_BACK][0], 605.000000, 0.000000);
	TextDrawAlignment(Textdraws[textdraw_PLAYER_REP_BACK][0], 1);
	TextDrawColor(Textdraws[textdraw_PLAYER_REP_BACK][0], -1);
	TextDrawUseBox(Textdraws[textdraw_PLAYER_REP_BACK][0], 1);
	TextDrawBoxColor(Textdraws[textdraw_PLAYER_REP_BACK][0], 255);
	TextDrawSetShadow(Textdraws[textdraw_PLAYER_REP_BACK][0], 0);
	TextDrawSetOutline(Textdraws[textdraw_PLAYER_REP_BACK][0], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_PLAYER_REP_BACK][0], 255);
	TextDrawFont(Textdraws[textdraw_PLAYER_REP_BACK][0], 1);
	TextDrawSetProportional(Textdraws[textdraw_PLAYER_REP_BACK][0], 1);
	TextDrawSetShadow(Textdraws[textdraw_PLAYER_REP_BACK][0], 0);

	Textdraws[textdraw_PLAYER_REP_BACK][1] = TextDrawCreate(502.000000, 15.000000, "box");
	TextDrawLetterSize(Textdraws[textdraw_PLAYER_REP_BACK][1], 0.000000, 0.200000);
	TextDrawTextSize(Textdraws[textdraw_PLAYER_REP_BACK][1], 603.000000, 0.000000);
	TextDrawAlignment(Textdraws[textdraw_PLAYER_REP_BACK][1], 1);
	TextDrawColor(Textdraws[textdraw_PLAYER_REP_BACK][1], -1);
	TextDrawUseBox(Textdraws[textdraw_PLAYER_REP_BACK][1], 1);
	TextDrawBoxColor(Textdraws[textdraw_PLAYER_REP_BACK][1], 5787296);
	TextDrawSetShadow(Textdraws[textdraw_PLAYER_REP_BACK][1], 0);
	TextDrawSetOutline(Textdraws[textdraw_PLAYER_REP_BACK][1], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_PLAYER_REP_BACK][1], 255);
	TextDrawFont(Textdraws[textdraw_PLAYER_REP_BACK][1], 1);
	TextDrawSetProportional(Textdraws[textdraw_PLAYER_REP_BACK][1], 1);
	TextDrawSetShadow(Textdraws[textdraw_PLAYER_REP_BACK][1], 0);

	Textdraws[textdraw_LOAD_SCREEN][0] = TextDrawCreate(-15.000000, -4.000000, "box");
	TextDrawLetterSize(Textdraws[textdraw_LOAD_SCREEN][0], 0.000000, 50.866653);
	TextDrawTextSize(Textdraws[textdraw_LOAD_SCREEN][0], 728.000000, 0.000000);
	TextDrawAlignment(Textdraws[textdraw_LOAD_SCREEN][0], 1);
	TextDrawColor(Textdraws[textdraw_LOAD_SCREEN][0], -1);
	TextDrawUseBox(Textdraws[textdraw_LOAD_SCREEN][0], 1);
	TextDrawBoxColor(Textdraws[textdraw_LOAD_SCREEN][0], 0x000000FF);
	TextDrawSetShadow(Textdraws[textdraw_LOAD_SCREEN][0], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_LOAD_SCREEN][0], 255);
	TextDrawFont(Textdraws[textdraw_LOAD_SCREEN][0], 1);
	TextDrawSetProportional(Textdraws[textdraw_LOAD_SCREEN][0], 1);

	Textdraws[textdraw_LOAD_SCREEN][1] = TextDrawCreate(-15.000000, -4.000000, "box");
	TextDrawLetterSize(Textdraws[textdraw_LOAD_SCREEN][1], 0.000000, 50.866653);
	TextDrawTextSize(Textdraws[textdraw_LOAD_SCREEN][1], 728.000000, 0.000000);
	TextDrawAlignment(Textdraws[textdraw_LOAD_SCREEN][1], 1);
	TextDrawColor(Textdraws[textdraw_LOAD_SCREEN][1], -1);
	TextDrawUseBox(Textdraws[textdraw_LOAD_SCREEN][1], 1);
	TextDrawBoxColor(Textdraws[textdraw_LOAD_SCREEN][1], 90);
	TextDrawSetShadow(Textdraws[textdraw_LOAD_SCREEN][1], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_LOAD_SCREEN][1], 255);
	TextDrawFont(Textdraws[textdraw_LOAD_SCREEN][1], 1);
	TextDrawSetProportional(Textdraws[textdraw_LOAD_SCREEN][1], 1);

	Textdraws[textdraw_LOAD_SCREEN][2] = TextDrawCreate(320.000000, 80.000000, SERVER_SHORT_NAME);
	TextDrawLetterSize(Textdraws[textdraw_LOAD_SCREEN][2], 0.505666, 2.649481);
	TextDrawAlignment(Textdraws[textdraw_LOAD_SCREEN][2], 2);
	TextDrawColor(Textdraws[textdraw_LOAD_SCREEN][2], -1);
	TextDrawSetShadow(Textdraws[textdraw_LOAD_SCREEN][2], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_LOAD_SCREEN][2], 255);
	TextDrawFont(Textdraws[textdraw_LOAD_SCREEN][2], 2);
	TextDrawSetProportional(Textdraws[textdraw_LOAD_SCREEN][2], 1);

	Textdraws[textdraw_LOAD_SCREEN][3] = TextDrawCreate(320.000000, 99.000000, "ROLEPLAY");
	TextDrawLetterSize(Textdraws[textdraw_LOAD_SCREEN][3], 0.311000, 1.811555);
	TextDrawAlignment(Textdraws[textdraw_LOAD_SCREEN][3], 2);
	TextDrawColor(Textdraws[textdraw_LOAD_SCREEN][3], -1);
	TextDrawSetShadow(Textdraws[textdraw_LOAD_SCREEN][3], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_LOAD_SCREEN][3], 255);
	TextDrawFont(Textdraws[textdraw_LOAD_SCREEN][3], 2);
	TextDrawSetProportional(Textdraws[textdraw_LOAD_SCREEN][3], 1);
	
	Textdraws[textdraw_SERVER_TIME] = TextDrawCreate(578.000000, 22.000000, "00:00");
	TextDrawLetterSize(Textdraws[textdraw_SERVER_TIME], 0.585666, 2.189037);
	TextDrawAlignment(Textdraws[textdraw_SERVER_TIME], 2);
	TextDrawColor(Textdraws[textdraw_SERVER_TIME], -1);
	TextDrawSetShadow(Textdraws[textdraw_SERVER_TIME], 0);
	TextDrawSetOutline(Textdraws[textdraw_SERVER_TIME], 2);
	TextDrawBackgroundColor(Textdraws[textdraw_SERVER_TIME], 255);
	TextDrawFont(Textdraws[textdraw_SERVER_TIME], 3);
	TextDrawSetProportional(Textdraws[textdraw_SERVER_TIME], 1);
	
	Textdraws[textdraw_SHOP][0] = TextDrawCreate(320.000000, 120.000000, "box");
	TextDrawLetterSize(Textdraws[textdraw_SHOP][0], 0.000000, 24.936986);
	TextDrawTextSize(Textdraws[textdraw_SHOP][0], 0.000000, 254.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP][0], 2);
	TextDrawColor(Textdraws[textdraw_SHOP][0], -1);
	TextDrawUseBox(Textdraws[textdraw_SHOP][0], 1);
	TextDrawBoxColor(Textdraws[textdraw_SHOP][0], 201);
	TextDrawSetShadow(Textdraws[textdraw_SHOP][0], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP][0], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP][0], 255);
	TextDrawFont(Textdraws[textdraw_SHOP][0], 1);
	TextDrawSetProportional(Textdraws[textdraw_SHOP][0], 1);
	TextDrawSetShadow(Textdraws[textdraw_SHOP][0], 0);

	Textdraws[textdraw_SHOP][1] = TextDrawCreate(320.000000, 125.000000, "box");
	TextDrawLetterSize(Textdraws[textdraw_SHOP][1], 0.000000, 23.766672);
	TextDrawTextSize(Textdraws[textdraw_SHOP][1], 0.000000, 254.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP][1], 2);
	TextDrawColor(Textdraws[textdraw_SHOP][1], -1);
	TextDrawUseBox(Textdraws[textdraw_SHOP][1], 1);
	TextDrawBoxColor(Textdraws[textdraw_SHOP][1], 11117257);
	TextDrawSetShadow(Textdraws[textdraw_SHOP][1], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP][1], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP][1], 255);
	TextDrawFont(Textdraws[textdraw_SHOP][1], 1);
	TextDrawSetProportional(Textdraws[textdraw_SHOP][1], 1);
	TextDrawSetShadow(Textdraws[textdraw_SHOP][1], 0);

	Textdraws[textdraw_SHOP][2] = TextDrawCreate(207.000000, 128.000000, "Compra_y_venta");
	TextDrawLetterSize(Textdraws[textdraw_SHOP][2], 0.295333, 1.388446);
	TextDrawAlignment(Textdraws[textdraw_SHOP][2], 1);
	TextDrawColor(Textdraws[textdraw_SHOP][2], -1);
	TextDrawSetShadow(Textdraws[textdraw_SHOP][2], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP][2], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP][2], 255);
	TextDrawFont(Textdraws[textdraw_SHOP][2], 1);
	TextDrawSetProportional(Textdraws[textdraw_SHOP][2], 1);
	TextDrawSetShadow(Textdraws[textdraw_SHOP][2], 0);

	Textdraws[textdraw_SHOP][3] = TextDrawCreate(320.000000, 145.000000, "box");
	TextDrawLetterSize(Textdraws[textdraw_SHOP][3], 0.000000, -0.266666);
	TextDrawTextSize(Textdraws[textdraw_SHOP][3], 0.000000, 222.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP][3], 2);
	TextDrawColor(Textdraws[textdraw_SHOP][3], -1);
	TextDrawUseBox(Textdraws[textdraw_SHOP][3], 1);
	TextDrawBoxColor(Textdraws[textdraw_SHOP][3], 255);
	TextDrawSetShadow(Textdraws[textdraw_SHOP][3], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP][3], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP][3], 255);
	TextDrawFont(Textdraws[textdraw_SHOP][3], 1);
	TextDrawSetProportional(Textdraws[textdraw_SHOP][3], 1);
	TextDrawSetShadow(Textdraws[textdraw_SHOP][3], 0);

	Textdraws[textdraw_SHOP][4] = TextDrawCreate(320.000000, 301.000000, "box");
	TextDrawLetterSize(Textdraws[textdraw_SHOP][4], 0.000000, -0.266666);
	TextDrawTextSize(Textdraws[textdraw_SHOP][4], 0.000000, 222.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP][4], 2);
	TextDrawColor(Textdraws[textdraw_SHOP][4], -1);
	TextDrawUseBox(Textdraws[textdraw_SHOP][4], 1);
	TextDrawBoxColor(Textdraws[textdraw_SHOP][4], 255);
	TextDrawSetShadow(Textdraws[textdraw_SHOP][4], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP][4], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP][4], 255);
	TextDrawFont(Textdraws[textdraw_SHOP][4], 1);
	TextDrawSetProportional(Textdraws[textdraw_SHOP][4], 1);
	TextDrawSetShadow(Textdraws[textdraw_SHOP][4], 0);
	
	
	Textdraws[textdraw_SHOP_ARTICLES_BOX][0] = TextDrawCreate(208.000000, 148.000000, "LD_SPAC:white");
	TextDrawLetterSize(Textdraws[textdraw_SHOP_ARTICLES_BOX][0], 0.000000, 0.000000);
	TextDrawTextSize(Textdraws[textdraw_SHOP_ARTICLES_BOX][0], 70.000000, 70.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP_ARTICLES_BOX][0], 1);
	TextDrawColor(Textdraws[textdraw_SHOP_ARTICLES_BOX][0], 255);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLES_BOX][0], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP_ARTICLES_BOX][0], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP_ARTICLES_BOX][0], 255);
	TextDrawFont(Textdraws[textdraw_SHOP_ARTICLES_BOX][0], 4);
	TextDrawSetProportional(Textdraws[textdraw_SHOP_ARTICLES_BOX][0], 0);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLES_BOX][0], 0);
	TextDrawSetSelectable(Textdraws[textdraw_SHOP_ARTICLES_BOX][0], true);

	Textdraws[textdraw_SHOP_ARTICLES_BOX][1] = TextDrawCreate(285.000000, 148.000000, "LD_SPAC:white");
	TextDrawLetterSize(Textdraws[textdraw_SHOP_ARTICLES_BOX][1], 0.000000, 0.000000);
	TextDrawTextSize(Textdraws[textdraw_SHOP_ARTICLES_BOX][1], 70.000000, 70.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP_ARTICLES_BOX][1], 1);
	TextDrawColor(Textdraws[textdraw_SHOP_ARTICLES_BOX][1], 255);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLES_BOX][1], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP_ARTICLES_BOX][1], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP_ARTICLES_BOX][1], 255);
	TextDrawFont(Textdraws[textdraw_SHOP_ARTICLES_BOX][1], 4);
	TextDrawSetProportional(Textdraws[textdraw_SHOP_ARTICLES_BOX][1], 0);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLES_BOX][1], 0);
	TextDrawSetSelectable(Textdraws[textdraw_SHOP_ARTICLES_BOX][1], true);

	Textdraws[textdraw_SHOP_ARTICLES_BOX][2] = TextDrawCreate(362.000000, 148.000000, "LD_SPAC:white");
	TextDrawLetterSize(Textdraws[textdraw_SHOP_ARTICLES_BOX][2], 0.000000, 0.000000);
	TextDrawTextSize(Textdraws[textdraw_SHOP_ARTICLES_BOX][2], 70.000000, 70.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP_ARTICLES_BOX][2], 1);
	TextDrawColor(Textdraws[textdraw_SHOP_ARTICLES_BOX][2], 255);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLES_BOX][2], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP_ARTICLES_BOX][2], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP_ARTICLES_BOX][2], 255);
	TextDrawFont(Textdraws[textdraw_SHOP_ARTICLES_BOX][2], 4);
	TextDrawSetProportional(Textdraws[textdraw_SHOP_ARTICLES_BOX][2], 0);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLES_BOX][2], 0);
	TextDrawSetSelectable(Textdraws[textdraw_SHOP_ARTICLES_BOX][2], true);

	Textdraws[textdraw_SHOP_ARTICLES_BOX][3] = TextDrawCreate(208.000000, 225.000000, "LD_SPAC:white");
	TextDrawLetterSize(Textdraws[textdraw_SHOP_ARTICLES_BOX][3], 0.000000, 0.000000);
	TextDrawTextSize(Textdraws[textdraw_SHOP_ARTICLES_BOX][3], 70.000000, 70.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP_ARTICLES_BOX][3], 1);
	TextDrawColor(Textdraws[textdraw_SHOP_ARTICLES_BOX][3], 255);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLES_BOX][3], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP_ARTICLES_BOX][3], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP_ARTICLES_BOX][3], 255);
	TextDrawFont(Textdraws[textdraw_SHOP_ARTICLES_BOX][3], 4);
	TextDrawSetProportional(Textdraws[textdraw_SHOP_ARTICLES_BOX][3], 0);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLES_BOX][3], 0);
	TextDrawSetSelectable(Textdraws[textdraw_SHOP_ARTICLES_BOX][3], true);

	Textdraws[textdraw_SHOP_ARTICLES_BOX][4] = TextDrawCreate(285.000000, 225.000000, "LD_SPAC:white");
	TextDrawLetterSize(Textdraws[textdraw_SHOP_ARTICLES_BOX][4], 0.000000, 0.000000);
	TextDrawTextSize(Textdraws[textdraw_SHOP_ARTICLES_BOX][4], 70.000000, 70.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP_ARTICLES_BOX][4], 1);
	TextDrawColor(Textdraws[textdraw_SHOP_ARTICLES_BOX][4], 255);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLES_BOX][4], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP_ARTICLES_BOX][4], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP_ARTICLES_BOX][4], 255);
	TextDrawFont(Textdraws[textdraw_SHOP_ARTICLES_BOX][4], 4);
	TextDrawSetProportional(Textdraws[textdraw_SHOP_ARTICLES_BOX][4], 0);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLES_BOX][4], 0);
	TextDrawSetSelectable(Textdraws[textdraw_SHOP_ARTICLES_BOX][4], true);

	Textdraws[textdraw_SHOP_ARTICLES_BOX][5] = TextDrawCreate(362.000000, 225.000000, "LD_SPAC:white");
	TextDrawLetterSize(Textdraws[textdraw_SHOP_ARTICLES_BOX][5], 0.000000, 0.000000);
	TextDrawTextSize(Textdraws[textdraw_SHOP_ARTICLES_BOX][5], 70.000000, 70.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP_ARTICLES_BOX][5], 1);
	TextDrawColor(Textdraws[textdraw_SHOP_ARTICLES_BOX][5], 255);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLES_BOX][5], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP_ARTICLES_BOX][5], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP_ARTICLES_BOX][5], 255);
	TextDrawFont(Textdraws[textdraw_SHOP_ARTICLES_BOX][5], 4);
	TextDrawSetProportional(Textdraws[textdraw_SHOP_ARTICLES_BOX][5], 0);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLES_BOX][5], 0);
	TextDrawSetSelectable(Textdraws[textdraw_SHOP_ARTICLES_BOX][5], true);
	
	Textdraws[textdraw_SHOP_ARTICLE_BOX] = TextDrawCreate(208.000000, 175.000000, "LD_SPAC:white");
	TextDrawLetterSize(Textdraws[textdraw_SHOP_ARTICLE_BOX], 0.000000, 0.000000);
	TextDrawTextSize(Textdraws[textdraw_SHOP_ARTICLE_BOX], 100.000000, 100.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP_ARTICLE_BOX], 1);
	TextDrawColor(Textdraws[textdraw_SHOP_ARTICLE_BOX], 255);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLE_BOX], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP_ARTICLE_BOX], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP_ARTICLE_BOX], 255);
	TextDrawFont(Textdraws[textdraw_SHOP_ARTICLE_BOX], 4);
	TextDrawSetProportional(Textdraws[textdraw_SHOP_ARTICLE_BOX], 0);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_ARTICLE_BOX], 0);
	
	Textdraws[textdraw_TERRITORY_BOX] = TextDrawCreate(320.000000, 355.000000, "-");
	TextDrawLetterSize(Textdraws[textdraw_TERRITORY_BOX], 12.321674, 9.937778);
	TextDrawAlignment(Textdraws[textdraw_TERRITORY_BOX], 2);
	TextDrawColor(Textdraws[textdraw_TERRITORY_BOX], 167);
	TextDrawSetShadow(Textdraws[textdraw_TERRITORY_BOX], 0);
	TextDrawSetOutline(Textdraws[textdraw_TERRITORY_BOX], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_TERRITORY_BOX], 255);
	TextDrawFont(Textdraws[textdraw_TERRITORY_BOX], 1);
	TextDrawSetProportional(Textdraws[textdraw_TERRITORY_BOX], 1);
	TextDrawSetShadow(Textdraws[textdraw_TERRITORY_BOX], 0);
	
	Textdraws[textdraw_SHOP_TUNING][0] = TextDrawCreate(320.000000, 330.000000, "box");
	TextDrawLetterSize(Textdraws[textdraw_SHOP_TUNING][0], 0.000000, 6.966667);
	TextDrawTextSize(Textdraws[textdraw_SHOP_TUNING][0], 0.000000, 152.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP_TUNING][0], 2);
	TextDrawColor(Textdraws[textdraw_SHOP_TUNING][0], -1);
	TextDrawUseBox(Textdraws[textdraw_SHOP_TUNING][0], 1);
	TextDrawBoxColor(Textdraws[textdraw_SHOP_TUNING][0], 95);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_TUNING][0], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP_TUNING][0], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP_TUNING][0], 255);
	TextDrawFont(Textdraws[textdraw_SHOP_TUNING][0], 1);
	TextDrawSetProportional(Textdraws[textdraw_SHOP_TUNING][0], 1);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_TUNING][0], 0);

	Textdraws[textdraw_SHOP_TUNING][1] = TextDrawCreate(320.000000, 330.000000, ""SERVER_SHORT_NAME"_Tuning_Club");
	TextDrawLetterSize(Textdraws[textdraw_SHOP_TUNING][1], 0.354000, 1.533629);
	TextDrawTextSize(Textdraws[textdraw_SHOP_TUNING][1], 0.000000, 152.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP_TUNING][1], 2);
	TextDrawColor(Textdraws[textdraw_SHOP_TUNING][1], -1);
	TextDrawUseBox(Textdraws[textdraw_SHOP_TUNING][1], 1);
	TextDrawBoxColor(Textdraws[textdraw_SHOP_TUNING][1], 10205093);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_TUNING][1], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP_TUNING][1], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP_TUNING][1], 255);
	TextDrawFont(Textdraws[textdraw_SHOP_TUNING][1], 0);
	TextDrawSetProportional(Textdraws[textdraw_SHOP_TUNING][1], 1);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_TUNING][1], 0);

	Textdraws[textdraw_SHOP_TUNING][2] = TextDrawCreate(320.000000, 376.000000, "COMPRAR");
	TextDrawLetterSize(Textdraws[textdraw_SHOP_TUNING][2], 0.219666, 1.031703);
	TextDrawTextSize(Textdraws[textdraw_SHOP_TUNING][2], 10.000000, 43.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP_TUNING][2], 2);
	TextDrawColor(Textdraws[textdraw_SHOP_TUNING][2], -1);
	TextDrawUseBox(Textdraws[textdraw_SHOP_TUNING][2], 1);
	TextDrawBoxColor(Textdraws[textdraw_SHOP_TUNING][2], -1936945921);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_TUNING][2], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP_TUNING][2], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP_TUNING][2], 255);
	TextDrawFont(Textdraws[textdraw_SHOP_TUNING][2], 1);
	TextDrawSetProportional(Textdraws[textdraw_SHOP_TUNING][2], 1);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_TUNING][2], 0);
	TextDrawSetSelectable(Textdraws[textdraw_SHOP_TUNING][2], true);

	Textdraws[textdraw_SHOP_TUNING][3] = TextDrawCreate(370.000000, 376.000000, "SIGUIENTE");
	TextDrawLetterSize(Textdraws[textdraw_SHOP_TUNING][3], 0.219666, 1.031703);
	TextDrawTextSize(Textdraws[textdraw_SHOP_TUNING][3], 10.000000, 43.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP_TUNING][3], 2);
	TextDrawColor(Textdraws[textdraw_SHOP_TUNING][3], -1);
	TextDrawUseBox(Textdraws[textdraw_SHOP_TUNING][3], 1);
	TextDrawBoxColor(Textdraws[textdraw_SHOP_TUNING][3], -1936945921);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_TUNING][3], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP_TUNING][3], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP_TUNING][3], 255);
	TextDrawFont(Textdraws[textdraw_SHOP_TUNING][3], 1);
	TextDrawSetProportional(Textdraws[textdraw_SHOP_TUNING][3], 1);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_TUNING][3], 0);
	TextDrawSetSelectable(Textdraws[textdraw_SHOP_TUNING][3], true);

	Textdraws[textdraw_SHOP_TUNING][4] = TextDrawCreate(270.000000, 376.000000, "ANTERIOR");
	TextDrawLetterSize(Textdraws[textdraw_SHOP_TUNING][4], 0.219666, 1.031703);
	TextDrawTextSize(Textdraws[textdraw_SHOP_TUNING][4], 10.000000, 43.000000);
	TextDrawAlignment(Textdraws[textdraw_SHOP_TUNING][4], 2);
	TextDrawColor(Textdraws[textdraw_SHOP_TUNING][4], -1);
	TextDrawUseBox(Textdraws[textdraw_SHOP_TUNING][4], 1);
	TextDrawBoxColor(Textdraws[textdraw_SHOP_TUNING][4], -1936945921);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_TUNING][4], 0);
	TextDrawSetOutline(Textdraws[textdraw_SHOP_TUNING][4], 0);
	TextDrawBackgroundColor(Textdraws[textdraw_SHOP_TUNING][4], 255);
	TextDrawFont(Textdraws[textdraw_SHOP_TUNING][4], 1);
	TextDrawSetProportional(Textdraws[textdraw_SHOP_TUNING][4], 1);
	TextDrawSetShadow(Textdraws[textdraw_SHOP_TUNING][4], 0);
	TextDrawSetSelectable(Textdraws[textdraw_SHOP_TUNING][4], true);
	return 1;
}

stock DestroyTextDraws()
{
	for(new i; i < sizeof(Textdraws); i++)
	{
		if(Textdraws[Textdraws_Enum:i] == INVALID_TEXT_DRAW) continue;
		TextDrawDestroy(Text:Textdraws[Textdraws_Enum:i]);
		Textdraws[Textdraws_Enum:i] = INVALID_TEXT_DRAW;
	}
	return 1;
}

#define MIN_SECONDS_BETWEEN_TALKS 100 // Deben pasar al menos 1 segundos para volver a hablar.

CMD:examen(playerid, params[])
{
	if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] != -1)
	{
		if(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] == INTERIOR_DRIVING_SCHOOL)
		{
			if(IsPlayerInRangeOfPoint(playerid, 1.5, 1063.718994, -343.093566, 2797.699951))
			{
				if(PLAYER_TEMP[playerid][pt_DL_EXAM]) SendNotification(playerid, "Ya estás en el examen, sal afuera y toma un vehículo para empezar con el examen.");
				else
				{
					if(PI[playerid][ac_DRIVE_LICENSE_POINTS] > 6) SendNotification(playerid, "Tienes más de 6 puntos del carnet, no te hace falta hacer el examen.");
					else
					{
						if(GivePlayerCash(playerid, -1750, false, true)) {
							PLAYER_TEMP[playerid][pt_DL_EXAM] = true;
							PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] = 0;
							SendNotification(playerid, "Para comenzar con el examen subete a un vehículo de la autoescuela, están en el aparcamiento.");
						}
						else SendNotification(playerid, "No tienes suficiente dinero.");
					}
				}
				return 1;
			}
		}
	}
	
	SendNotification(playerid, "No estás en el lugar adecuado.");
	return 1;
}

CMD:bebida(playerid, params[])
{
	if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] != -1)
	{
		if(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] == INTERIOR_CLUB)
		{
			if(IsPlayerInRangeOfPoint(playerid, 1.5, 1207.234375, -29.231435, 1000.953125))
			{
				if(GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_DRINK_WINE)
				{
					if(PI[playerid][ac_CASH] <= 50) return SendNotification(playerid, "La bebida cuesta 50$.");
					
					if(GivePlayerCash(playerid, -50, false, true)) {
						SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_BEER);
					}
				}
				return 1;
			}
		}
	}
	
	SendNotification(playerid, "No estás en el lugar adecuado.");
	return 1;
}

#define MIN_TIME_BETWEEN_DOUBT	60
#define IP_SPAM_MUTE_TIME		5 //minutos
CMD:duda(playerid, params[])
{
	if(!ACCOUNT_INFO[playerid][ac_DOUBT_CHANNEL]) return SendNotification(playerid, "Para enviar una duda primero debes activar el canal de dudas con ~g~/dudas");
	if(isnull(params)) return SendNotification(playerid, "~r~Modo de uso: ~w~/duda [DUDA]");
	if(PI[playerid][ac_MUTE] > gettime())
	{
		new seconds = PI[playerid][ac_MUTE] - gettime();
		SendFormatNotification(playerid, "Estás silenciado en el canal de dudas por %s minutos.", TimeConvert(seconds));
		return 1;
	}
	if(!ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL])
	{
		if(gettime() < PLAYER_TEMP[playerid][pt_DOUBT_CHANNEL_TIME] + MIN_TIME_BETWEEN_DOUBT)
		{
			new time = (MIN_TIME_BETWEEN_DOUBT-(gettime()-PLAYER_TEMP[playerid][pt_DOUBT_CHANNEL_TIME]));
			SendFormatNotification(playerid, "Tienes que esperar %s minutos para volver a realizar otra consulta.", TimeConvert(time));
			return 1;
		}
	}
	
 	SendMessageToDoubtChannel(playerid, params);
	return 1;
}

CMD:dudas(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_DOUBT_CHANNEL])
	{
		ACCOUNT_INFO[playerid][ac_DOUBT_CHANNEL] = false;
		SendNotification(playerid, "Canal de dudas ~r~deshabilitado~w~.");
	}
	else
	{
		ACCOUNT_INFO[playerid][ac_DOUBT_CHANNEL] = true;
		SendNotification(playerid, "Canal de dudas ~r~habilitado~w~.");
	}
	return 1;
}

CMD:g(playerid, params[])
{
	if(isnull(params)) return SendNotification(playerid, "~r~Modo de uso: ~w~/gritar [TEXTO]");
	
	new str_text[190];
	format(str_text, 190, "%s grita: ¡%s!", PLAYER_TEMP[playerid][pt_RP_NAME], params);
	ProxDetector(playerid, 25.0, str_text, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, 85);
	return 1;
}
alias:g("gritar");

CMD:s(playerid, params[])
{
	if(isnull(params)) return SendNotification(playerid, "~r~Modo de uso: ~w~/susurrar [TEXTO]");
	
	new str_text[190];
	format(str_text, sizeof(str_text), "%s susurra: %s", PLAYER_TEMP[playerid][pt_RP_NAME], params);
	ProxDetector(playerid, 5.0, str_text, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, 85);
	return 1;
}
alias:s("susurrar");

CMD:decir(playerid, params[])
{
	if(isnull(params)) return SendNotification(playerid, "~r~Modo de uso: ~w~/decir [TEXTO]");
	
    new str_text[190];

	format(str_text, 145, "%s dice: %s", PLAYER_TEMP[playerid][pt_RP_NAME], params);
	ProxDetector(playerid, 15.0, str_text, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, 85);
	return 1;
}


CMD:b(playerid, params[])
{
	if(isnull(params)) return SendNotification(playerid, "~r~Modo de uso: ~w~/b [TEXTO]");
	
    new str_text[190];
    format(str_text, sizeof(str_text), "ID: %d | %s: (( %s ))", playerid, PLAYER_TEMP[playerid][pt_RP_NAME], params);
	ProxDetector(playerid, 15.0, str_text, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, 85);
	return 1;
}

CMD:do(playerid, params[])
{
	if(isnull(params)) return SendNotification(playerid, "~r~Modo de uso: ~w~/do [TEXTO]");

    new str_text[190];
    format(str_text, sizeof(str_text), "* %s (( %s ))", PLAYER_TEMP[playerid][pt_RP_NAME], params);
	ProxDetector(playerid, 15.0, str_text, 0xADFF2FAA, 0xADFF2FAA, 0xADFF2FAA, 0xADFF2FAA, 0xADFF2FAA, 85);
	return 1;
}





#define TIME_BETWEEN_GIVE_CASH	30 // segundos




CMD:entrar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID] == 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
		
	new info[3];
	
	new Float:pos[3];
	Streamer_GetArrayData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
	if(info[0] == PICKUP_TYPE_NONE) return SendNotification(playerid, "No estás en el lugar adecuado.");

	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_X, pos[0]);
	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Y, pos[1]);
	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Z, pos[2]);

	if(!IsPlayerInRangeOfPoint(playerid, 1.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "No estás en el lugar adecuado.");

	switch(info[0])
	{
		case PICKUP_TYPE_ENTER_EXIT:
		{
			PLAYER_TEMP[playerid][pt_HOSPITAL] = GetNearestHospitalForPlayer(playerid);
	
			if(info[2] == 2) // Está en el Pickup Exterior y quiere ir al interior
			{
				if(ENTER_EXIT[info[1]][ee_TIME_OPEN] == 0 && ENTER_EXIT[info[1]][ee_TIME_CLOSE] == 0) // 24 horas
				{
					ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_INTERIOR;
					PI[playerid][ac_LOCAL_INTERIOR] = ENTER_EXIT[info[1]][ee_ID];
					PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] = info[1];
					SetPlayerPosEx(playerid, ENTER_EXIT[info[1]][ee_INT_X], ENTER_EXIT[info[1]][ee_INT_Y], ENTER_EXIT[info[1]][ee_INT_Z], ENTER_EXIT[info[1]][ee_INT_ANGLE], ENTER_EXIT[info[1]][ee_INT_INTERIOR], ENTER_EXIT[info[1]][ee_INT_WORLD], false /*ENTER_EXIT[info[1]][ee_INT_FREEZE], true*/);
					FreezePlayer(playerid);
					CallLocalFunction("OnPlayerEnterInterior", "ii", playerid, info[1]);
				}
				else
				{
					if(is_open(GetMyWorldHour(), ENTER_EXIT[info[1]][ee_TIME_OPEN], ENTER_EXIT[info[1]][ee_TIME_CLOSE]))
					{
						ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_INTERIOR;
						PI[playerid][ac_LOCAL_INTERIOR] = ENTER_EXIT[info[1]][ee_ID];
						PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] = info[1];
						SetPlayerPosEx(playerid, ENTER_EXIT[info[1]][ee_INT_X], ENTER_EXIT[info[1]][ee_INT_Y], ENTER_EXIT[info[1]][ee_INT_Z], ENTER_EXIT[info[1]][ee_INT_ANGLE], ENTER_EXIT[info[1]][ee_INT_INTERIOR], ENTER_EXIT[info[1]][ee_INT_WORLD], false /*ENTER_EXIT[info[1]][ee_INT_FREEZE], true*/);
						FreezePlayer(playerid);
						CallLocalFunction("OnPlayerEnterInterior", "ii", playerid, info[1]);

						if(ENTER_EXIT[info[1]][ee_INTERIOR_TYPE] == INTERIOR_CLUB) PlayAudioStreamForPlayerEx(playerid, "https://www.dropbox.com/s/yeuwio26i7y00ly/striptease.mp3?dl=1");							
					}
					else {
						if(ENTER_EXIT[info[1]][ee_TIME_OPEN] == -1 && ENTER_EXIT[info[1]][ee_TIME_CLOSE] == -1) SendFormatNotification(playerid, "~r~%s ~w~abrirá próximamente.", ENTER_EXIT[info[1]][ee_NAME]);
						else SendFormatNotification(playerid, "~r~%s ~w~está cerrado, su horario es: ~r~%02d:00h ~w~- ~r~%02d:00h~w~.", ENTER_EXIT[info[1]][ee_NAME], ENTER_EXIT[info[1]][ee_TIME_OPEN], ENTER_EXIT[info[1]][ee_TIME_CLOSE]);
					}
				}
			}
			else SendNotification(playerid, "No estás en el lugar adecuado.");
		}
		case PICKUP_TYPE_PROPERTY:
		{
			PLAYER_TEMP[playerid][pt_HOSPITAL] = GetNearestHospitalForPlayer(playerid);
			
			if(info[2] == 2) // Está en el Pickup Exterior y quiere ir al interior
			{
				if(PROPERTY_INFO[info[1]][property_POLICE_FORCED] && (PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET] && PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_POLICE)) {
					new Float:z_pos = PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Z];
					if(PROPERTY_INFO[info[1]][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
					SetPlayerPosEx(playerid, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_X], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Y], z_pos, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_ANGLE], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_INTERIOR], PROPERTY_INFO[info[1]][property_ID], false /*PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_FREEZE], true*/);
					FreezePlayer(playerid);
					return 1;
				}
				if(!PROPERTY_INFO[info[1]][property_SOLD]) return SendNotification(playerid, "Esta propiedad está en venta.");
				if(PROPERTY_INFO[info[1]][property_CREW])
				{
					if(!PI[playerid][ac_CREW]) return SendFormatNotification(playerid, "Solo miembros de la banda pueden entrar.");
					if(PI[playerid][ac_CREW] != PROPERTY_INFO[info[1]][property_CREW_ID]) return SendFormatNotification(playerid, "Solo miembros de la banda pueden entrar.");
					
					ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_GUEST_PROPERTY;
					PI[playerid][ac_LOCAL_INTERIOR] = PROPERTY_INFO[info[1]][property_ID];
					PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] = info[1];

					new Float:z_pos = PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Z];
					if(PROPERTY_INFO[info[1]][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
					SetPlayerPosEx(playerid, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_X], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Y], z_pos, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_ANGLE], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_INTERIOR], PROPERTY_INFO[info[1]][property_ID], false /*PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_FREEZE], true*/);
					FreezePlayer(playerid);
				}
				else
				{
					if(PROPERTY_INFO[info[1]][property_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
					{
						ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_OWN_PROPERTY;
						PI[playerid][ac_LOCAL_INTERIOR] = PROPERTY_INFO[info[1]][property_ID];
						PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] = info[1];

						new Float:z_pos = PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Z];
						if(PROPERTY_INFO[info[1]][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
						SetPlayerPosEx(playerid, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_X], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Y], z_pos, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_ANGLE], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_INTERIOR], PROPERTY_INFO[info[1]][property_ID], false /*PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_FREEZE], true*/);
						FreezePlayer(playerid);
					}
					else
					{
						if(gettime() < PLAYER_TEMP[playerid][pt_ANTIFLOOD_KNOCK_PROPERTY] + 10) return SendNotification(playerid, "Espera para volver a tocar, si no te quieren abrir, vete.");
						
						new owner_playerid = GetOwnerIntProperty(PROPERTY_INFO[info[1]][property_ID]);
						if(owner_playerid == -1) return SendNotification(playerid, "Esta no es tu propiedad y el dueño no está dentro para dejarte entrar.");
						
						PLAYER_TEMP[playerid][pt_ANTIFLOOD_KNOCK_PROPERTY] = gettime();
						PLAYER_TEMP[owner_playerid][pt_KNOCK_PLAYER_ID] = playerid;
						SendFormatNotification(owner_playerid, "~b~%s ~w~está tocando la puerta, para dejarle entrar ve a la puerta y usa ~b~/puerta~w~.", PLAYER_TEMP[playerid][pt_RP_NAME]);
						SendNotification(playerid, "Has tocado en la puerta, espera para que te abran o vete.");
					}
				}
			}
			else SendNotification(playerid, "No estás en el lugar adecuado.");
		}
		default: SendNotification(playerid, "No estás en el lugar adecuado.");
	}
	return 1;
}

CMD:puerta(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] != ROLEPLAY_STATE_OWN_PROPERTY) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID] == 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
		
	new info[3];
	Streamer_GetArrayData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
	if(info[0] != PICKUP_TYPE_PROPERTY) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	new Float:pos[3]; 
	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_X, pos[0]);
	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Y, pos[1]);
	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Z, pos[2]);
	
	if(!IsPlayerInRangeOfPoint(playerid, 1.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	if(info[2] == 1) // Está en el Pickup Interior
	{
		if(PROPERTY_INFO[info[1]][property_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
		{
			if(PLAYER_TEMP[playerid][pt_KNOCK_PLAYER_ID] == INVALID_PLAYER_ID) return SendNotification(playerid, "Nadie ha tocado en la puerta.");
			if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_KNOCK_PLAYER_ID])) return SendNotification(playerid, "Nadie ha tocado en la puerta o ya se ha ido.");
			if(!IsPlayerInRangeOfPoint(PLAYER_TEMP[playerid][pt_KNOCK_PLAYER_ID], 3.0, PROPERTY_INFO[info[1]][property_EXT_X], PROPERTY_INFO[info[1]][property_EXT_Y], PROPERTY_INFO[info[1]][property_EXT_Z])) return SendNotification(playerid, "Nadie ha tocado en la puerta o ya se ha ido.");
			if(PLAYER_TEMP[playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "Este jugador no puede entrar ahora.");
			
			PLAYER_TEMP[PLAYER_TEMP[playerid][pt_KNOCK_PLAYER_ID]][pt_HOSPITAL] = GetNearestHospitalForPlayer(PLAYER_TEMP[playerid][pt_KNOCK_PLAYER_ID]);
			
			ACCOUNT_INFO[PLAYER_TEMP[playerid][pt_KNOCK_PLAYER_ID]][ac_STATE] = ROLEPLAY_STATE_GUEST_PROPERTY;
			PI[PLAYER_TEMP[playerid][pt_KNOCK_PLAYER_ID]][ac_LOCAL_INTERIOR] = PROPERTY_INFO[info[1]][property_ID];
			PLAYER_TEMP[PLAYER_TEMP[playerid][pt_KNOCK_PLAYER_ID]][pt_PROPERTY_INDEX] = info[1];

			new Float:z_pos = PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Z];
			if(PROPERTY_INFO[info[1]][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
			SetPlayerPosEx(PLAYER_TEMP[playerid][pt_KNOCK_PLAYER_ID], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_X], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Y], z_pos, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_ANGLE], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_INTERIOR], PROPERTY_INFO[info[1]][property_ID], false /*PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_FREEZE], true*/);
			FreezePlayer(PLAYER_TEMP[playerid][pt_KNOCK_PLAYER_ID]);
		}
		else SendNotification(playerid, "No estás en el lugar adecuado.");
	}
	else SendNotification(playerid, "No estás en el lugar adecuado.");
	return 1;
}



CMD:echar(playerid, params[])
{
	if(sscanf(params, "u", params[0])) return SendNotification(playerid, "~r~Modo de uso: ~w~/echar [PlayerID/Nombre]");
	if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "El jugador no está conectado.");
	if(playerid == params[0]) return SendNotification(playerid, "No te eches a ti mismo.");
	
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_OWN_PROPERTY)
	{
		if( ACCOUNT_INFO[params[0]][ac_STATE] == ROLEPLAY_STATE_GUEST_PROPERTY && PI[params[0]][ac_LOCAL_INTERIOR] == PI[playerid][ac_LOCAL_INTERIOR])
		{		
			new index = GetPropertyIndexByID(PI[playerid][ac_LOCAL_INTERIOR]);
			if(index == -1) return SendNotification(playerid, "BUG: CMD /ECHAR, Tome captura y contacte con administrador.");
				
			ACCOUNT_INFO[params[0]][ac_STATE] = ROLEPLAY_STATE_NORMAL;
			PI[params[0]][ac_LOCAL_INTERIOR] = 0;
			PLAYER_TEMP[params[0]][pt_PROPERTY_INDEX] = -1;
			SetPlayerPosEx(params[0], PROPERTY_INFO[ index ][property_EXT_X], PROPERTY_INFO[ index ][property_EXT_Y], PROPERTY_INFO[ index ][property_EXT_Z], PROPERTY_INFO[ index ][property_EXT_ANGLE], PROPERTY_INFO[ index ][property_EXT_INTERIOR], 0, false /*PROPERTY_INFO[ index ][property_EXT_FREEZE], false*/);
			StopAudioStreamForPlayer(params[0]);
			FreezePlayer(params[0]);
			
			SendFormatNotification(params[0], "~r~%s ~w~te ha echado de su propiedad.", PLAYER_TEMP[playerid][pt_RP_NAME]);
			SendFormatNotification(playerid, "Has echado a ~r~%s ~w~de tu propiedad.", PLAYER_TEMP[params[0]][pt_RP_NAME]);
		}
		else SendNotification(playerid, "Este jugador no está en tu propiedad.");
		return 1;
	}

	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		
		if(!PLAYER_VEHICLES[vehicleid][player_vehicle_VALID]) return SendNotification(playerid, "Este no es tú vehículo.");
		if(PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return SendNotification(playerid, "Este no es tú vehículo.");
		if(GetPlayerVehicleID(params[0]) != vehicleid) return SendNotification(playerid, "Este jugador no está en tu vehículo.");
		
		RemovePlayerFromVehicle(params[0]);
		
		new action[64];
		format(action, sizeof action, "ha echado a %s de su vehículo.", PLAYER_TEMP[ params[0] ][pt_RP_NAME]);
		Auto_SendPlayerAction(playerid, action);
		return 1;
	}
	
	SendNotification(playerid, "No estás en tu propiedad o en tu vehículo.");
	return 1;
}


CMD:salir(playerid, params[])
{	
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID] == 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
		
	new info[3];
	
	new Float:pos[3];
	Streamer_GetArrayData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
	if(info[0] == PICKUP_TYPE_NONE) return SendNotification(playerid, "No estás en el lugar adecuado.");

	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_X, pos[0]);
	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Y, pos[1]);
	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Z, pos[2]);

	if(!IsPlayerInRangeOfPoint(playerid, 1.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "No estás en el lugar adecuado.");

	switch(info[0])
	{
		case PICKUP_TYPE_ENTER_EXIT:
		{
			if(info[2] == 1) // Está en el Pickup Interior y quiere ir al exterior
			{
				ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_NORMAL;
				PI[playerid][ac_LOCAL_INTERIOR] = 0;
				PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] = -1;
				SetPlayerPosEx(playerid, ENTER_EXIT[info[1]][ee_EXT_X], ENTER_EXIT[info[1]][ee_EXT_Y], ENTER_EXIT[info[1]][ee_EXT_Z], ENTER_EXIT[info[1]][ee_EXT_ANGLE], ENTER_EXIT[info[1]][ee_EXT_INTERIOR], ENTER_EXIT[info[1]][ee_EXT_WORLD], false /*ENTER_EXIT[info[1]][ee_EXT_FREEZE]*/);
				SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
				StopAudioStreamForPlayer(playerid);
				FreezePlayer(playerid);
				CallLocalFunction("OnPlayerExitInterior", "ii", playerid, info[1]);
			}
			else SendNotification(playerid, "No estás en el lugar adecuado.");
		}
		case PICKUP_TYPE_PROPERTY:
		{
			if(info[2] == 1) // Está en el Pickup Interior y quiere ir al exterior
			{
				ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_NORMAL;
				PI[playerid][ac_LOCAL_INTERIOR] = 0;
				PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] = -1;
				SetPlayerPosEx(playerid, PROPERTY_INFO[info[1]][property_EXT_X], PROPERTY_INFO[info[1]][property_EXT_Y], PROPERTY_INFO[info[1]][property_EXT_Z], PROPERTY_INFO[info[1]][property_EXT_ANGLE], PROPERTY_INFO[info[1]][property_EXT_INTERIOR], 0, false /*PROPERTY_INFO[info[1]][property_EXT_FREEZE], false*/);
				SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
				StopAudioStreamForPlayer(playerid);
				FreezePlayer(playerid);
			}
			else SendNotification(playerid, "No estás en el lugar adecuado.");
		}
		default: SendNotification(playerid, "No estás en el lugar adecuado.");
	}
	return 1;
}














AddNewAd(playerid, type, price, text[], modelid, vcol1 = 0, vcol2 = 0)
{
	new Float:rots[4], date[24];
	getDateTime(date);
	switch(type)
	{
		case SHOP_ARTICLE_TYPE_PROPERTY: rots = Float:{345.0, 0.0, 30.0, 1.20};
		case SHOP_ARTICLE_TYPE_VEHICLE: rots = Float:{345.0, 0.0, 330.0, 1.0};
		case SHOP_ARTICLE_TYPE_OTHER: rots = Float:{0.0, 0.0, 330.0, 1.20};
	}
	
	mysql_format
	(
		srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER,
		"\
			INSERT INTO shop (id_player, type, price, text, modelid, rx, ry, rz, zoom, vcol1, vcol2, date)\
			VALUES (%d, %d, %d, '%e', %d, %f, %f, %f, %f, %d, %d, '%e');\
		",
			ACCOUNT_INFO[playerid][ac_ID], type, price, text, modelid, rots[0], rots[1], rots[2], rots[3], vcol1, vcol2, date
	);
	mysql_tquery(srp_db, QUERY_BUFFER);
	return 1;
}

forward OnPlayerSongFound(index, response_code, data[]);
public OnPlayerSongFound(index, response_code, data[])
{
	if(!PLAYER_TEMP[index][pt_PLAYER_WAITING_MP3_HTTP]) return 1;
	
	if(response_code == 200)
	{
		if(data[0] == '|' && data[1] == '[')
		{
			new start_song_pos = -1, end_song_pos = -1, delimiter_pos_info = -1, counter, tmp[yt_result];
			for(new i = 0; i != sizeof RESULT_YOUTUBE; i ++) RESULT_YOUTUBE[i] = tmp;
			
			start_song_pos = strfind(data, "|[");
			while(start_song_pos != -1)
			{
				start_song_pos = strfind(data, "|[");
				if(start_song_pos == -1) break;
				delimiter_pos_info = strfind(data, "||");
				strmid(RESULT_YOUTUBE[counter][videoID], data, start_song_pos + 2, delimiter_pos_info, 11 + 1);
				
				end_song_pos = strfind(data, "]|");
				strmid(RESULT_YOUTUBE[counter][yt_title], data, delimiter_pos_info + 2, end_song_pos, 100 + 1);
				strdel(data, 0, end_song_pos + 2);
				counter ++;
			}
			
			if(counter == 0) return SendClientMessage(index, -1, "{"#SILVER_COLOR"}No se encontró ningún resultado para tu búsqueda.");
			
			new dialog_title[50], dialog[120 * MAX_RESULTS], dialog_counter;
			
			for(new i = 0; i != counter; i ++)
			{
				if(isnull(RESULT_YOUTUBE[i][yt_title])) continue;
				format(PLAYER_DIALOG_MP3_RESULT[index][dialog_counter][videoID], 11 + 1, "%s", RESULT_YOUTUBE[i][videoID]);
				format(PLAYER_DIALOG_MP3_RESULT[index][dialog_counter][yt_title], 100 + 1, "%s", RESULT_YOUTUBE[i][yt_title]);
				format(dialog, sizeof dialog, "%s{"#SILVER_COLOR"}%d. %s\n", dialog, dialog_counter + 1, RESULT_YOUTUBE[i][yt_title]);
				dialog_counter ++;
			}
			format(dialog_title, sizeof dialog_title, "{"#SILVER_COLOR"}Se han encontado '%d' resultados", dialog_counter);

			PLAYER_TEMP[index][pt_DIALOG_RESPONDED] = false;
			PLAYER_TEMP[index][pt_DIALOG_ID] = DIALOG_PLAYER_MP3_RESULTS;
			ShowPlayerDialog(index, DIALOG_PLAYER_MP3_RESULTS, DIALOG_STYLE_LIST, dialog_title, dialog, "Reproducir", "Cerrar");
		}
		else SendClientMessage(index, -1, "{"#SILVER_COLOR"}El resultado obtenido no puede ser comprendido.");
	}
	else SendClientMessage(index, -1, "{"#SILVER_COLOR"}La búsqueda falló, inténtelo de nuevo más tarde.");
	
	PLAYER_TEMP[index][pt_PLAYER_WAITING_MP3_HTTP] = false;
	return 1;
}

GetEmptyPlayer_GPS_Slot(playerid)
{
	for(new i = 0; i != MAX_PLAYER_GPS_SAVES; i ++)
	{
		if(!PLAYER_GPS[playerid][i][player_gps_VALID]) return i;
	}
	return -1;
}

SetPlayer_GPS_Checkpoint(playerid, Float:x, Float:y, Float:z, world, interior)
{
	if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT]))
	{
		DestroyDynamicCP(PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT] = INVALID_STREAMER_ID;
	}
	PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT] = CreateDynamicCP(x, y, z, 5.0, world, interior, playerid, 9999999999.0);
	
	new info[1];
	info[0] = CHECKPOINT_TYPE_GPS;
	Streamer_SetArrayData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT], E_STREAMER_EXTRA_ID, info);
	Streamer_Update(playerid, STREAMER_TYPE_CP);

	if(GetPlayerState(playerid) == PLAYER_STATE_PASSENGER)
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		if(WORK_VEHICLES[vehicleid][work_vehicle_VALID])
		{
			if(WORK_VEHICLES[vehicleid][work_vehicle_WORK] == WORK_TAXI)
			{
				if(TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_ENABLED])
				{
					if(IsPlayerConnected(GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER]))
					{
						if(IsValidDynamicCP(PLAYER_TEMP[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] ][pt_GPS_CHECKPOINT]))
						{
							DestroyDynamicCP(PLAYER_TEMP[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] ][pt_GPS_CHECKPOINT]);
							PLAYER_TEMP[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] ][pt_GPS_CHECKPOINT] = INVALID_STREAMER_ID;
						}
						PLAYER_TEMP[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] ][pt_GPS_CHECKPOINT] = CreateDynamicCP(x, y, z, 5.0, world, interior, GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER], 9999999999.0);
						
						new taxi_info[1];
						taxi_info[0] = CHECKPOINT_TYPE_GPS;
						Streamer_SetArrayData(STREAMER_TYPE_CP, PLAYER_TEMP[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] ][pt_GPS_CHECKPOINT], E_STREAMER_EXTRA_ID, taxi_info);
						
						Streamer_Update(GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER], STREAMER_TYPE_CP);
						Auto_SendPlayerAction(playerid, "configura su GPS y se lo muestra al taxista.");
						return 1;
					}
				}
			}
		}
	}
	
	Auto_SendPlayerAction(playerid, "configura su GPS.");
	return 1;
}
	








SetPlayerToys(playerid)
{
	for(new i = 0; i != MAX_SU_TOYS; i ++) RemovePlayerAttachedObject(playerid, i);
		
	for(new i = 0; i != MAX_SU_TOYS; i ++)
	{
		if(i >= MAX_NU_TOYS && !ACCOUNT_INFO[playerid][ac_VIP]) break;

		if(PLAYER_TOYS[playerid][i][player_toy_VALID])
		{
			if(PLAYER_TOYS[playerid][i][player_toy_ATTACHED])
			{
				SetPlayerAttachedObject
				(
					playerid, 
					i,
					PLAYER_TOYS[playerid][ i ][player_toy_MODELID],
					PLAYER_TOYS[playerid][ i ][player_toy_BONE],
					PLAYER_TOYS[playerid][ i ][player_toy_OFFSET_X],
					PLAYER_TOYS[playerid][ i ][player_toy_OFFSET_Y],
					PLAYER_TOYS[playerid][ i ][player_toy_OFFSET_Z],
					PLAYER_TOYS[playerid][ i ][player_toy_ROT_X],
					PLAYER_TOYS[playerid][ i ][player_toy_ROT_Y],
					PLAYER_TOYS[playerid][ i ][player_toy_ROT_Z],
					PLAYER_TOYS[playerid][ i ][player_toy_SCALE_X],
					PLAYER_TOYS[playerid][ i ][player_toy_SCALE_Y],
					PLAYER_TOYS[playerid][ i ][player_toy_SCALE_Z],
					PLAYER_TOYS[playerid][ i ][player_toy_COLOR_1],
					PLAYER_TOYS[playerid][ i ][player_toy_COLOR_2]
				);
			}
		}
	}
	
	return 1;
}





forward OnDatabaseConnected();








ClearPlayerChat(playerid)
{
	for(new i = 0; i != 32; i++) SendClientMessage(playerid, -1, " ");
	return 1;
}

RegisterBankAccountTransaction(from_id, to_id, transaction_type, amount)
{
	new date[24];
	getDateTime(date);
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO bank_movements (date, type, from_id, to_id, amount) VALUES('%e', %d, %d, %d, %d);", date, transaction_type, from_id, to_id, amount);
	mysql_tquery(srp_db, QUERY_BUFFER);
	return 1;
}




stock GetNombreRoca(roca)
	{
		new namer[60];
		switch(roca)
		{
			case 1:	namer = "Azufre";
			case 2: namer = "Andesita";
			case 3:	namer = "Carbon";
			case 4:	namer = "Cobre";
			case 5:	namer = "Hierro";
			case 6:	namer = "Esmeralda";
			case 7:	namer = "Cobre";
			case 8:	namer = "Andesita";
			case 9:	namer = "Hierro";
			case 10:	namer = "Oro";
			case 11:	namer = "Cuarzo Blanco";
			case 12:	namer = "Obsidiana";
			case 13:	namer = "Carbon";
			case 14:	namer = "Cobre";
			case 15:	namer = "Esmeralda";
			case 16:	namer = "Hierro";
			case 17:	namer = "Andesita";
			case 18:	namer = "Rubi";
			case 19:	namer = "Esmeralda";
			case 20:	namer = "Oro";
			case 21:	namer = "Diamante";
			case 22:	namer = "Hierro";
		}
		return namer;
	}
stock GetColorRoca(roca)
	{
		new colorr;
		switch(roca)
		{
			case 1:	colorr = 0xCEE326FF;//Azufre
			case 2: colorr = 0x999A8FFF;//"Andesita";
			case 3:	colorr = 0x505050FF;//"Carbon";
			case 4:	colorr = 0x943D1BFF;//Cobre
			case 5:	colorr = 0xD6CFCDFF;//"Hierro";
			case 6:	colorr = 0x02A305FF;//"Esmeralda";
			case 7:	colorr = 0x943D1BFF;//"Cobre";
			case 8:	colorr = 0x999A8FFF;//"Andesita";
			case 9:	colorr = 0xD6CFCDFF;//"Hierro";
			case 10:	colorr = 0xFFFC57FF;//"Oro";
			case 11:	colorr = 0xF6F5E6FF;//"Cuarzo";
			case 12:	colorr = 0x741A78FF;//"Obsidiana";
			case 13:	colorr = 0x505050FF;//"Carbon";
			case 14:	colorr = 0x943D1BFF;//"Cobre";
			case 15:	colorr = 0x02A305FF;//"Esmeralda";
			case 16:	colorr = 0xD6CFCDFF;//"Hierro";
			case 17:	colorr = 0x999A8FFF;//"Andesita";
			case 18:	colorr = 0xFF0505FF;//"Rubi";
			case 19:	colorr = 0x02A305FF;//"Esmeralda";
			case 20:	colorr = 0xFFFC57FF;//"Oro";
			case 21:	colorr = 0x05B0FFFF;//"Diamante";
			case 22:	colorr = 0xD6CFCDFF;//"Hierro";
		}
		return colorr;
	}
stock SetTexuraRoca(objeto,tipo)
	{
		switch(tipo)
		{
			//Mina A
			case 1: SetDynamicObjectMaterial(objeto, 0, 18901, "matclothes", "bandanagold", 0x00000000);//Azufre
			case 2: SetDynamicObjectMaterial(objeto, 0, 19071, "wssections", "rockbrown1", 0x00000000);//Andesita
			case 3: SetDynamicObjectMaterial(objeto, 0, 17555, "eastbeach3c_lae2", "decobuild2d_LAn", 0x00000000);//Carbon
			case 4: SetDynamicObjectMaterial(objeto, 0, 11712, "cross1", "cross1", 0x00000000);//Cobre
			case 5: SetDynamicObjectMaterial(objeto, 0, 18901, "matclothes", "metalalumox1", 0x00000000);//Hierro
			//Mina B
			case 6: SetDynamicObjectMaterial(objeto, 0, 2098, "cj_int", "CJ_GREEN_WOOD", 0x00000000);//Esmeralda
			case 7: SetDynamicObjectMaterial(objeto, 0, 11712, "cross1", "cross1", 0x00000000);//Cobre
			case 8: SetDynamicObjectMaterial(objeto, 0, 19071, "wssections", "rockbrown1", 0x00000000);//Andesita
			case 9: SetDynamicObjectMaterial(objeto, 0, 18901, "matclothes", "metalalumox1", 0x00000000);//Hierro
			case 10: SetDynamicObjectMaterial(objeto,0, 19063, "xmasorbs", "foil5-128x128", 0x00000000);//Oro
			//Mina C
			case 11: SetDynamicObjectMaterial(objeto, 0, 3922, "bistro", "Marble2", 0x00000000);//Cuarzo
			case 12: SetDynamicObjectMaterial(objeto, 0, 18996, "mattextures", "bluefoil", 0x00000000);//Obsidiana
			case 13: SetDynamicObjectMaterial(objeto, 0, 17555, "eastbeach3c_lae2", "decobuild2d_LAn", 0x00000000);//Carbon
			case 14: SetDynamicObjectMaterial(objeto, 0, 11712, "cross1", "cross1", 0x00000000);//Cobre
			case 15: SetDynamicObjectMaterial(objeto, 0, 2098, "cj_int", "CJ_GREEN_WOOD", 0x00000000);//Esmeralda
			case 16: SetDynamicObjectMaterial(objeto, 0, 18901, "matclothes", "metalalumox1", 0x00000000);//Hierro
			case 17: SetDynamicObjectMaterial(objeto, 0, 19071, "wssections", "rockbrown1", 0x00000000);//Andresita
			//Mina D
			case 18: SetDynamicObjectMaterial(objeto, 0, 18835, "mickytextures", "red032", 0x00000000);//Rubi
			case 19: SetDynamicObjectMaterial(objeto, 0, 2098, "cj_int", "CJ_GREEN_WOOD", 0x00000000);//Esmeralda
			case 20: SetDynamicObjectMaterial(objeto, 0, 19063, "xmasorbs", "foil5-128x128", 0x00000000);//Oro
			case 21: SetDynamicObjectMaterial(objeto, 0, 3474, "freightcrane", "bluecab4_256", 0x00000000);//Diamante
			case 22: SetDynamicObjectMaterial(objeto, 0, 18901, "matclothes", "metalalumox1", 0x00000000);//Hierro
		}
		return 1;
	}
stock GetPrecioRoca(roca)
	{
		new precio;
		switch(roca)
		{
			//Mina A
			case 1:	precio = 90;
			case 2: precio = 75;
			case 3:	precio = 80;
			case 4:	precio = 95;
			case 5:	precio = 110;
			//Mina B
			case 6:	precio = 230;
			case 7:	precio = 175;
			case 8:	precio = 180;
			case 9:	precio = 185;
			case 10:	precio = 195;
			//Mina C
			case 11:	precio = 250;//Cuarzo
			case 12:	precio = 375;//Obsidiana
			case 13:	precio = 235;//Carbon
			case 14:	precio = 240;//Cobre
			case 15:	precio = 320;//Esmeralda
			case 16:	precio = 255;//Hierro
			case 17:	precio = 195;//Andresita
			//Mina D
			case 18:	precio = 430;//Rubi
			case 19:	precio = 350;//Esmeralda
			case 20:	precio = 360;//Oro
			case 21:	precio = 490;//Diamante
			case 22:	precio = 370;//Hierro
		}
		return precio;
	}
stock GetTiempoRoca(roca)
	{
		new tiempo;
		switch(roca)
		{
			case 1:	tiempo = 2;//Azufre
			case 2: tiempo = 3;//Andesita
			case 3:	tiempo = 3;//Carbon
			case 4:	tiempo = 4;//Cobre
			case 5:	tiempo = 3;//Hierro
			//Mina B
			case 6:	tiempo = 3;//Esmeralda
			case 7:	tiempo = 6;//Cobre
			case 8:	tiempo = 4;//Andesita
			case 9:	tiempo = 3;//Hierro
			case 10:	tiempo = 2;//Oro
			//Mina C
			case 11:	tiempo = 2;//Cuarzo
			case 12:	tiempo = 1;//Obsidiana
			case 13:	tiempo = 3;//Carbon
			case 14:	tiempo = 4;//Cobre
			case 15:	tiempo = 2;//Esmeralda
			case 16:	tiempo = 7;//Hierro
			case 17:	tiempo = 4;//Andesita
			//Mina D
			case 18:	tiempo = 3;//Rubi
			case 19:	tiempo = 2;//Esmeralda
			case 20:	tiempo = 3;//Oro
			case 21:	tiempo = 2;//Diamante
			case 22:	tiempo = 3;//Hierro
		}
		return tiempo;
	}

stock randomize( min, max ) // Rango aleatorio
{
	max++;
	new
	a = random( max - min ) + min;
	return a;
}	
	
stock SetMinaRoca(mina,i)
	{
		new roca;
		switch(mina)
		{
			case 1:
			{
				roca = randomize(1, 5);
			}
			case 2:
			{
				roca = randomize(6, 10);
			}
			case 3:
			{
				roca = randomize(11, 17);
			}
			case 4:
			{
				roca = randomize(18, 22);
			}
		}
		MinaInfo[i][w_tipo] = roca;
		return 1;
	}
stock EnProcesadora(playerid)
	{
		if(IsPlayerInRangeOfPoint(playerid, 2.0, 597.3271,-41.2564,791.6342))		return 1;
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 591.7543,-41.0298,791.5934)) return 1;
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 568.9272,-41.1961,791.6544)) return 1;
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 563.0521,-41.0409,791.7198)) return 1;
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 511.8504,-37.4351,791.1703)) return 1;
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 506.3676,-35.9534,791.1245)) return 1;
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 456.9895,-35.9433,790.8328)) return 1;
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 451.4697,-36.5215,790.8202)) return 1;
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 380.8173,-38.6107,790.6462)) return 1;
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 374.9194,-36.8096,790.9003)) return 1;
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 270.3931,-38.0284,790.7828)) return 1;
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 236.9331,-37.8291,790.7186)) return 1;
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 263.7611,-37.5496,790.5789)) return 1;
		return 0;
	}


forward TimerPicando(playerid, wood);
public TimerPicando(playerid, wood)
{	
	if(ProcesoPicando[playerid] < 20) SetPlayerAttachedObject(playerid,0,19631,6,0.056999,0.015000,0.132999,-87.599998,95.299995,-1.900005,1.000000,1.000000,1.000000);
	if(ProcesoPicando[playerid] < 100)
	{
		ProcesoPicando[playerid] += GetTiempoRoca(MinaInfo[wood][w_tipo]);
		if(ProcesoPicando[playerid] > 99) ProcesoPicando[playerid] = 100;
		new PicandoT[126];
		format(PicandoT, sizeof PicandoT, "Picando %s...~n~~r~%d%",GetNombreRoca(MinaInfo[wood][w_tipo]),ProcesoPicando[playerid]);
		ShowPlayerMessage(playerid, PicandoT,3000);
		TimerMinando[playerid] = SetTimerEx( "TimerPicando", 700, false, "ddd", playerid, wood);
		ApplyAnimation(playerid, "BASEBALL","Bat_4", 4.1, 1, 0, 0, 1, 1000);
		return 1;
	}
	TogglePlayerControllableEx( playerid, true );
	KillTimer(TimerSonido[playerid]);
	new colorroca,colorfinal;
	colorroca = GetColorRoca(MinaInfo[wood][w_tipo]);
	colorfinal = RGBAToARGB(colorroca);
	RemovePlayerAttachedObject(playerid, 0);
	ClearAnimations(playerid);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
	ApplyAnimation(playerid,"CARRY","LIFTUP",2,0,1,1,0,0);
	ApplyAnimation(playerid,"CARRY","LIFTUP",2,0,1,1,0,0);
	SetPlayerAttachedObject(playerid,5,905,5,0.139000,0.085000,0.136000,75.700004,0.000000,0.000000,0.555000,0.409999,0.640999,colorfinal);
	ProcesoPicando[playerid] = 0;
	MinaInfo[wood][w_count] = 0;
	MinaInfo[wood][w_drop] = true;
	MinaInfo[wood][w_use] = false;
	ShowPlayerMessage(playerid, "Presiona ~y~ALT~w~ en la ~r~procesadora~w~ para entregar el mineral.",8000);	
	TieneRoca[playerid]	= MinaInfo[wood][w_tipo];
	SetPVarInt( playerid, "Job:MineroCP",1);
	if( MinaInfo[wood][w_count] <= 0 && IsValidDynamicObject(MinaInfo[wood][w_object])) DestroyDynamicObject( MinaInfo[wood][w_object] );
	return 1;
}




CreateDefaultContacts(playerid)
{
	new slot;
	PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_VALID] = true;
	format(PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_NAME], 24, "Policia");
	PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_PHONE_NUMBER] = 911;
	RegisterNewPlayerPhoneBook(playerid, slot);
	slot ++;
	
	PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_VALID] = true;
	format(PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_NAME], 24, "Banco");
	PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_PHONE_NUMBER] = 6740;
	RegisterNewPlayerPhoneBook(playerid, slot);
	slot ++;
	
	PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_VALID] = true;
	format(PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_NAME], 24, "Aseguradora");
	PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_PHONE_NUMBER] = 38350;
	RegisterNewPlayerPhoneBook(playerid, slot);
	slot ++;
	
	PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_VALID] = true;
	format(PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_NAME], 24, "Taxi");
	PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_PHONE_NUMBER] = 450;
	RegisterNewPlayerPhoneBook(playerid, slot);
	slot ++;
	return 1;
}


//eSendClientMessageEx(playerid, color, form[], {Float, _}: ...)
//{
//    #pragma unused form
//
//    static
//        tmp[145]
//    ;
//    new
//        t1 = playerid,
//        t2 = color
//    ;
//    const
//        n4 = -4,
//        n16 = -16,
//        size = sizeof tmp
//    ;
//    #emit stack 28
//    #emit push.c size
//    #emit push.c tmp
//    #emit stack n4
//    #emit sysreq.c format
//    #emit stack n16
//
//    return (t1 == -1 ? (SendClientMessageToAll(t2, tmp)) : (SendClientMessage(t1, t2, tmp)) );
//}*/










//IsValidName(name[])
//{
//	for(new c = 0; c < strlen(name); c++)
//	{
//	    switch(name[c])
//	    {
//	        case 'A' .. 'Z', 'a' .. 'z', '_': continue;
//	        default: return false;
//	    }
//	}
//	return true;
//}






RP_GetPlayerName(playerid)
{
	new name[24];
	format(name, 24, "%s", PLAYER_TEMP[playerid][pt_NAME]);
    for(new i = 0; i < 24; i++) {
		if (name[i] == '_') name[i] = ' ';
	}
 
	return name;
}



RegisterNewPlayerPhoneBook(playerid, slot)
{
	inline OnContactInserted()
	{
		PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_ID] = cache_insert_id();
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO pbook (id_player, name, number) VALUES (%d, '%e', %d);", ACCOUNT_INFO[playerid][ac_ID], PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_NAME], PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_PHONE_NUMBER]);
	MySQL_TQueryInline(srp_db,  using inline OnContactInserted, QUERY_BUFFER);
	return 1;
}










GetEmptyPlayerPhoneBookSlot(playerid)
{
	for(new i = 0; i != MAX_PHONE_CONTACTS; i ++)
	{
		if(!PLAYER_PHONE_BOOK[playerid][i][phone_book_contact_VALID]) return i;
	}
	return -1;
}

RegisterNewPlayerPocketObject(playerid, slot)
{
	inline OnPfoodInserted()
	{
		PLAYER_POCKET[playerid][slot][player_pocket_object_ID] = cache_insert_id();
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO pfoods (id_player, name, hungry, thirst, drunk) VALUES (%d, '%e', %f, %f, %d);", ACCOUNT_INFO[playerid][ac_ID], PLAYER_POCKET[playerid][slot][player_pocket_object_NAME], PLAYER_POCKET[playerid][slot][player_pocket_object_HUNGRY], PLAYER_POCKET[playerid][slot][player_pocket_object_THIRST], PLAYER_POCKET[playerid][slot][player_pocket_object_DRUNK]);
	MySQL_TQueryInline(srp_db,  using inline OnPfoodInserted, QUERY_BUFFER);
	return 1;
}





UpdateReputationTextDraws(playerid)
{
	new neccessary_rep = ACCOUNT_INFO[playerid][ac_LEVEL] * REP_MULTIPLIER, str[64];
	if(ACCOUNT_INFO[playerid][ac_REP] >= neccessary_rep)
	{
		PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], 640.0, 0.0);
		if(PI[playerid][ac_CONFIG_HUD]) PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0]);
		format(str, sizeof str, "Nivel_%d__(/comprarnivel)", ACCOUNT_INFO[playerid][ac_LEVEL]);
		PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][3], str);
	}
	else
	{
		new Float:sizeX = floatmul(floatdiv(ACCOUNT_INFO[playerid][ac_REP], neccessary_rep), 640.0);
		PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0], sizeX, 0.0);
		if(PI[playerid][ac_CONFIG_HUD]) PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][0]);
		format(str, sizeof str, "Nivel_%d__(%d/%d)", ACCOUNT_INFO[playerid][ac_LEVEL], ACCOUNT_INFO[playerid][ac_REP], neccessary_rep);
		PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][3], str);
	}
	return 1;
}





stock SetPlayerCash(playerid, amount, bool:update = true)
{
	PI[playerid][ac_CASH] = amount;
	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, PI[playerid][ac_CASH]);
	
	if(update)
	{
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET cash = %d WHERE id = %d;", PI[playerid][ac_CASH], ACCOUNT_INFO[playerid][ac_ID]);
		mysql_tquery(srp_db, QUERY_BUFFER);
	}
	return 1;
}



LoadEnterExits()
{
	new total_enterexits, label_str[256], info[3];
		
	for(new i = 0; i != sizeof ENTER_EXIT; i ++)
	{
		ENTER_EXIT[total_enterexits][ee_ID] = total_enterexits + 1;

		if(ENTER_EXIT[total_enterexits][ee_TIME_OPEN] == 0 && ENTER_EXIT[total_enterexits][ee_TIME_CLOSE] == 0) format(label_str, sizeof label_str, "{FF7373}%s\n{FFFFFF}Presiona {FF7373}[ Y ] {FFFFFF}para entrar", ENTER_EXIT[total_enterexits][ee_NAME]);
		else if(ENTER_EXIT[total_enterexits][ee_TIME_OPEN] == -1 && ENTER_EXIT[total_enterexits][ee_TIME_CLOSE] == -1) format(label_str, sizeof label_str, "{FF7373}%s\n{"#SILVER_COLOR"}PRÓXIMA APERTURA", ENTER_EXIT[total_enterexits][ee_NAME]);
		else format(label_str, sizeof label_str, "{FF7373}%s\n\n{FFFFFF}Horario: {FF7373}%02d:00h {FFFFFF}- {FF7373}%02d:00h\n\n{FFFFFF}Presiona {FF7373}[ Y ] {FFFFFF}para entrar", ENTER_EXIT[total_enterexits][ee_NAME], ENTER_EXIT[total_enterexits][ee_TIME_OPEN], ENTER_EXIT[total_enterexits][ee_TIME_CLOSE]);
		
		ENTER_EXIT[total_enterexits][ee_INT_LABEL_ID] = CreateDynamic3DTextLabel("{FF7373}Salida\n{FFFFFF}Presiona {FF7373}[ Y ] {FFFFFF}para salir", 0xFFFFFFFF, ENTER_EXIT[total_enterexits][ee_INT_X], ENTER_EXIT[total_enterexits][ee_INT_Y], ENTER_EXIT[total_enterexits][ee_INT_Z] + 0.25, 10.0, .testlos = true, .worldid = ENTER_EXIT[total_enterexits][ee_INT_WORLD], .interiorid = ENTER_EXIT[total_enterexits][ee_INT_INTERIOR]);
		ENTER_EXIT[total_enterexits][ee_EXT_LABEL_ID] = CreateDynamic3DTextLabel(label_str, 0xFFFFFFFF, ENTER_EXIT[total_enterexits][ee_EXT_X], ENTER_EXIT[total_enterexits][ee_EXT_Y], ENTER_EXIT[total_enterexits][ee_EXT_Z] + 0.25, 10.0, .testlos = true, .worldid = ENTER_EXIT[total_enterexits][ee_EXT_WORLD], .interiorid = ENTER_EXIT[total_enterexits][ee_EXT_INTERIOR]);
		
		if(ENTER_EXIT[total_enterexits][ee_INT_MAPICON] != -1) ENTER_EXIT[total_enterexits][ee_INT_MAPICON_ID] = CreateDynamicMapIcon(ENTER_EXIT[total_enterexits][ee_INT_X], ENTER_EXIT[total_enterexits][ee_INT_Y], ENTER_EXIT[total_enterexits][ee_INT_Z], ENTER_EXIT[total_enterexits][ee_INT_MAPICON], -1, ENTER_EXIT[total_enterexits][ee_INT_WORLD], ENTER_EXIT[total_enterexits][ee_INT_INTERIOR]);
		if(ENTER_EXIT[total_enterexits][ee_EXT_MAPICON] != -1) ENTER_EXIT[total_enterexits][ee_EXT_MAPICON_ID] = CreateDynamicMapIcon(ENTER_EXIT[total_enterexits][ee_EXT_X], ENTER_EXIT[total_enterexits][ee_EXT_Y], ENTER_EXIT[total_enterexits][ee_EXT_Z], ENTER_EXIT[total_enterexits][ee_EXT_MAPICON], -1, ENTER_EXIT[total_enterexits][ee_EXT_WORLD], ENTER_EXIT[total_enterexits][ee_EXT_INTERIOR]);
		
		ENTER_EXIT[total_enterexits][ee_INT_PICKUP_ID] = CreateDynamicPickup(0, 1, ENTER_EXIT[total_enterexits][ee_INT_X], ENTER_EXIT[total_enterexits][ee_INT_Y], ENTER_EXIT[total_enterexits][ee_INT_Z], ENTER_EXIT[total_enterexits][ee_INT_WORLD], ENTER_EXIT[total_enterexits][ee_INT_INTERIOR]);
		ENTER_EXIT[total_enterexits][ee_EXT_PICKUP_ID] = CreateDynamicPickup(1318, 1, ENTER_EXIT[total_enterexits][ee_EXT_X], ENTER_EXIT[total_enterexits][ee_EXT_Y], ENTER_EXIT[total_enterexits][ee_EXT_Z] + 0.10, ENTER_EXIT[total_enterexits][ee_EXT_WORLD], ENTER_EXIT[total_enterexits][ee_EXT_INTERIOR]);

		info[0] = PICKUP_TYPE_ENTER_EXIT;
		info[1] = total_enterexits; // Index
		info[2] = 1; // Pickup Interior
		Streamer_SetArrayData(STREAMER_TYPE_PICKUP, ENTER_EXIT[total_enterexits][ee_INT_PICKUP_ID], E_STREAMER_EXTRA_ID, info);

		info[0] = PICKUP_TYPE_ENTER_EXIT;
		info[1] = total_enterexits; // Index
		info[2] = 2; // Pickup Exterior
		Streamer_SetArrayData(STREAMER_TYPE_PICKUP, ENTER_EXIT[total_enterexits][ee_EXT_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
		ENTER_EXIT[total_enterexits][ee_MAIN_ACTOR] = CreateInteriorActor(ENTER_EXIT[total_enterexits][ee_INTERIOR_TYPE], ENTER_EXIT[total_enterexits][ee_INT_WORLD], ENTER_EXIT[total_enterexits][ee_INT_INTERIOR]);

		//textdraw intimidación
		if(ENTER_EXIT[total_enterexits][ee_ROBBABLE]) {
			ENTER_EXIT[total_enterexits][ee_TEXTDRAW] = TextDrawCreate(320.000000, 406.000000, "Intimidación:_0.00%%");
			TextDrawLetterSize(ENTER_EXIT[total_enterexits][ee_TEXTDRAW], 0.286000, 1.276444);
			TextDrawAlignment(ENTER_EXIT[total_enterexits][ee_TEXTDRAW], 2);
			TextDrawColor(ENTER_EXIT[total_enterexits][ee_TEXTDRAW], -76);
			TextDrawSetShadow(ENTER_EXIT[total_enterexits][ee_TEXTDRAW], 0);
			TextDrawSetOutline(ENTER_EXIT[total_enterexits][ee_TEXTDRAW], 0);
			TextDrawBackgroundColor(ENTER_EXIT[total_enterexits][ee_TEXTDRAW], 255);
			TextDrawFont(ENTER_EXIT[total_enterexits][ee_TEXTDRAW], 1);
			TextDrawSetProportional(ENTER_EXIT[total_enterexits][ee_TEXTDRAW], 1);
			TextDrawSetShadow(ENTER_EXIT[total_enterexits][ee_TEXTDRAW], 0);
		}
		
		total_enterexits ++;
	}
	return 1;
}



forward OnPropertiesLoaded();
LoadProperties()
{
	inline OnPropertiesLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			for(new i = 0; i != rows; i ++)
			{
				if(i >= MAX_PROPERTIES)
				{
					printf("---> Límite superado en array 'PROPERTY_INFO' al intentar cargar de la base de datos.");
					break;
				}

				TOTAL_PROPERTIES_LOADED ++;

				new bool:isnull_name;
				PROPERTY_INFO[i][property_VALID] = true;
				cache_get_value_name_int(i, "id", PROPERTY_INFO[i][property_ID]);
				cache_is_value_name_null(i, "name", isnull_name);
				if(!isnull_name) cache_get_value_name(i, "name", PROPERTY_INFO[i][property_NAME]);
				cache_get_value_name_float(i, "ext_x", PROPERTY_INFO[i][property_EXT_X]);
				cache_get_value_name_float(i, "ext_y", PROPERTY_INFO[i][property_EXT_Y]);
				cache_get_value_name_float(i, "ext_z", PROPERTY_INFO[i][property_EXT_Z]);
				cache_get_value_name_float(i, "ext_angle", PROPERTY_INFO[i][property_EXT_ANGLE]);
				cache_get_value_name_int(i, "ext_interior", PROPERTY_INFO[i][property_EXT_INTERIOR]);
				cache_get_value_name_int(i, "ext_freeze", PROPERTY_INFO[i][property_EXT_FREEZE]);
				cache_get_value_name_int(i, "id_interior", PROPERTY_INFO[i][property_ID_INTERIOR]);
				cache_get_value_name_int(i, "price", PROPERTY_INFO[i][property_PRICE]);
				PROPERTY_INFO[i][property_PRICE_BASE] = PROPERTY_INFO[i][property_PRICE];
				cache_get_value_name_int(i, "level", PROPERTY_INFO[i][property_LEVEL]);
				cache_get_value_name_int(i, "extra", PROPERTY_INFO[i][property_EXTRA]);
				cache_get_value_name_int(i, "vip_level", PROPERTY_INFO[i][property_VIP_LEVEL]);
				cache_get_value_name_int(i, "dis_default_interior", PROPERTY_INFO[i][property_DIS_DEFAULT_INTERIOR]);

				if(PROPERTY_INFO[i][property_EXTRA]) PROPERTY_INFO[i][property_PRICE] = 0;
				if(PROPERTY_INFO[i][property_VIP_LEVEL]) PROPERTY_INFO[i][property_LEVEL] = 1;

				new id_player, pname[24], tid, bool:isnull_id_player, bool:isnull_pname, bool:isnull_tid;
				cache_is_value_name_null(i, "id_player", isnull_id_player);
				if(!isnull_id_player) cache_get_value_name_int(i, "id_player", id_player);
				cache_is_value_name_null(i, "pname", isnull_pname);
				if(!isnull_pname) cache_get_value_name(i, "pname", pname);
				cache_is_value_name_null(i, "tid", isnull_tid);
				if(!isnull_tid) cache_get_value_name_int(i, "tid", tid);
				if(id_player) CreatePropertyInfo(i, id_player, pname, 0, "");
				else if(tid)
				{
					inline OnCrewInfoLoad()
					{
						new crows;
						if(cache_get_row_count(crows))
						{
							if(crows)
							{
								new cid, cname[24];
								cache_get_value_index_int(0, 0, cid);
								cache_get_value_index(0, 1, cname);
								if(cid) CreatePropertyInfo(i, 0, "", cid, cname);
								else CreatePropertyInfo(i, 0, "", 0, "");
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT crews.id, crews.name FROM territories INNER JOIN crews ON territories.id_crew = crews.id WHERE territories.id = %d;", tid);
					MySQL_TQueryInline(srp_db,  using inline OnCrewInfoLoad, QUERY_BUFFER);
				}
				else CreatePropertyInfo(i, 0, "", 0, "");

				inline OnPropertyClosetLoad()
				{
					new crows;
					if(cache_get_row_count(crows))
					{
						for(new x = 0; x != crows; x ++)
						{
							PROPERTY_CLOSET[i][x][property_closet_VALID] = true;
							cache_get_value_name_int(x, "id", PROPERTY_CLOSET[i][x][property_closet_OBJECT_ID]);
							cache_get_value_name_int(x, "type", PROPERTY_CLOSET[i][x][property_closet_TYPE]);
							cache_get_value_name_int(x, "int", PROPERTY_CLOSET[i][x][property_closet_INT]);
							cache_get_value_name_int(x, "int_extra", PROPERTY_CLOSET[i][x][property_closet_INT_EXTRA]);
						}
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM property_closet WHERE id_property = %d LIMIT %d;", PROPERTY_INFO[i][property_ID], MAX_CLOSET_SLOTS);
				MySQL_TQueryInline(srp_db,  using inline OnPropertyClosetLoad, QUERY_BUFFER);
			}
			CallLocalFunction("OnPropertiesLoaded", "");
			LoadCrews();
		}
	}
	new query[] = "SELECT properties.*, player.id AS pid, player.name AS pname, territories.id AS tid FROM properties LEFT JOIN player ON properties.id_player = player.id LEFT JOIN territories ON properties.id_territory = territories.id;";
	MySQL_TQueryInline(srp_db, using inline OnPropertiesLoad, query);
	return 1;
}

GetOwnerIntProperty(id_house)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(ACCOUNT_INFO[i][ac_STATE] == ROLEPLAY_STATE_OWN_PROPERTY)
			{
				if(PI[i][ac_LOCAL_INTERIOR] == id_house)
				{
					return i;
				}
			}
		}
	}
	return -1;
}

LoadCrews()
{
	inline OnCrewsLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			for(new i = 0; i != rows; i ++)
			{
				if(i >= MAX_CREWS)
				{
					printf("---> Límite superado en array 'CREW_INFO' al intentar cargar de la base de datos.");
					break;
				}

				CREW_INFO[i][crew_VALID] = true;
				cache_get_value_name_int(i, "id", CREW_INFO[i][crew_ID]);
				cache_get_value_name(i, "name", CREW_INFO[i][crew_NAME]);
				cache_get_value_name_int(i, "color", CREW_INFO[i][crew_COLOR]);
				CREW_INFO[i][crew_ONLINE_MEMBERS] = 0;

				//count
				inline OnCountQueryLoad()
				{
					new crows;
					if(cache_get_row_count(crows))
					{
						if(crows)
						{
							cache_get_value_index_int(0, 0, CREW_INFO[i][crew_MEMBERS]);
						}
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d;", CREW_INFO[i][crew_ID]);
				MySQL_TQueryInline(srp_db,  using inline OnCountQueryLoad, QUERY_BUFFER);

				//ranks
				inline OnCrewRanksLoad()
				{
					new crows;
					if(cache_get_row_count(crows))
					{
						for(new x = 0; x != crows; x ++)
						{
							new rank_pos;
							cache_get_value_name_int(x, "rank_pos", rank_pos);
							if(rank_pos < 0 || rank_pos > MAX_CREW_RANKS) continue;

							CREW_RANK_INFO[i][rank_pos][crew_rank_VALID] = true;
							cache_get_value_name_int(x, "id", CREW_RANK_INFO[i][rank_pos][crew_rank_ID]);
							cache_get_value_name(x, "rank_name", CREW_RANK_INFO[i][rank_pos][crew_rank_NAME]);
							cache_get_value_name_int(x, "permission0", CREW_RANK_INFO[i][rank_pos][crew_rank_PERMISSION][0]);
							cache_get_value_name_int(x, "permission1", CREW_RANK_INFO[i][rank_pos][crew_rank_PERMISSION][1]);
							cache_get_value_name_int(x, "permission2", CREW_RANK_INFO[i][rank_pos][crew_rank_PERMISSION][2]);
							cache_get_value_name_int(x, "permission3", CREW_RANK_INFO[i][rank_pos][crew_rank_PERMISSION][3]);
							cache_get_value_name_int(x, "permission4", CREW_RANK_INFO[i][rank_pos][crew_rank_PERMISSION][4]);
							cache_get_value_name_int(x, "permission5", CREW_RANK_INFO[i][rank_pos][crew_rank_PERMISSION][5]);
							cache_get_value_name_int(x, "permission6", CREW_RANK_INFO[i][rank_pos][crew_rank_PERMISSION][6]);
							cache_get_value_name_int(x, "permission7", CREW_RANK_INFO[i][rank_pos][crew_rank_PERMISSION][7]);
							cache_get_value_name_int(x, "permission8", CREW_RANK_INFO[i][rank_pos][crew_rank_PERMISSION][8]);
							cache_get_value_name_int(x, "permission9", CREW_RANK_INFO[i][rank_pos][crew_rank_PERMISSION][9]);
							cache_get_value_name_int(x, "permission10", CREW_RANK_INFO[i][rank_pos][crew_rank_PERMISSION][10]);

							if(rank_pos == 0) {
								for(new j = 0; j < CREW_RANK_SIZE; j ++)
								CREW_RANK_INFO[i][rank_pos][crew_rank_PERMISSION][j] = 1;
							}
						}
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM crew_ranks WHERE id_crew = %d;", CREW_INFO[i][crew_ID]);
				MySQL_TQueryInline(srp_db,  using inline OnCrewRanksLoad, QUERY_BUFFER);
			}
			LoadGangZones();
		}
	}
	MySQL_TQueryInline(srp_db,  using inline OnCrewsLoad, "SELECT * FROM crews;");
	return 1;
}


LoadGangZones()
{
	inline OnSaZonesLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			for(new i = 0; i != rows; i ++)
			{
				if(i >= MAX_TERRITORIES)
				{
					printf("---> Límite superado en array 'TERRITORIES' al intentar cargar de la base de datos.");
					break;
				}

				new crewid, color, bool:isnull_crewid;
				TERRITORIES[i][territory_VALID] = true;
				cache_get_value_name_int(i, "id", TERRITORIES[i][territory_ID]);
				cache_get_value_name(i, "name", TERRITORIES[i][territory_NAME]);
				cache_get_value_name_float(i, "min_x", TERRITORIES[i][territory_MIN_X]);
				cache_get_value_name_float(i, "min_y", TERRITORIES[i][territory_MIN_Y]);
				cache_get_value_name_float(i, "min_z", TERRITORIES[i][territory_MIN_Z]);
				cache_get_value_name_float(i, "max_x", TERRITORIES[i][territory_MAX_X]);
				cache_get_value_name_float(i, "max_y", TERRITORIES[i][territory_MAX_Y]);
				cache_get_value_name_float(i, "max_z", TERRITORIES[i][territory_MAX_Z]);
				cache_is_value_name_null(i, "id_crew", isnull_crewid);
				if(!isnull_crewid) {
					cache_get_value_name_int(i, "id_crew", crewid);
					cache_get_value_name_int(i, "color", color);
				}
				TERRITORIES[i][territory_WAR] = false;
				TERRITORIES[i][territory_ATTACKER_CREW_INDEX] = 0;

				if(crewid)
				{
					TERRITORIES[i][territory_OCCUPIED] = true;
					TERRITORIES[i][territory_CREW_ID] = crewid;
					
					new r, g, b, a;
					HexToRGBA(color, r, g, b, a);
					TERRITORIES[i][territory_COLOR] = RGBAToHex(r, g, b, 135);
					TERRITORIES[i][territory_CREW_INDEX] = GetCrewIndexById(crewid);
				}
				else
				{
					TERRITORIES[i][territory_OCCUPIED] = false;
					TERRITORIES[i][territory_CREW_ID] = 0;
					TERRITORIES[i][territory_CREW_INDEX] = 0;
					TERRITORIES[i][territory_COLOR] = 0xCCCCCC55;
				}

				TERRITORIES[i][territory_AREA] = CreateDynamicCube(TERRITORIES[i][territory_MIN_X], TERRITORIES[i][territory_MIN_Y], TERRITORIES[i][territory_MIN_Z], TERRITORIES[i][territory_MAX_X], TERRITORIES[i][territory_MAX_Y], TERRITORIES[i][territory_MAX_Z], 0, 0);

				new info[2];
				info[0] = AREA_TYPE_GANGZONE;
				info[1] = i;
				Streamer_SetArrayData(STREAMER_TYPE_AREA, TERRITORIES[i][territory_AREA], E_STREAMER_EXTRA_ID, info);
				TERRITORIES[i][territory_GANG_ZONE] = GangZoneCreate(TERRITORIES[i][territory_MIN_X], TERRITORIES[i][territory_MIN_Y], TERRITORIES[i][territory_MAX_X], TERRITORIES[i][territory_MAX_Y]);
				
				TERRITORIES[i][territory_TEXTDRAW] = TextDrawCreate(320.000000, 406.000000, "Conquista:_0");
				TextDrawLetterSize(TERRITORIES[i][territory_TEXTDRAW], 0.286000, 1.276444);
				TextDrawAlignment(TERRITORIES[i][territory_TEXTDRAW], 2);
				TextDrawColor(TERRITORIES[i][territory_TEXTDRAW], -76);
				TextDrawSetShadow(TERRITORIES[i][territory_TEXTDRAW], 0);
				TextDrawSetOutline(TERRITORIES[i][territory_TEXTDRAW], 0);
				TextDrawBackgroundColor(TERRITORIES[i][territory_TEXTDRAW], 255);
				TextDrawFont(TERRITORIES[i][territory_TEXTDRAW], 1);
				TextDrawSetProportional(TERRITORIES[i][territory_TEXTDRAW], 1);
				TextDrawSetShadow(TERRITORIES[i][territory_TEXTDRAW], 0);
			}
			CallLocalFunction("OnTerritoriesLoaded", "");
			SetTimerEx("server_loaded_request", 1000, false, "b", true);
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT territories.*, crews.id AS crewid, crews.color FROM territories LEFT JOIN crews ON territories.id_crew = crews.id WHERE territories.gangzone = 1 LIMIT %d;", MAX_TERRITORIES);
	MySQL_TQueryInline(srp_db,   using inline OnSaZonesLoad, QUERY_BUFFER);
	return 1;
}

























GetHospitalSpawnPosition(hospital, &Float:x, &Float:y, &Float:z, &Float:angle, &interior, &local_interior, &rp_state)
{
	x = Hospital_Spawn_Positions[hospital][hspi_X];
	y = Hospital_Spawn_Positions[hospital][hspi_Y];
	z = Hospital_Spawn_Positions[hospital][hspi_Z];
	angle = Hospital_Spawn_Positions[hospital][hspi_ANGLE];
	interior = 0;
	local_interior = 0;
	rp_state = ROLEPLAY_STATE_NORMAL;
	return 1;
}

UpdateHospitalSizeTextdrawLife(playerid)
{
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2], 270.0 + ( PLAYER_TEMP[playerid][pt_HOSPITAL_LIFE] * 0.98), 0.000000);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2]);
	return 1;
}





#define MAX_INTERIOR_ACTORS 10


CreateInteriorActor(interior_type, world, interior)
{
	if(interior_type == INTERIOR_NO_INFO) return INVALID_ACTOR_ID;
	
	new InteriorActorInfo[MAX_INTERIOR_ACTORS][Interior_Actor_Enum];
	
	switch(interior_type)
	{
		/*case INTERIOR_GENERAL_HOSPITAL:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 308;
			InteriorActorInfo[0][ia_POS_X]	= 1969.236572;
			InteriorActorInfo[0][ia_POS_Y]	= 1169.541259;
			InteriorActorInfo[0][ia_POS_Z]	= -5.208700;
			InteriorActorInfo[0][ia_POS_A]	= 178.0;
		}*/
		case INTERIOR_BINCO:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 119;
			InteriorActorInfo[0][ia_POS_X]	= 208.376373;
			InteriorActorInfo[0][ia_POS_Y]	= -98.703956;
			InteriorActorInfo[0][ia_POS_Z]	= 1005.257812;
			InteriorActorInfo[0][ia_POS_A]	= 180.0;
		}
		case INTERIOR_SUBURBAN:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 191;
			InteriorActorInfo[0][ia_POS_X]	= 203.826187;
			InteriorActorInfo[0][ia_POS_Y]	= -41.667518;
			InteriorActorInfo[0][ia_POS_Z]	= 1001.804687;
			InteriorActorInfo[0][ia_POS_A]	= 180.0;
		}
		case INTERIOR_PROLAPS:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 190;
			InteriorActorInfo[0][ia_POS_X]	= 207.139907;
			InteriorActorInfo[0][ia_POS_Y]	= -127.805473;
			InteriorActorInfo[0][ia_POS_Z]	= 1003.507812;
			InteriorActorInfo[0][ia_POS_A]	= 180.0;
		}
		case INTERIOR_DIDIER_SACHS:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 169;
			InteriorActorInfo[0][ia_POS_X]	= 204.275909;
			InteriorActorInfo[0][ia_POS_Y]	= -157.829010;
			InteriorActorInfo[0][ia_POS_Z]	= 1000.523437;
			InteriorActorInfo[0][ia_POS_A]	= 180.0;
		}
		case INTERIOR_VICTIM:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 217;
			InteriorActorInfo[0][ia_POS_X]	= 204.853225;
			InteriorActorInfo[0][ia_POS_Y]	= -8.764448;
			InteriorActorInfo[0][ia_POS_Z]	= 1001.210937;
			InteriorActorInfo[0][ia_POS_A]	= 270.0;
		}
		case INTERIOR_ZIP:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 211;
			InteriorActorInfo[0][ia_POS_X]	= 162.226135;
			InteriorActorInfo[0][ia_POS_Y]	= -81.191978;
			InteriorActorInfo[0][ia_POS_Z]	= 1001.804687;
			InteriorActorInfo[0][ia_POS_A]	= 180.0;
		}
		case INTERIOR_247:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 184;
			InteriorActorInfo[0][ia_POS_X]	= -27.483411;
			InteriorActorInfo[0][ia_POS_Y]	= -91.628837;
			InteriorActorInfo[0][ia_POS_Z]	= 1003.546875;
			InteriorActorInfo[0][ia_POS_A]	= 0.0;
		}
		case INTERIOR_PIZZA:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 155;
			InteriorActorInfo[0][ia_POS_X] 	= 375.114501;
			InteriorActorInfo[0][ia_POS_Y] 	= -117.272621;
			InteriorActorInfo[0][ia_POS_Z] 	= 1001.492187;
			InteriorActorInfo[0][ia_POS_A]	= 180.0;
		}
		case INTERIOR_CLUCKIN_BELL:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 167;
			InteriorActorInfo[0][ia_POS_X] 	= 368.652679;
			InteriorActorInfo[0][ia_POS_Y] 	= -4.492218;
			InteriorActorInfo[0][ia_POS_Z] 	= 1001.851562;
			InteriorActorInfo[0][ia_POS_A]	= 180.0;
		}
		case INTERIOR_BURGER_SHOT:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 205;
			InteriorActorInfo[0][ia_POS_X] 	= 376.114227;
			InteriorActorInfo[0][ia_POS_Y] 	= -65.848991;
			InteriorActorInfo[0][ia_POS_Z] 	= 1001.507812;
			InteriorActorInfo[0][ia_POS_A]	= 180.0;
		}
		case INTERIOR_BANK_LS:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 59;
			InteriorActorInfo[0][ia_POS_X] 	= 1407.526489;
			InteriorActorInfo[0][ia_POS_Y] 	= 1321.833496;
			InteriorActorInfo[0][ia_POS_Z] 	= 1501.091918;
			InteriorActorInfo[0][ia_POS_A]	= 180.0;
		}
		case INTERIOR_BANK_SF:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 59;
			InteriorActorInfo[0][ia_POS_X] 	= 1104.802001;
			InteriorActorInfo[0][ia_POS_Y] 	= 1531.275634;
			InteriorActorInfo[0][ia_POS_Z] 	= 1452.475097;
			InteriorActorInfo[0][ia_POS_A]	= 180.0;
		}
		case INTERIOR_BANK_LV:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 59;
			InteriorActorInfo[0][ia_POS_X] 	= 2692.497558;
			InteriorActorInfo[0][ia_POS_Y] 	= -610.933593;
			InteriorActorInfo[0][ia_POS_Z] 	= -71.658203;
			InteriorActorInfo[0][ia_POS_A]	= 90.0;
		}
		case INTERIOR_CITY_HALL_LS:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 186;
			InteriorActorInfo[0][ia_POS_X] 	= -474.572387;
			InteriorActorInfo[0][ia_POS_Y] 	= 287.982879;
			InteriorActorInfo[0][ia_POS_Z] 	= 2004.584960;
			InteriorActorInfo[0][ia_POS_A]	= 0.0;
		}
		case INTERIOR_UNITY_STATION:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 184;
			InteriorActorInfo[0][ia_POS_X] 	= 1489.038818;
			InteriorActorInfo[0][ia_POS_Y] 	= 1305.631591;
			InteriorActorInfo[0][ia_POS_Z] 	= 1093.296386;
			InteriorActorInfo[0][ia_POS_A]	= 270.0;
		}
		case INTERIOR_TRUCK:
		{
			InteriorActorInfo[0][ia_VALID]	= true;
			InteriorActorInfo[0][ia_SKIN]	= 127;
			InteriorActorInfo[0][ia_POS_X] 	= 1297.120117;
			InteriorActorInfo[0][ia_POS_Y] 	= -66.351226;
			InteriorActorInfo[0][ia_POS_Z] 	= 1002.497375;
			InteriorActorInfo[0][ia_POS_A]	= 0.0;
		}
		default: return INVALID_ACTOR_ID;
	}

	new mainActor = INVALID_ACTOR_ID;
	for(new i; i != sizeof InteriorActorInfo; i++) {
		if(InteriorActorInfo[i][ia_VALID]) {
			new tmp = CreateDynamicActor(InteriorActorInfo[i][ia_SKIN], InteriorActorInfo[i][ia_POS_X], InteriorActorInfo[i][ia_POS_Y], InteriorActorInfo[i][ia_POS_Z], InteriorActorInfo[i][ia_POS_A], true, 100.0, world, interior);
			if(i == 0) {
				mainActor = tmp;
			}
		}
	}

	return mainActor;
}









HideToysShopTextdraws(playerid)
{
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][0]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][1]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][2]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][3]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][4]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][8]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][9]);
	return 1;
}

ClosePlayerToysMenu(playerid)
{
	SetPlayerVirtualWorld(playerid, ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INT_WORLD] );
	TogglePlayerControllableEx(playerid, true);
	
	HideToysShopTextdraws(playerid);
	//PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] = 0;
	PLAYER_TEMP[playerid][pt_TOYS_SHOP] = false;
	CancelSelectTextDrawEx(playerid);
	return 1;
}

RegisterNewPlayerToy(playerid, slot)
{
	inline OnPlayerToyInserted()
	{
		PLAYER_TOYS[playerid][slot][player_toy_ID] = cache_insert_id();
	}

	mysql_format
	(
		srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER,
		"\
			INSERT INTO ptoys(id_player, name, attached, modelid, bone, offset_x, offset_y, offset_z, rot_x, rot_y, rot_z, scale_x, scale_y, scale_z, color1, color2)\
			VALUES(%d, '%e', %d, %d, %d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %d, %d);\
		",
			ACCOUNT_INFO[playerid][ac_ID], PLAYER_TOYS[playerid][slot][player_toy_NAME], PLAYER_TOYS[playerid][slot][player_toy_ATTACHED], PLAYER_TOYS[playerid][slot][player_toy_MODELID],
			PLAYER_TOYS[playerid][slot][player_toy_BONE], PLAYER_TOYS[playerid][slot][player_toy_OFFSET_X], PLAYER_TOYS[playerid][slot][player_toy_OFFSET_Y],
			PLAYER_TOYS[playerid][slot][player_toy_OFFSET_Z], PLAYER_TOYS[playerid][slot][player_toy_ROT_X], PLAYER_TOYS[playerid][slot][player_toy_ROT_Y],
			PLAYER_TOYS[playerid][slot][player_toy_ROT_Z], PLAYER_TOYS[playerid][slot][player_toy_SCALE_X], PLAYER_TOYS[playerid][slot][player_toy_SCALE_Y],
			PLAYER_TOYS[playerid][slot][player_toy_SCALE_Z], PLAYER_TOYS[playerid][slot][player_toy_COLOR_1], PLAYER_TOYS[playerid][slot][player_toy_COLOR_2]
	);
	MySQL_TQueryInline(srp_db, using inline OnPlayerToyInserted, QUERY_BUFFER);
	return 1;
}







HideClothingShopTextdraws(playerid)
{
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][0]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][1]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][5]);
	return 1;
}

ClosePlayerClothingMenu(playerid)
{
	PI[playerid][ac_POS_X] = Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_X];
	PI[playerid][ac_POS_Y] = Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_Y];
	PI[playerid][ac_POS_Z] = Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_Z];
	PI[playerid][ac_ANGLE] = Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_ANGLE];
	SetPlayerSkin(playerid, PI[playerid][ac_SKIN]);
	TogglePlayerControllableEx(playerid, true);
	
	HideClothingShopTextdraws(playerid);
	PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
	PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] = -1;
	CancelSelectTextDrawEx(playerid);
	return 1;
}







stock number_format_dash(e_number) /* By adri1 - SFR3 4.3 */
{
	new f_string[24], bool:negative;
	format(f_string, sizeof f_string, "%d", e_number);
	if(e_number < 0)
	{
		negative = true;
		strdel(f_string, 0, 1);
	}
	
	new numbers = strlen(f_string);
	while(numbers > 3)
	{
		numbers -= 3;
		strins(f_string, "-", numbers);
	}
	
	if(negative) strins(f_string, "- ", 0);
	return f_string;
}

#define INTERVAL_HUNGRY_THIRST_DOWN	2 // 2 segundos de timer
#define THIRST_HOURS_STOPPED	9 // 9 horas parado si agua esta a 100 va a 0
#define HUNGRY_HOURS_STOPPED	12 // 12 horas parado si alimentacion esta a 100 va a 0





HidePlayerHud(playerid)
{
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][5]);
	PLAYER_TEMP[playerid][pt_HUD_TEXTDRAWS] = false;
	
	TextDrawHideForPlayer(playerid, Textdraws[textdraw_SERVER_TIME]);
	
	for(new i = 0; i < HUD_TEXTDRAWS_SIZE; i ++) {
		TextDrawHideForPlayer(playerid, Textdraws[textdraw_HUD][i]);	
	}
	
	for(new i = 0; i < HUD_PTEXTDRAWS_SIZE; i ++) {
		PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][i]);
	}

	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1]);
	return 1;
}

is_open(hour, open, close)
{
	if(open < 0 || close < 0) return false;
	if(open > close)
	{
		if( ( (hour >= open) && (hour < 24) ) || ( (hour >= 0) && (hour < close) ) )
		{													
			return true;			
		}
		else
		{
			return false;
		}
	}
	else if(open < close)
	{
		if((hour >= open) && (hour < close))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}



SetActorSkin(actorid, skin, &actor)
{
	actor = INVALID_ACTOR_ID;
	
    new Float:x, Float:y, Float:z, Float:a, vw;
    GetActorPos(actorid, x, y, z); 
    GetActorFacingAngle(actorid, a);
	vw = GetActorVirtualWorld(actorid);
    DestroyActor(actorid); 
    actor = CreateActor(skin, x, y, z, a);
	SetActorVirtualWorld(actor, vw);
	return 1;
}

stock SetActorFacingAngleEx(actorid, skin, Float:angle, &actor)
{
	actor = INVALID_ACTOR_ID;
	
    new Float:x, Float:y, Float:z, vw;
    GetActorPos(actorid, x, y, z); 
	vw = GetActorVirtualWorld(actorid);
    DestroyActor(actorid); 
    actor = CreateActor(skin, x, y, z, angle);
	SetActorVirtualWorld(actor, vw);
	return 1;
}







const CRASHER_PACKET = 228;







AddTestVehicle(bool:static_veh, modelid, Float:X, Float:Y, Float:Z, Float:Angle, Color1, Color2, interior = 0, world = 0)
{
	new vehicle_id = INVALID_VEHICLE_ID;
	if(static_veh) vehicle_id = AddStaticVehicleEx(modelid, X, Y, Z, Angle, Color1, Color2, -1, false);
	else vehicle_id = CreateVehicle(modelid, X, Y, Z, Angle, Color1, Color2, -1, false);
	
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_VALID] = true;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_TYPE] = VEHICLE_TYPE_TEST;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] = modelid;
	format(GLOBAL_VEHICLES[vehicle_id][gb_vehicle_NUMBER_PLATE], 32, "Test Vehicle");
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X] = X;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y] = Y;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z] = Z;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_ANGLE] = Angle;
	
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][0] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][1] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][2] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z];
	
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_HEALTH] = 1000.0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_PANELS] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_DOORS] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_LIGHTS] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_TIRES] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_1] = Color1;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_2] = Color2;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PAINTJOB] = 3; // No paintjob
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS] = VEHICLE_INFO[modelid - 400][vehicle_info_MAX_GAS];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_GAS] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_STATE] = VEHICLE_STATE_NORMAL;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_INTERIOR] = interior;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_WORLD] = 0;
	SetVehicleVirtualWorldEx(vehicle_id, world);
	SetVehicleToRespawnEx(vehicle_id);
	return vehicle_id;
}

AddPersonalVehicle(playerid, modelid, Float:X, Float:Y, Float:Z, Float:Angle, Color1, Color2, Float:max_gas)
{
	new vehicle_id = INVALID_VEHICLE_ID;
	vehicle_id = CreateVehicle(modelid, X, Y, Z, Angle, Color1, Color2, -1);
	
	if(vehicle_id == INVALID_VEHICLE_ID) return false;
	
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_VALID] = true;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_TYPE] = VEHICLE_TYPE_PERSONAL;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] = modelid;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X] = X;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y] = Y;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z] = Z;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_ANGLE] = Angle;
	
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][0] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][1] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][2] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z];
	
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_HEALTH] = 1000.0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_PANELS] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_DOORS] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_LIGHTS] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_TIRES] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_1] = Color1;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_2] = Color2;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PAINTJOB] = 3; // No paintjob
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS] = max_gas;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_GAS] = max_gas;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_STATE] = VEHICLE_STATE_NORMAL;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_INTERIOR] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_WORLD] = 0;
	
	PLAYER_VEHICLES[vehicle_id][player_vehicle_VALID] = true;
	PLAYER_VEHICLES[vehicle_id][player_vehicle_OWNER_ID] = ACCOUNT_INFO[playerid][ac_ID];
	RegisterNewPlayerVehicle(playerid, vehicle_id);
	
	SetVehicleToRespawnEx(vehicle_id);
	return vehicle_id;
}

RegisterNewPlayerVehicle(playerid, vehicleid)
{
	inline OnVehicleInserted()
	{
		PLAYER_VEHICLES[vehicleid][player_vehicle_ID] = cache_insert_id();
		if(PLAYER_VEHICLES[vehicleid][player_vehicle_ID])
		{
			format(GLOBAL_VEHICLES[vehicleid][gb_vehicle_NUMBER_PLATE], 32, "%04d %c%c%c", PLAYER_VEHICLES[vehicleid][player_vehicle_ID], getRandomLetter(), getRandomLetter(), getRandomLetter());
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pvehicles SET plate = '%e' WHERE id = %d;", GLOBAL_VEHICLES[vehicleid][gb_vehicle_NUMBER_PLATE], PLAYER_VEHICLES[vehicleid][player_vehicle_ID]);
			mysql_tquery(srp_db, QUERY_BUFFER);
		}
	}

	mysql_format
	(
		srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER,
		"\
			INSERT INTO pvehicles (id_player, modelid, spawn_x, spawn_y, spawn_z, spawn_angle, color1, color2, gas, max_gas)\
			VALUES(%d, %d, %f, %f, %f, %f, %d, %d, %f, %f);\
		",
			ACCOUNT_INFO[playerid][ac_ID], GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID], GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_X], GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Y], GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Z],
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_ANGLE], GLOBAL_VEHICLES[vehicleid][gb_vehicle_COLOR_1], GLOBAL_VEHICLES[vehicleid][gb_vehicle_COLOR_2], GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS],
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS]
	);
	MySQL_TQueryInline(srp_db, using inline OnVehicleInserted, QUERY_BUFFER);
	return 1;
}







SetClampVehicleLabel(vehicleid) {
	if(PLAYER_VEHICLES[vehicleid][player_vehicle_VALID]) {
		if(IsValidDynamic3DTextLabel(GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL]))
		{
			DestroyDynamic3DTextLabel(GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL]);
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL] = Text3D:INVALID_STREAMER_ID;
		}

		if(PLAYER_VEHICLES[vehicleid][player_vehicle_CLAMP] > 0) {
			new string[256];
			format(string, sizeof string, "Cepo\nUsa {"#PRIMARY_COLOR"}/pagar {FFFFFF}para pagar la multa por %s$", number_format_thousand(PLAYER_VEHICLES[vehicleid][player_vehicle_CLAMP]));

			new Float:wX, Float:wY, Float:wZ;
			GetVehicleModelInfo(GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID], VEHICLE_MODEL_INFO_WHEELSREAR, wX, wY, wZ);
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL] = CreateDynamic3DTextLabel(string, -1, wX, wY, wZ, 20.0, .attachedvehicle = vehicleid);
		}
	}
}









GetPlayerIdFromAccountId(accountid)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(ACCOUNT_INFO[i][ac_ID] == accountid)
			{
				return i;
			}
		}
	}
	return INVALID_PLAYER_ID;
}

#define NECESSARY_TIME_BETWEEN_WORKS	600 //segundos







//RotateDynamicObject(objectid, Float:rotX, Float:rotY, Float:rotZ, Float:Speed)
//{
//	
//		Rotation Speeds:
//		0.01 - Very very slow speed
//		1.00 - Normal speed
//		5.00 - Fast speed
//	
//	new Float:X, Float:Y, Float:Z;
//	new Float:SpeedConverted = floatdiv(floatmul(Speed, 0.01), 2);
//	
//	if(IsDynamicObjectMoving(objectid)) StopDynamicObject(objectid);
//	GetDynamicObjectPos(objectid, X, Y, Z);
//	SetDynamicObjectPos(objectid, X, Y, Z + 0.01);
//	MoveDynamicObject(objectid, X, Y, Z, SpeedConverted, rotX, rotY, rotZ);
//	return 1;
//}





SetPlayerTruckCheckpoint(playerid, vehicleid)
{
	if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT]))
	{
		DestroyDynamicCP(PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT] = INVALID_STREAMER_ID;
	}

	if(TRUCK_VEHICLE[vehicleid][truck_vehicle_DELIVERED])
	{
		PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT] = CreateDynamicCP(Truck_Return_Positions[0], Truck_Return_Positions[1], Truck_Return_Positions[2], 5.0, 0, 0, playerid, 9999999999.0);
		
		new info[1];
		info[0] = CHECKPOINT_TYPE_FINISH_TRUCK;
		Streamer_SetArrayData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT], E_STREAMER_EXTRA_ID, info);
	}
	else
	{
		PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT] = CreateDynamicCP(Truck_Contents[ TRUCK_VEHICLE[vehicleid][truck_vehicle_POINT] ][truck_content_X], Truck_Contents[ TRUCK_VEHICLE[vehicleid][truck_vehicle_POINT] ][truck_content_Y], Truck_Contents[ TRUCK_VEHICLE[vehicleid][truck_vehicle_POINT] ][truck_content_Z], 5.0, 0, 0, playerid, 9999999999.0);
		
		new info[1];
		info[0] = CHECKPOINT_TYPE_UNLOAD_TRUCK;
		Streamer_SetArrayData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT], E_STREAMER_EXTRA_ID, info);
	}
	Streamer_Update(playerid, STREAMER_TYPE_CP);
	return 1;
}



CMD:basurero(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_TRASH][pwork_SET]) return SendNotification(playerid, "No eres basurero.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_NONE && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_TRASH)
	{
		SendFormatNotification(playerid, "Tienes que dejar de estar de servicio como %s primero.", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME]);
		return 1;
	}
	
	if(GetPlayerVirtualWorld(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerInterior(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	if(!IsPlayerInRangeOfPoint(playerid, 1.0, -1906.577514, -1756.457519, 22.079319)) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	if(!PLAYER_TEMP[playerid][pt_WORKING_IN]) CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_TRASH, INVALID_VEHICLE_ID);
	else CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
	return 1;
}







SetPlayerTrashCheckpoint(playerid, vehicleid)
{
	DestroyPlayerTrashCheckpoint(playerid);
	
	new cp = TRASH_VEHICLES[ vehicleid ][trash_vehicle_CP];
	new route = TRASH_VEHICLES[ vehicleid ][trash_vehicle_ROUTE];
	
	if(cp >= TRASH_OBJECTS[route][trash_route_OBJECTS])
	{
		PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT] = CreateDynamicCP(-1872.575073, -1720.129394, 22.30542, 5.0, 0, 0, playerid, 9999999999.0);
		new info[1];
		info[0] = CHECKPOINT_TYPE_FINISH_TRASH;
		Streamer_SetArrayData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT], E_STREAMER_EXTRA_ID, info);
	}
	else
	{
		PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT] = CreateDynamicCP(TRASH_OBJECTS[ route ][trash_object_X][ cp ], TRASH_OBJECTS[ route ][trash_object_Y][ cp ], TRASH_OBJECTS[ route ][trash_object_Z][ cp ], 1.5, 0, 0, playerid, 9999999999.0);
		
		new info[1];
		info[0] = CHECKPOINT_TYPE_TRASH;
		Streamer_SetArrayData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT], E_STREAMER_EXTRA_ID, info);
	}
	return 1;
}

DestroyPlayerTrashCheckpoint(playerid)
{
	if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT]))
	{
		DestroyDynamicCP(PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT] = INVALID_STREAMER_ID;
	}
	return 1;
}

CancelTrashWork(driver, mate, vehicleid)
{
	DestroyPlayerTrashCheckpoint(driver);
	DestroyPlayerTrashCheckpoint(mate);
	
	DestroyPlayerTrashRouteObjects(driver);
	DestroyPlayerTrashRouteObjects(mate);
	
	PLAYER_TEMP[driver][pt_TRASH_DRIVER] = false;
	PLAYER_TEMP[driver][pt_TRASH_PASSENGER] = false;
	PLAYER_TEMP[driver][pt_TRASH_VEHICLE_ID] = INVALID_VEHICLE_ID;
	
	PLAYER_TEMP[mate][pt_TRASH_DRIVER] = false;
	PLAYER_TEMP[mate][pt_TRASH_PASSENGER] = false;
	PLAYER_TEMP[mate][pt_TRASH_VEHICLE_ID] = INVALID_VEHICLE_ID;
	
	KillTimer(PLAYER_TEMP[mate][pt_TIMERS][10]);
	PlayerTextDrawHide(mate, PlayerTextdraws[mate][ptextdraw_RECYCLE_TRASH][0]);
	PlayerTextDrawHide(mate, PlayerTextdraws[mate][ptextdraw_RECYCLE_TRASH][1]);
	PlayerTextDrawHide(mate, PlayerTextdraws[mate][ptextdraw_RECYCLE_TRASH][2]);
	ClearAnimations(mate);
	TogglePlayerControllableEx(mate, true);
	
	ResetTrashInfo(vehicleid);
	SetVehicleToRespawnEx(vehicleid);
	return 1;
}

UpdatePlayerTrashRecycleSize(playerid)
{
	PlayerTextDrawTextSize(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2], 0.0, PLAYER_TEMP[playerid][pt_RECYCLE_BIN_VALUE] * 1.3);
	PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2]);
	return 1;
}



CancelLumberjackWork(playerid)
{
	if(PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] != -1)
	{
		DestroyDynamicObject(LUMBER_TREES[ PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] ][lumber_tree_OBJECT_ID]);
		LUMBER_TREES[ PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] ][lumber_tree_OBJECT_ID] = CreateDynamicObject(LUMBER_TREES[ PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] ][lumber_tree_MODELID], LUMBER_TREES[ PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] ][lumber_tree_X], LUMBER_TREES[ PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] ][lumber_tree_Y], LUMBER_TREES[ PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] ][lumber_tree_Z], LUMBER_TREES[ PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] ][lumber_tree_RX], LUMBER_TREES[ PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] ][lumber_tree_RY], LUMBER_TREES[ PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] ][lumber_tree_RZ], 0, 0);
		LUMBER_TREES[ PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] ][lumber_tree_STATE] = LUMBER_TREE_STATE_NORMAL;
		
		new label_str[256];
		format(label_str, sizeof label_str, "{f46530}Árbol (%d)\n\n{FFFFFF}Pulsa {f9d834}~k~~CONVERSATION_NO~ {FFFFFF}para talar el árbol.", PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE]);
		UpdateDynamic3DTextLabelText(LUMBER_TREES[ PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] ][lumber_tree_LABEL], -1, label_str);
	}
	
	if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT]))
	{
		DestroyDynamicCP(PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT] = INVALID_STREAMER_ID;
	}
	
	RemovePlayerAttachedObject(playerid, 8);
	RemovePlayerAttachedObject(playerid, 9);
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][11]);
	//ClearAnimations(playerid);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	TogglePlayerControllableEx(playerid, true);
	PLAYER_TEMP[playerid][pt_PLAYER_CARRYING_TREE] = false;
	PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] = -1;
	DestroyPlayerKeyPress(playerid);
	SetPlayerToys(playerid);
	return 1;
}

CreateFlashObject(Float:x, Float:y, Float:z)
{
	new oid = CreateObject(18680, x, y, z, 0.0, 0.0, 0.0);
	SetTimerEx("DestroyFlashObject", 1000, false, "i", oid);
	return 1;
}



ShowPlayerInventory(playerid, pid)
{
	if(!IsPlayerConnected(pid)) return 0;
	if(ACCOUNT_INFO[pid][ac_ID] == 0) return 0;
	
	new caption[48];
	format(caption, sizeof caption, "Inventario de %s", PLAYER_TEMP[pid][pt_RP_NAME]);
	
	new dialog[1800], line_str[128];
	
	format(line_str, sizeof line_str, "{"#SILVER_COLOR"}Dinero {"#SILVER_COLOR"}(%s$)\n", number_format_thousand(PI[pid][ac_CASH]));
	strcat(dialog, line_str);
	
	if(PI[pid][ac_DRIVE_LICENSE_POINTS])
	{
		format(line_str, sizeof line_str, "Licencia de conducir: {"#SILVER_COLOR"}sí, %d puntos\n", PI[pid][ac_DRIVE_LICENSE_POINTS]);
		strcat(dialog, line_str);
	}
	else
	{
		format(line_str, sizeof line_str, "Licencia de conducir: {"#SILVER_COLOR"}no\n");
		strcat(dialog, line_str);
	}

	if(PI[pid][ac_PHONE_NUMBER])
	{
		format(line_str, sizeof line_str, "Teléfono {"#SILVER_COLOR"}(%d)\n", PI[pid][ac_PHONE_NUMBER]);
		strcat(dialog, line_str);
	}
	
	new toys = CountPlayerToys(pid);
	if(toys > 0)
	{
		format(line_str, sizeof line_str, "Accesorios {"#SILVER_COLOR"}(%d)\n", toys);
		strcat(dialog, line_str);
	}
	
	new pocket_objects = CountPlayerPocketObjects(pid);
	if(pocket_objects > 0)
	{
		format(line_str, sizeof line_str, "Alimentos {"#SILVER_COLOR"}(%d)\n", pocket_objects);
		strcat(dialog, line_str);
	}
	
	if(PI[pid][ac_GPS]) strcat(dialog, "GPS\n");
	if(PI[pid][ac_Maso]) strcat(dialog, "Masa\n");
	if(PI[pid][ac_MP3]) strcat(dialog, "Reproductor MP3\n");
	if(PI[pid][ac_PHONE_RESOLVER]) strcat(dialog, "Guía telefónica\n");
	if(PI[pid][ac_SPEAKERS]) strcat(dialog, "Altavoces\n");
	
	new player_weapons = CountPlayerWeapons(pid);
	if(player_weapons > 0)
	{
		if(PLAYER_WORKS[pid][WORK_POLICE][pwork_SET]) format(line_str, sizeof line_str, "{"#PRIMARY_COLOR"}Armas {"#SILVER_COLOR"}(%d)\n", player_weapons);
		else format(line_str, sizeof line_str, "{"#RED_COLOR"}Armas {"#SILVER_COLOR"}(%d)\n", player_weapons);
		strcat(dialog, line_str);
	}
	
	if(PI[pid][ac_MECHANIC_PIECES] > 0)
	{
		format(line_str, sizeof line_str, "Piezas de mecánico {"#SILVER_COLOR"}(%s)\n", number_format_thousand(PI[pid][ac_MECHANIC_PIECES]));
		strcat(dialog, line_str);
	}
	
	if(PI[pid][ac_FUEL_DRUM] > 0)
	{
		format(line_str, sizeof line_str, "Bidón de gasolina {"#SILVER_COLOR"}(%d Litros)\n", PI[pid][ac_FUEL_DRUM]);
		strcat(dialog, line_str);
	}
	
	if(PI[pid][ac_SEED_MEDICINE] > 0)
	{
		format(line_str, sizeof line_str, "{"#BLUE_COLOR"}Semillas de medicina {"#SILVER_COLOR"}(%d)\n", PI[pid][ac_SEED_MEDICINE]);
		strcat(dialog, line_str);
	}
	
	if(PI[pid][ac_SEED_CANNABIS] > 0)
	{
		format(line_str, sizeof line_str, "{"#RED_COLOR"}Semillas de marihuana {"#SILVER_COLOR"}(%d)\n", PI[pid][ac_SEED_CANNABIS]);
		strcat(dialog, line_str);
	}
	
	if(PI[pid][ac_SEED_CRACK] > 0)
	{
		format(line_str, sizeof line_str, "{"#RED_COLOR"}Semillas de coca {"#SILVER_COLOR"}(%d)\n", PI[pid][ac_SEED_CRACK]);
		strcat(dialog, line_str);
	}
	
	if(PI[pid][ac_MEDICINE] > 0)
	{
		format(line_str, sizeof line_str, "{"#BLUE_COLOR"}Medicamentos {"#SILVER_COLOR"}(%dg)\n", PI[pid][ac_MEDICINE]);
		strcat(dialog, line_str);
	}
	
	if(PI[pid][ac_CANNABIS] > 0)
	{
		format(line_str, sizeof line_str, "{"#RED_COLOR"}Marihuana {"#SILVER_COLOR"}(%dg)\n", PI[pid][ac_CANNABIS]);
		strcat(dialog, line_str);
	}
	
	if(PI[pid][ac_CRACK] > 0)
	{
		format(line_str, sizeof line_str, "{"#RED_COLOR"}Crack {"#SILVER_COLOR"}(%dg)\n", PI[pid][ac_CRACK]);
		strcat(dialog, line_str);
	}

	if(PI[pid][ac_MECHANIC_KITS] > 0)
	{
		format(line_str, sizeof line_str, "Kits de reparación (%d)\n", PI[pid][ac_MECHANIC_KITS]);
		strcat(dialog, line_str);
	}

	if(PI[pid][ac_MEDICAL_KITS] > 0)
	{
		format(line_str, sizeof line_str, "Botiquines (%d)\n", PI[pid][ac_MEDICAL_KITS]);
		strcat(dialog, line_str);
	}
	
	ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, caption, dialog, "Cerrar", "");	
	return 1;
}

CountPlayerToys(playerid)
{
	new toys;
	
	for(new i = 0; i != MAX_SU_TOYS; i ++)
	{
		if(PLAYER_TOYS[playerid][i][player_toy_VALID])
		{
			toys ++;
		}
	}
	return toys;
}

CountPlayerPocketObjects(playerid)
{
	new objects;
	
	for(new i = 0; i != MAX_PLAYER_POCKET_OBJECTS; i ++)
	{
		if(PLAYER_POCKET[playerid][i][player_pocket_VALID])
		{
			objects ++;
		}
	}
	return objects;
}

CountPlayerWeapons(playerid)
{
	new weapons;
	
	for(new i = 0; i != sizeof PLAYER_WEAPONS[]; i ++)
	{
		if(PLAYER_WEAPONS[playerid][i][player_weapon_VALID])
		{
			weapons ++;
		}
	}
	return weapons;
}





CreatePlayerTrashRouteObjects(playerid, route)
{
	for(new i = 0; i != MAX_OBJECTS_PER_ROUTE; i ++)
	{
		TRASH_PLAYER_OBJECTS[playerid][i] = INVALID_STREAMER_ID;
		
		if(TRASH_OBJECTS[route][trash_object_MODELID][i] != 0)
		{
			TRASH_PLAYER_OBJECTS[playerid][i] = CreateDynamicObject(TRASH_OBJECTS[route][trash_object_MODELID][i], TRASH_OBJECTS[route][trash_object_X][i], TRASH_OBJECTS[route][trash_object_Y][i], TRASH_OBJECTS[route][trash_object_Z][i], TRASH_OBJECTS[route][trash_object_RX][i], TRASH_OBJECTS[route][trash_object_RY][i], TRASH_OBJECTS[route][trash_object_RZ][i], 0, 0, playerid);
		}
	}
	return 1;
}

DestroyPlayerTrashRouteObjects(playerid)
{
	for(new i = 0; i != MAX_OBJECTS_PER_ROUTE; i ++)
	{
		if(TRASH_PLAYER_OBJECTS[playerid][i] != INVALID_STREAMER_ID) DestroyDynamicObject(TRASH_PLAYER_OBJECTS[playerid][i]);
		TRASH_PLAYER_OBJECTS[playerid][i] = INVALID_STREAMER_ID;
	}
	return 1;
}

CreatePlayerFlashObject(playerid, Float:x, Float:y, Float:z)
{
	new oid = CreatePlayerObject(playerid, 18680, x, y, z, 0.0, 0.0, 0.0);
	SetTimerEx("DestroyPlayerFlashObject", 1000, false, "ii", playerid, oid);
	return 1;
}



GetVehicleFirstPassenger(vehicleid)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(GetPlayerState(i) == PLAYER_STATE_PASSENGER)
			{
				if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
				{
					if(GetPlayerVehicleID(i) == vehicleid)
					{
						return i;
					}
				}
			}
		}
	}
	return INVALID_PLAYER_ID;
}

CancelPlayerPlanting(playerid)
{
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][12]);
	PLAYER_TEMP[playerid][pt_PLANTING] = false;
	DestroyPlayerKeyPress(playerid);
	TogglePlayerControllableEx(playerid, true);
	ClearAnimations(playerid);
	return 1;
}

GetAvaiblePlantIndex()
{
	for(new i = 0; i != sizeof PLANTS; i ++)
	{
		if(!PLANTS[i][plant_VALID]) return i;
	}
	return -1;
}






	

#define lc_limit_X_WEST 4000.0
#define lc_limit_X_EAST 7500.0
#define lc_limit_Y_NORTH 6900.0
#define lc_limit_Y_SOUTH 4150.0

stock Lc3dTo2d(Float:map_x, Float:map_y, Float:map_size_x, Float:map_size_y, Float:x, Float:y, Float:z, &Float:td_x, &Float:td_y)
{
	#pragma unused z

	// Map limits 
	if(x > lc_limit_X_EAST) x = lc_limit_X_EAST;
	else if(x < lc_limit_X_WEST) x = lc_limit_X_WEST;

	if(y > lc_limit_Y_NORTH) y = lc_limit_Y_NORTH;
	else if(y < lc_limit_Y_SOUTH) y = lc_limit_Y_SOUTH;

	// Calculations 
	new Float:map_width = floatsub(lc_limit_X_EAST, lc_limit_X_WEST),
		Float:prop_X = floatdiv(map_size_x, map_width),
		Float:mv_X = floatsub(map_width, lc_limit_X_EAST);

	new Float:map_height = floatsub(lc_limit_Y_SOUTH, lc_limit_Y_NORTH),
		Float:prop_Y = floatdiv(map_size_y, map_height),
		Float:mv_Y = floatsub(map_height, lc_limit_Y_SOUTH);
	
	/// Conversion 
	x += mv_X;
	y += mv_Y;

	/// Result 
	td_x = map_x + floatmul(prop_X, x),
	td_y = map_y + floatmul(prop_Y, y);
	return 1;
}

stock GetIconTextdraw(iconid)
{
	new td_icon[24];
	
	switch(iconid)
	{
		case 2: td_icon = "hud:radar_centre";
		case 3: td_icon = "hud:arrow";
		case 4: td_icon = "hud:radar_north";
		case 5: td_icon = "hud:radar_airyard";
		case 6: td_icon = "hud:radar_ammugun";
		case 7: td_icon = "hud:radar_barbers";
		case 8: td_icon = "hud:radar_bigsmoke";
		case 9: td_icon = "hud:radar_boatyard";
		case 10: td_icon = "hud:radar_burgershot";
		case 11: td_icon = "hud:radar_bulldozer";
		case 12: td_icon = "hud:radar_catalinapink";
		case 13: td_icon = "hud:radar_cesarviapando";
		case 14: td_icon = "hud:radar_chicken";
		case 15: td_icon = "hud:radar_cj";
		case 16: td_icon = "hud:radar_crash1";
		case 17: td_icon = "hud:radar_diner";
		case 18: td_icon = "hud:radar_emmetgun";
		case 19: td_icon = "hud:radar_enemyattack";
		case 20: td_icon = "hud:radar_fire";
		case 21: td_icon = "hud:radar_girlfriend";
		case 22: td_icon = "hud:radar_hostpital";
		case 23: td_icon = "hud:radar_locosyndicate";
		case 24: td_icon = "hud:radar_maddog";
		case 25: td_icon = "hud:radar_mafiacasino";
		case 26: td_icon = "hud:radar_mcstrap";
		case 27: td_icon = "hud:radar_modgarage";
		case 28: td_icon = "hud:radar_ogloc";
		case 29: td_icon = "hud:radar_pizza";
		case 30: td_icon = "hud:radar_police";
		case 31: td_icon = "hud:radar_propertyg";
		case 32: td_icon = "hud:radar_propertyr";
		case 33: td_icon = "hud:radar_race";
		case 34: td_icon = "hud:radar_ryder";
		case 35: td_icon = "hud:radar_savegame";
		case 36: td_icon = "hud:radar_school";
		case 37: td_icon = "hud:radar_qmark";
		case 38: td_icon = "hud:radar_sweet";
		case 39: td_icon = "hud:radar_tattoo";
		case 40: td_icon = "hud:radar_thetruth";
		case 41: td_icon = "hud:radar_waypoint";
		case 42: td_icon = "hud:radar_toreno";
		case 43: td_icon = "hud:radar_triads";
		case 44: td_icon = "hud:radar_triadscasino";
		case 45: td_icon = "hud:radar_tshirt";
		case 46: td_icon = "hud:radar_woozie";
		case 47: td_icon = "hud:radar_zero";
		case 48: td_icon = "hud:radar_datedisco";
		case 49: td_icon = "hud:radar_datedrink";
		case 50: td_icon = "hud:radar_datefood";
		case 51: td_icon = "hud:radar_truck";
		case 52: td_icon = "hud:radar_cash";
		case 53: td_icon = "hud:radar_flag";
		case 54: td_icon = "hud:radar_gym";
		case 55: td_icon = "hud:radar_impound";
		case 56: td_icon = "hud:radar_light";
		case 57: td_icon = "hud:radar_runway";
		case 58: td_icon = "hud:radar_gangb";
		case 59: td_icon = "hud:radar_gangp";
		case 60: td_icon = "hud:radar_gangy";
		case 61: td_icon = "hud:radar_gangn";
		case 62: td_icon = "hud:radar_gangg";
		case 63: td_icon = "hud:radar_spray";
	}
	
	return td_icon;
}

GivePlayerWeaponEx(playerid, weapon_id, ammo)
{
	PLAYER_AC_INFO[playerid][CHEAT_WEAPON][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_AMMO][p_ac_info_IMMUNITY] = gettime() + 3;

	if(PLAYER_WEAPONS[playerid][ WEAPON_INFO[weapon_id][weapon_info_SLOT] ][player_weapon_ID] == weapon_id) PLAYER_WEAPONS[playerid][ WEAPON_INFO[weapon_id][weapon_info_SLOT] ][player_weapon_AMMO] += ammo;
	else PLAYER_WEAPONS[playerid][ WEAPON_INFO[weapon_id][weapon_info_SLOT] ][player_weapon_AMMO] = ammo;

	PLAYER_WEAPONS[playerid][ WEAPON_INFO[weapon_id][weapon_info_SLOT] ][player_weapon_VALID] = true;
	PLAYER_WEAPONS[playerid][ WEAPON_INFO[weapon_id][weapon_info_SLOT] ][player_weapon_ID] = weapon_id;
	
	SetPlayerWeapon(playerid, WEAPON_INFO[weapon_id][weapon_info_SLOT]);
	return 1;
}


CheckWeaponAndAmmo(playerid, weaponid)
{
	if(WEAPON_INFO[weaponid][weapon_info_ANTI_CHEAT])
	{
		new weapon_id, weapon_ammo;
		GetPlayerWeaponData(playerid, WEAPON_INFO[weaponid][weapon_info_SLOT], weapon_id, weapon_ammo);
		
		if(ac_Info[CHEAT_WEAPON][ac_Enabled])
		{
			if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_WEAPON][p_ac_info_IMMUNITY])
			{
				if(weapon_id != PLAYER_WEAPONS[playerid][ WEAPON_INFO[weapon_id][weapon_info_SLOT] ][player_weapon_ID])
				{
					if(!ac_Info[CHEAT_WEAPON][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_WEAPON);
					else
					{
						if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_WEAPON][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_WEAPON][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_WEAPON][p_ac_info_DETECTIONS] = 0;
						else PLAYER_AC_INFO[playerid][CHEAT_WEAPON][p_ac_info_DETECTIONS] ++;
						
						PLAYER_AC_INFO[playerid][CHEAT_WEAPON][p_ac_info_LAST_DETECTION] = gettime();
						if(PLAYER_AC_INFO[playerid][CHEAT_WEAPON][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_WEAPON][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_WEAPON);
					}
				}
			}
		}
		
		if(WEAPON_INFO[weapon_id][weapon_info_AMMO])
		{
			if(ac_Info[CHEAT_AMMO][ac_Enabled])
			{
				if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_AMMO][p_ac_info_IMMUNITY])
				{
					if(weapon_ammo > PLAYER_WEAPONS[playerid][ WEAPON_INFO[weapon_id][weapon_info_SLOT] ][player_weapon_AMMO] + 10)
					{
				
						if(!ac_Info[CHEAT_AMMO][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_AMMO);
						else
						{
							if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_AMMO][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_AMMO][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_AMMO][p_ac_info_DETECTIONS] = 0;
							else PLAYER_AC_INFO[playerid][CHEAT_AMMO][p_ac_info_DETECTIONS] ++;
							
							PLAYER_AC_INFO[playerid][CHEAT_AMMO][p_ac_info_LAST_DETECTION] = gettime();
							if(PLAYER_AC_INFO[playerid][CHEAT_AMMO][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_AMMO][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_AMMO);
						}
					}
					else PLAYER_WEAPONS[playerid][ WEAPON_INFO[weapon_id][weapon_info_SLOT] ][player_weapon_AMMO] = weapon_ammo;
				}
			}
		}
	}
	return 1;
}




RegisterNewPlayerWeapon(playerid, weapon_slot)
{
	inline OnPweaponInserted()
	{
		PLAYER_WEAPONS[playerid][weapon_slot][player_weapon_DB_ID] = cache_insert_id();
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO pweapons (id_player, weaponid, ammo) VALUES (%d, %d, %d);", ACCOUNT_INFO[playerid][ac_ID], PLAYER_WEAPONS[playerid][weapon_slot][player_weapon_ID], PLAYER_WEAPONS[playerid][weapon_slot][player_weapon_AMMO]);
	MySQL_TQueryInline(srp_db, using inline OnPweaponInserted, QUERY_BUFFER);
	QUERY_BUFFER[0] = EOS;
	return 1;
}

SavePlayerWeaponsData(playerid)
{
	if(!PLAYER_TEMP[playerid][pt_USER_EXIST] || !PLAYER_TEMP[playerid][pt_USER_LOGGED]) return 0;
	
	for(new i = 0; i != sizeof PLAYER_WEAPONS[]; i ++)
	{
		if(!PLAYER_WEAPONS[playerid][i][player_weapon_DB_ID]) continue;
		
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pweapons SET weaponid = %d, ammo = %d WHERE id = %d;", PLAYER_WEAPONS[playerid][i][player_weapon_ID], PLAYER_WEAPONS[playerid][i][player_weapon_AMMO], PLAYER_WEAPONS[playerid][i][player_weapon_DB_ID]);
		mysql_tquery(srp_db, QUERY_BUFFER);
	}
	return 1;
}



LoadPlayerWeaponsData(playerid)
{
	if(ACCOUNT_INFO[playerid][ac_ID] == 0) return 0;
	
	inline OnPweaponsLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			for(new i = 0; i != rows; i ++)
			{
				new weaponid;
				cache_get_value_name_int(i, "weaponid", weaponid);

				PLAYER_WEAPONS[playerid][ WEAPON_INFO[weaponid][weapon_info_SLOT] ][player_weapon_VALID] = true;
				cache_get_value_name_int(i, "id", PLAYER_WEAPONS[playerid][ WEAPON_INFO[weaponid][weapon_info_SLOT] ][player_weapon_DB_ID]);
				cache_get_value_name_int(i, "ammo", PLAYER_WEAPONS[playerid][ WEAPON_INFO[weaponid][weapon_info_SLOT] ][player_weapon_AMMO]);
				PLAYER_WEAPONS[playerid][ WEAPON_INFO[weaponid][weapon_info_SLOT] ][player_weapon_ID] = weaponid;
			}
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM pweapons WHERE id_player = %d LIMIT 13;", ACCOUNT_INFO[playerid][ac_ID]);
	MySQL_TQueryInline(srp_db, using inline OnPweaponsLoad, QUERY_BUFFER);
	return 1;
}

GetVehicleModelByName(const vehname[]) {
	new modelid = -1;
	for(new i = 0; i < sizeof VEHICLE_INFO; i ++) {
		if(strfind(VEHICLE_INFO[i][vehicle_info_NAME], vehname, true) != -1) {
			modelid = i + 400;
			break;
		}
	}
	return modelid;
}



//	CMD:gotopos( playerid, params[] )
//	{
//		new Float:g_x,Float:g_y,Float:g_z,int_id,vw_id;
//		if ( sscanf ( params, "p<,>fffdd", g_x, g_y, g_z, int_id,vw_id) ) return 1;
//		SetPlayerPosEx(playerid, g_x, g_y, g_z,0.0,int_id,vw_id);
//		return 1;
//	}
//	






