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

#if defined callbacks_included 
	#endinput
#endif
#define callbacks_included



public OnPlayerConnect(playerid)
{
	if(CheckPlayerFakeClient(playerid)) Kick(playerid);

	PLAYER_TEMP[playerid][pt_GAME_STATE] = GAME_STATE_CONNECTED;
	PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] = -1;
	PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] = -1;
	PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] = -1;
	PLAYER_TEMP[playerid][pt_HUD_TEXTDRAWS] = false;
	PLAYER_TEMP[playerid][pt_SEE_ACMD_LOG] = true;
	PLAYER_TEMP[playerid][pt_SEE_AC_LOG] = true;
	PLAYER_TEMP[playerid][pt_SEE_ADM_LOG] = true;
	PLAYER_TEMP[playerid][pt_SEE_ADM_CHAT] = true;
	PLAYER_TEMP[playerid][pt_REGISTER_ACTOR] = INVALID_ACTOR_ID;
	PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID] = INVALID_PLAYER_ID;
	PLAYER_TEMP[playerid][pt_KNOCK_PLAYER_ID] = INVALID_PLAYER_ID;
	PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT] = INVALID_STREAMER_ID;
	PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT] = INVALID_STREAMER_ID;
	PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT] = INVALID_STREAMER_ID;
	PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT] = INVALID_STREAMER_ID;
	PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT] = INVALID_STREAMER_ID;
	PLAYER_TEMP[playerid][pt_PIZZA_ACTOR] = INVALID_STREAMER_ID;
	PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] = INVALID_PLAYER_ID;
	PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] = INVALID_VEHICLE_ID;
	PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] = INVALID_VEHICLE_ID;
	PLAYER_TEMP[playerid][pt_PIVOT_OBJECT] = INVALID_OBJECT_ID;
	PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] = -1;
	PLAYER_TEMP[playerid][pt_CREW_HELP] = false;
	PLAYER_TEMP[playerid][pt_COMBAT_TIMER] = -1;
	PLAYER_TEMP[playerid][pt_COMBAT] = false;
	PLAYER_TEMP[playerid][pt_DIALOG_ID] = -1;
	PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER] = -1;
	for(new i = 0; i != MAX_OBJECTS_PER_ROUTE; i ++) TRASH_PLAYER_OBJECTS[playerid][i] = INVALID_STREAMER_ID;
	
	GetPlayerName(playerid, PLAYER_TEMP[playerid][pt_NAME], 24);
	GetPlayerIp(playerid, PLAYER_TEMP[playerid][pt_IP], 16);
	
	EnablePlayerCameraTarget(playerid, true);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	TogglePlayerControllableEx(playerid, true);
	HidePlayerDialog(playerid);
	ResetPlayerMoney(playerid);
	CancelSelectTextDrawEx(playerid);
	SetPlayerScore(playerid, 0);
	ResetPlayerWeapons(playerid);
	SetPlayerColorEx(playerid, PLAYER_COLOR);
	CancelEdit(playerid);
	

	//
	if(!IsValidRPName(PLAYER_TEMP[playerid][pt_NAME]))
	{
		PLAYER_TEMP[playerid][pt_USER_VALID_NAME] = false;
		ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, "Aviso", "Bienvenido, has entrado a un servidor de rol.\n\nTu nombre no cumple con el formato necesario.\nPor favor utilice el formato: Nombre_Apellido.", "Entiendo", "");
		KickEx(playerid, 500);
	}
	else
	{
		PLAYER_TEMP[playerid][pt_USER_VALID_NAME] = true;
		SetRolePlayNames(playerid);

		if(GetNumberOfPlayersOnThisIP(PLAYER_TEMP[playerid][pt_IP]) > 3)
		{
			ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, "Aviso", "Ha sido expulsado por exceder el máximo de conexiones desde una IP.", "Entiendo", "");
			KickEx(playerid, 500);
		}
		else
		{
			PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER] = SetTimerEx("KickPlayer", SECURE_LOGIN_TIME, false, "i", playerid);

			GameTextForPlayer(playerid, "~y~cargando...", 10000, 3);
			CreatePlayerTextDraws(playerid);
			SetPiDefaultValues(playerid);
		}
	}
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	for(new i = 0; i != MAX_TIMERS_PER_PLAYER; i++) KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][i]);

	if(PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER] != -1) {
		KillTimer(PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER]);
		PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER] = -1;
	}

	if(PLAYER_TEMP[playerid][pt_COMBAT])
	{
		new 
			str_text[190];

		PLAYER_TEMP[playerid][pt_COMBAT] = false;
		KillTimer(PLAYER_TEMP[playerid][pt_COMBAT_TIMER]);
		PLAYER_TEMP[playerid][pt_COMBAT_TIMER] = -1;

		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM pweapons WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
		mysql_tquery(srp_db, QUERY_BUFFER);

		ResetPlayerWeaponsEx(playerid);

		format(str_text, 190, "* %s ha perdido todas sus armas por desconectar estando en combate.", PLAYER_TEMP[playerid][pt_RP_NAME]);
		ProxDetector(playerid, 25.0, str_text, 0xDD4C4FFF, 0xDD4C4FFF, 0xDD4C4FFF, 0xDD4C4FFF, 0xDD4C4FFF, 85);
	}

	if(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR] != INVALID_ACTOR_ID)
	{
		DestroyActor(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR]);
		PLAYER_TEMP[playerid][pt_REGISTER_ACTOR] = INVALID_ACTOR_ID;
	}

	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		if(vehicleid)
		{
			GLOBAL_VEHICLES[ vehicleid ][gb_vehicle_TP_IMMUNITY] = gettime() + 5;
			GLOBAL_VEHICLES[ vehicleid ][gb_vehicle_DRIVER] = INVALID_PLAYER_ID;
			GLOBAL_VEHICLES[ vehicleid ][gb_vehicle_LAST_DRIVER] = INVALID_PLAYER_ID;
			GLOBAL_VEHICLES[ vehicleid ][gb_vehicle_OCCUPIED] = false;
		}
	}
	if(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] != INVALID_VEHICLE_ID)
	{
		if(GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_LAST_DRIVER] == playerid) GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_LAST_DRIVER] = INVALID_PLAYER_ID;
	}

	PLAYER_TEMP[playerid][pt_TIME_PASSED_LAST_REP] = gettime() * 1000 - PLAYER_TEMP[playerid][pt_TIME_PASSED_LAST_REP];
	ACCOUNT_INFO[playerid][ac_TIME_FOR_REP] -= PLAYER_TEMP[playerid][pt_TIME_PASSED_LAST_REP];
	if(PI[playerid][ac_CREW]) CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_ONLINE_MEMBERS] --;
	if(PLAYER_TEMP[playerid][pt_PIVOT_OBJECT] != INVALID_OBJECT_ID) DestroyPlayerObject(playerid, PLAYER_TEMP[playerid][pt_PIVOT_OBJECT]);
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL) PI[playerid][ac_POLICE_JAIL_TIME] -= gettime() - PLAYER_TEMP[playerid][pt_ENTER_JAIL_TIME];
	if(PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL]) EndPhoneCall(playerid);

	DestroyPlayerCheckpoints(playerid);
	DestroyPlayerTextDraws(playerid);
	CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], false);

	//db
	if(ACCOUNT_INFO[playerid][ac_ID] != 0)
	{
		if(PLAYER_TEMP[playerid][pt_USER_LOGGED]) //guardar datos solo si ha logeado
		{
			if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK && PI[playerid][ac_WANTED_LEVEL] > 0) {
				ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_HOSPITAL;
			}

			PI[playerid][ac_CONNECTED] = 0;
			PI[playerid][ac_PLAYERID] = 0;
			UpdatePlayerWorldInfo(playerid);

			getDateTime(ACCOUNT_INFO[playerid][ac_LAST_CONNECTION]);
			SavePlayerData(playerid);
			SavePlayerVehicles(playerid, true);
			SavePlayerToys(playerid);
			SavePlayerWeaponsData(playerid);
			SavePlayerWorkLevels(playerid);
			
			//msg
			new disconnect_message[128];
			switch(reason)
			{
				case 0: format(disconnect_message, sizeof disconnect_message, "%s se ha desconectado por error (crash).", PLAYER_TEMP[playerid][pt_RP_NAME]);
				case 1: format(disconnect_message, sizeof disconnect_message, "%s se ha desconectado.", PLAYER_TEMP[playerid][pt_RP_NAME]);
				case 2: format(disconnect_message, sizeof disconnect_message, "%s se ha desconectado porque fue expulsado del servidor.", PLAYER_TEMP[playerid][pt_RP_NAME]);
			}
			NearbyMessage(PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z], PI[playerid][ac_INTERIOR], GetPlayerVirtualWorld(playerid), 15.0, 0x909D95FF, disconnect_message);
		}
		else {
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET connected = 0, playerid = 0 WHERE id = %d;", ACCOUNT_INFO[playerid][ac_ID]);
			mysql_tquery(srp_db, QUERY_BUFFER);
		}
	}

	//reset
	ResetPlayerVariables(playerid);
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	if(!success) BlockIpAddress(ip, 12000);
    return 1;
}

public OnPlayerSpawn(playerid)
{
	if(!PLAYER_TEMP[playerid][pt_USER_LOGGED]) return Kick(playerid);

	if(PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_CONNECTED) // Primer spawn
	{
		if(PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER] != -1) {
			KillTimer(PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER]);
			PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER] = -1;
		}

		if(PI[playerid][ac_CONFIG_SECURE_LOGIN]) {
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM player_secure_login WHERE TIMESTAMPDIFF(DAY, last_connection, NOW()) > "#SECURE_LOGIN_IP_DAYS" AND id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
			mysql_tquery(srp_db, QUERY_BUFFER);
		}
		else {
			SendNotification(playerid, "Activa la verificación en dos pasos para poder proteger tu cuenta, utiliza /ayuda verificacion para más información.");
		}

		new now = gettime();
		PLAYER_TEMP[playerid][pt_LAST_PLANT_TIME] = now;
		PLAYER_TEMP[playerid][pt_COOLDOWN_MEDICAL_KIT] = now + 300;
		format(ACCOUNT_INFO[playerid][ac_IP], 16, "%s", PLAYER_TEMP[playerid][pt_IP]);
		
		if(PI[playerid][ac_CREW]) SetPlayerGangZones(playerid);
		SetPlayerSkillLevels(playerid);
		ApplyAnimation(playerid,"SWAT","null",0.0,0,0,0,0,0);
		ApplyAnimation(playerid,"MEDIC","null",0.0,0,0,0,0,0);
		ApplyAnimation(playerid,"SHOP","null",0.0,0,0,0,0,0);
		ApplyAnimation(playerid,"BOMBER","null",0.0,0,0,0,0,0);
		ApplyAnimation(playerid,"FOOD","null",0.0,0,0,0,0,0);
		ApplyAnimation(playerid,"CHAINSAW","null",0.0,0,0,0,0,0);
		ApplyAnimation(playerid,"ROB_BANK","null",0.0,0,0,0,0,0);
		ApplyAnimation(playerid,"CARRY","null",0.0,0,0,0,0,0);
		ApplyAnimation(playerid,"PED","null",0.0,0,0,0,0,0);
		ApplyAnimation(playerid,"SWEET","null",0.0,0,0,0,0,0);
		ApplyAnimation(playerid,"CRACK","null",0.0,0,0,0,0,0);
		ApplyAnimation(playerid,"POLICE","null",0.0,0,0,0,0,0);

		if(PI[playerid][ac_POLICE_DUTY] != 0)
		{
			if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_POLICE, INVALID_VEHICLE_ID);
			else PI[playerid][ac_POLICE_DUTY] = 0;
		}
		switch(ACCOUNT_INFO[playerid][ac_STATE])
		{
			case ROLEPLAY_STATE_NORMAL:
			{
				SetPlayerHud(playerid);
				SetCameraBehindPlayer(playerid);
				TogglePlayerControllableEx(playerid, false);
				KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][3]);
				PLAYER_TEMP[playerid][pt_TIMERS][3] = SetTimerEx("TogglePlayerControl", 2000, false, "ib", playerid, true);

				SetPlayerVirtualWorld(playerid, 0);

				if(PI[playerid][ac_WANTED_LEVEL] > 0)
				{
					PI[playerid][ac_POLICE_JAIL_ID] = 0;
					JailPlayer(playerid);
				}
			}
			case ROLEPLAY_STATE_JAIL:
			{
				ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_JAIL;
				if(PI[playerid][ac_POLICE_JAIL_TIME] < 5) PI[playerid][ac_POLICE_JAIL_TIME] = 5;
				KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][15]);
				PLAYER_TEMP[playerid][pt_TIMERS][15] = SetTimerEx("UnjailPlayer", PI[playerid][ac_POLICE_JAIL_TIME] * 1000, false, "i", playerid);
				
				PLAYER_TEMP[playerid][pt_ENTER_JAIL_TIME] = gettime();
				SetPlayerPosEx(playerid, JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID] ][jail_X], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_Y], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_Z], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_ANGLE], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_INTERIOR], 0, true);
				
				new time = PI[playerid][ac_POLICE_JAIL_TIME] - (gettime() - PLAYER_TEMP[playerid][pt_ENTER_JAIL_TIME]);
				SendFormatNotification(playerid, "Te quedan %s minutos de condena.", TimeConvert(time));
				
				SetPlayerHud(playerid);
				SetCameraBehindPlayer(playerid);
				TogglePlayerControllableEx(playerid, false);
				KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][3]);
				PLAYER_TEMP[playerid][pt_TIMERS][3] = SetTimerEx("TogglePlayerControl", 2000, false, "ib", playerid, true);
			}
			case ROLEPLAY_STATE_ARRESTED:
			{
				if(ACCOUNT_INFO[playerid][ac_VIP]) PI[playerid][ac_POLICE_JAIL_TIME] = 150 * PI[playerid][ac_WANTED_LEVEL];
				else PI[playerid][ac_POLICE_JAIL_TIME] = 300 * PI[playerid][ac_WANTED_LEVEL];
				
				PI[playerid][ac_WANTED_LEVEL] = 0;
				
				ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_JAIL;
				if(PI[playerid][ac_POLICE_JAIL_TIME] < 5) PI[playerid][ac_POLICE_JAIL_TIME] = 5;
				KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][15]);
				PLAYER_TEMP[playerid][pt_TIMERS][15] = SetTimerEx("UnjailPlayer", PI[playerid][ac_POLICE_JAIL_TIME] * 1000, false, "i", playerid);
				
				PLAYER_TEMP[playerid][pt_ENTER_JAIL_TIME] = gettime();
				SetPlayerPosEx(playerid, JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID] ][jail_X], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_Y], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_Z], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_ANGLE], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_INTERIOR], 0, true);
				
				new time = PI[playerid][ac_POLICE_JAIL_TIME] - (gettime() - PLAYER_TEMP[playerid][pt_ENTER_JAIL_TIME]);
				SendFormatNotification(playerid, "Te quedan %s minutos de condena.", TimeConvert(time));
				
				SetPlayerHud(playerid);
				SetCameraBehindPlayer(playerid);
				TogglePlayerControllableEx(playerid, false);
				KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][3]);
				PLAYER_TEMP[playerid][pt_TIMERS][3] = SetTimerEx("TogglePlayerControl", 2000, false, "ib", playerid, true);
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
					
					SetPlayerPosEx(playerid, PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z], PI[playerid][ac_ANGLE], PI[playerid][ac_INTERIOR], 0);
				}
				else
				{
					if(PROPERTY_INFO[index][property_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
					{
						SetPlayerInterior(playerid, PROPERTY_INTERIORS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_INT_INTERIOR]);
						SetPlayerVirtualWorld(playerid, PROPERTY_INFO[index][property_ID]);
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
						
						SetPlayerPosEx(playerid, PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z], PI[playerid][ac_ANGLE], PI[playerid][ac_INTERIOR], 0);
					}
				}
					
				SetPlayerHud(playerid);
				SetCameraBehindPlayer(playerid);
				TogglePlayerControllableEx(playerid, false);
				KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][3]);
				PLAYER_TEMP[playerid][pt_TIMERS][3] = SetTimerEx("TogglePlayerControl", 2000, false, "ib", playerid, true);
			}
			case ROLEPLAY_STATE_HOSPITAL:
			{
				PLAYER_TEMP[playerid][pt_HOSPITAL] = GetNearestHospitalForPlayer(playerid);
				PLAYER_TEMP[playerid][pt_PLAYER_FINISH_HOSPITAL] = false;
				PLAYER_TEMP[playerid][pt_GAME_STATE] = GAME_STATE_DEAD;
				
				TogglePlayerControllableEx(playerid, true);
				TogglePlayerSpectatingEx(playerid, true);
				SetPlayerInterior(playerid, 0);
				PI[playerid][ac_LOCAL_INTERIOR] = 0;
				PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] = -1;
				PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] = -1;
				
				PLAYER_TEMP[playerid][pt_HOSPITAL_LIFE] = 35.0;
				UpdateHospitalSizeTextdrawLife(playerid);
				KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][4]);
				PLAYER_TEMP[playerid][pt_TIMERS][4] = SetTimerEx("HealthUp", 1000, false, "i", playerid);

				DeleteIlegalInv(playerid);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2]);
								
				//ClearPlayerChat(playerid);
				if(PI[playerid][ac_WANTED_LEVEL] > 0)
				{
					if(ACCOUNT_INFO[playerid][ac_VIP]) PI[playerid][ac_POLICE_JAIL_TIME] = 150 * PI[playerid][ac_WANTED_LEVEL];
					else PI[playerid][ac_POLICE_JAIL_TIME] = 300 * PI[playerid][ac_WANTED_LEVEL];
					
					SendNotification(playerid, "Fuiste ingresado en ~r~el centro médico más cercano~w~.~n~~n~Espera mientras te recuperas.");
				}
				else
				{
					SendNotification(playerid, "Fuiste ingresado en ~r~el centro médico más cercano~w~.~n~~n~Espera mientras te recuperas.");
				}

				InterpolateCameraPos(playerid, Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][0], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][1], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][2], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][0], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][1], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][2], 1000, CAMERA_CUT);
				InterpolateCameraLookAt(playerid, Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][3], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][4], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][5], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][3], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][4], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][5], 1000, CAMERA_CUT);
			}
		}
		
		for(new i = 0; i < LOAD_SCREEN_TEXTDRAWS_SIZE; i ++) {
			TextDrawHideForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][i]);
		}

		new neccessary_rep = ACCOUNT_INFO[playerid][ac_LEVEL] * REP_MULTIPLIER;
		if(ACCOUNT_INFO[playerid][ac_REP] < neccessary_rep)
		{
			if(ACCOUNT_INFO[playerid][ac_TIME_FOR_REP] > TIME_FOR_REP) ACCOUNT_INFO[playerid][ac_TIME_FOR_REP] = TIME_FOR_REP;
			if(ACCOUNT_INFO[playerid][ac_TIME_FOR_REP] <= 900) ACCOUNT_INFO[playerid][ac_TIME_FOR_REP] = 3000;
			
			PLAYER_TEMP[playerid][pt_TIME_PASSED_LAST_REP] = gettime() * 1000;
			PLAYER_TEMP[playerid][pt_TIMERS][2] = SetTimerEx("AddPlayerReputation", ACCOUNT_INFO[playerid][ac_TIME_FOR_REP], false, "i", playerid);
		}
		else SendFormatNotification(playerid, "Escribe ~g~/comprarnivel ~w~para subir al nivel ~g~%d ~w~por ~g~%d dólares~w~.", ACCOUNT_INFO[playerid][ac_LEVEL] + 1, ACCOUNT_INFO[playerid][ac_LEVEL] * 150); //
		
		if(PI[playerid][ac_PHONE_NUMBER])
		{
			inline OnOfflineMessagesChecked()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					if(rows)
					{
						new unreaded_messages;
						cache_get_value_index_int(0, 0, unreaded_messages);
						if(unreaded_messages > 0) SendClientMessageEx(playerid, -1, "Tienes {f1f442}%d {FFFFFF}mensajes nuevos en tu /movil desde tu última conexión.", unreaded_messages);
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pmessages SET offline = 0 WHERE to_id = %d;", ACCOUNT_INFO[playerid][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM pmessages WHERE to_id = %d AND offline = 1;", ACCOUNT_INFO[playerid][ac_ID]);
			MySQL_TQueryInline(srp_db, using inline OnOfflineMessagesChecked, QUERY_BUFFER);

		}
	}
	else if(PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_DEAD) // Viene de morir
	{
		if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_HOSPITAL)
		{
			if(!PLAYER_TEMP[playerid][pt_PLAYER_FINISH_HOSPITAL])
			{
				TogglePlayerSpectatingEx(playerid, true);
				SetPlayerInterior(playerid, 0);
				SetPlayerVirtualWorld(playerid, 0);
				PLAYER_TEMP[playerid][pt_PLAYER_FINISH_HOSPITAL] = false;
				ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_HOSPITAL;
				PI[playerid][ac_LOCAL_INTERIOR] = 0;
				if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] != -1) {
					CallLocalFunction("OnPlayerExitInterior", "ii", playerid, PLAYER_TEMP[playerid][pt_INTERIOR_INDEX]);
				}
				PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] = -1;
				PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] = -1;
				
				PLAYER_TEMP[playerid][pt_HOSPITAL_LIFE] = 35.0;
				UpdateHospitalSizeTextdrawLife(playerid);
				KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][4]);
				PLAYER_TEMP[playerid][pt_TIMERS][4] = SetTimerEx("HealthUp", 1000, false, "i", playerid);

				DeleteIlegalInv(playerid);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2]);
				
				ClearPlayerChat(playerid);
				if(PI[playerid][ac_WANTED_LEVEL] > 0 || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED)
				{
					SendNotification(playerid, "Fuiste ingresado en el centro médico más cercano.");
					SendNotification(playerid, "Cuando te recuperes serás llevado a la cárcel.");
				}
				else
				{
					SendNotification(playerid, "Fuiste ingresado en el centro médico más cercano.");
					SendNotification(playerid, "Espera mientras te recuperas.");
				}
				
				InterpolateCameraPos(playerid, Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][0], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][1], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][2], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][0], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][1], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][2], 1000, CAMERA_CUT);
				InterpolateCameraLookAt(playerid, Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][3], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][4], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][5], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][3], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][4], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][5], 1000, CAMERA_CUT);
			}
		}
	}

	if(PLAYER_TEMP[playerid][pt_PLAYER_FINISH_HOSPITAL])
	{
		if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL)
		{
			KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][15]);
			PLAYER_TEMP[playerid][pt_TIMERS][15] = SetTimerEx("UnjailPlayer", PI[playerid][ac_POLICE_JAIL_TIME] * 1000, false, "i", playerid);
		}
		SetPlayerInterior(playerid, PI[playerid][ac_INTERIOR]);
		SetPlayerVirtualWorld(playerid, 0);
		SetPlayerHud(playerid);
		TogglePlayerControllableEx(playerid, false);
		PLAYER_TEMP[playerid][pt_PLAYER_FINISH_HOSPITAL] = false;
		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][3]);
		SetCameraBehindPlayer(playerid);
		PLAYER_TEMP[playerid][pt_TIMERS][3] = SetTimerEx("TogglePlayerControl", 2000, false, "ib", playerid, true);
	}
	
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) SetPlayerSkin(playerid, PI[playerid][ac_SKIN]);
	else SetPlayerSkin(playerid, PI[playerid][ac_POLICE_DUTY]);

	PLAYER_TEMP[playerid][pt_GAME_STATE] = GAME_STATE_NORMAL;
	SetPlayerToys(playerid);
	if(ACCOUNT_INFO[playerid][ac_STATE] != ROLEPLAY_STATE_JAIL && ACCOUNT_INFO[playerid][ac_STATE] != ROLEPLAY_STATE_CRACK) SetWeaponsForPlayer(playerid);
	if(ACCOUNT_INFO[playerid][ac_STATE] != ROLEPLAY_STATE_HOSPITAL) SetPlayerWantedLevelEx(playerid, PI[playerid][ac_WANTED_LEVEL]);
	SetPlayerArmedWeapon(playerid, 0);
	SetPlayerNormalColor(playerid);
	SetPlayerCityWeather(playerid);
	GameTextForPlayer(playerid, "_", 500, 3);
	PLAYER_TEMP[playerid][pt_CONTROL] = false;
	
	PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_PLAYER_HEALTH][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_PLAYER_ARMOUR][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_UNOCCUPIED_VEHICLE_TP][p_ac_info_IMMUNITY] = gettime() + 5;
	return 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
	if(PLAYER_TEMP[playerid][pt_KICKED] || !PLAYER_TEMP[playerid][pt_USER_LOGGED]) return 0;

	if(PLAYER_TEMP[playerid][pt_COMBAT]) ResetCombat(playerid);
	
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][16]);
	PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_PLAYER_HEALTH][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_IMMUNITY] = gettime() + 15;
	if(!ACCOUNT_INFO[playerid][ac_VIP]) SetPlayerArmourEx(playerid, 0.0);
	
	if(ac_Info[CHEAT_DEATH_SPAMMER][ac_Enabled])
	{
		if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_DEATH_SPAMMER][p_ac_info_IMMUNITY])
		{
			new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_DEATH]);
			if(interval < 50)
			{
				if(!ac_Info[CHEAT_DEATH_SPAMMER][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_DEATH_SPAMMER, float(interval));
				else
				{
					if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_DEATH_SPAMMER][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_DEATH_SPAMMER][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_DEATH_SPAMMER][p_ac_info_DETECTIONS] = 0;
					else PLAYER_AC_INFO[playerid][CHEAT_DEATH_SPAMMER][p_ac_info_DETECTIONS] ++;
					
					PLAYER_AC_INFO[playerid][CHEAT_DEATH_SPAMMER][p_ac_info_LAST_DETECTION] = gettime();
					if(PLAYER_AC_INFO[playerid][CHEAT_DEATH_SPAMMER][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_DEATH_SPAMMER][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_DEATH_SPAMMER, float(interval));
				}
			}
			PLAYER_TEMP[playerid][pt_ANTIFLOOD_DEATH] = GetTickCount();
		}
	}
	
	HidePlayerMessage(playerid);
	
	if(IsPlayerConnected(killerid) && ACCOUNT_INFO[killerid][ac_STATE] == ROLEPLAY_STATE_NORMAL && ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_NORMAL)
	{
		if(!PLAYER_WORKS[killerid][WORK_POLICE][pwork_SET])
		{
			if(IsPlayerInSafeZone(killerid))
			{
				SendClientMessage(killerid, -1, "Estás en una zona segura, la policía ha sido avisada del homicidio.");
				
				new level = PI[killerid][ac_WANTED_LEVEL] + 3;
				if(level > 6) level = 6;

				SetPlayerWantedLevelEx(killerid, level);
				
				new city[45], zone[45];
				GetPlayerZones(killerid, city, zone);

				new message[145];
				format(message, sizeof message, "{"#PRIMARY_COLOR"}[Central policía] {FFFFFF}%s ha causado un asesinato en {"#PRIMARY_COLOR"}%s, %s.", PLAYER_TEMP[killerid][pt_RP_NAME], city, zone);
				SendPoliceRadioMessage(-1, -1, message);
				
				PLAYER_TEMP[killerid][pt_LAST_SAFE_ZONE_WARNING] = gettime();
			}
		}
	}
	
	if(PLAYER_TEMP[playerid][pt_SELECT_TEXTDRAW])
	{
		if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] != -1) ClosePlayerClothingMenu(playerid);
		if(PLAYER_TEMP[playerid][pt_TOYS_SHOP]) ClosePlayerToysMenu(playerid);
		if(PLAYER_TEMP[playerid][pt_SHOP]) HidePlayerShop(playerid);
	}

	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][0]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_WANTED_LEVEL][1]);
	
	StopAudioStreamForPlayer(playerid);
	SetPlayerDrunkLevel(playerid, 0);
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][3]);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	GetPlayerPos(playerid, PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z]);
	GetPlayerFacingAngle(playerid, PI[playerid][ac_ANGLE]);
	PI[playerid][ac_INTERIOR] = GetPlayerInterior(playerid);
	HidePlayerHud(playerid);
	CancelEdit(playerid);
	HidePlayerDialog(playerid);
	SetNormalPlayerMarkers(playerid);
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], false);
	PLAYER_TEMP[playerid][pt_HUNGRY_MESSAGE] = false;
	PLAYER_TEMP[playerid][pt_THIRST_MESSAGE] = false;
	PLAYER_TEMP[playerid][pt_PLAYER_IN_ATM] = false;
	PLAYER_TEMP[playerid][pt_CUFFED] = false;
	PLAYER_TEMP[playerid][pt_CUFFING] = false;
	PLAYER_TEMP[playerid][pt_PLAYER_WAITING_MP3_HTTP] = false;
	if(PLAYER_TEMP[playerid][pt_WANT_TAXI])
	{
		PLAYER_TEMP[playerid][pt_WANT_TAXI] = false;
		DisablePlayerTaxiMark(playerid);
	}
	if(PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL]) EndPhoneCall(playerid);
	if(PLAYER_TEMP[playerid][pt_GPS_MAP]) HidePlayerGpsMap(playerid);
	if(PI[playerid][ac_WANTED_LEVEL] > 0) DisablePlayerPoliceMark(playerid);
	if(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_SHOP]) CancelPlayerTuningShop(playerid);
	if(PLAYER_TEMP[playerid][pt_IN_TUNING_GARAGE])
	{
		PLAYER_TEMP[playerid][pt_IN_TUNING_GARAGE] = false;
		SetVehicleVirtualWorldEx(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], 0);
		SetVehiclePosEx(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], 246.551910, 17.474195, 2.363841);
		SetVehicleZAngle(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], 0.0);
	}
	if(PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] != INVALID_VEHICLE_ID)
	{
		if(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_JOB_STARTED])
		{
			if(PLAYER_TEMP[playerid][pt_TRASH_DRIVER])
			{
				SendClientMessage(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], -1, "{"#SILVER_COLOR"}El trabajo se ha cancelado porque tu compañero ha dejado de trabajar.");
				CancelTrashWork(playerid, TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
			}
			if(PLAYER_TEMP[playerid][pt_TRASH_PASSENGER])
			{
				SendClientMessage(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], -1, "{"#SILVER_COLOR"}El trabajo se ha cancelado porque tu compañero ha dejado de trabajar.");
				CancelTrashWork(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], playerid, PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
			}
		}
	}
	
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL)
	{
		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][15]);
		PLAYER_TEMP[playerid][pt_PLAYER_FINISH_HOSPITAL] = true;
		PI[playerid][ac_POLICE_JAIL_TIME] -= gettime() - PLAYER_TEMP[playerid][pt_ENTER_JAIL_TIME];
		if(PI[playerid][ac_POLICE_JAIL_TIME] < 5) PI[playerid][ac_POLICE_JAIL_TIME] = 5;
		PLAYER_TEMP[playerid][pt_ENTER_JAIL_TIME] = gettime();
		SetSpawnInfo(playerid, NO_TEAM, PI[playerid][ac_SKIN], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID] ][jail_X], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_Y], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_Z], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_ANGLE], 0, 0, 0, 0, 0, 0);
		PI[playerid][ac_INTERIOR] = JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_INTERIOR];
		SetPlayerVirtualWorld(playerid, 0);
	}
	else
	{
		new bool:wanted_level_go_to_hospital = false;
		/*if(PI[playerid][ac_WANTED_LEVEL] > 0 && CountPolicesOnDuty() < 2) {
			wanted_level_go_to_hospital = true;
		}*/
		
		if(wanted_level_go_to_hospital || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || GetPlayerInterior(playerid) != 0 || GetPlayerVirtualWorld(playerid) != 0)
		{
			DisablePlayerInjuredMark(playerid);
			PLAYER_TEMP[playerid][pt_HOSPITAL] = GetNearestHospitalForPlayer(playerid);
			ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_HOSPITAL;
			PLAYER_TEMP[playerid][pt_PLAYER_FINISH_HOSPITAL] = false;
			KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][16]);
			SetSpawnInfo(playerid, NO_TEAM, PI[playerid][ac_SKIN], PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z], PI[playerid][ac_ANGLE], 0, 0, 0, 0, 0, 0);
		}
		else
		{
			PLAYER_TEMP[playerid][pt_DEATH_TIME] = gettime();
			ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_CRACK;
			
			new Float:pos[3], Float:angle;
			GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
			GetPlayerFacingAngle(playerid, angle);
			
			SetSpawnInfo(playerid, NO_TEAM, PI[playerid][ac_SKIN], pos[0], pos[1], pos[2], angle, 0, 0, 0, 0, 0, 0);
		}
	}
	
	PLAYER_TEMP[playerid][pt_GAME_STATE] = GAME_STATE_DEAD;
   	return 1;
}

forward OnSecureLoginRequestCode(index, response_code, data[]);
public OnSecureLoginRequestCode(index, response_code, data[]) {
	if(response_code == 200) {
		GameTextForPlayer(index, "_", 500, 3);
		PLAYER_TEMP[index][pt_BAD_LOGIN_ATTEMP] = 0;
		ShowDialog(index, DIALOG_SECURE_LOGIN_INSERT_CODE);
	}
	else Kick(index);
}

