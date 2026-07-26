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

#if defined commands_included 
	#endinput
#endif
#define commands_included




CMD:me(playerid, params[])
{
	if(isnull(params)) return SendNotification(playerid, "~r~Modo de uso: ~w~/me [TEXTO]");
	
	SendPlayerAction(playerid, params);
	return 1;
}
alias:me("y")

CMD:experiencia(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ShowPlayerSkills(playerid, playerid);
	return 1;
}
alias:experiencia("exp", "skills")

CMD:est(playerid, params[])
{
	ShowPlayerStats(playerid, playerid);
	return 1;
}
alias:est("estadisticas", "cuenta")

CMD:web(playerid, params[])
{
	SendClientMessage(playerid, -1, "Web: {a7f442}"SERVER_WEBSITE"");
	return 1;
}

CMD:csave(playerid, params[])
{
	if(gettime() < PLAYER_TEMP[playerid][pt_SAVE_ACCOUNT_TIME] + 30)
	{
		new time = (30-(gettime()-PLAYER_TEMP[playerid][pt_SAVE_ACCOUNT_TIME]));
		SendFormatNotification(playerid, "Tienes que esperar %s minutos para volver a guardar tus datos.", TimeConvert(time));
		return 1;
	}

	SavePlayerData(playerid);
	SavePlayerVehicles(playerid, false);
	SavePlayerToys(playerid);
	SavePlayerWeaponsData(playerid);
	SavePlayerWorkLevels(playerid);
	PLAYER_TEMP[playerid][pt_SAVE_ACCOUNT_TIME] = gettime();
	
	SendNotification(playerid, "Datos de la cuenta guardados.");
	return 1;
}

CMD:ayuda(playerid, params[])
{
	switch(YHash(params, false))
	{
		case _I<general>: ShowDialog(playerid, DIALOG_HELP_GENERAL);
		case _I<informacion>: ShowDialog(playerid, DIALOG_HELP_INFO);
		case _I<personaje>: ShowDialog(playerid, DIALOG_HELP_CHARACTER);
		case _I<propiedades>: ShowDialog(playerid, DIALOG_HELP_PROPERTIES);
		case _I<reportar>: ShowDialog(playerid, DIALOG_HELP_REPORT);
		case _I<trabajos>: ShowDialog(playerid, DIALOG_HELP_WORKS);
		case _I<vehiculos>: ShowDialog(playerid, DIALOG_HELP_VEHICLES);
		case _I<bandas>: ShowDialog(playerid, DIALOG_HELP_CREWS);
		case _I<vip>: ShowDialog(playerid, DIALOG_HELP_SU);
		case _I<verificacion>: ShowDialog(playerid, DIALOG_HELP_SECURE_LOGIN);

		default: ShowDialog(playerid, DIALOG_HELP);
	}
	return 1;
}

CMD:accesorios(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ShowDialog(playerid, DIALOG_PLAYER_TOYS);
	return 1;
}

CMD:armas(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ShowDialog(playerid, DIALOG_PLAYER_WEAPONS);
	return 1;
}

CMD:ropa(playerid, params[])
{
	if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] == -1) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] == INTERIOR_NO_INFO) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] = GetClothingShopIndexByIntType(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE]);
	if(PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] == -1) return SendNotification(playerid, "No estás en el lugar adecuado.");

	if(!IsPlayerInRangeOfPoint(playerid, 1.0, Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_X], Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_Y], Clothing_Shop_Positions[PLAYER_TEMP[playerid][pt_CLOTHING_SHOP]][clothing_shop_Z])) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	PLAYER_TEMP[playerid][pt_CLOTHING_SHOP_SELECTED_SKIN] = 0;
	SetPlayerPosEx(playerid, 257.251373, -40.797775, 1002.023437, 135.0, 14, playerid + MAX_PLAYERS, false); // World = Playerid + Max_players
	TogglePlayerControllableEx(playerid, false);
	
	SetPlayerCameraPos(playerid, 254.988632, -43.060516, 1002.9131);
	SetPlayerCameraLookAt(playerid, 257.251373, -40.797775, 1002.023437);
	
	UpdateClothingShop(playerid);
	ShowClothingShopTextdraws(playerid);
	
	SendNotification(playerid, "Pulsa ~r~'ESCAPE' ~w~para dejar de probarte ropa.");
	SelectTextDrawEx(playerid, 0xe8d08fFF);
	return 1;
}

CMD:toys(playerid, params[])
{
	if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] == -1) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] == INTERIOR_NO_INFO) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	new shop = GetClothingShopIndexByIntType(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE]);
	if(shop == -1) return SendNotification(playerid, "No estás en el lugar adecuado.");

	if(!IsPlayerInRangeOfPoint(playerid, 1.0, Clothing_Shop_Positions[shop][clothing_shop_X], Clothing_Shop_Positions[shop][clothing_shop_Y], Clothing_Shop_Positions[shop][clothing_shop_Z])) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	PLAYER_TEMP[playerid][pt_TOYS_SHOP] = true;
	
	SetPlayerVirtualWorld(playerid, playerid + MAX_PLAYERS);
	TogglePlayerControllableEx(playerid, false);
	
	UpdateToysShop(playerid);
	ShowToysShopTextdraws(playerid);
	
	SendNotification(playerid, "Pulsa ~r~'ESCAPE' ~w~para salir del menú.");
	SelectTextDrawEx(playerid, 0xe8d08fFF);
	return 1;
}

CMD:pedir(playerid, params[])
{
	new index = GetPlayerFastFoodIndex(playerid);
	if(index == -10) return SendNotification(playerid, "No estás en el lugar adecuado.");

	switch(Fast_Food_Positions[index][fast_food_INTERIOR_TYPE])
	{
		case INTERIOR_BURGER_SHOT: ShowDialog(playerid, DIALOG_FOOD_BURGER);
		case INTERIOR_PIZZA: ShowDialog(playerid, DIALOG_FOOD_PIZZA);
		case INTERIOR_CLUCKIN_BELL: ShowDialog(playerid, DIALOG_FOOD_CLUCKIN);
		case -1: ShowDialog(playerid, DIALOG_FOOD_BURGER);
	}
	return 1;
}

CMD:productos(playerid, params[])
{
	if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] == -1) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] != INTERIOR_247) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	if(!IsPlayerInRangeOfPoint(playerid, 1.0, -27.964675, -89.948631, 1003.546875)) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	ShowDialog(playerid, DIALOG_247_LIST);
	return 1;
}

CMD:mercado(playerid, params[])
{
	if(!IsPlayerInRangeOfPoint(playerid, 1.0, 2119.059814, -2001.701904, 7.984375)) return SendNotification(playerid, "No estás en el lugar adecuado.");	
	if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres bienvenido aquí");
	ShowDialog(playerid, DIALOG_BLACK_MARKET);
	return 1;
}

CMD:gps(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(!PI[playerid][ac_GPS])
	{
		SendNotification(playerid, "No tienes ningún GPS, puedes ir a cualquier 24/7 para comprar uno.");
		return 1;
	}
	
	Auto_SendPlayerAction(playerid, "mira su GPS.");
	ShowDialog(playerid, DIALOG_PLAYER_GPS);
	return 1;
}

CMD:tiempo(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL)
	{
		new time = PI[playerid][ac_POLICE_JAIL_TIME] - (gettime() - PLAYER_TEMP[playerid][pt_ENTER_JAIL_TIME]);
		if(time <= 0)
		{
			UnjailPlayer(playerid);
			return 1;
		}
		SendFormatNotification(playerid, "Te quedan %s minutos de condena.", TimeConvert(time));
	}
	else SendNotification(playerid, "No estás en la cárcel.");
	return 1;
}

CMD:mapa(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(!PI[playerid][ac_GPS])
	{
		SendNotification(playerid, "No tienes ningún GPS, puedes ir a cualquier 24/7 para comprar uno.");
		return 1;
	}
	
	if(PLAYER_TEMP[playerid][pt_GPS_MAP]) HidePlayerGpsMap(playerid);
	else ShowPlayerGpsMap(playerid);
	return 1;
}

CMD:mp3(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(!PI[playerid][ac_MP3])
	{
		SendNotification(playerid, "No tienes ningún MP3, puedes ir a cualquier 24/7 para comprar uno.");
		PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
		return 1;
	}
	if(PLAYER_TEMP[playerid][pt_PLAYER_WAITING_MP3_HTTP]) return SendNotification(playerid, "Espera que termine la búsqueda actual.");
	
	PLAYER_TEMP[playerid][pt_MUSIC_FOR_PROPERTY] = false;
	PLAYER_TEMP[playerid][pt_MUSIC_FOR_VEHICLE] = false;
	PLAYER_TEMP[playerid][pt_MUSIC_FOR_SPEAKERS] = false;
	Auto_SendPlayerAction(playerid, "busca música en su MP3.");
	ShowDialog(playerid, DIALOG_PLAYER_MP3);
	return 1;
}

CMD:amp3(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	//if(GetPlayerInterior(playerid)) return SendNotification(playerid, "No puedes hacer esto en un interior");
	if(!PI[playerid][ac_SPEAKERS])
	{
		SendNotification(playerid, "No tienes altavoces, puedes ir a cualquier 24/7 para comprarlos.");
		PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
		return 1;
	}
	if(PLAYER_TEMP[playerid][pt_PLAYER_WAITING_MP3_HTTP]) return SendNotification(playerid, "Espera que termine la búsqueda actual.");
	
	if(gettime() < PLAYER_TEMP[playerid][pt_SPEAKERS_TIME] + 120)
	{
		new time = (120-(gettime()-PLAYER_TEMP[playerid][pt_SPEAKERS_TIME]));
		SendFormatNotification(playerid, "Tienes que esperar %s minutos para volver a utilizar los altavoces.", TimeConvert(time));
		return 1;
	}

	PLAYER_TEMP[playerid][pt_MUSIC_FOR_SPEAKERS] = true;
	Auto_SendPlayerAction(playerid, "busca música en su MP3.");
	ShowDialog(playerid, DIALOG_PLAYER_MP3);
	return 1;
}

CMD:vmp3(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendNotification(playerid, "No conduces ningún vehículo.");
	if(!PI[playerid][ac_MP3])
	{
		SendNotification(playerid, "No tienes ningún MP3, puedes ir a cualquier 24/7 para comprar uno.");
		return 1;
	}
	if(PLAYER_TEMP[playerid][pt_PLAYER_WAITING_MP3_HTTP]) return SendNotification(playerid, "Espera que termine la búsqueda actual.");
	
	PLAYER_TEMP[playerid][pt_MUSIC_FOR_VEHICLE] = true;
	Auto_SendPlayerAction(playerid, "busca música en su MP3.");
	ShowDialog(playerid, DIALOG_PLAYER_MP3);
	return 1;
}

CMD:stop(playerid, params[])
{
	StopAudioStreamForPlayer(playerid);
	return 1;
}

CMD:pass(playerid, params[])
{
	ShowDialog(playerid, DIALOG_CHANGE_PASSWORD);
	return 1;
}
alias:pass("contra", "cpass")

CMD:gcp(playerid, params[])
{
	if(IsValidDynamicCP(PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT]))
	{
		DestroyDynamicCP(PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT]);
		PLAYER_TEMP[playerid][pt_GPS_CHECKPOINT] = INVALID_STREAMER_ID;
		
		SendNotification(playerid, "Has cancelado la ruta antigua de tu GPS.");
	}
	else SendNotification(playerid, "No tienes ningún punto marcado en tu GPS.");
	return 1;
}

CMD:movil(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(!PI[playerid][ac_PHONE_NUMBER])
	{
		SendNotification(playerid, "No tienes ningún teléfono, puedes ir a cualquier 24/7 para comprar uno.");
		return 1;
	}
	if(PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL]) SendFormatNotification(playerid, "Estás en una llamada.");
	
	if(!PLAYER_TEMP[playerid][pt_PHONE_COMMANDS_MESSAGE])
	{
		PLAYER_TEMP[playerid][pt_PHONE_COMMANDS_MESSAGE] = true;
		SendNotification(playerid, "Recuerda que también puedes usar ~y~/agenda~w~, ~y~/llamar~w~, ~y~/sms~w~, ~y~/tienda~w~.");
	}
	
	Auto_SendPlayerAction(playerid, "mira su teléfono.");
	ShowDialog(playerid, DIALOG_PHONE);
	return 1;
}
alias:movil("celular", "telefono", "tlf")

CMD:guia(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(!PI[playerid][ac_PHONE_RESOLVER])
	{
		SendNotification(playerid, "No tienes una guía telefónica, puedes ir a cualquier 24/7 para comprar una.");
		return 1;
	}
	
	if(sscanf(params, "u", params[0])) return SendNotification(playerid, "~r~Modo de uso: ~w~/guia [PlayerID/Nombre]");
	if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "Jugador desconectado.");
	
	if(!PI[params[0]][ac_PHONE_NUMBER]) return SendNotification(playerid, "Este jugador no tiene teléfono.");
	if(!PI[params[0]][ac_PHONE_VISIBLE_NUMBER]) return SendNotification(playerid, "Este jugador ha decidido no mostrar su número en la guía.");
	
	SendFormatNotification(playerid, "Persona encontrada:~n~~n~~b~Nombre: ~w~%s~n~~b~Teléfono: ~w~%d.", PLAYER_TEMP[params[0]][pt_RP_NAME], PI[params[0]][ac_PHONE_NUMBER]);
	return 1;
}

CMD:sguia(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(!PI[playerid][ac_PHONE_NUMBER])
	{
		SendNotification(playerid, "No tienes ningún teléfono, puedes ir a cualquier 24/7 para comprar uno.");
		return 1;
	}
	
	if(PI[playerid][ac_PHONE_VISIBLE_NUMBER])
	{
		PI[playerid][ac_PHONE_VISIBLE_NUMBER] = false;
		SendNotification(playerid, "Tu número no aparecerá ahora en la guía telefónica.");
	}
	else
	{
		PI[playerid][ac_PHONE_VISIBLE_NUMBER] = true;
		SendNotification(playerid, "Tu número aparecerá en la guía telefónica.");
	}
	return 1;
}

CMD:sms(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(!PI[playerid][ac_PHONE_NUMBER])
	{
		SendNotification(playerid, "No tienes ningún teléfono, puedes ir a cualquier 24/7 para comprar uno.");
		return 1;
	}
	if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendFormatNotification(playerid, "Tu teléfono está apagado, para encenderlo usa /movil.");
	if(PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL]) return SendFormatNotification(playerid, "Estás en una llamada.");
	
	new params_message[64], params_number, params_contact[24];
	if(!sscanf(params, "ds[64]", params_number, params_message))
	{
		if(params_number <= 0) SendNotification(playerid, "El número destino no es válido.");
		else
		{
			inline OnPhoneChecked()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					if(rows)
					{
						new id, connected, pid;
						cache_get_value_name_int(0, "id", id);
						cache_get_value_name_int(0, "connected", connected);
						cache_get_value_name_int(0, "playerid", pid);

						if(connected) RegisterPhoneMessage(ACCOUNT_INFO[playerid][ac_ID], id, params_message, 0);
						else RegisterPhoneMessage(ACCOUNT_INFO[playerid][ac_ID], id, params_message, 1);
						if(connected)
						{
							if(PI[pid][ac_PHONE_STATE] == PHONE_STATE_ON)
							{
								SendClientMessageEx(pid, -1, "{"#PRIMARY_COLOR"}[Nuevo mensaje recibido] {"#SILVER_COLOR"}Remitente: {FFFFFF}%s {"#SILVER_COLOR"}Mensaje: {FFFFFF}%s", convertPhoneNumber(pid, PI[playerid][ac_PHONE_NUMBER]), params_message);
							}
						}
						SendFormatNotification(playerid, "Mensaje enviado a ~b~%s~w~.", convertPhoneNumber(playerid, params_number));
					}
					else SendNotification(playerid, "El número destino no es válido.");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, connected, playerid FROM player WHERE phone_number = %d;", params_number);
			MySQL_TQueryInline(srp_db, using inline OnPhoneChecked, QUERY_BUFFER);
		}
	}
	else if(!sscanf(params, "s[24]s[64]", params_contact, params_message))
	{
		inline OnPhoneChecked()
		{
			new rows;
			if(cache_get_row_count(rows))
			{
				if(rows)
				{
					new number, id, connected, pid, bool:isnull_id;
					cache_is_value_name_null(0, "id", isnull_id);
					if(!isnull_id) {
						cache_get_value_name_int(0, "number", number);
						cache_get_value_name_int(0, "id", id);
						cache_get_value_name_int(0, "connected", connected);
						cache_get_value_name_int(0, "playerid", pid);

						if(connected) RegisterPhoneMessage(ACCOUNT_INFO[playerid][ac_ID], id, params_message, 0);
						else RegisterPhoneMessage(ACCOUNT_INFO[playerid][ac_ID], id, params_message, 1);

						if(connected)
						{
							if(PI[pid][ac_PHONE_STATE] == PHONE_STATE_ON)
							{
								SendClientMessageEx(pid, -1, "{"#PRIMARY_COLOR"}[Nuevo mensaje recibido] {"#SILVER_COLOR"}Remitente: {FFFFFF}%s {"#SILVER_COLOR"}Mensaje: {FFFFFF}%s", convertPhoneNumber(pid, PI[playerid][ac_PHONE_NUMBER]), params_message);
							}
						}
						SendFormatNotification(playerid, "Mensaje enviado a ~b~%s~w~.", convertPhoneNumber(playerid, number));
					}
					else SendNotification(playerid, "El número destino no es válido.");
				}
				else SendNotification(playerid, "El número destino no es válido.");
			}
		}
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT pbook.number, player.id, player.connected, player.playerid FROM pbook LEFT JOIN player ON pbook.number = player.phone_number WHERE pbook.id_player = %d AND pbook.name LIKE '%%%e%%' LIMIT 1;", ACCOUNT_INFO[playerid][ac_ID], params_contact);
		MySQL_TQueryInline(srp_db, using inline OnPhoneChecked, QUERY_BUFFER);
	}
	else SendNotification(playerid, "Usa /sms [numero o contacto]");
	return 1;
}

CMD:agenda(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(!PI[playerid][ac_PHONE_NUMBER])
	{
		SendNotification(playerid, "No tienes ningún teléfono, puedes ir a cualquier 24/7 para comprar uno.");
		return 1;
	}
	if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendFormatNotification(playerid, "Tu teléfono está apagado, para encenderlo usa /movil.");
	if(PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL]) return SendFormatNotification(playerid, "Estás en una llamada.");
	
	ShowDialog(playerid, DIALOG_PHONE_BOOK);
	return 1;
}

CMD:tienda(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(!PI[playerid][ac_PHONE_NUMBER])
	{
		SendNotification(playerid, "No tienes ningún teléfono, puedes ir a cualquier 24/7 para comprar uno.");
		return 1;
	}
	
	if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendFormatNotification(playerid, "Tu teléfono está apagado, para encenderlo usa /movil.");
	ShowPlayerShop(playerid);
	return 1;
}

HandleStaticPhoneNumbers(playerid, call_number)
{
	switch(call_number)
	{
		case 911:
		{
			if(ACCOUNT_INFO[playerid][ac_LEVEL] < 2) return SendNotification(playerid, "Debes ser al menos nivel 2 para llamar a la policía.");
			PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL] = true;
			PLAYER_TEMP[playerid][pt_POLICE_CALL_NAME] = true;
			PLAYER_TEMP[playerid][pt_POLICE_CALL_DESCRIPTION] = false;
			
			SendClientMessage(playerid, -1, "{"#SILVER_COLOR"}Operadora: ha llamado a la central de policía, indícanos su nombre por favor.");
			return 1;
		}
		case 450: //Taxi
		{
			if(PLAYER_TEMP[playerid][pt_WANT_TAXI])
			{
				PLAYER_TEMP[playerid][pt_WANT_TAXI] = false;
				DisablePlayerTaxiMark(playerid);
				SendClientMessage(playerid, -1, "{"#SILVER_COLOR"}Has cancelado el taxi.");
			}
			else
			{
				PLAYER_TEMP[playerid][pt_WANT_TAXI] = true;
				SendClientMessage(playerid, -1, "{"#SILVER_COLOR"}Tu llamada se ha enviado a los taxistas disponibles, espera a que llegue un taxi.");
				SendClientMessage(playerid, -1, "{"#SILVER_COLOR"}Vuelve a llamar a este número si ya no necesitas un taxi.");
				SendAlertToTaxiDrivers(playerid);
			}
			return 1;
		}
		case 6740:
		{
			SendClientMessage(playerid, -1, "{"#SILVER_COLOR"}No vuelvas a llamar al banco, cuidado.");
			return 1;
		}
		case 38350:
		{
			SendClientMessage(playerid, -1, "{"#SILVER_COLOR"}Aseguradora que asegura tus vehículos de mierda.");
			return 1;
		}
		case 666:
		{
			SendClientMessage(playerid, -1, "{"#SILVER_COLOR"}Mirar a las estrellas es mirar al pasado, no mires.");
			return 1;
		}
	}
	return 0;
}

CMD:llamar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(!PI[playerid][ac_PHONE_NUMBER])
	{
		SendNotification(playerid, "No tienes ningún teléfono, puedes ir a cualquier 24/7 para comprar uno.");
		return 1;
	}
	if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendFormatNotification(playerid, "Tu teléfono está apagado, para encenderlo usa /movil.");
	if(PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL]) return SendFormatNotification(playerid, "Estás en una llamada.");

	new params_number, params_contact[24];
	if(!sscanf(params, "d", params_number))
	{
		if(params_number <= 0) SendNotification(playerid, "El número destino no es válido.");
		else
		{
			if(!HandleStaticPhoneNumbers(playerid, params_number))
			{
				inline OnPhoneChecked()
				{
					new rows;
					if(cache_get_row_count(rows))
					{
						if(rows)
						{
							new id, connected, pid;
							cache_get_value_name_int(0, "id", id);
							cache_get_value_name_int(0, "connected", connected);
							cache_get_value_name_int(0, "playerid", pid);

							if(!connected) SendNotification(playerid, "El número al que intentas llamar está fuera de cobertura (desconectado).");
							else
							{
								if(pid == playerid) return SendNotification(playerid, "El número al que intentas llamar es el tuyo.");
								if(PI[pid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendNotification(playerid, "El número al que intentas llamar está apagado o fuera de cobertura.");
								if(PLAYER_TEMP[pid][pt_PLAYER_IN_CALL]) return SendNotification(playerid, "El número al que intentas llamar está en otra llamada.");
								
								SendFormatNotification(pid, "~b~[Llamada entrante] ~w~%s te está llamando, para responser usa ~b~/responder ~w~o ~b~/colgar~w~.", convertPhoneNumber(pid, PI[playerid][ac_PHONE_NUMBER]));
								PLAYER_TEMP[pid][pt_PLAYER_IN_CALL] = true;
								PLAYER_TEMP[pid][pt_PLAYER_PHONE_CALL_STATE] = CALL_STATE_INCOMING_CALL;
								PLAYER_TEMP[pid][pt_PLAYER_PHONE_CALL_PLAYERID] = playerid;
								
								SendFormatNotification(playerid, "Llamando a ~b~%s~w~...", convertPhoneNumber(playerid, params_number));
								PLAYER_TEMP[playerid][pt_TIMERS][6] = SetTimerEx("NoCallResponse", 15000, false, "i", playerid);
								PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL] = true;
								PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_STATE] = CALL_STATE_WAITING_RESPONSE;
								PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID] = pid;
								PlayerPlaySoundEx(playerid, 3600, 0.0, 0.0, 0.0);
							}
						}
						else SendNotification(playerid, "El número destino no es válido.");
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, connected, playerid FROM player WHERE phone_number = %d;", params_number);
				MySQL_TQueryInline(srp_db, using inline OnPhoneChecked, QUERY_BUFFER);
			}
		}
	}
	else if(!sscanf(params, "s[24]", params_contact))
	{
		if(!strcmp(params_contact, "policia", true)) HandleStaticPhoneNumbers(playerid, 911);
		else if(!strcmp(params_contact, "taxi", true)) HandleStaticPhoneNumbers(playerid, 450);
		else if(!strcmp(params_contact, "mecanico", true)) HandleStaticPhoneNumbers(playerid, 570);
		else
		{
			inline OnPhoneChecked()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					if(rows)
					{
						new number, id, connected, pid, bool:isnull_id;
						cache_is_value_name_null(0, "id", isnull_id);
						if(!isnull_id) {
							cache_get_value_name_int(0, "number", number);
							cache_get_value_name_int(0, "id", id);
							cache_get_value_name_int(0, "connected", connected);
							cache_get_value_name_int(0, "playerid", pid);

							if(!connected) SendNotification(playerid, "El número al que intentas llamar está fuera de cobertura (desconectado).");
							else
							{
								if(pid == playerid) return SendNotification(playerid, "El número al que intentas llamar es el tuyo.");
								if(PI[pid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendNotification(playerid, "El número al que intentas llamar está apagado o fuera de cobertura.");
								if(PLAYER_TEMP[pid][pt_PLAYER_IN_CALL]) return SendNotification(playerid, "El número al que intentas llamar está en otra llamada.");
								
								SendFormatNotification(pid, "~b~[Llamada entrante] ~w~%s te está llamando, para responser usa ~b~/responder ~w~o ~b~/colgar~w~.", convertPhoneNumber(pid, PI[playerid][ac_PHONE_NUMBER]));
								PLAYER_TEMP[pid][pt_PLAYER_IN_CALL] = true;
								PLAYER_TEMP[pid][pt_PLAYER_PHONE_CALL_STATE] = CALL_STATE_INCOMING_CALL;
								PLAYER_TEMP[pid][pt_PLAYER_PHONE_CALL_PLAYERID] = playerid;
								
								SendFormatNotification(playerid, "Llamando a ~b~%s~w~...", convertPhoneNumber(playerid, number));
								PLAYER_TEMP[playerid][pt_TIMERS][6] = SetTimerEx("NoCallResponse", 15000, false, "i", playerid);
								PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL] = true;
								PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_STATE] = CALL_STATE_WAITING_RESPONSE;
								PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID] = pid;
								PlayerPlaySoundEx(playerid, 3600, 0.0, 0.0, 0.0);
							}
						}
						else SendNotification(playerid, "El número destino no es válido.");
					}
					else SendNotification(playerid, "El número destino no es válido.");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT pbook.number, player.id, player.connected, player.playerid FROM pbook LEFT JOIN player ON pbook.number = player.phone_number WHERE pbook.id_player = %d AND pbook.name LIKE '%%%e%%' LIMIT 1;", ACCOUNT_INFO[playerid][ac_ID], params_contact);
			MySQL_TQueryInline(srp_db, using inline OnPhoneChecked, QUERY_BUFFER);
		}
	}
	else SendNotification(playerid, "Usa /llamar [numero o contacto]");
	return 1;
}

CMD:responder(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(!PI[playerid][ac_PHONE_NUMBER])
	{
		SendNotification(playerid, "No tienes ningún teléfono, puedes ir a cualquier 24/7 para comprar uno.");
		return 1;
	}
	if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendFormatNotification(playerid, "Tu teléfono está apagado, para encenderlo usa /movil.");
	
	if(!PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL]) return SendFormatNotification(playerid, "No hay ninguna llamada entrante.");
	if(PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID] == INVALID_PLAYER_ID) return SendFormatNotification(playerid, "No hay ninguna llamada entrante."); 
	if(PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_STATE] != CALL_STATE_INCOMING_CALL) return SendFormatNotification(playerid, "No hay ninguna llamada entrante.");
	
	new to_playerid = PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID];
	
	PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL] = true;
	PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_STATE] = CALL_STATE_ESTABLISHED;
	PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID] = to_playerid;
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
	
	KillTimer(PLAYER_TEMP[to_playerid][pt_TIMERS][6]);
	PLAYER_TEMP[to_playerid][pt_PLAYER_IN_CALL] = true;
	PLAYER_TEMP[to_playerid][pt_PLAYER_PHONE_CALL_STATE] = CALL_STATE_ESTABLISHED;
	PLAYER_TEMP[to_playerid][pt_PLAYER_PHONE_CALL_PLAYERID] = playerid;
	SetPlayerSpecialAction(to_playerid, SPECIAL_ACTION_USECELLPHONE);
	
	SendNotification(playerid, "Llamada establecida, usa ~r~/colgar ~w~para terminarla.");
	SendNotification(to_playerid, "Llamada establecida, usa ~r~/colgar ~w~para terminarla.");
	return 1;
}

CMD:colgar(playerid, params[])
{
	if(!PI[playerid][ac_PHONE_NUMBER])
	{
		SendNotification(playerid, "No tienes ningún teléfono, puedes ir a cualquier 24/7 para comprar uno.");
		return 1;
	}
	if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendFormatNotification(playerid, "Tu teléfono está apagado, para encenderlo usa /movil.");
	
	if(!PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL]) return SendFormatNotification(playerid, "No hay ninguna llamada entrante.");
	
	if(PLAYER_TEMP[playerid][pt_POLICE_CALL_NAME] || PLAYER_TEMP[playerid][pt_POLICE_CALL_DESCRIPTION])
	{
		PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL] = false;
		PLAYER_TEMP[playerid][pt_POLICE_CALL_NAME] = false;
		PLAYER_TEMP[playerid][pt_POLICE_CALL_DESCRIPTION] = false;
		SendNotification(playerid, "~r~Llamada finalizada~w~.");
		return 1;
	}
	
	if(PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID] == INVALID_PLAYER_ID) return SendFormatNotification(playerid, "No hay ninguna llamada entrante."); 
	
	switch(PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_STATE])
	{
		case CALL_STATE_WAITING_RESPONSE:
		{
			EndPhoneCall(playerid);
			SendNotification(playerid, "Colgaste.");
		}
		case CALL_STATE_INCOMING_CALL:
		{
			EndPhoneCall(playerid);
			SendNotification(playerid, "No has respondido la llamada.");
		}
		case CALL_STATE_ESTABLISHED:
		{
			EndPhoneCall(playerid);
			SendNotification(playerid, "~r~Llamada finalizada~w~.");
		}
		default: SendFormatNotification(playerid, "No estás en ninguna llamada.");	
	}
	return 1;
}

CMD:panel(playerid, params[])
{
	ShowDialog(playerid, DIALOG_PLAYER_CONFIG);
	return 1;
}
alias:panel("config", "configuracion", "ajustes")

CMD:comprarcasa(playerid, params[])
{
	if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] == -1) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] == INTERIOR_NO_INFO) return SendNotification(playerid, "No estás en el lugar adecuado.");

	
	new site_index = GetBuyPropertySiteIndexByIntTyp(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE]);
	if(site_index == -1) return SendNotification(playerid, "No estás en el lugar adecuado.");

	if(!IsPlayerInRangeOfPoint(playerid, 1.0, BUY_PROPERTIES_SITES[site_index][site_X], BUY_PROPERTIES_SITES[site_index][site_Y], BUY_PROPERTIES_SITES[site_index][site_Z])) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	if(PI[playerid][ac_BANK_ACCOUNT] == 0) return SendNotification(playerid, "No puedes comprar una casa sin tener cuenta bancaria.");

	if(sscanf(params, "d", params[0])) return SendNotification(playerid, "~r~Modo de uso: ~w~/comprarcasa [ID]");
	if(params[0] <= 0) return SendNotification(playerid, "Error: ID no es válida.");
	
	new player_properties = CountPlayerProperties(playerid);
	if(ACCOUNT_INFO[playerid][ac_VIP])
	{
		if(player_properties >= MAX_SU_PROPERTIES) return SendNotification(playerid, "No puedes comprar más propiedades.");
	}
	else
	{
		if(player_properties >= MAX_NU_PROPERTIES)
		{
			SendFormatNotification(playerid, "¡Los jugadores ~y~VIP ~w~pueden tener hasta %d propiedades! Usa ~y~/ayuda ~w~si quieres ser ~w~VIP.", MAX_SU_PROPERTIES);
			SendNotification(playerid, "No puedes comprar más propiedades.");
			return 1;
		}
	}
	
	new index = GetPropertyIndexByID(params[0]);
	if(index == -1) return SendNotification(playerid, "Error: ID no es válida.");
	
	if(PROPERTY_INFO[index][property_SOLD]) return SendNotification(playerid, "Está propiedad ya está vendida.");
	if(PROPERTY_INFO[index][property_LEVEL] > ACCOUNT_INFO[playerid][ac_LEVEL]) return SendFormatNotification(playerid, "Necesitas ser como mínimo nivel %d para comprar esta propiedad.", PROPERTY_INFO[index][property_LEVEL]);
	if(PROPERTY_INFO[index][property_VIP_LEVEL] > ACCOUNT_INFO[playerid][ac_VIP]) return SendFormatNotification(playerid, "Necesitas membresía VIP (%d) para comprar esta propiedad.", PROPERTY_INFO[index][property_VIP_LEVEL]);
	if(PROPERTY_INFO[index][property_EXTRA] > PI[playerid][ac_COINS]) return SendFormatNotification(playerid, "Necesitas %d "SERVER_COIN" para comprar esta propiedad.", PROPERTY_INFO[index][property_EXTRA]);
	if(PI[playerid][ac_BANK_MONEY] >= PROPERTY_INFO[index][property_PRICE])
	{
		PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX] = index;
		ShowDialog(playerid, DIALOG_CONFIRM_BUY_PROPERTY);
	}
	else SendFormatNotification(playerid, "No tienes fondos suficientes en tu cuenta, te faltan ~r~%s dólares ~w~para poder comprar esta propiedad.", number_format_thousand(PROPERTY_INFO[index][property_PRICE] - PI[playerid][ac_BANK_MONEY]));
	return 1;
}