forward OnSecureLoginCheckCode(index, response_code, data[]);
public OnSecureLoginCheckCode(index, response_code, data[]) {
	if(response_code == 200) {
		PLAYER_TEMP[index][pt_BAD_LOGIN_ATTEMP] = 0;
		
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO player_secure_login (ip, id_player) VALUES ('%e', %d);", PLAYER_TEMP[index][pt_IP], ACCOUNT_INFO[index][ac_ID]);
		mysql_tquery(srp_db, QUERY_BUFFER);

		SendNotification(index, "Se ha confirmado el inicio de sesión.");
		LoginPlayer(index);
	}
	else {
		PLAYER_TEMP[index][pt_BAD_LOGIN_ATTEMP] ++;
		if(PLAYER_TEMP[index][pt_BAD_LOGIN_ATTEMP] > MAX_BAD_LOGIN_ATTEMPS) return Kick(index);
		ShowDialog(index, DIALOG_SECURE_LOGIN_INSERT_CODE);
	}
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	if(PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_CONNECTED) // Recién conectado
	{
		PlayAudioStreamForPlayer(playerid, Intro_Audios[random(sizeof Intro_Audios)]);
		
		if(!PLAYER_TEMP[playerid][pt_USER_VALID_NAME] || PLAYER_TEMP[playerid][pt_KICKED] || PLAYER_TEMP[playerid][pt_CLASSED]) Kick(playerid);
		else
		{
			PLAYER_TEMP[playerid][pt_CLASSED] = true;
			TogglePlayerSpectatingEx(playerid, true);
			ClearPlayerChat(playerid);

			//check ban
			inline OnPlayerBannedCheck()
			{
				new rows, bool:continue_proccess;
				if(cache_get_row_count(rows))
				{
					if(rows)
					{
						new name[24], expire_date[24], id_player, by, text[128], date[24], expire_date_ts, now[24], now_ts, bool:isnull_name, bool:isnull_by, bool:isnull_expire_date_ts;
						cache_is_value_name_null(0, "name", isnull_name);
						if(!isnull_name) cache_get_value_name(0, "name", name, 24);
						cache_get_value_name(0, "expire_date", expire_date, 24);
						cache_get_value_name_int(0, "id_player", id_player);
						cache_is_value_name_null(0, "by", isnull_by);
						if(!isnull_by) cache_get_value_name_int(0, "by", by);
						cache_get_value_name(0, "text", text, 128);
						cache_get_value_name(0, "date", date, 24);
						cache_is_value_name_null(0, "expire_date_ts", isnull_expire_date_ts);
						if(!isnull_expire_date_ts) cache_get_value_name_int(0, "expire_date_ts", expire_date_ts);
						cache_get_value_name(0, "now", now, 24);
						cache_get_value_name_int(0, "now_ts", now_ts);

						if(expire_date_ts)
						{
							//temporal
							if(now_ts >= expire_date_ts)
							{
								continue_proccess = true;
								mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM bans WHERE name = '%e' OR ip = '%e';", PLAYER_TEMP[playerid][pt_NAME], PLAYER_TEMP[playerid][pt_IP]);
								mysql_tquery(srp_db, QUERY_BUFFER);

								if(id_player) AddPlayerBadHistory(id_player, -1, TYPE_UNBAN, "Readmitido (de tban)");
								SendNotification(playerid, "Tu cuenta ha sido re-admitida, no hagas que vuelvan a banearte.");
							}
							else
							{
								new dialog[600];
								format(dialog, sizeof dialog, 
									
									"\
										Esta IP o cuenta está expulsada temporalmente, detalles:\n\
										\n\
										Tu nombre: %s\n\
										\n\
										Nombre: %s\n\
										Expulsado por: %d\n\
										Fecha expulsión: %s\n\
										Razón: %s\n\
										\n\
										Fecha actual: %s\n\
										Fecha readmisión: %s\n\
										\n\
									",
									
									PLAYER_TEMP[playerid][pt_NAME],
									name, by, date, text, now, expire_date
								);

								ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, "Aviso", dialog, "Entiendo", "");
								KickEx(playerid, 500);
							}

						}
						else
						{
							//permanente
							new dialog[480];
							format(dialog, sizeof dialog, 
								
								"\
									Esta IP o cuenta está expulsada permanentemente, detalles:\n\
									\n\
									Tu nombre: %s\n\
									\n\
									Nombre: %s\n\
									Expulsado por: %d\n\
									Fecha expulsión: %s\n\
									Razón: %s\n\
									\n\
								",
								
								PLAYER_TEMP[playerid][pt_NAME],
								name, by, date, text
							);
								
							ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, "Aviso", dialog, "Entiendo", "");
							KickEx(playerid, 500);
						}
					}
					else continue_proccess = true;

					if(continue_proccess)
					{
						inline OnPlayerCheck()
						{
							if(cache_get_row_count(rows))
							{
								TextDrawShowForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][2]);
								TextDrawShowForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][3]);

								if(rows)
								{
									cache_get_value_name_int(0, "id", ACCOUNT_INFO[playerid][ac_ID]);
									cache_get_value_name(0, "ip", ACCOUNT_INFO[playerid][ac_IP], 16);
									cache_get_value_name(0, "email", ACCOUNT_INFO[playerid][ac_EMAIL], 32);
									cache_get_value_name(0, "salt", ACCOUNT_INFO[playerid][ac_SALT], 16);
									cache_get_value_name(0, "pass", ACCOUNT_INFO[playerid][ac_PASS], 65);
									cache_get_value_name_int(0, "config_secure_login", PI[playerid][ac_CONFIG_SECURE_LOGIN]);

									mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET connected = 1, playerid = %d WHERE id = %d;", playerid, ACCOUNT_INFO[playerid][ac_ID]);
									mysql_tquery(srp_db, QUERY_BUFFER);
									PLAYER_TEMP[playerid][pt_USER_EXIST] = true;
								}
								else PLAYER_TEMP[playerid][pt_USER_EXIST] = false;

								GameTextForPlayer(playerid, "_", 500, 3);
								if(PLAYER_TEMP[playerid][pt_USER_EXIST]) {
									new intro = random(sizeof(Intro_Info));
									SetPlayerVirtualWorld(playerid, WORLD_INTRO);

									TextDrawShowForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][1]);
									InterpolateCameraPos(playerid, Intro_Info[intro][intro_info_CAM_X], Intro_Info[intro][intro_info_CAM_Y], Intro_Info[intro][intro_info_CAM_Z], Intro_Info[intro][intro_info_CAM_RX], Intro_Info[intro][intro_info_CAM_RY], Intro_Info[intro][intro_info_CAM_RZ], 10000, CAMERA_MOVE);
									InterpolateCameraLookAt(playerid, Intro_Info[intro][intro_info_CAM_LOOK_AT_X], Intro_Info[intro][intro_info_CAM_LOOK_AT_Y], Intro_Info[intro][intro_info_CAM_LOOK_AT_Z], Intro_Info[intro][intro_info_CAM_LOOK_AT_RX], Intro_Info[intro][intro_info_CAM_LOOK_AT_RY], Intro_Info[intro][intro_info_CAM_LOOK_AT_RZ], 10000, CAMERA_MOVE);
									SetPlayerMyWeather(playerid, Intro_Info[intro][intro_info_WEATHER]);
									SetPlayerMyTime(playerid, Intro_Info[intro][intro_info_HOUR], 0);

									if(!PI[playerid][ac_CONFIG_SECURE_LOGIN] && strcmp(PLAYER_TEMP[playerid][pt_IP], ACCOUNT_INFO[playerid][ac_IP])) // La IP actual no es la misma IP de la última conexión
									{
										SendNotification(playerid, "Tu dirección IP ha cambiado desde tu última conexión.");
									}
									ShowDialog(playerid, DIALOG_LOGIN);
								}
								else
								{
									TextDrawShowForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][0]);
									// Nuevo usuario
									PLAYER_TEMP[playerid][pt_REGISTER_ACTOR] = CreateActor(Intro_Skins[0][0], 444.200225, 514.112182, 1001.419494, 0.939619); //ACTOR DE LA HABITACION DE LA INTRO
									SetActorVirtualWorld(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], playerid + 10);
									ApplyActorAnimation(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], "INT_SHOP", "shop_loop", 4.1, 1, 1, 1, 0, 0);
									
									SetPlayerVirtualWorld(playerid, playerid + 10);
									InterpolateCameraPos(playerid, 447.1717, 512.9380, 1001.6884, 447.1717, 512.9380, 1001.6884, 4000);
									InterpolateCameraLookAt(playerid, 446.4043, 513.5767, 1001.7037, 446.4043, 513.5767, 1001.7037, 4000);
									SetPlayerInterior(playerid, 12);
									
									ShowDialog(playerid, DIALOG_REGISTER);
								}
							}
							else Kick(playerid);
						}
						mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, ip, email, salt, pass, config_secure_login FROM player WHERE name = '%e';", PLAYER_TEMP[playerid][pt_NAME]);
						MySQL_TQueryInline(srp_db,  using inline OnPlayerCheck, QUERY_BUFFER);
					}
				}
				else Kick(playerid);
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT *, UNIX_TIMESTAMP(bans.expire_date) AS expire_date_ts, NOW() AS now, UNIX_TIMESTAMP(NOW()) AS now_ts FROM bans LEFT JOIN bad_history ON bans.id_history = bad_history.id WHERE bans.name = '%e' OR bans.ip = '%e';", PLAYER_TEMP[playerid][pt_NAME], PLAYER_TEMP[playerid][pt_IP]);
			MySQL_TQueryInline(srp_db, using inline OnPlayerBannedCheck, QUERY_BUFFER);
		}
	}
	else if((PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_NORMAL || PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_DEAD) && PLAYER_TEMP[playerid][pt_USER_LOGGED]) // Viene de jugar
	{
		SetSpawnInfo(playerid, NO_TEAM, PI[playerid][ac_SKIN], PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z], PI[playerid][ac_ANGLE], 0, 0, 0, 0, 0, 0);
		SpawnPlayer(playerid);
		return 0;
	}
	
	//PLAYER_TEMP[playerid][pt_GAME_STATE] = GAME_STATE_OCCUPIED;
	return 0;
}

public OnPlayerRequestSpawn(playerid) // Intentar 'spawnear' mediante la selección de skin << spawn >>
{
	if(PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_CONNECTED) CallLocalFunction("OnPlayerRequestClass", "dd", playerid, 0);
    return 0;
}

forward minconnecttime();
public minconnecttime()
{
	SendRconCommand("minconnectiontime 0"); //3000");
	return 1;
}

forward UpdateThePutis();
public UpdateThePutis()
{
	ApplyDynamicActorAnimation(Club_Bar_Actor, "BAR", "BARman_idle", 4.1, 1, 0, 0, 0, 0);
	
	new 
		strip_anim = random(sizeof(RandomStripAnimations)),
		stripper = random(sizeof(Club_Strippers_Actor));
	ApplyDynamicActorAnimation(Club_Strippers_Actor[stripper], "STRIP", RandomStripAnimations[strip_anim], 4.1, 1, 0, 0, 0, 0);
	
	
	for(new i = 0; i != sizeof(Club_Actors); i ++)
	{
		ApplyDynamicActorAnimation(Club_Actors[i], "STRIP", RandomPayStripAnimations[random(sizeof(RandomPayStripAnimations))], 4.1, 1, 0, 0, 0, 0);
	}
	return 1;
}

#define MIN_SECONDS_BETWEEN_TALKS 100 // Deben pasar al menos 1 segundos para volver a hablar.
hook OnPlayerText(playerid, text[])
{
	if(PLAYER_TEMP[playerid][pt_KICKED]) return 0;
	
	if(PLAYER_TEMP[playerid][pt_GAME_STATE] != GAME_STATE_NORMAL || PLAYER_TEMP[playerid][pt_SELECT_TEXTDRAW] || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_HOSPITAL || IsPlayerInWorkTutorial(playerid)) { SendNotification(playerid, "Ahora no puedes hablar."); return 0; }
	if(text[0] == '#' && ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] > 1 /*&& PLAYER_TEMP[playerid][pt_ADMIN_SERVICE]*/)
	{
      	new string[145];
        format(string, sizeof(string), "Admin Chat: {90D496}%s (%s): {"#SILVER_COLOR"}%s", ACCOUNT_INFO[playerid][ac_NAME], ADMIN_LEVELS[ ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] ], text[1]);
		SendChatMessageToAdmins(0x90C3D4FF, string);
		return 0;
	}

	new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_TALK]);
	if(interval < MIN_SECONDS_BETWEEN_TALKS)
	{
		if(ac_Info[CHEAT_TEXT_SPAMMER][ac_Enabled])
		{
			if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_TEXT_SPAMMER][p_ac_info_IMMUNITY])
			{
				if(interval < 50)
				{
					if(!ac_Info[CHEAT_TEXT_SPAMMER][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_TEXT_SPAMMER, float(interval));
					else
					{
						if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_TEXT_SPAMMER][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_TEXT_SPAMMER][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_TEXT_SPAMMER][p_ac_info_DETECTIONS] = 0;
						else PLAYER_AC_INFO[playerid][CHEAT_TEXT_SPAMMER][p_ac_info_DETECTIONS] ++;
						
						PLAYER_AC_INFO[playerid][CHEAT_TEXT_SPAMMER][p_ac_info_LAST_DETECTION] = gettime();
						if(PLAYER_AC_INFO[playerid][CHEAT_TEXT_SPAMMER][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_TEXT_SPAMMER][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_TEXT_SPAMMER, float(interval));
					}
				}
			}
		}
		SendNotification(playerid, "Cálmate."); 
		return 0; 
	}
	PLAYER_TEMP[playerid][pt_ANTIFLOOD_TALK] = GetTickCount();
	
	if(PLAYER_TEMP[playerid][pt_POLICE_CALL_NAME])
	{
		format(PLAYER_TEMP[playerid][pt_POLICE_CALL_NAME_STR], 24, "%s", text);
		SendNotification(playerid, "Operadora: de acuerdo, describe brevemente lo que sucede para enviar una patrulla.");
		PLAYER_TEMP[playerid][pt_POLICE_CALL_NAME] = false;
		PLAYER_TEMP[playerid][pt_POLICE_CALL_DESCRIPTION] = true;
		return 0;
	}
	if(PLAYER_TEMP[playerid][pt_POLICE_CALL_DESCRIPTION])
	{
		PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL] = false;
		PLAYER_TEMP[playerid][pt_POLICE_CALL_NAME] = false;
		PLAYER_TEMP[playerid][pt_POLICE_CALL_DESCRIPTION] = false;
		
		
		new city[45], zone[45];
		GetPlayerZones(playerid, city, zone);
			
		new message[145];
		format(message, sizeof message, "{"#PRIMARY_COLOR"}[Central policía] {FFFFFF}Reporte {"#SILVER_COLOR"}[%s (%d), gps: %s, %s]: {FFFFFF}%s", PLAYER_TEMP[playerid][pt_POLICE_CALL_NAME_STR], playerid, city, zone, text);
		SendPoliceRadioMessage(-1, -1, message);

		SendNotification(playerid, "Operadora: su petición fue enviada, en breve una patrulla acudirá.");
		SendNotification(playerid, "{"#PRIMARY_COLOR"}Llamada finalizada.");
		return 0;
	}
	
	new str_text[190];
	if(text[0] == '!')
	{
		if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET] && PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_POLICE)
		{
			if(text[1] == '!') format(str_text, sizeof str_text, "** [Radio] (( %s: %s ))", PLAYER_TEMP[playerid][pt_RP_NAME], text[2]);
			else format(str_text, sizeof str_text, "{8D8DFF}** [Radio]{FF8C00} %s %s: {FFFFFF}%s", POLICE_RANKS[ PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] ], PLAYER_TEMP[playerid][pt_RP_NAME], text[1]);
			
			SendPoliceRadioMessage(PLAYER_TEMP[playerid][pt_POLICE_RADIO], 0xCCCCCCCC, str_text);
			return 0;
		}
		
		if(PI[playerid][ac_CREW])
		{
			if(text[1] == '!') format(str_text, sizeof str_text, "** [Radio] (( %s: %s ))", PLAYER_TEMP[playerid][pt_RP_NAME], text[2]);
			else format(str_text, sizeof str_text, "{8D8DFF}** [Radio] %s %s: %s", PLAYER_TEMP[playerid][pt_RP_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_NAME], text[1]);
			
			SendMessageToCrewMembers(PI[playerid][ac_CREW], 0xCCCCCCCC, str_text);
			return 0;
		}
	}
	
	if(PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_STATE] == CALL_STATE_ESTABLISHED)
	{
		format(str_text, 145, "%s dice (por teléfono): %s", PLAYER_TEMP[playerid][pt_RP_NAME], text);
		SendClientMessageEx(PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID], -1, "{"#SILVER_COLOR"}Teléfono: %s: %s", convertPhoneNumber(PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID], PI[playerid][ac_PHONE_NUMBER]), text);
	}
	else
	{
		if(GetPlayerDrunkLevel(playerid) > 2000) format(str_text, 145, "%s dice (ebrio): %s", PLAYER_TEMP[playerid][pt_RP_NAME], text);
		else format(str_text, 145, "%s dice: %s", PLAYER_TEMP[playerid][pt_RP_NAME], text);
	}
	ProxDetector(playerid, 15.0, str_text, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, 85);
	return 0;
}

public OnPlayerEnterDynamicArea(playerid, areaid)
{
	PLAYER_TEMP[playerid][pt_LAST_AREA_ID] = areaid;

	new info[2];
	Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, info);
	switch(info[0])
	{
		case AREA_TYPE_GANGZONE:
		{
			new index = info[1];
			
			if(TERRITORIES[index][territory_VALID])
			{
				PLAYER_TEMP[playerid][pt_LAST_TERRITORY] = index;
				if(PI[playerid][ac_CREW])
				{
					if(TERRITORIES[index][territory_WAR])
					{
						SendFormatNotification(playerid, "Este territorio está siendo atacado por la banda ~r~'%s'~w~.",CREW_INFO[ TERRITORIES[index][territory_ATTACKER_CREW_INDEX] ][crew_NAME]);
						
						new r, g, b, a;
						HexToRGBA(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR], r, g, b, a);
						SetPlayerColorEx(playerid, RGBAToHex(r, g, b, 0));
						
						TextDrawShowForPlayer(playerid, Textdraws[textdraw_TERRITORY_BOX]);
						TextDrawShowForPlayer(playerid, TERRITORIES[index][territory_TEXTDRAW]);
					}
				}
			}
		}
	}

	return 1;
}

public OnPlayerLeaveDynamicArea(playerid, areaid)
{
	new info[2];
	Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, info);
	
	switch(info[0])
	{
		case AREA_TYPE_NONE:
		{
			if(areaid == Lumberjack_Area)
			{
				if(PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_LUMBERJACK)
				{
					SendNotification(playerid, "Has abandonado el trabajo de leñador porque te has alejado demasiado de la zona.");
					CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
				}
			}
			else if(areaid == Farmer_Area)
			{
				if(PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_FARMER) CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
			}
			else if(areaid == Harvest_Area)
			{
				if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
				{
					if(PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_HARVESTER)
					{
						SendNotification(playerid, "Has abandonado tu trabajo de cosechador porque te has alejado demasiado de la zona.");
						CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
					}
				}
			}
			
			if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL)
			{
				if(areaid == Jail_Areas[0] || areaid == Jail_Areas[1] || areaid == Jail_Areas[2] || areaid == Jail_Areas[3] )
				{
					SetPlayerPosEx(playerid, JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID] ][jail_X], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_Y], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_Z], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_ANGLE], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_INTERIOR], 0, true);
				}
			}
		}
		case AREA_TYPE_GANGZONE:
		{
			new index = info[1];
			
			if(TERRITORIES[index][territory_VALID])
			{
				if(PI[playerid][ac_CREW])
				{
					SetPlayerNormalColor(playerid);
					TextDrawHideForPlayer(playerid, Textdraws[textdraw_TERRITORY_BOX]);
					TextDrawHideForPlayer(playerid, TERRITORIES[index][territory_TEXTDRAW]);
				}
			}
		}
	}
	return 1;
}

public OnPlayerEnterDynamicCP(playerid, checkpointid)
{
	new info[1];
	Streamer_GetArrayData(STREAMER_TYPE_CP, checkpointid, E_STREAMER_EXTRA_ID, info);

	switch(info[0])
	{
		case CHECKPOINT_TYPE_GPS:
		{
			DestroyDynamicCP(PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT]);
			PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT] = INVALID_STREAMER_ID;
		}
		case CHECKPOINT_TYPE_UNLOAD_TRUCK:
		{
			if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 1;
			if(!PLAYER_WORKS[playerid][WORK_TRUCK][pwork_SET]) return 1;
			if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_TRUCK) return 1;
	
			new vehicleid = GetPlayerVehicleID(playerid);
			if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] != VEHICLE_TYPE_WORK) return 1;
			if(WORK_VEHICLES[vehicleid][work_vehicle_WORK] != WORK_TRUCK) return 1;
			if(TRUCK_VEHICLE[vehicleid][truck_vehicle_DELIVERED]) return 1;
			if(TRUCK_VEHICLE[vehicleid][truck_vehicle_UNLOADING]) return 1;
			if(TRUCK_VEHICLE[vehicleid][truck_vehicle_DRIVER_USER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return SendNotification(playerid, "Solo aceptaremos la carga del conductor de este camión.");
			
			DestroyDynamicCP(PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT]);
			PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT] = INVALID_STREAMER_ID;
			
			SetVehicleVelocity(vehicleid, 0.0, 0.0, 0.0);
				
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
			UpdateVehicleParams(vehicleid);
			
			TRUCK_VEHICLE[vehicleid][truck_vehicle_UNLOADING] = true;
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0], "");
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3]);
				
			PLAYER_TEMP[playerid][pt_TRUCK_LOADING_VALUE] = 40.0;
			UpdatePlayerLoadingTruckSize(playerid);
			KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][7]);
			KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][9]);
			PLAYER_TEMP[playerid][pt_TIMERS][9] = SetTimerEx("TruckUnLoadUp", 1000, false, "ii", playerid, vehicleid);
		}
		case CHECKPOINT_TYPE_FINISH_TRUCK:
		{
			if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 1;
			if(!PLAYER_WORKS[playerid][WORK_TRUCK][pwork_SET]) return 1;
			if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_TRUCK) return 1;
	
			new vehicleid = GetPlayerVehicleID(playerid);
			if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] != VEHICLE_TYPE_WORK) return 1;
			if(WORK_VEHICLES[vehicleid][work_vehicle_WORK] != WORK_TRUCK) return 1;
			if(!TRUCK_VEHICLE[vehicleid][truck_vehicle_DELIVERED]) return 1;
			if(TRUCK_VEHICLE[vehicleid][truck_vehicle_UNLOADING]) return 1;
			if(TRUCK_VEHICLE[vehicleid][truck_vehicle_DRIVER_USER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return SendNotification(playerid, "Solo le pagaremos al conductor que entregó la mercancía.");
			
			DestroyDynamicCP(PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT]);
			PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT] = INVALID_STREAMER_ID;
			
			SetVehicleVelocity(vehicleid, 0.0, 0.0, 0.0);
			
			new work_extra_payment;
			if(work_info[WORK_TRUCK][work_info_EXTRA_PAY] > 0 && work_info[WORK_TRUCK][work_info_EXTRA_PAY_EXP] > 0)
			{
				work_extra_payment = (work_info[WORK_TRUCK][work_info_EXTRA_PAY] * floatround(floatdiv(PLAYER_WORKS[playerid][WORK_TRUCK][pwork_LEVEL], work_info[WORK_TRUCK][work_info_EXTRA_PAY_EXP])));
				if(work_info[WORK_TRUCK][work_info_EXTRA_PAY_LIMIT] != 0) if(work_extra_payment > work_info[WORK_TRUCK][work_info_EXTRA_PAY_LIMIT]) work_extra_payment = work_info[WORK_TRUCK][work_info_EXTRA_PAY_LIMIT];
			
				if(ACCOUNT_INFO[playerid][ac_VIP]) work_extra_payment += SU_WORK_EXTRA_PAY;
			}
			
			if(GivePlayerCash(playerid, Truck_Contents[ TRUCK_VEHICLE[vehicleid][truck_vehicle_POINT] ][truck_content_MONEY] + work_extra_payment, true, false)) {
				PLAYER_WORKS[playerid][WORK_TRUCK][pwork_LEVEL] ++;
				AddPlayerJobPoints(playerid, WORK_TRUCK);
				
				new string[64];
				format(string, sizeof string, "~g~+%s$", number_format_thousand(Truck_Contents[ TRUCK_VEHICLE[vehicleid][truck_vehicle_POINT] ][truck_content_MONEY] + work_extra_payment));
				GameTextForPlayer(playerid, string, 5000, 1);
				SetVehicleToRespawnEx(vehicleid);
			}
		}
		case CHECKPOINT_TYPE_TRASH:
		{
			if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 1;
			if(!PLAYER_WORKS[playerid][WORK_TRASH][pwork_SET]) return DestroyPlayerTrashCheckpoint(playerid);
			if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_TRASH) return DestroyPlayerTrashCheckpoint(playerid);
			if(!PLAYER_TEMP[playerid][pt_TRASH_PASSENGER]) return 1;
			
			new driver = TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID];
			SendClientMessage(driver, -1, "{"#SILVER_COLOR"}Espera mientras tu compañero recoge la basura.");
			
			new Float:offset = floatdiv(PLAYER_WORKS[playerid][WORK_TRASH][pwork_LEVEL], 10);
			if(offset > 30.0) offset = 30.0;
			
			PLAYER_TEMP[playerid][pt_RECYCLE_BIN_VALUE] = 10.0 + offset;
			UpdatePlayerTrashRecycleSize(playerid);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2]);
			
			TogglePlayerControllableEx(playerid, false);
			SetCameraBehindPlayer(playerid);
			ApplyAnimation(playerid, "ROB_BANK", "CAT_Safe_Rob", 4.1, true, false, false, false, 0);
			
			KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][10]);
			PLAYER_TEMP[playerid][pt_TIMERS][10] = SetTimerEx("RecycleUp", 1000, false, "i", playerid);
		}
		case CHECKPOINT_TYPE_FINISH_TRASH:
		{
			if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 1;
			if(!PLAYER_WORKS[playerid][WORK_TRASH][pwork_SET]) return DestroyPlayerTrashCheckpoint(playerid);
			if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_TRASH) return DestroyPlayerTrashCheckpoint(playerid);
			if(!PLAYER_TEMP[playerid][pt_TRASH_DRIVER]) return 1;
			
			new route = TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE];
			new money = TRASH_OBJECTS[route][trash_route_MONEY];
			
			new driver_work_extra_payment, passenger_work_extra_payment;
			if(work_info[WORK_TRASH][work_info_EXTRA_PAY] > 0 && work_info[WORK_TRASH][work_info_EXTRA_PAY_EXP] > 0)
			{
				driver_work_extra_payment = (work_info[WORK_TRASH][work_info_EXTRA_PAY] * floatround(floatdiv(PLAYER_WORKS[playerid][WORK_TRASH][pwork_LEVEL], work_info[WORK_TRASH][work_info_EXTRA_PAY_EXP])));
				if(work_info[WORK_TRASH][work_info_EXTRA_PAY_LIMIT] != 0) if(driver_work_extra_payment > work_info[WORK_TRASH][work_info_EXTRA_PAY_LIMIT]) driver_work_extra_payment = work_info[WORK_TRASH][work_info_EXTRA_PAY_LIMIT];
				if(ACCOUNT_INFO[playerid][ac_VIP]) driver_work_extra_payment += SU_WORK_EXTRA_PAY;
				
				passenger_work_extra_payment = (work_info[WORK_TRASH][work_info_EXTRA_PAY] * floatround(floatdiv(PLAYER_WORKS[ TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID] ][WORK_TRASH][pwork_LEVEL], work_info[WORK_TRASH][work_info_EXTRA_PAY_EXP])));
				if(work_info[WORK_TRASH][work_info_EXTRA_PAY_LIMIT] != 0) if(passenger_work_extra_payment > work_info[WORK_TRASH][work_info_EXTRA_PAY_LIMIT]) passenger_work_extra_payment = work_info[WORK_TRASH][work_info_EXTRA_PAY_LIMIT];
				if(ACCOUNT_INFO[ TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID] ][ac_VIP]) passenger_work_extra_payment += SU_WORK_EXTRA_PAY;
			}
			
			GivePlayerCash(playerid, money + driver_work_extra_payment, true, false);
			SendFormatNotification(playerid, "Trabajo finalizado, has ganado ~g~%s dólares~w~.", number_format_thousand(money + driver_work_extra_payment));
			PLAYER_WORKS[playerid][WORK_TRASH][pwork_LEVEL] ++;
			AddPlayerJobPoints(playerid, WORK_TRASH);
			
			GivePlayerCash(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], money + passenger_work_extra_payment, true, false);
			SendFormatNotification(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], "Trabajo finalizado, has ganado ~g~%s dólares~w~.", number_format_thousand(money + passenger_work_extra_payment));
			PLAYER_WORKS[ TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID] ][WORK_TRASH][pwork_LEVEL] ++;
			AddPlayerJobPoints(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], WORK_TRASH);
			
			CancelTrashWork(playerid, TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
		}
		case CHECKPOINT_TYPE_LUMBERJACK:
		{
			if(PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_LUMBERJACK)
			{
				DestroyDynamicCP(PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT]);
				PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT] = INVALID_STREAMER_ID;
				
				SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
				RemovePlayerAttachedObject(playerid, 8);
				PLAYER_TEMP[playerid][pt_PLAYER_CARRYING_TREE] = false;
				ApplyAnimation(playerid, "CARRY", "putdwn05", 4.1, 0, 1, 1, 0, 0, 1);
				
				new work_extra_payment;
				if(work_info[WORK_LUMBERJACK][work_info_EXTRA_PAY] > 0 && work_info[WORK_LUMBERJACK][work_info_EXTRA_PAY_EXP] > 0)
				{
					work_extra_payment = (work_info[WORK_LUMBERJACK][work_info_EXTRA_PAY] * floatround(floatdiv(PLAYER_WORKS[playerid][WORK_LUMBERJACK][pwork_LEVEL], work_info[WORK_LUMBERJACK][work_info_EXTRA_PAY_EXP])));
					if(work_info[WORK_LUMBERJACK][work_info_EXTRA_PAY_LIMIT] != 0) if(work_extra_payment > work_info[WORK_LUMBERJACK][work_info_EXTRA_PAY_LIMIT]) work_extra_payment = work_info[WORK_LUMBERJACK][work_info_EXTRA_PAY_LIMIT];
					
					if(ACCOUNT_INFO[playerid][ac_VIP]) work_extra_payment += SU_WORK_EXTRA_PAY;
				}
				PLAYER_WORKS[playerid][WORK_LUMBERJACK][pwork_LEVEL] ++;
				AddPlayerJobPoints(playerid, WORK_LUMBERJACK);
				
				new amount = minrand(40, 50) + work_extra_payment;
				GivePlayerCash(playerid, amount, true, false);
				
				new string[64];
				format(string, sizeof string, "~g~+%d$", amount);
				GameTextForPlayer(playerid, string, 5000, 1);
			}
			else
			{
				DestroyDynamicCP(PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT]);
				PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT] = INVALID_STREAMER_ID;
			}
		}
		case CHECKPOINT_TYPE_PIZZA_DEL:
		{
			if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 1;
			PLAYER_TEMP[playerid][pt_PIZZA_PROCCESS] --;
			PLAYER_TEMP[playerid][pt_DELIVERED_PIZZAS] ++;
			RemovePlayerAttachedObject(playerid, 9);
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
			ApplyDynamicActorAnimation(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR], "FOOD", "EAT_Pizza", 4.1, false, true, true, false, 0);
			ApplyDynamicActorAnimation(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR], "FOOD", "EAT_Pizza", 4.1, false, true, true, false, 0);
			DestroyDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]);
			PIZZA_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][pizza_vehicle_PIZZA_POINT_DELI] = true;
			PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT] = INVALID_STREAMER_ID;
			
			SendNotification(playerid, "Genial, vuelve a subir a la moto para continuar.");
		}
		case CHECKPOINT_TYPE_PIZZA_FINISH:
		{
			if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 1;
			
			new vehicleid = GetPlayerVehicleID(playerid);
			if(!PIZZA_VEHICLE[vehicleid][pizza_vehicle_STARTED]) return 1;
			if(PIZZA_VEHICLE[vehicleid][pizza_vehicle_DRIVER_AID] != ACCOUNT_INFO[playerid][ac_ID]) return SendNotification(playerid, "Solo le pagaremos al conductor que realizó el trabajo.");
		
			new work_extra_payment;
			if(work_info[WORK_PIZZA][work_info_EXTRA_PAY] > 0 && work_info[WORK_PIZZA][work_info_EXTRA_PAY_EXP] > 0)
			{
				work_extra_payment = (work_info[WORK_PIZZA][work_info_EXTRA_PAY] * floatround(floatdiv(PLAYER_WORKS[playerid][WORK_PIZZA][pwork_LEVEL], work_info[WORK_PIZZA][work_info_EXTRA_PAY_EXP])));
				if(work_info[WORK_PIZZA][work_info_EXTRA_PAY_LIMIT] != 0) if(work_extra_payment > work_info[WORK_PIZZA][work_info_EXTRA_PAY_LIMIT]) work_extra_payment = work_info[WORK_PIZZA][work_info_EXTRA_PAY_LIMIT];
			
				if(ACCOUNT_INFO[playerid][ac_VIP]) work_extra_payment += SU_WORK_EXTRA_PAY;
			}
			
			PLAYER_WORKS[playerid][WORK_PIZZA][pwork_LEVEL] ++;
			new money = (PLAYER_TEMP[playerid][pt_DELIVERED_PIZZAS] * 200) + work_extra_payment;
			GivePlayerCash(playerid, money, true, false);
			SendFormatNotification(playerid, "Gracias por tu trabajo, has repartido %d pizzas y has ganado ~g~%s dólares~w~.", PLAYER_TEMP[playerid][pt_DELIVERED_PIZZAS], number_format_thousand(money));
			
			CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
		}
		default: return 1;
	}
	return 1;
}

public OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ)
{
	if(index != PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT]) return 0;
	
    if(response)
    {
        if(fScaleX >= 1.5 || fScaleY >= 1.5 || fScaleZ >= 1.5)
	    {
	        SendNotification(playerid, "El tamaño del accesorio es demasiado grande.");
			
			RemovePlayerAttachedObject(playerid, PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT]);
			if(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED])
			{
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
			}
			return 1;
	    }
        if((fOffsetX >= 0.6 || -0.6 >= fOffsetX) || (fOffsetY >= 0.6 || -0.6 >= fOffsetY) || (fOffsetZ >= 0.6 || -0.6 >= fOffsetZ))
        {
            SendNotification(playerid, "La posición del objeto está demasiado lejos de ti.");
			
			RemovePlayerAttachedObject(playerid, PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT]);
			if(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED])
			{
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
			}
			return 1;
        }
        
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_OFFSET_X] = fOffsetX;
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_OFFSET_Y] = fOffsetY;
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_OFFSET_Z] = fOffsetZ;
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ROT_X] = fRotX;
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ROT_Y] = fRotY;
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ROT_Z] = fRotZ;
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_SCALE_X] = fScaleX;
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_SCALE_Y] = fScaleY;
		PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_SCALE_Z] = fScaleZ;
		
		if(!PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED]) PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED] = true;
		
        SendFormatNotification(playerid, "Accesorio ~b~'%s' ~w~actualizado.", PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_NAME]);
    }
    else
    {
        RemovePlayerAttachedObject(playerid, PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT]);
		if(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED])
		{
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
		}
		
        SendNotification(playerid, "Has cancelado la edición.");
    }
    return 1;
}

forward OnDatabaseConnected();
forward OnDatabaseConnected_Timer();
public OnDatabaseConnected_Timer()
 {
	CallLocalFunction("OnDatabaseConnected", "");
}

ConnectDatabase()
{
	//mysql
	print("[MySQL] Conectando a la base de datos...");
	mysql_log(ERROR | WARNING);
	//mysql_log(NONE);

	//srp_db
	srp_db = mysql_connect_file("srp_db.ini");
	if(srp_db == MYSQL_INVALID_HANDLE || mysql_errno(srp_db) != 0)
	{
		printf("[MySQL] No se pudo conectar con la base de datos (srp_db) (%d)", mysql_errno(srp_db));
		SendRconCommand("exit");
	}
	else
	{
		mysql_tquery(srp_db, "UPDATE player SET connected = 0, playerid = 0;");
  		printf("[MySQL] Base de datos conectada (srp_db) (%d)", _:srp_db);
		SanAndreas();
	 	LoadServerInfo();
	 	UpdateEconomy();
	}
	return 1;
}

public OnDatabaseConnected() {
	SanAndreas();
	LoadServerInfo();
	UpdateEconomy();
}

public OnQueryError(errorid, const error[], const callback[], const query[], MySQL:handle)
{
	switch(errorid) 
	{
		case CR_SERVER_GONE_ERROR: printf("[MySQL] [ERROR]: Se ha perdido la conexión con el servidor (%d)", _:handle);
		case ER_SYNTAX_ERROR: printf("[MySQL] [ERROR]: Syntax error, query: %s (%d)", query, _:handle);
		default: printf("[MySQL] [ERROR] errorid: %d, error: %s, callback: %s, query: %s, handle: %d", errorid, error, callback, query, _:handle);
	}
	return 1;
}

forward BajarSegundos();
public BajarSegundos()
{
	for( new i; i < sizeof MinaInfo; i++ )
	{
		if( MinaInfo[i][w_count] == 0 )
		{
			MinaInfo[i][w_time]++;
		}
		if( MinaInfo[i][w_time] >= 30)
		{
			MinaInfo[i][w_object] = CreateDynamicObject(1207, MinaInfo[i][RocaX], MinaInfo[i][RocaY], MinaInfo[i][RocaZ], MinaInfo[i][RocaRX], MinaInfo[i][RocaRY], MinaInfo[i][RocaRZ]);
			SetMinaRoca(MinaInfo[i][w_mina],i);
			SetTexuraRoca(MinaInfo[i][w_object],MinaInfo[i][w_tipo]);
			MinaInfo[i][w_use] =
			MinaInfo[i][w_drop] = false;
			MinaInfo[i][w_count] = 100;
			MinaInfo[i][w_time] = 0;
		}
	}
	return 1;
}
forward TimerSound(playerid);
public TimerSound(playerid)
{
		PlayerPlaySound(playerid,1135,0.0,0.0,0.0);
		return 1;
}

forward KickPlayer(playerid);
public KickPlayer(playerid)
{
	return Kick(playerid);
}

forward NoCallResponse(playerid);
public NoCallResponse(playerid)
{
	if(!PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL]) return 0;
	if(PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID] == INVALID_PLAYER_ID) return 0;
	if(PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_STATE] != CALL_STATE_WAITING_RESPONSE) return 0;
	
	EndPhoneCall(playerid);
	return 1;
}



public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	if(PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_ENABLED])
	{
		if(playertextid == PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][2]) //Anterior
		{
			if(PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] == 0) PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] = sizeof(PROPERTY_INTERIORS) - 1;
			else PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] --;
			UpdatePlayerPropertyContructor(playerid);
			return 1;
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][3]) //Siguiente
		{
			if(PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] == (sizeof(PROPERTY_INTERIORS) - 1)) PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] = 0;			
			else PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_INTERIO] ++;
			UpdatePlayerPropertyContructor(playerid);
			return 1;
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][4]) //Crear
		{
			Create_PlayerPropertyConstructo(playerid);
			return 1;
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][5]) //Cancelar
		{
			ExitPlayerPropertyConstructor(playerid);
			return 1;
		}
		return 1;
	}
	
	
	if(PLAYER_TEMP[playerid][pt_READING_GUIDE])
	{
		if(playertextid == PlayerTextdraws[playerid][ptextdraw_GUIDE][2]) // Siguiente
		{
			if(PLAYER_TEMP[playerid][pt_GUIDE_STEP] + 1 >= sizeof(Server_Rules) )
			{
				PLAYER_TEMP[playerid][pt_READING_GUIDE] = false;
				PLAYER_TEMP[playerid][pt_GUIDE_STEP] = 0;
				DestroyPlayerRulesTextDraw(playerid);
				
				for(new i = 0; i < LOAD_SCREEN_TEXTDRAWS_SIZE; i ++) {
					TextDrawHideForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][i]);
				}

				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9]);
				
				PI[playerid][ac_GENDER] = -1;
				PLAYER_TEMP[playerid][pt_REGISTER_SELECTING_SKIN] = true;
				PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] = 0;
			
				new td_str[64];
				format(td_str, sizeof td_str, "%d/%d", PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] + 1, sizeof(Intro_Skins[]));
				PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], td_str);
				
				SendNotification(playerid, "Elige tu personaje, presionar 'continuar' para finalizar.");
				SelectTextDrawEx(playerid, 0xe8d08fFF);
				return 1;
			}
			
			PLAYER_TEMP[playerid][pt_GUIDE_STEP] ++;
			SetPlayerRulesTextDraw(playerid, Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Title], Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Info], Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Pages], PLAYER_TEMP[playerid][pt_GUIDE_STEP] + 1, sizeof(Server_Rules) );
			return 1;
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_GUIDE][3]) // Anterior
		{
			if(PLAYER_TEMP[playerid][pt_GUIDE_STEP] <= 0) return 1;
			
			PLAYER_TEMP[playerid][pt_GUIDE_STEP] --;
			SetPlayerRulesTextDraw(playerid, Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Title], Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Info], Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Pages], PLAYER_TEMP[playerid][pt_GUIDE_STEP] + 1, sizeof(Server_Rules) );
			return 1;
		}
	}
	
	if(PLAYER_TEMP[playerid][pt_REGISTER_SELECTING_SKIN])
	{
		if(playertextid == PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3]) // Hombre
		{
			PI[playerid][ac_GENDER] = 0;
			
			PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], -94);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2]);
			PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], -256);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4]);
			
			SetActorSkin(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], Intro_Skins[PI[playerid][ac_GENDER]][PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN]], PLAYER_TEMP[playerid][pt_REGISTER_ACTOR]);
			ApplyActorAnimation(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], "INT_SHOP", "shop_loop", 4.1, 1, 1, 1, 0, 0);
			return 1;
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5]) // Mujer
		{
			PI[playerid][ac_GENDER] = 1;
			
			PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2], -256);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2]);
			PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4], -94);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4]);
			
			SetActorSkin(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], Intro_Skins[PI[playerid][ac_GENDER]][PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN]], PLAYER_TEMP[playerid][pt_REGISTER_ACTOR]);
			ApplyActorAnimation(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], "INT_SHOP", "shop_loop", 4.1, 1, 1, 1, 0, 0);
			return 1;
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6]) // Continuar
		{
			if(PI[playerid][ac_GENDER] == -1)
			{
				SendNotification(playerid, "Antes de continuar debes optar por un género para tu personaje.");
				SelectTextDrawEx(playerid, 0xe8d08fFF);
				return 1;
			}
			
			TextDrawShowForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][0]);
			PI[playerid][ac_SKIN] = Intro_Skins[PI[playerid][ac_GENDER]][PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN]];
			
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0]);
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1]);
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2]);
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3]);
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4]);
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5]);
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6]);
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7]);
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8]);
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9]);
			
			PLAYER_TEMP[playerid][pt_REGISTER_SELECTING_SKIN] = false;
			PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] = 0;

			CancelSelectTextDrawEx(playerid);
			DestroyActor(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR]);
			PLAYER_TEMP[playerid][pt_REGISTER_ACTOR] = INVALID_ACTOR_ID;			

			// Spawn 
			SetPlayerScore(playerid, ACCOUNT_INFO[playerid][ac_LEVEL]);
			PLAYER_TEMP[playerid][pt_DOUBT_CHANNEL_TIME] = gettime();
			ResetPlayerWeapons(playerid);
			ResetPlayerMoney(playerid);
			GivePlayerMoney(playerid, PI[playerid][ac_CASH]);
			SetPlayerFightingStyle(playerid, PI[playerid][ac_FIGHT_STYLE]);
			SetPlayerHealthEx(playerid, PI[playerid][ac_HEALTH]);
			SetPlayerArmourEx(playerid, PI[playerid][ac_ARMOUR]);
			SetPlayerVirtualWorld(playerid, 0);
			SetSpawnInfo(playerid, NO_TEAM, PI[playerid][ac_SKIN], PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z], PI[playerid][ac_ANGLE], 0, 0, 0, 0, 0, 0);
			SetPlayerInterior(playerid, PI[playerid][ac_INTERIOR]);
			
			//
			RegisterNewPlayer(playerid);
			return 1;
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7]) // Siguiente ropa
		{
			if(PI[playerid][ac_GENDER] == -1) return SendNotification(playerid, "Debes optar por un género antes de cambiar de ropa.");
			
			if(PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] == (sizeof(Intro_Skins[]) - 1)) PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] = 0;			
			else PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] ++;
			
			new td_str[64];
			format(td_str, sizeof td_str, "%d/%d", PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] + 1, sizeof(Intro_Skins[]));
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], td_str);
			
			SetActorSkin(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], Intro_Skins[PI[playerid][ac_GENDER]][PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN]], PLAYER_TEMP[playerid][pt_REGISTER_ACTOR]);
			ApplyActorAnimation(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], "INT_SHOP", "shop_loop", 4.1, 1, 1, 1, 0, 0);
			return 1;
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8]) // Anterior ropa
		{
			if(PI[playerid][ac_GENDER] == -1) return SendNotification(playerid, "Debes optar por un género antes de cambiar de ropa.");
			
			if(PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] = sizeof(Intro_Skins[]) - 1;
			else PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] --;
			
			new td_str[64];
			format(td_str, sizeof td_str, "%d/%d", PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] + 1, sizeof(Intro_Skins[]));
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], td_str);
			
			SetActorSkin(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], Intro_Skins[PI[playerid][ac_GENDER]][PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN]], PLAYER_TEMP[playerid][pt_REGISTER_ACTOR]);
			ApplyActorAnimation(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], "INT_SHOP", "shop_loop", 4.1, 1, 1, 1, 0, 0);
			return 1;
		}
		return 1;
	}
	
	if(PLAYER_TEMP[playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return 0;
	
	if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] != -1)
	{
		if(playertextid == PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][2]) // Comprar
		{
			new clothe_info[3];
			switch(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE])
			{
				case INTERIOR_BINCO:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE: clothe_info = Binco_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
						case SEX_FEMALE: clothe_info = Binco_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
					}
				}
				case INTERIOR_SUBURBAN:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE: clothe_info = Suburban_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
						case SEX_FEMALE: clothe_info = Suburban_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
					}
				}
				case INTERIOR_PROLAPS:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE: clothe_info = Prolaps_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
						case SEX_FEMALE: clothe_info = Prolaps_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
					}
				}
				case INTERIOR_DIDIER_SACHS:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE: clothe_info = Didier_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
						case SEX_FEMALE: clothe_info = Didier_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
					}
				}
				case INTERIOR_VICTIM:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE: clothe_info = Victim_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
						case SEX_FEMALE: clothe_info = Victim_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
					}
				}
				case INTERIOR_ZIP:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE: clothe_info = Zip_Shop_Male_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
						case SEX_FEMALE: clothe_info = Zip_Shop_Female_Skins[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN]];
					}
				}
			}
			
			if(PI[playerid][ac_CASH] >= clothe_info[2])
			{
				if(GivePlayerCash(playerid, -clothe_info[2], true, true)) {
					PI[playerid][ac_SKIN] = clothe_info[0];
					
					PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
					SendFormatNotification(playerid, "Gracias por comprar ropa en ~g~%s~w~, vuelva pronto.", Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_NAME]);
					
					ClosePlayerClothingMenu(playerid);
					ExitPlayerWardrobe(playerid);
				}
			}
			else
			{
				PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
				SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar esta ropa.", number_format_thousand(clothe_info[2] - PI[playerid][ac_CASH]));
			}
			return 1;
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][3]) // Siguiente
		{
			PlayerPlaySoundEx(playerid, 1052, 0.0, 0.0, 0.0);
			
			switch(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE])
			{
				case INTERIOR_BINCO:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Binco_Shop_Male_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
						}
						case SEX_FEMALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Binco_Shop_Female_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
						}
					}
				}
				case INTERIOR_SUBURBAN:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Suburban_Shop_Male_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
						}
						case SEX_FEMALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Suburban_Shop_Female_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
						}
					}
				}
				case INTERIOR_PROLAPS:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Prolaps_Shop_Male_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
						}
						case SEX_FEMALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Prolaps_Shop_Female_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
						}
					}
				}
				case INTERIOR_DIDIER_SACHS:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Didier_Shop_Male_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
						}
						case SEX_FEMALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Didier_Shop_Female_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
						}
					}
				}
				case INTERIOR_VICTIM:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Victim_Shop_Male_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
						}
						case SEX_FEMALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Victim_Shop_Female_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
						}
					}
				}
				case INTERIOR_ZIP:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Zip_Shop_Male_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
						}
						case SEX_FEMALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == (sizeof(Zip_Shop_Female_Skins) - 1)) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] ++;
						}
					}
				}
			}
			
			UpdateClothingShop(playerid);
			return 1;
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_CLOTHING_SHOP][4]) // Anterior
		{
			PlayerPlaySoundEx(playerid, 1053, 0.0, 0.0, 0.0);
			
			switch(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE])
			{
				case INTERIOR_BINCO:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Binco_Shop_Male_Skins) - 1;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
						}
						case SEX_FEMALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Binco_Shop_Female_Skins) - 1;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
						}
					}
				}
				case INTERIOR_SUBURBAN:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Suburban_Shop_Male_Skins) - 1;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
						}
						case SEX_FEMALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Suburban_Shop_Female_Skins) - 1;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
						}
					}
				}
				case INTERIOR_PROLAPS:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Prolaps_Shop_Male_Skins) - 1;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
						}
						case SEX_FEMALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Prolaps_Shop_Female_Skins) - 1;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
						}
					}
				}
				case INTERIOR_DIDIER_SACHS:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Didier_Shop_Male_Skins) - 1;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
						}
						case SEX_FEMALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Didier_Shop_Female_Skins) - 1;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
						}
					}
				}
				case INTERIOR_VICTIM:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Victim_Shop_Male_Skins) - 1;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
						}
						case SEX_FEMALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Victim_Shop_Female_Skins) - 1;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
						}
					}
				}
				case INTERIOR_ZIP:
				{
					switch(PI[playerid][ac_GENDER])
					{
						case SEX_MALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Zip_Shop_Male_Skins) - 1;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
						}
						case SEX_FEMALE:
						{
							if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] == 0) PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = sizeof(Zip_Shop_Female_Skins) - 1;
							else PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] --;
						}
					}
				}
			}
			
			UpdateClothingShop(playerid);
			return 1;
		}
	}
	
	if(PLAYER_TEMP[playerid][pt_TOYS_SHOP])
	{
		if(playertextid == PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][5]) //Comprar
		{		
			if(PI[playerid][ac_CASH] >= Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_price])
			{
				new slot = GetEmptyPlayerToySlot(playerid);
				if(!ACCOUNT_INFO[playerid][ac_VIP] && slot >= MAX_NU_TOYS)
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendFormatNotification(playerid, "¡Los jugadores ~y~VIP ~w~pueden tener hasta %d accesorios! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP~w~.", MAX_SU_TOYS);
					return 1;
				}
				if(slot == -1)
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "No tienes más espacio para accesorios, elimina alguno si quieres comprar otro.");
					ClosePlayerToysMenu(playerid);
					return 1;
				}
				
				if(GivePlayerCash(playerid, -Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_price], true, true)) {
					PLAYER_TOYS[playerid][slot][player_toy_VALID] = true;
					format(PLAYER_TOYS[playerid][slot][player_toy_NAME], 24, "%s", Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_model]);
					PLAYER_TOYS[playerid][slot][player_toy_ATTACHED] = false;
					PLAYER_TOYS[playerid][slot][player_toy_MODELID] = Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_modelid];
					PLAYER_TOYS[playerid][slot][player_toy_BONE] = -1; // Sin utilizar aún
					PLAYER_TOYS[playerid][slot][player_toy_OFFSET_X] = 0.0;
					PLAYER_TOYS[playerid][slot][player_toy_OFFSET_Y] = 0.0;
					PLAYER_TOYS[playerid][slot][player_toy_OFFSET_Z] = 0.0;
					PLAYER_TOYS[playerid][slot][player_toy_ROT_X] = 0.0;
					PLAYER_TOYS[playerid][slot][player_toy_ROT_Y] = 0.0;
					PLAYER_TOYS[playerid][slot][player_toy_ROT_Z] = 0.0;
					PLAYER_TOYS[playerid][slot][player_toy_SCALE_X] = 1.0;
					PLAYER_TOYS[playerid][slot][player_toy_SCALE_Y] = 1.0;
					PLAYER_TOYS[playerid][slot][player_toy_SCALE_Z] = 1.0;
					PLAYER_TOYS[playerid][slot][player_toy_COLOR_1] = 0;
					PLAYER_TOYS[playerid][slot][player_toy_COLOR_2] = 0;
					RegisterNewPlayerToy(playerid, slot);

					PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
					SendFormatNotification(playerid, "Gracias por comprar accesorios en ~g~%s~w~, para ponertelo usa ~g~/accesorios~w~.", ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_NAME]);
					
					ClosePlayerToysMenu(playerid);
				}
			}
			else
			{
				PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
				SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar esta ropa.", number_format_thousand(Toys_Shop[ PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ][shop_toy_price] - PI[playerid][ac_CASH]));
			}
			
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][6]) // Siguiente
		{
			PlayerPlaySoundEx(playerid, 1052, 0.0, 0.0, 0.0);
			
			if(PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] == (sizeof(Toys_Shop) - 1)) PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] = 0;
			else PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] ++;
			
			UpdateToysShop(playerid);
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_TOYS_SHOP][7]) // Anterior
		{
			PlayerPlaySoundEx(playerid, 1053, 0.0, 0.0, 0.0);
			
			if(PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] == 0) PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] = sizeof(Toys_Shop) - 1;
			else PLAYER_TEMP[playerid][pt_TOYS_SHOP_TOY_SELECTED] --;
			
			UpdateToysShop(playerid);
		}
		
	}
	
	if(PLAYER_TEMP[playerid][pt_SHOP])
	{
		HidePlayerDialog(playerid);
		if(playertextid == PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][0])
		{
			switch(PLAYER_TEMP[playerid][pt_SHOP_STATE])
			{
				case PLAYER_SHOP_STATE_ALL:
				{
					if(ACCOUNT_INFO[playerid][ac_LEVEL] < 2) return SendNotification(playerid, "Debes ser al menos nivel 2 para publicar anuncios.");
					
					new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP]);
					if(interval < 500) return SendNotification(playerid, "Cálmate.");
					PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP] = GetTickCount();
					
					inline OnShopChecked()
					{
						new rows;
						if(cache_get_row_count(rows))
						{
							if(rows)
							{
								new total_ads;
								cache_get_value_index_int(0, 0, total_ads);

								if(total_ads >= MAX_ADS_PER_PLAYER) SendFormatNotification(playerid, "No puedes publicar más de %d anuncios, borra alguno.", MAX_ADS_PER_PLAYER);
								else
								{
									HidePlayerShop(playerid);
									ShowDialog(playerid, DIALOG_SHOP_ADD);
								}
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM shop WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
					MySQL_TQueryInline(srp_db,  using inline OnShopChecked, QUERY_BUFFER);
				}
			}
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][1])
		{
			switch(PLAYER_TEMP[playerid][pt_SHOP_STATE])
			{
				case PLAYER_SHOP_STATE_ALL:
				{
					PLAYER_TEMP[playerid][pt_SHOP_COME_FROM_MY_ADS] = true;
					PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
					PLAYER_TEMP[playerid][pt_SHOP_STATE] = PLAYER_SHOP_STATE_MY_ADS;
					UpdatePlayerShop(playerid);
				}
			}
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][2])
		{
			switch(PLAYER_TEMP[playerid][pt_SHOP_STATE])
			{
				case PLAYER_SHOP_STATE_ALL: HidePlayerShop(playerid);
				case PLAYER_SHOP_STATE_MY_ADS: // Atras
				{
					new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP]);
					if(interval < 500) return SendNotification(playerid, "Cálmate.");
					PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP] = GetTickCount();
					
					PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
					PLAYER_TEMP[playerid][pt_SHOP_COME_FROM_MY_ADS] = false;
					PLAYER_TEMP[playerid][pt_SHOP_STATE] = PLAYER_SHOP_STATE_ALL;
					UpdatePlayerShop(playerid);
				}
				case PLAYER_SHOP_STATE_AD: ShowDialog(playerid, DIALOG_SHOP_ARTICLE_MODIFY);
			}
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][3])
		{
			switch(PLAYER_TEMP[playerid][pt_SHOP_STATE])
			{
				case PLAYER_SHOP_STATE_ALL: // Siguiente
				{
					new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP]);
					if(interval < 500) return SendNotification(playerid, "Cálmate.");
					PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP] = GetTickCount();

					inline OnCountQueryLoad()
					{
						new rows;
						if(cache_get_row_count(rows))
						{
							if(rows)
							{
								new count, pages;
								cache_get_value_index_int(0, 0, count);
								pages = GetDatabasePages(count, PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);

								if(PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] >= pages - 1) PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
								else PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] ++;
								UpdatePlayerShop(playerid);
							}
						}
					}
					MySQL_TQueryInline(srp_db,  using inline OnCountQueryLoad, "SELECT COUNT(id) FROM shop;");
				}
				case PLAYER_SHOP_STATE_MY_ADS: // Siguiente
				{
					new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP]);
					if(interval < 500) return SendNotification(playerid, "Cálmate.");
					PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP] = GetTickCount();
					
					inline OnCountQueryLoad()
					{
						new rows;
						if(cache_get_row_count(rows))
						{
							if(rows)
							{
								new count, pages;
								cache_get_value_index_int(0, 0, count);
								pages = GetDatabasePages(count, PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);

								if(PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] >= pages - 1) PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
								else PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] ++;
								UpdatePlayerShop(playerid);
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM shop WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
				}
				case PLAYER_SHOP_STATE_AD: ShowDialog(playerid, DIALOG_SHOP_ARTICLE_REMOVE);
			}
		}
		else if(playertextid == PlayerTextdraws[playerid][ptextdraw_SHOP_BUTTONS][4])
		{
			switch(PLAYER_TEMP[playerid][pt_SHOP_STATE])
			{
				case PLAYER_SHOP_STATE_ALL: // Anterior
				{
					new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP]);
					if(interval < 500) return SendNotification(playerid, "Cálmate.");
					PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP] = GetTickCount();
					
					inline OnCountQueryLoad()
					{
						new rows;
						if(cache_get_row_count(rows))
						{
							if(rows)
							{
								new count, pages;
								cache_get_value_index_int(0, 0, count);
								pages = GetDatabasePages(count, PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);

								if(PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] <= 0) PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = pages - 1;
								else PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] --;
								UpdatePlayerShop(playerid);
							}
						}
					}
					MySQL_TQueryInline(srp_db,  using inline OnCountQueryLoad, "SELECT COUNT(id) FROM shop;");
				}
				case PLAYER_SHOP_STATE_MY_ADS: // Anterior
				{
					new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP]);
					if(interval < 500) return SendNotification(playerid, "Cálmate.");
					PLAYER_TEMP[playerid][pt_ANTIFLOOD_SHOP] = GetTickCount();
					
					inline OnCountQueryLoad()
					{
						new rows;
						if(cache_get_row_count(rows))
						{
							if(rows)
							{
								new count, pages;
								cache_get_value_index_int(0, 0, count);
								pages = GetDatabasePages(count, PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);

								if(PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] <= 0) PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = pages - 1;
								else PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] --;
								UpdatePlayerShop(playerid);
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM shop WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
					MySQL_TQueryInline(srp_db,  using inline OnCountQueryLoad, QUERY_BUFFER);
				}
				case PLAYER_SHOP_STATE_AD: // Atrás
				{
					if(PLAYER_TEMP[playerid][pt_SHOP_COME_FROM_MY_ADS])
					{
						PLAYER_TEMP[playerid][pt_SHOP_STATE] = PLAYER_SHOP_STATE_MY_ADS;
						UpdatePlayerShop(playerid);
					}
					else
					{
						PLAYER_TEMP[playerid][pt_SHOP_STATE] = PLAYER_SHOP_STATE_ALL;
						UpdatePlayerShop(playerid);
					}
				}
			}
		}
	}
    return 1;
}

forward AddPlayerReputation(playerid);
public AddPlayerReputation(playerid)
{
	new neccessary_rep = ACCOUNT_INFO[playerid][ac_LEVEL] * REP_MULTIPLIER;
	if(ACCOUNT_INFO[playerid][ac_REP] < neccessary_rep)
	{
		ACCOUNT_INFO[playerid][ac_REP] ++;
		UpdateReputationTextDraws(playerid);
		
		if(ACCOUNT_INFO[playerid][ac_REP] >= neccessary_rep) SendNotification(playerid, "Escribe ~g~/comprarnivel ~w~para subir de nivel.");
		
		ACCOUNT_INFO[playerid][ac_PAYDAY_REP] ++;
		if(ACCOUNT_INFO[playerid][ac_PAYDAY_REP] >= REP_FOR_PAYDAY)
		{
			PlayerPayday(playerid);
			ACCOUNT_INFO[playerid][ac_PAYDAY_REP] = 0;
		}
		
		ACCOUNT_INFO[playerid][ac_TIME_FOR_REP] = TIME_FOR_REP;
		PLAYER_TEMP[playerid][pt_TIME_PASSED_LAST_REP] = gettime() * 1000;

		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET time_playing = %d, level = %d, rep = %d, time_for_rep = %d, payday_rep = %d WHERE id = %d;", ACCOUNT_INFO[playerid][ac_TIME_PLAYING], ACCOUNT_INFO[playerid][ac_LEVEL], ACCOUNT_INFO[playerid][ac_REP], TIME_FOR_REP, ACCOUNT_INFO[playerid][ac_PAYDAY_REP], ACCOUNT_INFO[playerid][ac_ID]);
		mysql_tquery(srp_db, QUERY_BUFFER);

		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][2]);
		PLAYER_TEMP[playerid][pt_TIMERS][2] = SetTimerEx("AddPlayerReputation", ACCOUNT_INFO[playerid][ac_TIME_FOR_REP], false, "i", playerid);
		return 1;
	}
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][2]);
	return 0;
}

forward server_loaded_request(bool:toggle);
public server_loaded_request(bool:toggle) {
	server_loaded = toggle;
	if(toggle) {
		SendRconCommand("hostname "SERVER_HOSTNAME"");
	}
}

public OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
	PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID] = pickupid;
	return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(PLAYER_TEMP[playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return 1;
	
	if(newkeys &  KEY_CTRL_BACK)
	{
		for(new i = 0; i != E_WORKS; i ++)	
		{
			if(work_info[i][work_info_TYPE] == WORK_TYPE_FAMILY) continue;
			
			if(GetPlayerInterior(playerid) == obtain_work_coords[i][obtain_work_INTERIOR])
			{
				if(IsPlayerInRangeOfPoint(playerid, 1.0, obtain_work_coords[i][obtain_work_X], obtain_work_coords[i][obtain_work_Y], obtain_work_coords[i][obtain_work_Z]))
				{
					PC_EmulateCommand(playerid, "/trabajar");
					return 1;
				}
			}
		}
  		if(IsPlayerInRangeOfPoint(playerid, 1.0, 626.2347,116.3636,8.0031)) return ShowDialog(playerid, DIAlOG_BUY_MASO);
		if(IsPlayerInRangeOfPoint(playerid, 1.0, -474.596282, 289.679107, 2004.584960)) return PC_EmulateCommand(playerid, "/pvender");
		if(IsPlayerInRangeOfPoint(playerid, 1.0, -508.645385, 322.147644, 2004.585937)) return PC_EmulateCommand(playerid, "/grua");

		if(IsPlayerInDynamicArea(playerid, Farmer_Area))
		{
			for(new i = 0; i != MAX_PLANTS; i ++)
			{
				new Float:pos[3];
				GetDynamicObjectPos(PLANTS[i][plant_OBJECT_ID], pos[0], pos[1], pos[2]);

				if(IsPlayerInRangeOfPoint(playerid, 1.5, pos[0], pos[1], pos[2]))
				{
					PC_EmulateCommand(playerid, "/recoger");
					return 1;
				}
			}
		}

		if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] != -1 && ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] != INTERIOR_NO_INFO)
		{
			new index = GetBankLocalIndexByIntType(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE]);
			if(index != -1)
			{
				if(IsPlayerInRangeOfPoint(playerid, 1.0, Bank_Interior_Positions[index][bank_X], Bank_Interior_Positions[index][bank_Y], Bank_Interior_Positions[index][bank_Z])) 
				{
					PC_EmulateCommand(playerid, "/banco");
					return 1;
				}
			}
		}

		new info[3];
		new Float:pos[3];
		Streamer_GetArrayData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
		if(info[0] == PICKUP_TYPE_ATM)
		{
			Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_X, pos[0]);
			Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Y, pos[1]);
			Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Z, pos[2]);

			if(IsPlayerInRangeOfPoint(playerid, 1.0, pos[0], pos[1], pos[2]))
			{
				PC_EmulateCommand(playerid, "/cajero");
				return 1;
			}
		}

		new indexx = GetPlayerFastFoodIndex(playerid);
		if(indexx != -10) return PC_EmulateCommand(playerid, "/pedir");

		if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] != -1 && ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] != INTERIOR_NO_INFO)
		{
			PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] = GetClothingShopIndexByIntType(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE]);
			if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] != -1)
			{
				if(IsPlayerInRangeOfPoint(playerid, 1.0, Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_X], Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_Y], Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_Z]))
				{
					ShowDialog(playerid, DIALOG_CLOTHES);
					return 1;
				}
			}
		}

		if(IsPlayerInRangeOfPoint(playerid, 1.0, 2119.059814, -2001.701904, 7.984375)) 
		{
			PC_EmulateCommand(playerid, "/mercado");
			return 1;
		}
		if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] != -1 && ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] == INTERIOR_247)
		{
			if(IsPlayerInRangeOfPoint(playerid, 1.0, -27.964675, -89.948631, 1003.546875)) 
			{
				PC_EmulateCommand(playerid, "/productos");
				return 1;
			}
		}
		
		if(IsPlayerInRangeOfPoint(playerid, 1.0, 253.280288, 76.219017, 1003.640625) || IsPlayerInRangeOfPoint(playerid, 1.0, 266.547180, 119.652816, 1004.617187) || IsPlayerInRangeOfPoint(playerid, 1.0, 217.816406, 187.667922, 1003.031250))
		{
			if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
			if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_NONE && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE)
			{
				SendFormatNotification(playerid, "Tienes que dejar de estar de servicio como %s primero.", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME]);
				return 1;
			}
			
			if(!PLAYER_TEMP[playerid][pt_WORKING_IN]) ShowDialog(playerid, DIALOG_SELECT_POLICE_SKIN);
			else CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);

			return 1;
		}

		if((IsPlayerInRangeOfPoint(playerid, 2.0, 219.244018, 69.982215, 1005.039062) || IsPlayerInRangeOfPoint(playerid, 2.0, 262.945587, 109.785270, 1004.617187) || IsPlayerInRangeOfPoint(playerid, 2.0, 222.683914, 186.956054, 1003.031250)) && PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
		{
			if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");

			ShowDialog(playerid, DIALOG_POLICE_SHOP);
			return 1;
		}
	}
	else if(newkeys & KEY_FIRE)
	{
		if(ACCOUNT_INFO[playerid][ac_LEVEL] == 1 && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_NONE) ApplyAnimation(playerid, "PED", "IDLE_tired", 4.1, false, false, false, false, 0);
		return 1;
	}
	else if(newkeys == KEY_F && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && TieneRoca[playerid] == 0 )
	//else if(newkeys == KEY_F && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && TieneRoca[playerid] == 0 && PI[playerid][ac_GPS] == 1 && PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_MINERO)
		{
			for( new i; i < sizeof MinaInfo; i++ )
			{
				if(IsPlayerInRangeOfPoint(playerid, 1.8, MinaInfo[i][RocaX],MinaInfo[i][RocaY],MinaInfo[i][RocaZ]))
				{
					if(MinaInfo[i][w_use]) return 1;
					if(ProcesoPicando[playerid] != 0) return 1;
					if(PI[playerid][ac_Maso] == 0) return SendNotification(playerid, "No tienes un maso para minar, compralo afuera.");
					if( MinaInfo[i][w_count] > 0 )
					{
						ProcesoPicando[playerid] = 0;		
						//TiempoTecla[playerid] = 2;
						TieneRoca[playerid] = 0;		
						MinaInfo[i][w_use] = true;
						ApplyAnimation(playerid, "BASEBALL","Bat_4", 4.1, 1, 0, 0, 1, 1000);
						TimerMinando[playerid] = SetTimerEx( "TimerPicando", 300, false, "dd", playerid, i);
						TogglePlayerControllableEx(playerid, false );
						ApplyAnimation(playerid, "BASEBALL","Bat_4", 4.1, 1, 0, 0, 1, 1000);
						TimerSonido[playerid] = SetTimerEx( "TimerSound", 1130, true, "d", playerid);
						return 1;
					}
				}
			}
		}
	else if(newkeys & KEY_CROUCH)
	{
		if(PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_NORMAL)
		{
			if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
			{
				for(new i = 0; i != sizeof San_Andreas_Barriers; i ++)
				{
					if(IsPlayerInRangeOfPoint(playerid, San_Andreas_Barriers[i][barrier_DISTANCE], San_Andreas_Barriers[i][barrier_PLAYER_X], San_Andreas_Barriers[i][barrier_PLAYER_Y], San_Andreas_Barriers[i][barrier_PLAYER_Z]))
					{
						if(GetPlayerInterior(playerid) == San_Andreas_Barriers[i][barrier_INTERIOR] && GetPlayerVirtualWorld(playerid) == San_Andreas_Barriers[i][barrier_WORLD])
						{
							if(San_Andreas_Barriers[i][barrier_VEHICLE_TYPE] == VEHICLE_TYPE_WORK)
							{
								if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET] && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE)
								{
									if(San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN2] != WORK_NONE)
									{
										if(!PLAYER_WORKS[playerid][ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN] ][pwork_SET] && !PLAYER_WORKS[playerid][ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN2] ][pwork_SET]) break;
									}
									else
									{
										if(!PLAYER_WORKS[playerid][ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN] ][pwork_SET]) break;
									}
								}
							}
							if(IsDynamicObjectMoving(San_Andreas_Barriers[i][barrier_OBJECT_ID]) && !San_Andreas_Barriers[i][barrier_CLOSING]) break;
							if(San_Andreas_Barriers[i][barrier_PRICE] > 0) {
								if(GivePlayerCash(playerid, -San_Andreas_Barriers[i][barrier_PRICE], false, true)) {
									SendFormatNotification(playerid, "Has pagado el peaje por ~y~%s ~w~dólares.", number_format_thousand(San_Andreas_Barriers[i][barrier_PRICE]));
								}
								else {
									SendNotification(playerid, "No tienes suficiente dinero para pagar el peaje.");
									break;
								}
							}
							
							KillTimer(San_Andreas_Barriers[i][barrier_TIMER]);
							San_Andreas_Barriers[i][barrier_TIMER] = SetTimerEx("CloseBarrier", 5000, false, "i", i);
							MoveDynamicObject(San_Andreas_Barriers[i][barrier_OBJECT_ID], San_Andreas_Barriers[i][barrier_X], San_Andreas_Barriers[i][barrier_Y], San_Andreas_Barriers[i][barrier_Z] + 0.72967 + 0.01, 0.0065, 0.0, 0.0, San_Andreas_Barriers[i][barrier_ROTATION]);
							San_Andreas_Barriers[i][barrier_CLOSING] = false;
							break;
						}
						else break;
					}
				}
			}
			
			if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
			{
				if(GetPlayerInterior(playerid) == 0 && GetPlayerVirtualWorld(playerid) == 0)
				{
					for(new i = 0; i != sizeof POLICE_GARAGE_DOORS; i ++)
					{
						if(IsPlayerInRangeOfPoint(playerid, 20.0, POLICE_GARAGE_DOORS[i][police_gdoor_CLOSED_X], POLICE_GARAGE_DOORS[i][police_gdoor_CLOSED_Y], POLICE_GARAGE_DOORS[i][police_gdoor_CLOSED_Z]))
						{
							if(IsDynamicObjectMoving(POLICE_GARAGE_DOORS[i][police_gdoor_OBJECT_ID]) && !POLICE_GARAGE_DOORS[i][police_gdoor_CLOSING]) break;
							
							KillTimer(POLICE_GARAGE_DOORS[i][police_gdoor_TIMER]);
							POLICE_GARAGE_DOORS[i][police_gdoor_TIMER] = SetTimerEx("ClosePoliceGarageDoor", 5000, false, "i", i);
							MoveDynamicObject(POLICE_GARAGE_DOORS[i][police_gdoor_OBJECT_ID], POLICE_GARAGE_DOORS[i][police_gdoor_OPENED_X], POLICE_GARAGE_DOORS[i][police_gdoor_OPENED_Y], POLICE_GARAGE_DOORS[i][police_gdoor_OPENED_Z], POLICE_GARAGE_DOORS[i][police_gdoor_MOVE_SPEED], POLICE_GARAGE_DOORS[i][police_gdoor_OPENED_RX], POLICE_GARAGE_DOORS[i][police_gdoor_OPENED_RY], POLICE_GARAGE_DOORS[i][police_gdoor_OPENED_RZ]);
							POLICE_GARAGE_DOORS[i][police_gdoor_CLOSING] = false;
							break;
						}
					}
				}
			}
		}
		return 1;
	}
	else if(newkeys & KEY_SECONDARY_ATTACK)
	{
		if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
		{
			if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] != -1)
			{
				if(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] == INTERIOR_CLUB)
				{
					if(IsPlayerInRangeOfPoint(playerid, 1.5, 1208.744140, -34.261119, 1000.953125) || IsPlayerInRangeOfPoint(playerid, 1.5, 1213.738037, -33.667881, 1000.953125))
                    {
                        if(PI[playerid][ac_CASH] <= 25) return SendNotification(playerid, "Digamos que no tienes mucho dinero como para estar gastándotelo en esto...");
						
						if(GivePlayerCash(playerid, -random(25), false, true)) {
							ApplyAnimation(playerid, "STRIP", RandomPayStripAnimations[random(sizeof(RandomPayStripAnimations))], 4.1, 0, 0, 0, 0, 0);
						}
                        return 1;
                    }
				}
			}
	
			if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
			{
				for(new i = 0; i != sizeof POLICE_DOORS; i ++)
				{
					if(GetPlayerInterior(playerid) != POLICE_DOORS[i][police_door_INTERIOR] || GetPlayerVirtualWorld(playerid) != POLICE_DOORS[i][police_door_WORLD]) continue;
					if(IsDynamicObjectMoving(POLICE_DOORS[i][police_door_OBJECT_ID]) || !POLICE_DOORS[i][police_door_CLOSED]) continue;
					
					new Float:pos[4];
					
					pos[0] = POLICE_DOORS[i][police_door_X] + (1.0 * floatsin(-POLICE_DOORS[i][police_door_RZ], degrees));
					pos[1] = POLICE_DOORS[i][police_door_Y] + (1.0 * floatcos(-POLICE_DOORS[i][police_door_RZ], degrees));
					pos[2] = POLICE_DOORS[i][police_door_X] + (-1.0 * floatsin(-POLICE_DOORS[i][police_door_RZ], degrees));
					pos[3] = POLICE_DOORS[i][police_door_Y] + (-1.0 * floatcos(-POLICE_DOORS[i][police_door_RZ], degrees));
					
					if(IsPlayerInRangeOfPoint(playerid, 1.8, pos[0], pos[1], POLICE_DOORS[i][police_door_Z]))
					{
						KillTimer(POLICE_DOORS[i][police_door_TIMER]);
						POLICE_DOORS[i][police_door_TIMER] = SetTimerEx("ClosePoliceDoor", 3000, false, "i", i);
						MoveDynamicObject(POLICE_DOORS[i][police_door_OBJECT_ID], POLICE_DOORS[i][police_door_X], POLICE_DOORS[i][police_door_Y], POLICE_DOORS[i][police_door_Z] + 0.01, 0.0125, 0.0, 0.0, POLICE_DOORS[i][police_door_RZ] - 85.0);
						POLICE_DOORS[i][police_door_CLOSED] = false;
						break;
					}
					else if(IsPlayerInRangeOfPoint(playerid, 1.8, pos[2], pos[3], POLICE_DOORS[i][police_door_Z]))
					{
						KillTimer(POLICE_DOORS[i][police_door_TIMER]);
						POLICE_DOORS[i][police_door_TIMER] = SetTimerEx("ClosePoliceDoor", 3000, false, "i", i);
						MoveDynamicObject(POLICE_DOORS[i][police_door_OBJECT_ID], POLICE_DOORS[i][police_door_X], POLICE_DOORS[i][police_door_Y], POLICE_DOORS[i][police_door_Z] + 0.01, 0.0125, 0.0, 0.0, POLICE_DOORS[i][police_door_RZ] + 85.0);
						POLICE_DOORS[i][police_door_CLOSED] = false;
						break;
					}
				}
			}
		}
		return 1;
	}
	else if(newkeys & KEY_NO)
	{
		if(IsPlayerInDynamicArea(playerid, Lumberjack_Area))
		{
			if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
			{
				if(PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_LUMBERJACK)
				{
					if(PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] == -1)
					{
						if(PLAYER_TEMP[playerid][pt_PLAYER_CARRYING_TREE]) return SendClientMessage(playerid,-1, "{"#SILVER_COLOR"}Lleva la leña cortada al punto marcado para talar más árboles.");
						
						for(new i = 0; i != sizeof LUMBER_TREES; i ++)
						{
							if(IsPlayerInRangeOfPoint(playerid, 2.0, LUMBER_TREES[i][lumber_tree_X], LUMBER_TREES[i][lumber_tree_Y], LUMBER_TREES[i][lumber_tree_Z]))
							{
								if(LUMBER_TREES[i][lumber_tree_STATE] == LUMBER_TREE_STATE_NORMAL)
								{
									TogglePlayerControllableEx(playerid, false);
									SetCameraBehindPlayer(playerid);
									ApplyAnimation(playerid, "CHAINSAW", "WEAPON_csaw", 4.1, true, false, false, false, 0);
									LUMBER_TREES[i][lumber_tree_STATE] = LUMBER_TREE_STATE_CUTTING;
									PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] = i;
									
									RemovePlayerAttachedObject(playerid, 9);
									SetPlayerAttachedObject(playerid, 9, 341, 6);
									SetPlayerArmedWeapon(playerid, 0);
									
									new label_str[256];
									format(label_str, sizeof label_str, "{f46530}Árbol (%d)\n\n{FFFFFF}Talando... {"#SILVER_COLOR"}(%.1f%%)", PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE], 0.0);
									UpdateDynamic3DTextLabelText(LUMBER_TREES[ PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] ][lumber_tree_LABEL], -1, label_str);
									SetPlayerKeyPress(playerid, minrand(30, 50));
								}
								return Y_HOOKS_BREAK_RETURN_1;
							}
						}
					}
				}
			}
		}
		return 1;
	}
	
	else if(newkeys == KEY_WALK && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && TieneRoca[playerid] != 0 && EnProcesadora(playerid))
		{
			new ganancia;
			ganancia = GetPrecioRoca(TieneRoca[playerid]);
			GivePlayerCash(playerid, ganancia, true, false);
			PLAYER_WORKS[playerid][WORK_MINERO][pwork_LEVEL] ++;
			SendNotification(playerid, "+1 EXP en habilidad de minero.");
			new stringD[120];
			format(stringD, sizeof(stringD), "~g~+%d$", ganancia);		
			GameTextForPlayer(playerid, stringD, 1000, 1);
			TieneRoca[playerid] = 0;
			ApplyAnimation(playerid,"CARRY","putdwn",2,0,1,1,0,0);
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
			RemovePlayerAttachedObject(playerid, 5);
			return 1;
		}
	
	else if(newkeys & KEY_YES) {
		switch(GetPlayerState(playerid)) {
			case PLAYER_STATE_DRIVER: {
				new vehicleid = GetPlayerVehicleID(playerid);

				if(!GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE]) {
					if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK)
					{
						if(TRUCK_VEHICLE[vehicleid][truck_vehicle_LOADING]) return SendNotification(playerid, "Debes esperar a que se cargue el vehículo para arrancar.");
						if(TRUCK_VEHICLE[vehicleid][truck_vehicle_UNLOADING]) return SendNotification(playerid, "Debes esperar a que se descargue el vehículo para arrancar.");
					}
					
					if(PLAYER_TEMP[playerid][pt_IN_TUNING_GARAGE]) return SendNotification(playerid, "Termina primero.");
				}
				
				KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][7]);
				if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE])
				{
					PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_IMMUNITY] = gettime() + 15;
					GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
					UpdateVehicleParams(vehicleid);
					
					if(PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID]) Auto_SendPlayerAction(playerid, "ha detenido su vehículo.");
					else Auto_SendPlayerAction(playerid, "ha detenido el vehículo.");
				}
				else
				{
					if(PLAYER_VEHICLES[vehicleid][player_vehicle_VALID] && PLAYER_VEHICLES[vehicleid][player_vehicle_CLAMP] > 0) {
						SendNotification(playerid, "El vehículo tiene un cepo y no puede arrancar.");
						return 1;
					}
					SendNotification(playerid, "Encendiendo...");
					PLAYER_TEMP[playerid][pt_TIMERS][7] = SetTimerEx("StartVehicleEngine", 1000, false, "ii", playerid, vehicleid);
				}
			}
			case PLAYER_STATE_ONFOOT: {
				if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return 1;
				if(PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID] == 0) return 1;
					
				new info[3];
				new Float:pos[3];
				Streamer_GetArrayData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
				if(info[0] == PICKUP_TYPE_NONE) return 1;

				Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_X, pos[0]);
				Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Y, pos[1]);
				Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Z, pos[2]);

				if(!IsPlayerInRangeOfPoint(playerid, 1.0, pos[0], pos[1], pos[2])) return 1;

				switch(info[0])
				{
					case PICKUP_TYPE_ENTER_EXIT:
					{
						PLAYER_TEMP[playerid][pt_HOSPITAL] = GetNearestHospitalForPlayer(playerid);
				
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
						else if(info[2] == 2) // Está en el Pickup Exterior y quiere ir al interior
						{
							if(ENTER_EXIT[info[1]][ee_TIME_OPEN] == 0 && ENTER_EXIT[info[1]][ee_TIME_CLOSE] == 0) // 24 horas
							{
								ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_INTERIOR;
								PI[playerid][ac_LOCAL_INTERIOR] = ENTER_EXIT[info[1]][ee_ID];
								PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] = info[1];
								SetPlayerPosEx(playerid, ENTER_EXIT[info[1]][ee_INT_X], ENTER_EXIT[info[1]][ee_INT_Y], ENTER_EXIT[info[1]][ee_INT_Z], ENTER_EXIT[info[1]][ee_INT_ANGLE], ENTER_EXIT[info[1]][ee_INT_INTERIOR], ENTER_EXIT[info[1]][ee_INT_WORLD], false /*ENTER_EXIT[info[1]][ee_INT_FREEZE]*/, true);
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
									SetPlayerPosEx(playerid, ENTER_EXIT[info[1]][ee_INT_X], ENTER_EXIT[info[1]][ee_INT_Y], ENTER_EXIT[info[1]][ee_INT_Z], ENTER_EXIT[info[1]][ee_INT_ANGLE], ENTER_EXIT[info[1]][ee_INT_INTERIOR], ENTER_EXIT[info[1]][ee_INT_WORLD], false /*ENTER_EXIT[info[1]][ee_INT_FREEZE]*/, true);
									FreezePlayer(playerid);
									CallLocalFunction("OnPlayerEnterInterior", "ii", playerid, info[1]);

									if(ENTER_EXIT[info[1]][ee_INTERIOR_TYPE] == INTERIOR_CLUB) PlayAudioStreamForPlayerEx(playerid, "https://www.dropbox.com/s/yeuwio26i7y00ly/striptease.mp3?dl=1");							
								}
								else {
									if(ENTER_EXIT[info[1]][ee_TIME_OPEN] == -1 && ENTER_EXIT[info[1]][ee_TIME_CLOSE] == -1) SendFormatNotification(playerid, "~r~%s abrirá próximamente.", ENTER_EXIT[info[1]][ee_NAME]);
									else SendFormatNotification(playerid, "~r~%s ~w~está cerrado, su horario es: ~r~%02d:00h ~w~- ~r~%02d:00h~w~.", ENTER_EXIT[info[1]][ee_NAME], ENTER_EXIT[info[1]][ee_TIME_OPEN], ENTER_EXIT[info[1]][ee_TIME_CLOSE]);
								}
							}
						}
						else return 1;
					}
					case PICKUP_TYPE_PROPERTY:
					{
						PLAYER_TEMP[playerid][pt_HOSPITAL] = GetNearestHospitalForPlayer(playerid);
						
						if(info[2] == 1) // Está en el Pickup Interior y quiere ir al exterior
						{
							ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_NORMAL;
							PI[playerid][ac_LOCAL_INTERIOR] = 0;
							PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] = -1;
							SetPlayerPosEx(playerid, PROPERTY_INFO[info[1]][property_EXT_X], PROPERTY_INFO[info[1]][property_EXT_Y], PROPERTY_INFO[info[1]][property_EXT_Z], PROPERTY_INFO[info[1]][property_EXT_ANGLE], PROPERTY_INFO[info[1]][property_EXT_INTERIOR], 0, false /*PROPERTY_INFO[info[1]][property_EXT_FREEZE]*/, false);
							SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
							StopAudioStreamForPlayer(playerid);
							FreezePlayer(playerid);
						}
						else if(info[2] == 2) // Está en el Pickup Exterior y quiere ir al interior
						{
							if(PROPERTY_INFO[info[1]][property_POLICE_FORCED] && (PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET] && PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_POLICE)) {
								new Float:z_pos = PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Z];
								if(PROPERTY_INFO[info[1]][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
								SetPlayerPosEx(playerid, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_X], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Y], z_pos, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_ANGLE], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_INTERIOR], PROPERTY_INFO[info[1]][property_ID], false /*PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_FREEZE]*/, true);
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
								SetPlayerPosEx(playerid, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_X], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Y], z_pos, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_ANGLE], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_INTERIOR], PROPERTY_INFO[info[1]][property_ID], false /*PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_FREEZE]*/, true);
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
									SetPlayerPosEx(playerid, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_X], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_Y], z_pos, PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_ANGLE], PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_INTERIOR], PROPERTY_INFO[info[1]][property_ID], false /*PROPERTY_INTERIORS[ PROPERTY_INFO[info[1]][property_ID_INTERIOR] ][property_INT_FREEZE]*/, true);
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
						else return 1;
					}
					default: return 1;
				}
			}
		}
	}
	return 1;
}

forward ClosePoliceDoor(i);
public ClosePoliceDoor(i)
{
	POLICE_DOORS[i][police_door_CLOSED] = true;
	MoveDynamicObject(POLICE_DOORS[i][police_door_OBJECT_ID], POLICE_DOORS[i][police_door_X], POLICE_DOORS[i][police_door_Y], POLICE_DOORS[i][police_door_Z], 0.0125, 0.0, 0.0, POLICE_DOORS[i][police_door_RZ]);
	return 1;
}

forward ClosePoliceGarageDoor(index);
public ClosePoliceGarageDoor(index)
{
	POLICE_GARAGE_DOORS[index][police_gdoor_CLOSING] = true;
	MoveDynamicObject(POLICE_GARAGE_DOORS[index][police_gdoor_OBJECT_ID], POLICE_GARAGE_DOORS[index][police_gdoor_CLOSED_X], POLICE_GARAGE_DOORS[index][police_gdoor_CLOSED_Y], POLICE_GARAGE_DOORS[index][police_gdoor_CLOSED_Z], POLICE_GARAGE_DOORS[index][police_gdoor_MOVE_SPEED], POLICE_GARAGE_DOORS[index][police_gdoor_CLOSED_RX], POLICE_GARAGE_DOORS[index][police_gdoor_CLOSED_RY], POLICE_GARAGE_DOORS[index][police_gdoor_CLOSED_RZ]);
	return 1;
}

forward CloseBarrier(i);
public CloseBarrier(i)
{
	San_Andreas_Barriers[i][barrier_CLOSING] = true;
	MoveDynamicObject(San_Andreas_Barriers[i][barrier_OBJECT_ID], San_Andreas_Barriers[i][barrier_X], San_Andreas_Barriers[i][barrier_Y], San_Andreas_Barriers[i][barrier_Z] + 0.72967, 0.0065, 0.0, -90.0, San_Andreas_Barriers[i][barrier_ROTATION]);
	return 1;
}

public OnPlayerUpdate(playerid)
{

	if(GetPlayerInterior(playerid) == 66 && GetPlayerVirtualWorld(playerid) == 66) SetPlayerMyTime(playerid, 0, 0);
	else SetPlayerMyTime(playerid, GetMyWorldHour(), 0);
	if(PLAYER_TEMP[playerid][pt_KICKED] || PLAYER_TEMP[playerid][pt_GAME_STATE] != GAME_STATE_NORMAL || ACCOUNT_INFO[playerid][ac_ID] <= 0 || ACCOUNT_INFO[playerid][ac_LEVEL] <= 0) return 0;
	
	new player_state = GetPlayerState(playerid);
	if(player_state == PLAYER_STATE_SPAWNED || player_state == PLAYER_STATE_SPECTATING) return 1;
	
	new player_action = GetPlayerSpecialAction(playerid), Float:player_health, Float:player_armour, player_weapon = GetPlayerWeapon(playerid), current_gettime = gettime();
	GetPlayerHealth(playerid, player_health);
	GetPlayerArmour(playerid, player_armour);
	if(player_weapon) CheckWeaponAndAmmo(playerid, player_weapon);
	
	if(ac_Info[CHEAT_PLAYER_HEALTH][ac_Enabled])
	{
		if(player_health > PI[playerid][ac_HEALTH] + 5.0)
		{
			if(current_gettime > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_HEALTH][p_ac_info_IMMUNITY])
			{
				if(!ac_Info[CHEAT_PLAYER_HEALTH][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_HEALTH);
				else
				{
					if(current_gettime - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_HEALTH][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_HEALTH][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_HEALTH][p_ac_info_DETECTIONS] = 0;
					else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_HEALTH][p_ac_info_DETECTIONS] ++;
					
					PLAYER_AC_INFO[playerid][CHEAT_PLAYER_HEALTH][p_ac_info_LAST_DETECTION] = current_gettime;
					if(PLAYER_AC_INFO[playerid][CHEAT_PLAYER_HEALTH][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_HEALTH][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_HEALTH);
				}
			}
		}
	}
	
	if(ac_Info[CHEAT_PLAYER_ARMOUR][ac_Enabled])
	{
		if(player_armour > PI[playerid][ac_ARMOUR] + 5.0)
		{
			if(current_gettime > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_ARMOUR][p_ac_info_IMMUNITY])
			{
				if(!ac_Info[CHEAT_PLAYER_ARMOUR][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_ARMOUR);
				else
				{
					if(current_gettime - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_ARMOUR][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_ARMOUR][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_ARMOUR][p_ac_info_DETECTIONS] = 0;
					else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_ARMOUR][p_ac_info_DETECTIONS] ++;
					
					PLAYER_AC_INFO[playerid][CHEAT_PLAYER_ARMOUR][p_ac_info_LAST_DETECTION] = current_gettime;
					if(PLAYER_AC_INFO[playerid][CHEAT_PLAYER_ARMOUR][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_ARMOUR][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_ARMOUR);
				}
			}
		}
	}
	
	if(ac_Info[CHEAT_JETPACK][ac_Enabled])
	{
		if(player_action == SPECIAL_ACTION_USEJETPACK)
		{
			if(current_gettime > PLAYER_AC_INFO[playerid][CHEAT_JETPACK][p_ac_info_IMMUNITY])
			{
				if(!ac_Info[CHEAT_JETPACK][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_JETPACK);
				else
				{
					if(current_gettime - PLAYER_AC_INFO[playerid][CHEAT_JETPACK][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_JETPACK][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_JETPACK][p_ac_info_DETECTIONS] = 0;
					else PLAYER_AC_INFO[playerid][CHEAT_JETPACK][p_ac_info_DETECTIONS] ++;
					
					PLAYER_AC_INFO[playerid][CHEAT_JETPACK][p_ac_info_LAST_DETECTION] = current_gettime;
					if(PLAYER_AC_INFO[playerid][CHEAT_JETPACK][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_JETPACK][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_JETPACK);
				}
			}
		}
	}
	
	new Float:dis, Float:vehicle_health, vehicleid = GetPlayerVehicleID(playerid);
	if(!vehicleid) dis = GetPlayerDistanceFromPoint(playerid, PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z]);
	else
	{
		dis = GetVehicleDistanceFromPoint(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][0], GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][1], GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][2]);
		GetVehicleHealth(vehicleid, vehicle_health);
	}
	
	if(ac_Info[CHEAT_UNOCCUPIED_VEHICLE_TP][ac_Enabled] && vehicleid && player_state == PLAYER_STATE_PASSENGER)
	{
		if(floatabs(dis) > 20.0)
		{
			if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] == INVALID_PLAYER_ID)
			{
				GLOBAL_VEHICLES[vehicleid][gb_vehicle_TP_IMMUNITY] = current_gettime + 5;
			}
		}
	}
	
	if(ac_Info[CHEAT_HIGH_POS][ac_Enabled])
	{
		if(current_gettime > PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY])
		{
			if(floatabs(dis) > 80.0 && PI[playerid][ac_POS_Z] > -97.0)
			{
				if(player_state != PLAYER_STATE_PASSENGER)
				{
					if(!ac_Info[CHEAT_HIGH_POS][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_HIGH_POS, dis);
					else
					{
						if(current_gettime - PLAYER_AC_INFO[playerid][CHEAT_HIGH_POS][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_HIGH_POS][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_HIGH_POS][p_ac_info_DETECTIONS] = 0;
						else PLAYER_AC_INFO[playerid][CHEAT_HIGH_POS][p_ac_info_DETECTIONS] ++;
						
						PLAYER_AC_INFO[playerid][CHEAT_HIGH_POS][p_ac_info_LAST_DETECTION] = current_gettime;
						if(PLAYER_AC_INFO[playerid][CHEAT_HIGH_POS][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_HIGH_POS][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_HIGH_POS, dis);
					}
				}
			}
		}
	}

	switch(player_state)
	{
		case PLAYER_STATE_ONFOOT:
		{
			if(ac_Info[CHEAT_PLAYER_SPEED][ac_Enabled])
			{
				if(current_gettime > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_SPEED][p_ac_info_IMMUNITY])
				{
					new speed = GetPlayerSpeed(playerid);
					if(speed > 1000)
					{
						if(!ac_Info[CHEAT_PLAYER_SPEED][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_SPEED, speed);
						else
						{
							if(current_gettime - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_SPEED][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_SPEED][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_SPEED][p_ac_info_DETECTIONS] = 0;
							else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_SPEED][p_ac_info_DETECTIONS] ++;
							
							PLAYER_AC_INFO[playerid][CHEAT_PLAYER_SPEED][p_ac_info_LAST_DETECTION] = current_gettime;
							if(PLAYER_AC_INFO[playerid][CHEAT_PLAYER_SPEED][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_SPEED][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_SPEED, speed);
						}
					}
				}
			}
			
			if(ac_Info[CHEAT_POS][ac_Enabled])
			{
				if(current_gettime > PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY])
				{
					if(GetPlayerSurfingVehicleID(playerid) == INVALID_VEHICLE_ID && GetPlayerSurfingObjectID(playerid) == INVALID_OBJECT_ID && player_action != SPECIAL_ACTION_ENTER_VEHICLE && player_action != SPECIAL_ACTION_EXIT_VEHICLE)
					{
						if(floatabs(dis) > 6.0 && PI[playerid][ac_POS_Z] > -97.0)
						{
							if(!ac_Info[CHEAT_POS][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_POS, dis);
							else
							{
								if(current_gettime - PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_POS][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_DETECTIONS] = 0;
								else PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_DETECTIONS] ++;
								
								PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_LAST_DETECTION] = current_gettime;
								if(PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_POS][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_POS, dis);
							}
						}
					}
				}
			}
		}
		case PLAYER_STATE_DRIVER:
		{	
			new Float:vspeed = GetVehicleSpeed(vehicleid);
			
			if(ac_Info[CHEAT_VEHICLE_SPEED_HACK][ac_Enabled])
			{
				if(current_gettime > PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_IMMUNITY])
				{
					if(vspeed > 1000.0)
					{
						if(!ac_Info[CHEAT_VEHICLE_SPEED_HACK][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_VEHICLE_SPEED_HACK);
						else
						{
							if(current_gettime - PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_VEHICLE_SPEED_HACK][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_DETECTIONS] = 0;
							else PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_DETECTIONS] ++;
							
							PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_LAST_DETECTION] = current_gettime;
							if(PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_VEHICLE_SPEED_HACK][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_VEHICLE_SPEED_HACK);
						}
					}
				}
			}
			
			if(ac_Info[CHEAT_VEHICLE_HEALTH][ac_Enabled])
			{
				if(vehicle_health > (GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] + 20.0))
				{
					if(current_gettime > PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_HEALTH][p_ac_info_IMMUNITY])
					{						
						if(!ac_Info[CHEAT_VEHICLE_HEALTH][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_VEHICLE_HEALTH, (GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] + 5.0) - vehicle_health);
						else
						{
							if(current_gettime - PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_HEALTH][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_VEHICLE_HEALTH][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_HEALTH][p_ac_info_DETECTIONS] = 0;
							else PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_HEALTH][p_ac_info_DETECTIONS] ++;
							
							PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_HEALTH][p_ac_info_LAST_DETECTION] = current_gettime;
							if(PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_HEALTH][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_VEHICLE_HEALTH][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_VEHICLE_HEALTH);
						}
					}
				}
			}
			
			if(ac_Info[CHEAT_POS][ac_Enabled])
			{
				if(current_gettime > PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY])
				{
					if(vspeed < 20.0 && floatabs(dis) >= 10.0 && GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][2] > -97.0)
					{
						if(!ac_Info[CHEAT_POS][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_POS, dis);
						else
						{
							if(current_gettime - PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_POS][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_DETECTIONS] = 0;
							else PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_DETECTIONS] ++;
							
							PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_LAST_DETECTION] = current_gettime;
							if(PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_POS][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_POS, dis);
						}
					}
				}
			}
			
			if(ac_Info[CHEAT_VEHICLE_NOFUEL][ac_Enabled])
			{
				new Keys, ud, lr;
				GetPlayerKeys(playerid, Keys, ud, lr);
				if(Keys == KEY_SPRINT)
				{
					if(!GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] && vspeed > 60.0 && GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][2] > -97.0 && VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_ENGINE])
					{
						if(current_gettime > PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_IMMUNITY])
						{
							if(!ac_Info[CHEAT_VEHICLE_NOFUEL][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_VEHICLE_NOFUEL);
							else
							{
								if(current_gettime - PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_VEHICLE_NOFUEL][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_DETECTIONS] = 0;
								else PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_DETECTIONS] ++;
								
								PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_LAST_DETECTION] = current_gettime;
								if(PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_VEHICLE_NOFUEL][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_VEHICLE_NOFUEL);
							}
						}
					}
				}
			}
		}
	}
	
	PI[playerid][ac_HEALTH] = player_health;
	PI[playerid][ac_ARMOUR] = player_armour;
	GetPlayerPos(playerid, PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z]);
	if(vehicleid)
	{
		GetVehiclePos(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][0], GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][1], GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][2]);
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] = vehicle_health;
	}
	return 1;
}

forward TogglePlayerControl(playerid, bool:controlable);
public TogglePlayerControl(playerid, bool:controlable)
{
	return TogglePlayerControllableEx(playerid, controlable);
}