CMD:a(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] < CMD_HELPER_MODERATOR) return -1; //hacemos chequeo aqui para hacer que este comando no necesite duty
	if(isnull(params)) return SendNotification(playerid, "~r~Modo de uso:~w~ /a <texto>");

  	new string[145];
    format(string, sizeof(string), "Admin Chat: {90D496}%s (%s): {"#SILVER_COLOR"}%s", ACCOUNT_INFO[playerid][ac_NAME], ADMIN_LEVELS[ ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] ], params);
	SendChatMessageToAdmins(0x90C3D4FF, string);
	return 1;
}

CMD:banco(playerid, params[])
{
	if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] == -1) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] == INTERIOR_NO_INFO) return SendNotification(playerid, "No estás en el lugar adecuado.");

	
	new index = GetBankLocalIndexByIntType(ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE]);
	if(index == -1) return SendNotification(playerid, "No estás en el lugar adecuado.");

	if(!IsPlayerInRangeOfPoint(playerid, 1.0, Bank_Interior_Positions[index][bank_X], Bank_Interior_Positions[index][bank_Y], Bank_Interior_Positions[index][bank_Z])) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	if(PI[playerid][ac_BANK_ACCOUNT] == 0) // no tiene cuenta bancaria
	{
		ShowDialog(playerid, DIALOG_CREATE_BANK_ACCOUNT);
		return 1;
	}
	ShowDialog(playerid, DIALOG_BANK);
	return 1;
}

CMD:cajero(playerid, params[])
{
	if(PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID] == 0) return SendNotification(playerid, "No estás en el lugar adecuado.");

	new info[3];
	new Float:pos[3];
	Streamer_GetArrayData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
	if(info[0] != PICKUP_TYPE_ATM) return SendNotification(playerid, "No estás en el lugar adecuado.");

	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_X, pos[0]);
	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Y, pos[1]);
	Streamer_GetFloatData(STREAMER_TYPE_PICKUP, PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID], E_STREAMER_Z, pos[2]);

	if(!IsPlayerInRangeOfPoint(playerid, 1.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "No estás en el lugar adecuado.");

	if(PI[playerid][ac_BANK_ACCOUNT] == 0)
	{
		SendNotification(playerid, "No tienes cuenta bancaria, puedes crearla en cualquier Banco.");
		return 1;
	}
	
	PLAYER_TEMP[playerid][pt_PLAYER_IN_ATM] = true;
	ShowDialog(playerid, DIALOG_BANK);
	return 1;
}

CMD:alimentos(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ShowDialog(playerid, DIALOG_PLAYER_POCKET);
	return 1;
}

#define TIME_BETWEEN_GIVE_CASH	30 // segundos
CMD:dar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(ACCOUNT_INFO[playerid][ac_LEVEL] < 2) return SendNotification(playerid, "Debes ser al menos nivel 2 para usar este comando.");
	
	new option[24], to_playerid, extra;
	if(sscanf(params, "s[24]ud", option, to_playerid, extra)) return SendNotification(playerid, "Error en los parámetros, utilice {"#SILVER_COLOR"}/man dar.");
	
	if(!IsPlayerConnected(to_playerid)) return SendNotification(playerid, "El jugador no está conectado.");
	new Float:pos[3]; GetPlayerPos(to_playerid, pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, NEARS_PLAYERS_DISTANCE, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");
	if(PLAYER_TEMP[to_playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No puedes darle nada a este jugador ahora.");
	
	switch(YHash(option, false))
	{
		case _I<dinero>:
		{
			if(gettime() < PLAYER_TEMP[playerid][pt_LAST_GIVE_MONEY_TIME] + TIME_BETWEEN_GIVE_CASH)
			{
				new time = TIME_BETWEEN_GIVE_CASH - (gettime() - PLAYER_TEMP[playerid][pt_LAST_GIVE_MONEY_TIME]);
				SendFormatNotification(playerid, "Debes de esperar %s minutos para volver a dar dinero.", TimeConvert(time));
				return 1;
			}
			
			if(extra < 0 || extra > PI[playerid][ac_CASH]) return SendNotification(playerid, "Cantidad de dinero incorrecta.");
			if(extra > 50000) return SendNotification(playerid, "Para dar tanto dinero tienes que hacerlo a través de transferencias bancarias.");
			
			if(GivePlayerCash(playerid, -extra, true, true) && GivePlayerCash(to_playerid, extra, true, false)) {
				SendFormatNotification(playerid, "Le has dado ~g~%s dólares~w~ a %s.", number_format_thousand(extra), PLAYER_TEMP[to_playerid][pt_RP_NAME]);
				SendFormatNotification(to_playerid, "%s te ha dado ~g~%s dólares~w~.", PLAYER_TEMP[playerid][pt_RP_NAME], number_format_thousand(extra));
				
				new action[64];
				format(action, sizeof action, "le da dinero a %s.", PLAYER_TEMP[to_playerid][pt_RP_NAME]);
				Auto_SendPlayerAction(playerid, action);
				PLAYER_TEMP[playerid][pt_LAST_GIVE_MONEY_TIME] = gettime();
			}
		}
		case _I<alimento>:
		{
			if(extra < 1 || extra > MAX_PLAYER_POCKET_OBJECTS) return SendNotification(playerid, "~r~Modo de uso: ~w~/dar alimento [ID o Nombre] [Slot /alimentos]");
		
			extra --;
			
			if(!PLAYER_POCKET[playerid][extra][player_pocket_VALID]) return SendNotification(playerid, "No tienes nada en ese slot.");
			
			new to_player_slot = GetEmptyPlayerPocketSlot(to_playerid);
			if(to_player_slot == -1)
			{
				PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
				SendNotification(playerid, "El jugador no tiene más espacio entre sus alimentos.");
				return 1;
			}
			
			new action[64];
			format(action, sizeof action, "le da su %s a %s.", PLAYER_POCKET[playerid][extra][player_pocket_object_NAME], PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			Auto_SendPlayerAction(playerid, action);
			
			TransferPlayerPocketObject(playerid, extra, to_playerid, to_player_slot);
		}
		case _I<medicamentos>:
		{
			if(extra <= 0 || extra > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(extra > PI[playerid][ac_MEDICINE]) return SendNotification(playerid, "No tienes esa cantidad.");
			
			PI[playerid][ac_MEDICINE] -= extra;
			PI[to_playerid][ac_MEDICINE] += extra;
			
			SendFormatNotification(playerid, "Le has dado ~g~%d gramos~w~ de medicamentos a %s.", extra, PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			SendFormatNotification(to_playerid, "%s te ha dado ~g~%d gramos~w~ de medicamentos.", PLAYER_TEMP[playerid][pt_RP_NAME], extra);
	
			new action[64];
			format(action, sizeof action, "le da algo a %s.", PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			Auto_SendPlayerAction(playerid, action);
		}
		case _I<marihuana>:
		{
			if(extra <= 0 || extra > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(extra > PI[playerid][ac_CANNABIS]) return SendNotification(playerid, "No tienes esa cantidad.");
			
			PI[playerid][ac_CANNABIS] -= extra;
			PI[to_playerid][ac_CANNABIS] += extra;
			
			SendFormatNotification(playerid, "Le has dado %dg de marihuana a %s.", extra, PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			SendFormatNotification(to_playerid, "%s te ha dado %dg de marihuana.", PLAYER_TEMP[playerid][pt_RP_NAME], extra);
			
			new action[64];
			format(action, sizeof action, "le da algo a %s.", PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			Auto_SendPlayerAction(playerid, action);
		}
		case _I<crack>:
		{
			if(extra <= 0 || extra > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(extra > PI[playerid][ac_CRACK]) return SendNotification(playerid, "No tienes esa cantidad.");
			
			PI[playerid][ac_CRACK] -= extra;
			PI[to_playerid][ac_CRACK] += extra;
			
			SendFormatNotification(playerid, "Le has dado ~g~%d gramos~w~ de crack a %s.", extra, PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			SendFormatNotification(to_playerid, "%s te ha dado ~g~%d gramos~w~ de crack.", PLAYER_TEMP[playerid][pt_RP_NAME], extra);
			
			new action[64];
			format(action, sizeof action, "le da algo a %s.", PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			Auto_SendPlayerAction(playerid, action);
		}
		case _I<arma>:
		{
			if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "Los policías no pueden dar armas.");
			if(extra < 0 || extra >= sizeof PLAYER_WEAPONS[]) return SendNotification(playerid, "~r~Modo de uso: ~w~/dar arma [ID o Nombre] [Slot /armas]");
		
			if(!PLAYER_WEAPONS[playerid][extra][player_weapon_VALID]) return SendNotification(playerid, "No tienes nigún arma en ese slot.");
			
			if(PI[to_playerid][ac_LEVEL] < 2) return SendNotification(playerid, "La otra persona tiene que ser al menos nivel 2.");
			if(PLAYER_WEAPONS[to_playerid][extra][player_weapon_VALID])
			{
				PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
				SendNotification(playerid, "El jugador ya tiene un arma en ese slot.");
				return 1;
			}
			
			SendFormatNotification(playerid, "~w~Le has dado tu ~g~'%s'~w~ a %s.", WEAPON_INFO[ PLAYER_WEAPONS[playerid][extra][player_weapon_ID] ][weapon_info_NAME], PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			SendFormatNotification(to_playerid, "%s te ha dado su ~g~'%s'~w~.", PLAYER_TEMP[playerid][pt_RP_NAME], WEAPON_INFO[ PLAYER_WEAPONS[playerid][extra][player_weapon_ID] ][weapon_info_NAME]);
			
			new action[64];
			format(action, sizeof action, "le da un arma a %s.", PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			Auto_SendPlayerAction(playerid, action);
			
			TransferPlayerWeapon(playerid, extra, to_playerid);
		}
		case _I<kit>:
		{
			if(extra <= 0 || extra > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(extra > PI[playerid][ac_MECHANIC_KITS]) return SendNotification(playerid, "No tienes esa cantidad.");
			
			PI[playerid][ac_MECHANIC_KITS] -= extra;
			PI[to_playerid][ac_MECHANIC_KITS] += extra;
			
			SendFormatNotification(playerid, "Le has dado %d kits de reparación a %s.", extra, PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			SendFormatNotification(to_playerid, "%s te ha dado %d kits de reparación.", PLAYER_TEMP[playerid][pt_RP_NAME], extra);
			
			new action[64];
			format(action, sizeof action, "le da algo a %s.", PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			Auto_SendPlayerAction(playerid, action);
		}
		case _I<botiquin>:
		{
			if(extra <= 0 || extra > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(extra > PI[playerid][ac_MEDICAL_KITS]) return SendNotification(playerid, "No tienes esa cantidad.");
			
			PI[playerid][ac_MEDICAL_KITS] -= extra;
			PI[to_playerid][ac_MEDICAL_KITS] += extra;
			
			SendFormatNotification(playerid, "Le has dado %d botiquines a %s.", extra, PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			SendFormatNotification(to_playerid, "%s te ha dado %d botiquines.", PLAYER_TEMP[playerid][pt_RP_NAME], extra);
			
			new action[64];
			format(action, sizeof action, "le da algo a %s.", PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			Auto_SendPlayerAction(playerid, action);
		}
		default: SendNotification(playerid, "Error en los parámetros, utilice ~r~/man dar~w~.");
	}
	return 1;
}

CMD:vender(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(ACCOUNT_INFO[playerid][ac_LEVEL] < 2) return SendNotification(playerid, "Debes ser al menos nivel 2 para usar este comando.");
	
	new option[24], to_playerid, extra, price;
	if(sscanf(params, "s[24]udd", option, to_playerid, extra, price)) return SendNotification(playerid, "Error en los parámetros, utilice ~r~/man vender~w~.");
	
	if(price <= 0 || price > 10000000) return SendNotification(playerid, "El precio no es válido.");
	if(!IsPlayerConnected(to_playerid)) return SendNotification(playerid, "El jugador no está conectado.");
	new Float:pos[3]; GetPlayerPos(to_playerid, pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, NEARS_PLAYERS_DISTANCE, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");
	if(PLAYER_TEMP[to_playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No puedes venderle nada a este jugador ahora.");
	
	if(price > PI[to_playerid][ac_CASH])
	{
		SendNotification(playerid, "Esta persona no tiene el dinero que pides.");
		return 1;
	}
	
	PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_PID] = playerid;
	PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_AID] = ACCOUNT_INFO[playerid][ac_ID];
	PLAYER_TEMP[to_playerid][pt_TRICK_PRICE] = price;
	PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] = extra;
	PLAYER_TEMP[to_playerid][pt_TRICK_TIME] = gettime();

	switch(YHash(option, false))
	{
		case _I<alimento>:
		{
			if(PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] < 1 || PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] > MAX_PLAYER_POCKET_OBJECTS) return SendNotification(playerid, "~r~Modo de uso: ~w~/vender alimento [ID o Nombre] [Slot /alimentos] [precio]");
			PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] --;
			
			if(!PLAYER_POCKET[playerid][ PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] ][player_pocket_VALID]) return SendNotification(playerid, "No tienes nada en ese slot.");
			
			SendFormatNotification(playerid, "Le has ofrecido una venta a ~g~%s~w~, espera a ver si la acepta.", PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			ShowDialog(to_playerid, DIALOG_TRICKS_FOOD);
		}
		case _I<medicamentos>:
		{
			if(PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] <= 0 || PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] > PI[playerid][ac_MEDICINE]) return SendNotification(playerid, "No tienes esa cantidad.");
			
			SendFormatNotification(playerid, "Le has ofrecido una venta a ~g~%s~w~, espera a ver si la acepta.", PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			ShowDialog(to_playerid, DIALOG_TRICKS_MEDICINE);
		}
		case _I<marihuana>:
		{
			if(PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] <= 0 || PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] > PI[playerid][ac_CANNABIS]) return SendNotification(playerid, "No tienes esa cantidad.");
			
			SendFormatNotification(playerid, "Le has ofrecido una venta a ~g~%s~w~, espera a ver si la acepta.", PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			ShowDialog(to_playerid, DIALOG_TRICKS_CANNABIS);
		}
		case _I<crack>:
		{
			if(PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] <= 0 || PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] > PI[playerid][ac_CRACK]) return SendNotification(playerid, "No tienes esa cantidad.");
			
			SendFormatNotification(playerid, "Le has ofrecido una venta a ~g~%s~w~, espera a ver si la acepta.", PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			ShowDialog(to_playerid, DIALOG_TRICKS_CRACK);
		}
		case _I<arma>:
		{
			if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "Los policías no pueden vender armas.");
			if(PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] < 0 || PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] >= sizeof PLAYER_WEAPONS[]) return SendNotification(playerid, "Error en slot de /armas.");
		
			if(!PLAYER_WEAPONS[playerid][ PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] ][player_weapon_VALID]) return SendNotification(playerid, "No tienes nigún arma en ese slot.");
			
			if(PI[to_playerid][ac_LEVEL] < 2) return SendNotification(playerid, "La otra persona tiene que ser al menos nivel 2.");
			
			SendFormatNotification(playerid, "Le has ofrecido una venta a ~g~%s~w~, espera a ver si la acepta.", PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			ShowDialog(to_playerid, DIALOG_TRICKS_WEAPON);
		}
		case _I<coins>:
		{
			if(PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] <= 0 || PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_EXTRA] > PI[playerid][ac_COINS]) return SendNotification(playerid, "No tienes esa cantidad de "SERVER_COIN".");
			
			SendFormatNotification(playerid, "Le has ofrecido una venta a ~g~%s~w~, espera a ver si la acepta.", PLAYER_TEMP[to_playerid][pt_RP_NAME]);
			ShowDialog(to_playerid, DIALOG_TRICKS_SU);
		}
		default: SendNotification(playerid, "Error en los parámetros, utilice ~r~/man vender~w~.");
	}
	return 1;
}

CMD:consumir(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	new option[24], slot;
	if(!sscanf(params, "s[24]d", option, slot))	
	{
		if(!strcmp(option, "alimento", true))
		{
			if(slot < 1 || slot > MAX_PLAYER_POCKET_OBJECTS) return SendFormatNotification(playerid, "~r~Modo de uso: ~w~/consumir alimento [SLOT 1-%d]", MAX_PLAYER_POCKET_OBJECTS);
		
			slot --;
			
			if(!PLAYER_POCKET[playerid][slot][player_pocket_VALID]) return SendNotification(playerid, "No tienes nada en ese slot.");
			if(PI[playerid][ac_HUNGRY] >= 99.0 && PI[playerid][ac_THIRST] >= 99.0) return SendNotification(playerid, "¿Es que quieres reventar?");

			Add_Hungry_Thirst(playerid, PLAYER_POCKET[playerid][slot][player_pocket_object_HUNGRY], PLAYER_POCKET[playerid][slot][player_pocket_object_THIRST]);
			GivePlayerDrunkLevel(playerid, PLAYER_POCKET[playerid][slot][player_pocket_object_DRUNK]);
			
			new action[64];
			format(action, sizeof action, "consume %s.", PLAYER_POCKET[playerid][slot][player_pocket_object_NAME]);
			Auto_SendPlayerAction(playerid, action);
			
			DeletePlayerPocketObject(playerid, slot);

			ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 0, 0, 0, 0, 0, 0);
			ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 4.1, false, true, true, false, 1000);
		}
		else SendNotification(playerid, "~r~Modo de uso: ~w~/consumir [medicamento - marihuana - crack - alimento]");
	}
	else if(!sscanf(params, "s[24]", option))
	{
		switch(YHash(option, false))
		{
			case _I<medicamento>:
			{
				if(PLAYER_TEMP[playerid][pt_COOLDOWN_MEDICINE] > gettime()) return SendFormatNotification(playerid, "Debes esperar %d segundos para volver a poder consumir medicamentos", PLAYER_TEMP[playerid][pt_COOLDOWN_MEDICINE] - gettime());

				if(PI[playerid][ac_MEDICINE] <= 0) return SendNotification(playerid, "No tienes medicamentos.");
				
				PLAYER_TEMP[playerid][pt_COOLDOWN_MEDICINE] = gettime() + 30;
				PI[playerid][ac_MEDICINE] --;
				GivePlayerHealthEx(playerid, 25.0);
				Auto_SendPlayerAction(playerid, "se toma un medicamento.");
			}
			case _I<marihuana>:
			{
				if(PLAYER_TEMP[playerid][pt_COOLDOWN_WEED] > gettime()) return SendFormatNotification(playerid, "Debes esperar %d segundos para volver a poder consumir marihuana", PLAYER_TEMP[playerid][pt_COOLDOWN_WEED] - gettime());

				if(PI[playerid][ac_CANNABIS] <= 0) return SendNotification(playerid, "No tienes marihuana.");

				PLAYER_TEMP[playerid][pt_COOLDOWN_WEED] = gettime() + 30;
				
				PI[playerid][ac_CANNABIS] --;
				GivePlayerHealthEx(playerid, 30.0);
				Auto_SendPlayerAction(playerid, "consume marihuana.");
				GivePlayerDrunkLevel(playerid, 1000);
			}
			case _I<crack>:
			{
				if(PLAYER_TEMP[playerid][pt_COOLDOWN_CRACK] > gettime()) return SendFormatNotification(playerid, "Debes esperar %d segundos para volver a poder consumir crack", PLAYER_TEMP[playerid][pt_COOLDOWN_CRACK] - gettime());

				if(PI[playerid][ac_CRACK] <= 0) return SendNotification(playerid, "No tienes crack.");

				PLAYER_TEMP[playerid][pt_COOLDOWN_CRACK] = gettime() + 30;
				PI[playerid][ac_CRACK] --;
				GivePlayerArmourEx(playerid, 20.0);
				Auto_SendPlayerAction(playerid, "consume crack.");
				GivePlayerDrunkLevel(playerid, 2000);
			}
			case _I<alimento>: SendFormatNotification(playerid, "~r~Modo de uso: ~w~/consumir alimento [SLOT 1-%d]", MAX_PLAYER_POCKET_OBJECTS);
			
			default: SendNotification(playerid, "~r~Modo de uso: ~w~/consumir [medicamento - marihuana - crack - alimento]");
		}
	}
	else SendNotification(playerid, "~r~Modo de uso: ~w~/consumir [medicamento - marihuana - crack - alimento]");
	return 1;
}

CMD:man(playerid, params[])
{
	switch(YHash(params, false))
	{
		case _I<dar>:
		{
			ShowPlayerDialog
			(
				playerid,
				DIALOG_INFO,
				DIALOG_STYLE_MSGBOX,
				
					"Manual",
						"\
							Comando: dar\n\
							Sintaxis: dar <opcion> <playerid o nombre> <extra>\n\
							\n\
							Este comando sirve para dar algo a otro jugador cercano.\n\
							Para comprobar que tiene el objeto y la cantidad utilice /inv.\n\
							\n\
							Esta es la lista de opciones:\n\
							    - arma <playerid o nombre> <slot de /armas>\n\
							    - dinero <playerid o nombre> <cantidad>\n\
							    - alimento <playerid o nombre> <slot de /alimentos>\n\
							    - medicamentos <playerid o nombre> <cantidad>\n\
							    - marihuana <playerid o nombre> <cantidad>\n\
							    - crack <playerid o nombre> <cantidad>\n\
							    - kit <playerid o nombre> <cantidad>\n\
							\n\
							Para obtener ayuda diferente use /ayuda.\
						",
					"Cerrar", ""
					
			);
			return 1;
		}
		case _I<tirar>:
		{
			ShowPlayerDialog
			(
				playerid,
				DIALOG_INFO,
				DIALOG_STYLE_MSGBOX,
				
					"Manual",
						"\
							Comando: tirar\n\
							Sintaxis: tirar <opcion> <extra>\n\
							\n\
							Este comando sirve para deshacerte de algún objeto.\n\
							\n\
							Esta es la lista de opciones:\n\
							    - arma <slot de /armas>\n\
							    - dinero <cantidad>\n\
							    - alimento <slot de /alimentos>\n\
							    - medicamentos <cantidad>\n\
							    - marihuana <cantidad>\n\
							    - crack <cantidad>\n\
							    - kit <cantidad>\n\
							\n\
							Para obtener ayuda diferente use /ayuda.\
						",
					"Cerrar", ""
					
			);
			return 1;
		}
		case _I<vender>:
		{
			ShowPlayerDialog
			(
				playerid,
				DIALOG_INFO,
				DIALOG_STYLE_MSGBOX,
				
					"Manual",
						"\
							Comando: vender\n\
							Sintaxis: vender <opcion> <playerid o nombre> <extra> <precio>\n\
							\n\
							Este comando sirve para vender algo a otro jugador cercano.\n\
							Para comprobar que tiene el objeto y la cantidad utilice /inv.\n\
							\n\
							Esta es la lista de opciones:\n\
							    - arma <playerid o nombre> <slot de /armas> <precio>\n\
							    - alimento <playerid o nombre> <slot de /alimentos> <precio>\n\
							    - medicamentos <playerid o nombre> <cantidad> <precio>\n\
							    - marihuana <playerid o nombre> <cantidad> <precio>\n\
							    - crack <playerid o nombre> <cantidad> <precio>\n\
							    - coins <playerid o nombre> <cantidad> <precio>\n\
							\n\
							Para obtener ayuda diferente use /ayuda.\
						",
					"Cerrar", ""
					
			);
			return 1;
		}
		case _I<guardar>:
		{
			ShowPlayerDialog
			(
				playerid,
				DIALOG_INFO,
				DIALOG_STYLE_MSGBOX,
				
					"Manual",
						"\
							Comando: guardar\n\
							Sintaxis: dar <opcion> <extra>\n\
							\n\
							Este comando sirve para dar algo a otro jugador cercano.\n\
							Para comprobar que tiene el objeto y la cantidad utilice /inv.\n\
							\n\
							Esta es la lista de opciones:\n\
							    - arma <slot de /armas>\n\
							    - medicamentos <cantidad>\n\
							    - marihuana <cantidad>\n\
							    - crack <cantidad>\n\
							\n\
							Para obtener ayuda diferente use /ayuda.\
						",
					"Cerrar", ""
					
			);
			return 1;
		}
		default: SendNotification(playerid, "~r~Modo de uso: ~w~/man [dar-tirar-vender-guardar]");
	}
	return 1;
}

CMD:tirar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	new option[24], extra;
	if(sscanf(params, "s[24]d", option, extra)) return SendNotification(playerid, "Error en los parámetros, utilice ~r~/man tirar~w~.");

	switch(YHash(option, false))
	{
		case _I<dinero>:
		{
			if(extra < 0 || extra > PI[playerid][ac_CASH]) return SendNotification(playerid, "Cantidad de dinero incorrecta.");
			
			if(GivePlayerCash(playerid, -extra, true, true)) {
				SendFormatNotification(playerid, "Has tirado al suelo ~g~%s dólares~w~.", number_format_thousand(extra));
				Auto_SendPlayerAction(playerid, "tira dinero.");
			}
			return 1;
		}
		case _I<alimento>:
		{
			if(extra < 1 || extra > MAX_PLAYER_POCKET_OBJECTS) return SendFormatNotification(playerid, "~r~Modo de uso: ~w~/tirar alimento {FFFFFF}[SLOT 1-%d]", MAX_PLAYER_POCKET_OBJECTS);
		
			extra --;
			
			if(!PLAYER_POCKET[playerid][extra][player_pocket_VALID]) return SendNotification(playerid, "No tienes nada en ese slot.");

			SendFormatNotification(playerid, "Has eliminado ~g~\"%s\"~w~ de tus alimentos.", PLAYER_POCKET[playerid][extra][player_pocket_object_NAME]);
			DeletePlayerPocketObject(playerid, extra);
			return 1;
		}
		case _I<medicamentos>:
		{
			if(extra <= 0 || extra > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(extra > PI[playerid][ac_MEDICINE]) return SendNotification(playerid, "No tienes esa cantidad.");
			
			PI[playerid][ac_MEDICINE] -= extra;
			
			SendFormatNotification(playerid, "Has tirado ~g~%d gramos~w~ de medicamentos.", extra);
			return 1;
		}
		case _I<marihuana>:
		{
			if(extra <= 0 || extra > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(extra > PI[playerid][ac_CANNABIS]) return SendNotification(playerid, "No tienes esa cantidad.");
			
			PI[playerid][ac_CANNABIS] -= extra;
			
			SendFormatNotification(playerid, "Has tirado ~g~%d gramos~w~ de marihuana.", extra);
			return 1;
		}
		case _I<crack>:
		{
			if(extra <= 0 || extra > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(extra > PI[playerid][ac_CRACK]) return SendNotification(playerid, "No tienes esa cantidad.");
			
			PI[playerid][ac_CRACK] -= extra;
			
			SendFormatNotification(playerid, "Has tirado ~g~%d gramos~w~ de crack.", extra);
			return 1;
		}
		case _I<arma>:
		{
			if(extra < 0 || extra >= sizeof PLAYER_WEAPONS[]) return SendNotification(playerid, "~r~Modo de uso: ~w~/tirar arma [slot /armas]");
		
			if(!PLAYER_WEAPONS[playerid][extra][player_weapon_VALID]) return SendNotification(playerid, "No tienes nigún arma en ese slot.");
			
			SendFormatNotification(playerid, "Has eliminado tu ~g~'%s'~w~ de tus armas.", WEAPON_INFO[ PLAYER_WEAPONS[playerid][extra][player_weapon_ID] ][weapon_info_NAME]);
			
			new string[64];
			format(string, sizeof string, "tira su %s al suelo", WEAPON_INFO[ PLAYER_WEAPONS[playerid][extra][player_weapon_ID] ][weapon_info_NAME]);
			Auto_SendPlayerAction(playerid, string);

			RemovePlayerSlotWeapon(playerid, extra, true);
			return 1;
		}
		case _I<kit>:
		{
			if(extra <= 0 || extra > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(extra > PI[playerid][ac_MECHANIC_KITS]) return SendNotification(playerid, "No tienes esa cantidad.");
			
			PI[playerid][ac_MECHANIC_KITS] -= extra;
			
			SendFormatNotification(playerid, "Has tirado ~g~%d ~w~kits de reparación.", extra);
			return 1;
		}
		case _I<botiquin>:
		{
			if(extra <= 0 || extra > 10000000) return SendNotification(playerid, "Cantidad incorrecta.");
			if(extra > PI[playerid][ac_MEDICAL_KITS]) return SendNotification(playerid, "No tienes esa cantidad.");
			
			PI[playerid][ac_MEDICAL_KITS] -= extra;
			
			SendFormatNotification(playerid, "Has tirado ~g~%d ~w~botiquines.", extra);
			return 1;
		}
		default: SendNotification(playerid, "Error en los parámetros, utilice ~r~/man tirar~w~.");
	}
	return 1;
}




CMD:pvender(playerid, params[])
{
	if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] == -1 || ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] != INTERIOR_CITY_HALL_LS) return 1;
	if(!IsPlayerInRangeOfPoint(playerid, 1.0, -474.596282, 289.679107, 2004.584960)) return 1;
	if(ACCOUNT_INFO[playerid][ac_LEVEL] < 2) return SendNotification(playerid, "Debes ser al menos nivel 2 para usar este comando.");
	if(PI[playerid][ac_BANK_ACCOUNT] == 0) return SendNotification(playerid, "Necesitas tener una cuenta bancaria para poder realizar estas operaciones.");
	
	ShowDialog(playerid, DIALOG_NOTARY);
	return 1;
}

CMD:grua(playerid, params[])
{
	if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] == -1 || ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] != INTERIOR_CITY_HALL_LS) return 1;
	if(!IsPlayerInRangeOfPoint(playerid, 1.0, -508.645385, 322.147644, 2004.585937)) return 1;

	ShowDialog(playerid, DIALOG_CRANE_SELECT_VEHICLE);
	return 1;
}

CMD:casa(playerid, params[])
{
	if(PLAYER_TEMP[playerid][pt_LAST_PICKUP_ID] == 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_OWN_PROPERTY || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_GUEST_PROPERTY) {
		new index = GetPropertyIndexByID(PI[playerid][ac_LOCAL_INTERIOR]);
		if(index == -1) return SendNotification(playerid, "No estás en el lugar adecuado.");

		if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_OWN_PROPERTY)
		{	
			if(PROPERTY_INFO[index][property_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
			{
				PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] = index;
				ShowDialog(playerid, DIALOG_PROPERTY_OPTIONS);
			}
			else SendNotification(playerid, "No estás en el lugar adecuado.");
		}
		else if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_GUEST_PROPERTY)
		{
			if(PROPERTY_INFO[index][property_CREW])
			{
				if(PROPERTY_INFO[index][property_CREW_ID] != PI[playerid][ac_CREW]) return SendNotification(playerid, "Esta no es una propiedad de tu banda.");
				if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_DELETE_PROPERTIES]) return SendNotification(playerid, "No tienes permiso.");
				if(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_FIGHTING]) return SendNotification(playerid, "No puedes liberar una propiedad cuando tu banda está en combate."); 
				
				PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] = index;
				ShowDialog(playerid, DIALOG_CREW_LEAVE_PROPERTY);
			}
			else SendNotification(playerid, "No estás en el lugar adecuado.");
		}
	}
	else SendNotification(playerid, "No estás en el lugar adecuado.");
	return 1;
}

CMD:armario(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_OWN_PROPERTY)
	{
		new index = GetPropertyIndexByID(PI[playerid][ac_LOCAL_INTERIOR]);
		if(index == -1) return SendNotification(playerid, "BUG: CMD /armario, Tome captura y contacte con administrador.");
		if(PROPERTY_INFO[index][property_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return SendNotification(playerid, "Esta no es tu casa");

		new Float:z_pos = PROPERTY_CLOSET_POS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_closet_Z];
		if(PROPERTY_INFO[index][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
		if(IsPlayerInRangeOfPoint(playerid, 1.0, PROPERTY_CLOSET_POS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_closet_X], PROPERTY_CLOSET_POS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_closet_Y], z_pos))
		{
			PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] = index;
			ShowDialog(playerid, DIALOG_PROPERTY_CLOSET);
		}
		else SendNotification(playerid, "No estás cerca del armario.");
	}
	else SendNotification(playerid, "No estás en tu casa.");
	return 1;
}

CMD:tarifa(playerid, params[])
{
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendNotification(playerid, "No estás conduciendo.");
	if(!PLAYER_WORKS[playerid][WORK_TAXI][pwork_SET]) return SendNotification(playerid, "No eres taxista.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_TAXI) return SendNotification(playerid, "No estás de servicio como taxista.");
	
	new vehicleid = GetPlayerVehicleID(playerid);
	if(TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_ENABLED]) return SendNotification(playerid, "Solo puedes cambiar la tarifa cuando no haya ningún pasajero.");
	
	if(sscanf(params, "d", params[0])) return SendNotification(playerid, "~r~Modo de uso: ~w~/tarifa [0-14]");
	if(params[0] < 0 || params[0] > 14) return SendNotification(playerid, "~r~Modo de uso: ~w~/tarifa [0-14]");
	
	TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_PRICE] = params[0];
	Auto_SendPlayerAction(playerid, "ajusta el taxímetro.");
	UpdatePlayerTaxiMeterTextdraws(playerid);
	return 1;
}

CMD:cargar(playerid, params[])
{
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendNotification(playerid, "No estás conduciendo.");
	
	new vehicleid = GetPlayerVehicleID(playerid);
	if(!WORK_VEHICLES[vehicleid][work_vehicle_VALID]) return SendNotification(playerid, "No estás en un vehículo de carga.");
	if(WORK_VEHICLES[vehicleid][work_vehicle_WORK] != WORK_TRUCK && WORK_VEHICLES[vehicleid][work_vehicle_WORK] != WORK_DELIVERYMAN) return SendNotification(playerid, "No estás en un vehículo de carga.");

	if(PLAYER_WORKS[playerid][WORK_TRUCK][pwork_SET] && PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_TRUCK) {
		for(new i = 0; i != sizeof LoadTrucksPoints; i ++)
		{
			if(IsPlayerInRangeOfPoint(playerid, 3.0, LoadTrucksPoints[i][0], LoadTrucksPoints[i][1], LoadTrucksPoints[i][2]))
			{
				new Float:angle;
				GetVehicleZAngle(vehicleid, angle);
				if(angle > 180.0) angle -= 360.0;

				if(angle > (LoadTrucksPoints[i][3] - 15.0) && angle < (LoadTrucksPoints[i][3] + 15.0))
				{	
					if(TRUCK_VEHICLE[vehicleid][truck_vehicle_LOADED]) return SendNotification(playerid, "El camión ya está cargado.");
					if(TRUCK_VEHICLE[vehicleid][truck_vehicle_LOADING]) return SendNotification(playerid, "El camión ya se está cargando.");
		
					TRUCK_VEHICLE[vehicleid][truck_vehicle_LOADING] = true;
					TRUCK_VEHICLE[vehicleid][truck_vehicle_DRIVER_USER_ID] = ACCOUNT_INFO[playerid][ac_ID];
					
					SetVehicleVelocity(vehicleid, 0.0, 0.0, 0.0);
					
					GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
					UpdateVehicleParams(vehicleid);
					
					ShowDialog(playerid, DIALOG_SELECT_TRUCK_TYPE);
				}
				else SendNotification(playerid, "El camión no está correctamente colocado para cargarlo.");
				return 1;
			}
		}
		SendNotification(playerid, "Para cargar el camión colocate en cualquier punto de carga.");
	}
	if(PLAYER_WORKS[playerid][WORK_DELIVERYMAN][pwork_SET] && PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_DELIVERYMAN) {
		CallLocalFunction("OnDeliveryManRequestLoad", "ii", playerid, vehicleid);
	}
	else SendNotification(playerid, "No estás trabajando en camionero o repartidor.");
	return 1;
}



CMD:gasolina(playerid, params[])
{
	if(GetPlayerInterior(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendNotification(playerid, "No estás en nigún vehículo.");
	
	new vehicleid = GetPlayerVehicleID(playerid), modelid = GetVehicleModel(vehicleid);
	if(!VEHICLE_INFO[modelid - 400][vehicle_info_NORMAL_SPEEDO]) return SendNotification(playerid, "No puedes repostar este vehículo.");
	
	new fuel_station = -1;
	for(new i = 0; i < sizeof Fuel_Stations; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 5.0, Fuel_Stations[i][fs_X], Fuel_Stations[i][fs_Y], Fuel_Stations[i][fs_Z]))
		{
			fuel_station = i;
			break;
		}
	}
	if(fuel_station == -1) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] <= 0.0) return SendNotification(playerid, "Este vehículo no tiene depósito de gasolina.");
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE])
	{
		SendNotification(playerid, "Por favor, para primero el motor del vehículo.");
		return 1;
	}
	
	if(!sscanf(params, "d", params[0]))
	{
		new Float:amount = float(params[0]);
		if(amount < 0.0) return SendNotification(playerid, "~r~Modo de uso: ~w~/gasolina [litros / lleno]");
		if(amount + GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] > GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS]) amount = GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] - GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS];
		
		new price = floatround( floatmul(amount, 9.0) );
		
		if(PI[playerid][ac_CASH] >= price)
		{
			if(GivePlayerCash(playerid, -price, true, true)) {
				GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] += amount;
				
				PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
				SendFormatNotification(playerid, "Cantidad: ~b~%.1f litros~n~~w~Precio: ~g~%s dólares~w~", amount, number_format_thousand(price));
				Auto_SendPlayerAction(playerid, "ha repostado el vehículo.");
			}
		}
		else
		{
			PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
			SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder repostar {588dc9}%.1f.", number_format_thousand(price - PI[playerid][ac_CASH]), amount);
		}
		return 1;
	}
	
	new text[24];
	if(!sscanf(params, "s[24]", text))
	{
		if(!strcmp(text, "lleno", true))
		{
			new Float:amount = floatsub(GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS], GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS]);

			new price = floatround( floatmul(amount, 9.0) );
			if(PI[playerid][ac_CASH] >= price)
			{
				if(GivePlayerCash(playerid, -price, true, true)) {
					GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] = GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS];
					
					PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
					SendFormatNotification(playerid, "Cantidad: ~b~%.1f litros~n~~w~Precio: ~g~%s dólares~w~.", amount, number_format_thousand(price));
					Auto_SendPlayerAction(playerid, "ha repostado el vehículo.");
				}
			}
			else
			{
				PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
				SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder repostar {588dc9}%.1f.", number_format_thousand(price - PI[playerid][ac_CASH]), amount);
			}
		}
		else SendNotification(playerid, "~r~Modo de uso: ~w~/gasolina [litros / lleno]");
		return 1;
	}
	
	SendNotification(playerid, "~r~Modo de uso: ~w~/gasolina [litros / lleno]");
	return 1;
}

CMD:bidon(playerid, params[])
{
	if(GetPlayerInterior(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	new fuel_station = -1;
	for(new i = 0; i < sizeof Fuel_Stations; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 5.0, Fuel_Stations[i][fs_X], Fuel_Stations[i][fs_Y], Fuel_Stations[i][fs_Z]))
		{
			fuel_station = i;
			break;
		}
	}
	if(fuel_station == -1) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	if(PI[playerid][ac_FUEL_DRUM] <= 0)
	{
		PI[playerid][ac_FUEL_DRUM] = 0;
		if(GivePlayerCash(playerid, -250, true, true)) {
			PI[playerid][ac_FUEL_DRUM] = 20;
			SendNotification(playerid, "Has comprado un bidón de gasolina de 20 litros por 250$, usa ~g~/vertir ~w~para repostar un vehículo.");
		}
		else SendNotification(playerid, "No tienes suficiente dinero para comprar el bidón.");
	}
	else
	{
		if(PI[playerid][ac_FUEL_DRUM] >= 20) return SendNotification(playerid, "Tu bidón de gasolina está lleno.");
		
		new amount = (20 - PI[playerid][ac_FUEL_DRUM]);
		new price = (13 * amount);
		
		if(PI[playerid][ac_CASH] >= price)
		{
			if(GivePlayerCash(playerid, -price, true, true)) {
				PI[playerid][ac_FUEL_DRUM] += amount;
				SendFormatNotification(playerid, "Has llenado tu bidón de gasolina con 20 litros por %d$, usa ~g~/vertir ~w~para repostar un vehículo.", price);
			}
		}
		else SendNotification(playerid, "No tienes suficiente dinero para comprar el bidón.");
	}
	return 1;
}

CMD:vertir(playerid, params[])
{	
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "Tienes que estar fuera del vehículo para vertir el bidón.");
	
	new vehicleid = GetPlayerCameraTargetVehicle(playerid);
	if(vehicleid == INVALID_VEHICLE_ID) return SendNotification(playerid, "No estás cerca de ningún vehículo.");
	
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE])
	{
		SendNotification(playerid, "Por favor, para primero el motor del vehículo.");
		return 1;
	}
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] <= 0.0) return SendNotification(playerid, "Este vehículo no tiene depósito de gasolina.");

	if(sscanf(params, "d", params[0])) return SendNotification(playerid, "~r~Modo de uso: ~w~/vetir [Cantidad de litros]");
	if(params[0] <= 0) return SendNotification(playerid, "Cantidad de litros no válida.");
	if(params[0] > PI[playerid][ac_FUEL_DRUM]) return SendFormatNotification(playerid, "Solo tienes %d.0 litros en el bidón.", PI[playerid][ac_FUEL_DRUM]);
	
	new Float:amount = float(params[0]);
	if(amount + GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] > GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS]) amount = GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] - GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS];
	
	PI[playerid][ac_FUEL_DRUM] -= floatround(amount);
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] += amount;
	SendFormatNotification(playerid, "Has vertido ~b~%.1f litros~w~ del bidón, te quedan ~b~%d.0 litros~w~.", amount, PI[playerid][ac_FUEL_DRUM]);
	Auto_SendPlayerAction(playerid, "ha repostado el vehículo.");
	return 1;
}

CMD:setfdrum(playerid, params[])
{
	new to_player, amount;
	if(sscanf(params, "ud", to_player, amount)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setfdrum <player_id> <valor>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	
	PI[to_player][ac_FUEL_DRUM] = amount;
	
	SendCmdLogToAdmins(playerid, "setfdrum", params);
	return 1;
}

CMD:comprarnivel(playerid, params[])
{
	new neccessary_rep = ACCOUNT_INFO[playerid][ac_LEVEL] * REP_MULTIPLIER;
	if(ACCOUNT_INFO[playerid][ac_REP] < neccessary_rep) return SendNotification(playerid, "Aún no tienes la suficiente reputación para subir de nivel.");
	if(ACCOUNT_INFO[playerid][ac_LEVEL] * 150 > PI[playerid][ac_CASH]) return SendFormatNotification(playerid, "No tienes suficiente dinero para subir de nivel, te faltan ~g~%s dólares~w~ para poder comprarlo.", number_format_thousand((ACCOUNT_INFO[playerid][ac_LEVEL] * 150) - PI[playerid][ac_CASH]));
	
	if(GivePlayerCash(playerid, -(ACCOUNT_INFO[playerid][ac_LEVEL] * 150), true, true)) {
		ACCOUNT_INFO[playerid][ac_REP] = 1;
		ACCOUNT_INFO[playerid][ac_LEVEL] ++;
		UpdateReputationTextDraws(playerid);
		SetPlayerSkillLevels(playerid);
		
		SendFormatNotification(playerid, "~g~¡Felicidades! ~w~Has subido al nivel %d. Pagaste ~g~%d dólares~w~ por él.", ACCOUNT_INFO[playerid][ac_LEVEL], ACCOUNT_INFO[playerid][ac_LEVEL] * 150);
		SetPlayerScore(playerid, ACCOUNT_INFO[playerid][ac_LEVEL]);
		PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
		
		ACCOUNT_INFO[playerid][ac_TIME_FOR_REP] = TIME_FOR_REP;
		PLAYER_TEMP[playerid][pt_TIME_PASSED_LAST_REP] = gettime() * 1000;
		
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET time_playing = %d, level = %d, rep = %d, time_for_rep = %d, payday_rep = %d WHERE id = %d;", ACCOUNT_INFO[playerid][ac_TIME_PLAYING], ACCOUNT_INFO[playerid][ac_LEVEL], ACCOUNT_INFO[playerid][ac_REP], TIME_FOR_REP, ACCOUNT_INFO[playerid][ac_PAYDAY_REP], ACCOUNT_INFO[playerid][ac_ID]);
		mysql_tquery(srp_db, QUERY_BUFFER);
		
		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][2]);
		PLAYER_TEMP[playerid][pt_TIMERS][2] = SetTimerEx("AddPlayerReputation", ACCOUNT_INFO[playerid][ac_TIME_FOR_REP], false, "i", playerid);
	}
	return 1;
}
CMD:trabajar(playerid, params[])
{	
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "Debes estar a pie.");
	if(PI[playerid][ac_WANTED_LEVEL] > 0) return SendNotification(playerid, "No puedes obtener un trabajo cuando estás en búsqueda.");
	
	if(gettime() < PLAYER_TEMP[playerid][pt_LAST_GOT_WORK_TIME] + NECESSARY_TIME_BETWEEN_WORKS)
	{
		new time = NECESSARY_TIME_BETWEEN_WORKS - (gettime() - PLAYER_TEMP[playerid][pt_LAST_GOT_WORK_TIME]);
		SendFormatNotification(playerid, "Debes de esperar %s minutos para conseguir otro trabajo.", TimeConvert(time));
		return 1;
	}

	new player_jobs = CountPlayerJobs(playerid);
	if(ACCOUNT_INFO[playerid][ac_VIP])
	{
		if(player_jobs >= MAX_SU_WORKS)
		{
			SendFormatNotification(playerid, "Actualmente tienes ~r~%d trabajos~w~, usa /renunciar para dejar un trabajo.", player_jobs);
			return 1;
		}
	}
	else
	{
		if(player_jobs >= MAX_NU_WORKS)
		{
			SendFormatNotification(playerid, "Actualmente tienes ~r~%d trabajos~w~, usa /renunciar para dejar un trabajo.", player_jobs);
			SendFormatNotification(playerid, "¡Los jugadores ~y~VIP~w~ pueden tener hasta %d trabajos! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP~w~.", MAX_SU_WORKS);
			return 1;
		}
	}
	
	for(new i = 0; i != E_WORKS; i ++)	
	{
		if(work_info[i][work_info_TYPE] == WORK_TYPE_FAMILY) continue;
		
		if(GetPlayerInterior(playerid) == obtain_work_coords[i][obtain_work_INTERIOR])
		{
			if(IsPlayerInRangeOfPoint(playerid, 1.0, obtain_work_coords[i][obtain_work_X], obtain_work_coords[i][obtain_work_Y], obtain_work_coords[i][obtain_work_Z]))
			{
				if(work_info[i][work_info_LEVEL] > ACCOUNT_INFO[playerid][ac_LEVEL])
				{
					SendFormatNotification(playerid, "Necesitas ser al menos nivel %d para obtener este trabajo.", work_info[i][work_info_LEVEL]);
					return 1;
				}
				
				if(PLAYER_WORKS[playerid][i][pwork_SET])
				{
					SendFormatNotification(playerid, "Ya eres %s.", work_info[i][work_info_NAME]);
					return 1;
				}
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO pworks (id_player, id_work, `set`, level) VALUES(%d, %d, 1, 1) ON DUPLICATE KEY UPDATE `set` = 1;", ACCOUNT_INFO[playerid][ac_ID], i);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				PLAYER_WORKS[playerid][i][pwork_SET] = true;
				PLAYER_TEMP[playerid][pt_LAST_GOT_WORK_TIME] = gettime();
				CallLocalFunction("OnPlayerObtainWork", "ii", playerid, i);
				return 1;
			}
		}
	}
	
	SendNotification(playerid, "No estás en el lugar adecuado.");
	return 1;
}

CMD:dimitir(playerid, params[])
{
	new player_jobs = CountPlayerJobs(playerid);
	if(!player_jobs) return SendNotification(playerid, "No tienes ningún trabajo.");
	
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	
	new work;
	if(sscanf(params, "d", work))
	{
		new works[32 * sizeof(work_info)], line_str[32], count;
		for(new i = 1; i != sizeof(work_info); i ++)
		{
			if(PLAYER_WORKS[playerid][i][pwork_SET])
			{
				if(count > 0) strcat(works, "~w~, ");
				
				format(line_str, sizeof line_str, "~b~%s (%d)", work_info[i][work_info_NAME], i);
				strcat(works, line_str);
				
				count ++;
			}
		}
		SendFormatNotification(playerid, "Tus trabajos: ~b~%s", works);
		SendNotification(playerid, "Utiliza ~b~/renunciar~w~ <id del trabajo> para renunciar a un trabajo.");
		return 1;
	}
	
	if(work < 1 || work >= sizeof work_info) return SendNotification(playerid, "El trabajo no es válido.");
	
	if(!PLAYER_WORKS[playerid][work][pwork_SET]) return SendFormatNotification(playerid, "No eres %s.", work_info[work][work_info_NAME]);
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] == work) return SendNotification(playerid, "Ahora mismo estás de servicio en ese trabajo, deja de trabajar para abandonar el trabajo.");
	
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
	SendFormatNotification(playerid, "Has abandonado tu trabajo de ~b~%s~w~.", work_info[work][work_info_NAME]);
	PLAYER_WORKS[playerid][work][pwork_SET] = false;
	return 1;
}
alias:dimitir("renunciar")

CMD:pagar(playerid, params[])
{
	new vehicleid = INVALID_VEHICLE_ID;
	if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) vehicleid = GetPlayerCameraTargetVehicle(playerid);
	if(vehicleid == INVALID_VEHICLE_ID) return SendNotification(playerid, "No estás en el lugar adecuado.");

	if(!PLAYER_VEHICLES[vehicleid][player_vehicle_VALID] || PLAYER_VEHICLES[vehicleid][player_vehicle_CLAMP] <= 0) return SendNotification(playerid, "Este vehículo no tiene ninguna multa.");

	if(GivePlayerCash(playerid, -PLAYER_VEHICLES[vehicleid][player_vehicle_CLAMP], true, true)) {
		PLAYER_VEHICLES[vehicleid][player_vehicle_CLAMP] = 0;
		SendNotification(playerid, "Has pagado la multa y se ha quitado el cepo.");
		SetClampVehicleLabel(vehicleid);
	}
	else SendNotification(playerid, "No tienes suficiente dinero para pagar la multa.");
	return 1;
}

CMD:cepo(playerid, params[]) {
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] < 1) return SendFormatNotification(playerid, "Tienes que ser %s para poder hacer esto.", POLICE_RANKS[1]);

	new price;
	if(sscanf(params, "d", price)) return SendNotification(playerid, "~r~Modo de uso: ~w~/cepo [multa]");
	if(price < 0 || price > 2000) return SendNotification(playerid, "El precio de la multa no es válido.");

	new vehicleid = INVALID_VEHICLE_ID;
	if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) vehicleid = GetPlayerCameraTargetVehicle(playerid);
	if(vehicleid == INVALID_VEHICLE_ID) return SendNotification(playerid, "No estás cerca de un vehículo.");

	if(!PLAYER_VEHICLES[vehicleid][player_vehicle_VALID]) return SendNotification(playerid, "No puedes multar a este vehículo.");
	if(!VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400 ][vehicle_info_NORMAL_SPEEDO]) return SendNotification(playerid, "No puedes multar a este vehículo.");
	if(PLAYER_VEHICLES[vehicleid][player_vehicle_CLAMP] > 0 && price != 0) return SendNotification(playerid, "Este vehículo ya tiene una multa.");

	if(PLAYER_VEHICLES[vehicleid][player_vehicle_CLAMP] > 0 && price == 0) {
		PLAYER_VEHICLES[vehicleid][player_vehicle_CLAMP] = 0;
		SetClampVehicleLabel(vehicleid);

		SendNotification(playerid, "Se ha quitado la multa a este vehículo.");
		Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
	}
	else {
		if(price == 0) return SendNotification(playerid, "El precio de la multa no es válido.");
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] != INVALID_PLAYER_ID) return SendNotification(playerid, "No puedes multar a un vehículo si hay un conductor en él.");

		GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
		UpdateVehicleParams(vehicleid);

		PLAYER_VEHICLES[vehicleid][player_vehicle_CLAMP] = price;
		SetClampVehicleLabel(vehicleid);

		SendNotification(playerid, "Vehículo multado.");
		Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
	}
	return 1;
}

CMD:abrir(playerid, params[])
{
	new vehicleid = INVALID_VEHICLE_ID;
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_PASSENGER) vehicleid = GetPlayerVehicleID(playerid);
	else if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) vehicleid = GetPlayerCameraTargetVehicle(playerid);
		
	if(vehicleid == INVALID_VEHICLE_ID) return SendNotification(playerid, "No estás en tu vehículo o cerca de él para abrirlo.");
	if(!PLAYER_VEHICLES[vehicleid][player_vehicle_VALID]) return SendNotification(playerid, "Este no es tú vehículo.");
	if(PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return SendNotification(playerid, "Este no es tú vehículo.");
	
	if(!ACCOUNT_INFO[playerid][ac_VIP] && !PLAYER_VEHICLES[vehicleid][player_vehicle_ACCESSIBLE])
	{
		SendNotification(playerid, "Este vehículo está bloqueado, desbloquealo con VIP.");
		SendFormatNotification(playerid, "¡Los jugadores ~y~VIP ~w~pueden tener hasta %d vehículos! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP~w~.", MAX_SU_VEHICLES);
		return 1;
	}
	
	if(!GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS]) return SendNotification(playerid, "Las puertas de tu vehículo ya están abiertas.");
	
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS] = 0;
	UpdateVehicleParams(vehicleid);

	SendNotification(playerid, "Has ~g~abierto~w~ las puertas de tu vehículo.");
	Auto_SendPlayerAction(playerid, "ha abierto las puertas de su vehículo.");
	return 1;
}

CMD:cerrar(playerid, params[])
{
	new vehicleid = INVALID_VEHICLE_ID;
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_PASSENGER) vehicleid = GetPlayerVehicleID(playerid);
	else if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) vehicleid = GetPlayerCameraTargetVehicle(playerid);
		
	if(vehicleid == INVALID_VEHICLE_ID) return SendNotification(playerid, "No estás en tu vehículo o cerca de él para abrirlo.");
	if(!PLAYER_VEHICLES[vehicleid][player_vehicle_VALID]) return SendNotification(playerid, "Este no es tú vehículo.");
	if(PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return SendNotification(playerid, "Este no es tú vehículo.");
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS]) return SendNotification(playerid, "Las puertas de tu vehículo ya están cerradas.");
	
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_CLOSED_TIME] = gettime();
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS] = 1;
	UpdateVehicleParams(vehicleid);

	SendNotification(playerid, "Has ~r~cerrado~w~ las puertas de tu vehículo.");
	Auto_SendPlayerAction(playerid, "ha cerrado las puertas de su vehículo.");
	return 1;
}

CMD:aparcar(playerid, params[])
{
	new vehicleid = INVALID_VEHICLE_ID;
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_PASSENGER) vehicleid = GetPlayerVehicleID(playerid);
		
	if(vehicleid == INVALID_VEHICLE_ID) return SendNotification(playerid, "No estás en tu vehículo para aparcarlo.");
	if(!PLAYER_VEHICLES[vehicleid][player_vehicle_VALID]) return SendNotification(playerid, "Este no es tú vehículo.");
	if(PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return SendNotification(playerid, "Este no es tú vehículo.");
	
	new Float:pos[3], Float:angle;
	GetVehiclePos(vehicleid, pos[0], pos[1], pos[2]);
	GetVehicleZAngle(vehicleid, angle);
	
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_X] = pos[0];
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Y] = pos[1];
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Z] = pos[2];
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_ANGLE] = angle;
	
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_CLOSED_TIME] = gettime();
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_LIGHTS] = 0;
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS] = 1;
	UpdateVehicleParams(vehicleid);
	SendNotification(playerid, "Has estacionado tu vehículo, cuando sea destruido volverá a aparecer aquí.");
	return 1;
}
alias:aparcar("estacionar")

CMD:luces(playerid, params[])
{
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_LIGHTS])
		{
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_LIGHTS] = 0;
			UpdateVehicleParams(vehicleid);
			
			if(PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID]) Auto_SendPlayerAction(playerid, "ha apagado las luces de su vehículo.");
			else Auto_SendPlayerAction(playerid, "ha apagado las luces de el vehículo.");
		}
		else
		{
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_LIGHTS] = 1;
			UpdateVehicleParams(vehicleid);
			
			if(PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID]) Auto_SendPlayerAction(playerid, "ha encendido las luces de su vehículo.");
			else Auto_SendPlayerAction(playerid, "ha encendido las luces de el vehículo.");
		}
	}
	else SendNotification(playerid, "No estás conduciendo ningún vehículo.");
	return 1;
}

CMD:motor(playerid, params[])
{
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		new vehicleid = GetPlayerVehicleID(playerid);

		if(!GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE]) {
			if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK)
			{
				if(TRUCK_VEHICLE[vehicleid][truck_vehicle_LOADING]) return SendNotification(playerid, "Debes esperar a que se cargue el camión para arrancar.");
				if(TRUCK_VEHICLE[vehicleid][truck_vehicle_UNLOADING]) return SendNotification(playerid, "Debes esperar a que se descargue el camión para arrancar.");
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
	else SendNotification(playerid, "No estás conduciendo ningún vehículo.");
	return 1;
}
alias:motor("arrancar")

CMD:reparar(playerid, params[]) {
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	if(PI[playerid][ac_MECHANIC_KITS] <= 0) return SendNotification(playerid, "No tienes un kit de reparación, compra uno en el taller.");

	new vehicleid = GetPlayerCameraTargetVehicle(playerid);
	if(vehicleid == INVALID_VEHICLE_ID) return SendNotification(playerid, "No estás cerca de ningún vehículo.");
	
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] != VEHICLE_STATE_DAMAGED) return SendNotification(playerid, "El vehículo no está dañado.");

	RepairVehicleEx(vehicleid, playerid, MIN_VEHICLE_HEALTH + 50.0);
	PI[playerid][ac_MECHANIC_KITS] --;
	SendNotification(playerid, "~w~Has usado ~y~1 kit de reparación ~w~para arreglar este vehículo.");
	return 1;
}

CMD:vmenu(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_SET]) return SendNotification(playerid, "No eres mecánico.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_MECHANIC) return SendNotification(playerid, "No estás de servicio como mecánico.");
	
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	
	new vehicleid = GetPlayerCameraTargetVehicle(playerid);
	if(vehicleid == INVALID_VEHICLE_ID) return SendNotification(playerid, "No estás cerca de ningún vehículo.");
	
	new bool:in_mechanic_area;
	for(new i; i != sizeof Mechanic_Areas; i++)
		if(IsPlayerInDynamicArea(playerid, Mechanic_Areas[i]))
		{
			in_mechanic_area = true;
			break;
		}


	if(!in_mechanic_area)
	{
		SendNotification(playerid, "Para reparar este vehículo tienes que estar en el taller.");
		return 1;
	}

	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE]) return SendNotification(playerid, "Para trabajar sobre el vehículo el motor debe estar apagado.");
	if(!VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400 ][vehicle_info_NORMAL_SPEEDO] || GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] <= 0.0) return SendNotification(playerid, "No se puede modificar este vehículo aquí.");
	
	PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] = vehicleid;
	ShowDialog(playerid, DIALOG_MECHANIC_MENU);
	return 1;
}

CMD:policia(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_NONE && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE)
	{
		SendFormatNotification(playerid, "Tienes que dejar de estar de servicio como %s primero.", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME]);
		return 1;
	}
	
	if(GetPlayerVirtualWorld(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	
	if(IsPlayerInRangeOfPoint(playerid, 1.0, 253.280288, 76.219017, 1003.640625) || IsPlayerInRangeOfPoint(playerid, 1.0, 266.547180, 119.652816, 1004.617187) || IsPlayerInRangeOfPoint(playerid, 1.0, 217.816406, 187.667922, 1003.031250))
	{
		if(!PLAYER_TEMP[playerid][pt_WORKING_IN]) ShowDialog(playerid, DIALOG_SELECT_POLICE_SKIN);
		else CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
		return 1;
	}
	
	SendNotification(playerid, "No estás en el lugar adecuado.");
	return 1;
}
CMD:talar(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_LUMBERJACK][pwork_SET]) return SendNotification(playerid, "No eres leñador.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_NONE && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_LUMBERJACK)
	{
		SendFormatNotification(playerid, "Tienes que dejar de estar de servicio como %s primero.", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME]);
		return 1;
	}
	
	if(GetPlayerVirtualWorld(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerInterior(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	if(!IsPlayerInRangeOfPoint(playerid, 1.0, -527.670349, -97.338562, 63.176174)) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	if(!PLAYER_TEMP[playerid][pt_WORKING_IN]) CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_LUMBERJACK, INVALID_VEHICLE_ID);
	else CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
	return 1;
}

CMD:mecanico(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_SET]) return SendNotification(playerid, "No eres mecánico.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_NONE && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_MECHANIC)
	{
		SendFormatNotification(playerid, "Tienes que dejar de estar de servicio como %s primero.", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME]);
		return 1;
	}
	
	if(GetPlayerInterior(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");

	for(new i; i != sizeof MechanicStartWorkingCoords; i++)
		if(IsPlayerInRangeOfPoint(playerid, 1.0, MechanicStartWorkingCoords[i][0], MechanicStartWorkingCoords[i][1], MechanicStartWorkingCoords[i][2]))
		{
			if(!PLAYER_TEMP[playerid][pt_WORKING_IN]) CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_MECHANIC, INVALID_VEHICLE_ID);
			else CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);

			return 1;
		}


	SendNotification(playerid, "No estás en el lugar adecuado.");
	return 1;
}

CMD:medico(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_MEDIC][pwork_SET]) return SendNotification(playerid, "No eres médico.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_NONE && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_MEDIC)
	{
		SendFormatNotification(playerid, "Tienes que dejar de estar de servicio como %s primero.", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME]);
		return 1;
	}
	
	if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] == -1 || ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] != INTERIOR_HOSPITAL) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	if(!IsPlayerInRangeOfPoint(playerid, 1.0, -2029.751342, -114.503044, 1035.171875)) return SendNotification(playerid, "No estás en el lugar adecuado.");

	if(!PLAYER_TEMP[playerid][pt_WORKING_IN]) CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_MEDIC, INVALID_VEHICLE_ID);
	else CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
	return 1;
}

CMD:pizzero(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_PIZZA][pwork_SET]) return SendNotification(playerid, "No eres pizzero, el trabajo se consigue dentro de la pizzería.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_NONE && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_PIZZA)
	{
		SendFormatNotification(playerid, "Tienes que dejar de estar de servicio como %s primero.", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME]);
		return 1;
	}
	
	if(GetPlayerVirtualWorld(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	if(!IsPlayerInRangeOfPoint(playerid, 1.0, 2097.355712, -1818.040771, 13.382812)) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	if(!PLAYER_TEMP[playerid][pt_WORKING_IN]) CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_PIZZA, INVALID_VEHICLE_ID);
	else CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
	return 1;
}