forward HealthUp(playerid);
public HealthUp(playerid)
{
	if(ACCOUNT_INFO[playerid][ac_STATE] != ROLEPLAY_STATE_HOSPITAL) return 1;
	
	if(PLAYER_TEMP[playerid][pt_HOSPITAL_LIFE] < 100.0)
	{
		if(ACCOUNT_INFO[playerid][ac_VIP]) PLAYER_TEMP[playerid][pt_HOSPITAL_LIFE] += 10.0;
		else PLAYER_TEMP[playerid][pt_HOSPITAL_LIFE] += 5.0;
		
		if(PLAYER_TEMP[playerid][pt_HOSPITAL_LIFE] > 100.0) PLAYER_TEMP[playerid][pt_HOSPITAL_LIFE] = 100.0;
		UpdateHospitalSizeTextdrawLife(playerid);
		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][4]);
		PLAYER_TEMP[playerid][pt_TIMERS][4] = SetTimerEx("HealthUp", 1000, false, "i", playerid);
	}
	else
	{
		if(ACCOUNT_INFO[playerid][ac_VIP]) PI[playerid][ac_HEALTH] = 100.0;
		else PI[playerid][ac_HEALTH] = 50.0;
		
		SetPlayerHealthEx(playerid, PI[playerid][ac_HEALTH]);
		
		ClearPlayerChat(playerid);
		SendNotification(playerid, "Te has recuperado.");
		new price = ACCOUNT_INFO[playerid][ac_LEVEL] * 200;
		if(price > 1200) price = 1200;
		price += minrand(100, 200);
		
		if(price > PI[playerid][ac_CASH])
		{
			price = PI[playerid][ac_CASH];
			if(price < 0) price = 0;
		}

		SendFormatNotification(playerid, "El coste de los servicios médicos ofrecidos es de ~r~%s dólares~w~.", number_format_thousand(price));
		GivePlayerCash(playerid, -price, false, true);
		PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0]);
		PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1]);
		PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2]);
		PI[playerid][ac_LOCAL_INTERIOR] = 0;
		PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] = -1;
		
		if(PI[playerid][ac_WANTED_LEVEL] > 0 || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED)
		{
			ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_JAIL;
			if(ACCOUNT_INFO[playerid][ac_VIP]) PI[playerid][ac_POLICE_JAIL_TIME] = 150 * PI[playerid][ac_WANTED_LEVEL];
			else PI[playerid][ac_POLICE_JAIL_TIME] = 300 * PI[playerid][ac_WANTED_LEVEL];
			PI[playerid][ac_WANTED_LEVEL] = 0;
			PI[playerid][ac_POLICE_JAIL_ID] = 0;
			PLAYER_TEMP[playerid][pt_ENTER_JAIL_TIME] = gettime();
			SetSpawnInfo(playerid, NO_TEAM, PI[playerid][ac_SKIN], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID] ][jail_X], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_Y], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_Z], JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_ANGLE], 0, 0, 0, 0, 0, 0);
			PI[playerid][ac_INTERIOR] = JAIL_POSITIONS[ PI[playerid][ac_POLICE_JAIL_ID]  ][jail_INTERIOR];
			SetPlayerVirtualWorld(playerid, 0);
			
			new time = PI[playerid][ac_POLICE_JAIL_TIME] - (gettime() - PLAYER_TEMP[playerid][pt_ENTER_JAIL_TIME]);
			SendFormatNotification(playerid, "Te quedan %s minutos de condena.", TimeConvert(time));
		}
		else
		{
			GetHospitalSpawnPosition(PLAYER_TEMP[playerid][pt_HOSPITAL], PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z], PI[playerid][ac_ANGLE], PI[playerid][ac_INTERIOR], PI[playerid][ac_LOCAL_INTERIOR], ACCOUNT_INFO[playerid][ac_STATE]);
			SetSpawnInfo(playerid, NO_TEAM, PI[playerid][ac_SKIN], PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z], PI[playerid][ac_ANGLE], 0, 0, 0, 0, 0, 0);
		}
		PLAYER_TEMP[playerid][pt_PLAYER_FINISH_HOSPITAL] = true;
		TogglePlayerSpectatingEx(playerid, false);
	}
	return 1;
}

forward HungryThirstDown(playerid);
public HungryThirstDown(playerid)
{
	if(PLAYER_TEMP[playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return HidePlayerHud(playerid);
	if(!PLAYER_TEMP[playerid][pt_HUD_TEXTDRAWS]) return HidePlayerHud(playerid);
	
	new current_gettime = gettime();
	
	if(ACCOUNT_INFO[playerid][ac_VIP])
	{
		if(current_gettime - PLAYER_TEMP[playerid][pt_LAST_SU_CHECK] > 300)
		{
			CheckPlayerSuperUser(playerid);
		}
	}

	if(PI[playerid][ac_WANTED_LEVEL] > 0 && ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_NORMAL && !PLAYER_TEMP[playerid][pt_CUFFED])
	{
		if(gettime() - PLAYER_TEMP[playerid][pt_LAST_SET_WANTED_LEVEL] > 300)
		{
			if(PI[playerid][ac_WANTED_LEVEL] == 1)
			{
				SendFormatNotification(playerid, "Has perdido de vista a la policía.");
				SetPlayerWantedLevelEx(playerid, 0);
			}
			else SetPlayerWantedLevelEx(playerid, PI[playerid][ac_WANTED_LEVEL] - 1);
		}
	}

	PI[playerid][ac_THIRST] -= floatdiv(float(INTERVAL_HUNGRY_THIRST_DOWN), floatmul(THIRST_HOURS_STOPPED, 36));
	PI[playerid][ac_HUNGRY] -= floatdiv(float(INTERVAL_HUNGRY_THIRST_DOWN), floatmul(HUNGRY_HOURS_STOPPED, 36));
	
	if(PI[playerid][ac_THIRST] <= 0.5) PI[playerid][ac_THIRST] = 0.5;
	if(PI[playerid][ac_HUNGRY] <= 0.5) PI[playerid][ac_HUNGRY] = 0.5;
	if(PI[playerid][ac_THIRST] < 20.0)
	{
		PI[playerid][ac_HEALTH] -= 0.2;
		
		if(PI[playerid][ac_THIRST] <= 1.0 || PI[playerid][ac_HEALTH] <= 0.5)
		{
			SendNotification(playerid, "Te has desmayado por deshidratación.");
			SetPlayerHealthEx(playerid, 0.0);
			PI[playerid][ac_THIRST] = 75.0;
			return 1;
		}
		
		SetPlayerHealthEx(playerid, PI[playerid][ac_HEALTH]);
		
		if(!PLAYER_TEMP[playerid][pt_THIRST_MESSAGE])
		{
			SendNotification(playerid, "Tienes sed, si no bebes agua pronto acabarás muriendo.");
			PLAYER_TEMP[playerid][pt_THIRST_MESSAGE] = true;
		}
	}
	
	if(PI[playerid][ac_HUNGRY] < 20.0)
	{
		PI[playerid][ac_HEALTH] -= 0.2;
		
		if(PI[playerid][ac_HUNGRY] <= 1.0 || PI[playerid][ac_HEALTH] <= 0.5)
		{
			SendNotification(playerid, "Te has desmayado por hambre.");
			SetPlayerHealthEx(playerid, 0.0);
			PI[playerid][ac_HUNGRY] = 55.0;
			return 1;
		}
		
		SetPlayerHealthEx(playerid, PI[playerid][ac_HEALTH]);
		
		if(!PLAYER_TEMP[playerid][pt_HUNGRY_MESSAGE])
		{
			SendNotification(playerid, "Tienes hambre, si no comes nada pronto acabarás muriendo.");
			PLAYER_TEMP[playerid][pt_HUNGRY_MESSAGE] = true;
		}
	}
	
	UpdatePlayerHud(playerid);
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(PLAYER_TEMP[playerid][pt_KICKED]) return 1;
	
	if(PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_NORMAL)
	{
		if(newstate != PLAYER_STATE_SPAWNED && oldstate != PLAYER_STATE_SPAWNED)
		{
			if(ac_Info[CHEAT_STATE_SPAMMER][ac_Enabled])
			{
				if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_IMMUNITY])
				{
					new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_STATE]);
					
					if(interval < 50)
					{
						if(!ac_Info[CHEAT_STATE_SPAMMER][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_STATE_SPAMMER, float(interval));
						else
						{
							if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_STATE_SPAMMER][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_DETECTIONS] = 0;
							else PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_DETECTIONS] ++;
							
							PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_LAST_DETECTION] = gettime();
							if(PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_STATE_SPAMMER][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_STATE_SPAMMER, float(interval));
						}
					}
					PLAYER_TEMP[playerid][pt_ANTIFLOOD_STATE] = GetTickCount();
				}
			}
		}
	}
	
    if(newstate == PLAYER_STATE_DRIVER)
    {
		new vehicleid = GetPlayerVehicleID(playerid);
		if(ac_Info[CHEAT_CAR_JACKING][ac_Enabled])
		{
			if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_OCCUPIED] && GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] != INVALID_PLAYER_ID)
			{
				if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_CAR_JACKING][p_ac_info_IMMUNITY])
				{
					if(!ac_Info[CHEAT_CAR_JACKING][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_CAR_JACKING);
					else
					{
						if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_CAR_JACKING][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_CAR_JACKING][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_CAR_JACKING][p_ac_info_DETECTIONS] = 0;
						else PLAYER_AC_INFO[playerid][CHEAT_CAR_JACKING][p_ac_info_DETECTIONS] ++;
						
						PLAYER_AC_INFO[playerid][CHEAT_CAR_JACKING][p_ac_info_LAST_DETECTION] = gettime();
						if(PLAYER_AC_INFO[playerid][CHEAT_CAR_JACKING][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_CAR_JACKING][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_CAR_JACKING);
					}
					
					PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] = INVALID_VEHICLE_ID;
					return 1;
				}
			}
		}
		
		SetPlayerArmedWeapon(playerid, 0);
		PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_HEALTH][p_ac_info_IMMUNITY] = gettime() + 3;
		PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;
		PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] = vehicleid;
		
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] = playerid;
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_DRIVER] = playerid;
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_OCCUPIED] = true;
		if(PI[playerid][ac_DRIVE_LICENSE_POINTS] == 0 && !PLAYER_TEMP[playerid][pt_DL_EXAM] && VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_NORMAL_SPEEDO])
		{
			SendNotification(playerid, "Estás conduciendo sin licencia de conducción, la policía podría multarte.");
		}
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS] && VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_DRIVER_DOOR]) //cerrado
		{
			if(gettime() - GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_CLOSED_TIME] > 5) // Si el tiempo que paso desde que cerro es mayor a 5
			{
				if(ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Enabled])
				{
					if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_IMMUNITY])
					{
						if(!ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_DOORS);
						else
						{
							if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_DETECTIONS] = 0;
							else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_DETECTIONS] ++;
							
							PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_LAST_DETECTION] = gettime();
							if(PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_DOORS);
						}
					}
				}
			}
		}
		
		switch(GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE])
		{
			case VEHICLE_TYPE_NONE:
			{
				if(!VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_ENGINE])
				{
					GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 1;
					UpdateVehicleParams(vehicleid);
				}
			}
			case VEHICLE_TYPE_DRIVING_SCHOOL:
			{
				if(PLAYER_TEMP[playerid][pt_DL_EXAM])
				{
					SendNotification(playerid, "Sigue los puntos de control para completar el examen.");
					SetPlayerDrivingSchoolCP(playerid);
				}
			}
			case VEHICLE_TYPE_SELL:
			{
				PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] = vehicleid;
				new pvehicles = CountPlayerVehicles(playerid);
				if(pvehicles >= MAX_SU_VEHICLES) return SendFormatNotification(playerid, "No puedes comprar más vehículos, el límite es %d.", MAX_SU_VEHICLES);
				if(!ACCOUNT_INFO[playerid][ac_VIP])
				{
					if(pvehicles >= MAX_NU_VEHICLES)
					{
						SendFormatNotification(playerid, "¡Los jugadores ~y~VIP ~w~pueden tener hasta %d vehículos! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP~w~.", MAX_SU_VEHICLES);
						return 1;
					}
				}
				
				ShowDialog(playerid, DIALOG_BUY_VEHICLE);
				return 1;
			}
			case VEHICLE_TYPE_WORK:
			{
				if(!PLAYER_WORKS[playerid][ WORK_VEHICLES[vehicleid][work_vehicle_WORK] ][pwork_SET])
				{
					if(ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Enabled])
					{
						if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_IMMUNITY])
						{
							if(!ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
							else
							{
								if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] = 0;
								else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] ++;
								
								PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] = gettime();
								if(PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
							}
						}
					}
					return 1;
				}
				
				if(PLAYER_TEMP[playerid][pt_WORKING_IN] && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_VEHICLES[vehicleid][work_vehicle_WORK])
				{
					if(ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Enabled])
					{
						if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_IMMUNITY])
						{
							if(!ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
							else
							{
								if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] = 0;
								else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] ++;
								
								PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] = gettime();
								if(PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
							}
						}
					}
					return 1;
				}
				
				if(WORK_VEHICLES[vehicleid][work_vehicle_NEED_DUTY])
				{
					if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_VEHICLES[vehicleid][work_vehicle_WORK])
					{
						if(ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Enabled])
						{
							if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_IMMUNITY])
							{
								if(!ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
								else
								{
									if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] = 0;
									else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] ++;
									
									PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] = gettime();
									if(PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
								}
							}
						}
						return 1;
					}
				}
				
				if(WORK_VEHICLES[vehicleid][work_vehicle_EXP] > PLAYER_WORKS[playerid][ WORK_VEHICLES[vehicleid][work_vehicle_WORK] ][pwork_LEVEL])
				{
					if(ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Enabled])
					{
						if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_IMMUNITY])
						{
							if(!ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
							else
							{
								if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] = 0;
								else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] ++;
								
								PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_LAST_DETECTION] = gettime();
								if(PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_WORK][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_VEHICLE_WORK][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_WORK);
							}
						}
					}
					return 1;
				}
				
				switch(WORK_VEHICLES[vehicleid][work_vehicle_WORK])
				{
					case WORK_TAXI, WORK_HARVESTER, WORK_FUMIGATOR: CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_VEHICLES[vehicleid][work_vehicle_WORK], vehicleid);
					case WORK_TRUCK, WORK_DELIVERYMAN:
					{
						if(TRUCK_VEHICLE[vehicleid][truck_vehicle_LOADED])
						{
							if(TRUCK_VEHICLE[vehicleid][truck_vehicle_DRIVER_USER_ID] != ACCOUNT_INFO[playerid][ac_ID])
							{
								SendNotification(playerid, "No eres el conductor de este vehículo.");
								RemovePlayerFromVehicle(playerid);
								return 1;
							}
							else CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_VEHICLES[vehicleid][work_vehicle_WORK], vehicleid);
						}
						else CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_VEHICLES[vehicleid][work_vehicle_WORK], vehicleid);
					}
					case WORK_TRASH:
					{
						if(TRASH_VEHICLES[vehicleid][trash_vehicle_JOB_STARTED])
						{
							if(TRASH_VEHICLES[vehicleid][trash_vehicle_DRIVER_ID] != playerid)
							{
								SendNotification(playerid, "No eres el conductor de este camión.");
								RemovePlayerFromVehicle(playerid);
								return 1;
							}
							
							SetPlayerTrashCheckpoint(playerid, vehicleid);
						}
						else
						{
							if(PLAYER_TEMP[playerid][pt_TRASH_DRIVER] || PLAYER_TEMP[playerid][pt_TRASH_PASSENGER])
							{
								SendNotification(playerid, "Ya estás trabajando con otro compañero.");
								RemovePlayerFromVehicle(playerid);
								return 1;
							}
							
							PLAYER_TEMP[playerid][pt_TRASH_DRIVER] = true;
							PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] = vehicleid;
							TRASH_VEHICLES[vehicleid][trash_vehicle_DRIVER_ID] = playerid;
							
							if(TRASH_VEHICLES[vehicleid][trash_vehicle_PASSENGER_ID] == INVALID_PLAYER_ID) SendNotification(playerid, "Necesitas un compañero para empezar con el trabajo.");
							else
							{
								new route = random(MAX_ROUTES);
								while(!TRASH_OBJECTS[route][trash_route_VALID])
								{
									route = random(MAX_ROUTES);
								}
								
								TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_JOB_STARTED] = true;
								TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] = route;
								TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_CP] = 0;
								
								SetPlayerTrashCheckpoint(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
								SetPlayerTrashCheckpoint(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
								
								CreatePlayerTrashRouteObjects(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], route);
								CreatePlayerTrashRouteObjects(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], route);
								
								SendFormatNotification(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], "Ruta ~y~#%d ~w~establecida, pueden comenzar.", TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] + 1);
								SendFormatNotification(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], "Ruta ~y~#%d ~w~establecida, pueden comenzar.", TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] + 1);

							}
						}
					}
					case WORK_PIZZA:
					{
						if(PIZZA_VEHICLE[vehicleid][pizza_vehicle_STARTED])
						{
							if(PIZZA_VEHICLE[vehicleid][pizza_vehicle_DRIVER_AID] != ACCOUNT_INFO[playerid][ac_ID])
							{
								if(GetVehicleDistanceFromPoint(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_X], GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Y], GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Z]) < 20.0)
								{
									SetVehicleToRespawnEx(vehicleid);
								}
								
								SendNotification(playerid, "No eres el conductor de esta moto.");
								RemovePlayerFromVehicle(playerid);
								return 1;
							}
							
							if(!PLAYER_TEMP[playerid][pt_PIZZA_PROCCESS])
							{
								SendNotification(playerid, "Ya has repartido todas las pizzas, vuelve a la pizzería para cobrar.");
								SetPlayer_Pizza_Checkpoint(playerid, -1);
							}
							else
							{
								SendFormatNotification(playerid, "Te quedan %d pizzas por repartir.", PLAYER_TEMP[playerid][pt_PIZZA_PROCCESS]);
								if(PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT_DELI])
								{
									PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT] = randomEx(sizeof(PIZZA_DELIVERY_POINT), PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT]);
									PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT_DELI] = false;
								}
								SetPlayer_Pizza_Checkpoint(playerid, PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT]);
							}
						}
						else
						{
							PIZZA_VEHICLE[vehicleid][pizza_vehicle_STARTED] = true;
							PIZZA_VEHICLE[vehicleid][pizza_vehicle_DRIVER_AID] = ACCOUNT_INFO[playerid][ac_ID];
							PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZAS] = minrand(3, 6);
							PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT] = random( sizeof(PIZZA_DELIVERY_POINT) );
							PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT_DELI] = false;
							PLAYER_TEMP[playerid][pt_PIZZA_PROCCESS] = PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZAS];
							PLAYER_TEMP[playerid][pt_DELIVERED_PIZZAS] = 0;
							
							SendFormatNotification(playerid, "Esta moto tiene %d pizzas, repartelas todas y vuelve a la pizzería para cobrar.", PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZAS]);
							SetPlayer_Pizza_Checkpoint(playerid, PIZZA_VEHICLE[vehicleid][pizza_vehicle_PIZZA_POINT]);
						}
					}
				}
			}
		}
		
		if(PLAYER_TEMP[playerid][pt_WANT_TAXI])
		{
			if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] == VEHICLE_STATE_NORMAL)
			{
				PLAYER_TEMP[playerid][pt_WANT_TAXI] = false;
				DisablePlayerTaxiMark(playerid);
				
				SendNotification(playerid, "Tu taxi se ha cancelado porque te has subido a un vehículo.");
			}
		}
    }
	else if(newstate == PLAYER_STATE_PASSENGER)
	{
		SetPlayerArmedWeapon(playerid, 0);
		PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;
		
		new vehicleid = GetPlayerVehicleID(playerid);
		PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] = vehicleid;
		
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS] && VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_PASSENGER_DOOR]) //cerrado
		{
			if(gettime() - GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_CLOSED_TIME] > 5) // Si el tiempo que paso desde que cerro es mayor a 5
			{
				if(ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Enabled])
				{
					if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_IMMUNITY])
					{
						if(!ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_DOORS);
						else
						{
							if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_DETECTIONS] = 0;
							else PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_DETECTIONS] ++;
							
							PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_LAST_DETECTION] = gettime();
							if(PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_PLAYER_VEHICLE_DOORS][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_PLAYER_VEHICLE_DOORS);
						}
					}
				}
			}
		}
		
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK)
		{
			switch(WORK_VEHICLES[vehicleid][work_vehicle_WORK])
			{
				case WORK_TAXI:
				{
					if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] == INVALID_PLAYER_ID) return 1;
					if(TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_ENABLED])
					{
						if(PLAYER_TEMP[playerid][pt_WANT_TAXI])
						{
							PLAYER_TEMP[playerid][pt_WANT_TAXI] = false;
							DisablePlayerTaxiMark(playerid);
						}
						return 1;
					}
					
					ShowPlayerTaxiMeter(playerid);
					if(PLAYER_TEMP[playerid][pt_WANT_TAXI])
					{
						PLAYER_TEMP[playerid][pt_WANT_TAXI] = false;
						DisablePlayerTaxiMark(playerid);
					}
					
					TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_ENABLED] = true;
					GetVehiclePos(vehicleid, TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_OLD_X], TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_OLD_Y], TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_OLD_Z]);
					KillTimer(TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_TIMER]);
					TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_TIMER] = SetTimerEx("UpdateVehicleTaximeter", 4000, true, "iii", vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER], playerid);
				}
				case WORK_TRASH:
				{
					if(!PLAYER_WORKS[playerid][WORK_TRASH][pwork_SET])
					{
						SendFormatNotification(playerid, "Necesitas ser %s para poder conducir este vehículo.", work_info[ WORK_VEHICLES[vehicleid][work_vehicle_WORK] ][work_info_NAME]);
						RemovePlayerFromVehicle(playerid);
						return 1;
					}
					
					if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_TRASH)
					{
						SendNotification(playerid, "No estás de servicio como basurero, ve a vestirte en el puesto de la entrada del vertedero.");
						RemovePlayerFromVehicle(playerid);
						return 1;
					}
					
					if(TRASH_VEHICLES[vehicleid][trash_vehicle_JOB_STARTED])
					{
						if(TRASH_VEHICLES[vehicleid][trash_vehicle_PASSENGER_ID] != playerid)
						{
							SendNotification(playerid, "No eres el acompañante de este camión.");
							RemovePlayerFromVehicle(playerid);
							return 1;
						}
						
						SetPlayerTrashCheckpoint(playerid, vehicleid);
					}
					else
					{
						if(PLAYER_TEMP[playerid][pt_TRASH_DRIVER] || PLAYER_TEMP[playerid][pt_TRASH_PASSENGER])
						{
							SendNotification(playerid, "Ya estás trabajando con otro compañero.");
							RemovePlayerFromVehicle(playerid);
							return 1;
						}
						
						PLAYER_TEMP[playerid][pt_TRASH_PASSENGER] = true;
						PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] = vehicleid;
						TRASH_VEHICLES[vehicleid][trash_vehicle_PASSENGER_ID] = playerid;
						
						if(TRASH_VEHICLES[vehicleid][trash_vehicle_DRIVER_ID] == INVALID_PLAYER_ID) SendNotification(playerid, "Necesitas un compañero para empezar con el trabajo.");
						else
						{
							new route = random(MAX_ROUTES);
							while(!TRASH_OBJECTS[route][trash_route_VALID])
							{
								route = random(MAX_ROUTES);
							}
							
							TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_JOB_STARTED] = true;
							TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] = route;
							TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_CP] = 0;
							
							SetPlayerTrashCheckpoint(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
							SetPlayerTrashCheckpoint(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
							
							CreatePlayerTrashRouteObjects(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], route);
							CreatePlayerTrashRouteObjects(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], route);
							
							SendFormatNotification(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], "Ruta ~y~#%d ~w~establecida, pueden comenzar.", TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] + 1);
							SendFormatNotification(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], "Ruta ~y~#%d ~w~establecida, pueden comenzar.", TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] + 1);

						}
					}
				}
			}
		}
		
		if(PLAYER_TEMP[playerid][pt_WANT_TAXI])
		{
			if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] == VEHICLE_STATE_NORMAL)
			{
				PLAYER_TEMP[playerid][pt_WANT_TAXI] = false;
				DisablePlayerTaxiMark(playerid);
				
				SendNotification(playerid, "Tu taxi se ha cancelado porque te has subido a un vehículo.");
			}
		}
	}
	else if(newstate == PLAYER_STATE_SPECTATING)
	{
		if(!PLAYER_TEMP[playerid][pt_PLAYER_SPECTATE])
		{
			if(ac_Info[CHEAT_SPECTATE][ac_Enabled])
			{
				if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_SPECTATE][p_ac_info_IMMUNITY])
				{
					if(!ac_Info[CHEAT_SPECTATE][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_SPECTATE);
					else
					{
						if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_SPECTATE][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_SPECTATE][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_SPECTATE][p_ac_info_DETECTIONS] = 0;
						else PLAYER_AC_INFO[playerid][CHEAT_SPECTATE][p_ac_info_DETECTIONS] ++;
						
						PLAYER_AC_INFO[playerid][CHEAT_SPECTATE][p_ac_info_LAST_DETECTION] = gettime();
						if(PLAYER_AC_INFO[playerid][CHEAT_SPECTATE][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_SPECTATE][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_SPECTATE);
					}
				}
			}
		}
	}
	
	if(oldstate == PLAYER_STATE_DRIVER)
	{
		PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;
		if(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] != INVALID_VEHICLE_ID)
		{
			KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][7]);
			
			if(IsValidDynamicRaceCP(PLAYER_TEMP[playerid][pt_DL_EXAM_CP]))
			{
				DestroyDynamicRaceCP(PLAYER_TEMP[playerid][pt_DL_EXAM_CP]);
				PLAYER_TEMP[playerid][pt_DL_EXAM_CP] = INVALID_STREAMER_ID;
			}
			
			if(GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_ATTACHED_TO] != INVALID_VEHICLE_ID)
			{
				if(GLOBAL_VEHICLES[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_ATTACHED_TO] ][gb_vehicle_DRIVER] != INVALID_PLAYER_ID)
				{
					PLAYER_AC_INFO[ GLOBAL_VEHICLES[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_ATTACHED_TO] ][gb_vehicle_DRIVER] 	][CHEAT_FAKE_TRAILER_SYNC][p_ac_info_IMMUNITY] = gettime() + 5;
				}
				GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_ATTACHED_TO] = INVALID_VEHICLE_ID;
				DetachTrailerFromVehicle(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID]);
			}
			
			GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_TP_IMMUNITY] = gettime() + 5;
			GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_DRIVER] = INVALID_PLAYER_ID;
			GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_LAST_DRIVER] = playerid;
			GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_OCCUPIED] = false;
			
			StopAudioStreamForPlayer(playerid);
			
			if(GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK)
			{
				switch(WORK_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][work_vehicle_WORK])
				{
					case WORK_TAXI, WORK_HARVESTER, WORK_FUMIGATOR, WORK_TRUCK: CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
					case WORK_PIZZA:
					{
						if(PIZZA_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][pizza_vehicle_STARTED])
						{
							if(PLAYER_TEMP[playerid][pt_PIZZA_PROCCESS])
							{
								new Float:distance =	GetPlayerDistanceFromPoint
																(
																	playerid,
																	PIZZA_DELIVERY_POINT[ PIZZA_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][pizza_vehicle_PIZZA_POINT] ][0],
																	PIZZA_DELIVERY_POINT[ PIZZA_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][pizza_vehicle_PIZZA_POINT] ][1],
																	PIZZA_DELIVERY_POINT[ PIZZA_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][pizza_vehicle_PIZZA_POINT] ][2]
																);
										
								if(distance > 20.0)
								{
									SendNotification(playerid, "Vuelve a subir a la moto, aún estás muy lejos del cliente.");
									if(IsValidDynamicActor(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR]))
									{
										DestroyDynamicActor(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR]);
										PLAYER_TEMP[playerid][pt_PIZZA_ACTOR] = INVALID_STREAMER_ID;
									}
									if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]))
									{
										DestroyDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]);
										PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT] = INVALID_STREAMER_ID;
									}
								}
								else
								{
									if(IsPlayerInDynamicCP(playerid, PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]))
									{
										PLAYER_TEMP[playerid][pt_PIZZA_PROCCESS] --;
										PLAYER_TEMP[playerid][pt_DELIVERED_PIZZAS] ++;
										RemovePlayerAttachedObject(playerid, 9);
										SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
										ApplyDynamicActorAnimation(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR], "FOOD", "EAT_Pizza", 4.1, false, true, true, false, 0);
										ApplyDynamicActorAnimation(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR], "FOOD", "EAT_Pizza", 4.1, false, true, true, false, 0);
										DestroyDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]);
										PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT] = INVALID_STREAMER_ID;
										PIZZA_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][pizza_vehicle_PIZZA_POINT_DELI] = true;
										SendNotification(playerid, "Genial, vuelve a subir a la moto para continuar.");
									}
									else
									{
										SetPlayerArmedWeapon(playerid, 0);
										SetPlayerAttachedObject(playerid, 9, 1582, 6, 0.094, 0.077999, -0.165999, 72.0, 8.799999, 9.899992, 0.73, 0.774999, 1.0);
										SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
									}
								}
							}
							else
							{
								if(IsValidDynamicActor(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR]))
								{
									DestroyDynamicActor(PLAYER_TEMP[playerid][pt_PIZZA_ACTOR]);
									PLAYER_TEMP[playerid][pt_PIZZA_ACTOR] = INVALID_STREAMER_ID;
								}
								if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]))
								{
									DestroyDynamicCP(PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT]);
									PLAYER_TEMP[playerid][pt_PIZZA_CHECKPOINT] = INVALID_STREAMER_ID;
								}
								SendNotification(playerid, "Vuelve a subir a la moto y ve a la pizzería para cobrar.");
							}
						}
					}
				}
			}
			if(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] == PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID])
			{
				if(PLAYER_TEMP[playerid][pt_TRASH_DRIVER])
				{
					if(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_JOB_STARTED])
					{
						if(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID] == playerid) SendNotification(playerid, "Si quieres dejar de trabajar ve al vertedero y ponte tu ropa.");
					}
					else
					{
						TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID] = INVALID_PLAYER_ID;
						PLAYER_TEMP[playerid][pt_TRASH_DRIVER] = false;
						PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] = INVALID_VEHICLE_ID;
					}
				}
			}
		}
	}
	else if(oldstate == PLAYER_STATE_PASSENGER)
	{
		if(PLAYER_TEMP[playerid][pt_TAXI_METER_ENABLED])
		{
			new driver = GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_DRIVER];
			
			if(driver != INVALID_PLAYER_ID)
			{
				new work_extra_payment;
				if(work_info[WORK_TAXI][work_info_EXTRA_PAY] > 0 && work_info[WORK_TAXI][work_info_EXTRA_PAY_EXP] > 0)
				{
					work_extra_payment = (work_info[WORK_TAXI][work_info_EXTRA_PAY] * floatround(floatdiv(PLAYER_WORKS[ driver ][WORK_TAXI][pwork_LEVEL], work_info[WORK_TAXI][work_info_EXTRA_PAY_EXP])));
					if(work_info[WORK_TAXI][work_info_EXTRA_PAY_LIMIT] != 0) if(work_extra_payment > work_info[WORK_TAXI][work_info_EXTRA_PAY_LIMIT]) work_extra_payment = work_info[WORK_TAXI][work_info_EXTRA_PAY_LIMIT];
				
					if(ACCOUNT_INFO[playerid][ac_VIP]) work_extra_payment += SU_WORK_EXTRA_PAY;
				}
				PLAYER_WORKS[driver][WORK_TAXI][pwork_LEVEL] ++;
				
				SendFormatNotification(driver, "Has ganado ~g~%s dólares~w~ con este viaje.", number_format_thousand(TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_PAYMENT] + work_extra_payment));
				GivePlayerCash(driver, TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_PAYMENT] + work_extra_payment, true, false);
				GivePlayerCash(playerid, -TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_PAYMENT], true, true);
				
				SendFormatNotification(playerid, "El taxi te costó %s$.", number_format_thousand(TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_PAYMENT]));
				ResetVehicleTaxiMeter(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID]);
				HidePlayerTaxiMeter(playerid);
				UpdatePlayerTaxiMeterTextdraws(driver);
			}

			new passenger = GetVehicleFirstPassenger( PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] );
			if(passenger != INVALID_PLAYER_ID)
			{
				ShowPlayerTaxiMeter(passenger);
				if(PLAYER_TEMP[passenger][pt_WANT_TAXI])
				{
					PLAYER_TEMP[passenger][pt_WANT_TAXI] = false;
					DisablePlayerTaxiMark(passenger);
				}
				
				TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_ENABLED] = true;
				GetVehiclePos(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID], TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_OLD_X], TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_OLD_Y], TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_OLD_Z]);
				KillTimer(TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_TIMER]);
				TAXI_METER_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][veh_taxi_meter_TIMER] = SetTimerEx("UpdateVehicleTaximeter", 4000, true, "iii", PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_DRIVER], passenger);
			}
		}
		if(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] == PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID])
		{
			if(PLAYER_TEMP[playerid][pt_TRASH_PASSENGER])
			{
				if(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_JOB_STARTED])
				{
					if(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID] == playerid)
					{
						if(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_CP] >= TRASH_OBJECTS[ TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] ][trash_route_OBJECTS]) SendNotification(playerid, "Sube al camión para volver al vertedero y recibir la paga.");
						else
						{
							new cp = TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_CP];
							new route = TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE];
							new Float:distance =	GetPlayerDistanceFromPoint
													(
														playerid,
														TRASH_OBJECTS[ route ][trash_object_X][ cp ],
														TRASH_OBJECTS[ route ][trash_object_Y][ cp ],
														TRASH_OBJECTS[ route ][trash_object_Z][ cp ]
													);
							
							if(distance > 10.0) SendNotification(playerid, "Vuelve a subir al camión, aún estás demasiado lejos del punto de recogida.");
							else
							{
								if(IsPlayerInDynamicCP(playerid, PLAYER_TEMP[playerid][pt_TRASH_CHECKPOINT]))
								{
									if(!PLAYER_WORKS[playerid][WORK_TRASH][pwork_SET]) return DestroyPlayerTrashCheckpoint(playerid);
									if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_TRASH) return DestroyPlayerTrashCheckpoint(playerid);
									if(!PLAYER_TEMP[playerid][pt_TRASH_PASSENGER]) return 1;
									
									new driver = TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID];
									SendClientMessage(driver, -1, "{"#SILVER_COLOR"}Espera mientras tu compañero recoge la basura.");
									
									new Float:offset = floatdiv(PLAYER_WORKS[playerid][WORK_TRASH][pwork_LEVEL], 10);
									if(offset > 30.0) offset = 30.0;
									
									PLAYER_TEMP[playerid][pt_RECYCLE_BIN_VALUE] = 10.0 + offset;
									UpdatePlayerTrashRecycleSize(playerid);
									PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0]);
									PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1]);
									PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2]);
									
									TogglePlayerControllableEx(playerid, false);
									SetCameraBehindPlayer(playerid);
									ApplyAnimation(playerid, "ROB_BANK", "CAT_Safe_Rob", 4.1, true, false, false, false, 0);
									
									KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][10]);
									PLAYER_TEMP[playerid][pt_TIMERS][10] = SetTimerEx("RecycleUp", 1000, false, "i", playerid);
									return 1;
								}
								SendNotification(playerid, "Recoge la basura y vuelve al camión para continuar.");
							}
						}
					}
				}
				else
				{
					TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID] = INVALID_PLAYER_ID;
					PLAYER_TEMP[playerid][pt_TRASH_PASSENGER] = false;
					PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] = INVALID_VEHICLE_ID;
				}
			}
		}
	}
    return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_IMMUNITY] = gettime() + 3;
	return 1;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_IMMUNITY] = gettime() + 3;
	
	if(PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_NORMAL)
	{
		if(ac_Info[CHEAT_VEHICLE_ENTER_SPAMMER][ac_Enabled])
		{
			if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_ENTER_SPAMMER][p_ac_info_IMMUNITY])
			{
				new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_ENTER_VEHICLE]);
				
				if(interval < 50)
				{
					if(!ac_Info[CHEAT_VEHICLE_ENTER_SPAMMER][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_VEHICLE_ENTER_SPAMMER, float(interval));
					else
					{
						if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_ENTER_SPAMMER][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_VEHICLE_ENTER_SPAMMER][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_ENTER_SPAMMER][p_ac_info_DETECTIONS] = 0;
						else PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_ENTER_SPAMMER][p_ac_info_DETECTIONS] ++;
						
						PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_ENTER_SPAMMER][p_ac_info_LAST_DETECTION] = gettime();
						if(PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_ENTER_SPAMMER][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_VEHICLE_ENTER_SPAMMER][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_VEHICLE_ENTER_SPAMMER, float(interval));
					}
				}
				PLAYER_TEMP[playerid][pt_ANTIFLOOD_ENTER_VEHICLE] = GetTickCount();
			}
		}
	}
	
	new Float:sx, Float:sy, Float:sz;
	GetPlayerPos(playerid, sx, sy, sz);
	
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_MEDIC)
	{
		if(WORK_VEHICLES[vehicleid][work_vehicle_WORK] != WORK_MEDIC)
		{
			SendNotification(playerid, "~r~Estando de servicio como médico solo puedes llevar ambulancias.");
			RemovePlayerFromVehicle(playerid);
			SetPlayerPos(playerid, sx, sy, sz);
			return 1;
		}
	}
	
	if(!ispassenger)
	{
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_OCCUPIED])
		{
			if(PLAYER_TEMP[playerid][pt_CJ_WARNINGS] >= 5)
			{
				KickEx(playerid);
				return 1;
			}
			PLAYER_TEMP[playerid][pt_CJ_WARNINGS] ++;
			SendFormatNotification(playerid, "~w~No puedes robar vehículos así, congelado 5 segundos, aviso ~r~%d/5~w~.", PLAYER_TEMP[playerid][pt_CJ_WARNINGS]);
			RemovePlayerFromVehicle(playerid);
			SetPlayerPos(playerid, sx, sy, sz);

			TogglePlayerControllableEx(playerid, false);
			KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][19]);
			PLAYER_TEMP[playerid][pt_TIMERS][19] = SetTimerEx("CarJackingFinish", 5000, false, "i", playerid);

			SetTimerEx("ResyncPlayer", 0, false, "d", playerid);
			return 1;
		}
		
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK)
		{
			if(!PLAYER_WORKS[playerid][ WORK_VEHICLES[vehicleid][work_vehicle_WORK] ][pwork_SET])
			{
				SendFormatNotification(playerid, "Necesitas ser ~r~%s~w~ para poder conducir este vehículo.", work_info[ WORK_VEHICLES[vehicleid][work_vehicle_WORK] ][work_info_NAME]);
				RemovePlayerFromVehicle(playerid);
				SetPlayerPos(playerid, sx, sy, sz);
				return 1;
			}
			if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_VIP] && !ACCOUNT_INFO[playerid][ac_VIP])
			{
				SendNotification(playerid, "Necesitas adquirir membresía VIP para usar este vehículo. (/ayuda > membresía VIP)");
				RemovePlayerFromVehicle(playerid);
				SetPlayerPos(playerid, sx, sy, sz);
				return 1;
			}
			
			if(PLAYER_TEMP[playerid][pt_WORKING_IN] && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_VEHICLES[vehicleid][work_vehicle_WORK])
			{
				SendFormatNotification(playerid, "Tienes que dejar de estar de servicio de ~r~%s~w~.", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME]);
				RemovePlayerFromVehicle(playerid);
				SetPlayerPos(playerid, sx, sy, sz);
				return 1;
			}
			
			if(WORK_VEHICLES[vehicleid][work_vehicle_NEED_DUTY])
			{
				if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_VEHICLES[vehicleid][work_vehicle_WORK])
				{
					SendFormatNotification(playerid, "Necesitas estar de servicio de ~r~%s~w~ para poder conducir este vehículo.", work_info[ WORK_VEHICLES[vehicleid][work_vehicle_WORK] ][work_info_NAME]);
					RemovePlayerFromVehicle(playerid);
					SetPlayerPos(playerid, sx, sy, sz);
					return 1;
				}
			}
			
			if(WORK_VEHICLES[vehicleid][work_vehicle_EXP] > PLAYER_WORKS[playerid][ WORK_VEHICLES[vehicleid][work_vehicle_WORK] ][pwork_LEVEL])
			{
				SendFormatNotification(playerid, "Necesitas más experiencia en este trabajo para llevar este vehículo. ~r~(%d/%d)", PLAYER_WORKS[playerid][ WORK_VEHICLES[vehicleid][work_vehicle_WORK] ][pwork_LEVEL], WORK_VEHICLES[vehicleid][work_vehicle_EXP]);
				RemovePlayerFromVehicle(playerid);
				SetPlayerPos(playerid, sx, sy, sz);
				return 1;
			}
		}
		else if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] == VEHICLE_TYPE_DRIVING_SCHOOL)
		{
			if(!PLAYER_TEMP[playerid][pt_DL_EXAM])
			{
				SendNotification(playerid, "~r~Necesitas estar haciendo el examen de conducir.");
				RemovePlayerFromVehicle(playerid);
				SetPlayerPos(playerid, sx, sy, sz);
				return 1;	
			}
		}
	}
	else
	{
		if(WORK_VEHICLES[vehicleid][work_vehicle_VALID] && WORK_VEHICLES[vehicleid][work_vehicle_WORK] == WORK_TRASH)
		{
			if(!PLAYER_WORKS[playerid][WORK_TRASH][pwork_SET])
			{
				SendFormatNotification(playerid, "Necesitas ser %s para poder entrar a este vehículo.", work_info[ WORK_VEHICLES[vehicleid][work_vehicle_WORK] ][work_info_NAME]);
				RemovePlayerFromVehicle(playerid);
				SetPlayerPos(playerid, sx, sy, sz);
				return 1;
			}
			
			if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_VEHICLES[vehicleid][work_vehicle_WORK])
			{
				SendFormatNotification(playerid, "Necesitas estar de servicio como %s para poder conducir este vehículo.", work_info[ WORK_VEHICLES[vehicleid][work_vehicle_WORK] ][work_info_NAME]);
				RemovePlayerFromVehicle(playerid);
				SetPlayerPos(playerid, sx, sy, sz);
				return 1;
			}
		}
	}
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_TP_IMMUNITY] = gettime() + 5;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWNED] = true;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_ATTACHED_TO] = INVALID_VEHICLE_ID;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] = INVALID_PLAYER_ID;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_DRIVER] = INVALID_PLAYER_ID;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_OCCUPIED] = false;
	
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] < MIN_VEHICLE_HEALTH) GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] = MIN_VEHICLE_HEALTH;
	
	if(IsValidDynamic3DTextLabel(GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL]))
	{
		DestroyDynamic3DTextLabel(GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL]);
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL] = Text3D:INVALID_STREAMER_ID;
	}
	
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK)
	{
		if(WORK_VEHICLES[vehicleid][work_vehicle_WORK] == WORK_TRASH)
		{
			if(TRASH_VEHICLES[vehicleid][trash_vehicle_JOB_STARTED])
			{
				CancelTrashWork(TRASH_VEHICLES[vehicleid][trash_vehicle_DRIVER_ID], TRASH_VEHICLES[vehicleid][trash_vehicle_PASSENGER_ID], vehicleid);
			}
		}
		
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] = VEHICLE_STATE_NORMAL;
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] <= 0.0) GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] = 0.0;
		else GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] = frandom(GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS], GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] / 3, 2);
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] = 1000.0;
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_PANELS] = 0;
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_DOORS] = 0;
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_LIGHTS] = 0;
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_TIRES] = 0;
		
		switch(WORK_VEHICLES[vehicleid][work_vehicle_WORK])
		{
			case WORK_TAXI: ResetVehicleTaxiMeter(vehicleid);
			case WORK_TRUCK, WORK_DELIVERYMAN: ResetTruckInfo(vehicleid);
			case WORK_TRASH: ResetTrashInfo(vehicleid);
			case WORK_PIZZA: ResetPizzaInfo(vehicleid);
		}
	}
	SetVehicleNumberPlate(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_NUMBER_PLATE]);
	SetVehicleHealthEx(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH]);
	UpdateVehicleDamageStatus(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_PANELS], GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_DOORS], GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_LIGHTS], GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_TIRES]);
    ChangeVehicleColor(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_COLOR_1], GLOBAL_VEHICLES[vehicleid][gb_vehicle_COLOR_2]);
	ChangeVehiclePaintjob(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_PAINTJOB]);
	SetVehiclePosEx(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_X], GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Y], GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Z]);
	SetVehicleZAngle(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_ANGLE]);
	LinkVehicleToInteriorEx(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_INTERIOR]);
	SetVehicleVirtualWorldEx(vehicleid, 0);//GLOBAL_VEHICLES[vehicleid][gb_vehicle_WORLD]);
	AddVehicleComponents(vehicleid);
	
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_LIGHTS] = 0;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ALARM] = 0;
	if(!PLAYER_VEHICLES[vehicleid][player_vehicle_VALID]) GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS] = 0;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_BONNET] = 0;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_BOOT] = 0;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_OBJECTIVE] = 0;
	UpdateVehicleParams(vehicleid);
	SetClampVehicleLabel(vehicleid);
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWNED] = false;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] = INVALID_PLAYER_ID;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_DRIVER] = INVALID_PLAYER_ID;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_OCCUPIED] = false;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_ATTACHED_TO] = INVALID_VEHICLE_ID;
	
	if(IsValidDynamic3DTextLabel(GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL]))
	{
		DestroyDynamic3DTextLabel(GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL]);
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL] = Text3D:INVALID_STREAMER_ID;
	}
	
	switch(GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE])
	{
		case VEHICLE_TYPE_TEST: DestroyVehicleEx(vehicleid);
		case VEHICLE_TYPE_PERSONAL:
		{
			if(VEHICLE_INFO[GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_NORMAL_SPEEDO])
			{
				GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] = VEHICLE_STATE_DAMAGED;
				GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] = MIN_VEHICLE_HEALTH;
			}
			else
			{
				GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] = VEHICLE_STATE_NORMAL;
				GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] = 1000.0;
			}
			
			new playerid = GetPlayerIdFromAccountId(PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID]);
			if(playerid != INVALID_PLAYER_ID)
			{
				if(PI[playerid][ac_PHONE_NUMBER])
				{
					new message[64]; format(message, sizeof message, "Vehículo de sustitución entregado en últ. estacionamiento.");
					RegisterPhoneMessage(0, ACCOUNT_INFO[playerid][ac_ID], message);
					if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_ON)
					{
						if((PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_NORMAL || PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_DEAD))
						{
							SendClientMessageEx(playerid, -1, "{"#PRIMARY_COLOR"}[Nuevo mensaje recibido] {"#SILVER_COLOR"}Remitente: {FFFFFF}%s {"#SILVER_COLOR"}Mensaje: {FFFFFF}%s", convertPhoneNumber(playerid, 38350), message);
						}
					}
				}
			}
		}
		default:
		{
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] = VEHICLE_STATE_NORMAL;
			if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] <= 0.0) GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] = 0.0;
			else GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] = frandom(GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS], GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] / 3, 2);
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] = 1000.0;
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_PANELS] = 0;
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_DOORS] = 0;
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_LIGHTS] = 0;
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_DAMAGE_TIRES] = 0;
		}
	}
    return 1;
}