CMD:semillas(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_FARMER][pwork_SET]) return SendNotification(playerid, "No eres agricultor.");
	if(GetPlayerVirtualWorld(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerInterior(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	if(!IsPlayerInRangeOfPoint(playerid, 1.0, 1566.521606, 31.370532, 24.16406)) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	ShowDialog(playerid, DIALOG_SEED_LIST);
	return 1;
}

CMD:plantar(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_FARMER][pwork_SET]) return SendNotification(playerid, "No eres agricultor.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_FARMER)
	{
		SendFormatNotification(playerid, "Tienes que dejar de estar de servicio como %s primero.", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME]);
		return 1;
	}
	
	if(GetPlayerVirtualWorld(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerInterior(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	if(!IsPlayerInDynamicArea(playerid, Farmer_Area)) return SendNotification(playerid, "No estás en la zona para plantar, está marcada en el mapa con una bandera.");
	if(PLAYER_TEMP[playerid][pt_PLANTING]) return SendNotification(playerid, "Ahora mismo ya estás plantando algo.");
	
	if(gettime() < PLAYER_TEMP[playerid][pt_LAST_PLANT_TIME] + 60)
	{
		new time = (60-(gettime()-PLAYER_TEMP[playerid][pt_LAST_PLANT_TIME]));
		SendFormatNotification(playerid, "Tienes que esperar %s segundos para volver a plantar.", TimeConvert(time));
		return 1;
	}
	
	if(GetPlayerPlantedPlants(playerid) > 10) return SendNotification(playerid, "Por favor, recoge tus otras plantas para poder plantar más.");
	
	for(new i = 0; i != MAX_PLANTS; i ++)
	{
		if(!PLANTS[i][plant_VALID]) continue;
		
		new Float:pos[3];
		GetDynamicObjectPos(PLANTS[i][plant_OBJECT_ID], pos[0], pos[1], pos[2]);
		if(IsPlayerInRangeOfPoint(playerid, 3.0, pos[0], pos[1], pos[2]))
		{
			SendNotification(playerid, "Aquí ya hay una planta, aléjate un poco para plantar.");
			return 1;
		}
	}
	
	ShowDialog(playerid, DIALOG_PLANT_PLANTS);
	return 1;
}

CMD:piezas(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_SET]) return SendNotification(playerid, "No eres mecánico.");
	if(GetPlayerInterior(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	
	for(new i; i != sizeof MechanicBuyPiecesCoords; i++)
		if(IsPlayerInRangeOfPoint(playerid, 1.0, MechanicBuyPiecesCoords[i][0], MechanicBuyPiecesCoords[i][1], MechanicBuyPiecesCoords[i][2]))
		{
			if(sscanf(params, "d", params[0])) return SendNotification(playerid, "~r~Modo de uso: ~w~/piezas [cantidad]");
			if(params[0] <= 0 || params[0] >= 10000) return SendNotification(playerid, "~r~Modo de uso: ~w~/piezas [cantidad > 0]");

			new price = params[0] * 50;
			if(price > PI[playerid][ac_CASH])
			{
				SendFormatNotification(playerid, "Necesitas %s$ para poder comprar %d piezas.", number_format_thousand(price), params[0]);
				return 1;
			}
			if(price < 0) return 1;

			if(GivePlayerCash(playerid, -price, true, true)) {
				PI[playerid][ac_MECHANIC_PIECES] += params[0];

				SendFormatNotification(playerid, "Has comprado %s piezas por %s$, ahora tienes %s piezas.", number_format_thousand(params[0]), number_format_thousand(price), number_format_thousand(PI[playerid][ac_MECHANIC_PIECES]));
			}
			return 1;
		}

	SendNotification(playerid, "No estás en el lugar adecuado.");
	return 1;
}

CMD:kit(playerid, params[])
{
	if(GetPlayerInterior(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	
	for(new i; i != sizeof MechanicBuyKitsCoords; i++)
		if(IsPlayerInRangeOfPoint(playerid, 1.0, MechanicBuyKitsCoords[i][0], MechanicBuyKitsCoords[i][1], MechanicBuyKitsCoords[i][2]))
		{
			if(GivePlayerCash(playerid, -1000, true, true)) {
				PI[playerid][ac_MECHANIC_KITS] += 1;
				SendNotification(playerid, "~w~Has comprado un kit de reparación, para usararlo usa ~y~/reparar ~w~cerca del vehículo que quieras reparar.");
			}
			else SendNotification(playerid, "No tienes suficiente dinero.");
			return 1;
		}

	SendNotification(playerid, "No estás en el lugar adecuado.");
	return 1;
}

CMD:botiquin(playerid, params[])
{
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	if(!PLAYER_WORKS[playerid][WORK_MEDIC][pwork_SET] && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_MEDIC) return SendNotification(playerid, "Solo los médicos pueden comprar botiquines.");
	
	for(new i; i != sizeof MedicalBuyKitsCoords; i++)
		if(IsPlayerInRangeOfPoint(playerid, 1.0, MedicalBuyKitsCoords[i][0], MedicalBuyKitsCoords[i][1], MedicalBuyKitsCoords[i][2]))
		{
			if(GivePlayerCash(playerid, -5000, true, true)) {
				PI[playerid][ac_MEDICAL_KITS] += 1;
				SendNotification(playerid, "~w~Has comprado un botiquín, para usararlo usa ~y~/curar ~w~cerca de la persona que quieras curar.");
			}
			else SendNotification(playerid, "No tienes suficiente dinero.");
			return 1;
		}

	SendNotification(playerid, "No estás en el lugar adecuado.");
	return 1;
}


CMD:recoger(playerid, params[])
{
	if(!IsPlayerInDynamicArea(playerid, Farmer_Area)) return 1;

	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	
	for(new i = 0; i != MAX_PLANTS; i ++)
	{
		if(!PLANTS[i][plant_VALID] || PLANTS[i][plant_GROWING]) continue;
		
		new Float:pos[3];
		GetDynamicObjectPos(PLANTS[i][plant_OBJECT_ID], pos[0], pos[1], pos[2]);

		if(IsPlayerInRangeOfPoint(playerid, 1.5, pos[0], pos[1], pos[2]))
		{
			if(PLANTS[i][plant_IMMUNITY] > 0 && PLANTS[i][plant_PLANTED_BY_ACCOUNT_ID] != ACCOUNT_INFO[playerid][ac_ID]) return SendFormatNotification(playerid, "Esta planta tiene tiempo de inmunidad.");
			
			KillTimer(PLANTS[i][plant_TIMER]);
			switch(seed_info[ PLANTS[i][plant_TYPE] ][seed_info_PLANT_TYPE])
			{
				case PLANT_TYPE_MEDICINE:
				{
					PI[playerid][ac_MEDICINE] += 2;
					SendNotification(playerid, "Has recogido la planta y has obtenido ~g~2 gramos de medicamento~w~.");
				}
				case PLANT_TYPE_CANNABIS:
				{
					PI[playerid][ac_CANNABIS] += 6;
					SendNotification(playerid, "Has recogido la planta y has obtenido ~g~6 gramos de marihuana~w~.");
				}
				case PLANT_TYPE_CRACK:
				{
					PI[playerid][ac_CRACK] += 4;
					SendNotification(playerid, "Has recogido la planta y has obtenido ~g~4 gramos de crack~w~.");
				}
			}
			
			DestroyDynamicObject(PLANTS[i][plant_OBJECT_ID]);
			DestroyDynamic3DTextLabel(PLANTS[i][plant_LABEL_ID]);
			ApplyAnimation(playerid, "CARRY", "putdwn05", 4.1, 0, 1, 1, 0, 0, 1);
			
			new tmp_PLANTS[PLANTS_ENUM]; PLANTS[i] = tmp_PLANTS;
			return 1;
		}
	}
	return 1;
}

//Animaciones
//ANIMACIONES
CMD:animaciones(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ShowDialog(playerid, DIALOG_ANIMS);
	return 1;
}
alias:animaciones("anims", "acciones")
CMD:parar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.1, 0, 0, 0, 0, 0, true);
	ClearAnimations(playerid);
	return 1;
}
CMD:rendirse(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_HANDSUP); //rendirse
    SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:blowjob(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid, "BLOWJOBZ", "BJ_COUCH_LOOP_W", 4.0, 1, 1, 1, 0, 0);//blowjob
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:rodar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid,"PED","BIKE_fallR",4.0,0,1,1,1,0);
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:borracho(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    ApplyAnimation(playerid,"PED", "WALK_DRUNK",4.0,1,1,1,1,500);//borracho
    SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:bomba(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 1, 1, 0,0);//bomba
    SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:apuntar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation( playerid,"ped", "ARRESTgun", 4.0, 0, 1, 1, 1,500);
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:reir(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    ApplyAnimation(playerid, "RAPPING", "Laugh_01", 4.0, 0, 0, 0, 0,0);//reir
    SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:amenazar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    ApplyAnimation(playerid, "SHOP", "ROB_Loop_Threat", 4.0, 0, 0, 0, 1,500);//amenazar
    SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:paja(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid, "PAULNMAC", "wank_loop", 4.0, 1, 0, 0, 1, 0);
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:herido(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid, "SWEET", "LaFin_Sweet", 4.0, 0, 1, 1, 1, 0);
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:cruzarbrazos(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid, "COP_AMBIENT", "Coplook_loop", 4.0, 1, 1, 1, 0, 4000);
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:recostarse(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid,"SUNBATHE", "Lay_Bac_in", 4.0, 0, 0, 0, 1, 0);//recostarse
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:cubrirse(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    ApplyAnimation(playerid, "ped", "cower", 4.0, 1, 0, 0, 0, 0);//crubrirse
    SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:vomitar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    ApplyAnimation(playerid, "FOOD", "EAT_Vomit_P", 3.0, 0, 0, 0, 0, 0);//vomitar
    SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:comer(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.00, 0, 0, 0, 0, 0);
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:despedir(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid, "KISSING", "BD_GF_Wave", 3.0, 0, 0, 0, 0, 0);//despedir
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:nalgada(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid, "SWEET", "sweet_ass_slap", 4.0, 0, 0, 0, 0, 0);//nalgada
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:agonizar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid, "WUZI", "CS_Dead_Guy", 4.0, 0, 0, 0, 1, 0);//agonizar
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:besar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid, "KISSING", "Playa_Kiss_02", 4.0, 0, 0, 0, 0, 0);//besar
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:crack(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    ApplyAnimation(playerid, "CRACK", "crckdeth2", 4.0, 0, 0, 0, 1, 0);//crack
    SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:mear(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_PISSING);//mear
    SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:sentarse(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_in", 4.000000, 0, 1, 1, 1, 0);//sentarse
    SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:asiento(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    ApplyAnimation(playerid, "ped", "SEAT_down", 4.000000, 0, 1, 1, 1, 0);
    SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:fucku(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    ApplyAnimation( playerid,"ped", "fucku", 4.0, 0, 1, 1, 1, 1 );//fucku
    SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:taichi(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid, "PARK", "Tai_Chi_Loop",  4.1,7,5,1,1,1);//taichi
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:beber(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid, "BAR", "dnk_stndM_loop", 4.0, 0, 1, 1, 0, 4000);//beber
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:boxear(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid, "GYMNASIUM", "gym_shadowbox",  4.1,7,5,1,1,1);//boxear
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:saludar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid,"GANGS","hndshkfa_swt",4.1,0,0,0,0,0);//saludar
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:llorar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid,"GRAVEYARD","mrnF_loop",4.1,0,0,0,0,0);//llorar
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:dormir(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(!GetPlayerInterior(playerid)) return SendNotification(playerid, "Solo puedes hacer esto en un interior");
	ApplyAnimation(playerid,"INT_HOUSE","BED_In_R",4.1,0,0,0,1,0);//dormir
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:rapear(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid,"RAPPING","RAP_B_Loop",4.0,1,0,0,0,8000);//rapear
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:astrip(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid,"STRIP","strip_A",4.1,7,5,1,1,1);//strip
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:bailar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    if(sscanf(params, "d", params[0])) return SendClientMessage(playerid, 0xFF4500FF, "ERROR: /bailar [1-4]");

	switch(params[0])
	{
	    case 1: SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE1);
		case 2: SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE2);
		case 3: SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE3);
		case 4: SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE4);
		default: SendClientMessage(playerid, 0xFF4500FF, "ERROR: /bailar [1-4]");
	}
	return 1;
}
CMD:alentar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	ApplyAnimation(playerid,"ON_LOOKERS","shout_02",4.1,7,5,1,1,1);//alentar
	SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}
CMD:hablar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
    ApplyAnimation(playerid,"PED","IDLE_chat",4.1,7,5,1,1,1);//hablar
    SendNotification(playerid, "Para cancelar la animacion utiliza /parar.");
	return 1;
}

CMD:guardar(playerid, params[])
{
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	
	new option[24], extra;
	if(sscanf(params, "s[24]d", option, extra)) return SendNotification(playerid, "Error en los parámetros, utilice ~r~/man guardar~w~.");

	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_NORMAL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK)
	{
		new vehicleid = GetPlayerCameraTargetVehicle(playerid);
		if(vehicleid == INVALID_VEHICLE_ID) return SendNotification(playerid, "No estás cerca de tu vehículo.");
		
		if(!PLAYER_VEHICLES[vehicleid][player_vehicle_VALID]) return SendNotification(playerid, "Este no es tú vehículo.");
		if(PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return SendNotification(playerid, "Este no es tú vehículo.");
				
		new boot_slot = GetPlayerVehicleAvaibleBootSlot(vehicleid);
		if(boot_slot == -1) return SendNotification(playerid, "No tienes suficiente espacio en tu /maletero.");

		if(!strcmp(option, "arma", true))
		{
			if(extra < 0 || extra >= sizeof PLAYER_WEAPONS[]) return SendNotification(playerid, "Error: ~b~/guardar arma {FFFFFF}[slot/armas]");
			if(!PLAYER_WEAPONS[playerid][extra][player_weapon_VALID])
			{
				PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
				SendNotification(playerid, "No tienes ninguna arma en ese slot (/armas).");
				return 1;
			}

			SendFormatNotification(playerid, "Arma ~g~'%s' ~w~guardada en el maletero de tu %s.", WEAPON_INFO[ PLAYER_WEAPONS[playerid][extra][player_weapon_ID] ][weapon_info_NAME], VEHICLE_INFO[GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_NAME]);

			new string[128];
			format(string, sizeof string, "guarda un(a) %s en el maletero de su vehículo.", WEAPON_INFO[ PLAYER_WEAPONS[playerid][extra][player_weapon_ID] ][weapon_info_NAME]);
			Auto_SendPlayerAction(playerid, string);
			
			SavePlayerWeaponInVehicleBoot(playerid, extra, vehicleid, boot_slot);
		}
		else if(!strcmp(option, "medicamentos", true)) {
			if(extra < 0 || extra > PI[playerid][ac_MEDICINE]) return SendNotification(playerid, "Cantidad incorrecta.");
			
			PI[playerid][ac_MEDICINE] -= extra;
			RegisterVehicleBootObject(vehicleid, boot_slot, BOOT_TYPE_MEDICINES, extra, 0);
			
			Auto_SendPlayerAction(playerid, "guarda medicamentos en el maletero de su vehículo.");
		}
		else if(!strcmp(option, "marihuana", true)) {
			if(extra < 0 || extra > PI[playerid][ac_CANNABIS]) return SendNotification(playerid, "Cantidad incorrecta.");
			
			PI[playerid][ac_CANNABIS] -= extra;
			RegisterVehicleBootObject(vehicleid, boot_slot, BOOT_TYPE_CANNABIS, extra, 0);

			Auto_SendPlayerAction(playerid, "guarda marihuana en el maletero de su vehículo.");
		}
		else if(!strcmp(option, "crack", true)) {
			if(extra < 0 || extra > PI[playerid][ac_CRACK]) return SendNotification(playerid, "Cantidad incorrecta.");
			
			PI[playerid][ac_CRACK] -= extra;
			RegisterVehicleBootObject(vehicleid, boot_slot, BOOT_TYPE_CRACK, extra, 0);

			Auto_SendPlayerAction(playerid, "guarda crack en el maletero de su vehículo.");
		}
		else SendNotification(playerid, "Error en los parámetros, utilice ~b~/man guardar~w~.");
	}
	else if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_OWN_PROPERTY)
	{
		new index = GetPropertyIndexByID(PI[playerid][ac_LOCAL_INTERIOR]);
		if(index == -1) return SendNotification(playerid, "BUG: CMD /guardar (casa), Tome captura y contacte con administrador.");
		if(PROPERTY_INFO[index][property_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return SendNotification(playerid, "Esta no es tu casa");

		new Float:z_pos = PROPERTY_CLOSET_POS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_closet_Z];
		if(PROPERTY_INFO[index][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
		if(IsPlayerInRangeOfPoint(playerid, NEARS_PLAYERS_DISTANCE, PROPERTY_CLOSET_POS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_closet_X], PROPERTY_CLOSET_POS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_closet_Y], z_pos))
		{
			new closet_slot = GetPropertyAvaibleClosetSlot(index);
			if(closet_slot == -1) return SendNotification(playerid, "No tienes suficiente espacio en tu /armario.");
			
			if(!strcmp(option, "arma", true))
			{
				if(extra < 0 || extra >= sizeof PLAYER_WEAPONS[]) return SendNotification(playerid, "~r~Modo de uso: ~w~/guardar arma [slot /armas]");
				if(!PLAYER_WEAPONS[playerid][extra][player_weapon_VALID])
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "No tienes ninguna arma en ese slot (/armas).");
					return 1;
				}

				SendFormatNotification(playerid, "Arma ~g~'%s' ~w~guardada en el armario de tu casa.", WEAPON_INFO[ PLAYER_WEAPONS[playerid][extra][player_weapon_ID] ][weapon_info_NAME]);

				new string[128];
				format(string, sizeof string, "guarda un(a) %s en el armario de su casa.", WEAPON_INFO[ PLAYER_WEAPONS[playerid][extra][player_weapon_ID] ][weapon_info_NAME]);
				Auto_SendPlayerAction(playerid, string);
				
				SaveWeaponInPropertyCloset(playerid, extra, index, closet_slot);
			}
			else if(!strcmp(option, "medicamentos", true)) {
				if(extra < 0 || extra > PI[playerid][ac_MEDICINE]) return SendNotification(playerid, "Cantidad incorrecta.");
				
				PI[playerid][ac_MEDICINE] -= extra;
				RegisterPropertyClosetObject(index, closet_slot, BOOT_TYPE_MEDICINES, extra, 0);
				
				Auto_SendPlayerAction(playerid, "guarda medicamentos en el armario.");
			}
			else if(!strcmp(option, "marihuana", true)) {
				if(extra < 0 || extra > PI[playerid][ac_CANNABIS]) return SendNotification(playerid, "Cantidad incorrecta.");
				
				PI[playerid][ac_CANNABIS] -= extra;
				RegisterPropertyClosetObject(index, closet_slot, BOOT_TYPE_CANNABIS, extra, 0);
				
				Auto_SendPlayerAction(playerid, "guarda marihuana en el armario.");
			}
			else if(!strcmp(option, "crack", true)) {
				if(extra < 0 || extra > PI[playerid][ac_CRACK]) return SendNotification(playerid, "Cantidad incorrecta.");
				
				PI[playerid][ac_CRACK] -= extra;
				RegisterPropertyClosetObject(index, closet_slot, BOOT_TYPE_CRACK, extra, 0);
				
				Auto_SendPlayerAction(playerid, "guarda crack en el armario.");
			}
			else SendNotification(playerid, "Error en los parámetros, utilice ~b~/man guardar~w~.");
		}
		else SendNotification(playerid, "No estás cerca del armario.");
	}
	else SendNotification(playerid, "Ahora no puedes usar este comando.");
	return 1;
}

CMD:maletero(playerid, params[])
{
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	
	new vehicleid = GetPlayerCameraTargetVehicle(playerid);
	if(vehicleid == INVALID_VEHICLE_ID) return SendNotification(playerid, "No estás cerca de tu vehículo.");
	
	if((PLAYER_TEMP[playerid][pt_WORKING_IN] == WORK_POLICE) && PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID])
	{
		if(!PLAYER_VEHICLES[vehicleid][player_vehicle_VALID]) return SendNotification(playerid, "Este vehiculo es del gobierno o de un consecionario.");
		new pid = -1;
		for(new i, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(PLAYER_TEMP[i][pt_USER_LOGGED] && ACCOUNT_INFO[i][ac_ID] == PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID])
			pid = i;
		}

		if(pid == -1) return SendNotification(playerid, "Error del juego: No se ha podido encontrar al dueño de este vehículo.");
		if(!PLAYER_TEMP[pid][pt_CUFFED]) return SendNotification(playerid, "El dueño de este vehículo debe estar esposado para poder revisar su vehículo.");
		
		PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] = vehicleid;
		ShowDialog(playerid, DIALOG_VEHICLE_BOOT);

		Auto_SendPlayerAction(playerid, "revisa el maltero del vehículo");
		return 1;
	}

	if(!PLAYER_VEHICLES[vehicleid][player_vehicle_VALID]) return SendNotification(playerid, "Este no es tú vehículo.");
	if(PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return SendNotification(playerid, "Este no es tú vehículo.");
		
	PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] = vehicleid;
	ShowDialog(playerid, DIALOG_VEHICLE_BOOT);
	return 1;
}
alias:maletero("baul")

SetWantedPlayerMarkers(playerid)
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
			{
				if(PI[i][ac_WANTED_LEVEL] > 0)
				{
					SetPlayerMarkerForPlayer(playerid, i, PlayerWantedColor(i));
				}
			}
		}
	}
	return 1;
}