public OnTrailerUpdate(playerid, vehicleid)
{
    new towing_vehicle;
	towing_vehicle = GetPlayerVehicleID(playerid);
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_ATTACHED_TO] != towing_vehicle)
	{
		if(ac_Info[CHEAT_FAKE_TRAILER_SYNC][ac_Enabled])
		{
			if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_FAKE_TRAILER_SYNC][p_ac_info_IMMUNITY])
			{
				if(!ac_Info[CHEAT_FAKE_TRAILER_SYNC][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_FAKE_TRAILER_SYNC, vehicleid);
				else
				{
					if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_FAKE_TRAILER_SYNC][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_FAKE_TRAILER_SYNC][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_FAKE_TRAILER_SYNC][p_ac_info_DETECTIONS] = 0;
					else PLAYER_AC_INFO[playerid][CHEAT_FAKE_TRAILER_SYNC][p_ac_info_DETECTIONS] ++;
					
					PLAYER_AC_INFO[playerid][CHEAT_FAKE_TRAILER_SYNC][p_ac_info_LAST_DETECTION] = gettime();
					if(PLAYER_AC_INFO[playerid][CHEAT_FAKE_TRAILER_SYNC][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_FAKE_TRAILER_SYNC][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_FAKE_TRAILER_SYNC, vehicleid);
					return 0;
				}
				
			}
		}
	}
	return 1;
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_VALID])
	{
		if(ac_Info[CHEAT_UNOCCUPIED_VEHICLE_TP][ac_Enabled])
		{
			if(PLAYER_TEMP[playerid][pt_KICKED]) return 0;
			
			new Float:veh_x, Float:veh_y, Float:veh_z, Float:veh_dis;
			GetVehiclePos(vehicleid, veh_x, veh_y, veh_z);
			veh_dis = GetVehicleDistanceFromPoint(vehicleid, new_x, new_y, new_z);
			
			if(gettime() > GLOBAL_VEHICLES[vehicleid][gb_vehicle_TP_IMMUNITY] && GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWNED])
			{
				if(veh_dis > 20.0 && veh_z > -70.0)
				{
					if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_UNOCCUPIED_VEHICLE_TP][p_ac_info_IMMUNITY])
					{
						if(!ac_Info[CHEAT_UNOCCUPIED_VEHICLE_TP][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_UNOCCUPIED_VEHICLE_TP, veh_dis);
						else
						{
							if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_UNOCCUPIED_VEHICLE_TP][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_UNOCCUPIED_VEHICLE_TP][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_UNOCCUPIED_VEHICLE_TP][p_ac_info_DETECTIONS] = 0;
							else PLAYER_AC_INFO[playerid][CHEAT_UNOCCUPIED_VEHICLE_TP][p_ac_info_DETECTIONS] ++;
							
							PLAYER_AC_INFO[playerid][CHEAT_UNOCCUPIED_VEHICLE_TP][p_ac_info_LAST_DETECTION] = gettime();
							if(PLAYER_AC_INFO[playerid][CHEAT_UNOCCUPIED_VEHICLE_TP][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_UNOCCUPIED_VEHICLE_TP][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_UNOCCUPIED_VEHICLE_TP, veh_dis);
						}
					}
					
					SetVehicleVelocity(vehicleid, 0.0, 0.0, 0.0);
					return 0;
				}
			} 
		}
		
		new Float:spawn_distance = GetVehicleDistanceFromPoint(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_X], GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Y], GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Z]);
		switch(GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE])
		{
			case VEHICLE_TYPE_SELL:
			{
				if(spawn_distance > 10.0) SetVehicleToRespawnEx(vehicleid);
			}
			case VEHICLE_TYPE_WORK:
			{
				if(TRASH_VEHICLES[vehicleid][trash_vehicle_JOB_STARTED]) return 1;
				if(spawn_distance < 25.0)
				{
					if(spawn_distance > 10.0) SetVehicleToRespawnEx(vehicleid);
				}
			}
		}
	}
    return 1;
}

forward StartVehicleEngine(playerid, vehicleid);
public StartVehicleEngine(playerid, vehicleid)
{
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] == VEHICLE_STATE_DAMAGED) return Auto_SendPlayerAction(playerid, "intentó arrancar el vehículo pero el motor está dañado.");
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] <= 0.1) return Auto_SendPlayerAction(playerid, "intentó arrancar el vehículo pero no tiene gasolina.");
	
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 1;
	UpdateVehicleParams(vehicleid);
	
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_ATTACHED_TO] != INVALID_VEHICLE_ID)
	{
		if(GLOBAL_VEHICLES[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_ATTACHED_TO] ][gb_vehicle_DRIVER] != INVALID_PLAYER_ID)
			PLAYER_AC_INFO[ GLOBAL_VEHICLES[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_ATTACHED_TO] ][gb_vehicle_DRIVER] ][CHEAT_FAKE_TRAILER_SYNC][p_ac_info_IMMUNITY] = gettime() + 5;

		GLOBAL_VEHICLES[vehicleid][gb_vehicle_ATTACHED_TO] = INVALID_VEHICLE_ID;
		DetachTrailerFromVehicle(vehicleid);
	}
	
	if(PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID]) Auto_SendPlayerAction(playerid, "ha arrancado su vehículo.");
	else Auto_SendPlayerAction(playerid, "ha arrancado el vehículo.");

	SendNotification(playerid, "~h~~g~Encendido");
	return 1;
}
public OnEnterExitModShop(playerid, enterexit, interiorid)
{
    if(enterexit) // Entra
    {
		SendClientMessage(playerid, -1, " ");
		SendNotification(playerid, "Solo puedes tunear vehículos en el mecánico, búscalo con el /GPS.");
		SendClientMessage(playerid, -1, "{ffff35}[ATENCION] {"#SILVER_COLOR"}Si realizas alguna modificación al vehículo en este garaje serás expulsado.");
    }
	else
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		if(vehicleid != INVALID_VEHICLE_ID)
		{
			ChangeVehicleColor(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_COLOR_1], GLOBAL_VEHICLES[vehicleid][gb_vehicle_COLOR_2]);
			ChangeVehiclePaintjob(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_PAINTJOB]);
			AddVehicleComponents(vehicleid);
		}
	}
    return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	if(ac_Info[CHEAT_CAR_MOD][ac_Enabled])
	{
		if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_CAR_MOD][p_ac_info_IMMUNITY])
		{
			if(!ac_Info[CHEAT_CAR_MOD][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_CAR_MOD);
			else
			{
				if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_CAR_MOD][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_CAR_MOD][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_CAR_MOD][p_ac_info_DETECTIONS] = 0;
				else PLAYER_AC_INFO[playerid][CHEAT_CAR_MOD][p_ac_info_DETECTIONS] ++;
				
				PLAYER_AC_INFO[playerid][CHEAT_CAR_MOD][p_ac_info_LAST_DETECTION] = gettime();
				if(PLAYER_AC_INFO[playerid][CHEAT_CAR_MOD][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_CAR_MOD][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_CAR_MOD);
			}
		}
	}
    return 0;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	if(ac_Info[CHEAT_CAR_MOD][ac_Enabled])
	{
		if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_CAR_MOD][p_ac_info_IMMUNITY])
		{
			if(!(-1 < paintjobid < 3) && paintjobid != 255)
			{
				if(!ac_Info[CHEAT_CAR_MOD][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_CAR_MOD);
				else
				{
					if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_CAR_MOD][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_CAR_MOD][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_CAR_MOD][p_ac_info_DETECTIONS] = 0;
					else PLAYER_AC_INFO[playerid][CHEAT_CAR_MOD][p_ac_info_DETECTIONS] ++;
					
					PLAYER_AC_INFO[playerid][CHEAT_CAR_MOD][p_ac_info_LAST_DETECTION] = gettime();
					if(PLAYER_AC_INFO[playerid][CHEAT_CAR_MOD][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_CAR_MOD][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_CAR_MOD);
				}
			}
		}
	}
	return 1;
}



forward TruckUnLoadUp(playerid, vehicleid);
public TruckUnLoadUp(playerid, vehicleid)
{
	if(PLAYER_TEMP[playerid][pt_TRUCK_LOADING_VALUE] < 100.0)
	{
		PLAYER_TEMP[playerid][pt_TRUCK_LOADING_VALUE] += 10.0;
		if(PLAYER_TEMP[playerid][pt_TRUCK_LOADING_VALUE] > 100.0) PLAYER_TEMP[playerid][pt_TRUCK_LOADING_VALUE] = 100.0;
		
		UpdatePlayerLoadingTruckSize(playerid);
		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][9]);
		PLAYER_TEMP[playerid][pt_TIMERS][9] = SetTimerEx("TruckUnLoadUp", 1000, false, "ii", playerid, vehicleid);
		return 1;
	}
	
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3]);
	
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 1;
	UpdateVehicleParams(vehicleid);
	
	TRUCK_VEHICLE[vehicleid][truck_vehicle_UNLOADING] = false;
	TRUCK_VEHICLE[vehicleid][truck_vehicle_DELIVERED] = true;
	
	SendNotification(playerid, "La mercancía ha sido entregada, regresa al punto de partida para cobrar.");
	SetPlayerTruckCheckpoint(playerid, vehicleid);
	return 1;
}

public OnPlayerEnterDynamicRaceCP(playerid, checkpointid)
{
	new info[1];
	Streamer_GetArrayData(STREAMER_TYPE_RACE_CP, checkpointid, E_STREAMER_EXTRA_ID, info);
	
	switch(info[0])
	{
		case CHECKPOINT_TYPE_HARVEST:
		{
			if(PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] == sizeof(HARVEST_CHECKPOINTS) - 1)
			{
				new vehicleid = GetPlayerVehicleID(playerid);
				if(vehicleid == INVALID_VEHICLE_ID) return 1;
				
				new work_extra_payment;
				if(work_info[WORK_HARVESTER][work_info_EXTRA_PAY] > 0 && work_info[WORK_HARVESTER][work_info_EXTRA_PAY_EXP] > 0)
				{
					work_extra_payment = (work_info[WORK_HARVESTER][work_info_EXTRA_PAY] * floatround(floatdiv(PLAYER_WORKS[playerid][WORK_HARVESTER][pwork_LEVEL], work_info[WORK_HARVESTER][work_info_EXTRA_PAY_EXP])));
					if(work_info[WORK_HARVESTER][work_info_EXTRA_PAY_LIMIT] != 0) if(work_extra_payment > work_info[WORK_HARVESTER][work_info_EXTRA_PAY_LIMIT]) work_extra_payment = work_info[WORK_HARVESTER][work_info_EXTRA_PAY_LIMIT];
					
					if(ACCOUNT_INFO[playerid][ac_VIP]) work_extra_payment += SU_WORK_EXTRA_PAY;
				}
				
				new money = minrand(350, 410) + WORK_HARVESTER;
				GivePlayerCash(playerid, money, true, false);
				
				PLAYER_WORKS[playerid][WORK_HARVESTER][pwork_LEVEL] ++;
				AddPlayerJobPoints(playerid, WORK_HARVESTER);
				SendFormatNotification(playerid, "Has ganado %s$ con esta cosecha.", number_format_thousand(money));
				
				CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
				return 1;
			}
			
			new str[50];
			format(str, sizeof str, "~n~~n~~n~~n~~n~~n~~n~~w~~h~checkpoint_~y~%d/%d", PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] + 1, sizeof(HARVEST_CHECKPOINTS));
			GameTextForPlayer(playerid, str, 2000, 3);
			
			PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] ++;
			Set_HARVEST_Checkpoint(playerid);
		}
		case CHECKPOINT_TYPE_FUMIGATOR:
		{
			if(PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] == sizeof(FUMIGATOR_CHECKPOINTS) - 1)
			{
				new vehicleid = GetPlayerVehicleID(playerid);
				if(vehicleid == INVALID_VEHICLE_ID) return 1;
				
				new work_extra_payment;
				if(work_info[WORK_FUMIGATOR][work_info_EXTRA_PAY] > 0 && work_info[WORK_FUMIGATOR][work_info_EXTRA_PAY_EXP] > 0)
				{
					work_extra_payment = (work_info[WORK_FUMIGATOR][work_info_EXTRA_PAY] * floatround(floatdiv(PLAYER_WORKS[playerid][WORK_FUMIGATOR][pwork_LEVEL], work_info[WORK_FUMIGATOR][work_info_EXTRA_PAY_EXP])));
					if(work_info[WORK_FUMIGATOR][work_info_EXTRA_PAY_LIMIT] != 0) if(work_extra_payment > work_info[WORK_FUMIGATOR][work_info_EXTRA_PAY_LIMIT]) work_extra_payment = work_info[WORK_FUMIGATOR][work_info_EXTRA_PAY_LIMIT];
				
					if(ACCOUNT_INFO[playerid][ac_VIP]) work_extra_payment += SU_WORK_EXTRA_PAY;
				}
				
				new money = minrand(1200, 1500) + work_extra_payment;
				GivePlayerCash(playerid, money, true, false);
				
				PLAYER_WORKS[playerid][WORK_FUMIGATOR][pwork_LEVEL] ++;
				AddPlayerJobPoints(playerid, WORK_FUMIGATOR);
				SendFormatNotification(playerid, "Gracias por fumigar nuestros cielos, has ganado %s$.", number_format_thousand(money));
				
				CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
				return 1;
			}
			
			new str[50];
			format(str, sizeof str, "~n~~n~~n~~n~~n~~n~~n~~w~~h~checkpoint_~y~%d/%d", PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] + 1, sizeof(FUMIGATOR_CHECKPOINTS));
			GameTextForPlayer(playerid, str, 2000, 3);
			
			PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] ++;
			Set_FUMIGATOR_Checkpoint(playerid);
		}
		case CHECKPOINT_TYPE_DRIVING_SCHOOL:
		{
			if(GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_TYPE] == VEHICLE_TYPE_DRIVING_SCHOOL)
			{
				if(PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] == sizeof(Driving_School_Points) - 1)
				{
					new Float:vhealth;
					GetVehicleHealth(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID], vhealth);

					if(vhealth >= 800.0)
					{
						PI[playerid][ac_DRIVE_LICENSE_POINTS] = 12;
						SendNotification(playerid, "¡Felicidades! te has sacado el carnet de conducir, cuentas con 12 puntos.");
					}
					else SendNotification(playerid, "No has superado el examen.");

					if(IsValidDynamicRaceCP(PLAYER_TEMP[playerid][pt_DL_EXAM_CP]))
					{
						DestroyDynamicRaceCP(PLAYER_TEMP[playerid][pt_DL_EXAM_CP]);
						PLAYER_TEMP[playerid][pt_DL_EXAM_CP] = INVALID_STREAMER_ID;
					}
					PLAYER_TEMP[playerid][pt_DL_EXAM] = false;
					PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] = 0;
					SetVehicleToRespawnEx(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID]);
					return 1;
				}

				new str[50];
				format(str, sizeof str, "~n~~n~~n~~n~~n~~n~~n~~w~~h~checkpoint_~y~%d/%d", PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] + 1, sizeof(Driving_School_Points));
				GameTextForPlayer(playerid, str, 2000, 3);
				
				PLAYER_TEMP[playerid][pt_DL_EXAM_PROCCESS] ++;
				SetPlayerDrivingSchoolCP(playerid);
			}
		}
	}
	return 1;
}

forward RecycleUp(playerid);
public RecycleUp(playerid)
{
	if(PLAYER_TEMP[playerid][pt_RECYCLE_BIN_VALUE] < 100.0)
	{
		PLAYER_TEMP[playerid][pt_RECYCLE_BIN_VALUE] += 15.0;
		if(PLAYER_TEMP[playerid][pt_RECYCLE_BIN_VALUE] > 100.0) PLAYER_TEMP[playerid][pt_RECYCLE_BIN_VALUE] = 100.0;
		
		ApplyAnimation(playerid, "ROB_BANK", "CAT_Safe_Rob", 4.1, true, false, false, false, 0);
		UpdatePlayerTrashRecycleSize(playerid);
		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][10]);
		PLAYER_TEMP[playerid][pt_TIMERS][10] = SetTimerEx("RecycleUp", 1000, false, "i", playerid);
		return 1;
	}
	
	TogglePlayerControllableEx(playerid, true);
	ClearAnimations(playerid);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][0]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][1]);
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_RECYCLE_TRASH][2]);
	
	new driver = TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID];
	new cp = TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_CP];
	new route = TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE];
	
	DestroyDynamicObject(TRASH_PLAYER_OBJECTS[playerid][cp]);
	TRASH_PLAYER_OBJECTS[playerid][cp] = INVALID_STREAMER_ID;
	
	DestroyDynamicObject(TRASH_PLAYER_OBJECTS[driver][cp]);
	TRASH_PLAYER_OBJECTS[driver][cp] = INVALID_STREAMER_ID;
	
	CreatePlayerFlashObject(
								playerid, 
								TRASH_OBJECTS[ route ][trash_object_X][ cp ],
								TRASH_OBJECTS[ route ][trash_object_Y][ cp ],
								TRASH_OBJECTS[ route ][trash_object_Z][ cp ] - 1.5
							);
	
	CreatePlayerFlashObject(
								driver, 
								TRASH_OBJECTS[ route ][trash_object_X][ cp ],
								TRASH_OBJECTS[ route ][trash_object_Y][ cp ],
								TRASH_OBJECTS[ route ][trash_object_Z][ cp ] - 1.5
							);
							
	PlayerPlaySoundEx(playerid, 1190, 0.0, 0.0, 0.0);
	
	TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_CP] ++;
	if(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_CP] >= TRASH_OBJECTS[ TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] ][trash_route_OBJECTS])
	{
		SendNotification(playerid, "Sube al camión para volver al vertedero y recibir la paga.");
		SendNotification(driver, "Espera a que se suba tu compañero y vuelve al vertedero para recibir la paga.");
	}
	else SendNotification(playerid, "Sube al camión y sigue con la ruta.");

	SetPlayerTrashCheckpoint(driver, PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
	SetPlayerTrashCheckpoint(playerid, PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
	
	Streamer_Update(driver);
	Streamer_Update(playerid);
	return 1;
}

forward ContinueTreeAnimation(playerid, tree, Float:rotation);
public ContinueTreeAnimation(playerid, tree, Float:rotation)
{
	new Float:pos[3];
	pos[0] = LUMBER_TREES[ tree ][lumber_tree_X];
	pos[1] = LUMBER_TREES[ tree ][lumber_tree_Y];
	pos[2] = LUMBER_TREES[ tree ][lumber_tree_Z];
	
	pos[0] += (1.5 * floatsin(-rotation, degrees));
	pos[1] += (1.5 * floatcos(-rotation, degrees));
	
	CreateFlashObject(pos[0], pos[1], pos[2] - 0.4);
	DestroyDynamicObject(LUMBER_TREES[ tree ][lumber_tree_OBJECT_ID]);
	PlayerPlaySoundEx(playerid, 1190, 0.0, 0.0, 0.0);
	
	LUMBER_TREES[ tree ][lumber_tree_OBJECT_ID] = CreateDynamicObject(841, pos[0], pos[1], pos[2] + 0.2, 0.0, 0.0, rotation, 0, 0);
	
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
	PLAYER_TEMP[playerid][pt_PLAYER_CARRYING_TREE] = true;
	PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] = -1;
	
	RemovePlayerAttachedObject(playerid, 8);
	SetPlayerAttachedObject(playerid, 8, 19793, 6, 0.077999, 0.043999, -0.170999, -13.799953, 79.70, 0.0);
	
	if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT]))
	{
		DestroyDynamicCP(PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT] = INVALID_STREAMER_ID;
	}
	PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT] = CreateDynamicCP(-504.353912, -88.591995, 62.086818, 5.0, 0, 0, playerid, 9999999999.0);
	
	new info[1];
	info[0] = CHECKPOINT_TYPE_LUMBERJACK;
	Streamer_SetArrayData(STREAMER_TYPE_CP, PLAYER_TEMP[playerid][pt_LUMBERJACK_CHECKPOINT], E_STREAMER_EXTRA_ID, info);
	
	SendNotification(playerid, "Ve a llevar la leña al punto marcado para recibir la paga.");
	
	
	LUMBER_TREES[ tree ][lumber_tree_GROW_COUNTER] = 90;
	new label_str[256];
	format(label_str, sizeof label_str, "{f46530}Árbol (%d)\n\n{"#SILVER_COLOR"}(%s)", tree, TimeConvert(LUMBER_TREES[ tree ][lumber_tree_GROW_COUNTER]));
	UpdateDynamic3DTextLabelText(LUMBER_TREES[ tree ][lumber_tree_LABEL], -1, label_str);
	
	LUMBER_TREES[ tree ][lumber_tree_STATE] = LUMBER_TREE_STATE_CUTTED;
	KillTimer(LUMBER_TREES[ tree ][lumber_tree_GROW_TIMER]);
	LUMBER_TREES[ tree ][lumber_tree_GROW_TIMER] = SetTimerEx("GrowTreeUp", 1000, false, "i", tree);
	
	Streamer_Update(playerid);
	return 1;
}