CMD:nivel(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] < 2) return SendNotification(playerid, "Los cadetes y soldados rasos no pueden colocar cargos.");
	if(sscanf(params, "ud", params[0], params[1])) return SendNotification(playerid, "~r~Modo de uso: ~w~/nivel [PlayerID/Nombre] [nivel de búsqueda 0-6]");
	if(params[1] < 0 || params[1] > 6) return SendNotification(playerid, "~r~Modo de uso: ~w~/nivel [PlayerID/Nombre] [nivel de búsqueda 0-6]");
	
	if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "El jugador no está conectado.");
	if(PLAYER_WORKS[params[0]][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "Este jugador es policía.");
	if(ACCOUNT_INFO[params[0]][ac_STATE] == ROLEPLAY_STATE_JAIL) return SendNotification(playerid, "Esta persona está en la cárcel.");

	new Float:pos[3]; GetPlayerPos(params[0], pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, 100.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya. (A más de 100 metros)");

	new 
		action[64],
		message[145];

	if(params[1] == 0)
	{
		format(action, sizeof action, "le quita el nivel de búsqueda a %s.", PLAYER_TEMP[params[0]][pt_RP_NAME]);

		format(message, sizeof message, "{"#PRIMARY_COLOR"}[Central policía] {FFFFFF}%s reporta: {"#PRIMARY_COLOR"}%s {FFFFFF}ya no es sospechoso.", PLAYER_TEMP[playerid][pt_RP_NAME], PLAYER_TEMP[params[0]][pt_RP_NAME]);
		SendFormatNotification(playerid, "Le has removido el nivel de búsqueda a ~r~%s~w~.", PLAYER_TEMP[params[0]][pt_RP_NAME]);
	}
	else
	{
		new 
			zone[45], 
			city[45];

		GetPlayerZones(playerid, city, zone);

		format(action, sizeof action, "le pone nivel de búsqueda a %s.", PLAYER_TEMP[params[0]][pt_RP_NAME]);
		
		SendFormatNotification(playerid, "Has asignado nivel de búsqueda ~b~%d~w~ a ~r~%s~w~.", params[1], PLAYER_TEMP[params[0]][pt_RP_NAME]);
		format(message, sizeof message, "{"#PRIMARY_COLOR"}[Central policía] {FFFFFF}%s reporta: {"#PRIMARY_COLOR"}%s (%d*) {FFFFFF}visto por última vez en {"#PRIMARY_COLOR"}%s, %s.", PLAYER_TEMP[playerid][pt_RP_NAME], PLAYER_TEMP[params[0]][pt_RP_NAME], params[1], city, zone);
	}

	Auto_SendPlayerAction(playerid, action);

	SetPlayerWantedLevelEx(params[0], params[1]);
	SendPoliceRadioMessage(-1, -1, message);
	return 1;
}
alias:nivel("cargos", "c")

CMD:esposar(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	if(sscanf(params, "u", params[0])) return SendNotification(playerid, "Error: /esposar [PlayerID/Nombre]");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	
	if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "El jugador no está conectado.");
	new Float:pos[3]; GetPlayerPos(params[0], pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, NEARS_PLAYERS_DISTANCE, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");
	if(PLAYER_TEMP[params[0]][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No puedes esposar a este jugador ahora.");
	if(GetPlayerState(params[0]) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "Para esposar a esta persona tiene que estar depie.");
	if(PI[params[0]][ac_WANTED_LEVEL] == 0) return SendNotification(playerid, "Esta persona no tiene nivel de búsqueda.");
	
	new oldstate = ACCOUNT_INFO[params[0]][ac_STATE];
	DisablePlayerInjuredMark(params[0]);
	ACCOUNT_INFO[params[0]][ac_STATE] = ROLEPLAY_STATE_NORMAL;
	ApplyAnimation(params[0], "CARRY", "crry_prtial", 4.1, 0, 0, 0, 0, 0, true);
	ClearAnimations(params[0]);

	if(PLAYER_TEMP[params[0]][pt_CUFFED])
	{
		TogglePlayerControllableEx(params[0], true);
		PLAYER_TEMP[params[0]][pt_CUFFED] = false;
		PLAYER_TEMP[params[0]][pt_CUFFING] = false;
		SetPlayerSpecialAction(params[0], SPECIAL_ACTION_NONE);
		
		new action[64];
		format(action, sizeof action, "le quita las esposas a %s.", PLAYER_TEMP[params[0]][pt_RP_NAME]);
		Auto_SendPlayerAction(playerid, action);
		return 1;
	}
	
	if(!PLAYER_TEMP[params[0]][pt_CUFFING])
	{
		if(oldstate == ROLEPLAY_STATE_CRACK) {
			CuffPlayer(params[0]);
		}
		else {
			SendNotification(params[0], "Estás siendo esposado, puedes ~g~/resistirse~w~.");
			SendNotification(playerid, "Estás esposando a esta persona, aún puede resistirse.");
			
			PLAYER_TEMP[params[0]][pt_CUFFED] = false;
			PLAYER_TEMP[params[0]][pt_CUFFING] = true;
			KillTimer(PLAYER_TEMP[params[0]][pt_TIMERS][14]);
			TogglePlayerControllableEx(params[0], false);
			SetPlayerSpecialAction(params[0], SPECIAL_ACTION_HANDSUP);
			PLAYER_TEMP[params[0]][pt_TIMERS][14] = SetTimerEx("CuffPlayer", 6000, false, "i", params[0]);
		}
	}
	return 1;
}

CMD:placa(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	if(sscanf(params, "u", params[0])) return SendNotification(playerid, "Error: /placa [PlayerID/Nombre]");
	
	if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "El jugador no está conectado.");
	new Float:pos[3]; GetPlayerPos(params[0], pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");
	if(PLAYER_TEMP[params[0]][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No puedes enseñarle tu placa a este jugador ahora.");
	
	new action[64];
	format(action, sizeof action, "le enseña su placa a %s.", PLAYER_TEMP[params[0]][pt_RP_NAME]);
	Auto_SendPlayerAction(playerid, action);
	
	SendClientMessageEx(params[0], -1, "{"#SILVER_COLOR"}%s %c. %s {A9C4E4}[Placa: %d]", POLICE_RANKS[ PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] ], PLAYER_TEMP[playerid][pt_FIRST_NAME][0], PLAYER_TEMP[playerid][pt_SUB_NAME], PI[playerid][ac_PLACA_PD]);
	return 1;
}

CMD:licencia(playerid, params[])
{
	if(sscanf(params, "u", params[0])) return SendNotification(playerid, "Error: /licencia [PlayerID/Nombre]");
	if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "El jugador no está conectado.");
	new Float:pos[3]; GetPlayerPos(params[0], pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");

	if(PI[playerid][ac_DRIVE_LICENSE_POINTS] == 0) SendFormatNotification(params[0], "~r~%s~w~ no tiene licencia de conducción.", PLAYER_TEMP[playerid][pt_RP_NAME]);
	else
	{
		new action[128];
		format(action, 128, "le muestra su licencia de conducir a %s.", PLAYER_TEMP[params[0]][pt_RP_NAME]);
		Auto_SendPlayerAction(playerid, action);
		SendClientMessageEx(params[0], -1, "{"#SILVER_COLOR"}Licencia de conducir de %s, puntos: %d.", PLAYER_TEMP[playerid][pt_RP_NAME], PI[playerid][ac_DRIVE_LICENSE_POINTS]);
	}
	return 1;
}

CMD:revisar(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	if(sscanf(params, "u", params[0])) return SendNotification(playerid, "Error: /revisar [PlayerID/Nombre]");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	
	if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "El jugador no está conectado.");
	new Float:pos[3]; GetPlayerPos(params[0], pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");
	if(PLAYER_TEMP[params[0]][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No puedes revisar a este jugador ahora.");
	if(GetPlayerState(params[0]) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "Para revisar a esta persona tiene que estar depie.");
	if(PI[params[0]][ac_WANTED_LEVEL] == 0) return SendNotification(playerid, "Esta persona no tiene nivel de búsqueda.");
	if(!PLAYER_TEMP[params[0]][pt_CUFFED]) return SendNotification(playerid, "Para revisar a esta persona tiene que estar esposada.");
	
	ShowPlayerInventory(playerid, params[0]);
	
	new action[64];
	format(action, sizeof action, "revisa a %s.", PLAYER_TEMP[params[0]][pt_RP_NAME]);
	Auto_SendPlayerAction(playerid, action);
	return 1;
}

CMD:puntos(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	if(sscanf(params, "ud", params[0], params[1])) return SendNotification(playerid, "Error: /puntos [PlayerID/Nombre] [cantidad]");
	if(params[1] < 1 || params[1] > 12) return SendNotification(playerid, "Error: cantidad de puntos no válida.");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	
	if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "El jugador no está conectado.");
	new Float:pos[3]; GetPlayerPos(params[0], pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");
	if(PLAYER_TEMP[params[0]][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No puedes quitarle puntos a este jugador ahora.");
	
	PI[params[0]][ac_DRIVE_LICENSE_POINTS] -= params[1];
	if(PI[params[0]][ac_DRIVE_LICENSE_POINTS] < 0) PI[params[0]][ac_DRIVE_LICENSE_POINTS] = 0;
	
	SendFormatNotification(playerid, "Le has quitado ~r~%d puntos~w~ del carnet a %s, ahora tiene ~r~%d puntos~w~.", params[1], PLAYER_TEMP[params[0]][pt_RP_NAME], PI[params[0]][ac_DRIVE_LICENSE_POINTS]);
	SendFormatNotification(params[0], "El policía %s te ha quitado ~r~%d puntos~w~ del carnet de conducir, te quedan ~r~%d puntos~w~.", PLAYER_TEMP[playerid][pt_RP_NAME], params[1], PI[params[0]][ac_DRIVE_LICENSE_POINTS]);
	return 1;
}

CMD:requisar(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	if(sscanf(params, "u", params[0])) return SendNotification(playerid, "Error: /requisar [PlayerID/Nombre]");
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	
	if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "El jugador no está conectado.");
	new Float:pos[3]; GetPlayerPos(params[0], pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");
	if(PLAYER_TEMP[params[0]][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No puedes revisar a este jugador ahora.");
	if(GetPlayerState(params[0]) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "Para revisar a esta persona tiene que estar depie.");
	if(PI[params[0]][ac_WANTED_LEVEL] == 0) return SendNotification(playerid, "Esta persona no tiene nivel de búsqueda.");
	if(!PLAYER_TEMP[params[0]][pt_CUFFED]) return SendNotification(playerid, "Para revisar a esta persona tiene que estar esposada.");
	
	DeleteIlegalInv(params[0]);
	
	new action[64];
	format(action, sizeof action, "requisa las pertenencias ilegales de %s.", PLAYER_TEMP[params[0]][pt_RP_NAME]);
	Auto_SendPlayerAction(playerid, action);
	return 1;
}

CMD:ref(playerid, params[])
{
	if(PI[playerid][ac_CREW]) return Crew_RequestHelp(playerid, PI[playerid][ac_CREW]);

	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	
	new city[45], zone[45];
	GetPlayerZones(playerid, city, zone);
		
	new message[145];
	format(message, sizeof message, "{"#PRIMARY_COLOR"}[Central policía] {FFFFFF}%s %s necesita refuerzos en {"#PRIMARY_COLOR"}%s, %s.", POLICE_RANKS[ PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] ], PLAYER_TEMP[playerid][pt_RP_NAME], city, zone);
	SendPoliceRadioMessage(-1, -1, message);
	return 1;
}
alias:ref("refuerzos")

CMD:control(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] < 8) return SendFormatNotification(playerid, "Debes ser al menos rango ~r~%s~w~ para colocar objetos policiales.", POLICE_RANKS[8]);
	
	if(GetPlayerVirtualWorld(playerid) != 0 || GetPlayerInterior(playerid) != 0) return SendNotification(playerid, "No puedes colocar objetos policiales aquí.");
	
	new type;
	if(sscanf(params, "d", type)) return SendNotification(playerid, "~r~Modo de uso: ~w~/control [Tipo 1-5]");
	
	new modelid;
	switch(type)
	{
		case 1: modelid = 1459;
		case 2: modelid = 1238;
		case 3: modelid = 1425;
		case 4: modelid = 3265;
		case 5: modelid = 3091;
		default: return SendNotification(playerid, "Tipo de objeto no válido.");
	}
	
	new index = GetFreePoliceObjectSlot();
	if(index == -1) return SendNotification(playerid, "No se pueden crear más objetos policiales.");
	
	POLICE_OBJECTS[index][police_object_VALID] = true;
	format(POLICE_OBJECTS[index][police_object_USER], 24, "%s", ACCOUNT_INFO[playerid][ac_NAME]);
	
	new Float:pos[3], Float:angle;
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	GetPlayerFacingAngle(playerid, angle);
	pos[0] += (1.5 * floatsin(-angle, degrees));
	pos[1] += (1.5 * floatcos(-angle, degrees));

	POLICE_OBJECTS[index][police_object_OBJECT_ID] = CreateDynamicObject(modelid, pos[0], pos[1], pos[2], 0.0, 0.0, angle, GetPlayerVirtualWorld(playerid), 0);
	Streamer_Update(playerid);
	new info[3];
	info[1] = OBJECT_TYPE_POLICE;
	info[1] = index; // Index
	Streamer_SetArrayData(STREAMER_TYPE_OBJECT, POLICE_OBJECTS[index][police_object_OBJECT_ID], E_STREAMER_EXTRA_ID, info);
	PLAYER_TEMP[playerid][pt_SELECTED_POLICE_OBJECT_INDEX] = index;
	EditDynamicObject(playerid, POLICE_OBJECTS[index][police_object_OBJECT_ID]);
	
	SendNotification(playerid, "Coloca el objeto, posteriormente puedes usar /econtrol para moverlo o eliminarlo.");
	return 1;
}

CMD:econtrol(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] < 8) return SendFormatNotification(playerid, "Debes ser al menos rango ~r~%s~w~ para editar objetos policiales.", POLICE_RANKS[8]);
	
	if(GetPlayerVirtualWorld(playerid) != 0 || GetPlayerInterior(playerid) != 0) return SendNotification(playerid, "No puedes editar objetos policiales aquí.");
	
	SelectObject(playerid);
	return 1;
}

CMD:multar(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	if(sscanf(params, "ud", params[0], params[1])) return SendNotification(playerid, "~r~Modo de uso: ~w~/multar [PlayerID/Nombre] [precio$]");
	if(params[1] < 0) return SendNotification(playerid, "El precio no puede ser menor a 0$.");
	else if(params[1] > 100000) return SendNotification(playerid, "El precio no puede ser mayor a 100.000$.");
	
	if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "El jugador no está conectado.");
	new Float:pos[3]; GetPlayerPos(params[0], pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");
	if(PLAYER_TEMP[params[0]][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No puedes multar a este jugador ahora.");
	if(ACCOUNT_INFO[params[0]][ac_STATE] == ROLEPLAY_STATE_ARRESTED || ACCOUNT_INFO[params[0]][ac_STATE] == ROLEPLAY_STATE_JAIL) return SendNotification(playerid, "No puedes multar a este jugador ahora.");
	
	PLAYER_TEMP[params[0]][pt_POLICE_PEN_PID] = playerid;
	PLAYER_TEMP[params[0]][pt_POLICE_PEN_AID] = ACCOUNT_INFO[playerid][ac_ID];
	PLAYER_TEMP[params[0]][pt_POLICE_PEN_IM] = params[1];
	PLAYER_TEMP[params[0]][pt_POLICE_PEN_TIME] = gettime();
	ShowDialog(params[0], DIALOG_POLICE_PENALTY);
	
	SendNotification(playerid, "La multa ha sido enviada, espera para ver si el jugador la acepta.");
	return 1;
}

CMD:arrestar(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	
	if(sscanf(params, "ud", params[0], params[1])) return SendNotification(playerid, "Error: /arrestar [PlayerID/Nombre] [Asiento 1 ó 2]");
	if(params[1] < 1 || params[1] > 2) return SendNotification(playerid, "Error: /arrestar [PlayerID/Nombre] [Asiento 1 ó 2]");
	params[1] ++;
	
	if(!IsPlayerInAnyVehicle(playerid)) return SendNotification(playerid, "Para arrestar tienes que estar dentro de un vehículo policial.");
	
	new vehicleid = GetPlayerVehicleID(playerid);
	if(!WORK_VEHICLES[vehicleid][work_vehicle_VALID]) return SendNotification(playerid, "Para arrestar tienes que estar dentro de un vehículo policial.");
	if(WORK_VEHICLES[vehicleid][work_vehicle_WORK] != WORK_POLICE) return SendNotification(playerid, "Para arrestar tienes que estar dentro de un vehículo policial.");
	if( (params[1] + 1) > VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400 ][vehicle_info_SEATS]) return SendFormatNotification(playerid, "Este vehículo es de %d pasajeros, así que no puedes llevar arrestados en él.", VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400 ][vehicle_info_SEATS]);
	
	if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "El jugador no está conectado.");
	new Float:pos[3]; GetPlayerPos(params[0], pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");
	if(PLAYER_TEMP[params[0]][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No puedes arrestar a este jugador ahora.");
	
	if(PI[params[0]][ac_WANTED_LEVEL] == 0) return SendNotification(playerid, "Esta persona no tiene nivel de búsqueda.");
	if(!PLAYER_TEMP[params[0]][pt_CUFFED] && ACCOUNT_INFO[params[0]][ac_STATE] != ROLEPLAY_STATE_CRACK) return SendNotification(playerid, "Para arrestar a esta persona tiene que estar esposada.");
	
	if(ACCOUNT_INFO[params[0]][ac_STATE] == ROLEPLAY_STATE_ARRESTED)
	{
		TogglePlayerControllableEx(params[0], true);
		PLAYER_TEMP[params[0]][pt_CUFFED] = false;
		PLAYER_TEMP[params[0]][pt_CUFFING] = false;
		SetPlayerSpecialAction(params[0], SPECIAL_ACTION_NONE);
		ACCOUNT_INFO[params[0]][ac_STATE] = ROLEPLAY_STATE_NORMAL;
		
		SendFormatNotification(playerid, "Has soltado a ~r~%s~w~.", PLAYER_TEMP[params[0]][pt_RP_NAME]);
		return 1;
	}
	else
	{
		if(ACCOUNT_INFO[params[0]][ac_STATE] == ROLEPLAY_STATE_CRACK) {
			DisablePlayerInjuredMark(params[0]);
			ApplyAnimation(params[0], "CARRY", "crry_prtial", 4.1, 0, 0, 0, 0, 0, true);
			ClearAnimations(params[0]);
		}

		ACCOUNT_INFO[params[0]][ac_STATE] = ROLEPLAY_STATE_ARRESTED;
		TogglePlayerControllableEx(params[0], false);
		PutPlayerInVehicleEx(params[0], vehicleid, params[1]);
	}
	return 1;
}


CMD:callsing(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] < 11) return SendFormatNotification(playerid, "Debes ser al menos rango ~r~%s~w~ para asignar callsing.", POLICE_RANKS[11]);
	
	if(isnull(params)) return SendNotification(playerid, "Error: /callsing [Texto]");
	if(!IsPlayerInAnyVehicle(playerid)) return SendNotification(playerid, "Para asignar callsing tienes que estar dentro de un vehículo policial.");
	
	new vehicleid = GetPlayerVehicleID(playerid);
	if(!WORK_VEHICLES[vehicleid][work_vehicle_VALID]) return SendNotification(playerid, "Para asignar callsing tienes que estar dentro de un vehículo policial.");
	if(WORK_VEHICLES[vehicleid][work_vehicle_WORK] != WORK_POLICE) return SendNotification(playerid, "Para asignar callsing tienes que estar dentro de un vehículo policial.");
	
	if(IsValidDynamic3DTextLabel(GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL]))
	{
		DestroyDynamic3DTextLabel(GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL]);
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL] = Text3D:INVALID_STREAMER_ID;
	}
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_LABEL] = CreateDynamic3DTextLabel(params, 0xCCCCCCCC, -0.7, -1.9, -0.3, 20.0, .attachedvehicle = vehicleid);
	return 1;
}

CMD:m(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	
	if(!IsPlayerInAnyVehicle(playerid)) return SendNotification(playerid, "Para usar el megáfono tienes que estar dentro de un vehículo oficial.");
	
	new vehicleid = GetPlayerVehicleID(playerid);
	if(!WORK_VEHICLES[vehicleid][work_vehicle_VALID]) return SendNotification(playerid, "Para usar el megáfono tienes que estar dentro de un vehículo oficial.");
	if(WORK_VEHICLES[vehicleid][work_vehicle_WORK] != WORK_POLICE) return SendNotification(playerid, "Para usar el megáfono tienes que estar dentro de un vehículo oficial.");

	if(isnull(params)) return SendNotification(playerid, "Error: /m [Mensaje]");

	new str_text[190];
	format(str_text, 190, "(Megáfono) %s: {FFFFFF}%s", PLAYER_TEMP[playerid][pt_RP_NAME], params);
	ProxDetector(playerid, 40.0, str_text, 0xFF9B6AFF, 0xFF9B6AFF, 0xFF9B6AFF, 0xFF9B6AFF, 0xFF9B6AFF, 85);
	return 1;
}

PutPlayerInVehicleEx(playerid, vehicleid, seat)
{
	PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_PLAYER_VEHICLE_DOORS][p_ac_info_IMMUNITY] = gettime() + 5;
	PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_IMMUNITY] = gettime() + 15;
	return PutPlayerInVehicle(playerid, vehicleid, seat);
}

CMD:equiparse(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	
	if((IsPlayerInRangeOfPoint(playerid, 2.0, 219.244018, 69.982215, 1005.039062) || IsPlayerInRangeOfPoint(playerid, 2.0, 262.945587, 109.785270, 1004.617187) || IsPlayerInRangeOfPoint(playerid, 2.0, 222.683914, 186.956054, 1003.031250)) && PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
	{
		ShowDialog(playerid, DIALOG_POLICE_SHOP);
	}
	else SendNotification(playerid, "No estás en el lugar adecuado.");
	return 1;
}

CMD:entregar(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	
	if(sscanf(params, "u", params[0])) return SendNotification(playerid, "Error: /entregar [PlayerID/Nombre]");
	
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendNotification(playerid, "Para entregar tienes que estar dentro de un vehículo oficial.");
	
	new vehicleid = GetPlayerVehicleID(playerid);
	if(!WORK_VEHICLES[vehicleid][work_vehicle_VALID]) return SendNotification(playerid, "Para entregar tienes que estar dentro de un vehículo oficial.");
	if(WORK_VEHICLES[vehicleid][work_vehicle_WORK] != WORK_POLICE) return SendNotification(playerid, "Para entregar tienes que estar dentro de un vehículo oficial.");
	
	if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "El jugador no está conectado.");
	new Float:pos[3]; GetPlayerPos(params[0], pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");
	if(PLAYER_TEMP[params[0]][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No puedes arrestar a este jugador ahora.");
	if(GetPlayerState(params[0]) != PLAYER_STATE_PASSENGER) return SendNotification(playerid, "Para entregar a esta persona tiene que estar dentro del vehículo policial.");
	if(GetPlayerVehicleID(params[0]) != vehicleid) return SendNotification(playerid, "Para entregar a esta persona tiene que estar dentro del vehículo policial.");
	if(PI[params[0]][ac_WANTED_LEVEL] == 0) return SendNotification(playerid, "Esta persona no tiene nivel de búsqueda.");
	
	if(IsPlayerInRangeOfPoint(playerid, 20.0, 1564.971923, -1694.916381, 5.617697) || IsPlayerInRangeOfPoint(playerid, 20.0, 6808.6948, 5335.9800 ,14.9625))
	{
		SendNotification(playerid, "La persona ahora está en la cárcel.");
		PI[params[0]][ac_POLICE_JAIL_ID] = 0;
		JailPlayer(params[0]);
	}
	else if(IsPlayerInRangeOfPoint(playerid, 20.0, -1589.333496, 716.759521, -5.515106) || IsPlayerInRangeOfPoint(playerid, 20.0, 6005.6670, 4917.3179, 23.0543))
	{
		SendNotification(playerid, "La persona ahora está en la cárcel.");
		PI[params[0]][ac_POLICE_JAIL_ID] = 1;
		JailPlayer(params[0]);
	}
	else if(IsPlayerInRangeOfPoint(playerid, 20.0, 2282.200439, 2431.598632, 3.000518) || IsPlayerInRangeOfPoint(playerid, 20.0, 4405.0625, 5969.0493, 59.0018))
	{
		SendNotification(playerid, "La persona ahora está en la cárcel.");
		PI[params[0]][ac_POLICE_JAIL_ID] = 2;
		JailPlayer(params[0]);
	}
	else SendNotification(playerid, "No estás en el lugar adecuado.");
	return 1;
}

CMD:policias(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	
	PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] = 10;
	PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
	ShowDialog(playerid, DIALOG_POLICE_LIST);
	return 1;
}

CMD:entregarse(playerid, params[])
{
	if(PI[playerid][ac_WANTED_LEVEL] <= 0) return SendNotification(playerid, "No tienes cargos.");
	
	if(IsPlayerInRangeOfPoint(playerid, 1.5, 250.049026, 67.635704, 1003.640625))
	{
		PI[playerid][ac_POLICE_JAIL_ID] = 0;
		JailPlayer(playerid);
	}
	else if(IsPlayerInRangeOfPoint(playerid, 1.5, 246.349716, 118.358116, 1003.218750))
	{
		PI[playerid][ac_POLICE_JAIL_ID] = 1;
		JailPlayer(playerid);
	}
	else if(IsPlayerInRangeOfPoint(playerid, 1.5, 235.772537, 165.382186, 1003.029968))
	{
		PI[playerid][ac_POLICE_JAIL_ID] = 2;
		JailPlayer(playerid);
	}
	else SendNotification(playerid, "No estás en el lugar adecuado.");
	return 1;
}

CMD:resistirse(playerid, params[])
{
	if(PLAYER_TEMP[playerid][pt_CUFFING])
	{
		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][14]);
		PLAYER_TEMP[playerid][pt_CUFFING] = false;
		TogglePlayerControllableEx(playerid, true);
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		Auto_SendPlayerAction(playerid, "se resiste.");
	}
	return 1;
}

forward CuffPlayer(playerid);
public CuffPlayer(playerid)
{
	StopAudioStreamForPlayer(playerid);
	SetPlayerDrunkLevel(playerid, 0);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	CancelEdit(playerid);
	HidePlayerDialog(playerid);
	CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
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
	
	PLAYER_TEMP[playerid][pt_CUFFED] = true;
	PLAYER_TEMP[playerid][pt_CUFFING] = false;
	TogglePlayerControllableEx(playerid, true);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CUFFED);
	return 1;
}

CMD:frecuencias(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	
	ShowDialog(playerid, DIALOG_POLICE_RADIOS);
	return 1;
}

SendPoliceRadioMessage(radio, color, const message[])
{
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i) && PLAYER_TEMP[i][pt_USER_LOGGED])
		{
			if(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL)
			{
				if(PLAYER_WORKS[i][WORK_POLICE][pwork_SET])
				{
					if(PLAYER_TEMP[i][pt_WORKING_IN] == WORK_POLICE)
					{
						if(radio == -1) SendClientMessage(i, color, message);
						else
						{
							if(PLAYER_TEMP[i][pt_POLICE_RADIO] == radio)
							{
								SendClientMessage(i, color, message);
							}
						}
					}
				}
			}
		}
	}
	return 1;
}



// PROPERTY CONSTRUCTOR 

CMD:rproperty(playerid, params[])
{
	new id_property;
	if(sscanf(params, "d", id_property)) return SendNotification(playerid, "~r~Modo de uso:~w~ /rproperty <id>");
	if(id_property <= 0) return SendNotification(playerid, "~r~Modo de uso:~w~ /rproperty < id > 0 >");
	
	new index = GetPropertyIndexByID(id_property);
	if(index == -1) return SendNotification(playerid, "PROPIEDAD ID no encontrada.");
	
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM properties WHERE id = %d;", PROPERTY_INFO[index][property_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	
	if(IsValidDynamic3DTextLabel(PROPERTY_INFO[index][property_EXT_LABEL_ID])) DestroyDynamic3DTextLabel(PROPERTY_INFO[index][property_EXT_LABEL_ID]);
	if(IsValidDynamic3DTextLabel(PROPERTY_INFO[index][property_INT_LABEL_ID])) DestroyDynamic3DTextLabel(PROPERTY_INFO[index][property_INT_LABEL_ID]);
	if(IsValidDynamicPickup(PROPERTY_INFO[index][property_EXT_PICKUP_ID])) DestroyDynamicPickup(PROPERTY_INFO[index][property_EXT_PICKUP_ID]);
	if(IsValidDynamicPickup(PROPERTY_INFO[index][property_INT_PICKUP_ID])) DestroyDynamicPickup(PROPERTY_INFO[index][property_INT_PICKUP_ID]);
	new reset[Property_Info_Enum]; PROPERTY_INFO[index] = reset;
	
	TOTAL_PROPERTIES_LOADED --;
	
	SendFormatNotification(playerid, "PROPIEDAD %d ELIMINADA.", id_property);
	SendFormatNotification(playerid, "PROPIEDADES: %d/%d", TOTAL_PROPERTIES_LOADED, MAX_PROPERTIES);
	
	SendCmdLogToAdmins(playerid, "rproperty", params);
	return 1;
}

CMD:eproperty(playerid, params[])
{
	new id_property, new_int;
	if(sscanf(params, "dddddd", id_property, new_int, PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][0], PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][1], PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][2], PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][3])) return SendNotification(playerid, "~r~Modo de uso:~w~ /eproperty <id> <interior> <nivel> <precio> <vip level> <coins>");
	if(id_property <= 0) return SendNotification(playerid, "ID no válida.");
	
	new index = GetPropertyIndexByID(id_property);
	if(index == -1) return SendNotification(playerid, "PROPIEDAD ID no encontrada.");
	
	if(PROPERTY_INFO[index][property_SOLD]) return SendNotification(playerid, "Solo se pueden modificar propiedades que están libres.");
	
	if(new_int < 0 || new_int >= sizeof(PROPERTY_INTERIORS)) return SendNotification(playerid, "El interior elegido no es válido.");
	
	PROPERTY_INFO[index][property_ID_INTERIOR] = new_int;
	PROPERTY_INFO[index][property_PRICE] = PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][1];
	PROPERTY_INFO[index][property_LEVEL] = PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][0];
	PROPERTY_INFO[index][property_EXTRA] = PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][3];
	PROPERTY_INFO[index][property_VIP_LEVEL] = PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][2];
	PROPERTY_INFO[index][property_PRICE_BASE] = floatround(floatdiv(PROPERTY_INFO[index][property_PRICE], MULTIPLIER_PROPERITES));
	
	if(PROPERTY_INFO[index][property_EXTRA]) PROPERTY_INFO[index][property_PRICE] = 0;
	if(PROPERTY_INFO[index][property_VIP_LEVEL]) PROPERTY_INFO[index][property_LEVEL] = 1;
	
	UpdateUnnocupiedPropertyLabel(index);
	
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE properties SET id_interior = %d, price = %d, level = %d, extra = %d, vip_level = %d WHERE id = %d;", PROPERTY_INFO[index][property_ID_INTERIOR], PROPERTY_INFO[index][property_PRICE_BASE], PROPERTY_INFO[index][property_LEVEL], PROPERTY_INFO[index][property_EXTRA], PROPERTY_INFO[index][property_VIP_LEVEL], PROPERTY_INFO[index][property_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);

	SendCmdLogToAdmins(playerid, "eproperty", params);
	return 1;
}

CMD:cproperty(playerid, params[])
{
	if(TOTAL_PROPERTIES_LOADED >= MAX_PROPERTIES) return SendNotification(playerid, "Límite alcanzado.");
	
	if(sscanf(params, "dddd", PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][0], PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][1], PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][2], PLAYER_TEMP[playerid][pt_PROPERTY_CINFO][3])) return SendNotification(playerid, "~r~Modo de uso:~w~ /cproperty <nivel> <precio> <vip level> <coins>");
	
	GetPlayerPos(playerid, PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_X], PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_Y], PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_Z]);
	GetPlayerFacingAngle(playerid, PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_ANG]);
	PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_EXT_INT] = GetPlayerInterior(playerid);
	
	PLAYER_PROPERTY_CONSTRUCTOR[playerid][player_property_creator_ENABLED] = true;
	for(new i = 0; i != 6; i ++) PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_PROPERTY_CONSTRUCTOR][i]);
	
	UpdatePlayerPropertyContructor(playerid);
	return 1;
}

CMD:curar(playerid, params[])
{
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK) return SendNotification(playerid, "No puedes curar estando herido.");
	if(PI[playerid][ac_CREW] && CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_FIGHTING]) return SendNotification(playerid, "No puedes curar mientras tu banda está en combate.");

	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso: ~w~/curar [PlayerID/Nombre]");
	if(!IsPlayerConnected(to_player)) return SendNotification(playerid, "El jugador no está conectado.");
	if(to_player == playerid) return SendNotification(playerid, "~r~Modo de uso: ~w~/curar [PlayerID/Nombre]");
	
	new Float:pos[3];
	GetPlayerPos(to_player, pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, NEARS_PLAYERS_DISTANCE, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");

	if(ACCOUNT_INFO[to_player][ac_STATE] != ROLEPLAY_STATE_CRACK) return SendNotification(playerid, "Esta persona no está herida.");
	if(PI[to_player][ac_CREW] && CREW_INFO[ PLAYER_TEMP[to_player][pt_CREW_INDEX] ][crew_FIGHTING]) return SendNotification(playerid, "No puedes curar a esta persona porque su banda está en combate.");

	if(!PLAYER_WORKS[playerid][WORK_MEDIC][pwork_SET] && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_MEDIC) {
		if(PI[playerid][ac_MEDICAL_KITS] <= 0) return SendNotification(playerid, "No tienes botiquines para curar a esta persona.");
		else {
			new now = gettime();
			if(PLAYER_TEMP[playerid][pt_COOLDOWN_MEDICAL_KIT] > now) return SendFormatNotification(playerid, "Debes esperar %d segundos para volver a poder curar.", PLAYER_TEMP[playerid][pt_COOLDOWN_MEDICAL_KIT] - now);
			PLAYER_TEMP[playerid][pt_COOLDOWN_MEDICAL_KIT] = now + 300;

			PI[playerid][ac_MEDICAL_KITS] --;
			SendNotification(playerid, "Has usado 1 botiquín para curar a esta persona.");
		}
	}
	
	KillTimer(PLAYER_TEMP[to_player][pt_TIMERS][16]);
	PLAYER_TEMP[to_player][pt_TIMERS][16] = SetTimerEx("StandUp", 5000, false, "ii", playerid, to_player);
	DisablePlayerInjuredMark(to_player);
	
	ApplyAnimation(playerid, "MEDIC", "CPR", 4.1, false, 0, 0, 0, 0, 1);
	return 1;
}

CMD:banda(playerid, params[])
{
	if(!PI[playerid][ac_CREW])
	{
		if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "Los policías no pueden tener bandas.");
		
		new index = GetAvaibleCrewIndex();
		if(index == -1)
		{
			SendFormatNotification(playerid, "No perteneces a ninguna banda y no se puede crear una porque no hay espacio disponible, el límite de bandas es %d.", MAX_CREWS);
			return 1;
		}
		
		ShowDialog(playerid, DIALOG_CREATE_CREW);
		return 1;
	}
	
	ShowDialog(playerid, DIALOG_CREW_MENU);
	return 1;
}

CMD:abandonar(playerid, params[])
{
	if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
	if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_LEAVE_TERRITORY]) return SendNotification(playerid, "No tienes permiso para abandonar territorios.");
	if(PLAYER_TEMP[playerid][pt_LAST_TERRITORY] == INVALID_STREAMER_ID) return SendNotification(playerid, "No estás en un territorio de tu banda.");
	if(!IsPlayerInDynamicArea(playerid, TERRITORIES[ PLAYER_TEMP[playerid][pt_LAST_TERRITORY] ][territory_AREA])) return SendNotification(playerid, "No estás en un territorio de tu banda.");
	if(TERRITORIES[ PLAYER_TEMP[playerid][pt_LAST_TERRITORY] ][territory_CREW_ID] != PI[playerid][ac_CREW]) return SendNotification(playerid, "No estás en un territorio de tu banda.");			
	if(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_FIGHTING]) return SendNotification(playerid, "No puedes abandonar territorios mientras tu banda está en combate.");
	
	PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] = PLAYER_TEMP[playerid][pt_LAST_TERRITORY];
	ShowDialog(playerid, DIALOG_CREW_LEAVE_TERRITORY);
	return 1;
}

CMD:invitar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	
	if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No eres miembro de ninguna banda.");
	if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_INVITE]) return SendNotification(playerid, "No tienes permiso para invitar personas a la banda.");
	if(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_FIGHTING]) return SendNotification(playerid, "No puedes invitar a gente a la banda cuando la banda está en combate.");
	
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso: ~w~/invitar [PlayerID/Nombre]");
	if(!IsPlayerConnected(to_player)) return SendNotification(playerid, "El jugador está desconectado.");
	if(to_player == playerid) return SendNotification(playerid, "Eres tú.");
	
	new members = CountCrewPlayers(PI[playerid][ac_CREW]);
	if(members >= MAX_CREW_MEMBERS) {
		return SendFormatNotification(playerid, "Actualmente la banda cuenta con %d miembros, el límite es de %d miembros.", members, MAX_CREW_MEMBERS);
	}
	
	new Float:pos[3]; GetPlayerPos(to_player, pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");
	if(PLAYER_WORKS[to_player][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "Esta persona es policía y no puede tener banda.");
	if(PI[to_player][ac_CREW]) return SendNotification(playerid, "Esta persona pertenece a otra banda.");
	if(PLAYER_TEMP[to_player][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No puedes invitar a este jugador ahora.");
	
	if(gettime() < PLAYER_TEMP[ to_player ][pt_LAST_GOT_CREW] + 600)
	{
		new time = 600 - (gettime() - PLAYER_TEMP[to_player][pt_LAST_GOT_CREW]);
		SendFormatNotification(playerid, "La otra persona debe esperar %s minutos para volver a ser invitada.", TimeConvert(time));
		return 1;
	}
	
	PLAYER_TEMP[playerid][pt_CREW_INVITE_PID] = to_player;
	PLAYER_TEMP[playerid][pt_CREW_INVITE_AID] = ACCOUNT_INFO[to_player][ac_ID];
	SendFormatNotification(playerid, "Quieres invitar a %s a la banda, elige el rango que le ofreces.", PLAYER_TEMP[to_player][pt_RP_NAME]);
	ShowDialog(playerid, DIALOG_CREW_INVITE_RANK);
	return 1;
}

CMD:reclutar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "~r~Ahora no puedes usar este comando.");
	
	if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET])
	{
		if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] < 11) return SendFormatNotification(playerid, "Debes ser al menos rango ~r~%s~w~ para reclutar gente.", POLICE_RANKS[11]);
		
		new to_player;
		if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /reclutar [PlayerID/Nombre]");
		if(!IsPlayerConnected(to_player)) return SendNotification(playerid, "~r~El jugador está desconectado.");
		if(to_player == playerid) return SendNotification(playerid, "~r~No puedes invitarte.");
		
		new Float:pos[3]; GetPlayerPos(to_player, pos[0], pos[1], pos[2]);
		if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "~r~Este jugador no está cerca tuya.");
		if(PI[to_player][ac_CREW]) return SendClientMessage(playerid,-1, "{"#RED_COLOR"}Esta persona pertenece a una banda y no puede ser policía.");
		if(PLAYER_WORKS[to_player][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "~r~Éste usuario es miembro.");
		if(PLAYER_TEMP[to_player][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "~r~No puedes reclutar a este jugador ahora.");
		
		new player_jobs = CountPlayerJobs(to_player);
		if(ACCOUNT_INFO[to_player][ac_VIP])
		{
			if(player_jobs >= MAX_SU_WORKS)
			{
				SendNotification(playerid, "~r~Esta persona ya tiene trabajo y no puede tener más.");
				return 1;
			}
		}
		else
		{
			if(player_jobs >= MAX_NU_WORKS)
			{
				SendNotification(playerid, "~r~Esta persona ya tiene trabajo y no puede tener más.");
				return 1;
			}
		}
		
		if(PLAYER_TEMP[to_player][pt_WORKING_IN]) return SendNotification(playerid, "~r~Esta persona no puede unirse porque esta de servicio en su trabajo.");
		
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO pworks (id_player, id_work, `set`, level) VALUES(%d, %d, 1, 1) ON DUPLICATE KEY UPDATE `set` = 1, level = 1;", ACCOUNT_INFO[to_player][ac_ID], WORK_POLICE);
		mysql_tquery(srp_db, QUERY_BUFFER);

		PLAYER_WORKS[to_player][WORK_POLICE][pwork_SET] = true;
		PLAYER_WORKS[to_player][WORK_POLICE][pwork_LEVEL] = 1;
		
		PI[to_player][ac_PLACA_PD] = random(10000000);
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET placa_pd = %d WHERE id = %d;", PI[to_player][ac_PLACA_PD], ACCOUNT_INFO[to_player][ac_ID]);
		mysql_tquery(srp_db, QUERY_BUFFER);
		
		SendFormatNotification(playerid, "%s ahora es policía.", PLAYER_TEMP[to_player][pt_RP_NAME]);
		SendClientMessage(to_player, -1, "Ahora eres policía.");
	}
	else SendNotification(playerid, "~r~Debes ser miembro de la policía para usar este comando.");
	return 1;
}


CMD:eco(playerid, params[])
{
	ShowDialog(playerid, DIALOG_SERVER_ECO);
	return 1;
}

CMD:vip(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_VIP]) ShowDialog(playerid, DIALOG_SU);
	else ShowDialog(playerid, DIALOG_SU_BUY);
	return 1;
}

CMD:setcoins(playerid, params[])
{
	new to_player, sd;
	if(sscanf(params, "ud", to_player, sd)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setcoins <player_id> <sd>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	
	PI[to_player][ac_COINS] = sd;
	SendFormatNotification(to_player, "Ahora tienes ~g~%d "SERVER_COIN"~w~.", sd);
	PlayerPlaySoundEx(to_player, 1058, 0.0, 0.0, 0.0);
	
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET coins = %d WHERE id = %d;", PI[to_player][ac_COINS], ACCOUNT_INFO[to_player][ac_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	
	SendCmdLogToAdmins(playerid, "setcoins", params);
	return 1;
}

CMD:givecoins(playerid, params[])
{
	new to_player, sd;
	if(sscanf(params, "ud", to_player, sd)) return SendNotification(playerid, "~r~Modo de uso:~w~ /givecoins <player_id> <sd>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	
	PI[to_player][ac_COINS] += sd;
	SendClientMessageEx(to_player, -1, "{"#SILVER_COLOR"}Has comprado %d "SERVER_COIN".", sd);
	PlayerPlaySoundEx(to_player, 1058, 0.0, 0.0, 0.0);
	
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET coins = %d WHERE id = %d;", PI[to_player][ac_COINS], ACCOUNT_INFO[to_player][ac_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	
	SendCmdLogToAdmins(playerid, "givecoins", params);
	return 1;
}

CMD:setvip(playerid, params[])
{
	new to_player, vip_level, days;
	if(sscanf(params, "udd", to_player, vip_level, days)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setvip <player_id> <vip_level> <dias>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	
	SetPlayerVip(to_player, vip_level, 0, days);
	
	SendCmdLogToAdmins(playerid, "setvip", params);
	return 1;
}

CheckPlayerSuperUser(playerid)
{
	if(ACCOUNT_INFO[playerid][ac_VIP])
	{
		inline OnInfoQueryLoad()
		{
			new rows;
			if(cache_get_row_count(rows))
			{
				if(rows)
				{
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET vip = 0, vip_expire_date = '0000-00-00 00:00:00' WHERE id = %d;", ACCOUNT_INFO[playerid][ac_ID]);
					mysql_tquery(srp_db, QUERY_BUFFER);

					ACCOUNT_INFO[playerid][ac_VIP] = false;
					ACCOUNT_INFO[playerid][ac_VIP_EXPIRE_DATE][0] = EOS;
					SendNotification(playerid, "¡Tu VIP ha expirado! Usa /vip para comprar de nuevo.");
					if(GetPlayerSkin(playerid) == PI[playerid][ac_SKIN]) SetPlayerToys(playerid);
					ReLockPlayerVehicles(playerid, true);
				}
				PLAYER_TEMP[playerid][pt_LAST_SU_CHECK] = gettime();
			}
		}
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM player WHERE id = %d AND NOW() >= vip_expire_date;", ACCOUNT_INFO[playerid][ac_ID]);
		MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	}
	return 1;
}

CMD:darskin(playerid, params[])
{
    new to_player, skin;
    if(sscanf(params, "ud", to_player, skin)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setskin <player_id> <skin>");
    if(!IsPlayerConnected(to_player)) return SendNotification(playerid, "Jugador desconectado");

	PI[to_player][ac_SKIN] = skin;
    SetPlayerSkin(to_player, PI[to_player][ac_SKIN]);
	
	SendFormatNotification(playerid, "Skin '%s (%d)' cambiado a '%d'.", ACCOUNT_INFO[to_player][ac_NAME], to_player, skin);
    return 1;
}
alias:darskin("setskin")

CMD:setfstyle(playerid, params[])
{
    new to_player, style;
    if(sscanf(params, "ud", to_player, style)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setfstyle <player_id> <style>");
    if(!IsPlayerConnected(to_player)) return SendNotification(playerid, "Jugador desconectado");

	PI[to_player][ac_FIGHT_STYLE] = style;
    SetPlayerFightingStyle(to_player, PI[to_player][ac_FIGHT_STYLE]);
	
	SendFormatNotification(playerid, "FStyle '%s (%d)' cambiado a '%d'.", ACCOUNT_INFO[to_player][ac_NAME], to_player, style);
    return 1;
}

CMD:ls(playerid, params[])
{
    new to_player;
    if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /sendls <player_id>");
    if(!IsPlayerConnected(to_player)) return SendNotification(playerid, "Jugador desconectado");

	ACCOUNT_INFO[to_player][ac_STATE] = ROLEPLAY_STATE_NORMAL;
	PI[to_player][ac_LOCAL_INTERIOR] = 0;
	PLAYER_TEMP[to_player][pt_INTERIOR_INDEX] = -1;
	PI[to_player][ac_INTERIOR] = 0;
	PI[to_player][ac_POLICE_JAIL_TIME] = 0;
    SetPlayerPosEx(to_player, 1555.400390, -1675.611694, 16.195312, 0.0, 0, 0, true);
    SetPlayerCityWeather(to_player);
	SendFormatNotification(playerid, "Jugador '%s (%d)' fue llevado a LS.", ACCOUNT_INFO[to_player][ac_NAME], to_player);
    return 1;
}

CMD:lsdb(playerid, params[])
{
	new to_account;
	if(sscanf(params, "d", to_account)) return SendNotification(playerid, "~r~Modo de uso:~w~ /osendls <DB-ID>");
	
	inline OnInfoQueryLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			if(rows)
			{
				new id, ip[16], name[24], connected, pid, admin_level;
				cache_get_value_name_int(0, "id", id);
				cache_get_value_name(0, "ip", ip);
				cache_get_value_name(0, "name", name);
				cache_get_value_name_int(0, "connected", connected);
				cache_get_value_name_int(0, "playerid", pid);
				cache_get_value_name_int(0, "admin_level", admin_level);

				if(connected) SendFormatNotification(playerid, "JUGADOR '%s' DB-ID '%d' conectado utilice /ls, su player_id: %d.", name, id, pid);
				else
				{
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET pos_x = 1555.400390, pos_y = -1675.611694, pos_z = 16.195312, angle = 0, state = %d, interior = 0, local_interior = 0, police_jail_time = 0 WHERE id = %d;", ROLEPLAY_STATE_NORMAL, to_account);
					mysql_tquery(srp_db, QUERY_BUFFER);
					SendFormatNotification(playerid, "Jugador '%s' DB-ID '%d' ahora aparecerá en LS.", name, id);
				}
			}
			else SendNotification(playerid, "No se encontro la DB-ID.");
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, ip, name, connected, playerid, admin_level FROM player WHERE id = %d;", to_account);
	MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	return 1;
}

CMD:vpcar(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] < 5) return 0;
	
	new to_player, modelid;
	if(sscanf(params, "ud", to_player, modelid)) return SendNotification(playerid, "~r~Modo de uso:~w~ /vpcar <playerid> <modelid>");
	if(!IsPlayerConnected(to_player)) return SendNotification(playerid, "Jugador desconectado.");
	if(modelid < 400 || modelid > 611) return SendNotification(playerid, "Modelo de vehículo no válido.");
	
	new pvehicles = CountPlayerVehicles(to_player);
	if(pvehicles >= MAX_SU_VEHICLES) return SendFormatNotification(playerid, "El jugador no puede tener más vehículos, su límite es %d.", MAX_SU_VEHICLES);
	if(!ACCOUNT_INFO[to_player][ac_VIP])
	{
		if(pvehicles >= MAX_NU_VEHICLES)
		{
			SendFormatNotification(playerid, "El jugador no puede tener más vehículos, su límite es %d.", MAX_NU_VEHICLES);
			return 1;
		}
	}
	
	new Float:pos[4];
	GetPlayerPos(to_player, pos[0], pos[1], pos[2]);
	GetPlayerFacingAngle(to_player, pos[3]);
	
	new vid = AddPersonalVehicle
	(
		to_player,
		modelid,
		pos[0],
		pos[1],
		pos[2],
		pos[3],
		1,
		1,
		VEHICLE_INFO[ modelid - 400 ][vehicle_info_MAX_GAS]
	);
	if(!vid) return SendNotification(playerid, "No se pueden añadir más vehículos.");
	
	SendCmdLogToAdmins(playerid, "vpcar", params);
	return 1;
}

CMD:revivir(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /revivir <playerid>");
	
	if(ACCOUNT_INFO[to_player][ac_STATE] != ROLEPLAY_STATE_CRACK) return SendNotification(playerid, "Esta persona no está herida.");
	
	DisablePlayerInjuredMark(to_player);
	ACCOUNT_INFO[to_player][ac_STATE] = ROLEPLAY_STATE_NORMAL;
	SetWeaponsForPlayer(to_player);
	if(ACCOUNT_INFO[to_player][ac_VIP]) SetPlayerHealthEx(playerid, 50.0);
	else SetPlayerHealthEx(to_player, 25.0);
	ApplyAnimation(to_player, "CARRY", "crry_prtial", 4.1, 0, 0, 0, 0, 0, true);
	ClearAnimations(to_player);
	
	SendCmdLogToAdmins(playerid, "revivir", params);
	return 1;
}
alias:revivir("revive")

CMD:tuning(playerid, params[])
{	
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendNotification(playerid, "No estás en nigún vehículo.");
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_JAIL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_ARRESTED) return SendNotification(playerid, "Ahora no puedes usar este comando.");
	if(GetPlayerInterior(playerid) != 0 || GetPlayerVirtualWorld(playerid) != 0) return SendNotification(playerid, "No estás en el lugar adecuado.");
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, 263.592102, 22.960014, 2.170951)) return SendNotification(playerid, "No estás en el lugar adecuado.");
	
	new vehicleid = GetPlayerVehicleID(playerid);
	if(!PLAYER_VEHICLES[vehicleid][player_vehicle_VALID]) return SendNotification(playerid, "Solo puedes tunear tus vehículos personales.");
	if(PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return SendNotification(playerid, "Solo puedes tunear tus vehículos personales.");
	if(!VEHICLE_INFO[GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_NORMAL_SPEEDO]) return SendNotification(playerid, "Este vehículo no se puede tunear.");
	if(ACCOUNT_INFO[playerid][ac_LEVEL] < 2) return SendNotification(playerid, "Debes de ser al menos nivel 2 para tunear vehículos aquí.");
	
	PLAYER_TEMP[playerid][pt_IN_TUNING_GARAGE] = true;
	PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] = vehicleid;
	
	SetVehiclePosEx(vehicleid, 263.592102, 22.960014, 2.170951);
	SetVehicleZAngle(vehicleid, 0.0);
	SetVehicleVelocity(vehicleid, 0.0, 0.0, 0.0);
	SetVehicleVirtualWorldEx(vehicleid, playerid + MAX_PLAYERS);
	SetPlayerVirtualWorld(playerid, playerid + MAX_PLAYERS);
	
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
	UpdateVehicleParams(vehicleid);
	HidePlayerSpeedoMeter(playerid);
	SetCameraBehindPlayer(playerid);

	ShowDialog(playerid, DIALOG_VOBJECT_MENU);
	return 1;
}

CMD:muteard(playerid, params[])
{
    new to_player, reason[128], time;
    if(sscanf(params, "uds[128]", to_player, time, reason)) return SendNotification(playerid, "~r~Modo de uso:~w~ /muteard <player_id> <minutos> <razón>");
	if(time < 0 || time > 1440) return SendNotification(playerid, "Intervalo de minutos incorrecto.");
    if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
    if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	if(gettime() < PI[to_player][ac_MUTE]) return SendNotification(playerid, "El jugador ya está muteado.");

	new seconds = time * 60;
	PI[to_player][ac_MUTE] = gettime() + seconds;
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET mute = %d WHERE id = %d;", PI[to_player][ac_MUTE], ACCOUNT_INFO[to_player][ac_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	SendClientMessageEx(to_player, -1, "Has sido silenciado del canal de dudas por %d minutos, razón: %s", time, reason);
	
	new str[145]; format(str, 145, "{"#RED_COLOR"}[ADMIN] {FFFFFF}%s (%d) silenció a %s (%d) del canal de dudas: %s.", ACCOUNT_INFO[playerid][ac_NAME], playerid, ACCOUNT_INFO[to_player][ac_NAME], to_player, reason);
    SendAdminAd(-1, str);
	return 1;
}

CMD:desmuteard(playerid, params[])
{
    new to_player;
    if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /desmuteard <player_id>");
    if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
    if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");

	if(gettime() > PI[to_player][ac_MUTE]) return SendNotification(playerid, "Este jugador no está silenciado.");
	
	PI[to_player][ac_MUTE] = 0;
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET mute = %d WHERE id = %d;", PI[to_player][ac_MUTE], ACCOUNT_INFO[to_player][ac_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	SendClientMessage(to_player, -1, "Ya puedes volver a enviar dudas.");
	
	SendFormatNotification(playerid, "Jugador %s (%d) ha sido des-silenciado.", ACCOUNT_INFO[to_player][ac_NAME], to_player);
	return 1;
}
CMD:borrarop(playerid, params[])
{
	new affected_objects;
	for(new i = 0; i != MAX_POLICE_OBJECTS; i ++)
	{
		if(!POLICE_OBJECTS[i][police_object_VALID]) continue;
		
		DestroyDynamicObject(POLICE_OBJECTS[i][police_object_OBJECT_ID]);
		POLICE_OBJECTS[i][police_object_VALID] = false;
		POLICE_OBJECTS[i][police_object_USER][0] = EOS;
		POLICE_OBJECTS[i][police_object_OBJECT_ID] = INVALID_STREAMER_ID;
		affected_objects ++;
	}
	
	SendFormatNotification(playerid, "Se han eliminado todos los objetos policiales, %d objetos afectados.", affected_objects);
	SendCmdLogToAdmins(playerid, "borrarop", params);
	return 1;
}

CMD:abyc(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	
	new to_player, reason[128];
	if(sscanf(params, "us[128]", to_player, reason)) return SendNotification(playerid, "Error: /abyc [Playerid o nombre] [razon]");
	if(!IsPlayerConnected(to_player)) return SendNotification(playerid, "Jugador desconectado.");
	if(to_player == playerid) return 1;
	
	if(PLAYER_TEMP[to_player][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No se puede añadir byc a este jugador ahora.");
	if(PLAYER_WORKS[to_player][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No puedes añadir byc a este jugador porque es policía.");
	
	AddPlayerPoliceHistory(to_player, ACCOUNT_INFO[playerid][ac_ID], reason);
	SendFormatNotification(playerid, "Se ha añadido el informe al historial policial de %s.", PLAYER_TEMP[to_player][pt_RP_NAME]);
	return 1;
}

CMD:dbyc(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] < 11) return SendFormatNotification(playerid, "~r~Debes ser al menos rango %s para colocar objetos policiales.", POLICE_RANKS[11]);
	
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "Error: /dbyc [Playerid o nombre]");
	if(!IsPlayerConnected(to_player)) return SendNotification(playerid, "Jugador desconectado.");
	if(PLAYER_TEMP[to_player][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No se puede eliminar el historial policial de este jugador ahora.");
	
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM police_history WHERE id_player = %d;", ACCOUNT_INFO[to_player][ac_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	
	SendFormatNotification(playerid, "Has borrado el historial policial de %s.", PLAYER_TEMP[to_player][pt_RP_NAME]);
	return 1;
}


CMD:byc(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) return SendNotification(playerid, "No estás de servicio como policía.");
	
	ShowDialog(playerid, DIALOG_POLICE_BYC);
	return 1;
}

CMD:admac(playerid, params[])
{
	if(sscanf(params, "d", params[0])) return SendNotification(playerid, "~r~Modo de uso:~w~ /admac <nivel>");
	ADMIN_LEVEL_AC_IMMUNITY = params[0];
	
	SendFormatNotification(playerid, "Admin level %d o mayor no sera detectado por ac.", ADMIN_LEVEL_AC_IMMUNITY);
	return 1;
}

CMD:cleancrews(playerid, params[])
{
	SendNotification(playerid, "deshabilitado");
	return 1;
}

CMD:cleanproperties(playerid, params[])
{
	new total;
	for(new i = 0; i != MAX_PROPERTIES; i ++)
	{
		if(PROPERTY_INFO[i][property_SOLD])
		{
			PROPERTY_INFO[i][property_SOLD] = false;
			PROPERTY_INFO[i][property_CREW] = false;
			PROPERTY_INFO[i][property_OWNER_ID] = 0;
			PROPERTY_INFO[i][property_CREW_ID] = 0;
			format(PROPERTY_INFO[i][property_NAME], 24, "PROPIEDAD %d", PROPERTY_INFO[i][property_ID]);
			
			new label_str[256], info[3];
			format
			(
				label_str, 
				sizeof label_str, 
				"\
					Propiedad {"#PRIMARY_COLOR"}#%d {FFFFFF}está en venta\n\n\
					Interior: {"#PRIMARY_COLOR"}%d\n\
					{FFFFFF}Precio: {"#PRIMARY_COLOR"}%s$\n\
					{FFFFFF}Nivel necesario: {"#PRIMARY_COLOR"}%d\n\
					{FFFFFF}Usa {"#PRIMARY_COLOR"}/ayuda {FFFFFF}para ver como adquirirla.\
				", PROPERTY_INFO[i][property_ID], PROPERTY_INFO[i][property_ID_INTERIOR], number_format_thousand(PROPERTY_INFO[i][property_PRICE]), PROPERTY_INFO[i][property_LEVEL]
			);
			UpdateDynamic3DTextLabelText(PROPERTY_INFO[i][property_EXT_LABEL_ID], 0xFFFFFFFF, label_str);
			
			DestroyDynamicPickup(PROPERTY_INFO[i][property_EXT_PICKUP_ID]);
			PROPERTY_INFO[i][property_EXT_PICKUP_ID] = INVALID_STREAMER_ID;
			
			PROPERTY_INFO[i][property_EXT_PICKUP_ID] = CreateDynamicPickup(1273, 1, PROPERTY_INFO[i][property_EXT_X], PROPERTY_INFO[i][property_EXT_Y], PROPERTY_INFO[i][property_EXT_Z], 0, PROPERTY_INFO[i][property_EXT_INTERIOR]);
			info[0] = PICKUP_TYPE_PROPERTY;
			info[1] = i; // Index
			info[2] = 2; // Pickup Exterior
			Streamer_SetArrayData(STREAMER_TYPE_PICKUP, PROPERTY_INFO[i][property_EXT_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
						
			//
			total ++;
		}
	}
	
	//
	mysql_tquery(srp_db, "UPDATE properties SET id_player = NULL, id_territory = NULL;");
	
	SendFormatNotification(playerid, "Se han liberado todas las propiedades (%d).", total);
	
	SendCmdLogToAdmins(playerid, "cleanproperties", params);
	return 1;
}

CMD:cleanshop(playerid, params[])
{
	mysql_tquery(srp_db, "DELETE FROM shop;");
	SendNotification(playerid, "Ok.");
	return 1;
}

CMD:countv(playerid, params[])
{
	if(sscanf(params, "u", params[0])) return SendNotification(playerid, "~r~Modo de uso:~w~ /countv <playerid>");
	SendFormatNotification(playerid, "Vehicles: %d", CountPlayerVehicles(params[0]));
	return 1;
}

CMD:countp(playerid, params[])
{
	if(sscanf(params, "u", params[0])) return SendNotification(playerid, "~r~Modo de uso:~w~ /countp <playerid>");
	SendFormatNotification(playerid, "Properties: %d", CountPlayerProperties(params[0]));
	return 1;
}

CMD:presolv(playerid, params[])
{
	if(sscanf(params, "d", params[0])) return SendNotification(playerid, "~r~Modo de uso:~w~ /presolv <numero>");
	
	inline OnInfoQueryLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			if(rows)
			{
				new id, name[24], connected, pid;
				cache_get_value_name_int(0, "id", id);
				cache_get_value_name(0, "name", name);
				cache_get_value_name_int(0, "connected", connected);
				cache_get_value_name_int(0, "playerid", pid);
				SendFormatNotification(playerid, "PN: %d --- Nombre: '%s' DB-ID: '%d' Conectado: '%d' Playerid: '%d'", params[0], name, id, connected, pid);
			}
			else SendNotification(playerid, "No se encontró ningún resultado para ese número.");
		}	
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, name, connected, playerid FROM player WHERE phone_number = %d LIMIT 1;", params[0]);
	MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	return 1;
}

CMD:osetname(playerid, params[])
{
	new to_account, new_name[24];
	if(sscanf(params, "ds[20]", to_account, new_name)) return SendNotification(playerid, "~r~Modo de uso:~w~ /osetname <DB-ID> <name>");
	if(!IsValidRPName(new_name)) return SendFormatNotification(playerid, "El nombre '%s' no cumple con el formato Nombre_Apellido.", new_name);

	inline OnNameChecked()
	{
		new crows;
		if(cache_get_row_count(crows))
		{
			if(crows) SendFormatNotification(playerid, "El nombre %s está en uso", new_name);
			else
			{
				inline OnInfoQueryLoad()
				{
					new rows;
					if(cache_get_row_count(rows))
					{
						if(rows)
						{
							new id, ip[16], name[24], connected, pid, admin_level;
							cache_get_value_name_int(0, "id", id);
							cache_get_value_name(0, "ip", ip);
							cache_get_value_name(0, "name", name);
							cache_get_value_name_int(0, "connected", connected);
							cache_get_value_name_int(0, "playerid", pid);
							cache_get_value_name_int(0, "admin_level", admin_level);

							if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] >= admin_level)
							{
								if(connected) SendFormatNotification(playerid, "JUGADOR '%s' DB-ID '%d' conectado utilice /setname, su player_id: %d.", name, id, pid);
								else
								{
									mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET name = '%e' WHERE id = %d;", new_name, id);
									mysql_tquery(srp_db, QUERY_BUFFER);

									for(new i = 0; i != MAX_PROPERTIES; i ++)
									{
										if(!PROPERTY_INFO[i][property_VALID]) continue;
										if(PROPERTY_INFO[i][property_OWNER_ID] != id) continue;
										
										new label_str[256];
										format
										(
											label_str, 
											sizeof label_str, 
											"\
												Propiedad {"#PRIMARY_COLOR"}#%d\n\n\
												{FFFFFF}Propietario: {"#PRIMARY_COLOR"}%s\n\
												{FFFFFF}Presiona {"#PRIMARY_COLOR"}~k~~CONVERSATION_YES~ {FFFFFF}para entrar.\
											", PROPERTY_INFO[i][property_ID], new_name
										);
										UpdateDynamic3DTextLabelText(PROPERTY_INFO[i][property_EXT_LABEL_ID], 0xFFFFFFFF, label_str);
									}
									SendFormatNotification(playerid, "El nombre de '%s' ahora es: %s", name, new_name);
								}
							}
							else SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
						}
						else SendNotification(playerid, "No se encontro la DB-ID.");
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, ip, name, connected, playerid, admin_level FROM player WHERE id = %d;", to_account);
				MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
			}
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM player WHERE name = '%e';", new_name);
	MySQL_TQueryInline(srp_db, using inline OnNameChecked, QUERY_BUFFER);
	return 1;
}

CMD:ogivecoins(playerid, params[])
{
	new to_account, amount;
	if(sscanf(params, "dd", to_account, amount)) return SendNotification(playerid, "~r~Modo de uso:~w~ /ogivecoins <DB-ID> <cantidad>");

	inline OnInfoQueryLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			if(rows)
			{
				new id, ip[16], name[24], connected, pid, admin_level;
				cache_get_value_name_int(0, "id", id);
				cache_get_value_name(0, "ip", ip);
				cache_get_value_name(0, "name", name);
				cache_get_value_name_int(0, "connected", connected);
				cache_get_value_name_int(0, "playerid", pid);
				cache_get_value_name_int(0, "admin_level", admin_level);

				if(connected) SendFormatNotification(playerid, "JUGADOR '%s' DB-ID '%d' conectado, player_id: %d, use /givecoins.", name, id, pid);
				else
				{
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET coins = coins + %d WHERE id = %d;", amount, id);
					mysql_tquery(srp_db, QUERY_BUFFER);
					SendFormatNotification(playerid, "Le has dado %d "SERVER_COIN" a %s.", amount, name);
				}
			}
			else SendNotification(playerid, "No se encontro la DB-ID.");
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, ip, name, connected, playerid, admin_level FROM player WHERE id = %d;", to_account);
	MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	return 1;
}

CMD:con(playerid, params[])
{
	new stats[400+1];
	GetNetworkStats(stats, sizeof(stats));
	ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, "Estadísticas del servidor", stats, "OK", "");
	return 1;
}

CMD:notifica(playerid, params[])
{
	if(isnull(params)) return SendNotification(playerid, "Uso: /notifica [cmd/adm/ac/chat]");

	switch(YHash(params, false))
	{
		case _I<cmd>:
		{
			PLAYER_TEMP[playerid][pt_SEE_ACMD_LOG] ^= true;
			SendFormatNotification(playerid, "Log de comandos %sactivado", PLAYER_TEMP[playerid][pt_SEE_ACMD_LOG] ? ("") : ("des"));
		}
		case _I<adm>:
		{
			PLAYER_TEMP[playerid][pt_SEE_ADM_LOG] ^= true;
			SendFormatNotification(playerid, "Log admin general %sactivado", PLAYER_TEMP[playerid][pt_SEE_ADM_LOG] ? ("") : ("des"));
		}
		case _I<ac>:
		{
			PLAYER_TEMP[playerid][pt_SEE_AC_LOG] ^= true;
			SendFormatNotification(playerid, "Log de anticheat %sactivado", PLAYER_TEMP[playerid][pt_SEE_AC_LOG] ? ("") : ("des"));
		}
		case _I<chat>:
		{
			PLAYER_TEMP[playerid][pt_SEE_ADM_CHAT] ^= true;
			SendFormatNotification(playerid, "Chat admin %sactivado", PLAYER_TEMP[playerid][pt_SEE_ADM_CHAT] ? ("") : ("des"));
		}
		default:
			SendNotification(playerid, "Uso: /notifica [cmd/log/ac/chat]");
	}

	return 1;
}

CMD:changeflags(playerid, params[])
{
	new cmd[24], flags;
	if(sscanf(params, "s[24]d", cmd, flags)) return SendNotification(playerid, "Uso: /changeflags [cmd] [flags]");
	
	PC_SetFlags(cmd, flags);
	SendFormatNotification(playerid, "Flag de /%s cambiada a %d", cmd, flags);
	return 1;
}

CMD:cobject(playerid, params[]) {
	new modelid;
	if(sscanf(params, "d", modelid)) return SendNotification(playerid, "Uso: /cobject [modelid]");

	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	new objectid = CreateDynamicObject(modelid, pos[0], pos[1], pos[2], 0.0, 0.0, 0.0);
	Streamer_SetArrayData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_EXTRA_ID, {OBJECT_TYPE_TEST});
	Streamer_Update(playerid);
	EditDynamicObject(playerid, objectid);
	return 1;
}

CMD:selectobject(playerid, params[])
{
	SendNotification(playerid, "Entraste en selección de objetos");
	SelectObject(playerid);

	return 1;
}

CMD:std(playerid, params[]) {
	SelectTextDrawEx(playerid, 0xe8d08fFF);
	return 1;
}


CMD:historial(playerid, params[])
{	
	//
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /historial <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	if(!ACCOUNT_INFO[to_player][ac_ID]) return SendNotification(playerid, "DB ID = 0!");

	inline OnInfoQueryLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			new caption[40], dialog[1600], line_str[256];
			format(caption, sizeof caption, "Bad historial de %s (%d)", ACCOUNT_INFO[to_player][ac_NAME], to_player);
			format(dialog, sizeof dialog, "Por\tFecha\tTipo\tDescr.\n");
			if(rows)
			{
				for(new i = 0; i != rows; i ++)
				{
					new name[24], by, type, date[24], text[128], bool:isnull_by;
					cache_get_value_name(i, "name", name);
					cache_is_value_name_null(i, "by", isnull_by);
					if(!isnull_by) cache_get_value_name_int(i, "by", by);
					cache_get_value_name_int(i, "type", type);
					cache_get_value_name(i, "date", date);
					cache_get_value_name(i, "text", text);

					if(by == -1) name = "AC";
					switch(type)
					{
						case TYPE_WARNING: format(line_str, sizeof line_str, "%s\t%s\tAdv\t%s\n", name, date, text);
						case TYPE_KICK: format(line_str, sizeof line_str, "%s\t%s\tKick\t%s\n", name, date, text);
						case TYPE_BAN: format(line_str, sizeof line_str, "%s\t%s\tBan\t%s\n", name, date, text);
						case TYPE_TEMP_BAN: format(line_str, sizeof line_str, "%s\t%s\tTBan\t%s\n", name, date, text);
						case TYPE_UNBAN: format(line_str, sizeof line_str, "%s\t%s\tUnban\t%s\n", name, date, text);
					}
					strcat(dialog, line_str);
				}
			}
			else strcat(dialog, "Sin resultados");
			ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_TABLIST_HEADERS, caption, dialog, "X", "");
			SendNotification(playerid, "Limite: 20");
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT bad_history.*, player.name FROM bad_history LEFT JOIN player ON bad_history.by = player.id WHERE bad_history.id_player = %d ORDER BY bad_history.date LIMIT 20;", ACCOUNT_INFO[to_player][ac_ID]);
	MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	return 1;
}

CMD:getversion(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /getversion <playerid>");
	if(!IsPlayerConnected(to_player)) return SendNotification(playerid, "Jugador desconectado");

	new player_version[32];
	GetPlayerVersion(to_player, player_version, sizeof player_version);

	SendFormatNotification(playerid, "Jugador %s usa la versión %s", PLAYER_TEMP[to_player][pt_NAME], player_version);
	return 1;
}

CMD:arma(playerid, params[])
{
	new weaponid, ammo;
	if(sscanf(params, "dd", weaponid, ammo)) return SendNotification(playerid, "Uso: /arma [weaponid] [balas]");
	if(!WEAPON_INFO[weaponid][weapon_info_AMMO]) ammo = 1;
	
	GivePlayerWeaponEx(playerid, weaponid, ammo);
	RegisterNewPlayerWeapon(playerid, WEAPON_INFO[weaponid][weapon_info_SLOT]);
	return 1;
}

CMD:printtestvehicles(playerid, params[])
{
	for(new i, j = GetVehiclePoolSize(); i <= j; i++)
	{
		if(GLOBAL_VEHICLES[i][gb_vehicle_VALID] && GLOBAL_VEHICLES[i][gb_vehicle_TYPE] == VEHICLE_TYPE_TEST)
		{
			new Float:x, Float:y, Float:z, Float:angle;
			GetVehiclePos(i, x, y, z);
			GetVehicleZAngle(i, angle);
			printf("{VEHICLE_TYPE_, WORK_, exp, %d, %f, %f, %f, %f, color1, color2, vip, world},", GetVehicleModel(i), x, y, z, angle);
		}
	}
	SendNotification(playerid, "Se hizo un printf de todos los vehiculos de prueba - revisa la consola o el server_log.txt");
	return 1;
}

CMD:placaex(playerid, params[])
{
	if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "No eres policía.");
	if(PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_NONE && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE)
	{
		SendFormatNotification(playerid, "Tienes que dejar de estar de servicio como %s primero.", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME]);
		return 1;
	}
	
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
	
	if(!PLAYER_TEMP[playerid][pt_WORKING_IN]) ShowDialog(playerid, DIALOG_SELECT_POLICE_SKIN);
	else
	{
		CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
		PI[playerid][ac_POLICE_DUTY] = 0;
	}
	return 1;
}
alias:placaex("policiaadmin")

CMD:vehicles(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /vehicles <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	
	new dialog[115 * (MAX_SU_VEHICLES + 2)], total_vehicles;
	format(dialog, sizeof dialog, "Vehículo\tMatrícula\tDistancia\n");
	for(new i = 0; i != MAX_VEHICLES; i ++)
	{
		if(!GLOBAL_VEHICLES[i][gb_vehicle_VALID]) continue;
		if(!PLAYER_VEHICLES[i][player_vehicle_VALID]) continue;
		if(total_vehicles > MAX_SU_VEHICLES) break;
		
		if(PLAYER_VEHICLES[i][player_vehicle_OWNER_ID] == ACCOUNT_INFO[to_player][ac_ID])
		{
			
			new Float:pos[3];
			GetVehiclePos(i, pos[0], pos[1], pos[2]);
			
			new line_str[115];
			format(line_str, sizeof line_str, "{FFFFFF}%d. %s\t{FFFFFF}%s\t{"#SILVER_COLOR"}/gotoveh %d\n", total_vehicles + 1, VEHICLE_INFO[ GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME], GLOBAL_VEHICLES[i][gb_vehicle_NUMBER_PLATE], i);
			strcat(dialog, line_str);
			total_vehicles ++;
		}
	}
	if(total_vehicles == 0) strcat(dialog, "{"#SILVER_COLOR"}El jugador no tiene vehiculos");
	
	ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_TABLIST_HEADERS, "Vehiculos", dialog, "OK", "");
	return 1;
}
alias:vehicles("vehiculos")

CMD:reportar(playerid, params[])
{
	if(gettime() < PLAYER_TEMP[playerid][pt_ANTIFLOOD_REPORT] + 5) return SendNotification(playerid, "Cálmate.");
	PLAYER_TEMP[playerid][pt_ANTIFLOOD_REPORT] = gettime();
	
	new reason[128];
	if(sscanf(params, "us[128]", params[0], reason)) return SendNotification(playerid, "~r~Modo de uso: ~w~/reportar [PlayerID/Nombre] [Razón]");
    if(!IsPlayerConnected(params[0])) return SendNotification(playerid, "Jugador desconectado.");
	
	SendNotification(playerid, "Tu reporte ha sido enviado a los administradores en línea.");
	
	new str[145]; format(str, 145, "{"#RED_COLOR"}[REPORTE] {FFFFFF}%s (%d) > %s (%d): %s", ACCOUNT_INFO[playerid][ac_NAME], playerid, ACCOUNT_INFO[params[0]][ac_NAME], params[0], reason);
	SendMessageToAdmins(-1, str);
	return 1;
}

CMD:r(playerid, params[])
{
	new message[128];
	if(sscanf(params, "s[128]", message)) return SendNotification(playerid, "~r~Modo de uso: ~w~/r [MENSAJE]");
	
	if(PLAYER_TEMP[playerid][pt_ADMIN_PM_PID] == INVALID_PLAYER_ID || !PLAYER_TEMP[playerid][pt_ADMIN_PM_AID]) return SendNotification(playerid, "Nada para responder.");
	if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_ADMIN_PM_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_ADMIN_PM_AID])
	{
		PLAYER_TEMP[playerid][pt_ADMIN_PM_PID] = INVALID_PLAYER_ID;
		PLAYER_TEMP[playerid][pt_ADMIN_PM_AID] = 0;
		PLAYER_TEMP[playerid][pt_ADMIN_PM_TIME] = 0;
		SendNotification(playerid, "El administrador que te envió el mensaje está desconectado.");
		return 1;
	}
	if(gettime() > PLAYER_TEMP[playerid][pt_ADMIN_PM_TIME] + 60)
	{
		PLAYER_TEMP[playerid][pt_ADMIN_PM_PID] = INVALID_PLAYER_ID;
		PLAYER_TEMP[playerid][pt_ADMIN_PM_AID] = 0;
		PLAYER_TEMP[playerid][pt_ADMIN_PM_TIME] = 0;
		SendNotification(playerid, "Has tardado demasiado en responser.");
		return 1;
	}
	
	SendClientMessageEx(PLAYER_TEMP[playerid][pt_ADMIN_PM_PID], -1, "Respuesta de %s (%d): %s", ACCOUNT_INFO[playerid][ac_NAME], playerid, message);
	SendNotification(playerid, "Tu mensaje ha sido enviado al administrador.");
	PLAYER_TEMP[playerid][pt_ADMIN_PM_PID] = INVALID_PLAYER_ID;
	PLAYER_TEMP[playerid][pt_ADMIN_PM_AID] = 0;
	PLAYER_TEMP[playerid][pt_ADMIN_PM_TIME] = 0;
	return 1;
}

CMD:id(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso: ~w~/id [PlayerID/Nombre]");
	if(!IsPlayerConnected(to_player)) return SendNotification(playerid, "Jugador desconectado.");
	
	SendFormatNotification(playerid, "Nombre: '%s'~n~DB-ID: '%d'~n~Playerid: '%d'~n~Nivel: %d~n~Ping: %d", ACCOUNT_INFO[to_player][ac_NAME], ACCOUNT_INFO[to_player][ac_ID], to_player, ACCOUNT_INFO[to_player][ac_LEVEL], GetPlayerPing(to_player));
	return 1;
}

CMD:admins(playerid, params[])
{
	if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] < 2) return SendNotification(playerid, "SERVER: Unknown command.");
	PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] = 10;
	PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
	ShowDialog(playerid, DIALOG_ADMIN_LIST);
	return 1;
}