forward GrowTreeUp(tree);
public GrowTreeUp(tree)
{
	if(LUMBER_TREES[ tree ][lumber_tree_STATE] != LUMBER_TREE_STATE_CUTTED) return 0;
	
	if(LUMBER_TREES[ tree ][lumber_tree_GROW_COUNTER] > 0)
	{
		LUMBER_TREES[ tree ][lumber_tree_GROW_COUNTER] --;
		new label_str[256];
		format(label_str, sizeof label_str, "{f46530}Árbol (%d)\n\n{"#SILVER_COLOR"}(%s)", tree, TimeConvert(LUMBER_TREES[ tree ][lumber_tree_GROW_COUNTER]));
		UpdateDynamic3DTextLabelText(LUMBER_TREES[ tree ][lumber_tree_LABEL], -1, label_str);
		
		KillTimer(LUMBER_TREES[ tree ][lumber_tree_GROW_TIMER]);
		LUMBER_TREES[ tree ][lumber_tree_GROW_TIMER] = SetTimerEx("GrowTreeUp", 1000, false, "i", tree);
		return 1;
	}
	
	DestroyDynamicObject(LUMBER_TREES[ tree ][lumber_tree_OBJECT_ID]);
	LUMBER_TREES[ tree ][lumber_tree_OBJECT_ID] = CreateDynamicObject(LUMBER_TREES[ tree ][lumber_tree_MODELID], LUMBER_TREES[ tree ][lumber_tree_X], LUMBER_TREES[ tree ][lumber_tree_Y], LUMBER_TREES[ tree ][lumber_tree_Z], LUMBER_TREES[ tree ][lumber_tree_RX], LUMBER_TREES[ tree ][lumber_tree_RY], LUMBER_TREES[ tree ][lumber_tree_RZ], 0, 0);
	LUMBER_TREES[ tree ][lumber_tree_STATE] = LUMBER_TREE_STATE_NORMAL;

	new label_str[256];
	format(label_str, sizeof label_str, "{f46530}Árbol (%d)\n\n{FFFFFF}Pulsa {f9d834}~k~~CONVERSATION_NO~ {FFFFFF}para talar el árbol.", tree);
	UpdateDynamic3DTextLabelText(LUMBER_TREES[ tree ][lumber_tree_LABEL], -1, label_str);
	return 1;
}

forward DestroyFlashObject(objectid);
public DestroyFlashObject(objectid)
{
	return DestroyObject(objectid);
}

forward DestroyPlayerFlashObject(playerid, objectid);
public DestroyPlayerFlashObject(playerid, objectid)
{
	return DestroyPlayerObject(playerid, objectid);
}

forward GrowPlantUp(plant);
public GrowPlantUp(plant)
{
	if(!PLANTS[plant][plant_VALID] || !PLANTS[plant][plant_GROWING]) return 0;
	
	if(PLANTS[plant][plant_GROWING_PROGRESS] < 100.0)
	{
		PLANTS[plant][plant_GROWING_PROGRESS] += floatdiv(25, plant_info[ PLANTS[plant][plant_TYPE] ][plant_info_SEEDS]);
		if(PLANTS[plant][plant_GROWING_PROGRESS] > 100.0) PLANTS[plant][plant_GROWING_PROGRESS] = 100.0;
		
		if(PLANTS[plant][plant_INVISIBLE] && PLANTS[plant][plant_GROWING_PROGRESS] > 50.0)
		{
			PLANTS[plant][plant_INVISIBLE] = false;
			SetDynamicObjectMaterial(PLANTS[plant][plant_OBJECT_ID], 2, 2244, "plants_tabletop", "CJ_PLANT", 0xFFFFFFFF);
		}
		
		new label_str[256];
		if(seed_info[ PLANTS[plant][plant_TYPE] ][seed_info_TYPE] == TYPE_LEGAL) format(label_str, sizeof label_str, "{FF7373}Planta (%s)\n{CCCCCC}Agricultor: %s\n\n{FF7373}(%.1f%%)", plant_info[ PLANTS[plant][plant_TYPE] ][plant_info_NAME], PLANTS[plant][plant_PLANTED_BY_NAME], PLANTS[plant][plant_GROWING_PROGRESS]);
		else format(label_str, sizeof label_str, "{FF7373}Planta (%s)\n{CCCCCC}Agricultor: %s\n\n{FF7373}(%.1f%%)", plant_info[ PLANTS[plant][plant_TYPE] ][plant_info_NAME], PLANTS[plant][plant_PLANTED_BY_NAME], PLANTS[plant][plant_GROWING_PROGRESS]);
		UpdateDynamic3DTextLabelText(PLANTS[plant][plant_LABEL_ID], -1, label_str);
		
		KillTimer(PLANTS[plant][plant_TIMER]);
		PLANTS[plant][plant_TIMER] = SetTimerEx("GrowPlantUp", 5000, false, "d", plant);
		return 1;
	}
	
	SetDynamicObjectMaterial(PLANTS[plant][plant_OBJECT_ID], 2, 2244, "plants_tabletop", "CJ_PLANT", 0xFFff0000);
	
	PLANTS[plant][plant_GROWING] = false;
	PLANTS[plant][plant_EXPIRE_TIME] = 300;
	PLANTS[plant][plant_IMMUNITY] = 60;
		
	new label_str[256];
	if(seed_info[ PLANTS[plant][plant_TYPE] ][seed_info_TYPE] == TYPE_LEGAL) format(label_str, sizeof label_str, "{FF7373}Planta (%s)\n\n{CCCCCC}Agricultor: %s\nExpira en %sm\nInmunidad: %ss\n\nPresiona {FF7373}[ H ]{CCCCCC} para recogerla", plant_info[ PLANTS[plant][plant_TYPE] ][plant_info_NAME], PLANTS[plant][plant_PLANTED_BY_NAME], TimeConvert(PLANTS[plant][plant_EXPIRE_TIME]), TimeConvert(PLANTS[plant][plant_IMMUNITY]));
	else format(label_str, sizeof label_str, "{FF7373}Planta (%s)\n\n{CCCCCC}Agricultor: %s\nExpira en %sm\nInmunidad: %ss\n\nPresiona {FF7373}[ H ]{CCCCCC} para recogerla", plant_info[ PLANTS[plant][plant_TYPE] ][plant_info_NAME], PLANTS[plant][plant_PLANTED_BY_NAME], TimeConvert(PLANTS[plant][plant_EXPIRE_TIME]), TimeConvert(PLANTS[plant][plant_IMMUNITY]));
	UpdateDynamic3DTextLabelText(PLANTS[plant][plant_LABEL_ID], -1, label_str);
	
	KillTimer(PLANTS[plant][plant_TIMER]);
	PLANTS[plant][plant_TIMER] = SetTimerEx("ExpirePlantTime", 1000, false, "d", plant);
	return 1;
}

forward ExpirePlantTime(plant);
public ExpirePlantTime(plant)
{
	if(!PLANTS[plant][plant_VALID]) return 0;
	
	if(PLANTS[plant][plant_EXPIRE_TIME] > 0)
	{
		new 
			label_str[256];

		PLANTS[plant][plant_EXPIRE_TIME] --;
		PLANTS[plant][plant_IMMUNITY] --;

		if(seed_info[ PLANTS[plant][plant_TYPE] ][seed_info_TYPE] == TYPE_LEGAL) 
		{
			if(PLANTS[plant][plant_IMMUNITY] > 0) format(label_str, sizeof label_str, "{FF7373}Planta (%s)\n\n{CCCCCC}Agricultor: %s\nExpira en %sm\nInmunidad: %ss\n\nPresiona {FF7373}[ H ]{CCCCCC} para recogerla", plant_info[ PLANTS[plant][plant_TYPE] ][plant_info_NAME], PLANTS[plant][plant_PLANTED_BY_NAME], TimeConvert(PLANTS[plant][plant_EXPIRE_TIME]), TimeConvert(PLANTS[plant][plant_IMMUNITY]));
			else format(label_str, sizeof label_str, "{FF7373}Planta (%s)\n\n{CCCCCC}Agricultor: %s\nExpira en %sm\nInmunidad: Nada\n\nPresiona {FF7373}[ H ]{CCCCCC} para recogerla", plant_info[ PLANTS[plant][plant_TYPE] ][plant_info_NAME], PLANTS[plant][plant_PLANTED_BY_NAME], TimeConvert(PLANTS[plant][plant_EXPIRE_TIME]));
		}
		else 
		{
			if(PLANTS[plant][plant_IMMUNITY] > 0) format(label_str, sizeof label_str, "{FF7373}Planta (%s)\n\n{CCCCCC}Agricultor: %s\nExpira en %sm\nInmunidad: %ss\n\nPresiona {FF7373}[ H ]{CCCCCC} para recogerla", plant_info[ PLANTS[plant][plant_TYPE] ][plant_info_NAME], PLANTS[plant][plant_PLANTED_BY_NAME], TimeConvert(PLANTS[plant][plant_EXPIRE_TIME]), TimeConvert(PLANTS[plant][plant_IMMUNITY]));
			else format(label_str, sizeof label_str, "{FF7373}Planta (%s)\n\n{CCCCCC}Agricultor: %s\nExpira en %sm\nInmunidad: Nada\n\nPresiona {FF7373}[ H ]{CCCCCC} para recogerla", plant_info[ PLANTS[plant][plant_TYPE] ][plant_info_NAME], PLANTS[plant][plant_PLANTED_BY_NAME], TimeConvert(PLANTS[plant][plant_EXPIRE_TIME]));
		}

		UpdateDynamic3DTextLabelText(PLANTS[plant][plant_LABEL_ID], -1, label_str);
		
		KillTimer(PLANTS[plant][plant_TIMER]);
		PLANTS[plant][plant_TIMER] = SetTimerEx("ExpirePlantTime", 1000, false, "d", plant);
		return 1;
	}
	
	new Float:pos[3];
	GetDynamicObjectPos(PLANTS[plant][plant_OBJECT_ID], pos[0], pos[1], pos[2]);
	CreateFlashObject(pos[0], pos[1], pos[2] - 1.3);
	
	DestroyDynamicObject(PLANTS[plant][plant_OBJECT_ID]);
	DestroyDynamic3DTextLabel(PLANTS[plant][plant_LABEL_ID]);
	
	new tmp_PLANTS[PLANTS_ENUM]; PLANTS[plant] = tmp_PLANTS;
	return 1;
}

public OnPlayerShootDynamicObject(playerid, weaponid, objectid, Float:x, Float:y, Float:z)
{
	if(IsPlayerInDynamicArea(playerid, Farmer_Area))
	{
		new info[3];
		Streamer_GetArrayData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_EXTRA_ID, info);
		
		if(info[0] == OBJECT_TYPE_PLANT)
		{
			KillTimer(PLANTS[info[1]][plant_TIMER]);
			
			new Float:pos[3];
			GetDynamicObjectPos(PLANTS[info[1]][plant_OBJECT_ID], pos[0], pos[1], pos[2]);
			CreateFlashObject(pos[0], pos[1], pos[2] - 1.3);
			
			DestroyDynamicObject(PLANTS[info[1]][plant_OBJECT_ID]);
			DestroyDynamic3DTextLabel(PLANTS[info[1]][plant_LABEL_ID]);
			
			new tmp_PLANTS[PLANTS_ENUM]; PLANTS[info[1]] = tmp_PLANTS;
		}
	}
	return 1;
}



public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{	
	if(WEAPON_INFO[weaponid][weapon_info_AMMO]) PLAYER_WEAPONS[playerid][ WEAPON_INFO[weaponid][weapon_info_SLOT] ][player_weapon_AMMO] --;
	if(PLAYER_WEAPONS[playerid][ WEAPON_INFO[weaponid][weapon_info_SLOT] ][player_weapon_AMMO] <= 0) PLAYER_WEAPONS[playerid][ WEAPON_INFO[weaponid][weapon_info_SLOT] ][player_weapon_AMMO] = 0;
	if(ac_Info[CHEAT_UNDETECTED_WEAPON][ac_Enabled])
	{
		if(GetPlayerWeapon(playerid) != weaponid)
		{
			if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_UNDETECTED_WEAPON][p_ac_info_IMMUNITY])
			{
				if(!ac_Info[CHEAT_UNDETECTED_WEAPON][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_UNDETECTED_WEAPON);
				else
				{
					if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_UNDETECTED_WEAPON][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_UNDETECTED_WEAPON][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_UNDETECTED_WEAPON][p_ac_info_DETECTIONS] = 0;
					else PLAYER_AC_INFO[playerid][CHEAT_UNDETECTED_WEAPON][p_ac_info_DETECTIONS] ++;
					
					PLAYER_AC_INFO[playerid][CHEAT_UNDETECTED_WEAPON][p_ac_info_LAST_DETECTION] = gettime();
					if(PLAYER_AC_INFO[playerid][CHEAT_UNDETECTED_WEAPON][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_UNDETECTED_WEAPON][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_UNDETECTED_WEAPON);
				}
			}
		}
	}
	if(ac_Info[CHEAT_DRIVE_BY][ac_Enabled])
	{
		new player_state = GetPlayerState(playerid);
		if(player_state == PLAYER_STATE_DRIVER || player_state == PLAYER_STATE_PASSENGER)
		{
			if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_DRIVE_BY][p_ac_info_IMMUNITY])
			{
				if(!ac_Info[CHEAT_DRIVE_BY][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_DRIVE_BY);
				else
				{
					if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_DRIVE_BY][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_DRIVE_BY][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_DRIVE_BY][p_ac_info_DETECTIONS] = 0;
					else PLAYER_AC_INFO[playerid][CHEAT_DRIVE_BY][p_ac_info_DETECTIONS] ++;
					
					PLAYER_AC_INFO[playerid][CHEAT_DRIVE_BY][p_ac_info_LAST_DETECTION] = gettime();
					if(PLAYER_AC_INFO[playerid][CHEAT_DRIVE_BY][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_DRIVE_BY][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_DRIVE_BY);
				}
			}
		}
	}
	if(PI[playerid][ac_CREW])
	{
		if(gettime() > CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_LAST_ATTACK] + 300)
		{
			if(PLAYER_TEMP[playerid][pt_LAST_TERRITORY] != INVALID_STREAMER_ID)
			{
				if(IsPlayerInDynamicArea(playerid, TERRITORIES[ PLAYER_TEMP[playerid][pt_LAST_TERRITORY] ][territory_AREA]))
				{
					if(TERRITORIES[ PLAYER_TEMP[playerid][pt_LAST_TERRITORY] ][territory_CREW_ID] != PI[playerid][ac_CREW])
					{
						if(!TERRITORIES[ PLAYER_TEMP[playerid][pt_LAST_TERRITORY] ][territory_WAR] && !CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_FIGHTING])
						{
							if(gettime() > TERRITORIES[ PLAYER_TEMP[playerid][pt_LAST_TERRITORY] ][territory_LAST_ATTACK] + 300)
							{
								if(!TERRITORIES[ PLAYER_TEMP[playerid][pt_LAST_TERRITORY] ][territory_OCCUPIED])
								{
									new attackers_in_area = CountCrewPlayersInTerritory(PLAYER_TEMP[playerid][pt_CREW_INDEX], PLAYER_TEMP[playerid][pt_LAST_TERRITORY]);
									if(attackers_in_area >= 2) StartTerritoryAttack(PLAYER_TEMP[playerid][pt_CREW_INDEX], PLAYER_TEMP[playerid][pt_LAST_TERRITORY], 420);
								}
								else
								{
									if(CREW_INFO[ TERRITORIES[ PLAYER_TEMP[playerid][pt_LAST_TERRITORY] ][territory_CREW_INDEX] ][crew_VALID])
									{
										if(!CREW_INFO[ TERRITORIES[ PLAYER_TEMP[playerid][pt_LAST_TERRITORY] ][territory_CREW_INDEX] ][crew_FIGHTING])
										{	
											new attackers_in_area = CountCrewPlayersInTerritory(PLAYER_TEMP[playerid][pt_CREW_INDEX], PLAYER_TEMP[playerid][pt_LAST_TERRITORY]);
											if(attackers_in_area >= 5) StartTerritoryAttack(PLAYER_TEMP[playerid][pt_CREW_INDEX], PLAYER_TEMP[playerid][pt_LAST_TERRITORY], 420);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
    return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float: amount, weaponid, bodypart)
{
	if(damagedid != INVALID_PLAYER_ID)
	{
		if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE && ACCOUNT_INFO[playerid][ac_STATE] != ROLEPLAY_STATE_CRACK && ACCOUNT_INFO[playerid][ac_STATE] != ROLEPLAY_STATE_JAIL) {
			if(PLAYER_TEMP[playerid][pt_COMBAT])
			{
				KillTimer(PLAYER_TEMP[playerid][pt_COMBAT_TIMER]);
				PLAYER_TEMP[playerid][pt_COMBAT_TIMER] = SetTimerEx("ResetCombat", 60000 * 2, false, "i", playerid);
			}
			else
			{
				PLAYER_TEMP[playerid][pt_COMBAT_TIMER] = SetTimerEx("ResetCombat", 60000 * 2, false, "i", playerid);
				PLAYER_TEMP[playerid][pt_COMBAT] = true;

				TextDrawShowForPlayer(playerid, Textdraws[textdraw_COMBAT_MODE]);

				SendNotification(playerid, "Has entrado en modo de combate. Saldrás de este modo en ~r~2 minutos~w~.~n~~n~Si te desconectas, perderás todas tus armas, y en caso de tener cargos irás a prisión.");
			}
		}

		if(PLAYER_TEMP[damagedid][pt_WORKING_IN] != WORK_POLICE && ACCOUNT_INFO[damagedid][ac_STATE] != ROLEPLAY_STATE_CRACK && ACCOUNT_INFO[damagedid][ac_STATE] != ROLEPLAY_STATE_JAIL) {
			if(PLAYER_TEMP[damagedid][pt_COMBAT])
			{
				KillTimer(PLAYER_TEMP[damagedid][pt_COMBAT_TIMER]);
				PLAYER_TEMP[damagedid][pt_COMBAT_TIMER] = SetTimerEx("ResetCombat", 60000 * 2, false, "i", damagedid);
			}
			else
			{
				PLAYER_TEMP[damagedid][pt_COMBAT_TIMER] = SetTimerEx("ResetCombat", 60000 * 2, false, "i", damagedid);
				PLAYER_TEMP[damagedid][pt_COMBAT] = true;

				TextDrawShowForPlayer(damagedid, Textdraws[textdraw_COMBAT_MODE]);

				SendNotification(damagedid, "Has entrado en modo de combate. Saldrás de este modo en ~r~2 minutos~w~.~n~~n~Si te desconectas, perderás todas tus armas, y en caso de tener cargos irás a prisión.");
			}
		}
	}
		
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
	{
		if(gettime() > PLAYER_TEMP[playerid][pt_LAST_SAFE_ZONE_WARNING] + 60)
		{
			if(IsPlayerInSafeZone(playerid))
			{
				SendNotification(playerid, "Estás en una zona segura, la policía ha sido avisada de la agresión.");
				
				new city[45], zone[45];
				GetPlayerZones(playerid, city, zone);

				if(!PI[playerid][ac_WANTED_LEVEL]) SetPlayerWantedLevelEx(playerid, 1);

				new message[145];
				format(message, sizeof message, "{"#PRIMARY_COLOR"}[Central policía] {FFFFFF}%s está causando disturbios en {"#PRIMARY_COLOR"}%s, %s.", PLAYER_TEMP[playerid][pt_RP_NAME], city, zone);
				SendPoliceRadioMessage(-1, -1, message);
				
				PLAYER_TEMP[playerid][pt_LAST_SAFE_ZONE_WARNING] = gettime();
			}
		}
	}
    return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float: amount, weaponid, bodypart)
{
    return 1;
}

public OnPlayerSelectDynamicObject(playerid, STREAMER_TAG_OBJECT:objectid, modelid, Float:x, Float:y, Float:z)
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
			
			SendFormatNotification(playerid, "Este objeto fue colocado por ~g~%s.~n~~n~~w~ Usa ~g~'ESC'~w~ para borrar el objeto, o usa los controles para moverlo.", POLICE_OBJECTS[ info[1] ][police_object_USER]);
			PLAYER_TEMP[playerid][pt_SELECTED_POLICE_OBJECT_INDEX] = info[1];
			EditDynamicObject(playerid, objectid);
			return 1;
		}
		default: {
			if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] == CMD_DEVELOPER) {
				EditDynamicObject(playerid, objectid);
			}
		}
	}
	
	return 1;
}

forward ResyncPlayer(playerid);
public ResyncPlayer(playerid)
{
	new Float:x, Float:y, Float:z, Float:angle;
	GetPlayerFacingAngle(playerid, angle);

	new BitStream:bsr = BS_New();
	new BitStream:bsa = BS_New();

	BS_WriteValue(bsr, PR_UINT16, playerid);

	BS_WriteValue(
		bsa,
		PR_UINT16, playerid,
		PR_UINT8, NO_TEAM,
		PR_UINT32, GetPlayerSkin(playerid),
		PR_FLOAT, x,
		PR_FLOAT, y,
		PR_FLOAT, z,
		PR_FLOAT, angle,
		PR_UINT32, PLAYER_TEMP[playerid][pt_PLAYER_COLOR],
		PR_UINT8, GetPlayerFightingStyle(playerid)
	);

	for(new i, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerStreamedIn(playerid, i))
		{
			PR_SendRPC(bsr, i, 163); // RPC_WorldPlayerRemove
			PR_SendRPC(bsa, i, 32); // RPC_WorldPlayerAdd
		}
	}

	BS_Delete(bsr);
	BS_Delete(bsa);
}





public OnPlayerStreamIn(playerid, forplayerid)
{
	if(PLAYER_TEMP[playerid][pt_WANT_TAXI])
	{
		if(GetPlayerState(forplayerid) == PLAYER_STATE_DRIVER)
		{			
			if(!PLAYER_WORKS[forplayerid][WORK_TAXI][pwork_SET]) return 1;
			if(PLAYER_TEMP[forplayerid][pt_WORKING_IN] != WORK_TAXI) return 1;
			SetPlayerMarkerForPlayer(forplayerid, playerid, 0xf2f224FF);
		}
	}

	if(PI[playerid][ac_WANTED_LEVEL] > 0)
	{
		if(!PLAYER_WORKS[forplayerid][WORK_POLICE][pwork_SET]) return 1;
		if(PLAYER_TEMP[forplayerid][pt_WORKING_IN] != WORK_POLICE) return 1;
		SetPlayerMarkerForPlayer(forplayerid, playerid, PlayerWantedColor(playerid));
	}
	
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK)
	{
		if(!PLAYER_WORKS[forplayerid][WORK_MEDIC][pwork_SET]) return 1;
		if(PLAYER_TEMP[forplayerid][pt_WORKING_IN] != WORK_MEDIC) return 1;
		SetPlayerMarkerForPlayer(forplayerid, playerid, 0x4aba00FF);
	}
    return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	if(PLAYER_TEMP[playerid][pt_WANT_TAXI])
	{
		if(GetPlayerState(forplayerid) == PLAYER_STATE_DRIVER)
		{			
			if(!PLAYER_WORKS[forplayerid][WORK_TAXI][pwork_SET]) return 1;
			if(PLAYER_TEMP[forplayerid][pt_WORKING_IN] != WORK_TAXI) return 1;
			SetPlayerMarkerForPlayer(forplayerid, playerid, 0xf2f224FF);
		}
	}
	
	if(PI[playerid][ac_WANTED_LEVEL] > 0)
	{
		if(!PLAYER_WORKS[forplayerid][WORK_POLICE][pwork_SET]) return 1;
		if(PLAYER_TEMP[forplayerid][pt_WORKING_IN] != WORK_POLICE) return 1;
		SetPlayerMarkerForPlayer(forplayerid, playerid, PlayerWantedColor(playerid));
	}
	
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK)
	{
		if(!PLAYER_WORKS[forplayerid][WORK_MEDIC][pwork_SET]) return 1;
		if(PLAYER_TEMP[forplayerid][pt_WORKING_IN] != WORK_MEDIC) return 1;
		SetPlayerMarkerForPlayer(forplayerid, playerid, 0x4aba00FF);
	}
    return 1;
}

forward StartPlayerJob(playerid, work, vehicleid);
public StartPlayerJob(playerid, work, vehicleid)
{
	switch(work)
	{
		case WORK_TAXI:
		{
			SetTaxiPlayerMarkers(playerid);
			ResetVehicleTaxiMeter(vehicleid);
			ShowPlayerTaxiMeter(playerid);
			
			new passenger = GetVehicleFirstPassenger(vehicleid);
			if(passenger != INVALID_PLAYER_ID)
			{
				ShowPlayerTaxiMeter(passenger);
				if(PLAYER_TEMP[passenger][pt_WANT_TAXI])
				{
					PLAYER_TEMP[passenger][pt_WANT_TAXI] = false;
					DisablePlayerTaxiMark(passenger);
				}
				
				TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_ENABLED] = true;
				GetVehiclePos(vehicleid, TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_OLD_X], TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_OLD_Y], TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_OLD_Z]);
				KillTimer(TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_TIMER]);
				TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_TIMER] = SetTimerEx("UpdateVehicleTaximeter", 4000, true, "iii", vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER], passenger);
			}
		}
		case WORK_TRUCK:
		{
			if(TRUCK_VEHICLE[vehicleid][truck_vehicle_LOADED])
			{
				if(TRUCK_VEHICLE[vehicleid][truck_vehicle_DELIVERED]) SendNotification(playerid, "El camión ya ha entregado la mercancía, ve al punto de partida para cobrar.");
				else SendNotification(playerid, "Ve a entregar la mercancía al punto marcado.");
				
				SetPlayerTruckCheckpoint(playerid, vehicleid);
			}
			else SendNotification(playerid, "Para comenzar a trabajar carga el camión en la zona indicada con una flecha amarilla.");
		}
		case WORK_HARVESTER:
		{
			PLAYER_TEMP[playerid][pt_HARVERT_PROCCESS] = 0;
			SendNotification(playerid, "Ve hasta los puntos de control para completar el trabajo.");
			SendNotification(playerid, "Si te bajas de la cosechadora se cancelará el trabajo.");
			Set_HARVEST_Checkpoint(playerid);
		}
		case WORK_FUMIGATOR:
		{	
			PLAYER_TEMP[playerid][pt_FUMIGATOR_PROCCESS] = 0;
			SendNotification(playerid, "Ve hasta los puntos de control para completar el trabajo.");
			SendNotification(playerid, "Si te bajas del avión se cancelará el trabajo.");
			Set_FUMIGATOR_Checkpoint(playerid);
			
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 1;
			UpdateVehicleParams(vehicleid);
		}
		case WORK_LUMBERJACK:
		{
			PLAYER_TEMP[playerid][pt_PLAYER_CARRYING_TREE] = false;
			PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] = -1;
			
			DestroyPlayerKeyPress(playerid);
			DisablePlayerArmedWeapons(playerid);
			RemovePlayerAttachedObject(playerid, 9);
			SetPlayerAttachedObject(playerid, 9, 341, 1, -0.172999, -0.165, -0.053999, 0.0, -7.599999, -9.6);
			
			SendClientMessage(playerid,-1, "{"#SILVER_COLOR"}Los árboles que puedes talar están repartidos por la zona, si te alejas demasiado el trabajo se cancelará.");
		}
		case WORK_TRASH:
		{
			if(PI[playerid][ac_GENDER] == SEX_MALE)
			{
				SetPlayerSkin(playerid, 16);
				for(new i = 0; i != MAX_SU_TOYS; i ++) RemovePlayerAttachedObject(playerid, i);
			}
		}
		case WORK_MECHANIC:
		{
			if(PI[playerid][ac_GENDER] == SEX_MALE)
			{
				SetPlayerSkin(playerid, 50);
				for(new i = 0; i != MAX_SU_TOYS; i ++) RemovePlayerAttachedObject(playerid, i);
			}
		}
		case WORK_POLICE:
		{
			new label_str[128];
			format(label_str, sizeof label_str, "%s | Nº%d | %c. %s", POLICE_RANKS[ PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] ], PI[playerid][ac_PLACA_PD], PLAYER_TEMP[playerid][pt_FIRST_NAME][0], PLAYER_TEMP[playerid][pt_SUB_NAME]);
			
			if(IsValidDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_POLICE_LABEL]))
			{
				DestroyDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_POLICE_LABEL]);
				PLAYER_TEMP[playerid][pt_POLICE_LABEL] = Text3D:INVALID_STREAMER_ID;
			}
			PLAYER_TEMP[playerid][pt_POLICE_LABEL] = CreateDynamic3DTextLabel(label_str, 0xCCCCCCCC, 0.0, 0.0, 0.3, 20.0, playerid, .testlos = true);

			for(new i = 0; i != MAX_SU_TOYS; i ++) RemovePlayerAttachedObject(playerid, i);
			SetWantedPlayerMarkers(playerid);
			SetPlayerSkin(playerid, PI[playerid][ac_POLICE_DUTY]);
		}
		case WORK_PIZZA:
		{
			if(PI[playerid][ac_GENDER] == SEX_MALE)
			{
				SetPlayerSkin(playerid, 155);
				for(new i = 0; i != MAX_SU_TOYS; i ++) RemovePlayerAttachedObject(playerid, i);
				DisablePlayerArmedWeapons(playerid);
			}
			PLAYER_TEMP[playerid][pt_PIZZA_PROCCESS] = 0;
			PLAYER_TEMP[playerid][pt_DELIVERED_PIZZAS] = 0;
			SendNotification(playerid, "Para comenzar a repartir súbete a una moto de la pizzería.");
		}
		case WORK_MEDIC:
		{
			if(PI[playerid][ac_GENDER] == SEX_MALE) SetPlayerSkin(playerid, male_medic_skins[ random(sizeof(male_medic_skins)) ]);
			else SetPlayerSkin(playerid, 308);
			for(new i = 0; i != MAX_SU_TOYS; i ++) RemovePlayerAttachedObject(playerid, i);
			SetInjuredPlayerMarkers(playerid);
		}
	}
	
	PLAYER_TEMP[playerid][pt_WORKING_IN] = work;
	return 1;
}

forward EndPlayerJob(playerid, work, bool:changeskin);
public EndPlayerJob(playerid, work, bool:changeskin)
{
	switch(PLAYER_TEMP[playerid][pt_WORKING_IN])
	{
		case WORK_TAXI:
		{
			if(PLAYER_TEMP[playerid][pt_TAXI_METER_ENABLED])
			{
				SetNormalPlayerMarkers(playerid);
				HidePlayerTaxiMeter(playerid);
			}
		}
		case WORK_TRUCK:
		{
			if(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] != INVALID_VEHICLE_ID)
			{
				if(TRUCK_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][truck_vehicle_DRIVER_USER_ID] == ACCOUNT_INFO[playerid][ac_ID])
				{
					if(TRUCK_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][truck_vehicle_LOADING])
					{
						HidePlayerDialog(playerid);
						KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][9]);
						PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0]);
						PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1]);
						PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2]);
						PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3]);
						SetVehicleToRespawnEx(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID]);
					}
					if(TRUCK_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][truck_vehicle_UNLOADING])
					{
						TRUCK_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][truck_vehicle_UNLOADING] = false;
						KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][9]);
						PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0]);
						PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1]);
						PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2]);
						PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3]);
					}
				}
			}
			if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT]))
			{
				DestroyDynamicCP(PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT]);
				PLAYER_TEMP[playerid][pt_TRUCK_CHECKPOINT] = INVALID_STREAMER_ID;
			}
		}
		case WORK_HARVESTER:
		{
			if(IsValidDynamicRaceCP(PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT]))
			{
				DestroyDynamicRaceCP(PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT]);
				PLAYER_TEMP[playerid][pt_HARVERT_CHECKPOINT] = INVALID_STREAMER_ID;
			}
			if(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] != INVALID_VEHICLE_ID)
			{
				if(WORK_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][work_vehicle_WORK] == WORK_HARVESTER) SetVehicleToRespawnEx(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID]);
			}
		}
		case WORK_FUMIGATOR:
		{
			if(IsValidDynamicRaceCP(PLAYER_TEMP[playerid][pt_FUMIGATOR_CHECKPOINT]))
			{
				DestroyDynamicRaceCP(PLAYER_TEMP[playerid][pt_FUMIGATOR_CHECKPOINT]);
				PLAYER_TEMP[playerid][pt_FUMIGATOR_CHECKPOINT] = INVALID_STREAMER_ID;
			}
			if(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] != INVALID_VEHICLE_ID)
			{
				if(WORK_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][work_vehicle_WORK] == WORK_FUMIGATOR) SetVehicleToRespawnEx(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID]);
			}
		}
		case WORK_LUMBERJACK: CancelLumberjackWork(playerid);
		case WORK_FARMER: CancelPlayerPlanting(playerid);
		case WORK_TRASH:
		{
			if(changeskin)
			{
				SetPlayerSkin(playerid, PI[playerid][ac_SKIN]);
				SetPlayerToys(playerid);
			}
			
			if(PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] != INVALID_VEHICLE_ID)
			{
				if(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_JOB_STARTED])
				{
					if(PLAYER_TEMP[playerid][pt_TRASH_DRIVER])
					{
						SendClientMessage(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], -1, "{"#SILVER_COLOR"}El trabajo se ha cancelado porque tu compañero ha dejado de trabajar.");
						CancelTrashWork(playerid, TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
					}
					if(PLAYER_TEMP[playerid][pt_TRASH_PASSENGER])
					{
						SendClientMessage(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], -1, "{"#SILVER_COLOR"}El trabajo se ha cancelado porque tu compañero ha dejado de trabajar.");
						CancelTrashWork(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], playerid, PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
					}
				}
			}
		}
		case WORK_POLICE:
		{
			PLAYER_TEMP[playerid][pt_POLICE_RADIO] = 0;
			
			if(IsValidDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_POLICE_LABEL]))
			{
				DestroyDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_POLICE_LABEL]);
				PLAYER_TEMP[playerid][pt_POLICE_LABEL] = Text3D:INVALID_STREAMER_ID;
			}
			if(changeskin)
			{
				SetPlayerSkin(playerid, PI[playerid][ac_SKIN]);
				SetPlayerToys(playerid);
				PI[playerid][ac_POLICE_DUTY] = 0;
			}
			SetNormalPlayerMarkers(playerid);
		}
		case WORK_MECHANIC:
		{
			if(changeskin)
			{
				SetPlayerSkin(playerid, PI[playerid][ac_SKIN]);
				SetPlayerToys(playerid);
			}
			SetNormalPlayerMarkers(playerid);
		}
		case WORK_PIZZA:
		{
			if(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] != INVALID_VEHICLE_ID)
			{
				if(PIZZA_VEHICLE[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][pizza_vehicle_DRIVER_AID] == ACCOUNT_INFO[playerid][ac_ID]) SetVehicleToRespawnEx(PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID]);
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
			if(changeskin)
			{
				SetPlayerSkin(playerid, PI[playerid][ac_SKIN]);
				SetPlayerToys(playerid);
			}
		}
		case WORK_MEDIC:
		{
			if(changeskin)
			{
				SetPlayerSkin(playerid, PI[playerid][ac_SKIN]);
				SetPlayerToys(playerid);
			}
			SetNormalPlayerMarkers(playerid);
		}
	}
	
	EnablePlayerArmedWeapons(playerid);
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pworks SET level = %d WHERE id_player = %d AND id_work = %d;", PLAYER_WORKS[playerid][ PLAYER_TEMP[playerid][pt_WORKING_IN] ][pwork_LEVEL], ACCOUNT_INFO[playerid][ac_ID], PLAYER_TEMP[playerid][pt_WORKING_IN]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	PLAYER_TEMP[playerid][pt_WORKING_IN] = WORK_NONE;
	return 1;
}