CMD:trabajos(playerid)
{
	ShowDialog(playerid, DIALOG_WORKS_TELEPORTS);
	return 1;
}

CMD:getid(playerid, params[])
{
	new findname[24];
	if(sscanf(params, "s[24]", findname)) return SendNotification(playerid, "~r~Modo de uso:~w~ /getid <nombre o parte del nombre>");

	inline OnInfoQueryLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			for(new i = 0; i != rows; i ++)
			{
				new id, name[24], connected, pid;
				cache_get_value_name_int(i, "id", id);
				cache_get_value_name(i, "name", name);
				cache_get_value_name_int(i, "connected", connected);
				cache_get_value_name_int(i, "playerid", pid);
				SendClientMessageEx(playerid, -1, "Nombre: '%s' DB-ID: '%d' Conectado: '%d' Playerid: '%d'", name, id, connected, pid);
			}
			SendClientMessageEx(playerid, -1, "Se encontraron %d coincidencias, el límite es 20.", rows);
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, name, connected, playerid FROM player WHERE name LIKE '%%%e%%' LIMIT 20;", findname);
	MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	return 1;
}

CMD:getname(playerid, params[])
{
	new db_id;
	if(sscanf(params, "d", db_id)) return SendNotification(playerid, "~r~Modo de uso:~w~ /getname <DB-ID>");
	
	inline OnInfoQueryLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			for(new i = 0; i != rows; i ++)
			{
				new id, name[24], connected, pid;
				cache_get_value_name_int(i, "id", id);
				cache_get_value_name(i, "name", name);
				cache_get_value_name_int(i, "connected", connected);
				cache_get_value_name_int(i, "playerid", pid);
				SendClientMessageEx(playerid, -1, "Nombre: '%s' DB-ID: '%d' Conectado: '%d' Playerid: '%d'", name, id, connected, pid);
			}
			SendClientMessageEx(playerid, -1, "Se encontraron %d coincidencias, el límite es 20.", rows);
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, name, connected, playerid FROM player WHERE id = %d LIMIT 20;", db_id);
	MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	return 1;
}

CMD:aka(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /aka <player_id>");
	if(!IsPlayerConnected(to_player)) return SendNotification(playerid, "Jugador desconectado.");
	if(isnull(ACCOUNT_INFO[to_player][ac_IP])) return SendNotification(playerid, "IP no válida.");
	
	inline OnInfoQueryLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			SendFormatNotification(playerid, "AKA de %s (%d):", ACCOUNT_INFO[to_player][ac_NAME], to_player);
			for(new i = 0; i != rows; i ++)
			{
				new id, name[24];
				cache_get_value_name_int(i, "id", id);
				cache_get_value_name(i, "name", name);
				SendClientMessageEx(playerid, -1, "Nombre: {f4f442}'%s' {FFFFFF}DB-ID: '%d'", name, id);
			}
			SendClientMessageEx(playerid, -1, "Se encontraron %d coincidencias, el límite es 20.", rows);
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, name FROM player WHERE ip = '%e' LIMIT 20;", ACCOUNT_INFO[to_player][ac_IP]);
	MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	return 1;
}
alias:aka("cuentas", "multicuentas")

CMD:adv(playerid, params[])
{
	new to_player, reason[128];
	if(sscanf(params, "us[128]", to_player, reason)) return SendNotification(playerid, "~r~Modo de uso:~w~ /adv <player_id> <razon>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	
	AddPlayerBadHistory(ACCOUNT_INFO[to_player][ac_ID], ACCOUNT_INFO[playerid][ac_ID], TYPE_WARNING, reason);
	
	new dialog[170];
	format(dialog, sizeof dialog, "Has recibido una advertencia, razón:\n%s\n", reason);
	ShowPlayerDialog(to_player, DIALOG_INFO, DIALOG_STYLE_MSGBOX, "Aviso", dialog, "Entiendo", "");
	
	SendFormatNotification(playerid, "Jugador (nick: '%s' dbid: '%d', pid: '%d') advertido.", ACCOUNT_INFO[to_player][ac_NAME], ACCOUNT_INFO[to_player][ac_ID], to_player);
	

	new str[145]; format(str, 145, "{"#RED_COLOR"}[ADMIN] {FFFFFF}%s (%d) advirtió a %s (%d): %s", ACCOUNT_INFO[playerid][ac_NAME], playerid, ACCOUNT_INFO[to_player][ac_NAME], to_player, reason);
	SendAdminAd(-1, str);
	return 1;
}
alias:adv("advertencia", "san")

CMD:kick(playerid, params[])
{
	new to_player, reason[128];
	if(sscanf(params, "us[128]", to_player, reason)) return SendNotification(playerid, "~r~Modo de uso:~w~ /kick <player_id> <razon>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	if(PLAYER_TEMP[to_player][pt_KICKED]) return SendNotification(playerid, "El jugador ya está expulsado.");
	if(!ACCOUNT_INFO[to_player][ac_ID]) return Kick(to_player);
	
	
	AddPlayerBadHistory(ACCOUNT_INFO[to_player][ac_ID], ACCOUNT_INFO[playerid][ac_ID], TYPE_KICK, reason);
	
	new dialog[170];
	format(dialog, sizeof dialog, "Has sido expulsado, razón:\n%s\n", reason);
	ShowPlayerDialog(to_player, DIALOG_INFO, DIALOG_STYLE_MSGBOX, "Aviso", dialog, "Entiendo", "");
	KickEx(to_player, 500);
	
	SendFormatNotification(playerid, "Jugador (nick: '%s' dbid: '%d', pid: '%d') expulsado.", ACCOUNT_INFO[to_player][ac_NAME], ACCOUNT_INFO[to_player][ac_ID], to_player);
	
	
	new str[145]; format(str, 145, "{"#RED_COLOR"}[ADMIN] {FFFFFF}%s (%d) expulsó a %s (%d): %s", ACCOUNT_INFO[playerid][ac_NAME], playerid, ACCOUNT_INFO[to_player][ac_NAME], to_player, reason);
	SendAdminAd(-1, str);
	return 1;
}

CMD:spec(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /spec <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");

	if(GetPlayerState(playerid) != PLAYER_STATE_SPECTATING)
    {
		new Float:p[4];
		GetPlayerPos(playerid, p[0], p[1], p[2]);
		GetPlayerFacingAngle(playerid, p[3]);
		SetSpawnInfo(playerid, NO_TEAM, GetPlayerSkin(playerid), p[0], p[1], p[2], p[3], 0, 0, 0, 0, 0, 0);
		PI[playerid][ac_INTERIOR] = GetPlayerInterior(playerid);
		PLAYER_TEMP[playerid][pt_VIRTUAL_WORLD] = GetPlayerVirtualWorld(playerid);
	}
	
	TogglePlayerSpectatingEx(playerid, true);
	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(to_player));
	SetPlayerInterior(playerid, GetPlayerInterior(to_player));
	if(IsPlayerInAnyVehicle(to_player)) PlayerSpectateVehicle(playerid, GetPlayerVehicleID(to_player));
	else PlayerSpectatePlayer(playerid, to_player);
	SendNotification(playerid, "Utiliza /specoff para terminar este modo.");
	
	
	SendCmdLogToAdmins(playerid, "spec", params);
	return 1;
}
alias:spec("specp", "spp")

CMD:specoff(playerid, params[])
{
	SetPlayerInterior(playerid, PI[playerid][ac_INTERIOR]);
	SetPlayerVirtualWorld(playerid, PLAYER_TEMP[playerid][pt_VIRTUAL_WORLD]);
	TogglePlayerSpectatingEx(playerid, false);
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][3]);
	PLAYER_TEMP[playerid][pt_TIMERS][3] = SetTimerEx("TogglePlayerControl", 2000, false, "ib", playerid, true);
	return 1;
}
alias:specoff("listo")

CMD:freeze(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /freeze <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	
	SendFormatNotification(playerid, "Jugador '%s' (%d) congelado.", ACCOUNT_INFO[to_player][ac_NAME], to_player);
	TogglePlayerControllableEx(to_player, false);
	return 1;
}
alias:freeze("congelar")

CMD:unfreeze(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /unfreeze <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	
	SendFormatNotification(playerid, "Jugador '%s' (%d) descongelado.", ACCOUNT_INFO[to_player][ac_NAME], to_player);
	TogglePlayerControllableEx(to_player, true);
	return 1;
}
alias:unfreeze("descongelar")

CMD:pest(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /pest <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	ShowPlayerStats(playerid, to_player);
	return 1;
}

CMD:pinv(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /pinv <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	ShowPlayerInventory(playerid, to_player);
	return 1;
}

CMD:pexp(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /pexp <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	ShowPlayerSkills(playerid, to_player);
	return 1;
}

CMD:parmas(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /parmas <player_id>");
	
	new dialog[95 * 15], line_str[95];
	format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Arma\t{"#BLUE_COLOR"}Munición\t{"#SILVER_COLOR"}Slot\n");
	
	for(new i = 0; i < sizeof PLAYER_WEAPONS[]; i ++)
	{
		if(!PLAYER_WEAPONS[to_player][i][player_weapon_VALID]) continue;
		
		format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}%s\t{"#SILVER_COLOR"}%d\n", WEAPON_INFO[ PLAYER_WEAPONS[to_player][i][player_weapon_ID] ][weapon_info_NAME], number_format_thousand(PLAYER_WEAPONS[to_player][i][player_weapon_AMMO]), i);
		strcat(dialog, line_str);
	}

	format(line_str, sizeof line_str, "Armas de %s", PLAYER_TEMP[playerid][pt_RP_NAME]);
	
	ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_TABLIST_HEADERS, line_str, dialog, "Continuar", "Cerrar");
	return 1;
}

CMD:pbank(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /pbank <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	if(!PI[to_player][ac_BANK_ACCOUNT]) SendNotification(playerid, "El jugador no tiene cuenta bancaria.");
	else SendFormatNotification(playerid, "Cuenta bancaria ID: '%d' Balance: '%s'", PI[to_player][ac_BANK_ACCOUNT], number_format_thousand(PI[to_player][ac_BANK_MONEY]));
	return 1;
}

CMD:unjail(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /unjail <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	if(ACCOUNT_INFO[to_player][ac_STATE] != ROLEPLAY_STATE_JAIL) return SendNotification(playerid, "El jugador no está en el cárcel.");
	
	UnjailPlayer(to_player);
	SendFormatNotification(playerid, "El jugador %s (%d) ahora está en libertad.", ACCOUNT_INFO[to_player][ac_NAME], to_player);
	
	
	SendCmdLogToAdmins(playerid, "unjail", params);
	return 1;
}

CMD:ip(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /ip <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");

	SendFormatNotification(playerid, "IP %s (%d): %s", ACCOUNT_INFO[to_player][ac_NAME], to_player, ACCOUNT_INFO[to_player][ac_IP]);
	return 1;
}

CMD:traerveh(playerid, params[])
{
	new to_car;
	if(sscanf(params, "i", to_car)) return SendNotification(playerid, "~r~Modo de uso:~w~ /traerveh <car_id>");
	if(to_car >= MAX_VEHICLES) return 1;

	if(!GLOBAL_VEHICLES[to_car][gb_vehicle_VALID]) return SendNotification(playerid, "Vehículo no válido.");
	if(GLOBAL_VEHICLES[to_car][gb_vehicle_OCCUPIED]) return SendNotification(playerid, "Vehículo está ocupado.");
		
    new Float:gPos[3];
    GetPlayerPos(playerid, gPos[0], gPos[1], gPos[2]);
    SetVehiclePosEx(to_car, gPos[0] + 2.0, gPos[1], gPos[2]);
    return 1;
}
alias:traerveh("mover", "getveh", "traervehiculo", "traerauto", "getcar")


///	CMD:gotopos( playerid, params[] )
//	{
//		new Float:g_x,Float:g_y,Float:g_z,int_id,vw_id;
//		if ( sscanf ( params, "p<,>fffdd", g_x, g_y, g_z, int_id,vw_id) ) return 1;
//		SetPlayerPosEx(playerid, g_x, g_y, g_z,0.0,int_id,vw_id);
//		return 1;
//	}
//	

CMD:gotoveh(playerid, params[])
{
	new to_car;
	if(sscanf(params, "i", to_car)) return SendNotification(playerid, "~r~Modo de uso:~w~ /gotoveh <car_id>");
	if(to_car >= MAX_VEHICLES) return 1;

	if(!GLOBAL_VEHICLES[to_car][gb_vehicle_VALID]) return SendNotification(playerid, "Vehículo no válido.");
		
    new Float:gPos[3];
    GetVehiclePos(to_car, gPos[0], gPos[1], gPos[2]);
    SetPlayerPosEx(playerid, gPos[0], gPos[1], gPos[2], 0.0, GLOBAL_VEHICLES[to_car][gb_vehicle_INTERIOR], 0);//GLOBAL_VEHICLES[to_car][gb_vehicle_WORLD]);
    return 1;
}
alias:gotoveh("irvehiculo", "gotocar", "iracarro")

CMD:duty(playerid)
{
	if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] < 1) return SendNotification(playerid, "SERVER: Unknown command.");
	if(PLAYER_TEMP[playerid][pt_ADMIN_SERVICE])
	{
		PLAYER_TEMP[playerid][pt_ADMIN_SERVICE] = false;
		SetPlayerNormalColor(playerid);
		
		if(IsValidDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_ADMIN_LABEL]))
		{
			DestroyDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_ADMIN_LABEL]);
			PLAYER_TEMP[playerid][pt_ADMIN_LABEL] = Text3D:INVALID_STREAMER_ID;
		}

		SendFormatNotification(playerid, "Ahora no estás de servicio como %s.", ADMIN_LEVELS[ ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] ]);
	}	
	else
	{
		PLAYER_TEMP[playerid][pt_ADMIN_SERVICE] = true;
		SetPlayerNormalColor(playerid);
		
		if(IsValidDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_ADMIN_LABEL]))
		{
			DestroyDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_ADMIN_LABEL]);
			PLAYER_TEMP[playerid][pt_ADMIN_LABEL] = Text3D:INVALID_STREAMER_ID;
		}
		
		new label_str[64];
		switch(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL])
		{
			case 1: format(label_str, sizeof label_str, "{0099FF}%s en servicio", ADMIN_LEVELS[ ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] ]);
			case 2: format(label_str, sizeof label_str, "{FFFF00}%s en servicio", ADMIN_LEVELS[ ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] ]);
			case 3: format(label_str, sizeof label_str, "{00CC00}%s en servicio", ADMIN_LEVELS[ ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] ]);
			case 4: format(label_str, sizeof label_str, "{FFCC00}%s en servicio", ADMIN_LEVELS[ ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] ]);
			case 5: format(label_str, sizeof label_str, "{FFFFCC}%s en servicio", ADMIN_LEVELS[ ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] ]);
		}
		PLAYER_TEMP[playerid][pt_ADMIN_LABEL] = CreateDynamic3DTextLabel(label_str, -1, 0.0, 0.0, 0.3, 20.0, playerid, .testlos = true);

		if(PLAYER_TEMP[playerid][pt_WORKING_IN]) SendFormatNotification(playerid, "Deberías dejar de estar de servicio como %s para administrar mejor...", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME]);
		SendFormatNotification(playerid, "Ahora estás de servicio como %s.", ADMIN_LEVELS[ ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] ]);
	}
	return 1;
}

CMD:goto(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /goto <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");

	new Float:p[4];
	GetPlayerPos(to_player, p[0], p[1], p[2]);
	GetPlayerFacingAngle(to_player, p[3]);
	
	SetPlayerPosEx(playerid, p[0], p[1], p[2], p[3], GetPlayerInterior(to_player), GetPlayerVirtualWorld(to_player), false, true);
	SetPlayerFacingAngle(playerid, p[3] + 180.0);

    SetPlayerCityWeather(playerid);
	
	SendCmdLogToAdmins(playerid, "goto", params);
	return 1;
}
alias:goto("ir")

CMD:get(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /get <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");

	new Float:p[4];
	GetPlayerPos(playerid, p[0], p[1], p[2]);
	GetPlayerFacingAngle(playerid, p[3]);
	
	SetPlayerPosEx(to_player, p[0], p[1], p[2], p[3], GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid), false, true);
	SetPlayerFacingAngle(to_player, p[3] + 180.0);

    SetPlayerCityWeather(to_player);

	SendCmdLogToAdmins(playerid, "get", params);
	return 1;
}
alias:get("traer")