forward StandUp(medic, playerid);
public StandUp(medic, playerid)
{
	if(PLAYER_WORKS[medic][WORK_MEDIC][pwork_SET] && PLAYER_TEMP[medic][pt_WORKING_IN] == WORK_MEDIC) {
		new work_extra_payment;
		if(work_info[WORK_MEDIC][work_info_EXTRA_PAY] > 0 && work_info[WORK_MEDIC][work_info_EXTRA_PAY_EXP] > 0)
		{
			work_extra_payment = (work_info[WORK_MEDIC][work_info_EXTRA_PAY] * floatround(floatdiv(PLAYER_WORKS[medic][WORK_MEDIC][pwork_LEVEL], work_info[WORK_MEDIC][work_info_EXTRA_PAY_EXP])));
			if(work_info[WORK_MEDIC][work_info_EXTRA_PAY_LIMIT] != 0) if(work_extra_payment > work_info[WORK_MEDIC][work_info_EXTRA_PAY_LIMIT]) work_extra_payment = work_info[WORK_MEDIC][work_info_EXTRA_PAY_LIMIT];
		
			if(ACCOUNT_INFO[medic][ac_VIP]) work_extra_payment += SU_WORK_EXTRA_PAY;
		}
		
		new money = minrand(600, 800) + work_extra_payment;
		GivePlayerCash(medic, money, true, false);
		PLAYER_WORKS[medic][WORK_MEDIC][pwork_LEVEL] ++;
		SendClientMessageEx(medic, -1, "Has salvado la vida de esta persona, has ganado {"#PRIMARY_COLOR"}%s$.", number_format_thousand(money));
	}
	
	ACCOUNT_INFO[playerid][ac_STATE] = ROLEPLAY_STATE_NORMAL;
	SetWeaponsForPlayer(playerid);
	if(ACCOUNT_INFO[playerid][ac_VIP]) SetPlayerHealthEx(playerid, 50.0);
	else SetPlayerHealthEx(playerid, 25.0);
	ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.1, 0, 0, 0, 0, 0, true);
	ClearAnimations(playerid);
	return 1;
}

forward UpdateTerritoryAttack(territory_index);
public UpdateTerritoryAttack(territory_index)
{
	if(!TERRITORIES[territory_index][territory_VALID]) return 1;
	if(!TERRITORIES[territory_index][territory_WAR]) return 1;
	
	if(TERRITORIES[territory_index][territory_WAR_TIME_LEFT] <= 0)
	{
		new old_crew = TERRITORIES[territory_index][territory_CREW_ID];
		
		CREW_INFO[ TERRITORIES[territory_index][territory_CREW_INDEX] ][crew_FIGHTING] = false;
		TERRITORIES[territory_index][territory_CREW_ID] = CREW_INFO[ TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX] ][crew_ID];
		TERRITORIES[territory_index][territory_CREW_INDEX] = TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX];
		
		new r, g, b, a;
		HexToRGBA(CREW_INFO[ TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX] ][crew_COLOR], r, g, b, a);
		TERRITORIES[territory_index][territory_COLOR] = RGBAToHex(r, g, b, 135);
		
		new message[145];
		format(message, sizeof message, "{"#PRIMARY_COLOR"}[BANDAS] {FFFFFF}La banda '%s' ha conquistado un nuevo territorio.", CREW_INFO[ TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX] ][crew_NAME]);
	
		new label_str[256];
		if(TERRITORIES[territory_index][territory_OCCUPIED])
		{
			for(new i = 0; i != MAX_PROPERTIES; i ++)
			{
				if(!PROPERTY_INFO[i][property_VALID]) continue;
				if(!PROPERTY_INFO[i][property_CREW]) continue;
				if(PROPERTY_INFO[i][property_CREW_ID] != old_crew) continue;
				
				if(IsPointInDynamicArea(TERRITORIES[territory_index][territory_AREA], PROPERTY_INFO[i][property_EXT_X], PROPERTY_INFO[i][property_EXT_Y], PROPERTY_INFO[i][property_EXT_Z]))
				{
					PROPERTY_INFO[i][property_CREW_ID] = TERRITORIES[territory_index][territory_CREW_ID];
					
					format
					(
						label_str, 
						sizeof label_str, 
						"\
							Propiedad {"#PRIMARY_COLOR"}#%d\n\n\
							{FFFFFF}Banda: {"#PRIMARY_COLOR"}%s\n\
							{FFFFFF}Presiona {"#PRIMARY_COLOR"}~k~~CONVERSATION_YES~ {FFFFFF}para entrar.\
						", PROPERTY_INFO[i][property_ID], CREW_INFO[ TERRITORIES[territory_index][territory_CREW_INDEX] ][crew_NAME]
					);
					
					UpdateDynamic3DTextLabelText(PROPERTY_INFO[i][property_EXT_LABEL_ID], 0xFFFFFFFF, label_str);
				}
			}
		}
		
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE territories SET id_crew = %d WHERE id = %d;", CREW_INFO[ TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX] ][crew_ID], TERRITORIES[territory_index][territory_ID]);
		mysql_tquery(srp_db, QUERY_BUFFER);
		
		CREW_INFO[ TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX] ][crew_LAST_ATTACK] = gettime();
		CREW_INFO[ TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX] ][crew_FIGHTING] = false;
		TERRITORIES[territory_index][territory_WAR] = false;
		TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX] = 0;
		TERRITORIES[territory_index][territory_WAR_TIME_LEFT] = 0;
		TERRITORIES[territory_index][territory_LAST_ATTACK] = gettime();
		KillTimer(TERRITORIES[territory_index][territory_TIMER]);
		UpdateGangZoneColor(territory_index);
		TERRITORIES[territory_index][territory_OCCUPIED] = true;
		
		new prize = minrand(5000, 7525);
		new prize_message[64];
		format(prize_message, sizeof prize_message, "{"#SILVER_COLOR"}Has ganado %d$ por conquistar este territorio.", prize);
		
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(IsPlayerConnected(i))
			{
				if(PI[i][ac_CREW])
				{
					SendClientMessage(i, -1, message);
					if(IsPlayerInDynamicArea(i, TERRITORIES[territory_index][territory_AREA]))
					{
						if(PI[i][ac_CREW] == TERRITORIES[territory_index][territory_CREW_ID])
						{
							GivePlayerCash(i, prize, true, false);
							SendClientMessage(i, -1, prize_message);
						}
						SetPlayerNormalColor(i);
						
						TextDrawHideForPlayer(i, Textdraws[textdraw_TERRITORY_BOX]);
						TextDrawHideForPlayer(i, TERRITORIES[territory_index][territory_TEXTDRAW]);
					}
				}
			}
		}

		CallLocalFunction("OnCrewCaptureTerritory", "iiii", TERRITORIES[territory_index][territory_CREW_ID], TERRITORIES[territory_index][territory_CREW_INDEX], TERRITORIES[territory_index][territory_ID], territory_index);
		return 1;
	}
	
	
	TERRITORIES[territory_index][territory_WAR_TIME_LEFT] --;
	new message[145];
	format(message, sizeof message, "Conquista:_%s", TimeConvert(TERRITORIES[territory_index][territory_WAR_TIME_LEFT]));
	TextDrawSetString(TERRITORIES[territory_index][territory_TEXTDRAW], message);
	
	
	new attackers_in_area = CountCrewPlayersInTerritory(TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX], territory_index);
	if(attackers_in_area <= 0)
	{
		format(message, sizeof message, "{"#PRIMARY_COLOR"}[BANDAS] {FFFFFF}La banda '%s' no ha podido conquistar el territorio que estaba atacando.", CREW_INFO[ TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX] ][crew_NAME]);

		CREW_INFO[ TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX] ][crew_LAST_ATTACK] = gettime();
		CREW_INFO[ TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX] ][crew_FIGHTING] = false;
		CREW_INFO[ TERRITORIES[territory_index][territory_CREW_INDEX] ][crew_FIGHTING] = false;
		TERRITORIES[territory_index][territory_WAR] = false;
		TERRITORIES[territory_index][territory_ATTACKER_CREW_INDEX] = 0;
		TERRITORIES[territory_index][territory_WAR_TIME_LEFT] = 0;
		TERRITORIES[territory_index][territory_LAST_ATTACK] = gettime();
		KillTimer(TERRITORIES[territory_index][territory_TIMER]);
		UpdateGangZoneColor(territory_index);

		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(IsPlayerConnected(i))
			{
				if(PI[i][ac_CREW])
				{
					SendClientMessage(i, -1, message);
					if(IsPlayerInDynamicArea(i, TERRITORIES[territory_index][territory_AREA]))
					{
						SetPlayerNormalColor(i);
						
						TextDrawHideForPlayer(i, Textdraws[textdraw_TERRITORY_BOX]);
						TextDrawHideForPlayer(i, TERRITORIES[territory_index][territory_TEXTDRAW]);
					}
				}
			}
		}
		return 1;
	}
	
	KillTimer(TERRITORIES[territory_index][territory_TIMER]);
	TERRITORIES[territory_index][territory_TIMER] = SetTimerEx("UpdateTerritoryAttack", 1000, false, "i", territory_index);
	return 1;
}

public OnPlayerEditObject(playerid, playerobject, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ)
{
	if(playerobject)
	{
		if(objectid == PLAYER_TEMP[playerid][pt_PIVOT_OBJECT])
		{
			switch(response)
			{
				case EDIT_RESPONSE_CANCEL:
				{
					if(PLAYER_TEMP[playerid][pt_PIVOT_OBJECT] != INVALID_OBJECT_ID)
					{
						DestroyPlayerObject(playerid, PLAYER_TEMP[playerid][pt_PIVOT_OBJECT]);
						PLAYER_TEMP[playerid][pt_PIVOT_OBJECT] = INVALID_OBJECT_ID;
					}
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][0] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][0];
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][1] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][1];
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][2] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][2];
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][0] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][3];
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][1] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][4];
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][2] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][5];
					UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT]);
					Streamer_Update(playerid);
					
					SendNotification(playerid, "Has cancelado la edición.");
					ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
				}
				case EDIT_RESPONSE_FINAL:
				{
					if(PLAYER_TEMP[playerid][pt_PIVOT_OBJECT] != INVALID_OBJECT_ID)
					{
						DestroyPlayerObject(playerid, PLAYER_TEMP[playerid][pt_PIVOT_OBJECT]);
						PLAYER_TEMP[playerid][pt_PIVOT_OBJECT] = INVALID_OBJECT_ID;
					}
					
					new Float:v_size[3];
					GetVehicleModelInfo(GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_MODELID], VEHICLE_MODEL_INFO_SIZE, v_size[0], v_size[1], v_size[2]);
					
					if	(
							(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][0] >= v_size[0] || -v_size[0] >= VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][0]) || 
							(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][1] >= v_size[1] || -v_size[1] >= VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][1]) ||
							(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][2] >= v_size[2] || -v_size[2] >= VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][2])
						)
					{
						SendNotification(playerid, "La posición del objeto está demasiado lejos del vehículo.");
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][0] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][0];
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][1] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][1];
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][2] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][2];
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][0] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][3];
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][1] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][4];
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][2] = PLAYER_TEMP[playerid][pt_OLD_EDIT_VOBJECT_POS][5];
						UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT]);
						Streamer_Update(playerid);
					}
					
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ATTACHED] = true;
					SendNotification(playerid, "Objeto actualizado.");
					ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
				}
				case EDIT_RESPONSE_UPDATE:
				{
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][0] = fX - GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_POS][0];
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][1] = fY - GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_POS][1];
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OFFSET][2] = fZ - GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_POS][2];
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][0] = fRotX;
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][1] = fRotY;
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ROT][2] = fRotZ;
					UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT]);
				}
			}
		}
	}
	return 1;
}

forward HidePlayerMessage(playerid);
public HidePlayerMessage(playerid)
{
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][17]);
	if(!PLAYER_TEMP[playerid][pt_USER_LOGGED]) return 0;
	PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_MESSAGE], "_");
	PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_MESSAGE]);
	return 1;
}
callbackp:HidePlayerMessageP(playerid)
{
	KillTimer(TimerMensaje[playerid]);
	PlayerTextDrawSetString(playerid, MensajeTD[playerid], "_");
	PlayerTextDrawHide(playerid, MensajeTD[playerid]);
	return 1;
}

forward CarJackingFinish(playerid);
public CarJackingFinish(playerid)
{
	TogglePlayerControllableEx(playerid, true);
	return 1;
}

public OnPlayerCommandReceived(playerid, cmd[], params[], flags) 
{
	if(PLAYER_TEMP[playerid][pt_KICKED]) return 0;
	
	if(PLAYER_TEMP[playerid][pt_GAME_STATE] != GAME_STATE_NORMAL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_HOSPITAL || IsPlayerInWorkTutorial(playerid))
	{
		SendNotification(playerid, "Ahora no puedes usar comandos.");
		return 0;
	}
	if(PLAYER_TEMP[playerid][pt_SELECT_TEXTDRAW]) { SendNotification(playerid, "Ahora no puedes usar comandos, pulsa 'ESCAPE' para cerrar el menú."); return 0; }
	
	
	new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_COMMANDS]);
	if(interval < MIN_SECONDS_BETWEEN_COMMANDS)
	{
		if(ac_Info[CHEAT_COMMAND_SPAMMER][ac_Enabled])
		{
			if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_COMMAND_SPAMMER][p_ac_info_IMMUNITY])
			{
				if(interval < 50)
				{
					if(!ac_Info[CHEAT_COMMAND_SPAMMER][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_COMMAND_SPAMMER, float(interval));
					else
					{
						if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_COMMAND_SPAMMER][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_COMMAND_SPAMMER][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_COMMAND_SPAMMER][p_ac_info_DETECTIONS] = 0;
						else PLAYER_AC_INFO[playerid][CHEAT_COMMAND_SPAMMER][p_ac_info_DETECTIONS] ++;
						
						PLAYER_AC_INFO[playerid][CHEAT_COMMAND_SPAMMER][p_ac_info_LAST_DETECTION] = gettime();
						if(PLAYER_AC_INFO[playerid][CHEAT_COMMAND_SPAMMER][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_COMMAND_SPAMMER][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_COMMAND_SPAMMER, float(interval));
					}
				}
			}
		}
		SendNotification(playerid, "Cálmate."); 
		return 0; 
	}
	
	if(flags)
	{
		if(flags == CMD_DISABLED)
		{
			SendNotification(playerid, "Este comando esta deshabilitado.");
			return 0;
		}

		if(flags > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL])
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "SERVER: Unknown command."); 
			return 0;
		}
		
		/*if(!PLAYER_TEMP[playerid][pt_ADMIN_SERVICE])
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "Debes estar de servicio como admin, usa /duty."); 
			return 0;
		}*/
	}
	
	PLAYER_TEMP[playerid][pt_ANTIFLOOD_COMMANDS] = GetTickCount();
	
	#if CMD_LOGGIN
		printf("[CMD] %s (%d): /%s %s", ACCOUNT_INFO[playerid][ac_NAME], ACCOUNT_INFO[playerid][ac_ID], cmd, params);
	#endif
	return 1;
}

public OnPlayerCommandPerformed(playerid, cmd[], params[], result, flags) 
{ 
    if(result == -1) 
    { 
		SendFormatNotification(playerid, "El comando ~r~\"%s\"~w~ no existe.~n~~n~Utiliza el comando ~r~\"/ayuda\"~w~ para ver información sobre el servidor.", cmd);
        return 0;
    }
    return 1; 
}



forward OnPlayerDownloadedSong(index, response_code, data[]);
public OnPlayerDownloadedSong(index, response_code, data[])
{
	if(!PLAYER_TEMP[index][pt_PLAYER_WAITING_MP3_HTTP]) return 1;
	
	if(response_code == 200)
	{
		if(!strcmp(data, "OK"))
		{
			new url[128];
			format(url, 128, ""DOWNLOADED_YT_PATH"%s.mp3", PLAYER_TEMP[index][pt_VIDEO_ID]);
			PlayMp3(index, url, PLAYER_TEMP[index][pt_VIDEO_NAME]);
		}
		else SendClientMessage(index, -1, "{"#SILVER_COLOR"}Esta canción no se puede reproducir.");
	}
	else SendClientMessage(index, -1, "{"#SILVER_COLOR"}Hubo un problema con el servidor de descarga, inténtelo de nuevo más tarde.");
	
	PLAYER_TEMP[index][pt_PLAYER_WAITING_MP3_HTTP] = false;
	return 1;
}

public OnPlayerRegister(playerid)
{
	if(PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER] != -1) {
		KillTimer(PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER]);
		PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER] = -1;
	}
	PLAYER_TEMP[playerid][pt_USER_LOGGED] = true;

	TogglePlayerSpectatingEx(playerid, false);
	StopAudioStreamForPlayer(playerid);
	ClearPlayerChat(playerid);
	TogglePlayerControllable(playerid, false);
	SendFormatNotification(playerid, "Bienvenido ~b~%s, ~w~esperemos que disfrutes en ~b~"SERVER_NAME"~w~.", PLAYER_TEMP[playerid][pt_RP_NAME]);
	SendNotification(playerid, "Si necesitas ayuda puedes utilizar el comando ~b~/ayuda ~w~o puedes usar el canal de dudas con el comando ~b~/d~w~.");
	return 1;
}

public OnPlayerLogin(playerid)
{
	/*
	if(PI[playerid][ac_CONFIG_SECURE_LOGIN]) {
		if(PI[playerid][ac_CONFIG_SECURE_LOGIN] && strcmp(PLAYER_TEMP[playerid][pt_IP], ACCOUNT_INFO[playerid][ac_IP])) {
			inline OnSecureLoginCheck() {
				new rows;
				if(cache_get_row_count(rows)) {
					if(rows) {
						new date[24];
						getDateTime(date);
						mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player_secure_login SET last_connection = '%e' WHERE ip = '%e' AND id_player = %d;", date, PLAYER_TEMP[playerid][pt_IP], ACCOUNT_INFO[playerid][ac_ID]);
						mysql_tquery(srp_db, QUERY_BUFFER);
						LoginPlayer(playerid);
					}
					else {
						new string[128];
						format(string, sizeof string, "id=%d&ip=%s", ACCOUNT_INFO[playerid][ac_ID], PLAYER_TEMP[playerid][pt_IP]);
						HTTP(playerid, HTTP_POST, SECURE_LOGIN_REQUEST_URL, string, "OnSecureLoginRequestCode");
					}
				}
				else Kick(playerid);
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT ip FROM player_secure_login WHERE ip = '%e' AND id_player = %d;", PLAYER_TEMP[playerid][pt_IP], ACCOUNT_INFO[playerid][ac_ID]);
			MySQL_TQueryInline(srp_db, using inline OnSecureLoginCheck, QUERY_BUFFER);
			return 1;
		}
	}
	*/
	LoginPlayer(playerid);
	return 1;
}

public OnNearPlayerSelected(playerid, to_player, id) {
	switch(id) {
		case NEAR_PLAYERS_POCKET_GIVE: {
			new
				command[128],
				index = GetInvSelectedItem_Index(playerid);
			
			format(command, sizeof command, "/dar alimento %d %d", to_player, index + 1);
			PC_EmulateCommand(playerid, command);
			return Y_HOOKS_BREAK_RETURN_1;
		}
		case NEAR_PLAYERS_POCKET_SELL: {
			ShowSelectPriceDialog(playerid, NEAR_PLAYERS_POCKET_SELL);
			return Y_HOOKS_BREAK_RETURN_1;
		}

		case NEAR_PLAYERS_WEAPON_GIVE: {
			new
				command[128],
				index = GetInvSelectedItem_Index(playerid);
			
			format(command, sizeof command, "/dar arma %d %d", to_player, index);
			PC_EmulateCommand(playerid, command);
			return Y_HOOKS_BREAK_RETURN_1;
		}
		case NEAR_PLAYERS_WEAPON_SELL: {
			ShowSelectPriceDialog(playerid, NEAR_PLAYERS_WEAPON_SELL);
			return Y_HOOKS_BREAK_RETURN_1;
		}
	}
    return Y_HOOKS_CONTINUE_RETURN_1;
}

public OnPriceSelected(playerid, price, id) {
	new
		command[128],
		to_player = GetNP_PlayerLastSelectedId(playerid),
		index = GetInvSelectedItem_Index(playerid);

	switch(id) {
		case NEAR_PLAYERS_POCKET_SELL: {
			format(command, sizeof command, "/vender alimento %d %d %d", to_player, index + 1, price);
			PC_EmulateCommand(playerid, command);
        	return Y_HOOKS_BREAK_RETURN_1;
		}

		case NEAR_PLAYERS_WEAPON_SELL: {
			format(command, sizeof command, "/vender arma %d %d %d", to_player, index, price);
			PC_EmulateCommand(playerid, command);
        	return Y_HOOKS_BREAK_RETURN_1;
		}
	}
    return Y_HOOKS_CONTINUE_RETURN_1;
}

forward ResetCombat(damagedid);
public ResetCombat(damagedid)
{
	PLAYER_TEMP[damagedid][pt_COMBAT] = false;

	KillTimer(PLAYER_TEMP[damagedid][pt_COMBAT_TIMER]);
	PLAYER_TEMP[damagedid][pt_COMBAT_TIMER] = -1;

	TextDrawHideForPlayer(damagedid, Textdraws[textdraw_COMBAT_MODE]);
	
	SendNotification(damagedid, "Ya no estás en combate.");
	return 1;
}

public OnDynamicActorStreamIn(actorid, forplayerid)
{
	for(new i = 0; i < 3; i++)
	{
		if(actorid == INTRO_1[i])
		{
			SetDynamicActorPos(INTRO_1[0], 47.1593, -239.3631, 1.5781);
			SetDynamicActorPos(INTRO_1[1], 48.4112, -236.3764, 1.5781);
			SetDynamicActorPos(INTRO_1[2], 48.0544, -235.0872, 1.5781);

			ClearDynamicActorAnimations(INTRO_1[i]);

			ApplyDynamicActorAnimation(INTRO_1[2], "PED", "IDLE_chat", 4.1, 1, 0, 0, 1, 0);
			ApplyDynamicActorAnimation(INTRO_1[1], "COP_AMBIENT", "Coplook_loop", 4.1, 1, 0, 0, 1, 0);
			ApplyDynamicActorAnimation(INTRO_1[0], "CAR", "Fixn_Car_Loop", 4.1, 1, 0, 0, 1, 0);
			return Y_HOOKS_BREAK_RETURN_1;
		}
	}
	for(new i = 2; i < 6; i++)
	{
		if(actorid == INTRO_2[i])
		{
			SetDynamicActorPos(INTRO_2[2], 417.3246, 2510.1504, 16.4844);
			SetDynamicActorPos(INTRO_2[3], 417.5173, 2508.6733, 16.4844);
			SetDynamicActorPos(INTRO_2[4], 417.3795, 2507.4189, 16.4844);
			SetDynamicActorPos(INTRO_2[5], 412.8536, 2508.1931, 16.4844);

			ClearDynamicActorAnimations(INTRO_2[i]);

			ApplyDynamicActorAnimation(INTRO_2[2], "COP_AMBIENT", "Coplook_loop", 4.1, 1, 0, 0, 1, 0);
			ApplyDynamicActorAnimation(INTRO_2[4], "COP_AMBIENT", "Coplook_loop", 4.1, 1, 0, 0, 1, 0);
			ApplyDynamicActorAnimation(INTRO_2[5], "PED", "HANDSUP", 4.1, 0, 0, 0, 1, 0);
			ApplyDynamicActorAnimation(INTRO_2[3], "PED", "ARRESTGUN", 4.1, 0, 0, 0, 1, 0);
			return Y_HOOKS_BREAK_RETURN_1;
		}
	}
	if(actorid == INTRO_3[2])
	{
		SetDynamicActorPos(INTRO_3[2], 313.3412, 1131.9296, 9.0343);

		ClearDynamicActorAnimations(INTRO_3[2]);
		ApplyDynamicActorAnimation(INTRO_3[2], "BOMBER", "BOMB_PLANT", 4.1, 0, 0, 0, 1, 0);
		return Y_HOOKS_BREAK_RETURN_1;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyPressFinish(playerid) {
	if(PLAYER_TEMP[playerid][pt_PLANTING]) {
		PLAYER_TEMP[playerid][pt_WORKING_IN] = WORK_NONE;
		PLAYER_TEMP[playerid][pt_PLANTING] = false;
		PLAYER_TEMP[playerid][pt_LAST_PLANT_TIME] = gettime();

		TogglePlayerControllableEx(playerid, true);
		ClearAnimations(playerid);

		//objeto planta: 2244
		new index = GetAvaiblePlantIndex();
		if(index == -1)
		{
			switch(seed_info[ PLAYER_TEMP[playerid][pt_PLANTING_PLANT_SELECTED] ][seed_info_PLANT_TYPE])
			{
				case PLANT_TYPE_MEDICINE: PI[playerid][ac_SEED_MEDICINE] += plant_info[ PLAYER_TEMP[playerid][pt_PLANTING_PLANT_SELECTED] ][plant_info_SEEDS];
				case PLANT_TYPE_CANNABIS: PI[playerid][ac_SEED_CANNABIS] += plant_info[ PLAYER_TEMP[playerid][pt_PLANTING_PLANT_SELECTED] ][plant_info_SEEDS];
				case PLANT_TYPE_CRACK: PI[playerid][ac_SEED_CRACK] += plant_info[ PLAYER_TEMP[playerid][pt_PLANTING_PLANT_SELECTED] ][plant_info_SEEDS];
			}

			SendNotification(playerid, "No queda espacio para más plantas, te hemos devuelto las semillas. Prueba ms tarde.");
			return 1;
		}

		GetPlayerPos(playerid, PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z]);
		GetPlayerFacingAngle(playerid, PI[playerid][ac_ANGLE]);
		PI[playerid][ac_POS_X] += (1.0 * floatsin(-PI[playerid][ac_ANGLE], degrees));
		PI[playerid][ac_POS_Y] += (1.0 * floatcos(-PI[playerid][ac_ANGLE], degrees));
		PI[playerid][ac_POS_Z] -= 0.75;

		PLANTS[index][plant_VALID] = true;
		PLANTS[index][plant_GROWING] = true;
		PLANTS[index][plant_INVISIBLE] = true;
		PLANTS[index][plant_GROWING_PROGRESS] = frandom(10.0, 8.0, 2);
		PLANTS[index][plant_TYPE] = PLAYER_TEMP[playerid][pt_PLANTING_PLANT_SELECTED];
		PLANTS[index][plant_PLANTED_BY_ACCOUNT_ID] = ACCOUNT_INFO[playerid][ac_ID];
		PLANTS[index][plant_IMMUNITY] = -1;

		format(PLANTS[index][plant_PLANTED_BY_NAME], 24, "%s", PLAYER_TEMP[playerid][pt_RP_NAME]);
		PLANTS[index][plant_OBJECT_ID] = CreateDynamicObject(2244, PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z], 0.0, 0.0, PI[playerid][ac_ANGLE], 0, 0);
		SetDynamicObjectMaterial(PLANTS[index][plant_OBJECT_ID], 2, 2244, "plants_tabletop", "CJ_PLANT", 0x00FFFFFF);

		new info[3];
		info[0] = OBJECT_TYPE_PLANT;
		info[1] = index;
		Streamer_SetArrayData(STREAMER_TYPE_OBJECT, PLANTS[index][plant_OBJECT_ID], E_STREAMER_EXTRA_ID, info);

		new label_str[256];
		if(seed_info[ PLANTS[index][plant_TYPE] ][seed_info_TYPE] == TYPE_LEGAL) format(label_str, sizeof label_str, "{FF7373}Planta (%s)\n{CCCCCC}Agricultor: %s\n\n{FF7373}(%.1f%%)", plant_info[ PLANTS[index][plant_TYPE] ][plant_info_NAME], PLANTS[index][plant_PLANTED_BY_NAME], PLANTS[index][plant_GROWING_PROGRESS]);
		else format(label_str, sizeof label_str, "{FF7373}Planta (%s)\n{CCCCCC}Agricultor: %s\n\n{FF7373}(%.1f%%)", plant_info[ PLANTS[index][plant_TYPE] ][plant_info_NAME], PLANTS[index][plant_PLANTED_BY_NAME], PLANTS[index][plant_GROWING_PROGRESS]);
		PLANTS[index][plant_LABEL_ID] = CreateDynamic3DTextLabel(label_str, 0xFFFFFFFF, PI[playerid][ac_POS_X], PI[playerid][ac_POS_Y], PI[playerid][ac_POS_Z] + 0.25, 10.0, .testlos = false, .interiorid = 0, .worldid = 0);

		KillTimer(PLANTS[index][plant_TIMER]);
		PLANTS[index][plant_TIMER] = SetTimerEx("GrowPlantUp", 5000, false, "d", index);

		PLAYER_WORKS[playerid][WORK_FARMER][pwork_LEVEL] ++;
		AddPlayerJobPoints(playerid, WORK_FARMER);
		SendNotification(playerid, "Ahora espera a que la planta crezca, si pasan más de ~r~5 minutos~w~ después de que la planta haya crecido se destruirá.");
		Streamer_Update(playerid);
		return Y_HOOKS_BREAK_RETURN_1;
	}
	else if(PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] != -1) {
		new tree = PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE];
		GetPlayerFacingAngle(playerid, PI[playerid][ac_ANGLE]);
		SetDynamicObjectRot(LUMBER_TREES[ tree ][lumber_tree_OBJECT_ID], LUMBER_TREES[ tree ][lumber_tree_RX], LUMBER_TREES[ tree ][lumber_tree_RY], PI[playerid][ac_ANGLE]);	
		MoveDynamicObject(LUMBER_TREES[ tree ][lumber_tree_OBJECT_ID], LUMBER_TREES[ tree ][lumber_tree_X], LUMBER_TREES[ tree ][lumber_tree_Y], LUMBER_TREES[ tree ][lumber_tree_Z] + 0.01, 0.0065, -85.0, 0.0, PI[playerid][ac_ANGLE]);
		RemovePlayerAttachedObject(playerid, 9);
		SetPlayerAttachedObject(playerid, 9, 341, 1, -0.172999, -0.165, -0.053999, 0.0, -7.599999, -9.6);
		
		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][11]);
		PLAYER_TEMP[playerid][pt_TIMERS][11] = SetTimerEx("ContinueTreeAnimation", 1400, false, "iif", playerid, tree, PI[playerid][ac_ANGLE]);
		
		ClearAnimations(playerid);
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		TogglePlayerControllableEx(playerid, true);
		return Y_HOOKS_BREAK_RETURN_1;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyPress(playerid, Float:percentage) {
	if(PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE] != -1) {
		new tree = PLAYER_TEMP[playerid][pt_PLAYER_LUMBERJACK_TREE];
		SetPlayerArmedWeapon(playerid, 9);
		
		new label_str[256];
		format(label_str, sizeof label_str, "{f46530}Árbol (%d)\n\n{FFFFFF}Talando... {"#SILVER_COLOR"}(%.1f%%)", tree, percentage);
		UpdateDynamic3DTextLabelText(LUMBER_TREES[ tree ][lumber_tree_LABEL], -1, label_str);
		
		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][11]);
		PLAYER_TEMP[playerid][pt_TIMERS][11] = SetTimerEx("LumberjackUp", 2000, false, "ii", playerid, tree);
		return Y_HOOKS_BREAK_RETURN_1;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}