CMD:unban(playerid, params[])
{
	new name[24];
	if(sscanf(params, "s[24]", name)) return SendNotification(playerid, "~r~Modo de uso:~w~ /unban <nombre completo o ip>");
	
	inline OnInfoQueryLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			if(rows)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM bans WHERE name = '%e' OR ip = '%e';", name, name);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				SendFormatNotification(playerid, "'%s' ha sido desbaneado.", name);
				
				new str[145]; format(str, 145, "{"#RED_COLOR"}[ADMIN] {FFFFFF}%s (%d) ha desbaneado a '%s'.", ACCOUNT_INFO[playerid][ac_NAME], playerid, name);
				SendMessageToAdmins(-1, str);

				inline OnCountQueryLoad()
				{
					new crows;
					if(cache_get_row_count(crows))
					{
						if(crows)
						{
							new id_player;
							cache_get_value_index_int(0, 0, id_player);
							if(id_player) AddPlayerBadHistory(id_player, ACCOUNT_INFO[playerid][ac_ID], TYPE_UNBAN, "/unban");
						}
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM player WHERE name = '%e' OR ip = '%e';", name, name);
				MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
			}
			else SendFormatNotification(playerid, "'%s' no está en la lista de baneados.", name);
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM bans WHERE name = '%e' OR ip = '%e';", name, name);
	MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	return 1;
}

CMD:jail(playerid, params[])
{
    new to_player, reason[128], time;
    if(sscanf(params, "uds[128]", to_player, time, reason)) return SendNotification(playerid, "~r~Modo de uso:~w~ /jail <player_id> <minutos> <razón>");
	if(time < 0 || time > 1440) return SendNotification(playerid, "Intervalo de minutos incorrecto.");
    if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
    if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");

    StopAudioStreamForPlayer(to_player);
    CancelEdit(to_player);
    HidePlayerDialog(to_player);
	CallLocalFunction("EndPlayerJob", "iib", to_player, PLAYER_TEMP[to_player][pt_WORKING_IN], true);
    PLAYER_TEMP[to_player][pt_HUNGRY_MESSAGE] = false;
    PLAYER_TEMP[to_player][pt_THIRST_MESSAGE] = false;
    PLAYER_TEMP[to_player][pt_PLAYER_IN_ATM] = false;
    PLAYER_TEMP[to_player][pt_CUFFED] = false;
    PLAYER_TEMP[to_player][pt_CUFFING] = false;
    PLAYER_TEMP[to_player][pt_PLAYER_WAITING_MP3_HTTP] = false;
    if(PLAYER_TEMP[to_player][pt_WANT_TAXI])
    {
        PLAYER_TEMP[to_player][pt_WANT_TAXI] = false;
        DisablePlayerTaxiMark(to_player);
    }
    if(PLAYER_TEMP[to_player][pt_PLAYER_IN_CALL]) EndPhoneCall(to_player);
    if(PLAYER_TEMP[to_player][pt_GPS_MAP]) HidePlayerGpsMap(to_player);

	PI[params[0]][ac_POLICE_JAIL_ID] = 0;
    JailPlayer(to_player, time * 60);
    SendClientMessageEx(to_player, -1, "{"#SILVER_COLOR"}Te quedan %s minutos de sanción, razón: %s.", TimeConvert(time * 60), reason);
    SetPlayerSpecialAction(to_player, SPECIAL_ACTION_NONE);

    new str[145]; format(str, 145, "{"#RED_COLOR"}[ADMIN] {FFFFFF}%s (%d) sancionó a %s (%d): %s.", ACCOUNT_INFO[playerid][ac_NAME], playerid, ACCOUNT_INFO[to_player][ac_NAME], to_player, reason);
    SendAdminAd(-1, str);
    return 1;
}

CMD:say(playerid, params[])
{
	new 
		to_player, 
		command[128];

	if(sscanf(params, "us[128]", to_player, command)) return SendNotification(playerid, "~r~Modo de uso:~w~ /say <player_id> <comando>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");

	if(strfind(command, "/", true) == -1) CallLocalFunction("OnPlayerText", "is", to_player, command);
	else 
	{
		PLAYER_TEMP[to_player][pt_ANTIFLOOD_TALK] = GetTickCount();
		PC_EmulateCommand(to_player, command);
	}

	SendCmdLogToAdmins(playerid, "say", params);
	return 1;
}

CMD:ban(playerid, params[])
{	
	new to_player, reason[128];
	if(sscanf(params, "us[128]", to_player, reason)) return SendNotification(playerid, "~r~Modo de uso:~w~ /ban <player_id> <razon>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	if(PLAYER_TEMP[to_player][pt_KICKED]) return SendNotification(playerid, "El jugador ya está expulsado.");
	if(!ACCOUNT_INFO[to_player][ac_ID]) return Kick(to_player);
	
	AddPlayerBan(ACCOUNT_INFO[to_player][ac_ID], ACCOUNT_INFO[to_player][ac_NAME], ACCOUNT_INFO[to_player][ac_IP], ACCOUNT_INFO[playerid][ac_ID], TYPE_BAN, reason);
	
	new dialog[250];
	format(dialog, sizeof dialog, "Has sido baneado, razón:\n%s\n", reason);
	ShowPlayerDialog(to_player, DIALOG_INFO, DIALOG_STYLE_MSGBOX, "Aviso", dialog, "Entiendo", "");
	KickEx(to_player, 500);
	
	SendFormatNotification(playerid, "Jugador (nick: '%s' dbid: '%d', id: '%d') baneado.", ACCOUNT_INFO[to_player][ac_NAME], ACCOUNT_INFO[to_player][ac_ID], to_player);
	
	new str[145]; format(str, 145, "{"#RED_COLOR"}[ADMIN] {FFFFFF}%s (%d) baneó a %s (%d): %s.", ACCOUNT_INFO[playerid][ac_NAME], playerid, ACCOUNT_INFO[to_player][ac_NAME], to_player, reason);
	SendAdminAd(-1, str);
	return 1;
}

CMD:cls(playerid,params[])
{	
	for(new i = 0; i != 80; i ++) SendClientMessageToAll(-1, " ");
	
	SendCmdLogToAdmins(playerid, "cls", params);
	return 1;
}
alias:cls("log")

CMD:tban(playerid, params[])
{	
	new to_player, days, reason[128];
	if(sscanf(params, "uds[128]", to_player, days, reason)) return SendNotification(playerid, "~r~Modo de uso:~w~ /tban <player_id> <dias> <razon>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	if(days <= 0 || days > 9999) return SendNotification(playerid, "~r~Modo de uso:~w~ /ban <player_id> <dias> <razon>");
	
	if(PLAYER_TEMP[to_player][pt_KICKED]) return SendNotification(playerid, "El jugador ya está expulsado.");
	if(!ACCOUNT_INFO[to_player][ac_ID]) return Kick(to_player);
	
	AddPlayerBan(ACCOUNT_INFO[to_player][ac_ID], ACCOUNT_INFO[to_player][ac_NAME], ACCOUNT_INFO[to_player][ac_IP], ACCOUNT_INFO[playerid][ac_ID], TYPE_TEMP_BAN, reason, days);
	
	new dialog[250];
	format(dialog, sizeof dialog, "Has sido baneado por %d días, razón:\n%s\n", days, reason);
	ShowPlayerDialog(to_player, DIALOG_INFO, DIALOG_STYLE_MSGBOX, "Aviso", dialog, "Entiendo", "");
	KickEx(to_player, 500);
	
	SendFormatNotification(playerid, "Jugador (nick: '%s' dbid: '%d', pid: '%d') baneado por %d días.", ACCOUNT_INFO[to_player][ac_NAME], ACCOUNT_INFO[to_player][ac_ID], to_player, days);
	
	
	new str[145]; format(str, 145, "{"#RED_COLOR"}[ADMIN] {FFFFFF}%s (%d) baneó %d días a %s (%d): %s", ACCOUNT_INFO[playerid][ac_NAME], playerid, days, ACCOUNT_INFO[to_player][ac_NAME], to_player, reason);
	SendAdminAd(-1, str);
	return 1;
}

CMD:setwlevel(playerid, params[]) {
	new to_player, level;
	if(sscanf(params, "ud", to_player, level)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setwlevel <to_player> <level>");
	SetPlayerWantedLevelEx(to_player, level);
	return 1;
}

CMD:sound(playerid, params[]) {
	new id;
	if(sscanf(params, "d", id)) return SendNotification(playerid, "~r~Modo de uso:~w~ /sound <id>");
	PlayerPlaySound(playerid, id, 0.0, 0.0, 0.0);
	return 1;
}

CMD:anim(playerid, params[]) {
	new animLib[32], animName[32];
	if(sscanf(params, "s[32]s[32]", animLib, animName)) return SendNotification(playerid, "~r~Modo de uso:~w~ /anim <animLib> <animName>");
	ApplyAnimation(playerid, animLib, animName, 4.1, 1, 0, 0, 0, 0, 1);
	return 1;
}

CMD:dban(playerid, params[])
{
	new reason[128], to_account;
	if(sscanf(params, "ds[128]", to_account, reason)) return SendNotification(playerid, "~r~Modo de uso:~w~ /dban <DB-ID> <razon>");
	
	inline OnInfoQueryLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			if(rows)
			{
				new id, ip[16], name[24], connected, pid, admin_level;
				cache_get_value_name_int(0, "id", id);
				cache_get_value_name(0, "ip", ip);
				cache_get_value_name(0, "name", name);
				cache_get_value_name_int(0, "connected", connected);
				cache_get_value_name_int(0, "playerid", pid);
				cache_get_value_name_int(0, "admin_level", admin_level);

				if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] >= admin_level)
				{
					if(connected) SendFormatNotification(playerid, "JUGADOR '%s' DB-ID '%d' conectado utilice /ban, su player_id: %d.", name, id, pid);
					else
					{
						inline OnCountQueryLoad()
						{
							new crows;
							if(cache_get_row_count(crows))
							{
								if(crows)
								{
									new expire_date[24];
									cache_get_value_name(0, "expire_date", expire_date);
									
									if(isnull(expire_date)) SendFormatNotification(playerid, "JUGADOR (Nombre: '%s' DB-ID: '%d') ya está baneado (permanentemente).", name, id);
									else SendFormatNotification(playerid, "JUGADOR (Nombre: '%s' DB-ID: '%d') ya está baneado (temporalmente, fecha de readmisión: %s).", name, id, expire_date);
								}
								else
								{
									AddPlayerBan(id, name, ip, ACCOUNT_INFO[playerid][ac_ID], TYPE_BAN, reason);
									SendFormatNotification(playerid, "Jugador (nick: '%s' db_id: '%d') baneado.", name, id);
									
									new str[145]; format(str, 145, "{"#RED_COLOR"}[ADMIN] {FFFFFF}%s (%d) baneó a %s (offline, db_id: %d): %s", ACCOUNT_INFO[playerid][ac_NAME], playerid, name, id, reason);
									SendMessageToAdmins(-1, str);
								}
							}
						}
						mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM bans WHERE name = '%e' OR ip = '%e';", name, ip);
						MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
					}
				}
				else SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
			}
			else SendNotification(playerid, "No se encontro la DB-ID.");
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, ip, name, connected, playerid, admin_level FROM player WHERE id = %d;", to_account);
	MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	return 1;
}

CMD:dtban(playerid, params[])
{
	new reason[128], to_account, days;
	if(sscanf(params, "dds[128]", to_account, days, reason)) return SendNotification(playerid, "~r~Modo de uso:~w~ /dtban <DB-ID> <dias> <razon>");
	if(days <= 0 || days > 9999) return SendNotification(playerid, "~r~Modo de uso:~w~ /dtban <DB-ID> <dias> <razon>");
	
	inline OnInfoQueryLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			if(rows)
			{
				new id, ip[16], name[24], connected, pid, admin_level;
				cache_get_value_name_int(0, "id", id);
				cache_get_value_name(0, "ip", ip);
				cache_get_value_name(0, "name", name);
				cache_get_value_name_int(0, "connected", connected);
				cache_get_value_name_int(0, "playerid", pid);
				cache_get_value_name_int(0, "admin_level", admin_level);

				if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] >= admin_level)
				{
					if(connected) SendFormatNotification(playerid, "JUGADOR '%s' DB-ID '%d' conectado utilice /ban, su player_id: %d.", name, id, pid);
					else
					{
						inline OnCountQueryLoad()
						{
							new crows;
							if(cache_get_row_count(crows))
							{
								if(crows)
								{
									new expire_date[24];
									cache_get_value_name(0, "expire_date", expire_date);
									
									if(isnull(expire_date)) SendFormatNotification(playerid, "JUGADOR (Nombre: '%s' DB-ID: '%d') ya está baneado (permanentemente).", name, id);
									else SendFormatNotification(playerid, "JUGADOR (Nombre: '%s' DB-ID: '%d') ya está baneado (temporalmente, fecha de readmisión: %s).", name, id, expire_date);
								}
								else
								{
									AddPlayerBan(id, name, ip, ACCOUNT_INFO[playerid][ac_ID], TYPE_BAN, reason, days);
									SendFormatNotification(playerid, "Jugador (nick: '%s' dbid: '%d') baneado por %d días.", name, id, days);
					
									new str[145]; format(str, 145, "{"#RED_COLOR"}[ADMIN] {FFFFFF}%s (%d) baneó %d días a %s (offline, db_id: %d): %s", ACCOUNT_INFO[playerid][ac_NAME], playerid, days, name, id, reason);
									SendMessageToAdmins(-1, str);
								}
							}
						}
						mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM bans WHERE name = '%e' OR ip = '%e';", name, ip);
						MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
					}
				}
				else SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
			}
			else SendNotification(playerid, "No se encontro la DB-ID.");
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, ip, name, connected, playerid, admin_level FROM player WHERE id = %d;", to_account);
	MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	return 1;
}

CMD:rac(playerid, params[])
{
	RespawnGlobalUnoccupiedVehicles();
	SendNotification(playerid, "Todos los vehículos no personales y sin ocupar han sido re-spawneados.");
	
	SendCmdLogToAdmins(playerid, "rac", params);
	return 1;
}

CMD:rac2(playerid, params[])
{
	RespawnAllUnoccupiedVehicles();
	SendNotification(playerid, "Todos los vehículos sin ocupar han sido re-spawneados.");
	
	SendCmdLogToAdmins(playerid, "rac2", params);
	return 1;
}

CMD:rv(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if(!vehicleid) return SendNotification(playerid, "No estás en un vehículo.");
	
	SetVehicleToRespawnEx(vehicleid);
	SendFormatNotification(playerid, "Vehículo (%d) re-spawneado.", vehicleid);
	return 1;
}

CMD:pm(playerid, params[])
{
	new to_player, message[128];
	if(sscanf(params, "us[128]", to_player, message)) return SendNotification(playerid, "~r~Modo de uso:~w~ /pm <player_id> <mensaje>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	
	SendFormatNotification(playerid, "Mensaje enviado a %s (%d): %s", ACCOUNT_INFO[to_player][ac_NAME], to_player, message);
	
	PLAYER_TEMP[to_player][pt_ADMIN_PM_PID] = playerid;
	PLAYER_TEMP[to_player][pt_ADMIN_PM_AID] = ACCOUNT_INFO[playerid][ac_ID];
	PLAYER_TEMP[to_player][pt_ADMIN_PM_TIME] = gettime();
	SendClientMessageEx(to_player, -1, "{"#SILVER_COLOR"}Mensaje de un administrador (/r para responder): {FFFFFF}%s", message);
	
	SendCmdLogToAdmins(playerid, "pm", params);
	return 1;
}

CMD:deletead(playerid, params[])
{
	new ad_id;
	if(sscanf(params, "d", ad_id)) return SendNotification(playerid, "~r~Modo de uso:~w~ /deletead <ID>");
	
	inline OnShopChecked()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			if(rows)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM shop WHERE id = %d;", ad_id);
				mysql_tquery(srp_db, QUERY_BUFFER);
				SendFormatNotification(playerid, "Anuncio '%d' eliminado.", ad_id);
				SendCmdLogToAdmins(playerid, "deletead", params);
			}
			else SendNotification(playerid, "No se encontro la ID de ese anuncio.");
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM shop WHERE id = %d;", ad_id);
	MySQL_TQueryInline(srp_db, using inline OnShopChecked, QUERY_BUFFER);
	return 1;
}
alias:deletead("borrarad")

CMD:settime(playerid, params[])
{
	new hour, minute;
	if(sscanf(params, "dd", hour, minute)) return SendNotification(playerid, "~r~Modo de uso:~w~ /settime <hora> <minuto>");
	SetMyWorldTime(hour, minute);
	SendNotification(playerid, "El tiempo se ajustará cuando el reloj avance.");
	SendCmdLogToAdmins(playerid, "settime", params);
	return 1;
}

CMD:setweather(playerid, params[])
{
	new weather;
	if(sscanf(params, "d", weather)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setweather <ID>");
	InterpolateWeather(weather);
	SendCmdLogToAdmins(playerid, "setweather", params);
	return 1;
}

CMD:dameadmin(playerid, params[])
{
	if(!IsPlayerAdmin(playerid)) return SendNotification(playerid, "SERVER: Unknown command.");
	
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET admin_level = 5 WHERE id = %d;", ACCOUNT_INFO[playerid][ac_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	
	ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] = 5;
	SendNotification(playerid, "Ok.");
	return 1;
}

CMD:givemod(playerid, params[])
{
	new to_player, level;
	if(sscanf(params, "ud", to_player, level)) return SendNotification(playerid, "~r~Modo de uso:~w~ /givemod <player_id> <rango>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	if(level < 0 || level >= sizeof ADMIN_LEVELS) return SendNotification(playerid, "El rango no es válido.");
	if(level > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "No puedes dar este rango por ser un rango superior al tuyo.");
	
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET admin_level = %d WHERE id = %d;", level, ACCOUNT_INFO[to_player][ac_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	
	ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] = level;
	SendClientMessageEx(to_player, -1, "%s cambió tu rango administrativo a: %s.", ACCOUNT_INFO[playerid][ac_NAME], ADMIN_LEVELS[level]);
	SendFormatNotification(playerid, "El rango administrativo de %s (%d) ahora es %s.", ACCOUNT_INFO[to_player][ac_NAME], to_player, ADMIN_LEVELS[level]);
	
	SendCmdLogToAdmins(playerid, "givemod", params);
	return 1;
}
alias:givemod("staff")

CMD:ac(playerid, params[])
{
	ShowDialog(playerid, DIALOG_ANTI_CHEAT);
	return 1;
}

CMD:setthirst(playerid, params[])
{
	new to_player, Float:amount;
	if(sscanf(params, "uf", to_player, amount)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setthirst <player_id> <valor>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(amount < 0.0 || amount > 100.0) return SendNotification(playerid, "Cantidad no válida.");
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	PI[to_player][ac_THIRST] = amount;
	SendFormatNotification(playerid, "La hidratación de %s (%d) ahora es %.1f.", ACCOUNT_INFO[to_player][ac_NAME], to_player, amount);
	
	SendCmdLogToAdmins(playerid, "setthirst", params);
	return 1;
}
alias:setthirst("setsed")

CMD:sethunger(playerid, params[])
{
	new to_player, Float:amount;
	if(sscanf(params, "uf", to_player, amount)) return SendNotification(playerid, "~r~Modo de uso:~w~ /sethungry <player_id> <valor>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(amount < 0.0 || amount > 100.0) return SendNotification(playerid, "Cantidad no válida.");
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	PI[to_player][ac_HUNGRY] = amount;
	SendFormatNotification(playerid, "La alimentación de %s (%d) ahora es %.1f.", ACCOUNT_INFO[to_player][ac_NAME], to_player, amount);
	
	SendCmdLogToAdmins(playerid, "sethunger", params);
	return 1;
}
alias:sethunger("sethambre", "sethungry")

CMD:setgas(playerid, params[])
{
	new Float:val;
	if(sscanf(params, "f", val)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setgas <valor>");
	if(val < 0.0) return SendNotification(playerid, "Valor no válido.");
	
	new vehicleid = GetPlayerVehicleID(playerid);
	if(!vehicleid) return SendNotification(playerid, "No estás en un vehículo.");
	
	if(val > GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS]) val = GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS];
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] = val;
	
	SendFormatNotification(playerid, "La gasolina del vehículo (%d) ahora es %.1f.", vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS]);
	
	SendCmdLogToAdmins(playerid, "setgas", params);
	return 1;
}

CMD:repairveh(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if(!vehicleid) return SendNotification(playerid, "No estás en un vehículo.");
	
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) RepairVehicleEx(vehicleid, playerid);
	else {
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] != INVALID_PLAYER_ID) RepairVehicleEx(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER]);
		else RepairVehicleEx(vehicleid);
	}
	SendFormatNotification(playerid, "Vehículo (%d) reparado.", vehicleid);
	
	SendCmdLogToAdmins(playerid, "repairveh", params);
	return 1;
}
alias:repairveh("repararveh")

CMD:sethealth(playerid, params[])
{
	new to_player, Float:amount;
	if(sscanf(params, "uf", to_player, amount)) return SendNotification(playerid, "~r~Modo de uso:~w~ /sethealth <player_id> <valor>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(amount < 0.0 || amount > 100.0) return SendNotification(playerid, "Valor no válido.");
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	SetPlayerHealthEx(to_player, amount);
	SendFormatNotification(playerid, "La vida de %s (%d) ahora es %.1f.", ACCOUNT_INFO[to_player][ac_NAME], to_player, amount);
	
	SendCmdLogToAdmins(playerid, "sethealth", params);
	return 1;
}
alias:sethealth("setvida")

CMD:setarmour(playerid, params[])
{
	new to_player, Float:amount;
	if(sscanf(params, "uf", to_player, amount)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setarmour <player_id> <valor>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(amount < 0.0 || amount > 100.0) return SendNotification(playerid, "Valor no válido.");
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	SetPlayerArmourEx(to_player, amount);
	SendFormatNotification(playerid, "El chaleco de %s (%d) ahora es %.1f.", ACCOUNT_INFO[to_player][ac_NAME], to_player, amount);
	
	SendCmdLogToAdmins(playerid, "setarmour", params);
	return 1;
}
alias:setarmour("setchaleco")

CMD:setlevel(playerid, params[])
{
	new to_player, level;
	if(sscanf(params, "ud", to_player, level)) return SendNotification(playerid, "~r~Modo de uso:~w~ /level <player_id> <nivel>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	KillTimer(PLAYER_TEMP[to_player][pt_TIMERS][2]);
	
	ACCOUNT_INFO[to_player][ac_REP] = 1;
	ACCOUNT_INFO[to_player][ac_LEVEL] = level;
	UpdateReputationTextDraws(to_player);
	SetPlayerSkillLevels(to_player);
	
	SendFormatNotification(to_player, "~g~¡Felicidades! ~w~Has subido al nivel %d.", ACCOUNT_INFO[to_player][ac_LEVEL]);
	SetPlayerScore(to_player, ACCOUNT_INFO[to_player][ac_LEVEL]);
	PlayerPlaySoundEx(to_player, 1058, 0.0, 0.0, 0.0);
	
	ACCOUNT_INFO[to_player][ac_TIME_FOR_REP] = TIME_FOR_REP;
	PLAYER_TEMP[to_player][pt_TIME_PASSED_LAST_REP] = gettime() * 1000;
	
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET time_playing = %d, level = %d, rep = %d, time_for_rep = %d, payday_rep = %d WHERE id = %d;", ACCOUNT_INFO[to_player][ac_TIME_PLAYING], ACCOUNT_INFO[to_player][ac_LEVEL], ACCOUNT_INFO[to_player][ac_REP], TIME_FOR_REP, ACCOUNT_INFO[to_player][ac_PAYDAY_REP], ACCOUNT_INFO[to_player][ac_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	
	KillTimer(PLAYER_TEMP[to_player][pt_TIMERS][2]);
	PLAYER_TEMP[to_player][pt_TIMERS][2] = SetTimerEx("AddPlayerReputation", ACCOUNT_INFO[to_player][ac_TIME_FOR_REP], false, "i", to_player);
	
	SendFormatNotification(playerid, "El nivel de %s (%d) ahora es %d.", ACCOUNT_INFO[to_player][ac_NAME], to_player, ACCOUNT_INFO[to_player][ac_LEVEL]);
	
	SendCmdLogToAdmins(playerid, "setlevel", params);
	return 1;
}

CMD:setwork(playerid, params[])
{
	new to_player, work, set;
	if(sscanf(params, "udd", to_player, work, set)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setwork <player_id> <work> <set>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	if(work < 0 || work >= sizeof work_info) return SendNotification(playerid, "El trabajo no es válido, para verlos usa /works.");
	
	if(PLAYER_TEMP[to_player][pt_WORKING_IN]) return SendNotification(playerid, "No se puede cambiar el trabajo del jugador porque está de servicio en su trabajo.");
	
	
	new player_jobs = CountPlayerJobs(to_player);
	if(ACCOUNT_INFO[to_player][ac_VIP])
	{
		if(player_jobs >= MAX_SU_WORKS && set)
		{
			SendFormatNotification(playerid, "El limite de trabajos para este jugador es %d.", player_jobs);
			return 1;
		}
	}
	else
	{
		if(player_jobs >= MAX_NU_WORKS && set)
		{
			SendFormatNotification(playerid, "El limite de trabajos para este jugador es %d.", player_jobs);
			return 1;
		}
	}

	if(set)
	{
		if(work == WORK_POLICE)
		{
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO pworks (id_player, id_work, `set`, level) VALUES(%d, %d, 1, 1) ON DUPLICATE KEY UPDATE `set` = 1, level = 1;", ACCOUNT_INFO[to_player][ac_ID], work);
			mysql_tquery(srp_db, QUERY_BUFFER);

			PI[to_player][ac_PLACA_PD] = random(10000000);
			PLAYER_WORKS[to_player][work][pwork_LEVEL] = 1;
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET placa_pd = %d WHERE id = %d;", PI[to_player][ac_PLACA_PD], ACCOUNT_INFO[to_player][ac_ID]);
			mysql_tquery(srp_db, QUERY_BUFFER);
		}
		else
		{
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO pworks (id_player, id_work, `set`, level) VALUES(%d, %d, 1, 1) ON DUPLICATE KEY UPDATE `set` = 1;", ACCOUNT_INFO[to_player][ac_ID], work);
			mysql_tquery(srp_db, QUERY_BUFFER);
		}
	
		PLAYER_WORKS[to_player][work][pwork_SET] = 1;
		SendClientMessageEx(to_player, -1, "{"#PRIMARY_COLOR"}¡Felicidades! {FFFFFF}Has conseguido tu trabajo de %s.", work_info[work][work_info_NAME]);
		SendClientMessage(to_player, -1, "Puedes utilizar {"#PRIMARY_COLOR"}/ayuda trabajos {FFFFFF}para ver como trabajar aquí.");
		
	}
	else
	{
		if(work == WORK_POLICE)
		{
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pworks SET `set` = 0, level = 0 WHERE id_player = %d AND id_work = %d;", ACCOUNT_INFO[to_player][ac_ID], work);
			mysql_tquery(srp_db, QUERY_BUFFER);

			PI[to_player][ac_PLACA_PD] = 0;
			PLAYER_WORKS[to_player][work][pwork_LEVEL] = 0;
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET placa_pd = %d WHERE id = %d;", PI[to_player][ac_PLACA_PD], ACCOUNT_INFO[to_player][ac_ID]);
			mysql_tquery(srp_db, QUERY_BUFFER);
		}
		else
		{
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pworks SET `set` = 0 WHERE id_player = %d AND id_work = %d;", ACCOUNT_INFO[to_player][ac_ID], work);
			mysql_tquery(srp_db, QUERY_BUFFER);
		}

		PLAYER_WORKS[to_player][work][pwork_SET] = 0;
		SendClientMessageEx(to_player, -1, "{"#SILVER_COLOR"}Has abandonado tu trabajo de %s.", work_info[work][work_info_NAME]);
	}

	SendFormatNotification(playerid, "El trabajo %s de %s (%d) es: '%d'.", work_info[ work ][work_info_NAME], ACCOUNT_INFO[to_player][ac_NAME], to_player, set);
	SendCmdLogToAdmins(playerid, "setwork", params);
	return 1;
}

CMD:setworkexp(playerid, params[])
{
	new to_player, work, exp;
	if(sscanf(params, "udd", to_player, work, exp)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setworkexp <player_id> <work, para verlos /works> <exp>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	if(work < 0 || work >= sizeof work_info) return SendNotification(playerid, "El trabajo no es válido, para verlos usa /works.");
	
	if(PLAYER_WORKS[to_player][ work ][pwork_SET])
	{
		mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pworks SET level = %d WHERE id_player = %d AND id_work = %d;", exp, ACCOUNT_INFO[to_player][ac_ID], work);
		mysql_tquery(srp_db, QUERY_BUFFER);

		PLAYER_WORKS[to_player][ work ][pwork_LEVEL] = exp;
		SendFormatNotification(playerid, "La experiencia de %s (%d) en el trabajo '%s' ahora es %d.", ACCOUNT_INFO[to_player][ac_NAME], to_player, work_info[ work ][work_info_NAME], exp);
		SendCmdLogToAdmins(playerid, "setworkexp", params);
	}
	else SendNotification(playerid, "El jugador no tiene el trabajo");
	return 1;
}

CMD:setcash(playerid, params[])
{
	new to_player, value;
	if(sscanf(params, "ud", to_player, value)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setcash <player_id> <amount>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	SetPlayerCash(to_player, value);
	SendFormatNotification(playerid, "El dinero de %s (%d) ahora es %d.", ACCOUNT_INFO[to_player][ac_NAME], to_player, value);
	
	SendCmdLogToAdmins(playerid, "setcash", params);
	return 1;
}

CMD:givecash(playerid, params[])
{
	new to_player, value;
	if(sscanf(params, "ud", to_player, value)) return SendNotification(playerid, "~r~Modo de uso:~w~ /givecash <player_id> <amount>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	GivePlayerCash(to_player, value, true, value < 0 ? true : false);
	SendFormatNotification(playerid, "El jugador %s (%d) ha recibido %d.", ACCOUNT_INFO[to_player][ac_NAME], to_player, value);
	
	SendCmdLogToAdmins(playerid, "givecash", params);
	return 1;
}

CMD:setbmlevel(playerid, params[])
{
	new to_player, value;
	if(sscanf(params, "ud", to_player, value)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setbmlevel <player_id> <level>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(ACCOUNT_INFO[to_player][ac_ADMIN_LEVEL] > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) return SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	PI[to_player][ac_BLACK_MARKET_LEVEL] = value;
	SendFormatNotification(playerid, "El nivel 'black_market' de %s (%d) ahora es %d.", ACCOUNT_INFO[to_player][ac_NAME], to_player, value);
	
	SendCmdLogToAdmins(playerid, "setbmlevel", params);
	return 1;
}

CMD:asay(playerid, params[])
{
	if(isnull(params)) return SendNotification(playerid, "~r~Modo de uso:~w~ /asay <message>");
	new str[145]; format(str, 145, "* Admin: %s", params);
	SendClientMessageToAll(0x2587CEFF, str);

	SendCmdLogToAdmins(playerid, "asay", params);
	return 1;
}
alias:asay("global")

CMD:spos(playerid, params[])
{
	new Float:p[4], interior, vw;
	if(sscanf(params, "p<,>ffffdd", p[0], p[1], p[2], p[3], interior, vw)) return SendNotification(playerid, "~r~Modo de uso:~w~ /spos [X], [Y], [Z], [ANGLE], [INTERIOR], [VIRTUAL WORLD]");

	SetPlayerPosEx(playerid, p[0], p[1], p[2], p[3], interior, vw);
	return 1;
}



CMD:payday(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /payday [Player/Name]");
	PlayerPayday(to_player);
	return 1;
}

CMD:v(playerid, params[])
{
	new modelid, vehname[24], color1 = 1, color2 = 1;
	if(!sscanf(params, "ddd", modelid, color1, color2)) { }
	else if(!sscanf(params, "d", modelid)) { }
	else if(!sscanf(params, "s[24]dd", vehname, color1, color2)) { modelid = GetVehicleModelByName(vehname); }
	else if(!sscanf(params, "s[24]", vehname)) { modelid = GetVehicleModelByName(vehname); }
	else return SendNotification(playerid, "~r~Modo de uso:~w~ /v <modelid/name> <color 1 = 1> <color 2 = 1>");
	
	if(modelid < 400 || modelid > 611) return SendNotification(playerid, "Modelo de vehículo no válido.");
		
	new Float:p[4];
	GetPlayerPos(playerid, p[0], p[1], p[2]);
	GetPlayerFacingAngle(playerid, p[3]);
	
	new vid = AddTestVehicle(false, modelid, p[0], p[1], p[2], p[3], color1, color2, GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid));
	PutPlayerInVehicleEx(playerid, vid, 0);

	GLOBAL_VEHICLES[vid][gb_vehicle_PARAMS_ENGINE] = 1;
	UpdateVehicleParams(vid);
	
	SendCmdLogToAdmins(playerid, "v", params);
	return 1;
}

CMD:dv(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if(!vehicleid) return SendNotification(playerid, "Error: no estás en ningún vehículo de prueba.");
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] != VEHICLE_TYPE_TEST) return SendNotification(playerid, "Error: no estás en ningún vehículo de prueba.");
	
	DestroyVehicleEx(vehicleid);
	SendNotification(playerid, "Vehículo de prueba destruido.");
	return 1;
}

CMD:restart(playerid, params[])
{
	mysql_tquery(srp_db, "UPDATE player SET connected = 0, playerid = 0;");
	SendRconCommand("gmx");
	return 1;
}

CMD:nombre(playerid, params[])
{
	if(CHANGE_NAME_SD_PRICE > PI[playerid][ac_COINS])
	{
		SendFormatNotification(playerid, "Te faltan %d "SERVER_COIN" para poder cambiarte el nombre.", CHANGE_NAME_SD_PRICE - PI[playerid][ac_COINS]);
		return 1;
	}

	ShowDialog(playerid, DIALOG_CHANGE_NAME);
	return 1;
}
alias:nombre("cname")

CMD:correo(playerid, params[])
{
	ShowDialog(playerid, DIALOG_CHANGE_EMAIL);
	return 1;
}
alias:correo("email", "mail")

CMD:setnametemp(playerid, params[])
{
	new to_player, new_name[24];
	if(sscanf(params, "us[24]", to_player, new_name)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setnametemp <player_id> <nuevo nombre>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);

	if(SetPlayerName(to_player, new_name) == 1) 
	{
		for(new i = 0; i < 24; i++) 
		{
			if(new_name[i] == '_')
			{
				new_name[i] = ' ';
			}
		}

		format(PLAYER_TEMP[to_player][pt_RP_NAME], 24, "%s", new_name);
		SendNotification(playerid, "Nombre cambiado temporalmente.");
	}
	else SendNotification(playerid, "Nombre no válido.");
	return 1;
}

CMD:setnameplayer(playerid, params[])
{
	new to_player;
	if(sscanf(params, "u", to_player)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setnameplayer <player_id>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);

	SetRolePlayNames(to_player);
	SetPlayerName(to_player, ACCOUNT_INFO[to_player][ac_NAME]);
	SendNotification(playerid, "Nombre cambiado.");
	return 1;
}

CMD:setname(playerid, params[])
{
	new to_player, new_name[24];
	if(sscanf(params, "us[24]", to_player, new_name)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setname <player_id> <nuevo nombre>");
	if(!IsPlayerConnected(to_player)) return SendFormatNotification(playerid, "Jugador (%d) desconectado", to_player);
	if(!IsValidRPName(new_name)) return SendFormatNotification(playerid, "El nombre '%s' no cumple con el formato Nombre_Apellido.", new_name);
	
	inline OnInfoQueryLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			if(rows) SendFormatNotification(playerid, "El nombre '%s' está en uso.", new_name);
			else
			{
				format(PLAYER_TEMP[to_player][pt_NAME], 24, "%s", new_name);
				format(ACCOUNT_INFO[to_player][ac_NAME], 24, "%s", new_name);
				SetRolePlayNames(to_player);
				SetPlayerName(to_player, new_name);
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET name = '%e' WHERE id = %d;", new_name, ACCOUNT_INFO[to_player][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				for(new i = 0; i != MAX_PROPERTIES; i ++)
				{
					if(!PROPERTY_INFO[i][property_VALID]) continue;
					if(PROPERTY_INFO[i][property_OWNER_ID] != ACCOUNT_INFO[to_player][ac_ID]) continue;
					
					new label_str[256];
					format
					(
						label_str, 
						sizeof label_str, 
						"\
							Propiedad {"#PRIMARY_COLOR"}#%d\n\n\
							{FFFFFF}Propietario: {"#PRIMARY_COLOR"}%s\n\
							{FFFFFF}Presiona {"#PRIMARY_COLOR"}~k~~CONVERSATION_YES~ {FFFFFF}para entrar.\
						", PROPERTY_INFO[i][property_ID], ACCOUNT_INFO[to_player][ac_NAME]
					);
					UpdateDynamic3DTextLabelText(PROPERTY_INFO[i][property_EXT_LABEL_ID], 0xFFFFFFFF, label_str);
				}
				
				SendClientMessageEx(to_player, -1, "{"#SILVER_COLOR"}Tu nombre ha sido cambiado a '%s'", new_name);
				SendFormatNotification(playerid, "El nombre del jugador ha sido cambiado a '%s'", new_name);
				
				SendCmdLogToAdmins(playerid, "setname", params);
			}
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM player WHERE name = '%e';", new_name);
	MySQL_TQueryInline(srp_db,  using inline OnInfoQueryLoad, QUERY_BUFFER, "s", params);
	return 1;
}

CMD:exproperty(playerid, params[])
{
	new id_property;
	if(sscanf(params, "d", id_property)) return SendNotification(playerid, "~r~Modo de uso:~w~ /exproperty <id>");
	if(id_property <= 0) return SendNotification(playerid, "~r~Modo de uso:~w~ /exproperty < id > 0 >");
	
	new index = GetPropertyIndexByID(id_property);
	if(index == -1) return SendNotification(playerid, "PROPIEDAD ID no encontrada.");
	
	if(!PROPERTY_INFO[index][property_SOLD]) return SendNotification(playerid, "Esta propiedad ya está en venta.");
	
	
	PROPERTY_INFO[ index ][property_SOLD] = false;
	PROPERTY_INFO[ index ][property_CREW] = false;
	PROPERTY_INFO[ index ][property_OWNER_ID] = 0;
	PROPERTY_INFO[ index ][property_CREW_ID] = 0;
	format(PROPERTY_INFO[ index ][property_NAME], 24, "PROPIEDAD %d", PROPERTY_INFO[ index ][property_ID]);
	
	new info[3];
	UpdateUnnocupiedPropertyLabel(index);
	
	DestroyDynamicPickup(PROPERTY_INFO[ index ][property_EXT_PICKUP_ID]);
	PROPERTY_INFO[ index ][property_EXT_PICKUP_ID] = INVALID_STREAMER_ID;
	
	PROPERTY_INFO[ index ][property_EXT_PICKUP_ID] = CreateDynamicPickup(1273, 1, PROPERTY_INFO[ index ][property_EXT_X], PROPERTY_INFO[ index ][property_EXT_Y], PROPERTY_INFO[ index ][property_EXT_Z], 0, PROPERTY_INFO[ index ][property_EXT_INTERIOR]);
	info[0] = PICKUP_TYPE_PROPERTY;
	info[1] = index; // Index
	info[2] = 2; // Pickup Exterior
	Streamer_SetArrayData(STREAMER_TYPE_PICKUP, PROPERTY_INFO[ index ][property_EXT_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
				
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE properties SET id_player = NULL, id_territory = NULL WHERE id = %d;", PROPERTY_INFO[ index ][property_ID]);
	mysql_tquery(srp_db, QUERY_BUFFER);
	SendFormatNotification(playerid, "Propiedad (%d) expropiada.", PROPERTY_INFO[ index ][property_ID]);
	
	SendCmdLogToAdmins(playerid, "exproperty", params);
	return 1;
}

CMD:gotoproperty(playerid, params[])
{
	new int_type;
	if(sscanf(params, "d", int_type)) return SendNotification(playerid, "~r~Modo de uso:~w~ /gotoproperty <interior>");
	if(int_type < 0 || int_type >= sizeof PROPERTY_INTERIORS) return SendFormatNotification(playerid, "Error, rango de interior: 0-%d", sizeof(PROPERTY_INTERIORS) - 1);

	SetPlayerPosEx(playerid, PROPERTY_INTERIORS[int_type][property_INT_X], PROPERTY_INTERIORS[int_type][property_INT_Y], PROPERTY_INTERIORS[int_type][property_INT_Z], PROPERTY_INTERIORS[int_type][property_INT_ANGLE], PROPERTY_INTERIORS[int_type][property_INT_INTERIOR], 0, false, true);
	return 1;
}
alias:gotoproperty("ircasa")

CMD:setpass(playerid, params[])
{
	new to_account, new_pass[MAX_PASS_LENGTH + 1];
	if(sscanf(params, "ds[19]", to_account, new_pass)) return SendNotification(playerid, "~r~Modo de uso:~w~ /setpass <DB-ID> <pass>");
	
	inline OnInfoQueryLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			if(rows)
			{
				new id, name[24], connected, pid, admin_level;
				cache_get_value_name_int(0, "id", id);
				cache_get_value_name(0, "name", name);
				cache_get_value_name_int(0, "connected", connected);
				cache_get_value_name_int(0, "playerid", pid);
				cache_get_value_name_int(0, "admin_level", admin_level);
				if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] >= admin_level)
				{
					if(connected) SendFormatNotification(playerid, "JUGADOR '%s' DB-ID '%d' conectado, player_id: %d, no es necario cambiar la contraseña.", name, id, pid);
					else
					{
						new salt[16], salt_ex[16], pass_ex[64 + 1];
						getRandomSalt(salt);
						format(salt_ex, 16, "%s", salt);
						SHA256_PassHash(new_pass, salt_ex, pass_ex, 64 + 1);
						
						mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET salt = '%e', pass = '%e' WHERE id = %d;", salt_ex, pass_ex, id);
						mysql_tquery(srp_db, QUERY_BUFFER);
						
						SendFormatNotification(playerid, "La contraseña de '%s' ahora es: %s", name, new_pass);
					}
				}
				else SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
			}
			else SendNotification(playerid, "No se encontro la DB-ID.");
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, name, connected, playerid, admin_level FROM player WHERE id = %d;", to_account);
	MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	return 1;
}

CMD:delete(playerid, params[])
{
	new to_account;
	if(sscanf(params, "d", to_account)) return SendNotification(playerid, "~r~Modo de uso:~w~ /delete <DB-ID>");
	
	inline OnInfoQueryLoad()
	{
		new rows;
		if(cache_get_row_count(rows))
		{
			if(rows)
			{
				new id, ip[16], name[24], connected, pid, admin_level;
				cache_get_value_name_int(0, "id", id);
				cache_get_value_name(0, "ip", ip);
				cache_get_value_name(0, "name", name);
				cache_get_value_name_int(0, "connected", connected);
				cache_get_value_name_int(0, "playerid", pid);
				cache_get_value_name_int(0, "admin_level", admin_level);

				if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] >= admin_level)
				{
					if(connected) SendFormatNotification(playerid, "JUGADOR '%s' DB-ID '%d' conectado para eliminarlo debe estar desconectado, utilice kick, su player_id: %d.", name, id, pid);
					else
					{
						mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM player WHERE id = %d;", id);
						mysql_tquery(srp_db, QUERY_BUFFER);
						
						new ex_properties;
						for(new index = 0; index != MAX_PROPERTIES; index ++)
						{
							if(!PROPERTY_INFO[index][property_VALID]) continue;
							if(!PROPERTY_INFO[index][property_SOLD]) continue;
							if(PROPERTY_INFO[index][property_OWNER_ID] != id) continue;
						
							PROPERTY_INFO[ index ][property_SOLD] = false;
							PROPERTY_INFO[ index ][property_OWNER_ID] = 0;
							format(PROPERTY_INFO[ index ][property_NAME], 24, "PROPIEDAD %d", PROPERTY_INFO[ index ][property_ID]);
							
							new info[3];
							UpdateUnnocupiedPropertyLabel(index);
							
							DestroyDynamicPickup(PROPERTY_INFO[ index ][property_EXT_PICKUP_ID]);
							PROPERTY_INFO[ index ][property_EXT_PICKUP_ID] = INVALID_STREAMER_ID;
							
							PROPERTY_INFO[ index ][property_EXT_PICKUP_ID] = CreateDynamicPickup(1273, 1, PROPERTY_INFO[ index ][property_EXT_X], PROPERTY_INFO[ index ][property_EXT_Y], PROPERTY_INFO[ index ][property_EXT_Z], 0, PROPERTY_INFO[ index ][property_EXT_INTERIOR]);
							info[0] = PICKUP_TYPE_PROPERTY;
							info[1] = index; // Index
							info[2] = 2; // Pickup Exterior
							Streamer_SetArrayData(STREAMER_TYPE_PICKUP, PROPERTY_INFO[ index ][property_EXT_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
							
							ex_properties ++;
						}
						
						
						SendFormatNotification(playerid, "CUENTA (Nombre '%s' DB-ID: '%d') ha sido eliminada.", name, id);
						if(ex_properties > 0) SendFormatNotification(playerid, "Se han expropiado '%d' propiedades del jugador eliminado.", ex_properties);
						
						new str[145]; format(str, 145, "{"#RED_COLOR"}[ADMIN] {FFFFFF}%s (%d) ha eliminado el usuario '%s'", ACCOUNT_INFO[playerid][ac_NAME], playerid, name);
						SendMessageToAdmins(-1, str);
					}
				}
				else SendNotification(playerid, "El rango administrativo de este jugador es superior al tuyo.");
			}
			else SendNotification(playerid, "No se encontro la DB-ID.");
		}
	}
	mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, ip, name, connected, playerid, admin_level FROM player WHERE id = %d;", to_account);
	MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
	return 1;
}

