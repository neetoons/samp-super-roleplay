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
#if defined response_included 
	#endinput
#endif
#define response_included

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(PLAYER_TEMP[playerid][pt_DIALOG_RESPONDED] || PLAYER_TEMP[playerid][pt_DIALOG_ID] != dialogid) return Y_HOOKS_BREAK_RETURN_0;
	PLAYER_TEMP[playerid][pt_DIALOG_RESPONDED] = true;
	PLAYER_TEMP[playerid][pt_DIALOG_ID] = -1;
 
	if(!isnull(inputtext))
	{
		for(new i = 0; i != strlen(inputtext); i ++)
		{
			if(inputtext[i] == '%') inputtext[i] = '#';
		}
	}
	
	switch(dialogid)
	{
		case DIALOG_INFO: return 1;
		case DIALOG_REGISTER:
		{
			if(response)
			{
				if(strlen(inputtext) < MIN_PASS_LENGTH || strlen(inputtext) > MAX_PASS_LENGTH) return ShowDialog(playerid, dialogid);
				format(PLAYER_TEMP[playerid][pt_PASSWD], 24, "%s", inputtext);
				
				new salt[16];
				getRandomSalt(salt);
				format(ACCOUNT_INFO[playerid][ac_SALT], 16, "%s", salt);
				SHA256_PassHash(inputtext, ACCOUNT_INFO[playerid][ac_SALT], ACCOUNT_INFO[playerid][ac_PASS], 64 + 1);
				ShowDialog(playerid, DIALOG_REGISTER_EMAIL);
			}
			else Kick(playerid);
			return 1;
		}
		case DIALOG_REGISTER_EMAIL:
		{
			if(!response) return Kick(playerid);

			new mail[32];
			format(mail, 32, "%s", inputtext);
			if(!IsValidEmail(mail))
			{
				SendNotification(playerid, "Correo no válido.");
				ShowDialog(playerid, DIALOG_REGISTER_EMAIL);
				return 1;	
			}

			inline OnCheckEmail()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					if(rows)
					{
						SendNotification(playerid, "Este correco electrónico ya está en uso, use otro distinto.");
						ShowDialog(playerid, dialogid);
					}
					else
					{
						format(ACCOUNT_INFO[playerid][ac_EMAIL], 32, "%s", mail);
			
						ApplyActorAnimation(PLAYER_TEMP[playerid][pt_REGISTER_ACTOR], "INT_SHOP", "shop_loop", 4.1, 1, 1, 1, 0, 0);
						PLAYER_TEMP[playerid][pt_READING_GUIDE] = true;
						PLAYER_TEMP[playerid][pt_GUIDE_STEP] = 0;
						SendNotification(playerid, "Lee detenidamente la guía, para pasar de concepto haz clic en siguiente.");
						SetPlayerRulesTextDraw(playerid, Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Title], Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Info], Server_Rules[ PLAYER_TEMP[playerid][pt_GUIDE_STEP] ][Rule_Pages], PLAYER_TEMP[playerid][pt_GUIDE_STEP] + 1, sizeof(Server_Rules) );
						SelectTextDrawEx(playerid, 0xe8d08fFF);
					}
				}
				else Kick(playerid);
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM player WHERE email = '%e';", mail);
			MySQL_TQueryInline(srp_db, using inline OnCheckEmail, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_LOGIN:
		{
			if(!response) return Kick(playerid);
			if(!strlen(inputtext)) return ShowDialog(playerid, dialogid);
			
			new password[64 + 1];
			SHA256_PassHash(inputtext, ACCOUNT_INFO[playerid][ac_SALT], password, sizeof password);
			if(!strcmp(password, ACCOUNT_INFO[playerid][ac_PASS], false))
			{
				inline OnPlayerDataLoad()
				{
					new rows;
					if(cache_get_row_count(rows))
					{
						if(rows)
						{
							new bool:isnull_crew;
							cache_get_value_name(0, "reg_date", ACCOUNT_INFO[playerid][ac_REG_DATE], 24);
							cache_get_value_name(0, "last_connection", ACCOUNT_INFO[playerid][ac_LAST_CONNECTION], 24);
							cache_get_value_name_int(0, "last_connection_timestamp", ACCOUNT_INFO[playerid][ac_LAST_CONNECTION_TIMESTAMP]);
							cache_get_value_name_int(0, "time_playing", ACCOUNT_INFO[playerid][ac_TIME_PLAYING]);
							cache_get_value_name_int(0, "level", ACCOUNT_INFO[playerid][ac_LEVEL]);
							cache_get_value_name_int(0, "rep", ACCOUNT_INFO[playerid][ac_REP]);
							cache_get_value_name_int(0, "doubt_channel", ACCOUNT_INFO[playerid][ac_DOUBT_CHANNEL]);
							cache_get_value_name_int(0, "time_for_rep", ACCOUNT_INFO[playerid][ac_TIME_FOR_REP]);
							cache_get_value_name_int(0, "admin_level", ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]);
							cache_get_value_name_int(0, "payday_rep", ACCOUNT_INFO[playerid][ac_PAYDAY_REP]);
							cache_get_value_name_int(0, "vip", ACCOUNT_INFO[playerid][ac_VIP]);
							cache_get_value_name(0, "vip_expire_date", ACCOUNT_INFO[playerid][ac_VIP_EXPIRE_DATE], 24);
							cache_get_value_name_int(0, "coins", PI[playerid][ac_COINS]);
							cache_get_value_name_int(0, "skin", PI[playerid][ac_SKIN]);
							cache_get_value_name_int(0, "cash", PI[playerid][ac_CASH]);
							cache_get_value_name_float(0, "pos_x", PI[playerid][ac_POS_X]);
							cache_get_value_name_float(0, "pos_y", PI[playerid][ac_POS_Y]);
							cache_get_value_name_float(0, "pos_z", PI[playerid][ac_POS_Z]);
							cache_get_value_name_float(0, "angle", PI[playerid][ac_ANGLE]);
							cache_get_value_name_int(0, "state", ACCOUNT_INFO[playerid][ac_STATE]);
							cache_get_value_name_int(0, "interior", PI[playerid][ac_INTERIOR]);
							cache_get_value_name_int(0, "local_interior", PI[playerid][ac_LOCAL_INTERIOR]);
							cache_get_value_name_int(0, "fight_style", PI[playerid][ac_FIGHT_STYLE]);
							cache_get_value_name_float(0, "health", PI[playerid][ac_HEALTH]);
							cache_get_value_name_float(0, "armour", PI[playerid][ac_ARMOUR]);
							cache_get_value_name_int(0, "gender", PI[playerid][ac_GENDER]);
							cache_get_value_name_float(0, "hungry", PI[playerid][ac_HUNGRY]);
							cache_get_value_name_float(0, "thirst", PI[playerid][ac_THIRST]);
							cache_get_value_name_int(0, "black_market_level", PI[playerid][ac_BLACK_MARKET_LEVEL]);
							cache_get_value_name_int(0, "wanted_level", PI[playerid][ac_WANTED_LEVEL]);
							cache_get_value_name_int(0, "police_jail_time", PI[playerid][ac_POLICE_JAIL_TIME]);
							cache_get_value_name_int(0, "police_duty", PI[playerid][ac_POLICE_DUTY]);
							cache_get_value_name_int(0, "police_jail_id", PI[playerid][ac_POLICE_JAIL_ID]);
							cache_get_value_name_int(0, "drive_license_points", PI[playerid][ac_DRIVE_LICENSE_POINTS]);
							cache_get_value_name_int(0, "bank_account", PI[playerid][ac_BANK_ACCOUNT]);
							cache_get_value_name_int(0, "bank_money", PI[playerid][ac_BANK_MONEY]);
							cache_get_value_name_int(0, "phone_number", PI[playerid][ac_PHONE_NUMBER]);
							cache_get_value_name_int(0, "phone_state", PI[playerid][ac_PHONE_STATE]);
							cache_get_value_name_int(0, "phone_visible_number", PI[playerid][ac_PHONE_VISIBLE_NUMBER]);
							cache_get_value_name_int(0, "gps", PI[playerid][ac_GPS]);
							cache_get_value_name_int(0, "maso", PI[playerid][ac_Maso]);
							cache_get_value_name_int(0, "mp3", PI[playerid][ac_MP3]);
							cache_get_value_name_int(0, "phone_resolver", PI[playerid][ac_PHONE_RESOLVER]);
							cache_get_value_name_int(0, "speakers", PI[playerid][ac_SPEAKERS]);
							cache_get_value_name_int(0, "mechanic_pieces", PI[playerid][ac_MECHANIC_PIECES]);
							cache_get_value_name_int(0, "fuel_drum", PI[playerid][ac_FUEL_DRUM]);
							cache_get_value_name_int(0, "seed_medicine", PI[playerid][ac_SEED_MEDICINE]);
							cache_get_value_name_int(0, "seed_cannabis", PI[playerid][ac_SEED_CANNABIS]);
							cache_get_value_name_int(0, "seed_crack", PI[playerid][ac_SEED_CRACK]);
							cache_get_value_name_int(0, "medicine", PI[playerid][ac_MEDICINE]);
							cache_get_value_name_int(0, "cannabis", PI[playerid][ac_CANNABIS]);
							cache_get_value_name_int(0, "crack", PI[playerid][ac_CRACK]);
							cache_get_value_name_int(0, "config_sounds", PI[playerid][ac_CONFIG_SOUNDS]);
							cache_get_value_name_int(0, "config_audio", PI[playerid][ac_CONFIG_AUDIO]);
							cache_get_value_name_int(0, "config_time", PI[playerid][ac_CONFIG_TIME]);
							cache_get_value_name_int(0, "config_hud", PI[playerid][ac_CONFIG_HUD]);
							cache_get_value_name_int(0, "config_admin", PI[playerid][ac_CONFIG_ADMIN]);
							cache_get_value_name_int(0, "config_secure_login", PI[playerid][ac_CONFIG_SECURE_LOGIN]);
							cache_get_value_name_int(0, "mute", PI[playerid][ac_MUTE]);
							cache_get_value_name_int(0, "placa_pd", PI[playerid][ac_PLACA_PD]);
							cache_get_value_name_int(0, "can_buy_bm", PI[playerid][ac_CAN_BUY_BM]);
							cache_is_value_name_null(0, "crew", isnull_crew);
							if(!isnull_crew) cache_get_value_name_int(0, "crew", PI[playerid][ac_CREW]);
							cache_get_value_name_int(0, "crew_rank", PI[playerid][ac_CREW_RANK]);
							cache_get_value_name_int(0, "mechanic_kits", PI[playerid][ac_MECHANIC_KITS]);
							cache_get_value_name_int(0, "medical_kits", PI[playerid][ac_MEDICAL_KITS]);
							CallLocalFunction("OnPlayerLogin", "i", playerid);
						}
						else Kick(playerid);
					}
					else Kick(playerid);
				}
				GameTextForPlayer(playerid, "~y~cargando...", 10000, 3);
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM player WHERE id = %d;", ACCOUNT_INFO[playerid][ac_ID]);
				MySQL_TQueryInline(srp_db, using inline OnPlayerDataLoad, QUERY_BUFFER);
			}
			else // Error
			{
				PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] ++;
				if(PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] > MAX_BAD_LOGIN_ATTEMPS) return Kick(playerid);
				ShowDialog(playerid, dialogid);
			}
			return 1;
		}
		case DIALOG_SECURE_LOGIN_INSERT_CODE: {
			if(!response) return Kick(playerid);
			if(!strlen(inputtext)) return ShowDialog(playerid, dialogid);

			new string[128];
			format(string, sizeof string, "id=%d&code=%s", ACCOUNT_INFO[playerid][ac_ID], SpaceFix(inputtext));
			HTTP(playerid, HTTP_POST, SECURE_LOGIN_CHECK_URL, string, "OnSecureLoginCheckCode");
		}
		case DIALOG_CLOTHES:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] == -1 && ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] == INTERIOR_NO_INFO) return 1;
				
				switch(listitem)
				{
					case 0:
					{
						PC_EmulateCommand(playerid, "/ropa");
					}
					case 1:
					{
						PC_EmulateCommand(playerid, "/toys");
					}
				}
			}
			return 1;
		}
		case DIALOG_FOOD_PIZZA:
		{
			if(response)
			{
				if(PI[playerid][ac_CASH] >= PIZZA_FOOD[listitem][food_PRICE])
				{
					if(PI[playerid][ac_HUNGRY] >= 99.0 && PI[playerid][ac_THIRST] >= 99.0) return SendNotification(playerid, "¿Es que quieres reventar?");
					
					if(GivePlayerCash(playerid, -PIZZA_FOOD[listitem][food_PRICE], true, true)) {
						Add_Hungry_Thirst(playerid, PIZZA_FOOD[listitem][food_HUNGRY], PIZZA_FOOD[listitem][food_THIRST]);
						
						PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
						
						new action[64];
						format(action, sizeof action, "compra %s y lo consume.", PIZZA_FOOD[listitem][food_NAME]);
						Auto_SendPlayerAction(playerid, action);
						
						ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 0, 0, 0, 0, 0, 0);
						ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 4.1, false, true, true, false, 1000);
						
						ShowDialog(playerid, dialogid);
					}
				}
				else
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%d dólares ~w~para poder comprarlo.", PIZZA_FOOD[listitem][food_PRICE] - PI[playerid][ac_CASH]);
				}
			}
			else SendFormatNotification(playerid, "Gracias por su visita a ~g~%s ~w~vuelva pronto.", ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_NAME]);
			return 1;
		}
		case DIALOG_FOOD_BURGER:
		{
			if(response)
			{
				if(PI[playerid][ac_CASH] >= BURGER_SHOT_FOOD[listitem][food_PRICE])
				{
					if(PI[playerid][ac_HUNGRY] >= 99.0 && PI[playerid][ac_THIRST] >= 99.0) return SendNotification(playerid, "¿Es que quieres reventar?");
					
					if(GivePlayerCash(playerid, -BURGER_SHOT_FOOD[listitem][food_PRICE], true, true)) {
						Add_Hungry_Thirst(playerid, BURGER_SHOT_FOOD[listitem][food_HUNGRY], BURGER_SHOT_FOOD[listitem][food_THIRST]);
						
						PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
						
						new action[64];
						format(action, sizeof action, "compra %s y lo consume.", BURGER_SHOT_FOOD[listitem][food_NAME]);
						Auto_SendPlayerAction(playerid, action);
						
						ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 0, 0, 0, 0, 0, 0);
						ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 4.1, false, true, true, false, 1000);
						
						ShowDialog(playerid, dialogid);
					}
				}
				else
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%d dólares ~w~para poder comprarlo.", BURGER_SHOT_FOOD[listitem][food_PRICE] - PI[playerid][ac_CASH]);
				}
			}
			else SendFormatNotification(playerid, "Gracias por su visita a ~g~%s ~w~vuelva pronto.", ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_NAME]);
			return 1;
		}
		case DIALOG_FOOD_CLUCKIN:
		{
			if(response)
			{
				if(PI[playerid][ac_CASH] >= CLUCKIN_BELL_FOOD[listitem][food_PRICE])
				{
					if(PI[playerid][ac_HUNGRY] >= 99.0 && PI[playerid][ac_THIRST] >= 99.0) return SendNotification(playerid, "¿Es que quieres reventar?");
					
					if(GivePlayerCash(playerid, -CLUCKIN_BELL_FOOD[listitem][food_PRICE], true, true)) {
						Add_Hungry_Thirst(playerid, CLUCKIN_BELL_FOOD[listitem][food_HUNGRY], CLUCKIN_BELL_FOOD[listitem][food_THIRST]);
						
						PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
						
						new action[64];
						format(action, sizeof action, "compra %s y lo consume.", CLUCKIN_BELL_FOOD[listitem][food_NAME]);
						Auto_SendPlayerAction(playerid, action);
						
						ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 0, 0, 0, 0, 0, 0);
						ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 4.1, false, true, true, false, 1000);
						
						ShowDialog(playerid, dialogid);
					}
				}
				else
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%d dólares ~w~para poder comprarlo.", CLUCKIN_BELL_FOOD[listitem][food_PRICE] - PI[playerid][ac_CASH]);
				}
			}
			else SendFormatNotification(playerid, "Gracias por su visita a ~g~%s ~w~vuelva pronto.", ENTER_EXIT[ PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] ][ee_NAME]);
			return 1;
		}
		case DIALOG_PLAYER_TOYS:
		{
			if(response)
			{	
				if(listitem == MAX_SU_TOYS) return ShowDialog(playerid, DIALOG_PLAYER_TOY_DELETE_ALL);
				if(!ACCOUNT_INFO[playerid][ac_VIP] && listitem >= MAX_NU_TOYS)
				{
					SendFormatNotification(playerid, "¡Los jugadores ~y~VIP ~w~pueden tener hasta %d accesorios! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP.", MAX_SU_TOYS);
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] = listitem;
				ShowDialog(playerid, DIALOG_PLAYER_TOY_MENU);
			}
			return 1;
		}
		case DIALOG_PLAYER_TOY_MENU:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: //colocar / quitar
					{
						if(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED]) // tiene accesorio puesto
						{
							PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED] = false;
							RemovePlayerAttachedObject(playerid, PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT]);
							
							new action[45]; format(action, sizeof action, "se quita su \"%s\"", PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_NAME]);
							Auto_SendPlayerAction(playerid, action);
						}
						else // No lo tiene puesto
						{
							if(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_BONE] == -1) return ShowDialog(playerid, DIALOG_PLAYER_TOY_MENU);
							
							PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED] = true;
							RemovePlayerAttachedObject(playerid, PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT]);
							
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
							
							new action[45]; format(action, sizeof action, "se coloca su \"%s\"", PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_NAME]);
							Auto_SendPlayerAction(playerid, action);
						}
					}
					case 1: ShowDialog(playerid, DIALOG_PLAYER_TOY_EDIT_NAME);
					case 2: //Configurar
					{
						if(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_BONE] == -1)
						{
							ShowDialog(playerid, DIALOG_PLAYER_TOY_SELECT_BONE);
							return 1;
						}
						ShowDialog(playerid, DIALOG_PLAYER_TOY_EDIT_BONE);
					}
					case 3: ShowDialog(playerid, DIALOG_PLAYER_TOY_COLOR_1);
					case 4: ShowDialog(playerid, DIALOG_PLAYER_TOY_COLOR_2);
					case 5: ShowDialog(playerid, DIALOG_PLAYER_TOY_CONFIRMDELETE);
				}
			}
			return 1;
		}
		case DIALOG_PLAYER_TOY_EDIT_NAME:
		{
			if(response)
			{
				if(!strlen(inputtext) || strlen(inputtext) > 24)
				{
					SendNotification(playerid, "El nombre del accesorio debe contener de 1 a 24 caracteres.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				format(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_NAME], 24, "%s", inputtext);
				ShowDialog(playerid, DIALOG_PLAYER_TOY_MENU);
			}
			else ShowDialog(playerid, DIALOG_PLAYER_TOY_MENU);
			return 1;
		}
		case DIALOG_PLAYER_TOY_CONFIRMDELETE:
		{
			if(response)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM ptoys WHERE id = %d;", PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				if(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED]) RemovePlayerAttachedObject(playerid, PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT]);
				
				SendFormatNotification(playerid, "Accesorio ~g~\"%s\"~w~ eliminado.", PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_NAME]);
				
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_VALID] = false;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ID] = 0;
				format(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_NAME], 24, "");
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED] = false;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_MODELID] = 0;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_BONE] = 0;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_OFFSET_X] = 0.0;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_OFFSET_Y] = 0.0;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_OFFSET_Z] = 0.0;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ROT_X] = 0.0;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ROT_Y] = 0.0;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ROT_Z] = 0.0;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_SCALE_X] = 0.0;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_SCALE_Y] = 0.0;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_SCALE_Z] = 0.0;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_COLOR_1] = 0;
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_COLOR_2] = 0;
				PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] = 0;
				
				ShowDialog(playerid, DIALOG_PLAYER_TOYS);
			}
			else ShowDialog(playerid, DIALOG_PLAYER_TOY_MENU);
			return 1;
		}
		case DIALOG_PLAYER_TOY_SELECT_BONE:
		{
			if(response)
			{
				PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_BONE] = listitem + 1;
				EditPlayerToy(playerid);
			}
			else ShowDialog(playerid, DIALOG_PLAYER_TOY_MENU);
			return 1;
		}
		case DIALOG_PLAYER_TOY_EDIT_BONE:
		{
			if(response) ShowDialog(playerid, DIALOG_PLAYER_TOY_SELECT_BONE);
			else EditPlayerToy(playerid);
			return 1;
		}
		case DIALOG_PLAYER_TOY_COLOR_1:
		{
			if(response)
			{
				if(listitem == 0) PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_COLOR_1] = 0;
				else PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_COLOR_1] = RGBAToARGB(RandomColors[listitem]);
				
				if(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED]) UpdatePlayerToy(playerid, PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT]);
				SendNotification(playerid, "Color del accesorio actualizado.");
			}
			else ShowDialog(playerid, DIALOG_PLAYER_TOY_MENU);
			return 1;
		}
		case DIALOG_PLAYER_TOY_COLOR_2:
		{
			if(response)
			{
				if(listitem == 0) PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_COLOR_2] = 0;
				else PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_COLOR_2] = RGBAToARGB(RandomColors[listitem]);
				
				if(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED]) UpdatePlayerToy(playerid, PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT]);
				SendNotification(playerid, "Color del accesorio actualizado.");
			}
			else ShowDialog(playerid, DIALOG_PLAYER_TOY_MENU);
			return 1;
		}
		case DIALOG_PLAYER_TOY_DELETE_ALL:
		{
			if(response)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM ptoys WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				new temp_PLAYER_TOYS[Player_Toys_Info];
				for(new i = 0; i != MAX_SU_TOYS; i ++)
				{
					RemovePlayerAttachedObject(playerid, i);
					PLAYER_TOYS[playerid][i] = temp_PLAYER_TOYS;
				}
		
				SendNotification(playerid, "Has eliminado todos tus accesorios.");
			}
			else ShowDialog(playerid, DIALOG_PLAYER_TOYS);
			return 1;
		}
		case DIALOG_BANK:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: ShowDialog(playerid, DIALOG_BANK_WITHDRAW);
					case 1: ShowDialog(playerid, DIALOG_BANK_DEPOSIT);
					case 2: ShowDialog(playerid, DIALOG_BANK_TRANSFER);
					case 3: ShowDialog(playerid, DIALOG_BANK_TRANSACTIONS);
				}
			}
			else PLAYER_TEMP[playerid][pt_PLAYER_IN_ATM] = false;
			return 1;
		}
		case DIALOG_BANK_WITHDRAW:
		{
			if(response)
			{
				if(sscanf(inputtext, "d", inputtext[0]))
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "Introduce un valor numérico.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(inputtext[0] <= 0)
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "Introduce un valor positivo.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(inputtext[0] > PI[playerid][ac_BANK_MONEY])
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "Saldo insuficiente.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(PLAYER_TEMP[playerid][pt_PLAYER_IN_ATM])
				{
					if(inputtext[0] > 10000)
					{
						PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
						SendFormatNotification(playerid, "Las operaciones de más de ~r~10.000$ ~w~no se pueden realizar desde un cajero, solo en el banco.");
						ShowDialog(playerid, dialogid);
						return 1;
					}
				}
				
				if(GivePlayerCash(playerid, inputtext[0], true, false)) {
					PI[playerid][ac_BANK_MONEY] -= inputtext[0];
					
					RegisterBankAccountTransaction(ACCOUNT_INFO[playerid][ac_ID], ACCOUNT_INFO[playerid][ac_ID], BANK_TRANSACTION_WITHDRAW, inputtext[0]);
					
					SendFormatNotification(playerid, "Operación realizada con éxito, has retirado ~g~%s dólares~w~.", number_format_thousand(inputtext[0]));
					PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
				}
				PLAYER_TEMP[playerid][pt_PLAYER_IN_ATM] = false;
			}
			else ShowDialog(playerid, DIALOG_BANK);
			return 1;
		}
		case DIALOG_BANK_DEPOSIT:
		{
			if(response)
			{
				if(sscanf(inputtext, "d", inputtext[0]))
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "Introduce un valor numérico.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(inputtext[0] <= 0)
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "Introduce un valor positivo.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(inputtext[0] > PI[playerid][ac_CASH])
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "Dinero insuficiente.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(PLAYER_TEMP[playerid][pt_PLAYER_IN_ATM])
				{
					if(inputtext[0] > 10000)
					{
						PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
						SendFormatNotification(playerid, "Las operaciones de más de ~r~10.000$ ~w~no se pueden realizar desde un cajero, solo en el banco.");
						ShowDialog(playerid, dialogid);
						return 1;
					}
				}
				
				if(GivePlayerCash(playerid, -inputtext[0], true, true)) {
					PI[playerid][ac_BANK_MONEY] += inputtext[0];
					
					RegisterBankAccountTransaction(ACCOUNT_INFO[playerid][ac_ID], ACCOUNT_INFO[playerid][ac_ID], BANK_TRANSACTION_DEPOSIT, inputtext[0]);
					
					SendFormatNotification(playerid, "Operación realizada con éxito, has depositado ~g~%s dólares~w~.", number_format_thousand(inputtext[0]));
					PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
				}
				PLAYER_TEMP[playerid][pt_PLAYER_IN_ATM] = false;
			}
			else ShowDialog(playerid, DIALOG_BANK);
			return 1;
		}
		case DIALOG_BANK_TRANSFER:
		{
			if(response)
			{
				new to_bank_account;
				if(sscanf(inputtext, "d", to_bank_account))
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "Introduce un valor numérico.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(to_bank_account <= 0)
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "Introduce un valor positivo.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				inline OnBankAccountChecked()
				{
					new rows;
					if(cache_get_row_count(rows))
					{
						if(rows)
						{
							cache_get_value_index_int(0, 0, PLAYER_TEMP[playerid][pt_SELECT_BANK_TRANSFER_ID]);
							PLAYER_TEMP[playerid][pt_SELECT_BANK_TRANSFER_ACCOUNT] = to_bank_account;
							ShowDialog(playerid, DIALOG_BANK_TRANSFER_SEND);
						}
						else
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendNotification(playerid, "El número de cuenta introducido no existe o no es correcto.");
							PLAYER_TEMP[playerid][pt_PLAYER_IN_ATM] = false;
						}
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM player WHERE bank_account = %d;", to_bank_account);
				MySQL_TQueryInline(srp_db, using inline OnBankAccountChecked, QUERY_BUFFER);
			}
			else ShowDialog(playerid, DIALOG_BANK);
			return 1;
		}
		case DIALOG_BANK_TRANSFER_SEND:
		{
			if(response)
			{
				if(sscanf(inputtext, "d", inputtext[0]))
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "Introduce un valor numérico.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(inputtext[0] <= 0)
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "Introduce un valor positivo.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(inputtext[0] > PI[playerid][ac_BANK_MONEY])
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "Saldo insuficiente.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(PLAYER_TEMP[playerid][pt_PLAYER_IN_ATM])
				{
					if(inputtext[0] > 10000)
					{
						PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
						SendFormatNotification(playerid, "Las operaciones de más de ~r~10.000$ ~w~no se pueden realizar desde un cajero, solo en el banco.");
						ShowDialog(playerid, dialogid);
						return 1;
					}
				}
				
				PI[playerid][ac_BANK_MONEY] -= inputtext[0];
				new transfer_player_id = GetPlayerIdByBankAccountId(PLAYER_TEMP[playerid][pt_SELECT_BANK_TRANSFER_ACCOUNT]);
				
				new message[64];
				format(message, sizeof message, "Tranferencia recibida: +%s$", number_format_thousand(inputtext[0]));
				if(transfer_player_id == -1)
				{
					inline OnPhoneChecked()
					{
						new rows;
						if(cache_get_row_count(rows))
						{
							if(rows)
							{
								new id;
								cache_get_value_index_int(0, 0, id);
								if(id) RegisterPhoneMessage(0, id, message, true);
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM player WHERE bank_account = %d;", PLAYER_TEMP[playerid][pt_SELECT_BANK_TRANSFER_ACCOUNT]);
					MySQL_TQueryInline(srp_db, using inline OnPhoneChecked, QUERY_BUFFER);
				}
				else
				{
					PI[transfer_player_id][ac_BANK_MONEY] += inputtext[0];
					
					if(PI[transfer_player_id][ac_PHONE_NUMBER])
					{
						RegisterPhoneMessage(0, ACCOUNT_INFO[transfer_player_id][ac_ID], message);
						if(PI[transfer_player_id][ac_PHONE_STATE] == PHONE_STATE_ON)
						{
							if((PLAYER_TEMP[transfer_player_id][pt_GAME_STATE] == GAME_STATE_NORMAL || PLAYER_TEMP[transfer_player_id][pt_GAME_STATE] == GAME_STATE_DEAD))
							{
								SendClientMessageEx(transfer_player_id, -1, "{"#PRIMARY_COLOR"}[Nuevo mensaje recibido] {"#SILVER_COLOR"}Remitente: {FFFFFF}%s {"#SILVER_COLOR"}Mensaje: {FFFFFF}%s", convertPhoneNumber(transfer_player_id, 6740), message);
							}
						}
					}	
				}
				

				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET bank_money = bank_money + %d WHERE bank_account = %d;", inputtext[0], PLAYER_TEMP[playerid][pt_SELECT_BANK_TRANSFER_ACCOUNT]);
				mysql_tquery(srp_db, QUERY_BUFFER);

				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET bank_money = %d WHERE id = %d;", PI[playerid][ac_BANK_MONEY], ACCOUNT_INFO[playerid][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				RegisterBankAccountTransaction(ACCOUNT_INFO[playerid][ac_ID], PLAYER_TEMP[playerid][pt_SELECT_BANK_TRANSFER_ID], BANK_TRANSACTION_TRANSFER, inputtext[0]);
				
				SendFormatNotification(playerid, "Operación realizada con éxito, has transferido ~g~%s dólares ~w~al número de cuenta ~g~%s~w~.", number_format_thousand(inputtext[0]), number_format_thousand(PLAYER_TEMP[playerid][pt_SELECT_BANK_TRANSFER_ACCOUNT]));
				PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
				PLAYER_TEMP[playerid][pt_SELECT_BANK_TRANSFER_ACCOUNT] = 0;
				PLAYER_TEMP[playerid][pt_PLAYER_IN_ATM] = false;
			}
			else
			{
				PLAYER_TEMP[playerid][pt_SELECT_BANK_TRANSFER_ACCOUNT] = 0;
				ShowDialog(playerid, DIALOG_BANK_TRANSFER);
			}
			return 1;
		}
		case DIALOG_CREATE_BANK_ACCOUNT:
		{
			if(response)
			{
				if(PI[playerid][ac_CASH] >= 500)
				{
					if(GivePlayerCash(playerid, -500, true, true)) {
						PI[playerid][ac_BANK_MONEY] = 500;
						PI[playerid][ac_BANK_ACCOUNT] = getBankAccountNumber(ACCOUNT_INFO[playerid][ac_ID]);
						mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET bank_account = %d, bank_money = %d WHERE id = %d;", PI[playerid][ac_BANK_ACCOUNT], PI[playerid][ac_BANK_MONEY], ACCOUNT_INFO[playerid][ac_ID]);
						mysql_tquery(srp_db, QUERY_BUFFER);
						
						RegisterBankAccountTransaction(ACCOUNT_INFO[playerid][ac_ID], ACCOUNT_INFO[playerid][ac_ID], BANK_TRANSACTION_CREATE_ACCOUNT, 500);
						
						PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
						SendFormatNotification(playerid, "Tu cuenta bancaria ha sido creada con éxito, tu número de cuenta es ~g~%s~w~.", number_format_thousand(PI[playerid][ac_BANK_ACCOUNT]));
					}
				}
				else
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendFormatNotification(playerid, "No tienes dinero suficiente para crear la cuenta bancaria, te faltan ~r~%d dólares~w~.", 500 - PI[playerid][ac_CASH]);
				}
			}
			return 1;
		}
		case DIALOG_BANK_TRANSACTIONS:
		{
			if(response) PLAYER_TEMP[playerid][pt_PLAYER_IN_ATM] = false;
			else ShowDialog(playerid, DIALOG_BANK);
			return 1;
		}
		case DIALOG_247_LIST:
		{
			if(response)
			{
				switch(Supermarket_Product_List[listitem][product_TYPE])
				{
					case PRODUCT_TYPE_FOOD:
					{
						if(PI[playerid][ac_CASH] >= Supermarket_Product_List[listitem][product_PRICE])
						{
							new slot = GetEmptyPlayerPocketSlot(playerid);
							if(slot == -1)
							{
								PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
								SendNotification(playerid, "No tienes más espacio entre tus alimentos, elimina algo con ~g~/alimentos~w~.");
								return 1;
							}
							
							if(GivePlayerCash(playerid, -Supermarket_Product_List[listitem][product_PRICE], true, true)) {
								PLAYER_POCKET[playerid][slot][player_pocket_VALID] = true;
								format(PLAYER_POCKET[playerid][slot][player_pocket_object_NAME], 24, "%s", Supermarket_Product_List[listitem][product_NAME]);
								PLAYER_POCKET[playerid][slot][player_pocket_object_HUNGRY] = Supermarket_Product_List[listitem][product_HUNGRY];
								PLAYER_POCKET[playerid][slot][player_pocket_object_THIRST] = Supermarket_Product_List[listitem][product_THIRST];
								PLAYER_POCKET[playerid][slot][player_pocket_object_DRUNK] = Supermarket_Product_List[listitem][product_DRUNK];
								RegisterNewPlayerPocketObject(playerid, slot);
								
								SendFormatNotification(playerid, "Has comprado ~b~%s por ~b~%d dólares~w~,para consumirlo usa ~y~/alimentos~w~.", Supermarket_Product_List[listitem][product_NAME], Supermarket_Product_List[listitem][product_PRICE]);
								PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
							}
						}
						else
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar este producto.", number_format_thousand(Supermarket_Product_List[listitem][product_PRICE] - PI[playerid][ac_CASH]));
						}
					}
					case PRODUCT_TYPE_PHONE:
					{						
						if(PI[playerid][ac_CASH] >= Supermarket_Product_List[listitem][product_PRICE])
						{
							if(PI[playerid][ac_PHONE_NUMBER])
							{
								SendNotification(playerid, "Ya tienes un teléfono.");
								PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
								return 1;
							}
							
							if(GivePlayerCash(playerid, -Supermarket_Product_List[listitem][product_PRICE], true, true)) {
								PI[playerid][ac_PHONE_NUMBER] = getPhoneNumber(ACCOUNT_INFO[playerid][ac_ID]);
								PI[playerid][ac_PHONE_STATE] = PHONE_STATE_ON;
								PI[playerid][ac_PHONE_VISIBLE_NUMBER] = true;

								mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET phone_number = %d, phone_state = %d, phone_visible_number = %d WHERE id = %d;", PI[playerid][ac_PHONE_NUMBER], PI[playerid][ac_PHONE_STATE], PI[playerid][ac_PHONE_VISIBLE_NUMBER], ACCOUNT_INFO[playerid][ac_ID]);
								mysql_tquery(srp_db, QUERY_BUFFER);
								
								SendFormatNotification(playerid, "Has comprado un ~b~%s ~w~por ~b~%d dólares~w~, tu número es ~y~%d~w~.", Supermarket_Product_List[listitem][product_NAME], Supermarket_Product_List[listitem][product_PRICE], PI[playerid][ac_PHONE_NUMBER]);
								PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
							}
						}
						else
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar este producto.", number_format_thousand(Supermarket_Product_List[listitem][product_PRICE] - PI[playerid][ac_CASH]));
						}
						
					}
					case PRODUCT_TYPE_GPS:
					{
						if(PI[playerid][ac_CASH] >= Supermarket_Product_List[listitem][product_PRICE])
						{
							if(PI[playerid][ac_GPS])
							{
								SendNotification(playerid, "Ya tienes un GPS.");
								PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
								return 1;
							}
							
							if(GivePlayerCash(playerid, -Supermarket_Product_List[listitem][product_PRICE], true, true)) {	
								PI[playerid][ac_GPS] = true;
								SendFormatNotification(playerid, "Has comprado un ~b~%s ~w~por ~b~%d dólares~w~, usa ~b~/GPS ~w~para usarlo.", Supermarket_Product_List[listitem][product_NAME], Supermarket_Product_List[listitem][product_PRICE]);
								PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
							}
						}
						else
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar este producto.", number_format_thousand(Supermarket_Product_List[listitem][product_PRICE] - PI[playerid][ac_CASH]));
						}
					}
					case PRODUCT_TYPE_MP3:
					{
						if(PI[playerid][ac_CASH] >= Supermarket_Product_List[listitem][product_PRICE])
						{
							if(PI[playerid][ac_MP3])
							{
								SendNotification(playerid, "Ya tienes un MP3.");
								PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
								return 1;
							}
							
							if(GivePlayerCash(playerid, -Supermarket_Product_List[listitem][product_PRICE], true, true)) {
								PI[playerid][ac_MP3] = true;
								SendFormatNotification(playerid, "Has comprado un ~b~%s ~w~por ~b~%d dólares~w~, usa ~b~/MP3 ~w~para usarlo.", Supermarket_Product_List[listitem][product_NAME], Supermarket_Product_List[listitem][product_PRICE]);
								PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
							}
						}
						else
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar este producto.", number_format_thousand(Supermarket_Product_List[listitem][product_PRICE] - PI[playerid][ac_CASH]));
						}
					}
					case PRODUCT_TYPE_SPEAKERS:
					{
						if(PI[playerid][ac_CASH] >= Supermarket_Product_List[listitem][product_PRICE])
						{
							if(PI[playerid][ac_SPEAKERS])
							{
								SendNotification(playerid, "Ya tienes altavoces.");
								PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
								return 1;
							}
							
							if(GivePlayerCash(playerid, -Supermarket_Product_List[listitem][product_PRICE], true, true)) {
								PI[playerid][ac_SPEAKERS] = true;
								SendFormatNotification(playerid, "Has comprado ~b~%s ~w~por ~b~%d dólares~w~, ~w~usa ~b~/amp3 ~w~para usarlo.", Supermarket_Product_List[listitem][product_NAME], Supermarket_Product_List[listitem][product_PRICE]);
								PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
							}
						}
						else
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar este producto.", number_format_thousand(Supermarket_Product_List[listitem][product_PRICE] - PI[playerid][ac_CASH]));
						}
					}
					case PRODUCT_TYPE_PHONE_RESOLVER:
					{
						if(PI[playerid][ac_CASH] >= Supermarket_Product_List[listitem][product_PRICE])
						{
							if(PI[playerid][ac_PHONE_RESOLVER])
							{
								SendNotification(playerid, "Ya tienes una guía telefónica.");
								PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
								return 1;
							}
							
							if(GivePlayerCash(playerid, -Supermarket_Product_List[listitem][product_PRICE], true, true)) {
								PI[playerid][ac_PHONE_RESOLVER] = true;
								SendFormatNotification(playerid, "Has comprado una ~b~%s ~w~por ~b~%d dólares~w~, ~w~usa ~b~/guia ~w~para usarla.", Supermarket_Product_List[listitem][product_NAME], Supermarket_Product_List[listitem][product_PRICE]);
								PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
							}
						}
						else
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar este producto.", number_format_thousand(Supermarket_Product_List[listitem][product_PRICE] - PI[playerid][ac_CASH]));
						}
					}
				}
			}
			return 1;
		}
		case DIALOG_PLAYER_POCKET:
		{
			if(response)
			{
				if(listitem == MAX_PLAYER_POCKET_OBJECTS) return ShowDialog(playerid, DIALOG_PLAYER_POCKET_DELETE_ALL);
				if(!PLAYER_POCKET[playerid][listitem][player_pocket_VALID])
				{
					SendNotification(playerid, "Este slot está vacío.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				PLAYER_TEMP[playerid][pt_POCKET_SLOT_SELECTED] = listitem;
				ShowDialog(playerid, DIALOG_PLAYER_POCKET_OPTIONS);
			}
			return 1;
		}
		case DIALOG_PLAYER_WEAPONS_OPTIONS: {
			if(response) {
				switch(listitem) {
					case 0: ShowNearsPlayersToPlayer(playerid, NEAR_PLAYERS_WEAPON_GIVE);
					case 1: ShowNearsPlayersToPlayer(playerid, NEAR_PLAYERS_WEAPON_SELL);
					case 2: ShowDialog(playerid, DIALOG_PLAYER_WEAPONS_DELETE);
					case 3: {
						//guardar
						new command[128];
						format(command, sizeof command, "/guardar arma %d", PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT]);
						PC_EmulateCommand(playerid, command);
					}
				}
			}
		}
		case DIALOG_PLAYER_POCKET_OPTIONS:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: //Consumir
					{
						if(PI[playerid][ac_HUNGRY] >= 99.0 && PI[playerid][ac_THIRST] >= 99.0) 
						{
							PLAYER_TEMP[playerid][pt_POCKET_SLOT_SELECTED] = 0;
							SendNotification(playerid, "¿Es que quieres reventar?");
							return 1;
						}
					
						Add_Hungry_Thirst(playerid, PLAYER_POCKET[playerid][PLAYER_TEMP[playerid][pt_POCKET_SLOT_SELECTED]][player_pocket_object_HUNGRY], PLAYER_POCKET[playerid][PLAYER_TEMP[playerid][pt_POCKET_SLOT_SELECTED]][player_pocket_object_THIRST]);
						GivePlayerDrunkLevel(playerid, PLAYER_POCKET[playerid][PLAYER_TEMP[playerid][pt_POCKET_SLOT_SELECTED]][player_pocket_object_DRUNK]);
						
						new action[64];
						format(action, sizeof action, "consume %s.", PLAYER_POCKET[playerid][PLAYER_TEMP[playerid][pt_POCKET_SLOT_SELECTED]][player_pocket_object_NAME]);
						Auto_SendPlayerAction(playerid, action);
						
						DeletePlayerPocketObject(playerid, PLAYER_TEMP[playerid][pt_POCKET_SLOT_SELECTED]);
						PLAYER_TEMP[playerid][pt_POCKET_SLOT_SELECTED] = 0;
					
						ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 0, 0, 0, 0, 0, 0);
						ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 4.1, false, true, true, false, 1000);
					}
					case 1: ShowNearsPlayersToPlayer(playerid, NEAR_PLAYERS_POCKET_GIVE);
					case 2: ShowNearsPlayersToPlayer(playerid, NEAR_PLAYERS_POCKET_SELL);
					case 3: //Eliminar
					{
						SendFormatNotification(playerid, "Has eliminado \"%s\" de tus alimentos.", PLAYER_POCKET[playerid][PLAYER_TEMP[playerid][pt_POCKET_SLOT_SELECTED]][player_pocket_object_NAME]);
						DeletePlayerPocketObject(playerid, PLAYER_TEMP[playerid][pt_POCKET_SLOT_SELECTED]);
						PLAYER_TEMP[playerid][pt_POCKET_SLOT_SELECTED] = 0;
					}
				}
			}
			return 1;
		}
		case DIALOG_PLAYER_POCKET_DELETE_ALL:
		{
			if(response)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM pfoods WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
				mysql_query(srp_db, QUERY_BUFFER);
				
				new temp_PLAYER_POCKET[Player_Pocket_Enum];
				for(new i = 0; i != MAX_PLAYER_POCKET_OBJECTS; i ++) PLAYER_POCKET[playerid][i] = temp_PLAYER_POCKET;
				
				SendNotification(playerid, "Has eliminado todo lo que tenías en tus alimentos.");
			}
			else ShowDialog(playerid, DIALOG_PLAYER_POCKET);
			return 1;
		}
		case DIALOG_PHONE:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: //Agenda
					{
						if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_OFF)
						{
							PI[playerid][ac_PHONE_STATE] = PHONE_STATE_ON;
							ShowDialog(playerid, dialogid);
							return 1;
						}
						ShowDialog(playerid, DIALOG_PHONE_BOOK);
					}
					case 1: //Llamar
					{
						if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendNotification(playerid, "Tu teléfono está apagado, enciéndelo para usarlo.");
						ShowDialog(playerid, DIALOG_PHONE_CALL_NUMBER);
					}
					case 2: //Enviar mensaje
					{
						if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendNotification(playerid, "Tu teléfono está apagado, enciéndelo para usarlo.");
						ShowDialog(playerid, DIALOG_PHONE_SMS_NUMBER);
					}
					case 3: //Ver mensajes recibidos
					{
						if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendNotification(playerid, "Tu teléfono está apagado, enciéndelo para usarlo.");
						ShowDialog(playerid, DIALOG_PHONE_RECEIVED_MESSAGES);
					}
					case 4: //Ver mensajes enviados
					{
						if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendNotification(playerid, "Tu teléfono está apagado, enciéndelo para usarlo.");
						ShowDialog(playerid, DIALOG_PHONE_SENT_MESSAGES);
					}
					case 5: //Tienda
					{
						if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendNotification(playerid, "Tu teléfono está apagado, enciéndelo para usarlo.");
						
						ShowPlayerShop(playerid);
					}
					case 6: //Apagar
					{
						if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_OFF) return SendNotification(playerid, "Tu teléfono está apagado, enciéndelo para usarlo.");
						
						if(PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL]) EndPhoneCall(playerid);
						PI[playerid][ac_PHONE_STATE] = PHONE_STATE_OFF;
						SendNotification(playerid, "Has apagado tu teléfono, ahora no recibirás llamadas.");
					}
				}
			}
			return 1;
		}
		case DIALOG_PHONE_BOOK:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return ShowDialog(playerid, dialogid);
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == MAX_PHONE_CONTACTS + 10) return ShowDialog(playerid, DIALOG_PHONE_BOOK_ADD_NUMBER);
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == MAX_PHONE_CONTACTS + 20) return ShowDialog(playerid, DIALOG_PHONE_BOOK_DELETE_ALL);
				
				PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
				ShowDialog(playerid, DIALOG_PHONE_BOOK_OPTIONS);
			}
			else ShowDialog(playerid, DIALOG_PHONE);
			return 1;
		}
		case DIALOG_PHONE_BOOK_ADD_NUMBER:
		{
			if(response)
			{
				if(sscanf(inputtext, "d", inputtext[0]))
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "Introduce un valor numérico.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(inputtext[0] <= 0)
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "Introduce un valor positivo.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_ADD_NUMBER] = inputtext[0];
				ShowDialog(playerid, DIALOG_PHONE_BOOK_ADD_NAME);
			}
			else ShowDialog(playerid, DIALOG_PHONE_BOOK);
			return 1;
		}
		case DIALOG_PHONE_BOOK_ADD_NAME:
		{
			if(response)
			{
				if(isnull(inputtext))
				{ 
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(strlen(inputtext) > 24)
				{
					SendNotification(playerid, "Caracteres: 1-24.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				new slot = GetEmptyPlayerPhoneBookSlot(playerid);
				if(slot == -1) return SendNotification(playerid, "Memoria de la agenda llena.");
			
			
				PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_VALID] = true;
				format(PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_NAME], 24, "%s", inputtext);
				PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_PHONE_NUMBER] = PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_ADD_NUMBER];
				RegisterNewPlayerPhoneBook(playerid, slot);
				
				PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
				SendFormatNotification(playerid, "Nuevo contacto añadido a la agenda:~n~~n~~b~Nombre: ~w~%s~n~~b~Teléfono:~w~ %d.", PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_NAME], PLAYER_PHONE_BOOK[playerid][slot][phone_book_contact_PHONE_NUMBER]);
				ShowDialog(playerid, DIALOG_PHONE_BOOK);
			}
			else ShowDialog(playerid, DIALOG_PHONE_BOOK_ADD_NUMBER);
			return 1;
		}
		case DIALOG_PHONE_BOOK_OPTIONS:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0:  SendNotification(playerid, "Usa /llamar");
					case 1: ShowDialog(playerid, DIALOG_PHONE_BOOK_SEND_MESSAGE);
					case 2: ShowDialog(playerid, DIALOG_PHONE_BOOK_CHANGE_NAME);
					case 3: ShowDialog(playerid, DIALOG_PHONE_BOOK_CONFIRM_DELET);
				}
			}
			else ShowDialog(playerid, DIALOG_PHONE_BOOK);
			return 1;
		}
		case DIALOG_PHONE_BOOK_CHANGE_NAME:
		{
			if(response)
			{
				if(isnull(inputtext))
				{ 
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(strlen(inputtext) > 24)
				{
					SendNotification(playerid, "Caracteres: 1-24.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				format(PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_NAME], 24, "%s", inputtext);
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pbook SET name = '%e' WHERE id = %d;", PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_NAME], PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				SendNotification(playerid, "Contacto modificado.");
				ShowDialog(playerid, DIALOG_PHONE_BOOK);
			}
			else ShowDialog(playerid, DIALOG_PHONE_BOOK_OPTIONS);
			return 1;
		}
		case DIALOG_PHONE_BOOK_CONFIRM_DELET:
		{
			if(response)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM pbook WHERE id = %d;", PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				SendFormatNotification(playerid, "Contacto ~g~\"%s\" ~w~eliminado de tu agenda.", PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_NAME]);
				
				PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_VALID] = false;
				PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_ID] = 0;
				format(PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_NAME], 24, "");
				PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_PHONE_NUMBER] = 0;
				
				ShowDialog(playerid, DIALOG_PHONE_BOOK);
			}
			else ShowDialog(playerid, DIALOG_PHONE_BOOK_OPTIONS);
			return 1;
		}
		case DIALOG_PHONE_BOOK_DELETE_ALL:
		{
			if(response)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM pbook WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				new temp_PLAYER_PHONE_BOOK[Phone_Book_Enum]; 
				for(new i = 0; i != MAX_PHONE_CONTACTS; i ++) PLAYER_PHONE_BOOK[playerid][i] = temp_PLAYER_PHONE_BOOK;
				
				SendNotification(playerid, "Ha eliminado todos sus contactos.");
			}
			else ShowDialog(playerid, DIALOG_PHONE_BOOK);
			return 1;
		}
		case DIALOG_PHONE_CALL_NUMBER:
		{
			if(response) SendNotification(playerid, "Usa /llamar");
			else ShowDialog(playerid, DIALOG_PHONE);
			return 1;
		}
		case DIALOG_PHONE_SMS_NUMBER:
		{
			if(response) SendNotification(playerid, "Usa /sms");
			else ShowDialog(playerid, DIALOG_PHONE);
			return 1;
		}
		case DIALOG_PHONE_SMS_MESSAGE:
		{
			if(response) SendNotification(playerid, "Usa /sms");
			else ShowDialog(playerid, DIALOG_PHONE_SMS_MESSAGE);
			return 1;
		}
		case DIALOG_PHONE_BOOK_SEND_MESSAGE:
		{
			if(response) SendNotification(playerid, "Usa /sms");
			else ShowDialog(playerid, DIALOG_PHONE_BOOK_OPTIONS);
			return 1;
		}
		case DIALOG_PHONE_SENT_MESSAGES:
		{
			if(!response) ShowDialog(playerid, DIALOG_PHONE);
			return 1;
		}
		case DIALOG_PHONE_RECEIVED_MESSAGES:
		{
			if(!response) ShowDialog(playerid, DIALOG_PHONE);
			return 1;
		}
		case DIALOG_CONFIRM_BUY_PROPERTY:
		{
			if(response)
			{
				if(PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_SOLD]) return SendNotification(playerid, "Está propiedad ya está vendida.");
				if(PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_LEVEL] > ACCOUNT_INFO[playerid][ac_LEVEL]) return SendFormatNotification(playerid, "Necesitas ser como mínimo nivel %d para comprar esta propiedad.", PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_LEVEL]);
				if(PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_PRICE] > PI[playerid][ac_BANK_MONEY]) return SendNotification(playerid, "No.");
				if(PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_VIP_LEVEL] > ACCOUNT_INFO[playerid][ac_VIP]) return SendNotification(playerid, "No.");
				if(PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_EXTRA] > PI[playerid][ac_COINS]) return SendNotification(playerid, "No.");
	
				if(!PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_EXTRA])
				{
					if(PI[playerid][ac_PHONE_NUMBER])
					{
						new message[64]; format(message, sizeof message, "PROPIEDAD COMPRADA: -%s$", number_format_thousand(PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_PRICE]));
						RegisterPhoneMessage(0, ACCOUNT_INFO[playerid][ac_ID], message);
						if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_ON) SendClientMessageEx(playerid, -1, "{"#PRIMARY_COLOR"}[Nuevo mensaje recibido] {"#SILVER_COLOR"}Remitente: {FFFFFF}%s {"#SILVER_COLOR"}Mensaje: {FFFFFF}%s", convertPhoneNumber(playerid, 6740), message);
					}
					
					PI[playerid][ac_BANK_MONEY] -= PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_PRICE];
				
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET bank_money = %d WHERE id = %d;", PI[playerid][ac_BANK_MONEY], ACCOUNT_INFO[playerid][ac_ID]);
					mysql_tquery(srp_db, QUERY_BUFFER);
					RegisterBankAccountTransaction(ACCOUNT_INFO[playerid][ac_ID], ACCOUNT_INFO[playerid][ac_ID], BANK_TRANSACTION_BUY_PROPERTY, PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_PRICE]);
				}
				else
				{
					PI[playerid][ac_COINS] -= PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_EXTRA];
					
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET coins = %d WHERE id = %d;", PI[playerid][ac_COINS], ACCOUNT_INFO[playerid][ac_ID]);
					mysql_tquery(srp_db, QUERY_BUFFER);
					
					SendFormatNotification(playerid, "Has gastado %d "SERVER_COIN" en la compra de esta propiedad.", PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_EXTRA]);
				}
				
				
				PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_SOLD] = true;
				PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_CREW] = false;
				PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_OWNER_ID] = ACCOUNT_INFO[playerid][ac_ID];
				CreatePropertyInfo(PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX], ACCOUNT_INFO[playerid][ac_ID], ACCOUNT_INFO[playerid][ac_NAME], 0, "");
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE properties SET id_player = %d, id_territory = NULL WHERE id = %d;", ACCOUNT_INFO[playerid][ac_ID], PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				
				SendFormatNotification(playerid, "~g~¡Propiedad %d comprada!~n~~n~ ~w~Ahora puedes ir a tu casa, si no sabes donde es puedes marcarla con el ~b~/GPS~w~.", PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_ID]);
				PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
			}
			return 1;
		}
		case DIALOG_PLAYER_GPS:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: ShowDialog(playerid, DIALOG_PLAYER_GPS_PLAYER);
					case 1: ShowDialog(playerid, DIALOG_PLAYER_GPS_PROPERTIES);
					case 2: ShowDialog(playerid, DIALOG_PLAYER_GPS_VEHICLES);
					case 3: ShowDialog(playerid, DIALOG_PLAYER_GPS_SITES);
					case 4: ShowDialog(playerid, DIALOG_PLAYER_GPS_WORKS);
					case 5:
					{
						if(PLAYER_TEMP[playerid][pt_GPS_MAP]) HidePlayerGpsMap(playerid);
						else ShowPlayerGpsMap(playerid);
					}
				}
			}
			return 1;
		}
		case DIALOG_PLAYER_GPS_PLAYER:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return ShowDialog(playerid, dialogid);
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == MAX_PLAYER_GPS_SAVES + 10) return ShowDialog(playerid, DIALOG_PLAYER_GPS_PLAYER_ADD);
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == MAX_PLAYER_GPS_SAVES + 20) return ShowDialog(playerid, DIALOG_PLAYER_GPS_DELETE_ALL);
				
				PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
				ShowDialog(playerid, DIALOG_PLAYER_GPS_PLAYER_OPTION);
			}
			else ShowDialog(playerid, DIALOG_PLAYER_GPS);
			return 1;
		}
		case DIALOG_PLAYER_GPS_PLAYER_ADD:
		{
			if(response)
			{
				if(isnull(inputtext))
				{ 
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(strlen(inputtext) > 24)
				{
					SendNotification(playerid, "Caracteres: 1-24.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				new slot = GetEmptyPlayer_GPS_Slot(playerid);
				if(slot == -1) return SendNotification(playerid, "Memoria del GPS llena.");
			
				PLAYER_GPS[playerid][slot][player_gps_VALID] = true;
				format(PLAYER_GPS[playerid][slot][player_gps_NAME], 24, "%s", inputtext);
				GetPlayerPos(playerid, PLAYER_GPS[playerid][slot][player_gps_X], PLAYER_GPS[playerid][slot][player_gps_Y], PLAYER_GPS[playerid][slot][player_gps_Z]);
				PLAYER_GPS[playerid][slot][player_gps_WORLD] = GetPlayerVirtualWorld(playerid);
				PLAYER_GPS[playerid][slot][player_gps_INTERIOR] = GetPlayerInterior(playerid);
				RegisterNewPlayer_GPS_Site(playerid, slot);
				
				PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
				SendFormatNotification(playerid, "Nuevo sitio añadido al GPS con el nombre: ~g~%s~w~.", PLAYER_GPS[playerid][slot][player_gps_NAME]);
				ShowDialog(playerid, DIALOG_PLAYER_GPS_PLAYER);
			}
			else ShowDialog(playerid, DIALOG_PLAYER_GPS_PLAYER);
			return 1;
		}
		case DIALOG_PLAYER_GPS_PLAYER_OPTION:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: SetPlayer_GPS_Checkpoint(playerid, PLAYER_GPS[playerid][PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED]][player_gps_X], PLAYER_GPS[playerid][PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED]][player_gps_Y], PLAYER_GPS[playerid][PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED]][player_gps_Z], PLAYER_GPS[playerid][PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED]][player_gps_WORLD], PLAYER_GPS[playerid][PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED]][player_gps_INTERIOR]);
					case 1: ShowDialog(playerid, DIALOG_PLAYER_GPS_CHANGE_NAME);
					case 2: ShowDialog(playerid, DIALOG_PLAYER_GPS_CONFIRM_DELET);
				}
			}
			else ShowDialog(playerid, DIALOG_PLAYER_GPS_PLAYER);
			return 1;
		}
		case DIALOG_PLAYER_GPS_CHANGE_NAME:
		{
			if(response)
			{
				if(isnull(inputtext))
				{ 
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(strlen(inputtext) > 24)
				{
					SendNotification(playerid, "Caracteres: 1-24.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				format(PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_NAME], 24, "%s", inputtext);
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pgps SET name = '%e' WHERE id = %d;", PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_NAME], PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
				SendNotification(playerid, "Has modificado el nombre de este lugar correctamente");
				ShowDialog(playerid, DIALOG_PLAYER_GPS_PLAYER);
			}
			else ShowDialog(playerid, DIALOG_PLAYER_GPS_PLAYER_OPTION);
			return 1;
		}
		case DIALOG_PLAYER_GPS_CONFIRM_DELET:
		{
			if(response)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM pgps WHERE id = %d;", PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);

				PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_VALID] = false;
				PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_ID] = 0;
				format(PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_NAME], 24, "");
				PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_X] = 0.0;
				PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_Y] = 0.0;
				PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_Z] = 0.0;
				PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_WORLD] = 0;
				PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_INTERIOR] = 0;
				
				PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
				SendNotification(playerid, "Has eliminado este lugar de tu GPS.");
				ShowDialog(playerid, DIALOG_PLAYER_GPS_PLAYER);
			}
			else ShowDialog(playerid, DIALOG_PLAYER_GPS_PLAYER_OPTION);
			return 1;
		}
		case DIALOG_PLAYER_GPS_DELETE_ALL:
		{
			if(response)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM pgps WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				new temp_PLAYER_GPS[Player_GPS_Enum]; 
				for(new i = 0; i != MAX_PLAYER_GPS_SAVES; i ++) PLAYER_GPS[playerid][i] = temp_PLAYER_GPS;
				
				SendNotification(playerid, "Ha eliminado todos tus lugares guardados.");
			}
			else ShowDialog(playerid, DIALOG_PLAYER_GPS_PLAYER);
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITES:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: ShowDialog(playerid, DIALOG_PLAYER_GPS_SITE_0);
					case 1: ShowDialog(playerid, DIALOG_PLAYER_GPS_SITE_1);
					case 2: ShowDialog(playerid, DIALOG_PLAYER_GPS_SITE_2);
					case 3: ShowDialog(playerid, DIALOG_PLAYER_GPS_SITE_3);
					case 4: ShowDialog(playerid, DIALOG_PLAYER_GPS_SITE_4);
					case 5: ShowDialog(playerid, DIALOG_PLAYER_GPS_SITE_5);
					case 6: ShowDialog(playerid, DIALOG_PLAYER_GPS_SITE_6);
					case 7: ShowDialog(playerid, DIALOG_PLAYER_GPS_SITE_7);
					case 8: SetPlayer_GPS_Checkpoint(playerid, 1480.966918, -1772.065673, 18.795755, 0, 0);
					case 9: ShowDialog(playerid, DIALOG_PLAYER_GPS_SITE_8);
					case 10: SetPlayer_GPS_Checkpoint(playerid, 263.592102, 22.960014, 2.170951, 0, 0);
					case 11: SetPlayer_GPS_Checkpoint(playerid, -2026.736938, -101.953544, 35.164062, 0, 0);
					case 12: SetPlayer_GPS_Checkpoint(playerid, 1670.253662, 735.604919, 10.820312, 0, 0);
				}
			}
			else ShowDialog(playerid, DIALOG_PLAYER_GPS);
			return 1;
		}
		case DIALOG_PLAYER_GPS_WORKS:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return true;
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == WORK_MEDIC) SendNotification(playerid, "Este trabajo se puede conseguir en cualquier hospital.");
				else SetPlayer_GPS_Checkpoint(playerid, obtain_work_coords[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][obtain_work_MAP_ICON_X], obtain_work_coords[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][obtain_work_MAP_ICON_Y], obtain_work_coords[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][obtain_work_MAP_ICON_Z], 0, 0);
			}
			else ShowDialog(playerid, DIALOG_PLAYER_GPS);
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_0: //Hospitales
		{
			if(response) SetPlayer_GPS_Checkpoint(playerid, Hospital_Spawn_Positions[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][hspi_X], Hospital_Spawn_Positions[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][hspi_Y], Hospital_Spawn_Positions[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][hspi_Z], 0, 0);
			else ShowDialog(playerid, DIALOG_PLAYER_GPS_SITES);
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_1 .. DIALOG_PLAYER_GPS_SITE_4, DIALOG_PLAYER_GPS_SITE_8: //EE
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return ShowDialog(playerid, DIALOG_PLAYER_GPS);
				SetPlayer_GPS_Checkpoint(playerid, ENTER_EXIT[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][ee_EXT_X], ENTER_EXIT[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][ee_EXT_Y], ENTER_EXIT[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][ee_EXT_Z], ENTER_EXIT[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][ee_EXT_WORLD], ENTER_EXIT[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][ee_EXT_INTERIOR]);
			}
			else ShowDialog(playerid, DIALOG_PLAYER_GPS_SITES);
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_5: //Cajeros
		{
			if(response) SetPlayer_GPS_Checkpoint(playerid, ATM_BANK[listitem][atm_X], ATM_BANK[listitem][atm_Y], ATM_BANK[listitem][atm_Z], 0, 0);
			else ShowDialog(playerid, DIALOG_PLAYER_GPS_SITES);
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_6: //Gasolineras
		{
			if(response) SetPlayer_GPS_Checkpoint(playerid, Fuel_Stations[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][fs_X], Fuel_Stations[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][fs_Y], Fuel_Stations[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][fs_Z], 0, 0);
			else ShowDialog(playerid, DIALOG_PLAYER_GPS_SITES);
			return 1;
		}
		case DIALOG_WORKS_TELEPORTS:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return true;
				else SetPlayerPosEx(playerid, obtain_work_coords[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][obtain_work_MAP_ICON_X], obtain_work_coords[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][obtain_work_MAP_ICON_Y], obtain_work_coords[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][obtain_work_MAP_ICON_Z], 0.0, 0, 0, true);
			}
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_7: //Concesionarios
		{
			if(response) SetPlayer_GPS_Checkpoint(playerid, SELL_VEHICLES_SHOPS_SPAWN[listitem][0], SELL_VEHICLES_SHOPS_SPAWN[listitem][1], SELL_VEHICLES_SHOPS_SPAWN[listitem][2], 0, 0);
			else ShowDialog(playerid, DIALOG_PLAYER_GPS_SITES);
			return 1;
		}
		case DIALOG_PLAYER_GPS_PROPERTIES:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_GPS_SELECTED_PROPERTY][listitem] == -1) return ShowDialog(playerid, DIALOG_PLAYER_GPS);
				
				new index = GetPropertyIndexByID(PLAYER_TEMP[playerid][pt_PLAYER_GPS_SELECTED_PROPERTY][listitem]);
				if(index != -1) SetPlayer_GPS_Checkpoint(playerid, PROPERTY_INFO[index][property_EXT_X], PROPERTY_INFO[index][property_EXT_Y], PROPERTY_INFO[index][property_EXT_Z], 0, PROPERTY_INFO[index][property_EXT_INTERIOR]);
			}
			else ShowDialog(playerid, DIALOG_PLAYER_GPS);
			return 1;
		}
		case DIALOG_PLAYER_GPS_VEHICLES:
		{
			if(response)
			{
				new const vehicleid = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];

				if(vehicleid == -1) return ShowDialog(playerid, DIALOG_PLAYER_GPS);

				if(!ACCOUNT_INFO[playerid][ac_VIP])
				{
					if(!PLAYER_VEHICLES[vehicleid][player_vehicle_ACCESSIBLE])
					{
						SendNotification(playerid, "No podrás usar este vehículo hasta que no consigas VIP.");
					}
				}
				
				new Float:pos[3];
				GetVehiclePos(vehicleid, pos[0], pos[1], pos[2]);
				SetPlayer_GPS_Checkpoint(playerid, pos[0], pos[1], pos[2], GLOBAL_VEHICLES[vehicleid][gb_vehicle_WORLD], 0);
			}
			else ShowDialog(playerid, DIALOG_PLAYER_GPS);
			return 1;
		}
		case DIALOG_PROPERTY_OPTIONS:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: ShowDialog(playerid, DIALOG_PROPERTY_NAME);
					case 1:
					{
						new total;
						for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
						{
							if(IsPlayerConnected(i))
							{
								if( ACCOUNT_INFO[i][ac_STATE] == ROLEPLAY_STATE_GUEST_PROPERTY && PI[i][ac_LOCAL_INTERIOR] == PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_ID])
								{
									if(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_POLICE_FORCED] && (PLAYER_WORKS[i][WORK_POLICE][pwork_SET] && PLAYER_TEMP[i][pt_WORKING_IN] == WORK_POLICE)) {
										continue;
									}

									ACCOUNT_INFO[i][ac_STATE] = ROLEPLAY_STATE_NORMAL;
									PI[i][ac_LOCAL_INTERIOR] = 0;
									PLAYER_TEMP[i][pt_PROPERTY_INDEX] = -1;
									SetPlayerPosEx(i, PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_X], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_Y], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_Z], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_ANGLE], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_INTERIOR], 0, PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_FREEZE], false);
									StopAudioStreamForPlayer(i);
									SendFormatNotification(i, "~r~%s~w~ te ha echado de su propiedad.", PLAYER_TEMP[playerid][pt_RP_NAME]);
									total ++;
								}
							}
						}
						if(total == 0) SendNotification(playerid, "No hay nadie en tu propiedad.");
						else SendFormatNotification(playerid, "Has echado %d personas de tu propiedad.", total);
					}
					case 2:
					{
						if(!PI[playerid][ac_MP3])
						{
							SendFormatNotification(playerid, "No tienes ningún MP3, puedes ir a cualquier 24/7 para comprar uno.");
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							return 1;
						}
						if(PLAYER_TEMP[playerid][pt_PLAYER_WAITING_MP3_HTTP]) return SendNotification(playerid, "Espera que termine la búsqueda actual.");
						
						PLAYER_TEMP[playerid][pt_MUSIC_FOR_PROPERTY] = true;
						Auto_SendPlayerAction(playerid, "busca música en su MP3.");
						ShowDialog(playerid, DIALOG_PLAYER_MP3);
					}
					case 3: { //muebles
						ShowDialog(playerid, DIALOG_PROPERTY_OBJECTS_MENU);
					}
					case 4: {
						if(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_DIS_DEFAULT_INTERIOR]) {
							SetPropertyDefaultFurniture(PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED], true);
							SendNotification(playerid, "Se han vuelto a crear los muebles por defecto de esta propiedad.");
						}
						else {
							if(PROPERTY_INTERIORS[ PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_ID_INTERIOR] ][property_EMPTY_INTERIOR]) {
								SetPropertyDefaultFurniture(PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED], false);
								SendNotification(playerid, "Se han eliminado los muebles por defecto de esta propiedad.");
							}
							else SendNotification(playerid, "A esta propiedad no se le pueden eliminar los muebles por defecto.");
						}
					}
					case 5:
					{
						if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
						if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_ADD_PROPERTIES]) return SendNotification(playerid, "No tienes permiso.");
						
						
						for(new i = 0; i != MAX_TERRITORIES; i ++)
						{
							if(!TERRITORIES[i][territory_VALID]) continue;
							if(!TERRITORIES[i][territory_OCCUPIED]) continue;
							if(TERRITORIES[i][territory_CREW_ID] != PI[playerid][ac_CREW]) continue;
							
							if(IsPointInDynamicArea(TERRITORIES[i][territory_AREA], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_X], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_Y], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_Z]))
							{
								PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] = i;
								ShowDialog(playerid, DIALOG_CREW_PROPERTY_CONFIRM);
								return 1;
							}
						}
						SendNotification(playerid, "Esta propiedad no está dentro de un territorio de tu banda.");
					}
				}
			}
			return 1;
		}
		case DIALOG_PROPERTY_NAME:
		{
			if(response)
			{
				if(!PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_VALID]) return 1;
				if(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return 1;
				
				if(isnull(inputtext)) return ShowDialog(playerid, dialogid);
				if(strlen(inputtext) > 24)
				{
					ShowDialog(playerid, dialogid);
					SendNotification(playerid, "{"#SILVER_COLOR"}Caracteres: 1-24.");
					return 1;
				}
				
				format(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_NAME], 24, "%s", inputtext);
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE properties SET name = '%e' WHERE id = %d;", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_NAME], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				SendFormatNotification(playerid, "Nombre de la propiedad actualizado a ~g~'%s'~w~.", inputtext);
			}
			else ShowDialog(playerid, DIALOG_PROPERTY_OPTIONS);
			return 1;
		}
		case DIALOG_PLAYER_MP3:
		{
			if(response)
			{
				if(isnull(inputtext)) return ShowDialog(playerid, dialogid);
				
				ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, "Cargando ...", "Espera mientras buscamos resultados para su búsqueda ...", "X", "");
				
				//new title[100 + 1];
				//title = SpaceFix(inputtext);
				new str[180];
				format(str, sizeof str, ""SEARCH_YT_PATH"?limit=10&song_title=%s", SpaceFix(inputtext));
				
				PLAYER_TEMP[playerid][pt_PLAYER_WAITING_MP3_HTTP] = true;
				HTTP(playerid, HTTP_GET, str, "", "OnPlayerSongFound");
			}
			return 1;
		}
		case DIALOG_PLAYER_MP3_RESULTS:
		{
			if(response)
			{
				SendNotification(playerid, "Espere mientras se descarga la canción...");
				format(PLAYER_TEMP[playerid][pt_VIDEO_ID], 24, "%s", PLAYER_DIALOG_MP3_RESULT[playerid][listitem][videoID]);
				format(PLAYER_TEMP[playerid][pt_VIDEO_NAME], 64, "%s", PLAYER_DIALOG_MP3_RESULT[playerid][listitem][yt_title]);

				new str[180];
				format(str, sizeof str, ""DOWNLOAD_YT_PATH"?id=%s", PLAYER_TEMP[playerid][pt_VIDEO_ID]);
				
				PLAYER_TEMP[playerid][pt_PLAYER_WAITING_MP3_HTTP] = true;
				HTTP(playerid, HTTP_GET, str, "", "OnPlayerDownloadedSong");
			}
			return 1;
		}
		case DIALOG_BUY_VEHICLE:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] == INVALID_VEHICLE_ID) return 1;
				if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][gb_vehicle_VALID]) return 1;
				if(GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][gb_vehicle_TYPE] != VEHICLE_TYPE_SELL) return 1;
				if(!SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_VALID]) return 1;
				if(SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_VIP_LEVEL] > ACCOUNT_INFO[playerid][ac_VIP]) return 1;
				if(SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_LEVEL] > ACCOUNT_INFO[playerid][ac_LEVEL]) return 1;
				if(PI[playerid][ac_BANK_ACCOUNT] == 0) return 1;
				if(SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_PRICE] > PI[playerid][ac_BANK_MONEY]) return 1;
				if(SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_EXTRA] > PI[playerid][ac_COINS]) return 1;
			
				new vid = AddPersonalVehicle
				(
					playerid,
					GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][gb_vehicle_MODELID],
					SELL_VEHICLES_SHOPS_SPAWN[ SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_SHOP] ][0],
					SELL_VEHICLES_SHOPS_SPAWN[ SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_SHOP] ][1],
					SELL_VEHICLES_SHOPS_SPAWN[ SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_SHOP] ][2],
					SELL_VEHICLES_SHOPS_SPAWN[ SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_SHOP] ][3],
					GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][gb_vehicle_COLOR_1],
					GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][gb_vehicle_COLOR_2],
					VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_MAX_GAS]
				);
				if(!vid) return SendNotification(playerid, "No se pueden añadir más vehículos.");
				
				if(SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_EXTRA])
				{
					PI[playerid][ac_COINS] -= SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_EXTRA];
					
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET coins = %d WHERE id = %d;", PI[playerid][ac_COINS], ACCOUNT_INFO[playerid][ac_ID]);
					mysql_tquery(srp_db, QUERY_BUFFER);
					
					SendFormatNotification(playerid, "Has gastado ~r~%d "SERVER_COIN"~w~ en la compra de este vehículo.", SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_EXTRA]);
				}
				else
				{
					if(PI[playerid][ac_PHONE_NUMBER])
					{
						new message[64]; format(message, sizeof message, "VEHICULO COMPRADO: -%s$", number_format_thousand(SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_PRICE]));
						RegisterPhoneMessage(0, ACCOUNT_INFO[playerid][ac_ID], message);
						if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_ON) SendClientMessageEx(playerid, -1, "{"#PRIMARY_COLOR"}[Nuevo mensaje recibido] {"#SILVER_COLOR"}Remitente: {FFFFFF}%s {"#SILVER_COLOR"}Mensaje: {FFFFFF}%s", convertPhoneNumber(playerid, 6740), message);
					}
					
					PI[playerid][ac_BANK_MONEY] -= SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_PRICE];
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET bank_money = %d WHERE id = %d;", PI[playerid][ac_BANK_MONEY], ACCOUNT_INFO[playerid][ac_ID]);
					mysql_tquery(srp_db, QUERY_BUFFER);
					RegisterBankAccountTransaction(ACCOUNT_INFO[playerid][ac_ID], ACCOUNT_INFO[playerid][ac_ID], BANK_TRANSACTION_BUY_VEHICLE, SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_PRICE]);
				}	
				
				SendNotification(playerid, "~g~¡Vehículo comprado! ~n~~n~~w~Utiliza ~y~/ayuda vehiculos ~w~para ver que puedes hacer con tu vehículo.");
				PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
				
				new Float:pos[4];
				pos[0] = SELL_VEHICLES_SHOPS_SPAWN[ SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_SHOP] ][0];
				pos[1] = SELL_VEHICLES_SHOPS_SPAWN[ SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_SHOP] ][1];
				pos[2] = SELL_VEHICLES_SHOPS_SPAWN[ SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_SHOP] ][2];
				pos[3] = SELL_VEHICLES_SHOPS_SPAWN[ SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_SHOP] ][3];
				
				pos[0] += (2.0 * floatsin(-(pos[3] + 90.0), degrees));
				pos[1] += (2.0 * floatcos(-(pos[3] + 90.0), degrees));
				SetPlayerPosEx(playerid, pos[0], pos[1], pos[2], pos[3], 0, 0);
				
				if(!ACCOUNT_INFO[playerid][ac_VIP]) ReLockPlayerVehicles(playerid);
			}
			return 1;
		}
		case DIALOG_NOTARY:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0:
					{
						PLAYER_TEMP[playerid][pt_NOTARY_OPTION] = 0;
						ShowDialog(playerid, DIALOG_NOTARY_SELECT_PROPERTY);
					}
					case 1:
					{
						PLAYER_TEMP[playerid][pt_NOTARY_OPTION] = 0;
						ShowDialog(playerid, DIALOG_NOTARY_SELECT_VEHICLE);
					}
					case 2:
					{
						PLAYER_TEMP[playerid][pt_NOTARY_OPTION] = 1;
						ShowDialog(playerid, DIALOG_NOTARY_SELECT_PROPERTY);
					}
					case 3:
					{
						PLAYER_TEMP[playerid][pt_NOTARY_OPTION] = 1;
						ShowDialog(playerid, DIALOG_NOTARY_SELECT_VEHICLE);
					}
				}
			}
			return 1;
		}
		case DIALOG_NOTARY_SELECT_PROPERTY:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				
				PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] = GetPropertyIndexByID(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem]);
				if(PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] == -1) return SendNotification(playerid, "PROPIEDAD ID no encontrada.");
				
				switch(PLAYER_TEMP[playerid][pt_NOTARY_OPTION])
				{
					case 0: ShowDialog(playerid, DIALOG_PROPERTY_BANK_SELL);
					case 1: ShowDialog(playerid, DIALOG_PROPERTY_SELL_PRICE);
					case 2:
					{
						PLAYER_TEMP[playerid][pt_SHOP_ADD_TYPE] = SHOP_ARTICLE_TYPE_PROPERTY;
						PLAYER_TEMP[playerid][pt_SHOP_ADD_MODELID] = 1273;
						PLAYER_TEMP[playerid][pt_SHOP_ADD_VCOL1] = 0;
						PLAYER_TEMP[playerid][pt_SHOP_ADD_VCOL2] = 0;
						ShowDialog(playerid, DIALOG_SHOP_ADD_TEXT);
					}
				}
			}
			else
			{
				if(PLAYER_TEMP[playerid][pt_NOTARY_OPTION] == 2) ShowDialog(playerid, DIALOG_SHOP_ADD);
				else ShowDialog(playerid, DIALOG_NOTARY);
			}
			return 1;
		}
		case DIALOG_PROPERTY_BANK_SELL:
		{
			if(response)
			{
				if(!PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_VALID]) return 1;
				if(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return 1;
				if(PI[playerid][ac_BANK_ACCOUNT] == 0) return SendNotification(playerid, "Necesitas tener una cuenta bancaria para vender la propiedad.");

				PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_SOLD] = false;
				PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_CREW] = false;
				PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_OWNER_ID] = 0;
				format(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_NAME], 24, "PROPIEDAD %d", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_ID]);
				
				new info[3];
				UpdateUnnocupiedPropertyLabel(PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED]);
				
				DestroyDynamicPickup(PROPERTY_INFO[PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED]][property_EXT_PICKUP_ID]);
				PROPERTY_INFO[PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED]][property_EXT_PICKUP_ID] = INVALID_STREAMER_ID;
				
				PROPERTY_INFO[PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED]][property_EXT_PICKUP_ID] = CreateDynamicPickup(1273, 1, PROPERTY_INFO[PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED]][property_EXT_X], PROPERTY_INFO[PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED]][property_EXT_Y], PROPERTY_INFO[PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED]][property_EXT_Z], 0, PROPERTY_INFO[PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED]][property_EXT_INTERIOR]);
				info[0] = PICKUP_TYPE_PROPERTY;
				info[1] = PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED]; // Index
				info[2] = 2; // Pickup Exterior
				Streamer_SetArrayData(STREAMER_TYPE_PICKUP, PROPERTY_INFO[PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED]][property_EXT_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
				
				new Float:price, payment;
				price = PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_PRICE_BASE];
				if(ACCOUNT_INFO[playerid][ac_VIP]) payment = floatround( floatmul(price, 0.50) );
				else payment = floatround( floatmul(price, 0.25) );
				
				if(PI[playerid][ac_PHONE_NUMBER])
				{
					new message[64]; format(message, sizeof message, "PROPIEDAD VENDIDA: +%s$", number_format_thousand(payment));
					RegisterPhoneMessage(0, ACCOUNT_INFO[playerid][ac_ID], message);
					if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_ON) SendClientMessageEx(playerid, -1, "{"#PRIMARY_COLOR"}[Nuevo mensaje recibido] {"#SILVER_COLOR"}Remitente: {FFFFFF}%s {"#SILVER_COLOR"}Mensaje: {FFFFFF}%s", convertPhoneNumber(playerid, 6740), message);
				}
				
				PI[playerid][ac_BANK_MONEY] += payment;
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET bank_money = %d WHERE id = %d;", PI[playerid][ac_BANK_MONEY], ACCOUNT_INFO[playerid][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);

				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE properties SET id_player = NULL, id_territory = NULL WHERE id = %d;", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
						
				RegisterBankAccountTransaction(ACCOUNT_INFO[playerid][ac_ID], ACCOUNT_INFO[playerid][ac_ID], BANK_TRANSACTION_SOLD_PROPERTY, payment);
				SendFormatNotification(playerid, "Has vendido esta propiedad, has recibido %s$ en tu cuenta bancaria.", number_format_thousand(payment));
				PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
			
			}
			else ShowDialog(playerid, DIALOG_NOTARY_SELECT_PROPERTY);
			return 1;
		}
		case DIALOG_PROPERTY_SELL_PRICE:
		{
			if(response)
			{
				if(sscanf(inputtext, "d", inputtext[0])) return ShowDialog(playerid, dialogid);
				
				new Float:price, min_price, max_price;
				price = PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_PRICE];
				min_price = floatround( floatmul(price, 0.15) );
				max_price = floatround( floatmul(price, 6.0) );
				
				if(inputtext[0] < min_price || inputtext[0] > max_price)
				{
					SendNotification(playerid, "Este precio no está dentro del rango ofrecido.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				PLAYER_TEMP[playerid][pt_NOTARY_PRICE] = inputtext[0];
				ShowDialog(playerid, DIALOG_PROPERTY_SELL_TO_PLAYER);
			}
			else ShowDialog(playerid, DIALOG_NOTARY_SELECT_PROPERTY);
			return 1;
		}
		case DIALOG_PROPERTY_SELL_TO_PLAYER:
		{
			if(response)
			{
				if(sscanf(inputtext, "u", inputtext[0])) return ShowDialog(playerid, dialogid);
				
				if(!IsPlayerConnected(inputtext[0])) return SendNotification(playerid, "Error, el comprador está desconectado.");
				if(inputtext[0] == playerid) return SendNotification(playerid, "¿Pero como te vas a vender algo a ti mismo?");
				if(PLAYER_TEMP[inputtext[0]][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "Error, el comprador no está disponible.");
				
				if(ENTER_EXIT[ PLAYER_TEMP[inputtext[0]][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] != INTERIOR_CITY_HALL_LS) return SendNotification(playerid, "El comprador no está en la sala.");
				if(!IsPlayerInRangeOfPoint(inputtext[0], 3.0, -474.596282, 289.679107, 2004.584960)) return SendNotification(playerid, "El comprador no está en la sala.");
				if(PI[inputtext[0]][ac_BANK_ACCOUNT] == 0) return SendNotification(playerid, "El comprador no tiene cuenta bancaria.");
				
				new player_properties = CountPlayerProperties(inputtext[0]);
				if(player_properties >= MAX_SU_PROPERTIES) return SendNotification(playerid, "El comprador no puede adquirir más propiedades.");
				if(!ACCOUNT_INFO[inputtext[0]][ac_VIP])
				{
					if(player_properties >= MAX_NU_PROPERTIES) return SendNotification(playerid, "El comprador no puede adquirir más propiedades.");
				}
				
				if(PI[inputtext[0]][ac_LEVEL] < 2) return SendNotification(playerid, "Error, el comprador necesita ser al menos nivel 2.");
				if(PLAYER_TEMP[playerid][pt_NOTARY_PRICE] > PI[inputtext[0]][ac_BANK_MONEY]) return SendNotification(playerid, "Error, el comprador no tiene el dinero que pides.");
				
				SendNotification(playerid, "Tu oferta se ha enviado al comprador, espera para ver si la acepta.");
				
				new action[64]; format(action, sizeof action, "quiere llegar a un acuerdo con %s.", PLAYER_TEMP[inputtext[0]][pt_RP_NAME]);
				Auto_SendPlayerAction(playerid, action);
				
				PLAYER_TEMP[inputtext[0]][pt_PLAYER_PROPERTY_SELECTED] = PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED];
				PLAYER_TEMP[inputtext[0]][pt_NOTARY_PRICE] = PLAYER_TEMP[playerid][pt_NOTARY_PRICE];
				PLAYER_TEMP[inputtext[0]][pt_NOTARY_TO_PLAYER] = playerid;
				PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] = inputtext[0];
				
				ShowDialog(inputtext[0], DIALOG_SELL_CONFIRM_PROPERTY);
			}
			else ShowDialog(playerid, DIALOG_PROPERTY_SELL_PRICE);
			return 1;
		}
		case DIALOG_SELL_CONFIRM_PROPERTY:
		{
			if(response)
			{
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER])) return SendNotification(playerid, "El vendedor se ha desconectado.");
				if(PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] ][pt_NOTARY_TO_PLAYER] != playerid) return SendNotification(playerid, "El vendedor ya no está.");
				if(PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] ][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "Error, el vendedor no está disponible.");
				if(ENTER_EXIT[ PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] ][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] != INTERIOR_CITY_HALL_LS) return SendNotification(playerid, "El vendedor no está en la sala.");
				if(!IsPlayerInRangeOfPoint(PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER], 3.0, -474.596282, 289.679107, 2004.584960)) return SendNotification(playerid, "El vendedor no está en la sala.");
				if(PI[ PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] ][ac_BANK_ACCOUNT] == 0) return SendNotification(playerid, "El vendedor no tiene cuenta bancaria.");
				
				// Traspasar
				new label_str[256];
				format
				(
					label_str, 
					sizeof label_str, 
					"\
						Propiedad {"#PRIMARY_COLOR"}#%d\n\n\
						{FFFFFF}Propietario: {"#PRIMARY_COLOR"}%s\n\
						{FFFFFF}Presiona {"#PRIMARY_COLOR"}~k~~CONVERSATION_YES~ {FFFFFF}para entrar.\
					", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_ID], ACCOUNT_INFO[playerid][ac_NAME]
				);
				UpdateDynamic3DTextLabelText(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_LABEL_ID], 0xFFFFFFFF, label_str);
				
				PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_OWNER_ID] = ACCOUNT_INFO[playerid][ac_ID];
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE properties SET id_player = %d, id_territory = NULL WHERE id = %d;", ACCOUNT_INFO[playerid][ac_ID], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				// Banco
				new price = PLAYER_TEMP[playerid][pt_NOTARY_PRICE], seller = PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER];
				
				if(PI[playerid][ac_PHONE_NUMBER])
				{
					new message[64]; format(message, sizeof message, "PROPIEDAD COMPRADA: -%s$", number_format_thousand(price));
					RegisterPhoneMessage(0, ACCOUNT_INFO[playerid][ac_ID], message);
					if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_ON) SendClientMessageEx(playerid, -1, "{"#PRIMARY_COLOR"}[Nuevo mensaje recibido] {"#SILVER_COLOR"}Remitente: {FFFFFF}%s {"#SILVER_COLOR"}Mensaje: {FFFFFF}%s", convertPhoneNumber(playerid, 6740), message);
				}
				if(PI[seller][ac_PHONE_NUMBER])
				{
					new message[64]; format(message, sizeof message, "PROPIEDAD VENDIDA: +%s$", number_format_thousand(price));
					RegisterPhoneMessage(0, ACCOUNT_INFO[seller][ac_ID], message);
					if(PI[seller][ac_PHONE_STATE] == PHONE_STATE_ON) SendClientMessageEx(seller, -1, "{"#PRIMARY_COLOR"}[Nuevo mensaje recibido] {"#SILVER_COLOR"}Remitente: {FFFFFF}%s {"#SILVER_COLOR"}Mensaje: {FFFFFF}%s", convertPhoneNumber(seller, 6740), message);
				}
				
				PI[playerid][ac_BANK_MONEY] -= price;
				PI[seller][ac_BANK_MONEY] += price;
				RegisterBankAccountTransaction(ACCOUNT_INFO[playerid][ac_ID], ACCOUNT_INFO[seller][ac_ID], BANK_TRANSACTION_TRANSFER, price);
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET bank_money = %d WHERE id = %d;", PI[playerid][ac_BANK_MONEY], ACCOUNT_INFO[playerid][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);

				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET bank_money = %d WHERE id = %d;", PI[seller][ac_BANK_MONEY], ACCOUNT_INFO[seller][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				SendFormatNotification(playerid, "~g~¡Propiedad %d comprada!~n~~n~~w~Ahora puedes ir a tu casa, si no sabes donde es puedes marcarla con el ~b~/GPS~w~.", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_ID]);
				SendFormatNotification(seller, "~g~¡Propiedad %d vendida!~n~~n~~w~Has recibido ~g~%s dólares ~w~en tu cuenta bancaria.", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_ID], number_format_thousand(price));
				PlayerPlaySoundEx(seller, 1058, 0.0, 0.0, 0.0);
				PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
				
				new action[64]; format(action, sizeof action, "y %s han llegado a un acuerdo.", PLAYER_TEMP[seller][pt_RP_NAME]);
				Auto_SendPlayerAction(playerid, action);
			}
			return 1;
		}
		case DIALOG_NOTARY_SELECT_VEHICLE:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
				
				switch(PLAYER_TEMP[playerid][pt_NOTARY_OPTION])
				{
					case 0: ShowDialog(playerid, DIALOG_VEHICLE_BANK_SELL);
					case 1: ShowDialog(playerid, DIALOG_VEHICLE_SELL_PRICE);
					case 2:
					{
						PLAYER_TEMP[playerid][pt_SHOP_ADD_TYPE] = SHOP_ARTICLE_TYPE_VEHICLE;
						PLAYER_TEMP[playerid][pt_SHOP_ADD_MODELID] = GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_MODELID];
						PLAYER_TEMP[playerid][pt_SHOP_ADD_VCOL1] = GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_COLOR_1];
						PLAYER_TEMP[playerid][pt_SHOP_ADD_VCOL2] = GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_COLOR_2];
						ShowDialog(playerid, DIALOG_SHOP_ADD_TEXT);
					}
				}
			}
			else
			{
				if(PLAYER_TEMP[playerid][pt_NOTARY_OPTION] == 2) ShowDialog(playerid, DIALOG_SHOP_ADD);
				else ShowDialog(playerid, DIALOG_NOTARY);
			}
			return 1;
		}
		case DIALOG_VEHICLE_BANK_SELL:
		{
			if(response)
			{
				if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_VALID]) return 1;
				if(PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][player_vehicle_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return 1;
				if(PI[playerid][ac_BANK_ACCOUNT] == 0) return SendNotification(playerid, "Necesitas tener una cuenta bancaria para vender el vehículo.");

				new Float:price, payment;
				price = VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_MODELID] - 400 ][vehicle_info_PRICE];
				if(ACCOUNT_INFO[playerid][ac_VIP]) payment = floatround( floatmul(price, 0.50) );
				else payment = floatround( floatmul(price, 0.25) );
				
				//Destruir veh
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM pvehicles WHERE id = %d;", PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][player_vehicle_ID]);
				mysql_query(srp_db, QUERY_BUFFER);
				
				DestroyVehicleEx(PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED]);
				
				
				if(PI[playerid][ac_PHONE_NUMBER])
				{
					new message[64]; format(message, sizeof message, "VEHICULO VENDIDO: +%s$", number_format_thousand(payment));
					RegisterPhoneMessage(0, ACCOUNT_INFO[playerid][ac_ID], message);
					if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_ON) SendClientMessageEx(playerid, -1, "{"#PRIMARY_COLOR"}[Nuevo mensaje recibido] {"#SILVER_COLOR"}Remitente: {FFFFFF}%s {"#SILVER_COLOR"}Mensaje: {FFFFFF}%s", convertPhoneNumber(playerid, 6740), message);
				}
				PI[playerid][ac_BANK_MONEY] += payment;
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET bank_money = %d WHERE id = %d;", PI[playerid][ac_BANK_MONEY], ACCOUNT_INFO[playerid][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				RegisterBankAccountTransaction(ACCOUNT_INFO[playerid][ac_ID], ACCOUNT_INFO[playerid][ac_ID], BANK_TRANSACTION_SOLD_VEHICLE, payment);
				
				SendFormatNotification(playerid, "Has vendido este vehículo, has recibido %s$ en tu cuenta bancaria.", number_format_thousand(payment));
				PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
				
				if(!ACCOUNT_INFO[playerid][ac_VIP]) ReLockPlayerVehicles(playerid);
			}
			else ShowDialog(playerid, DIALOG_NOTARY_SELECT_VEHICLE);
			return 1;
		}
		case DIALOG_VEHICLE_SELL_PRICE:
		{
			if(response)
			{
				if(sscanf(inputtext, "d", inputtext[0])) return ShowDialog(playerid, dialogid);
				
				new Float:price, min_price, max_price;
				price = VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_MODELID] - 400 ][vehicle_info_PRICE];
				min_price = floatround( floatmul(price, 0.15) );
				max_price = floatround( floatmul(price, 6.0) );
				
				if(inputtext[0] < min_price || inputtext[0] > max_price)
				{
					SendNotification(playerid, "Este precio no está dentro del rango ofrecido.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				PLAYER_TEMP[playerid][pt_NOTARY_PRICE] = inputtext[0];
				ShowDialog(playerid, DIALOG_VEHICLE_SELL_TO_PLAYER);
			}
			else ShowDialog(playerid, DIALOG_NOTARY_SELECT_VEHICLE);
			return 1;
		}
		case DIALOG_VEHICLE_SELL_TO_PLAYER:
		{
			if(response)
			{
				if(sscanf(inputtext, "u", inputtext[0])) return ShowDialog(playerid, dialogid);
				
				if(!IsPlayerConnected(inputtext[0])) return SendNotification(playerid, "Error, el comprador está desconectado.");
				if(inputtext[0] == playerid) return SendNotification(playerid, "¿Pero como te vas a vender algo a ti mismo?");
				if(PLAYER_TEMP[inputtext[0]][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "Error, el comprador no está disponible.");
				
				if(PLAYER_TEMP[inputtext[0]][pt_INTERIOR_INDEX] == -1 || ENTER_EXIT[ PLAYER_TEMP[inputtext[0]][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] != INTERIOR_CITY_HALL_LS) return SendNotification(playerid, "El comprador no está en la sala.");
				if(!IsPlayerInRangeOfPoint(inputtext[0], 3.0, -474.596282, 289.679107, 2004.584960)) return SendNotification(playerid, "El comprador no está en la sala.");
				if(PI[inputtext[0]][ac_BANK_ACCOUNT] == 0) return SendNotification(playerid, "El comprador no tiene cuenta bancaria.");
				
				new pvehicles = CountPlayerVehicles(inputtext[0]);
				if(pvehicles >= MAX_SU_VEHICLES) return SendNotification(playerid, "El comprador no puede adquirir más vehículos.");
				if(!ACCOUNT_INFO[inputtext[0]][ac_VIP])
				{
					if(pvehicles >= MAX_NU_VEHICLES) return SendNotification(playerid, "El comprador no puede adquirir más vehículos.");
				}
				
				if(PI[inputtext[0]][ac_LEVEL] < 2) return SendNotification(playerid, "Error, el comprador necesita ser al menos nivel 2.");
				if(PLAYER_TEMP[playerid][pt_NOTARY_PRICE] > PI[inputtext[0]][ac_BANK_MONEY]) return SendNotification(playerid, "Error, el comprador no tiene el dinero que pides.");
				
				if(PI[inputtext[0]][ac_DRIVE_LICENSE_POINTS] == 0)
				{
					SendNotification(playerid, "Tu oferta se ha enviado al comprador, pero el no tiene licencia de conducir y no puede aceptar tu oferta.");
					SendFormatNotification(inputtext[0], "%s te ha ofrecido un vehículo, pero ~r~necesitas una licencia de conducir~w~ para aceptarlo.", PLAYER_TEMP[playerid][pt_RP_NAME]);
					return 1;
				}

				SendNotification(playerid, "Tu oferta se ha enviado al comprador, espera para ver si la acepta.");
				
				new action[64]; format(action, sizeof action, "quiere llegar a un acuerdo con %s.", PLAYER_TEMP[inputtext[0]][pt_RP_NAME]);
				Auto_SendPlayerAction(playerid, action);
				
				PLAYER_TEMP[inputtext[0]][pt_PLAYER_VEHICLE_SELECTED] = PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED];
				PLAYER_TEMP[inputtext[0]][pt_NOTARY_PRICE] = PLAYER_TEMP[playerid][pt_NOTARY_PRICE];
				PLAYER_TEMP[inputtext[0]][pt_NOTARY_TO_PLAYER] = playerid;
				PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] = inputtext[0];
				
				ShowDialog(inputtext[0], DIALOG_SELL_CONFIRM_VEHICLE);
			}
			else ShowDialog(playerid, DIALOG_VEHICLE_SELL_PRICE);
			return 1;
		}
		case DIALOG_SELL_CONFIRM_VEHICLE:
		{
			if(response)
			{
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER])) return SendNotification(playerid, "El vendedor se ha desconectado.");
				if(PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] ][pt_NOTARY_TO_PLAYER] != playerid) return SendNotification(playerid, "El vendedor ya no está.");
				if(PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] ][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "Error, el vendedor no está disponible.");
				if(ENTER_EXIT[ PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] ][pt_INTERIOR_INDEX] ][ee_INTERIOR_TYPE] != INTERIOR_CITY_HALL_LS) return SendNotification(playerid, "El vendedor no está en la sala.");
				if(!IsPlayerInRangeOfPoint(PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER], 3.0, -474.596282, 289.679107, 2004.584960)) return SendNotification(playerid, "El vendedor no está en la sala.");
				if(PI[ PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] ][ac_BANK_ACCOUNT] == 0) return SendNotification(playerid, "El vendedor no tiene cuenta bancaria.");
				
				// Traspasar
				PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][player_vehicle_OWNER_ID] = ACCOUNT_INFO[playerid][ac_ID];
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pvehicles SET id_player = %d WHERE id = %d;", ACCOUNT_INFO[playerid][ac_ID], PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][player_vehicle_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				
				// Banco
				new price = PLAYER_TEMP[playerid][pt_NOTARY_PRICE], seller = PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER];
				
				if(PI[playerid][ac_PHONE_NUMBER])
				{
					new message[64]; format(message, sizeof message, "VEHICULO COMPRADO: -%s$", number_format_thousand(price));
					RegisterPhoneMessage(0, ACCOUNT_INFO[playerid][ac_ID], message);
					if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_ON) SendClientMessageEx(playerid, -1, "{"#PRIMARY_COLOR"}[Nuevo mensaje recibido] {"#SILVER_COLOR"}Remitente: {FFFFFF}%s {"#SILVER_COLOR"}Mensaje: {FFFFFF}%s", convertPhoneNumber(playerid, 6740), message);
				}
				if(PI[seller][ac_PHONE_NUMBER])
				{
					new message[64]; format(message, sizeof message, "VEHICULO VENDIDO: +%s$", number_format_thousand(price));
					RegisterPhoneMessage(0, ACCOUNT_INFO[seller][ac_ID], message);
					if(PI[seller][ac_PHONE_STATE] == PHONE_STATE_ON) SendClientMessageEx(seller, -1, "{"#PRIMARY_COLOR"}[Nuevo mensaje recibido] {"#SILVER_COLOR"}Remitente: {FFFFFF}%s {"#SILVER_COLOR"}Mensaje: {FFFFFF}%s", convertPhoneNumber(seller, 6740), message);
				}
				
				PI[playerid][ac_BANK_MONEY] -= price;
				PI[seller][ac_BANK_MONEY] += price;
				RegisterBankAccountTransaction(ACCOUNT_INFO[playerid][ac_ID], ACCOUNT_INFO[seller][ac_ID], BANK_TRANSACTION_TRANSFER, price);
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET bank_money = %d WHERE id = %d;", PI[playerid][ac_BANK_MONEY], ACCOUNT_INFO[playerid][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);

				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET bank_money = %d WHERE id = %d;", PI[seller][ac_BANK_MONEY], ACCOUNT_INFO[seller][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				
				SendFormatNotification(playerid, "~g~¡Vehículo %s comprado!~n~~n~~w~Utiliza ~b~/GPS ~w~para localizarlo.", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
				SendFormatNotification(seller, "~g~¡Vehículo vendido!~n~~n~~w~Has recibido ~g~%s dólares ~w~en tu cuenta bancaria.", number_format_thousand(price));
				PlayerPlaySoundEx(seller, 1058, 0.0, 0.0, 0.0);
				PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
				
				new action[64]; format(action, sizeof action, "y %s han llegado a un acuerdo.", PLAYER_TEMP[seller][pt_RP_NAME]);
				Auto_SendPlayerAction(playerid, action);
				
				if(!ACCOUNT_INFO[seller][ac_VIP]) ReLockPlayerVehicles(seller);
				if(!ACCOUNT_INFO[playerid][ac_VIP]) ReLockPlayerVehicles(playerid);
			}
			return 1;
		}
		case DIALOG_SELECT_TRUCK_TYPE:
		{
			if(response)
			{
				if(Truck_Contents[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][truck_content_EXP] > PLAYER_WORKS[playerid][WORK_TRUCK][pwork_LEVEL])
				{
					SendFormatNotification(playerid, "Has realizado %d trabajos como camionero, necesitas %d para este viaje.", PLAYER_WORKS[playerid][WORK_TRUCK][pwork_LEVEL], Truck_Contents[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][truck_content_EXP]);
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				new vehicleid = GetPlayerVehicleID(playerid);
				TRUCK_VEHICLE[vehicleid][truck_vehicle_POINT] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
				
				PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0], "");
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2]);
				PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3]);
				
				PLAYER_TEMP[playerid][pt_TRUCK_LOADING_VALUE] = 35.0;
				UpdatePlayerLoadingTruckSize(playerid);
				KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][7]);
				KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][9]);
				PLAYER_TEMP[playerid][pt_TIMERS][9] = SetTimerEx("TruckLoadUp", 1000, false, "ii", playerid, vehicleid);
				
				SendClientMessage(playerid,-1, "{"#SILVER_COLOR"}El camión se está cargando, si sales de él se cancelará el viaje.");
			}
			else
			{
				KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][9]);
				PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][0]);
				PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][1]);
				PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][2]);
				PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_TRUCK_LOAD][3]);
				SetVehicleToRespawnEx(GetPlayerVehicleID(playerid));
			}
			return 1;
		}
		case DIALOG_TUNING_MENU_PART:
		{
			if(response)
			{
				if(isnull(PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_NAME])) return 1;
				
				if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_VALID]) return 1;
				
				new Float:Vehicle_Pos[3];
				GetVehiclePos(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2]);
				
				if(!IsPlayerInRangeOfPoint(playerid, 5.0, Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2])) return SendNotification(playerid, "El vehículo está demasiado lejos.");
				
				format(PLAYER_TEMP[playerid][pt_TUNING_SELECTED_PART], 24, "%s", PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_NAME]);
				
				ShowDialog(playerid, DIALOG_TUNING_MENU_COMPONENT);
			}
			else ShowDialog(playerid, DIALOG_MECHANIC_MENU);
			return 1;
		}
		case DIALOG_TUNING_MENU_COMPONENT:
		{
			if(response)
			{
				if(isnull(PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_NAME]) || !PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_ID]) return 1;
				
				if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_VALID]) return 1;
				
				new Float:Vehicle_Pos[3];
				GetVehiclePos(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2]);
				
				if(!IsPlayerInRangeOfPoint(playerid, 5.0, Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2])) return SendNotification(playerid, "El vehículo está demasiado lejos.");
				
				if(PI[playerid][ac_MECHANIC_PIECES] < PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_PIECES])
				{
					SendNotification(playerid, "No tienes suficientes piezas, puedes comprar más piezas en el taller.");
					return 1;
				}
				
				if(PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
				{
					new vehicleid = PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID];
					
					new slot = GetVehicleComponentType( PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_ID] );
					new current_component = GetVehicleComponentInSlot(vehicleid, slot);
					if(current_component) RemoveVehicleComponent(vehicleid, current_component);
					
					GLOBAL_VEHICLES[vehicleid][gb_vehicle_COMPONENTS][slot] = PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_ID];
					AddVehicleComponent(vehicleid, PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_ID]);
					
					PI[playerid][ac_MECHANIC_PIECES] -= PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_PIECES];
					SendFormatNotification(playerid, "Componente '%s' añadido, has necesitado %d piezas.", PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_NAME], PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_PIECES]);
				}
				else
				{
					format(PLAYER_TEMP[playerid][pt_TUNING_SELECTED_COMPONENT], 24, "%s", PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_NAME]);
					PLAYER_TEMP[playerid][pt_TUNING_SELECTED_COMPONENT_ID] = PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_ID];
					
					PLAYER_TEMP[playerid][pt_TUNING_SELECTED_PIECES] = PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_PIECES];
					ShowDialog(playerid, DIALOG_MECHANIC_TUNING_PRICE);
				}
			}
			else ShowDialog(playerid, DIALOG_TUNING_MENU_PART);
			return 1;
		}
		case DIALOG_MECHANIC_MENU:
		{
			if(response)
			{
				if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_VALID]) return 1;
				
				new Float:Vehicle_Pos[3];
				GetVehiclePos(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2]);
				
				if(!IsPlayerInRangeOfPoint(playerid, 5.0, Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2])) return SendNotification(playerid, "El vehículo está demasiado lejos.");
				
				switch(listitem)
				{
					case 0: // Reparar
					{
						if(PI[playerid][ac_MECHANIC_PIECES] < 10) return SendNotification(playerid, "No tienes suficientes piezas, puedes comprar más piezas en el taller.");
						
						if(gettime() < GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_LAST_REPAIR_TIME] + 300)
						{
							new time = (300-(gettime()-GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_LAST_REPAIR_TIME]));
							SendFormatNotification(playerid, "Tienes que esperar %s minutos para volver a reparar este vehículo.", TimeConvert(time));
							return 1;
						}
						
						if(PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
						{
							PI[playerid][ac_MECHANIC_PIECES] -= 10;
							RepairVehicleEx(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], playerid);
							
							SendNotification(playerid, "Este es tu vehículo por lo que no se cobrará.");
							SendNotification(playerid, "Has necesitado 10 piezas para la reparacion.");
							return 1;
						}
						else
						{
							if(!PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_VALID])
							{
								if(GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_DRIVER] == playerid || GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_LAST_DRIVER] == playerid)
								{
									PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_HEALTH][p_ac_info_IMMUNITY] = gettime() + 3;
									PI[playerid][ac_MECHANIC_PIECES] -= 10;
									RepairVehicleEx(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], playerid);
									
									SendNotification(playerid, "Eres el conductor o el último conductor de este vehículo por lo que no se cobrará.");
									SendNotification(playerid, "Has necesitado 10 piezas para la reparacion.");
									return 1;
								}
							}
							ShowDialog(playerid, DIALOG_MECHANIC_REPAIR_PRICE);
						}
					}
					case 1:
					{
						if(PI[playerid][ac_MECHANIC_PIECES] < 5)
						{
							ShowDialog(playerid, dialogid);
							SendNotification(playerid, "No tienes suficientes piezas, puedes comprar más piezas en el taller.");
							return 1;
						}
						
						if(!PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_VALID])
						{
							ShowDialog(playerid, dialogid);
							SendNotification(playerid, "Solo los vehículos personales se pueden pintar.");
							return 1;
						}
						
						ShowDialog(playerid, DIALOG_MECHANIC_SELECT_COL_SLOT);
					}
					case 2:
					{
						if(100 > PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_LEVEL])
						{
							SendFormatNotification(playerid, "Has realizado %d reparaciones como mecánico, necesitas 100 para tunear vehículos.", PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_LEVEL]);
							ShowDialog(playerid, dialogid);
							return 1;
						}
						
						ShowDialog(playerid, DIALOG_MECHANIC_REMOVE_COMPONEN);
					}
					case 3:
					{
						if(100 > PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_LEVEL])
						{
							SendFormatNotification(playerid, "Has realizado %d reparaciones como mecánico, necesitas 100 para tunear vehículos.", PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_LEVEL]);
							ShowDialog(playerid, dialogid);
							return 1;
						}
						
						if(!PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_VALID])
						{
							ShowDialog(playerid, dialogid);
							SendNotification(playerid, "Solo los vehículos personales se pueden tunear.");
							return 1;
						}
						
						ShowDialog(playerid, DIALOG_TUNING_MENU_PART);
					}
					case 4:
					{
						if(100 > PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_LEVEL])
						{
							SendFormatNotification(playerid, "Has realizado %d reparaciones como mecánico, necesitas 100 para paintjobs.", PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_LEVEL]);
							ShowDialog(playerid, dialogid);
							return 1;
						}
						
						if(!PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_VALID])
						{
							ShowDialog(playerid, dialogid);
							SendNotification(playerid, "Solo los vehículos personales se le pueden cambiar el paintjob.");
							return 1;
						}
						ShowDialog(playerid, DIALOG_TUNING_PAINTJOB);
					}
				}
			}
			return 1;
		}
		case DIALOG_MECHANIC_REPAIR_PRICE:
		{
			if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_VALID]) return 1;
				
			new Float:Vehicle_Pos[3];
			GetVehiclePos(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2]);
			
			if(!IsPlayerInRangeOfPoint(playerid, 5.0, Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2])) return SendNotification(playerid, "El vehículo está demasiado lejos.");
			
			if(response)
			{
				new price;
				if(sscanf(inputtext, "d", price))
				{
					SendNotification(playerid, "El precio no es correcto.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(price < 0 || price > 2000)
				{
					SendNotification(playerid, "El rango del precio es: 0$ - 2.000$.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				if(PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_VALID])
				{
					new buyer = GetPlayerIdFromAccountId(PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_OWNER_ID]);
					if(buyer == INVALID_PLAYER_ID) return SendNotification(playerid, "El dueño del vehículo no está cerca.");
					
					new Float:mechanic_pos[3];
					GetPlayerPos(playerid, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2]);
					if(!IsPlayerInRangeOfPoint(buyer, 10.0, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2])) return SendNotification(playerid, "El dueño del vehículo está demasiado lejos.");
					
					PLAYER_TEMP[buyer][pt_MECHANIC_PID] = playerid;
					PLAYER_TEMP[buyer][pt_MECHANIC_AID] = ACCOUNT_INFO[playerid][ac_ID];
					PLAYER_TEMP[buyer][pt_MECHANIC_PRICE] = price;
					format(PLAYER_TEMP[buyer][pt_MECHANIC_TEXT], 64, "Reparación completa del vehículo");
					PLAYER_TEMP[buyer][pt_MECHANIC_TYPE] = MECHANIC_OPTION_REPAIR;
					PLAYER_TEMP[buyer][pt_MECHANIC_EXTRA] = 0;
					PLAYER_TEMP[buyer][pt_MECHANIC_VEHICLE_ID] = PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID];
					PLAYER_TEMP[buyer][pt_MECHANIC_PIECES] = 10;
					PLAYER_TEMP[buyer][pt_MECHANIC_TIME] = gettime();
					
					ShowDialog(buyer, DIALOG_MECHANIC_ACCEPT);
					SendFormatNotification(playerid, "Le has ofrecido a %s reparar su vehículo por %s$.", PLAYER_TEMP[buyer][pt_RP_NAME], number_format_thousand(PLAYER_TEMP[buyer][pt_MECHANIC_PRICE]));
				}
				else
				{
					if(GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_OCCUPIED])
					{
						new buyer = GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_DRIVER];
						if(buyer == INVALID_PLAYER_ID) return SendNotification(playerid, "El conductor del vehículo no está cerca.");
					
						new Float:mechanic_pos[3];
						GetPlayerPos(playerid, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2]);
						if(!IsPlayerInRangeOfPoint(buyer, 10.0, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2])) return SendNotification(playerid, "El conductor del vehículo está demasiado lejos.");
						
						PLAYER_TEMP[buyer][pt_MECHANIC_PID] = playerid;
						PLAYER_TEMP[buyer][pt_MECHANIC_AID] = ACCOUNT_INFO[playerid][ac_ID];
						PLAYER_TEMP[buyer][pt_MECHANIC_PRICE] = price;
						format(PLAYER_TEMP[buyer][pt_MECHANIC_TEXT], 64, "Reparación completa del vehículo");
						PLAYER_TEMP[buyer][pt_MECHANIC_TYPE] = MECHANIC_OPTION_REPAIR;
						PLAYER_TEMP[buyer][pt_MECHANIC_EXTRA] = 0;
						PLAYER_TEMP[buyer][pt_MECHANIC_VEHICLE_ID] = PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID];
						PLAYER_TEMP[buyer][pt_MECHANIC_PIECES] = 10;
						PLAYER_TEMP[buyer][pt_MECHANIC_TIME] = gettime();
						
						ShowDialog(buyer, DIALOG_MECHANIC_ACCEPT);
						SendFormatNotification(playerid, "Le has ofrecido a %s reparar el vehículo por %s$.", PLAYER_TEMP[buyer][pt_RP_NAME], number_format_thousand(PLAYER_TEMP[buyer][pt_MECHANIC_PRICE]));
					}
					else
					{
						new buyer = GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_LAST_DRIVER];
						if(buyer == INVALID_PLAYER_ID) return SendNotification(playerid, "Este vehículo no tiene conductor.");
					
						new Float:mechanic_pos[3];
						GetPlayerPos(playerid, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2]);
						if(!IsPlayerInRangeOfPoint(buyer, 10.0, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2])) return SendNotification(playerid, "El conductor del vehículo está demasiado lejos.");
						
						PLAYER_TEMP[buyer][pt_MECHANIC_PID] = playerid;
						PLAYER_TEMP[buyer][pt_MECHANIC_AID] = ACCOUNT_INFO[playerid][ac_ID];
						PLAYER_TEMP[buyer][pt_MECHANIC_PRICE] = price;
						format(PLAYER_TEMP[buyer][pt_MECHANIC_TEXT], 64, "Reparación completa del vehículo");
						PLAYER_TEMP[buyer][pt_MECHANIC_TYPE] = MECHANIC_OPTION_REPAIR;
						PLAYER_TEMP[buyer][pt_MECHANIC_EXTRA] = 0;
						PLAYER_TEMP[buyer][pt_MECHANIC_VEHICLE_ID] = PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID];
						PLAYER_TEMP[buyer][pt_MECHANIC_PIECES] = 10;
						PLAYER_TEMP[buyer][pt_MECHANIC_TIME] = gettime();
						
						ShowDialog(buyer, DIALOG_MECHANIC_ACCEPT);
						SendFormatNotification(playerid, "Le has ofrecido a %s reparar el vehículo por %s$.", PLAYER_TEMP[buyer][pt_RP_NAME], number_format_thousand(PLAYER_TEMP[buyer][pt_MECHANIC_PRICE]));
					}
				}
			}
			else ShowDialog(playerid, DIALOG_MECHANIC_MENU);
			return 1;
		}
		case DIALOG_MECHANIC_COLOR_PRICE:
		{
			if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_VALID]) return 1;
				
			new Float:Vehicle_Pos[3];
			GetVehiclePos(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2]);
			
			if(!IsPlayerInRangeOfPoint(playerid, 5.0, Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2])) return SendNotification(playerid, "El vehículo está demasiado lejos.");
			
			if(response)
			{
				new price;
				if(sscanf(inputtext, "d", price))
				{
					SendNotification(playerid, "El precio no es correcto.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(price < 0 || price > 1750)
				{
					SendNotification(playerid, "El rango del precio es: 0$ - 1.750$.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				new buyer = GetPlayerIdFromAccountId(PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_OWNER_ID]);
				if(buyer == INVALID_PLAYER_ID) return SendNotification(playerid, "El dueño del vehículo no está cerca.");
				
				new Float:mechanic_pos[3];
				GetPlayerPos(playerid, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2]);
				if(!IsPlayerInRangeOfPoint(buyer, 10.0, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2])) return SendNotification(playerid, "El dueño del vehículo está demasiado lejos.");
				
				PLAYER_TEMP[buyer][pt_MECHANIC_PID] = playerid;
				PLAYER_TEMP[buyer][pt_MECHANIC_AID] = ACCOUNT_INFO[playerid][ac_ID];
				PLAYER_TEMP[buyer][pt_MECHANIC_PRICE] = price;
				
				switch(PLAYER_TEMP[playerid][pt_MECHANIC_COLOR_SLOT])
				{
					case 0:
					{
						format(PLAYER_TEMP[buyer][pt_MECHANIC_TEXT], 64, "Pintar color primario {%06x}(%d)", VEHICLE_COLORS[ PLAYER_TEMP[playerid][pt_MECHANIC_SELECTED_COLOR] ] >>> 8, PLAYER_TEMP[playerid][pt_MECHANIC_SELECTED_COLOR]);
						PLAYER_TEMP[buyer][pt_MECHANIC_TYPE] = MECHANIC_OPTION_COLOR_1;
					}
					case 1:
					{
						format(PLAYER_TEMP[buyer][pt_MECHANIC_TEXT], 64, "Pintar color secundario {%06x}(%d)", VEHICLE_COLORS[ PLAYER_TEMP[playerid][pt_MECHANIC_SELECTED_COLOR] ] >>> 8, PLAYER_TEMP[playerid][pt_MECHANIC_SELECTED_COLOR]);
						PLAYER_TEMP[buyer][pt_MECHANIC_TYPE] = MECHANIC_OPTION_COLOR_2;
					}
				}
				
				PLAYER_TEMP[buyer][pt_MECHANIC_EXTRA] = PLAYER_TEMP[playerid][pt_MECHANIC_SELECTED_COLOR];
				PLAYER_TEMP[buyer][pt_MECHANIC_VEHICLE_ID] = PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID];
				PLAYER_TEMP[buyer][pt_MECHANIC_PIECES] = 5;
				PLAYER_TEMP[buyer][pt_MECHANIC_TIME] = gettime();
				
				ShowDialog(buyer, DIALOG_MECHANIC_ACCEPT);
				
				SendFormatNotification(playerid, "Le has ofrecido a %s pintar su vehículo por %s$.", PLAYER_TEMP[buyer][pt_RP_NAME], number_format_thousand(PLAYER_TEMP[buyer][pt_MECHANIC_PRICE]));
			}
			else ShowDialog(playerid, DIALOG_MECHANIC_MENU);
			return 1;
		}
		case DIALOG_MECHANIC_TUNING_PRICE:
		{
			if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_VALID]) return 1;
				
			new Float:Vehicle_Pos[3];
			GetVehiclePos(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2]);
			
			if(!IsPlayerInRangeOfPoint(playerid, 5.0, Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2])) return SendNotification(playerid, "El vehículo está demasiado lejos.");
			
			if(response)
			{
				new price;
				if(sscanf(inputtext, "d", price))
				{
					SendNotification(playerid, "El precio no es correcto.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(price < 0 || price > ((PLAYER_TEMP[playerid][pt_TUNING_SELECTED_PIECES] * 50) + 2000))
				{
					SendFormatNotification(playerid, "El rango del precio es: 0$ - %s$.", number_format_thousand((PLAYER_TEMP[playerid][pt_TUNING_SELECTED_PIECES] * 50) + 2000));
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				new buyer = GetPlayerIdFromAccountId(PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_OWNER_ID]);
				if(buyer == INVALID_PLAYER_ID) return SendNotification(playerid, "El dueño del vehículo no está cerca.");
				
				new Float:mechanic_pos[3];
				GetPlayerPos(playerid, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2]);
				if(!IsPlayerInRangeOfPoint(buyer, 10.0, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2])) return SendNotification(playerid, "El dueño del vehículo está demasiado lejos.");
				
				PLAYER_TEMP[buyer][pt_MECHANIC_PID] = playerid;
				PLAYER_TEMP[buyer][pt_MECHANIC_AID] = ACCOUNT_INFO[playerid][ac_ID];
				PLAYER_TEMP[buyer][pt_MECHANIC_PRICE] = price;
				format(PLAYER_TEMP[buyer][pt_MECHANIC_TEXT], 64, "'%s' > '%s'", PLAYER_TEMP[playerid][pt_TUNING_SELECTED_PART], PLAYER_TEMP[playerid][pt_TUNING_SELECTED_COMPONENT]);
				PLAYER_TEMP[buyer][pt_MECHANIC_TYPE] = MECHANIC_OPTION_TUNING;
				PLAYER_TEMP[buyer][pt_MECHANIC_EXTRA] = PLAYER_TEMP[playerid][pt_TUNING_SELECTED_COMPONENT_ID];
				PLAYER_TEMP[buyer][pt_MECHANIC_VEHICLE_ID] = PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID];
				PLAYER_TEMP[buyer][pt_MECHANIC_PIECES] = PLAYER_TEMP[playerid][pt_TUNING_SELECTED_PIECES];
				PLAYER_TEMP[buyer][pt_MECHANIC_TIME] = gettime();
				
				ShowDialog(buyer, DIALOG_MECHANIC_ACCEPT);
				
				SendFormatNotification(playerid, "Le has ofrecido a %s tunear su vehículo por %s$.", PLAYER_TEMP[buyer][pt_RP_NAME], number_format_thousand(PLAYER_TEMP[buyer][pt_MECHANIC_PRICE]));
			}
			else ShowDialog(playerid, DIALOG_MECHANIC_MENU);
			return 1;
		}
		case DIALOG_MECHANIC_PAINTJOB_PRICE:
		{
			if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_VALID]) return 1;
				
			new Float:Vehicle_Pos[3];
			GetVehiclePos(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2]);
			
			if(!IsPlayerInRangeOfPoint(playerid, 5.0, Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2])) return SendNotification(playerid, "El vehículo está demasiado lejos.");
			
			if(response)
			{
				new price;
				if(sscanf(inputtext, "d", price))
				{
					SendNotification(playerid, "El precio no es correcto.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(price < 0 || price > 2750)
				{
					SendNotification(playerid, "El rango del precio es: 0$ - 2.750$.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				new buyer = GetPlayerIdFromAccountId(PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_OWNER_ID]);
				if(buyer == INVALID_PLAYER_ID) return SendNotification(playerid, "El dueño del vehículo no está cerca.");
				
				new Float:mechanic_pos[3];
				GetPlayerPos(playerid, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2]);
				if(!IsPlayerInRangeOfPoint(buyer, 10.0, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2])) return SendNotification(playerid, "El dueño del vehículo está demasiado lejos.");
				
				PLAYER_TEMP[buyer][pt_MECHANIC_PID] = playerid;
				PLAYER_TEMP[buyer][pt_MECHANIC_AID] = ACCOUNT_INFO[playerid][ac_ID];
				PLAYER_TEMP[buyer][pt_MECHANIC_PRICE] = price;
				if(PLAYER_TEMP[playerid][pt_PAINTJOB_SELECTED_ID] == 3)
					format(PLAYER_TEMP[buyer][pt_MECHANIC_TEXT], 64, "Eliminar Paintjob", PLAYER_TEMP[playerid][pt_PAINTJOB_SELECTED_ID]);
				else
					format(PLAYER_TEMP[buyer][pt_MECHANIC_TEXT], 64, "Paintjob (%d)", PLAYER_TEMP[playerid][pt_PAINTJOB_SELECTED_ID]);
				PLAYER_TEMP[buyer][pt_MECHANIC_TYPE] = MECHANIC_OPTION_PAINTJOB;
				PLAYER_TEMP[buyer][pt_MECHANIC_EXTRA] = PLAYER_TEMP[playerid][pt_PAINTJOB_SELECTED_ID];
				PLAYER_TEMP[buyer][pt_MECHANIC_VEHICLE_ID] = PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID];
				PLAYER_TEMP[buyer][pt_MECHANIC_PIECES] = 15;
				PLAYER_TEMP[buyer][pt_MECHANIC_TIME] = gettime();
				
				ShowDialog(buyer, DIALOG_MECHANIC_ACCEPT);
				
				SendFormatNotification(playerid, "Le has ofrecido a %s cambiar el paintjob de su vehículo por %s$.", PLAYER_TEMP[buyer][pt_RP_NAME], number_format_thousand(PLAYER_TEMP[buyer][pt_MECHANIC_PRICE]));
			}
			else ShowDialog(playerid, DIALOG_MECHANIC_MENU);
			return 1;
		}
		case DIALOG_MECHANIC_REM_COM_PRICE:
		{
			if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_VALID]) return 1;
				
			new Float:Vehicle_Pos[3];
			GetVehiclePos(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2]);
			
			if(!IsPlayerInRangeOfPoint(playerid, 5.0, Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2])) return SendNotification(playerid, "El vehículo está demasiado lejos.");
			
			if(response)
			{
				new price;
				if(sscanf(inputtext, "d", price))
				{
					SendNotification(playerid, "El precio no es correcto.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(price < 0 || price > 2750)
				{
					SendNotification(playerid, "El rango del precio es: 0$ - 2.750$.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				new buyer = GetPlayerIdFromAccountId(PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_OWNER_ID]);
				if(buyer == INVALID_PLAYER_ID) return SendNotification(playerid, "El dueño del vehículo no está cerca.");
				
				new Float:mechanic_pos[3];
				GetPlayerPos(playerid, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2]);
				if(!IsPlayerInRangeOfPoint(buyer, 10.0, mechanic_pos[0], mechanic_pos[1], mechanic_pos[2])) return SendNotification(playerid, "El dueño del vehículo está demasiado lejos.");

				inline OnComponentsInfoLoad()
				{
					new rows;
					if(cache_get_row_count(rows))
					{
						if(rows)
						{
							new part[24], name[24];
							cache_get_value_name(0, "part", part);
							cache_get_value_name(0, "name", name);

							PLAYER_TEMP[buyer][pt_MECHANIC_PID] = playerid;
							PLAYER_TEMP[buyer][pt_MECHANIC_AID] = ACCOUNT_INFO[playerid][ac_ID];
							PLAYER_TEMP[buyer][pt_MECHANIC_PRICE] = price;
							format(PLAYER_TEMP[buyer][pt_MECHANIC_TEXT], 64, "Eliminar componente (%s > %s)", part, name);
							PLAYER_TEMP[buyer][pt_MECHANIC_TYPE] = MECHANIC_OPTION_REMOVE_COMPONEN;
							PLAYER_TEMP[buyer][pt_MECHANIC_EXTRA] = PLAYER_TEMP[playerid][pt_MECHANIC_SELECTED_COMPONENT];
							PLAYER_TEMP[buyer][pt_MECHANIC_VEHICLE_ID] = PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID];
							PLAYER_TEMP[buyer][pt_MECHANIC_PIECES] = PLAYER_TUNING_MENU[playerid][ PLAYER_TEMP[playerid][pt_MECHANIC_SELECTED_COMPONENT] ][tuning_menu_PIECES];
							PLAYER_TEMP[buyer][pt_MECHANIC_TIME] = gettime();
							ShowDialog(buyer, DIALOG_MECHANIC_ACCEPT);
							SendFormatNotification(playerid, "Le has ofrecido a %s quitar un componente de su vehículo por %s$.", PLAYER_TEMP[buyer][pt_RP_NAME], number_format_thousand(PLAYER_TEMP[buyer][pt_MECHANIC_PRICE]));
						}
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM vcomponents_info WHERE id = %d;", GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_COMPONENTS][ PLAYER_TUNING_MENU[playerid][ PLAYER_TEMP[playerid][pt_MECHANIC_SELECTED_COMPONENT] ][tuning_menu_ID] ]);
				MySQL_TQueryInline(srp_db, using inline OnComponentsInfoLoad, QUERY_BUFFER);
			}
			else ShowDialog(playerid, DIALOG_MECHANIC_MENU);
			return 1;
		}
		case DIALOG_MECHANIC_ACCEPT:
		{
			if(response)
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_MECHANIC_TIME] + 20) return SendNotification(playerid, "Has tardardo mucho en aceptarlo.");
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_MECHANIC_PID])) return SendNotification(playerid, "El mecánico ya no está en el servidor.");
				if(ACCOUNT_INFO[PLAYER_TEMP[playerid][pt_MECHANIC_PID]][ac_ID] != PLAYER_TEMP[playerid][pt_MECHANIC_AID]) return SendNotification(playerid, "El mecánico ya no está en el servidor.");

				new Float:pos[3]; GetPlayerPos(PLAYER_TEMP[playerid][pt_MECHANIC_PID], pos[0], pos[1], pos[2]);
				if(!IsPlayerInRangeOfPoint(playerid, 10.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "El mecánico está demasiado lejos.");
				if(PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "El mecánico no está disponible.");
				
				if(PLAYER_TEMP[playerid][pt_MECHANIC_PRICE] > PI[playerid][ac_CASH])
				{
					SendClientMessage(PLAYER_TEMP[playerid][pt_MECHANIC_PID], -1, "{"#SILVER_COLOR"}La persona no tiene suficiente dinero.");
					SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder aceptarlo.", number_format_thousand(PLAYER_TEMP[playerid][pt_MECHANIC_PRICE] - PI[playerid][ac_CASH]));
					return 1;
				}
				if(PLAYER_TEMP[playerid][pt_MECHANIC_PIECES] > PI[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][ac_MECHANIC_PIECES])
				{
					SendNotification(playerid, "Al mecánico no le quedan piezas para el trabajo.");
					SendClientMessage(PLAYER_TEMP[playerid][pt_MECHANIC_PID], -1, "{"#SILVER_COLOR"}La persona ha aceptado pero no tienes piezas suficientes para el trabajo.");
					return 1;
				}
				
				if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID] ][gb_vehicle_VALID])
				{
					SendNotification(playerid, "El vehículo está muy lejos.");
					SendClientMessage(PLAYER_TEMP[playerid][pt_MECHANIC_PID], -1, "{"#SILVER_COLOR"}La persona ha aceptado pero el vehículo está muy lejos.");
					return 1;
				}
				if(GetVehicleDistanceFromPoint(PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID], pos[0], pos[1], pos[2]) > 10.0)
				{
					SendNotification(playerid, "El vehículo está muy lejos del mecánico.");
					SendClientMessage(PLAYER_TEMP[playerid][pt_MECHANIC_PID], -1, "{"#SILVER_COLOR"}La persona ha aceptado pero estás muy lejos del vehículo.");
					return 1;
				}
				
				if(GivePlayerCash(playerid, -PLAYER_TEMP[playerid][pt_MECHANIC_PRICE], true, true) && GivePlayerCash(PLAYER_TEMP[playerid][pt_MECHANIC_PID], PLAYER_TEMP[playerid][pt_MECHANIC_PRICE], true, false)) {
					switch(PLAYER_TEMP[playerid][pt_MECHANIC_TYPE])
					{
						case MECHANIC_OPTION_REPAIR:
						{
							PI[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][ac_MECHANIC_PIECES] -= PLAYER_TEMP[playerid][pt_MECHANIC_PIECES];
							PLAYER_WORKS[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][WORK_MECHANIC][pwork_LEVEL] ++;
							AddPlayerJobPoints(PLAYER_TEMP[playerid][pt_MECHANIC_PID], WORK_MECHANIC);
							
							RepairVehicleEx(PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID], playerid);
							SendClientMessage(PLAYER_TEMP[playerid][pt_MECHANIC_PID], -1, "{"#SILVER_COLOR"}Has necesitado 10 piezas para la reparacion.");
						}
						case MECHANIC_OPTION_COLOR_1:
						{
							PI[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][ac_MECHANIC_PIECES] -= 5;
							GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID] ][gb_vehicle_COLOR_1] = PLAYER_TEMP[playerid][pt_MECHANIC_EXTRA];
							ChangeVehicleColor(PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID] ][gb_vehicle_COLOR_1], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID] ][gb_vehicle_COLOR_2]);
							
							SendClientMessage(PLAYER_TEMP[playerid][pt_MECHANIC_PID], -1, "{"#SILVER_COLOR"}Has necesitado 5 piezas para pintar el vehículo.");
						}
						case MECHANIC_OPTION_COLOR_2:
						{
							PI[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][ac_MECHANIC_PIECES] -= 5;
							GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID] ][gb_vehicle_COLOR_2] = PLAYER_TEMP[playerid][pt_MECHANIC_EXTRA];
							ChangeVehicleColor(PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID] ][gb_vehicle_COLOR_1], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID] ][gb_vehicle_COLOR_2]);
							
							SendClientMessage(PLAYER_TEMP[playerid][pt_MECHANIC_PID], -1, "{"#SILVER_COLOR"}Has necesitado 5 piezas para pintar el vehículo.");
						}
						case MECHANIC_OPTION_TUNING:
						{
							new slot = GetVehicleComponentType(PLAYER_TEMP[playerid][pt_MECHANIC_EXTRA]);
							new current_component = GetVehicleComponentInSlot(PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID], slot);
							if(current_component) RemoveVehicleComponent(PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID], current_component);
							
							GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID] ][gb_vehicle_COMPONENTS][slot] = PLAYER_TEMP[playerid][pt_MECHANIC_EXTRA];
							AddVehicleComponent(PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID], PLAYER_TEMP[playerid][pt_MECHANIC_EXTRA]);
							
							PI[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][ac_MECHANIC_PIECES] -= PLAYER_TEMP[playerid][pt_MECHANIC_PIECES];
							SendFormatNotification(PLAYER_TEMP[playerid][pt_MECHANIC_PID], "Has necesitado ~r~%d piezas~w~ para tunear el vehículo.", PLAYER_TEMP[playerid][pt_MECHANIC_PIECES]);
						}
						case MECHANIC_OPTION_PAINTJOB:
						{
							if(PLAYER_TEMP[playerid][pt_MECHANIC_EXTRA] == 3)
							{
								PI[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][ac_MECHANIC_PIECES] += 15;
								SendFormatNotification(playerid, "Paintjob eliminado, piezas ganadas: 15, piezas totales: %d.", PI[playerid][ac_MECHANIC_PIECES]);
							}
							else
							{
								PI[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][ac_MECHANIC_PIECES] -= 15;
								SendNotification(playerid, "Has necesitado 15 piezas para el paintjob de el vehículo.");
							}
							GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID] ][gb_vehicle_PAINTJOB] = PLAYER_TEMP[playerid][pt_MECHANIC_EXTRA];
							ChangeVehiclePaintjob(PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID] ][gb_vehicle_PAINTJOB]);
						}

						case MECHANIC_OPTION_REMOVE_COMPONEN:
						{
							new slot = PLAYER_TUNING_MENU[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][ PLAYER_TEMP[playerid][pt_MECHANIC_EXTRA] ][tuning_menu_ID];
							RemoveVehicleComponent(PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID] ][gb_vehicle_COMPONENTS][slot]);
							GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_MECHANIC_VEHICLE_ID] ][gb_vehicle_COMPONENTS][slot] = 0;
							
							PI[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][ac_MECHANIC_PIECES] += PLAYER_TEMP[playerid][pt_MECHANIC_PIECES];
							SendFormatNotification(PLAYER_TEMP[playerid][pt_MECHANIC_PID], "Componente eliminado, piezas ganadas: ~g~%d~w~, piezas totales: ~g~%d~w~.", PLAYER_TEMP[playerid][pt_MECHANIC_PIECES], PI[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][ac_MECHANIC_PIECES]);
						}
					}
					
					SendFormatNotification(playerid, "Le has pagado %s$ al mecánico por su trabajo.", number_format_thousand(PLAYER_TEMP[playerid][pt_MECHANIC_PRICE]));
					SendFormatNotification(PLAYER_TEMP[playerid][pt_MECHANIC_PID], "%s te ha pagado ~g~%s dólares~w~ por tu trabajo.", PLAYER_TEMP[playerid][pt_RP_NAME], number_format_thousand(PLAYER_TEMP[playerid][pt_MECHANIC_PRICE]));
				}
			}
			else
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_MECHANIC_TIME] + 30) return 1;
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_MECHANIC_PID])) return 1;
				if(ACCOUNT_INFO[PLAYER_TEMP[playerid][pt_MECHANIC_PID]][ac_ID] != PLAYER_TEMP[playerid][pt_MECHANIC_AID]) return 1;

				SendClientMessage(PLAYER_TEMP[playerid][pt_MECHANIC_PID], -1, "{"#SILVER_COLOR"}La persona no aceptó lo que le ofreciste.");
			}
			return 1;
		}
		case DIALOG_MECHANIC_SELECT_COL_SLOT:
		{
			if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_VALID]) return 1;
				
			new Float:Vehicle_Pos[3];
			GetVehiclePos(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2]);
			
			if(!IsPlayerInRangeOfPoint(playerid, 5.0, Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2])) return SendNotification(playerid, "El vehículo está demasiado lejos.");
			
			if(response)
			{
				PLAYER_TEMP[playerid][pt_MECHANIC_COLOR_SLOT] = listitem;
				ShowDialog(playerid, DIALOG_MECHANIC_SELECT_COLOR);
			}
			else ShowDialog(playerid, DIALOG_MECHANIC_MENU);
			return 1;
		}
		case DIALOG_MECHANIC_SELECT_COLOR:
		{
			if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_VALID]) return 1;
				
			new Float:Vehicle_Pos[3];
			GetVehiclePos(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2]);
			
			if(!IsPlayerInRangeOfPoint(playerid, 5.0, Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2])) return SendNotification(playerid, "El vehículo está demasiado lejos.");
			
			if(response)
			{
				if(PI[playerid][ac_MECHANIC_PIECES] < 5) return SendNotification(playerid, "No tienes suficientes piezas, puedes comprar más piezas en el taller.");

				if(PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
				{
					PI[playerid][ac_MECHANIC_PIECES] -= 5;
					switch(PLAYER_TEMP[playerid][pt_MECHANIC_COLOR_SLOT])
					{
						case 0: GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_COLOR_1] = listitem;
						case 1: GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_COLOR_2] = listitem;
					}
					ChangeVehicleColor(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_COLOR_1], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_COLOR_2]);
					
					SendNotification(playerid, "Has necesitado 5 piezas para pintar el vehículo.");
				}
				else
				{
					PLAYER_TEMP[playerid][pt_MECHANIC_SELECTED_COLOR] = listitem;
					ShowDialog(playerid, DIALOG_MECHANIC_COLOR_PRICE);
				}
			}
			else ShowDialog(playerid, DIALOG_MECHANIC_SELECT_COL_SLOT);
			return 1;
		}
		case DIALOG_MECHANIC_REMOVE_COMPONEN:
		{
			if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_VALID]) return 1;
			
			new Float:Vehicle_Pos[3];
			GetVehiclePos(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2]);
			
			if(!IsPlayerInRangeOfPoint(playerid, 5.0, Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2])) return SendNotification(playerid, "El vehículo está demasiado lejos.");
		
			if(response)
			{
				if(100 > PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_LEVEL])
				{
					SendFormatNotification(playerid, "Has realizado %d reparaciones como mecánico, necesitas 100 para tunear vehículos.", PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_LEVEL]);
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_ID] < 0) return ShowDialog(playerid, dialogid);

				if(PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
				{
					RemoveVehicleComponent(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_COMPONENTS][ PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_ID] ]);
					GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_COMPONENTS][ PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_ID] ] = 0;
					
					PI[playerid][ac_MECHANIC_PIECES] += PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_PIECES];
					SendFormatNotification(playerid, "Componente eliminado, piezas ganadas: %d, piezas totales: %d.", PLAYER_TUNING_MENU[playerid][listitem][tuning_menu_PIECES], PI[playerid][ac_MECHANIC_PIECES]);
				}
				else
				{
					PLAYER_TEMP[playerid][pt_MECHANIC_SELECTED_COMPONENT] = listitem;
					ShowDialog(playerid, DIALOG_MECHANIC_REM_COM_PRICE);
				}
			}
			else ShowDialog(playerid, DIALOG_MECHANIC_MENU);
			return 1;
		}
		case DIALOG_TUNING_PAINTJOB:
		{
			if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_VALID]) return 1;
			
			new Float:Vehicle_Pos[3];
			GetVehiclePos(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2]);
			
			if(!IsPlayerInRangeOfPoint(playerid, 5.0, Vehicle_Pos[0], Vehicle_Pos[1], Vehicle_Pos[2])) return SendNotification(playerid, "El vehículo está demasiado lejos.");
			
			if(response)
			{
				if(100 > PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_LEVEL])
				{
					SendFormatNotification(playerid, "Has realizado %d reparaciones como mecánico, necesitas 100 para cambiar el paintjob de vehículos.", PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_LEVEL]);
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(PI[playerid][ac_MECHANIC_PIECES] < 15) return SendNotification(playerid, "No tienes suficientes piezas, puedes comprar más piezas en el taller.");
				
				if(listitem == 0)
				{
					if(GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_PAINTJOB] == 3)
					{
						SendNotification(playerid, "El vehículo no tiene ningún paintjob.");
						return 1;
					}

					if(PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
					{
						GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_PAINTJOB] = 3;
						ChangeVehiclePaintjob(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_PAINTJOB]);
						PI[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][ac_MECHANIC_PIECES] += 15;
						SendFormatNotification(playerid, "Paintjob eliminado, piezas ganadas: 15, piezas totales: %d.", PI[playerid][ac_MECHANIC_PIECES]);
					}
					else
					{
						PLAYER_TEMP[playerid][pt_PAINTJOB_SELECTED_ID] = 3;
						ShowDialog(playerid, DIALOG_MECHANIC_PAINTJOB_PRICE);
					}
					
					return 1;
				}
				
				if(PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
				{
					GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_PAINTJOB] = listitem - 1;
					ChangeVehiclePaintjob(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_PAINTJOB]);
					PI[playerid][ac_MECHANIC_PIECES] -= 15;
					SendNotification(playerid, "Has necesitado 15 piezas para el paintjob de el vehículo.");
				}
				else
				{
					PLAYER_TEMP[playerid][pt_PAINTJOB_SELECTED_ID] = listitem - 1;
					ShowDialog(playerid, DIALOG_MECHANIC_PAINTJOB_PRICE);
				}
			}
			else ShowDialog(playerid, DIALOG_MECHANIC_MENU);
			return 1;
		}
		/*case DIALOG_SELECT_TRASH_ROUTE:
		{
			if(response)
			{
				if(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID] == INVALID_PLAYER_ID)
				{
					SendNotification(playerid, "Necesitas un compañero para comenzar a recoger basura.");
					return 1;
				}
				
				TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_JOB_STARTED] = true;
				TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] = listitem;
				TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_CP] = 0;
				
				SetPlayerTrashCheckpoint(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
				SetPlayerTrashCheckpoint(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
				
				CreatePlayerTrashRouteObjects(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], listitem);
				CreatePlayerTrashRouteObjects(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], listitem);
				
				SendClientMessageEx(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], -1, "Ruta {42aaf4}#%d {FFFFFF}establecida, pueden comenzar.", TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] + 1);
				SendClientMessageEx(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], -1, "Ruta {42aaf4}#%d {FFFFFF}establecida, pueden comenzar.", TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_ROUTE] + 1);
			}
			return 1;
		}*/
		case DIALOG_SEED_LIST:
		{
			if(response)
			{
				if(seed_info[listitem][seed_info_EXP] > PLAYER_WORKS[playerid][WORK_FARMER][pwork_LEVEL]) return SendFormatNotification(playerid, "Necesitas haber plantado al menos %d plantas como agricultor para poder comprar esta semilla.", seed_info[listitem][seed_info_EXP]);
				
				PLAYER_TEMP[playerid][pt_SELECTED_BUY_SEED_ID] = listitem;
				ShowDialog(playerid, DIALOG_SEED_BUY);
			}
			return 1;
		}
		case DIALOG_SEED_BUY:
		{
			if(response)
			{
				if(sscanf(inputtext, "d", inputtext[0])) return SendNotification(playerid, "La cantidad de semillas no es correcta.");
				if(inputtext[0] <= 0 || inputtext[0] > 100000) return SendNotification(playerid, "La cantidad de semillas no es correcta.");
				
				new price = seed_info[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_SEED_ID] ][seed_info_PRICE] * inputtext[0];
				if(price > PI[playerid][ac_CASH]) return SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar las semillas.", number_format_thousand(price - PI[playerid][ac_CASH]));
				
				if(GivePlayerCash(playerid, -price, true, true)) {
					switch(seed_info[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_SEED_ID] ][seed_info_PLANT_TYPE])
					{
						case PLANT_TYPE_MEDICINE: PI[playerid][ac_SEED_MEDICINE] += inputtext[0];
						case PLANT_TYPE_CANNABIS: PI[playerid][ac_SEED_CANNABIS] += inputtext[0];
						case PLANT_TYPE_CRACK: PI[playerid][ac_SEED_CRACK] += inputtext[0];
					}
					
					SendFormatNotification(playerid, "Has comprado %s semillas de %s por %s$.", number_format_thousand(inputtext[0]), seed_info[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_SEED_ID] ][seed_info_NAME], number_format_thousand(price));
				}
			}
			else ShowDialog(playerid, DIALOG_SEED_LIST);
			return 1;
		}
		case DIALOG_PLANT_PLANTS:
		{
			if(response)
			{
				if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendNotification(playerid, "No estás depie.");
				if(!IsPlayerInDynamicArea(playerid, Farmer_Area)) return SendNotification(playerid, "No estás en la zona para plantar, está marcada en el mapa con una bandera.");
				
				switch(seed_info[listitem][seed_info_PLANT_TYPE])
				{
					case PLANT_TYPE_MEDICINE:
					{
						if(plant_info[listitem][plant_info_SEEDS] > PI[playerid][ac_SEED_MEDICINE])
						{
							SendNotification(playerid, "No tienes las semillas necesarias para plantar esta planta.");
							return 1;
						}
						PI[playerid][ac_SEED_MEDICINE] -= plant_info[listitem][plant_info_SEEDS];
					}
					case PLANT_TYPE_CANNABIS:
					{
						if(plant_info[listitem][plant_info_SEEDS] > PI[playerid][ac_SEED_CANNABIS])
						{
							SendNotification(playerid, "No tienes las semillas necesarias para plantar esta planta.");
							return 1;
						}
						PI[playerid][ac_SEED_CANNABIS] -= plant_info[listitem][plant_info_SEEDS];
					}
					case PLANT_TYPE_CRACK:
					{
						if(plant_info[listitem][plant_info_SEEDS] > PI[playerid][ac_SEED_CRACK])
						{
							SendNotification(playerid, "No tienes las semillas necesarias para plantar esta planta.");
							return 1;
						}
						PI[playerid][ac_SEED_CRACK] -= plant_info[listitem][plant_info_SEEDS];
					}
				}
				
				SendFormatNotification(playerid, "Vas a plantar '%s' y vas a necesitar %d semillas.", plant_info[listitem][plant_info_NAME], plant_info[listitem][plant_info_SEEDS]);
				
				
				new Float:offset = floatdiv(PLAYER_WORKS[playerid][WORK_FARMER][pwork_LEVEL], 10);
				if(offset > 30.0) offset = 30.0;
				
				PLAYER_TEMP[playerid][pt_PLANTING_PLANT_SELECTED] = listitem;
				PLAYER_TEMP[playerid][pt_PLANTING] = true;
				PLAYER_TEMP[playerid][pt_WORKING_IN] = WORK_FARMER;
				
				TogglePlayerControllableEx(playerid, false);
				SetCameraBehindPlayer(playerid);
				ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Loop", 4.1, true, false, false, false, 0);
				SetPlayerKeyPress(playerid, plant_info[listitem][plant_info_SEEDS] * 7);
			}
			return 1;
		}
		case DIALOG_BLACK_MARKET:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0:
					{
						if(ACCOUNT_INFO[playerid][ac_LEVEL] < 3) return SendNotification(playerid, "Necesitas ser al menos nivel 3 para poder comprar armas.");
						if(!PI[playerid][ac_CREW] && !ACCOUNT_INFO[playerid][ac_VIP]) return SendNotification(playerid, "Solo puedes comprar armas aquí si perteneces a una banda o eres VIP (/ayuda).");
						
						if(!ACCOUNT_INFO[playerid][ac_VIP])
						{
							if(PI[playerid][ac_CAN_BUY_BM] > gettime())
							{
								SendFormatNotification(playerid, "Tienes que esperar %s minutos para volver a comprar aquí.", TimeConvert( PI[playerid][ac_CAN_BUY_BM] - gettime() ));
								return 1;
							}
						}
						
						ShowDialog(playerid, DIALOG_BLACK_MARKET_WEAPONS);
					}
					case 1: ShowDialog(playerid, DIALOG_BLACK_MARKET_SELECT_WEA);
				}
			}
			return 1;
		}
		case DIALOG_BLACK_MARKET_WEAPONS:
		{
			if(response)
			{
				if(BLACK_MARKT_WEAPONS[listitem][black_market_EXP] > PI[playerid][ac_BLACK_MARKET_LEVEL])
				{
					SendFormatNotification(playerid, "Necesitas ser un cliente más habitual para que te vendan esta arma (%d/%d).", PI[playerid][ac_BLACK_MARKET_LEVEL], BLACK_MARKT_WEAPONS[listitem][black_market_EXP]);
					return 1;
				}
				
				
				if(PI[playerid][ac_CASH] >= BLACK_MARKT_WEAPONS[listitem][black_market_WEAPON_PRICE])
				{
					new weapon_slot = WEAPON_INFO[ BLACK_MARKT_WEAPONS[listitem][black_market_WEAPON_ID] ][weapon_info_SLOT];
					if(PLAYER_WEAPONS[playerid][weapon_slot][player_weapon_ID] != 0)
					{
						SendFormatNotification(playerid, "Para comprar esta arma debes deshacerte de tu \"%s (%d)\"~w~ para tener espacio.", WEAPON_INFO[ PLAYER_WEAPONS[playerid][weapon_slot][player_weapon_ID] ][weapon_info_NAME], weapon_slot);
						return 1;
					}
					
					PI[playerid][ac_CAN_BUY_BM] = gettime() + 300;
					
					if(GivePlayerCash(playerid, -BLACK_MARKT_WEAPONS[listitem][black_market_WEAPON_PRICE], true, true)) {
						PI[playerid][ac_BLACK_MARKET_LEVEL] ++;
						
						if(WEAPON_INFO[ BLACK_MARKT_WEAPONS[listitem][black_market_WEAPON_ID] ][weapon_info_AMMO]) GivePlayerWeaponEx(playerid, BLACK_MARKT_WEAPONS[listitem][black_market_WEAPON_ID], 50);
						else GivePlayerWeaponEx(playerid, BLACK_MARKT_WEAPONS[listitem][black_market_WEAPON_ID], 1);

						RegisterNewPlayerWeapon(playerid, weapon_slot); 
						SendFormatNotification(playerid, "Arma ~y~\"%s\" ~w~comprada por \"%s$\"", WEAPON_INFO[ BLACK_MARKT_WEAPONS[listitem][black_market_WEAPON_ID] ][weapon_info_NAME], number_format_thousand(BLACK_MARKT_WEAPONS[listitem][black_market_WEAPON_PRICE]));
					}
				}
				else
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar esta arma.", number_format_thousand(BLACK_MARKT_WEAPONS[listitem][black_market_WEAPON_PRICE] - PI[playerid][ac_CASH]));
				}
			}
			return 1;
		}
		case DIALOG_BLACK_MARKET_SELECT_WEA:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				
				PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
				
				if(!WEAPON_INFO[ PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_ID] ][weapon_info_AMMO])
				{
					SendNotification(playerid, "Este tipo de arma no necesita munición.");
					return 1;
				}
				
				ShowDialog(playerid, DIALOG_BLACK_MARKET_AMMO);
			}
			return 1;
		}
		case DIALOG_BLACK_MARKET_AMMO:
		{
			if(response)
			{
				if(sscanf(inputtext, "d", inputtext[0])) return ShowDialog(playerid, dialogid);
				if(inputtext[0] <= 0 || inputtext[0] > 9999) return SendNotification(playerid, "La cantidad de munición no es correcta.");
				
				new price = 3 * inputtext[0];
				if(price > PI[playerid][ac_CASH]) return SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$ ~w~para poder comprar la munición.", number_format_thousand(price - PI[playerid][ac_CASH]));
				
				if(PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_AMMO] + inputtext[0] > 9999)
				{
					SendNotification(playerid, "A esta arma no le entra tanta munición.");
					return 1;
				}
				
				if(GivePlayerCash(playerid, -price, true, true)) {
					GivePlayerWeaponEx(playerid, PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_ID], inputtext[0]);
					
					SendFormatNotification(playerid, "Has comprado ~y~%s~w~ balas para tu ~y~%s~w~ por %s dólares.", number_format_thousand(inputtext[0]), WEAPON_INFO[ PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_ID] ][weapon_info_NAME], number_format_thousand(price));
				}
			}
			return 1;
		}
		case DIALOG_PLAYER_WEAPONS:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == 13 + 20) // Eliminar todo
				{
					ShowDialog(playerid, DIALOG_PLAYER_WEAPONS_DELETE_A);
					return 1;
				}
				
				PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
				ShowDialog(playerid, DIALOG_PLAYER_WEAPONS_OPTIONS);
			}
			return 1;
		}
		case DIALOG_PLAYER_WEAPONS_DELETE_A:
		{
			if(response)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM pweapons WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				ResetPlayerWeaponsEx(playerid);
		
				SendNotification(playerid, "Has eliminado todas tus armas.");
				Auto_SendPlayerAction(playerid, "tira todas sus armas al suelo");
			}
			else ShowDialog(playerid, DIALOG_PLAYER_WEAPONS);
			return 1;
		}
		case DIALOG_PLAYER_WEAPONS_DELETE:
		{
			if(response)
			{
				SendFormatNotification(playerid, "Has eliminado tu ~y~'%s'~w~ de tus armas.", WEAPON_INFO[ PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_ID] ][weapon_info_NAME]);
				new string[64];
				format(string, sizeof string, "tira su %s al suelo", WEAPON_INFO[ PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_ID] ][weapon_info_NAME]);
				Auto_SendPlayerAction(playerid, string);

				RemovePlayerSlotWeapon(playerid, PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT], true);
			}
			return 1;
		}
		case DIALOG_ANIMS:
		{
		    if(response)
		    {
		        switch(listitem)
		        {
		            case 0: SetPlayerSpecialAction(playerid,SPECIAL_ACTION_HANDSUP);//rendirse
		            case 1: ApplyAnimation(playerid, "BLOWJOBZ", "BJ_COUCH_LOOP_W", 4.0, 1, 1, 1, 0, 0);//blowjob
		            case 2: ApplyAnimation(playerid,"PED","BIKE_fallR",4.0,0,1,1,1,0);//rodar
		            case 3: ApplyAnimation(playerid,"PED", "WALK_DRUNK",4.0,1,1,1,1,500);//borracho
		            case 4: ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 1, 1, 0,0);//bomba
		            case 5: ApplyAnimation( playerid,"ped", "ARRESTgun", 4.0, 0, 1, 1, 1,500);//apuntar
		            case 6: ApplyAnimation(playerid, "RAPPING", "Laugh_01", 4.0, 0, 0, 0, 0,0);//reir
		            case 7: ApplyAnimation(playerid, "SHOP", "ROB_Loop_Threat", 4.0, 0, 0, 0, 1,500);//amenazar
		            case 8: ApplyAnimation(playerid, "PAULNMAC", "wank_loop", 4.0, 1, 0, 0, 1, 0);//paja
		            case 9: ApplyAnimation(playerid, "SWEET", "LaFin_Sweet", 4.0, 0, 1, 1, 1, 0);//herido
		            case 10: ApplyAnimation(playerid, "COP_AMBIENT", "Coplook_loop", 4.0, 1, 1, 1, 0, 4000);//cruzarbrazos
		            case 11: ApplyAnimation(playerid,"SUNBATHE", "Lay_Bac_in", 4.0, 0, 0, 0, 1, 0);//recostarse
		            case 12: ApplyAnimation(playerid, "ped", "cower", 4.0, 1, 0, 0, 0, 0);//crubrirse
		            case 13: ApplyAnimation(playerid, "FOOD", "EAT_Vomit_P", 3.0, 0, 0, 0, 0, 0);//vomitar
		            case 14: ApplyAnimation(playerid, "FOOD", "EAT_Burger", 3.00, 0, 0, 0, 0, 0);//comer
		            case 15: ApplyAnimation(playerid, "KISSING", "BD_GF_Wave", 3.0, 0, 0, 0, 0, 0);//despedir
		            case 16: ApplyAnimation(playerid, "SWEET", "sweet_ass_slap", 4.0, 0, 0, 0, 0, 0);//nalgada
		            case 17: ApplyAnimation(playerid, "WUZI", "CS_Dead_Guy", 4.0, 0, 0, 0, 1, 0);//agonizar
		            case 18: ApplyAnimation(playerid, "KISSING", "Playa_Kiss_02", 4.0, 0, 0, 0, 0, 0);//besar
		            case 19: ApplyAnimation(playerid, "CRACK", "crckdeth2", 4.0, 0, 0, 0, 1, 0);//crack
		            case 20: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_PISSING);//mear
		            case 21: ApplyAnimation(playerid, "SUNBATHE", "ParkSit_M_in", 4.000000, 0, 1, 1, 1, 0);//sentarse
		            case 22: ApplyAnimation( playerid,"ped", "fucku", 4.0, 0, 1, 1, 1, 1 );//fucku
		            case 23: ApplyAnimation(playerid, "ped", "phone_in", 4.000000, 0, 0, 0, 1, 4000);//llamar
		            case 24: ApplyAnimation(playerid, "PARK", "Tai_Chi_Loop",  4.1,7,5,1,1,1);//taichi
		            case 25: ApplyAnimation(playerid, "BAR", "dnk_stndM_loop", 4.0, 0, 1, 1, 0, 4000);//beber
		            case 26: ApplyAnimation(playerid, "GYMNASIUM", "gym_shadowbox",  4.1,7,5,1,1,1);//boxear
		            case 27: ApplyAnimation(playerid,"GANGS","hndshkfa_swt",4.1,0,0,0,0,0);//saludar
		            case 28: ApplyAnimation(playerid,"GRAVEYARD","mrnF_loop",4.1,0,0,0,0,0);//llorar
		            case 29: ApplyAnimation(playerid,"INT_HOUSE","BED_In_R",4.1,0,0,0,1,0);//dormir
		            case 30: ApplyAnimation(playerid,"RAPPING","RAP_B_Loop",4.0,1,0,0,0,8000);//rapear
		            case 31: ApplyAnimation(playerid,"STRIP","strip_A",4.1,7,5,1,1,1);//strip
		            case 32: ApplyAnimation(playerid,"DANCING","DAN_Down_A",4.1,7,5,1,1,1);//bailar
		            case 33: ApplyAnimation(playerid,"ON_LOOKERS","shout_02",4.1,7,5,1,1,1);//alentar
		            case 34: ApplyAnimation(playerid,"PED","IDLE_chat",4.1,7,5,1,1,1);//hablar
					case 35: ApplyAnimation(playerid, "ped", "SEAT_down", 4.000000, 0, 1, 1, 1, 0);//asiento
		        }
				SendNotification(playerid, "Para detener la animacion utiliza ~b~/parar~w~.");
			}
			return 1;
		}
		case DIALOG_TRICKS_FOOD:
		{
			if(response)
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return SendNotification(playerid, "Has tardado mucho en aceptarlo.");
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return SendNotification(playerid, "El vendedor ya no está en el servidor.");
				if(ACCOUNT_INFO[PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return SendNotification(playerid, "El vendedor ya no está en el servidor.");

				new Float:pos[3]; GetPlayerPos(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], pos[0], pos[1], pos[2]);
				if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "El vendedor está demasiado lejos.");
				if(PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "El vendedor no está disponible.");
				

				new to_player_slot = GetEmptyPlayerPocketSlot(playerid);
				if(to_player_slot == -1)
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendNotification(playerid, "No tienes espacio en tus ~r~/alimentos~w~ para comprar esto.");
					return 1;
				}
				
				if(GivePlayerCash(playerid, -PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, true) && GivePlayerCash(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, false)) {
					TransferPlayerPocketObject(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA], playerid, to_player_slot);
					SendFormatNotification(playerid, "Te has gastado ~g~%s$~w~ con esta compra.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));
					SendFormatNotification(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "Has ganado ~g~%s$~w~ con esta venta.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));
					
					new action[64];
					format(action, sizeof action, "y %s llegan a un acuerdo.", PLAYER_TEMP[playerid][pt_RP_NAME]);
					Auto_SendPlayerAction(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], action);
				}
			}
			else
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return 1;
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return 1;
				if(ACCOUNT_INFO[PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID]][ac_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return 1;

				SendNotification(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "El comprador no ha aceptado tu trato.");
			}
			return 1;
		}
		case DIALOG_TRICKS_MEDICINE:
		{
			if(response)
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return SendNotification(playerid, "Has tardardo mucho en aceptarlo.");
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return SendNotification(playerid, "El vendedor ya no está en el servidor.");
				if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return SendNotification(playerid, "El vendedor ya no está en el servidor.");

				new Float:pos[3]; GetPlayerPos(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], pos[0], pos[1], pos[2]);
				if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "El vendedor está demasiado lejos.");
				if(PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "El vendedor no está disponible.");

				
				if(GivePlayerCash(playerid, -PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, true) && GivePlayerCash(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, false)) {
					PI[playerid][ac_MEDICINE] += PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA];
					PI[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_MEDICINE] -= PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA];
				
					SendFormatNotification(playerid, "Has gastado ~g~%s$~w~ con esta compra.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));
					SendFormatNotification(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "Has ganado ~g~%s$~w~ con esta venta.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));
					
					new action[64];
					format(action, sizeof action, "y %s llegan a un acuerdo.", PLAYER_TEMP[playerid][pt_RP_NAME]);
					Auto_SendPlayerAction(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], action);
				}
			}
			else
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return 1;
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return 1;
				if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return 1;

				SendNotification(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "El comprador no ha aceptado tu trato.");
			}
			return 1;
		}
		case DIALOG_TRICKS_CANNABIS:
		{
			if(response)
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return SendNotification(playerid, "Has tardardo mucho en aceptarlo.");
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return SendNotification(playerid, "El vendedor ya no está en el servidor.");
				if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return SendNotification(playerid, "El vendedor ya no está en el servidor.");

				new Float:pos[3]; GetPlayerPos(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], pos[0], pos[1], pos[2]);
				if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "El vendedor está demasiado lejos.");
				if(PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "El vendedor no está disponible.");				
				
				if(GivePlayerCash(playerid, -PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, true) && GivePlayerCash(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, false)) {
					PI[playerid][ac_CANNABIS] += PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA];
					PI[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_CANNABIS] -= PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA];
				
					SendFormatNotification(playerid, "Has gastado ~g~%s$~w~ con esta compra.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));
					SendFormatNotification(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "Has ganado ~g~%s$~w~ con esta venta.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));
					
					new action[64];
					format(action, sizeof action, "y %s llegan a un acuerdo.", PLAYER_TEMP[playerid][pt_RP_NAME]);
					Auto_SendPlayerAction(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], action);
				}
			}
			else
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return 1;
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return 1;
				if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return 1;

				SendNotification(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "El comprador no ha aceptado tu trato.");
			}
			return 1;
		}
		case DIALOG_TRICKS_CRACK:
		{
			if(response)
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return SendNotification(playerid, "Has tardardo mucho en aceptarlo.");
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return SendNotification(playerid, "El vendedor ya no está en el servidor.");
				if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return SendNotification(playerid, "El vendedor ya no está en el servidor.");

				new Float:pos[3]; GetPlayerPos(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], pos[0], pos[1], pos[2]);
				if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "El vendedor está demasiado lejos.");
				if(PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "El vendedor no está disponible.");
				
				if(GivePlayerCash(playerid, -PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, true) && GivePlayerCash(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, false)) {
					PI[playerid][ac_CRACK] += PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA];
					PI[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_CRACK] -= PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA];
				
					SendFormatNotification(playerid, "Has gastado ~g~%s$~w~ con esta compra.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));
					SendFormatNotification(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "Has ganado ~g~%s$~w~ con esta venta.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));
					
					new action[64];
					format(action, sizeof action, "y %s llegan a un acuerdo.", PLAYER_TEMP[playerid][pt_RP_NAME]);
					Auto_SendPlayerAction(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], action);
				}
			}
			else
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return 1;
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return 1;
				if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return 1;

				SendNotification(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "El comprador no ha aceptado tu trato.");
			}
			return 1;
		}
		case DIALOG_TRICKS_SU:
		{
			if(response)
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return SendNotification(playerid, "Has tardardo mucho en aceptarlo.");
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return SendNotification(playerid, "El vendedor ya no está en el servidor.");
				if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return SendNotification(playerid, "El vendedor ya no está en el servidor.");

				new Float:pos[3]; GetPlayerPos(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], pos[0], pos[1], pos[2]);
				if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "El vendedor está demasiado lejos.");
				if(PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "El vendedor no está disponible.");

				
				PI[playerid][ac_COINS] += PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA];
				PI[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_COINS] -= PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA];

				if(GivePlayerCash(playerid, -PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, true) && GivePlayerCash(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, false)) {
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET coins = %d WHERE id = %d;", PI[playerid][ac_COINS], ACCOUNT_INFO[playerid][ac_ID]);
					mysql_tquery(srp_db, QUERY_BUFFER);

					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET coins = %d WHERE id = %d;", PI[PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID]][ac_COINS], ACCOUNT_INFO[PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID]][ac_ID]);
					mysql_tquery(srp_db, QUERY_BUFFER);

					SendFormatNotification(playerid, "Has gastado ~g~%s$~w~ con esta compra.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));
					SendFormatNotification(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "Has ganado ~g~%s$~w~ con esta venta.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));
					
					new action[64];
					format(action, sizeof action, "y %s llegan a un acuerdo.", PLAYER_TEMP[playerid][pt_RP_NAME]);
					Auto_SendPlayerAction(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], action);
				}
			}
			else
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return 1;
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return 1;
				if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return 1;

				SendNotification(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "El comprador no ha aceptado tu trato.");
			}
			return 1;
		}
		case DIALOG_TRICKS_WEAPON:
		{
			if(response)
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return SendNotification(playerid, "Has tardardo mucho en aceptarlo.");
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return SendNotification(playerid, "El vendedor ya no está en el servidor.");
				if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return SendNotification(playerid, "El vendedor ya no está en el servidor.");

				new Float:pos[3]; GetPlayerPos(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], pos[0], pos[1], pos[2]);
				if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "El vendedor está demasiado lejos.");
				if(PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "El vendedor no está disponible.");

				
				if(PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA] ][player_weapon_VALID])
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendFormatNotification(playerid, "Para comprar esta arma tienes que deshacerte de tu '%s' (%d) para tener espacio.", WEAPON_INFO[ PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA] ][player_weapon_ID] ][weapon_info_NAME], PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA]);
					return 1;
				}
				
				if(GivePlayerCash(playerid, -PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, true) && GivePlayerCash(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, false)) {
					TransferPlayerWeapon(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA], playerid);

					SendFormatNotification(playerid, "Has gastado ~g~%s$~w~ con esta compra.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));
					SendFormatNotification(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "Has ganado ~g~%s$~w~ con esta venta.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));
					
					new action[64];
					format(action, sizeof action, "y %s llegan a un acuerdo.", PLAYER_TEMP[playerid][pt_RP_NAME]);
					Auto_SendPlayerAction(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], action);
				}
			}
			else
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return 1;
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return 1;
				if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return 1;

				SendNotification(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "El comprador no ha aceptado tu trato.");
			}
			return 1;
		}
		case DIALOG_VEHICLE_BOOT:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == MAX_BOOT_SLOTS + 20)
				{
					ShowDialog(playerid, DIALOG_VEHICLE_BOOT_DELETE_ALL);
					return 1;
				}
				
				PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
				ShowDialog(playerid, DIALOG_VEHICLE_BOOT_OPTIONS);
			}
			
		}
		case DIALOG_VEHICLE_BOOT_DELETE_ALL:
		{
			if(response)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM vboot WHERE id_vehicle = %d;", PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][player_vehicle_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				new temp_VEHICLE_BOOT[enum_VEHICLE_BOOT];
				for(new i = 0; i != MAX_BOOT_SLOTS; i ++) VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][i] = temp_VEHICLE_BOOT;
		
				SendNotification(playerid, "Ha eliminado todo de este maletero.");
				Auto_SendPlayerAction(playerid, "tira todas las armas de su maltero");
			}
			return 1;
		}
		case DIALOG_VEHICLE_BOOT_OPTIONS:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0:
					{
						if(!VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_VALID]) return SendNotification(playerid, "No hay nada en ese slot.");

						new string[128];
						switch(VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_TYPE]) {
							case BOOT_TYPE_WEAPON: {
								new to_slot = WEAPON_INFO[ VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_INT] ][weapon_info_SLOT];
								if(PLAYER_WEAPONS[playerid][to_slot][player_weapon_VALID])
								{
									PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
									SendFormatNotification(playerid, "Para sacar esta arma tienes que deshacerte de tu ~r~\"%s (%d)\"~w~ para tener espacio.", WEAPON_INFO[ PLAYER_WEAPONS[playerid][ to_slot ][player_weapon_ID] ][weapon_info_NAME], to_slot);
									return 1;
								}
								GivePlayerWeaponEx(playerid, VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_INT], VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_INT_EXTRA]);
								RegisterNewPlayerWeapon(playerid, to_slot);
								format(string, sizeof string, "saca un(a) %s del maletero de su vehículo.", WEAPON_INFO[ VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_INT] ][weapon_info_NAME]);
							}
							case BOOT_TYPE_MEDICINES: {
								PI[playerid][ac_MEDICINE] += VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_INT];
								format(string, sizeof string, "saca medicamentos del maletero de su vehículo.");
							}
							case BOOT_TYPE_CANNABIS: {
								PI[playerid][ac_CANNABIS] += VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_INT];
								format(string, sizeof string, "saca marihuana del maletero de su vehículo.");
							}
							case BOOT_TYPE_CRACK: {
								PI[playerid][ac_CRACK] += VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_INT];
								format(string, sizeof string, "saca crack del maletero de su vehículo.");
							}
						}

						mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM vboot WHERE id = %d;", VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_OBJECT_ID]);
						mysql_tquery(srp_db, QUERY_BUFFER);

						new temp_VEHICLE_BOOT[enum_VEHICLE_BOOT]; VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ] = temp_VEHICLE_BOOT;
						Auto_SendPlayerAction(playerid, string);
					}
					case 1: ShowDialog(playerid, DIALOG_VEHICLE_BOOT_DELETE);
				}
			}
			else ShowDialog(playerid, DIALOG_VEHICLE_BOOT);
			return 1;
		}
		case DIALOG_VEHICLE_BOOT_DELETE:
		{
			if(response)
			{
				SendNotification(playerid, "Eliminado.");
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM vboot WHERE id = %d;", VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_OBJECT_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				new temp_VEHICLE_BOOT[enum_VEHICLE_BOOT]; VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ] = temp_VEHICLE_BOOT;
			}
			else ShowDialog(playerid, DIALOG_VEHICLE_BOOT_OPTIONS);
			return 1;
		}
		case DIALOG_SELECT_POLICE_SKIN:
		{
			if(response)
			{
				if(!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) return 1;
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				if(POLICE_SKINS[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][police_skin_RANK] > PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL]) return ShowDialog(playerid, dialogid);

				PI[playerid][ac_POLICE_DUTY] = POLICE_SKINS[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][police_skin_SKIN];
				CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_POLICE, INVALID_VEHICLE_ID);
			}
			return 1;
		}
		case DIALOG_POLICE_RADIOS:
		{
			if(response)
			{
				PLAYER_TEMP[playerid][pt_POLICE_RADIO] = listitem;
				SendFormatNotification(playerid, "Tu radio ha sido ajustada a la frecuencia %d.", listitem + 1);
			}
			return 1;
		}
		case DIALOG_POLICE_LIST:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -2) // Siguiente
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id_player) FROM pworks WHERE pworks.id_work = %d AND pworks.`set` = 1;", WORK_POLICE);
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
				}
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -3) //Anterior
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id_player) FROM pworks WHERE pworks.id_work = %d AND pworks.`set` = 1;", WORK_POLICE);
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
				}
				else
				{
					if(PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] >= 11)
					{
						PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
						ShowDialog(playerid, DIALOG_POLICE_MODIFY);
					}
				}
			}
			return 1;
		}
		case DIALOG_POLICE_MODIFY:
		{
			if(response)
			{
				if(listitem > PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL])
				{
					SendNotification(playerid, "El rango que has seleccionado es superior al tuyo.");
					return 1;
				}

				inline OnDialogQueryLoad()
				{
					new rows;
					if(cache_get_row_count(rows))
					{
						if(rows)
						{
							new level, name[24], connected, pid;
							cache_get_value_name_int(0, "level", level);
							cache_get_value_name(0, "name", name);
							cache_get_value_name_int(0, "connected", connected);
							cache_get_value_name_int(0, "playerid", pid);

							if(level > PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL]) SendNotification(playerid, "No puedes modificar el rango de este policía porque es un rango superior al tuyo.");
							else
							{
								SendFormatNotification(playerid, "El nuevo rango de %s es: '%s'.", name, POLICE_RANKS[listitem]);
								if(listitem == 0)
								{
									mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pworks SET `set` = 0, level = 0 WHERE id_player = %d AND id_work = %d;", PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID], WORK_POLICE);
									mysql_tquery(srp_db, QUERY_BUFFER);

									mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET police_duty = 0, placa_pd = 0 WHERE id = %d;", PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID]);
									mysql_tquery(srp_db, QUERY_BUFFER);

									if(connected)
									{
										if(PLAYER_TEMP[pid][pt_WORKING_IN] == WORK_POLICE) CallLocalFunction("EndPlayerJob", "iib", pid, PLAYER_TEMP[pid][pt_WORKING_IN], true);
										SendFormatNotification(pid, "El %s %s te ha expulsado del cuerpo de policía.", POLICE_RANKS[ PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] ], PLAYER_TEMP[playerid][pt_RP_NAME]);

										PLAYER_WORKS[pid][WORK_POLICE][pwork_SET] = 0;
										PLAYER_WORKS[pid][WORK_POLICE][pwork_LEVEL] = 0;
										PI[pid][ac_POLICE_DUTY] = 0;
										PI[pid][ac_PLACA_PD] = 0;
									}
								}
								else
								{
									mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pworks SET level = %d WHERE id_player = %d AND id_work = %d;", listitem, PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID], WORK_POLICE);
									mysql_tquery(srp_db, QUERY_BUFFER);

									if(connected)
									{
										PLAYER_WORKS[pid][WORK_POLICE][pwork_LEVEL] = listitem;
										SendFormatNotification(pid, "{"#SILVER_COLOR"}El %s %s ha modificado tu rango del cuerpo de policía a '%s'.", POLICE_RANKS[ PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL] ], PLAYER_TEMP[playerid][pt_RP_NAME], POLICE_RANKS[listitem]);
									}
								}
							}
						}
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT pworks.level, player.name, player.connected, player.playerid FROM pworks INNER JOIN player ON pworks.id_player = player.id WHERE pworks.id_player = %d AND pworks.id_work = %d;", PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID], WORK_POLICE);
				MySQL_TQueryInline(srp_db, using inline OnDialogQueryLoad, QUERY_BUFFER);
			}
			else ShowDialog(playerid, DIALOG_POLICE_LIST);
			return 1;
		}
		case DIALOG_POLICE_SHOP:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: ShowDialog(playerid, DIALOG_POLICE_WEAPONS);
					case 1: ShowDialog(playerid, DIALOG_POLICE_SELECT_WEAPON);
					case 2: ShowDialog(playerid, DIALOG_POLICE_ARMOUR);
				}
			}
			return 1;
		}
		case DIALOG_POLICE_SELECT_WEAPON:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				
				PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
				
				if(!WEAPON_INFO[ PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_ID] ][weapon_info_AMMO])
				{
					SendNotification(playerid, "Este tipo de arma no necesita munición.");
					return 1;
				}
				
				ShowDialog(playerid, DIALOG_POLICE_AMMO);
			}
			return 1;
		}
		case DIALOG_POLICE_AMMO:
		{
			if(response)
			{
				if(sscanf(inputtext, "d", inputtext[0])) return ShowDialog(playerid, dialogid);
				if(inputtext[0] <= 0 || inputtext[0] > 9999) return SendNotification(playerid, "La cantidad de munición no es correcta.");
				
				new price = 2 * inputtext[0];
				if(price > PI[playerid][ac_CASH]) return SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar la munición.", number_format_thousand(price - PI[playerid][ac_CASH]));
				
				if(PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_AMMO] + inputtext[0] > 9999)
				{
					SendNotification(playerid, "A esta arma no le entra tanta municion.");
					return 1;
				}
				
				if(GivePlayerCash(playerid, -price, true, true)) {
					GivePlayerWeaponEx(playerid, PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_ID], inputtext[0]);
					
					SendFormatNotification(playerid, "Has comprado ~y~\"%s\"~w~ balas para tu ~y~\"%s\"~w~ por ~g~\"%s$\"~w~.", number_format_thousand(inputtext[0]), WEAPON_INFO[ PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_ID] ][weapon_info_NAME], number_format_thousand(price));
				}
			}
			return 1;
		}
		case DIALOG_POLICE_ARMOUR:
		{
			if(response)
			{
				if(300 > PI[playerid][ac_CASH]) return SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar la munición.", number_format_thousand(300 - PI[playerid][ac_CASH]));
				
				GetPlayerArmour(playerid, PI[playerid][ac_ARMOUR]);
				if(PI[playerid][ac_ARMOUR] > 99.5) return SendNotification(playerid, "Ya tienes un chaleco antibalas");
				
				if(GivePlayerCash(playerid, -300, true, true)) {
					SetPlayerArmourEx(playerid, 100.0);
					SendNotification(playerid, "Has comprado un ~b~chaleco antibalas~w~ por 300 dólares.");
				}
			}
			return 1;
		}
		case DIALOG_POLICE_WEAPONS:
		{
			if(response)
			{
				if(POLICE_SHOP_WEAPONS[listitem][police_shop_RANK] > PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL])
				{
					SendFormatNotification(playerid, "Necesitas el rango ~r~%s~w~ para poder comprar esta arma.", POLICE_RANKS[ POLICE_SHOP_WEAPONS[listitem][police_shop_RANK] ]);
					return 1;
				}
				
				
				if(PI[playerid][ac_CASH] >= POLICE_SHOP_WEAPONS[listitem][police_shop_WEAPON_PRICE])
				{
					new weapon_slot = WEAPON_INFO[ POLICE_SHOP_WEAPONS[listitem][police_shop_WEAPON_ID] ][weapon_info_SLOT];
					if(PLAYER_WEAPONS[playerid][weapon_slot][player_weapon_ID] != 0)
					{
						SendFormatNotification(playerid, "Para comprar esta arma debes deshacerte de tu ~r~\"%s (%d)\" de tu inventario.", WEAPON_INFO[ PLAYER_WEAPONS[playerid][weapon_slot][player_weapon_ID] ][weapon_info_NAME], weapon_slot);
						return 1;
					}
					
					if(GivePlayerCash(playerid, -POLICE_SHOP_WEAPONS[listitem][police_shop_WEAPON_PRICE], true, true)) {
						if(WEAPON_INFO[ POLICE_SHOP_WEAPONS[listitem][police_shop_WEAPON_ID] ][weapon_info_AMMO]) GivePlayerWeaponEx(playerid, POLICE_SHOP_WEAPONS[listitem][police_shop_WEAPON_ID], 100);
						else GivePlayerWeaponEx(playerid, POLICE_SHOP_WEAPONS[listitem][police_shop_WEAPON_ID], 1);

						RegisterNewPlayerWeapon(playerid, weapon_slot); 
						SendFormatNotification(playerid, "Arma ~y~\"%s\"~w~ comprada por ~g~\"%s$\"~w~.", WEAPON_INFO[ POLICE_SHOP_WEAPONS[listitem][police_shop_WEAPON_ID] ][weapon_info_NAME], number_format_thousand(POLICE_SHOP_WEAPONS[listitem][police_shop_WEAPON_PRICE]));
					}
				}
				else
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendFormatNotification(playerid, "No tienes suficiente dinero, te faltan ~r~\"%s\"~w~ para comprar esta arma.", number_format_thousand(POLICE_SHOP_WEAPONS[listitem][police_shop_WEAPON_PRICE] - PI[playerid][ac_CASH]));
				}
			}
			return 1;
		}
		case DIALOG_HELP:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: ShowDialog(playerid, DIALOG_HELP_GENERAL);
					case 1: ShowDialog(playerid, DIALOG_HELP_INFO);
					case 2: ShowDialog(playerid, DIALOG_HELP_CHARACTER);
					case 3: ShowDialog(playerid, DIALOG_HELP_PROPERTIES);
					case 4: ShowDialog(playerid, DIALOG_HELP_REPORT);
					case 5: ShowDialog(playerid, DIALOG_HELP_WORKS);
					case 6: ShowDialog(playerid, DIALOG_HELP_VEHICLES);
					case 7: ShowDialog(playerid, DIALOG_HELP_CREWS);
					case 8: ShowDialog(playerid, DIALOG_HELP_SU);
					case 9: ShowDialog(playerid, DIALOG_HELP_SECURE_LOGIN);
				}
			}
			return 1;
		}
		case DIALOG_HELP_WORKS:
		{
			if(response)
			{
				new caption[45], dialog[1800], work = listitem + 1;
				format(caption, sizeof caption, "Trabajo - %c%s", toupper(work_info[work][work_info_NAME][0]), work_info[work][work_info_NAME][1]);
			
				new header[350];
				if(work_info[work][work_info_TYPE] != WORK_TYPE_FAMILY)
				{
					if(work_info[work][work_info_EXTRA_PAY] > 0 && work_info[work][work_info_EXTRA_PAY_EXP] > 0) // con paga extra
					{
						if(!work_info[work][work_info_EXTRA_PAY_LIMIT]) // sin limite
						{
							format(header, sizeof header, 
						
								"\
									Trabajo: %s\n\
									Tipo: %s\n\
									Nivel necesario: %d\n\
									Paga extra: por cada %s trabajos realizados recibes %s$ extra.\n\
									Puedes localizar donde adquirir este trabajo desde el GPS.\n\
								",
									work_info[work][work_info_NAME],
									(work_info[work][work_info_TYPE] == WORK_TYPE_NORMAL) ? ("normal") : ("complementario"),
									work_info[work][work_info_LEVEL],
									number_format_thousand(work_info[work][work_info_EXTRA_PAY_EXP]),
									number_format_thousand(work_info[work][work_info_EXTRA_PAY])
							);
						}
						else // con limite
						{
							format(header, sizeof header, 
						
								"\
									Trabajo: %s\n\
									Tipo: %s\n\
									Nivel necesario: %d\n\
									Paga extra: por cada %s trabajos realizados recibes %s$ extra con un límite de %s$.\n\
									Puedes localizar donde adquirir este trabajo desde el GPS.\n\
								",
									work_info[work][work_info_NAME],
									(work_info[work][work_info_TYPE] == WORK_TYPE_NORMAL) ? ("normal") : ("complementario"),
									work_info[work][work_info_LEVEL],
									number_format_thousand(work_info[work][work_info_EXTRA_PAY_EXP]),
									number_format_thousand(work_info[work][work_info_EXTRA_PAY]),
									number_format_thousand(work_info[work][work_info_EXTRA_PAY_LIMIT])
							);
						}
					}
					else
					{
						format(header, sizeof header, 
						
							"\
								Trabajo: %s\n\
								Tipo: %s\n\
								Nivel necesario: %d\n\
								Puedes localizar donde adquirir este trabajo desde el GPS.\n\
							",
								work_info[work][work_info_NAME],
								(work_info[work][work_info_TYPE] == WORK_TYPE_NORMAL) ? ("normal") : ("complementario"),
								work_info[work][work_info_LEVEL]
						);
					}
				}
				
				switch(work)
				{
					case WORK_TAXI:
					{
						format(dialog, sizeof dialog, 
						
							"\
							%s\
							\n\
							Solo súbete a un taxi y empieza a trabajar, recibirás las llamadas de los clientes.\n\
							Las personas que hayan llamado a la central de taxi aparecerán marcados de color amarillo.\n\
							Si tienes un GPS puedes utilizar /mapa para ver las personas que necesitan un taxi.\n\
							\n\
							El taxímetro se pondrá en marcha para el primer pasajero que se suba al taxi, el precio se\n\
							mide en precio por km, puedes ajustarlo con el comando /tarifa.\n\
							\n\
							",
								header
						);
					}
					case WORK_TRUCK:
					{
						format(dialog, sizeof dialog, 
						
							"\
							%s\
							\n\
							Súbete a cualquier camión de la empresa y ve a cargarlo en cualquier punto de carga.\n\
							Según tu experiencia en el trabajo podrás transportar diferentes productos de más valor.\n\
							\n\
							",
								header
						);
					}
					case WORK_MECHANIC:
					{
						format(dialog, sizeof dialog, 
						
							"\
							%s\
							\n\
							Para empezar a trabajar ve al taller y utiliza /mecanico para empezar a trabajar o dejar de trabajar.\n\
							Con este trabajo podrás reparar los vehículos dañados en el taller o ir a arreglarlos por San Andreas.\n\
							Las personas que hayan llamado al mecánico aparecerán marcados en el mapa.\n\
							Si tienes un GPS puedes utilizar /mapa para ver las personas que necesitan un mecánico.\n\
							Para reparar o tunear un vehículo usa /vmenu tras elegir una opción se le ofrecerá al dueño del vehículo\n\
							en caso de ser un vehículo personal, si no se le ofrecerá al conductor o el último conductor del vehículo.\n\
							\n\
							Puedes utilizar los vehículos de tu trabajo o tu vehículo personal.\n\
							Para cualquier reparación o modificación necesitarás piezas que puedes comprar en el taller.\n\
							Para reparar o modificar un vehículo utiliza cerca del vehículo el comando /vmenu.\n\
							\n\
							Si el vehículo no está en el mecánico y se puede remolcar tendrás que remolcarlo hasta el mecánico\n\
							para repararlo, se usa el comando /remolcar.\n\
							\n\
							En el taller también podrás pintar los vehículos.\n\
							Para tunearlos o remover componentes tendrás que haber realizado al menos 100 reparaciones en vehículos ajenos.\n\
							\n\
							",
								header
						);
					}
					case WORK_HARVESTER:
					{
						format(dialog, sizeof dialog, 
						
							"\
							%s\
							\n\
							Súbete a una cosechadora y empieza a cosechar.\n\
							\n\
							",
								header
						);
					}
					case WORK_FUMIGATOR:
					{
						format(dialog, sizeof dialog, 
						
							"\
							%s\
							\n\
							Súbete a un avión y empieza a fumigar.\n\
							\n\
							",
								header
						);
					}
					case WORK_TRASH:
					{
						format(dialog, sizeof dialog, 
						
							"\
							%s\
							\n\
							Para este trabajo se necesitan 2 personas, uno que conduzca el camión y otro que recoja la basura.\n\
							Para empezar a trabajar sube a alguien a un camión y selecciona la ruta que queráis realizar.\n\
							\n\
							Dependiendo de tu experiencia recogerás más rápido la basura.\n\
							\n\
							",
								header
						);
					}
					case WORK_LUMBERJACK:
					{
						format(dialog, sizeof dialog, 
						
							"\
							%s\
							\n\
							Para empezar a trabajar ve a los árboles asignados y empieza a talar.\n\
							Dependiendo de tu experiencia tu velocidad talando irá aumentando.\n\
							\n\
							",
								header
						);
					}
					case WORK_FARMER:
					{
						format(dialog, sizeof dialog, 
						
							"\
							%s\
							\n\
							Necesitarás semillas para poder plantar que puedes comprar en la granja.\n\
							Dependiendo de tu experiencia tu velocidad plantando irá aumentando.\n\
							\n\
							Después de que la planta crezca tienes 5 minutos para recogerla si no la planta se secará y se destruirá.\n\
							Cuando termines de plantar espera a que la planta termine de crecer, para recogerla.\n\
							Una vez recogida puedes vender el producto adquirido o usarlo para consumo propio.\n\
							\n\
							Cuida tus plantas, la gente puede destuirlas si le disparan.\n\
							\n\
							",
								header
						);
					}
					case WORK_POLICE:
					{
						format(dialog, sizeof dialog, 
						
							"\
							%s\
							\n\
							Los policías en servicio aparecerán de color azul.\n\
							Utiliza /policias para ver la lista de policías, los rangos altos podrán desde aquí\n\
							bajar, subir el rango de los policías, también pueden expulsarlos del cuerpo.\n\
							\n\
							Para asignar un BYC una persona utiliza el comando /abyc.\n\
							Usa /byc para ver los BYC de todos o de una persona, también para eliminarlos.\n\
							Utiliza /esposar para esposar o desesposar a alguien que tenga nivel de búsqueda.\n\
							Utiliza /revisar para ver las pertenencias del jugador esposado, utiliza /requisar para requisar las pertenencias ilegales.\n\
							Utiliza /arrestar en tu vehículo personal para detener a alguien que está esposado.\n\
							Utiliza /ref para pedir refuerzos.\n\
							Para utiliza el megáfono utiliza /m dentro de un vehículo policial.\n\
							Para entregar a alguien tiene que estar arrestado en un vehículo policial\n\
							y llevarlo a cualquiera de las 3 comisarías de San Andreas al parking y utilizar /entregar.\n\
							\n\
							Usa /control para añadir objetos como barreras etc.\n\
							Usa /allanar para entrar a una casa.\n\
							Usa /cepo para multar a un vehículo mal aparcado.\n\
							\n\
							Para ponerte de servicio ve a cualquier comisaría y busca el vestuario para /policia.\n\
							Los policías pueden equiparse en la comisaría.\n\
							Para utilizar la radio debes estar de servicio, puedes seleccionar la frecuencia con /frecuencias, uso: !texto.\n\
							Puedes abrir cualquier puerta de la comisaría pulsando la tecla F cerca de la puerta.\n\
							Los policías recibirán la paga en el payday, su paga dependerá de su rango.\n\
							\n\
							",
								header
						);
					}
					case WORK_PIZZA:
					{
						format(dialog, sizeof dialog, 
						
							"\
							%s\
							\n\
							Súbete a una moto y empieza a repartir pizzas.\n\
							\n\
							",
								header
						);
					}
					case WORK_MEDIC:
					{
						format(dialog, sizeof dialog, 
						
							"\
							%s\
							\n\
							Ponte de servicio en cualquier hospital, tras esto verás a las personas que necesiten un médico\n\
							marcadas en el mapa, ve rápido hasta ellas para poder salvarlos a tiempo.\n\
							Para salvar la vida a alguien que este herido usa /curar.\n\
							\n\
							Cuando estás de servicio solo puedes conducir ambulancias.\n\
							\n\
							",
								header
						);
					}
					case WORK_WAREHOUSE:
					{
						format(dialog, sizeof dialog, 
						
							"\
							%s\
							\n\
							Para empezar sube a la planta de arriba donde puedes ver los pedidos en espera.\n\
							Selecciona uno de estos pedidos y coge todos los productos para llevarlos a la cinta transportadora.\n\
							\n\
							",
								header
						);
					}
					case WORK_DELIVERYMAN:
					{
						format(dialog, sizeof dialog, 
						
							"\
							%s\
							\n\
							Súbete a cualquier furgoneta de la empresa y ve a cargarla en cualquier punto de carga.\n\
							Según tu experiencia en el trabajo ganarás más dinero por cada reparto.\n\
							\n\
							",
								header
						);
					}
				}
				
				ShowPlayerDialog(playerid, DIALOG_INFO, DIALOG_STYLE_MSGBOX, caption, dialog, "Cerrar", "");
			}
			return 1;
		}
		case DIALOG_ADMIN_LIST:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -2) // Siguiente
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					MySQL_TQueryInline(srp_db,  using inline OnCountQueryLoad, "SELECT COUNT(id) FROM player WHERE admin_level > 0;");
				}
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -3) //Anterior
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, "SELECT COUNT(id) FROM player WHERE admin_level > 0;");
				}
				else
				{
					if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] >= 4)
					{
						PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
						ShowDialog(playerid, DIALOG_ADMIN_MODIFY);
					}
				}
			}
			return 1;
		}
		case DIALOG_ADMIN_MODIFY:
		{
			if(response)
			{
				if(listitem > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL])
				{
					SendNotification(playerid, "El rango que has seleccionado es superior al tuyo.");
					return 1;
				}					
				
				inline OnDialogQueryLoad()
				{
					new rows;
					if(cache_get_row_count(rows))
					{
						if(rows)
						{
							new name[24], connected, pid, admin_level;
							cache_get_value_name(0, "name", name);
							cache_get_value_name_int(0, "connected", connected);
							cache_get_value_name_int(0, "playerid", pid);
							cache_get_value_name_int(0, "admin_level", admin_level);

							if(admin_level > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) SendNotification(playerid, "No puedes modificar el rango de este admin porque es un rango superior al tuyo.");
							else
							{
								mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET admin_level = %d WHERE id = %d;", listitem, PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID]);
								mysql_tquery(srp_db, QUERY_BUFFER);
								SendFormatNotification(playerid, "El nuevo rango de %s es: '%s'.", name, ADMIN_LEVELS[listitem]);
								if(connected)
								{
									ACCOUNT_INFO[pid][ac_ADMIN_LEVEL] = listitem;
									SendFormatNotification(pid, "%s cambió tu rango administrativo a: %s.", ACCOUNT_INFO[playerid][ac_NAME], ADMIN_LEVELS[listitem]);
								}
							}
						}
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT name, connected, playerid, admin_level FROM player WHERE id = %d;", PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID]);
				MySQL_TQueryInline(srp_db, using inline OnDialogQueryLoad, QUERY_BUFFER);
			}
			else ShowDialog(playerid, DIALOG_ADMIN_LIST);
			return 1;
		}
		case DIALOG_CHANGE_PASSWORD:
		{
			if(response)
			{
				if(!strlen(inputtext)) return ShowDialog(playerid, dialogid);
				
				new password[64 + 1];
				SHA256_PassHash(inputtext, ACCOUNT_INFO[playerid][ac_SALT], password, sizeof password);
			
				if(!strcmp(password, ACCOUNT_INFO[playerid][ac_PASS], false))
				{
					ShowDialog(playerid, DIALOG_CHANGE_PASSWORD_PASS);
					PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] = 0;
				}
				else // Error
				{
					PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] ++;
					if(PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] > MAX_BAD_LOGIN_ATTEMPS) return Kick(playerid);
					SendFormatNotification(playerid, "Contraseña incorrecta, aviso %d/%d.", PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP], MAX_BAD_LOGIN_ATTEMPS);
				}
			}
			return 1;
		}
		case DIALOG_CHANGE_PASSWORD_PASS:
		{
			if(response)
			{
				if(strlen(inputtext) < MIN_PASS_LENGTH || strlen(inputtext) > MAX_PASS_LENGTH) return ShowDialog(playerid, dialogid);
				
				new salt[16];
				getRandomSalt(salt);
				format(ACCOUNT_INFO[playerid][ac_SALT], 16, "%s", salt);
				SHA256_PassHash(inputtext, ACCOUNT_INFO[playerid][ac_SALT], ACCOUNT_INFO[playerid][ac_PASS], 64 + 1);
	
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET salt = '%e', pass = '%e' WHERE id = %d;", ACCOUNT_INFO[playerid][ac_SALT], ACCOUNT_INFO[playerid][ac_PASS], ACCOUNT_INFO[playerid][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				SendNotification(playerid, "Tu contraseña ha sido cambiada correctamente.");
			}
			return 1;
		}
		case DIALOG_ANTI_CHEAT:
		{
			if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] < 4) return 1;
			
			if(response)
			{
				PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] = listitem;
				ShowDialog(playerid, DIALOG_ANTI_CHEAT_MODIFY);
			}
			return 1;
		}
		case DIALOG_ANTI_CHEAT_MODIFY:
		{
			if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] < 4) return 1;
			
			if(response)
			{
				switch(listitem)
				{
					case 0:
					{
						if(ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Enabled]) ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Enabled] = false;
						else ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Enabled] = true;
					}
					case 1: return ShowDialog(playerid, DIALOG_ANTI_CHEAT_MODIFY_DETECT);
					case 2:
					{
						if(ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Kick]) ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Kick] = false;
						else ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Kick] = true;
					}
				}
				WriteAnticheatRules();
				ShowDialog(playerid, DIALOG_ANTI_CHEAT_MODIFY);
			}
			else ShowDialog(playerid, DIALOG_ANTI_CHEAT);
			return 1;
		}
		case DIALOG_ANTI_CHEAT_MODIFY_DETECT:
		{
			if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] < 4) return 1;
			
			if(response)
			{
				new detections, seconds;
				if(sscanf(inputtext, "p<:>dd", detections, seconds))
				{
					SendNotification(playerid, "Sintaxis no válida.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(detections < 1 || detections > 99999)
				{
					SendNotification(playerid, "El número de detecciones debe ser 1 o mayor.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(seconds < 0 || seconds > 99999)
				{
					SendNotification(playerid, "Los segundos no pueden ser un valor negativo.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Detections] = detections;
				ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Interval] = seconds;
				
				WriteAnticheatRules();
				ShowDialog(playerid, DIALOG_ANTI_CHEAT_MODIFY);
			}
			else ShowDialog(playerid, DIALOG_ANTI_CHEAT_MODIFY);
			return 1;
		}
		case DIALOG_SHOP_ARTICLE_MODIFY:
		{
			if(response)
			{
				if(sscanf(inputtext, "d", inputtext[0]))
				{
					SendNotification(playerid, "Por favor introduce un valor numérico.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(inputtext[0] < 0)
				{
					SendNotification(playerid, "Por favor introduce un valor positivo.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(inputtext[0] > 1000000000)
				{
					SendNotification(playerid, "El precio es demasiado alto.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				inline OnShopUpdated()
				{
					UpdatePlayerShop(playerid);
					SendNotification(playerid, "El precio de tu artículo ha sido actualizado.");
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE shop SET price = %d WHERE id = %d;", inputtext[0], PLAYER_TEMP[playerid][pt_SHOP_SELECTED_ARTICLE_ID]);
				MySQL_TQueryInline(srp_db, using inline OnShopUpdated, QUERY_BUFFER);
			}
			return 1;
		}
		case DIALOG_SHOP_ARTICLE_REMOVE:
		{
			if(response)
			{
				inline OnShopUpdated()
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
					SendNotification(playerid, "Tu producto se ha eliminado.");
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM shop WHERE id = %d;", PLAYER_TEMP[playerid][pt_SHOP_SELECTED_ARTICLE_ID]);
				MySQL_TQueryInline(srp_db, using inline OnShopUpdated, QUERY_BUFFER);
			}
			return 1;
		}
		case DIALOG_SHOP_ADD:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0:
					{
						PLAYER_TEMP[playerid][pt_NOTARY_OPTION] = 2;
						ShowDialog(playerid, DIALOG_NOTARY_SELECT_PROPERTY);
					}
					case 1:
					{
						PLAYER_TEMP[playerid][pt_NOTARY_OPTION] = 2;
						ShowDialog(playerid, DIALOG_NOTARY_SELECT_VEHICLE);
					}
					case 2:
					{
						PLAYER_TEMP[playerid][pt_SHOP_ADD_TYPE] = SHOP_ARTICLE_TYPE_OTHER;
						PLAYER_TEMP[playerid][pt_SHOP_ADD_MODELID] = 1314;
						PLAYER_TEMP[playerid][pt_SHOP_ADD_VCOL1] = 0;
						PLAYER_TEMP[playerid][pt_SHOP_ADD_VCOL2] = 0;
						ShowDialog(playerid, DIALOG_SHOP_ADD_TEXT);
					}
				}
			}
			return 1;
		}
		case DIALOG_SHOP_ADD_TEXT:
		{
			if(response)
			{
				if(strlen(inputtext) > 24) return ShowDialog(playerid, dialogid);
				new text[24];
				if(sscanf(inputtext, "s[24]", text)) return ShowDialog(playerid, dialogid);
				if(!td_str_legal(text)) return ShowDialog(playerid, dialogid);
				
				format(PLAYER_TEMP[playerid][pt_SHOP_ADD_TEXT], 24, "%s", inputtext);
				ShowDialog(playerid, DIALOG_SHOP_ADD_PRICE);
			}
			return 1;
		}
		case DIALOG_SHOP_ADD_PRICE:
		{
			if(response)
			{
				if(sscanf(inputtext, "d", inputtext[0]))
				{
					SendNotification(playerid, "Por favor introduce un valor numérico.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(inputtext[0] < 0)
				{
					SendNotification(playerid, "Por favor introduce un valor positivo.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(inputtext[0] > 1000000000)
				{
					SendNotification(playerid, "El precio es demasiado alto.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				if(500 > PI[playerid][ac_CASH]) return SendNotification(playerid, "Necesitas 500$ para poner el anuncio.");
				
				if(GivePlayerCash(playerid, -500, true, true)) {
					AddNewAd(playerid, PLAYER_TEMP[playerid][pt_SHOP_ADD_TYPE], inputtext[0], PLAYER_TEMP[playerid][pt_SHOP_ADD_TEXT], PLAYER_TEMP[playerid][pt_SHOP_ADD_MODELID], PLAYER_TEMP[playerid][pt_SHOP_ADD_VCOL1], PLAYER_TEMP[playerid][pt_SHOP_ADD_VCOL2]);
					SendNotification(playerid, "Tu anuncio se ha publicado correctamente.");
				}
			}
			return 1;
		}
		case DIALOG_CRANE_SELECT_VEHICLE:
		{
			if(response)
			{
				new const vehicleid = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];

				if(vehicleid == -1) return 1;

				new veh_money = (VEHICLE_INFO[GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400][vehicle_info_PRICE] / 100) + 350;
				if(veh_money > PI[playerid][ac_CASH])
				{
					SendFormatNotification(playerid, "Necesitas %s$ para remolcar este vehículo al depósito municipal.", veh_money);
					return 1;
				}
				
				if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_OCCUPIED])
				{
					SendNotification(playerid, "No podemos remolcar tu vehículo porque hay alguien conduciéndolo.");
					return 1; 
				}
				
				if(GivePlayerCash(playerid, -veh_money, true, true)) {
					new crane_point = random(sizeof(CRANE_POINTS[]));
					GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_X] = CRANE_POINTS[crane_point][0];
					GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Y] = CRANE_POINTS[crane_point][1];
					GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_Z] = CRANE_POINTS[crane_point][2];
					GLOBAL_VEHICLES[vehicleid][gb_vehicle_SPAWN_ANGLE] = CRANE_POINTS[crane_point][3];
					GLOBAL_VEHICLES[vehicleid][gb_vehicle_WORLD] = 0;

					GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_CLOSED_TIME] = gettime();
					GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
					GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_LIGHTS] = 0;
					GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS] = 1;
					UpdateVehicleParams(vehicleid);
					SetVehicleToRespawnEx(vehicleid);

					SendFormatNotification(playerid, "Tu vehículo %s ha sido remolcado hasta el depósito municipal.", VEHICLE_INFO[GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][gb_vehicle_MODELID] - 400][vehicle_info_NAME]);
				}
			}
			return 1;
		}
		case DIALOG_CREATE_CREW:
		{
			if(response)
			{
				if(ACCOUNT_INFO[playerid][ac_LEVEL] < 5) return SendNotification(playerid, "Necesitas ser nivel 5 para poder crear una banda.");
				ShowDialog(playerid, DIALOG_CREATE_CREW_NAME);
			}
			return 1;
		}
		case DIALOG_CREATE_CREW_NAME:
		{
			if(response)
			{
				if(strlen(inputtext) >= 32)
				{
					SendNotification(playerid, "Como máximo puedes introducir un nombre de 32 caracteres.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				new crew_name[32];
				if(sscanf(inputtext, "s[32]", crew_name))
				{
					SendNotification(playerid, "Introduce un nombre para tu banda.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				format(PLAYER_TEMP[playerid][pt_CREATE_CREW_NAME], 32, "%s", crew_name);
				ShowDialog(playerid, DIALOG_CREATE_CREW_COLOR);
			}
			return 1;
		}
		case DIALOG_CREATE_CREW_COLOR:
		{
			if(response)
			{
				PLAYER_TEMP[playerid][pt_CREATE_CREW_COLOR] = listitem;
				ShowDialog(playerid, DIALOG_CREATE_CREW_CONFIRM);
			}
			else ShowDialog(playerid, DIALOG_CREATE_CREW_NAME);
			return 1;
		}
		case DIALOG_CREATE_CREW_CONFIRM:
		{
			if(response)
			{
				new index = GetAvaibleCrewIndex();
				if(index == -1)
				{
					SendFormatNotification(playerid, "No se puede crear la banda porque no hay espacio disponible, el límite de bandas es %d.", MAX_CREWS);
					return 1;
				}
				
				if(600000 > PI[playerid][ac_CASH])
				{
					SendNotification(playerid, "Necesitas 600.000$ para crear la banda.");
					return 1;
				}
				
				if(GivePlayerCash(playerid, -600000, true, true)) {
					CREW_INFO[index][crew_VALID] = true;
					format(CREW_INFO[index][crew_NAME], 32, "%s", PLAYER_TEMP[playerid][pt_CREATE_CREW_NAME]);
					CREW_INFO[index][crew_COLOR] = RandomColors[ PLAYER_TEMP[playerid][pt_CREATE_CREW_COLOR] ];
		
					CREW_RANK_INFO[index][0][crew_rank_VALID] = true;
					format(CREW_RANK_INFO[index][0][crew_rank_NAME], 32, "Fundador");
					
					CREW_RANK_INFO[index][0][crew_rank_PERMISSION][CREW_RANK_CHANGE_NAME] =
					CREW_RANK_INFO[index][0][crew_rank_PERMISSION][CREW_RANK_INVITE] =
					CREW_RANK_INFO[index][0][crew_rank_PERMISSION][CREW_RANK_CAST_MEMBERS] =
					CREW_RANK_INFO[index][0][crew_rank_PERMISSION][CREW_RANK_MODIFY_MEMBERS] =
					CREW_RANK_INFO[index][0][crew_rank_PERMISSION][CREW_RANK_MODIFY_RANKS] =
					CREW_RANK_INFO[index][0][crew_rank_PERMISSION][CREW_RANK_ADD_PROPERTIES] =
					CREW_RANK_INFO[index][0][crew_rank_PERMISSION][CREW_RANK_DELETE_PROPERTIES] =
					CREW_RANK_INFO[index][0][crew_rank_PERMISSION][CREW_RANK_CHANGE_COLOR] =
					CREW_RANK_INFO[index][0][crew_rank_PERMISSION][CREW_RANK_LEAVE_TERRITORY] =
					CREW_RANK_INFO[index][0][crew_rank_PERMISSION][CREW_RANK_DELETE] =
					CREW_RANK_INFO[index][0][crew_rank_PERMISSION][CREW_RANK_GRAFFITI] = 1;
						
					NewCrewRegister(index, playerid);
					SetPlayerGangZones(playerid);				
					SendFormatNotification(playerid, "La banda ~g~'%s' ~w~se ha creado correctamente, usa ~g~/banda ~w~para administrarla.", CREW_INFO[index][crew_NAME]);
				}
			}
			return 1;
		}
		case DIALOG_CREW_MENU:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				
				switch(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem])
				{
					case CREW_RANK_SIZE: //Lista de miembros
					{
						PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] = 10;
						PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
						ShowDialog(playerid, DIALOG_CREW_MEMBER_LIST);
					}
					case CREW_RANK_CHANGE_NAME:
					{
						if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_CHANGE_NAME]) return SendNotification(playerid, "No tienes permiso.");
						if(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_FIGHTING]) return SendNotification(playerid, "No se puede cambiar el nombre de la banda cuando la banda está en combate.");
						ShowDialog(playerid, DIALOG_CREW_CHANGE_NAME);
					}
					case CREW_RANK_CAST_MEMBERS:
					{
						if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_CAST_MEMBERS]) return SendNotification(playerid, "No tienes permiso.");
						
						PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] = 10;
						PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
						ShowDialog(playerid, DIALOG_CREW_MEMBER_LIST_DELETE);
					}
					
					case CREW_RANK_MODIFY_RANKS:
					{
						if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_RANKS]) return SendNotification(playerid, "No tienes permiso.");
						
						ShowDialog(playerid, DIALOG_CREW_RANKS);
					}
					case CREW_RANK_CHANGE_COLOR:
					{
						if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_CHANGE_COLOR]) return SendNotification(playerid, "No tienes permiso.");
						
						if(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_FIGHTING]) return SendNotification(playerid, "No se puede cambiar el color cuando la banda está en combate.");
						ShowDialog(playerid, DIALOG_CREW_MODIFY_COLOR);
					}
					case CREW_RANK_DELETE:
					{
						if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_DELETE]) return SendNotification(playerid, "No tienes permiso.");
						
						ShowDialog(playerid, DIALOG_CREW_DELETE);
					}
					case CREW_RANK_MODIFY_MEMBERS:
					{
						if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_MEMBERS]) return SendNotification(playerid, "No tienes permiso.");
						
						PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] = 10;
						PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
						ShowDialog(playerid, DIALOG_CREW_MODIFY_MEMBERS);
					}
					case CREW_RANK_SIZE + 1: //Abandonar banda
					{
						if(PI[playerid][ac_CREW_RANK] == 0)
						{
							inline OnCrewInfoLoad()
							{
								new rows;
								if(cache_get_row_count(rows))
								{
									new total;
									cache_get_value_index_int(0, 0, total);
									if(total <= 1) SendNotification(playerid, "No puedes abandonar la banda porque eres el único miembro con el rango fundador.");
									else ShowDialog(playerid, DIALOG_CREW_LEAVE);
								}
							}
							mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d AND crew_rank = 0;", PI[playerid][ac_CREW]);
							MySQL_TQueryInline(srp_db, using inline OnCrewInfoLoad, QUERY_BUFFER);
						}
						else ShowDialog(playerid, DIALOG_CREW_LEAVE);
					}
				}
			}
			return 1;
		}
		case DIALOG_CREW_MEMBER_LIST:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -2) // Siguiente
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d", PI[playerid][ac_CREW]);
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
				}
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -3) //Anterior
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d", PI[playerid][ac_CREW]);
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
				}
			}
			else ShowDialog(playerid, DIALOG_CREW_MENU);
			return 1;
		}
		case DIALOG_CREW_CHANGE_NAME:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_CHANGE_NAME]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_FIGHTING]) return SendNotification(playerid, "No se puede cambiar el nombre de la banda cuando la banda está en combate.");
				if(strlen(inputtext) >= 32)
				{
					SendNotification(playerid, "Como máximo puedes introducir un nombre de 32 caracteres.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				new crew_name[32];
				if(sscanf(inputtext, "s[32]", crew_name))
				{
					SendNotification(playerid, "Introduce un nombre para tu banda.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				format(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_NAME], 32, "%s", crew_name);
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE crews SET name = '%e' WHERE id = %d;", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_NAME], CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				//actualizar nombre casas pickups
				
				
				new message[145], label_str[256];
				format(message, sizeof message, "{%06x}[Banda] {FFFFFF}%s (%s) cambió el nombre de la banda a '%s'.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, PLAYER_TEMP[playerid][pt_RP_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_NAME], CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_NAME]);
				SendMessageToCrewMembers(PI[playerid][ac_CREW], -1, message);
				
				
				for(new i = 0; i != MAX_PROPERTIES; i ++)
				{
					if(!PROPERTY_INFO[i][property_VALID]) continue;
					if(!PROPERTY_INFO[i][property_CREW]) continue;
					if(PROPERTY_INFO[i][property_CREW_ID] != PI[playerid][ac_CREW]) continue;
		
					format
					(
						label_str, 
						sizeof label_str, 
						"\
							Propiedad {"#PRIMARY_COLOR"}#%d\n\n\
							{FFFFFF}Banda: {"#PRIMARY_COLOR"}%s\n\
							{FFFFFF}Presiona {"#PRIMARY_COLOR"}~k~~CONVERSATION_YES~ {FFFFFF}para entrar.\
						", PROPERTY_INFO[i][property_ID], CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_NAME]
					);
					
					UpdateDynamic3DTextLabelText(PROPERTY_INFO[i][property_EXT_LABEL_ID], 0xFFFFFFFF, label_str);
				}
				
				ShowDialog(playerid, DIALOG_CREW_MENU);
			}
			else ShowDialog(playerid, DIALOG_CREW_MENU);
			return 1;
		}
		case DIALOG_CREW_MEMBER_LIST_DELETE:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_CAST_MEMBERS]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -2) // Siguiente
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d", PI[playerid][ac_CREW]);
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
				}
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -3) //Anterior
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d", PI[playerid][ac_CREW]);
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
				}
				else
				{
					PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
					
					inline OnCrewInfoLoad()
					{
						new rows;
						if(cache_get_row_count(rows))
						{
							if(rows)
							{
								new name[24], crew_rank;
								cache_get_value_index(0, 0, name);
								cache_get_value_index_int(0, 1, crew_rank);

								if(crew_rank < PI[playerid][ac_CREW_RANK]) SendNotification(playerid, "No puedes echar a este miembro porque es un rango superior al tuyo.");
								else
								{
									if(crew_rank == 0)
									{
										inline OnCountQueryLoad()
										{
											if(cache_get_row_count(rows))
											{
												if(rows)
												{
													new total;
													cache_get_value_index_int(0, 0, total);
													if(total <= 1) SendNotification(playerid, "No se puede echar a este miembro ya que es el único miembro con el rango fundador.");
													else ShowDialog(playerid, DIALOG_CREW_CAST_MEMBER_CONFIRM);
												}
											}
										}
										mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d AND crew_rank = 0;", PI[playerid][ac_CREW]);
										MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
									}
									else ShowDialog(playerid, DIALOG_CREW_CAST_MEMBER_CONFIRM);
								}
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT name, crew_rank FROM player WHERE id = %d;", PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID]);
					MySQL_TQueryInline(srp_db, using inline OnCrewInfoLoad, QUERY_BUFFER);
				}
			}
			else ShowDialog(playerid, DIALOG_CREW_MENU);
			return 1;
		}
		case DIALOG_CREW_CAST_MEMBER_CONFIRM:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_CAST_MEMBERS]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				inline OnCrewInfoLoad()
				{
					new rows;
					if(cache_get_row_count(rows))
					{
						new name[24], connected, pid, crew_rank;
						cache_get_value_name(0, "name", name);
						cache_get_value_name_int(0, "connected", connected);
						cache_get_value_name_int(0, "playerid", pid);
						cache_get_value_name_int(0, "crew_rank", crew_rank);
						if(crew_rank < PI[playerid][ac_CREW_RANK]) SendNotification(playerid, "No puedes echar a este miembro porque es un rango superior al tuyo.");
						else
						{
							if(crew_rank == 0)
							{
								inline OnCountQueryLoad()
								{
									if(cache_get_row_count(rows))
									{
										if(rows)
										{
											new total;
											cache_get_value_index_int(0, 0, total);
											if(total <= 1) SendNotification(playerid, "No se puede echar a este miembro ya que es el único miembro con el rango fundador.");
											else
											{
												new message[145];
												format(message, sizeof message, "{%06x}[Banda] {FFFFFF}%s (%s) ha echado a %s de la banda.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, PLAYER_TEMP[playerid][pt_RP_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_NAME], name);
												SendMessageToCrewMembers(PI[playerid][ac_CREW], -1, message);
											
												mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET crew = NULL, crew_rank = 0 WHERE id = %d;", PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID]);
												mysql_tquery(srp_db, QUERY_BUFFER);
												
												CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_MEMBERS] --;
												if(connected)
												{
													CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_ONLINE_MEMBERS] --;
													SetPlayerNormalColor(pid);
													if(PLAYER_TEMP[playerid][pt_LAST_TERRITORY] != INVALID_STREAMER_ID)
													{
														TextDrawHideForPlayer(pid, Textdraws[textdraw_TERRITORY_BOX]);
														TextDrawHideForPlayer(pid, TERRITORIES[ PLAYER_TEMP[playerid][pt_LAST_TERRITORY] ][territory_TEXTDRAW]);
													}

													PI[pid][ac_CREW] = 0;
													PI[pid][ac_CREW_RANK] = 0;
													PLAYER_TEMP[pid][pt_CREW_INDEX] = 0;
													HidePlayerGangZones(pid);
												}
											}
										}
									}
								}
								mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d AND crew_rank = 0;", PI[playerid][ac_CREW]);
								MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
							}
							else
							{
								new message[145];
								format(message, sizeof message, "{%06x}[Banda] {FFFFFF}%s (%s) ha echado a %s de la banda.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, PLAYER_TEMP[playerid][pt_RP_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_NAME], name);
								SendMessageToCrewMembers(PI[playerid][ac_CREW], -1, message);
							
								mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET crew = NULL, crew_rank = 0 WHERE id = %d;", PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID]);
								mysql_tquery(srp_db, QUERY_BUFFER);
								
								CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_MEMBERS] --;
								if(connected)
								{
									CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_ONLINE_MEMBERS] --;
									SetPlayerNormalColor(pid);
									if(PLAYER_TEMP[playerid][pt_LAST_TERRITORY] != INVALID_STREAMER_ID)
									{
										TextDrawHideForPlayer(pid, Textdraws[textdraw_TERRITORY_BOX]);
										TextDrawHideForPlayer(pid, TERRITORIES[ PLAYER_TEMP[playerid][pt_LAST_TERRITORY] ][territory_TEXTDRAW]);
									}

									PI[pid][ac_CREW] = 0;
									PI[pid][ac_CREW_RANK] = 0;
									PLAYER_TEMP[pid][pt_CREW_INDEX] = 0;
									HidePlayerGangZones(pid);
								}
							}
						}
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT name, connected, playerid, crew_rank FROM player WHERE id = %d;", PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID]);
				MySQL_TQueryInline(srp_db, using inline OnCrewInfoLoad, QUERY_BUFFER);
			}
			else ShowDialog(playerid, DIALOG_CREW_MEMBER_LIST_DELETE);
			return 1;
		}
		case DIALOG_CREW_INVITE_RANK:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_INVITE]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] < PI[playerid][ac_CREW_RANK])
				{
					SendNotification(playerid, "No puedes invitar con este rango porque es superior al tuyo.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_CREW_INVITE_PID])) return SendNotification(playerid, "El jugador está desconectado.");
				
				new Float:pos[3]; GetPlayerPos(PLAYER_TEMP[playerid][pt_CREW_INVITE_PID], pos[0], pos[1], pos[2]);
				if(!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "Este jugador no está cerca tuya.");
				if(PLAYER_WORKS[ PLAYER_TEMP[playerid][pt_CREW_INVITE_PID] ][WORK_POLICE][pwork_SET]) return SendNotification(playerid, "Esta persona es policía y no puede tener banda.");
				if(PI[ PLAYER_TEMP[playerid][pt_CREW_INVITE_PID] ][ac_CREW]) return SendNotification(playerid, "Esta persona pertenece a otra banda.");
				if(PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_CREW_INVITE_PID] ][pt_GAME_STATE] != GAME_STATE_NORMAL) return SendNotification(playerid, "No puedes invitar a este jugador ahora.");
				if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_CREW_INVITE_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_CREW_INVITE_AID]) return SendNotification(playerid, "El jugador está desconectado.");
				
				SendFormatNotification(playerid, "Has ofrecido a ~g~%s~w~ unirse a la banda con el rango ~g~%s~w~.", PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_CREW_INVITE_PID] ][pt_RP_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][crew_rank_NAME]);
				
				PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_CREW_INVITE_PID] ][pt_CREW_INVITE_INFO][0] = PI[playerid][ac_CREW];
				PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_CREW_INVITE_PID] ][pt_CREW_INVITE_INFO][1] = PLAYER_TEMP[playerid][pt_CREW_INDEX];
				PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_CREW_INVITE_PID] ][pt_CREW_INVITE_INFO][2] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
				PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_CREW_INVITE_PID] ][pt_CREW_INVITE_INFO][3] = gettime();
				ShowDialog(PLAYER_TEMP[playerid][pt_CREW_INVITE_PID], DIALOG_CREW_INVITE_ACCEPT);
			}
			return 1;
		}
		case DIALOG_CREW_INVITE_ACCEPT:
		{
			if(response)
			{
				if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INVITE_INFO][1] ][crew_VALID]) return SendNotification(playerid, "Esta banda ya no existe.");
				if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INVITE_INFO][1] ][ PLAYER_TEMP[playerid][pt_CREW_INVITE_INFO][2] ][crew_rank_VALID]) return SendNotification(playerid, "El rango que te han ofrecio ya no existe.");
				if(gettime() > PLAYER_TEMP[playerid][pt_CREW_INVITE_INFO][3] + 20) return SendNotification(playerid, "Has tardado mucho en aceptar.");
				if(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INVITE_INFO][1] ][crew_FIGHTING]) return SendNotification(playerid, "No puedes unirte a la banda cuando la banda está en combate.");
				
				PI[playerid][ac_CREW] = PLAYER_TEMP[playerid][pt_CREW_INVITE_INFO][0];
				PI[playerid][ac_CREW_RANK] = PLAYER_TEMP[playerid][pt_CREW_INVITE_INFO][2];
				PLAYER_TEMP[playerid][pt_CREW_INDEX] = PLAYER_TEMP[playerid][pt_CREW_INVITE_INFO][1];
				CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_MEMBERS] ++;
				CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_ONLINE_MEMBERS] ++;
				PLAYER_TEMP[playerid][pt_LAST_GOT_CREW] = gettime();
				SetPlayerGangZones(playerid);
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET crew = %d, crew_rank = %d WHERE id = %d;", PI[playerid][ac_CREW], PI[playerid][ac_CREW_RANK], ACCOUNT_INFO[playerid][ac_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				new message[145];
				format(message, sizeof message, "{%06x}[Banda] {FFFFFF}%s se ha unido a la banda con el rango %s.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, PLAYER_TEMP[playerid][pt_RP_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_NAME]);
				SendMessageToCrewMembers(PI[playerid][ac_CREW], -1, message);
			}
			return 1;
		}
		case DIALOG_CREW_RANKS:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_RANKS]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(listitem < PI[playerid][ac_CREW_RANK])
				{
					if(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][listitem][crew_rank_VALID]) SendNotification(playerid, "No puedes modificar este rango porque es un rango superior al tuyo.");
					else SendNotification(playerid, "No puedes crear un rango en este puesto porque es superior al tuyo.");
					return 1;
				}
				
				PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] = listitem;
				if(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][listitem][crew_rank_VALID]) ShowDialog(playerid, DIALOG_CREW_RANK_MODIFY);
				else ShowDialog(playerid, DIALOG_CREW_RANK_CREATE);
			}
			else ShowDialog(playerid, DIALOG_CREW_MENU);
			return 1;
		}
		case DIALOG_CREW_RANK_MODIFY:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_RANKS]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				switch(listitem)
				{
					case 0: ShowDialog(playerid, DIALOG_CREW_RANK_MODIFY_NAME);
					case 1:
					{
						if(PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] == 0)
						{
							SendNotification(playerid, "No se pueden modificar los permisos de este rango porque es el rango fundador.");
							return 1;
						}
						
						ShowDialog(playerid, DIALOG_CREW_RANK_MODIFY_PERMISS);
					}
					case 2:
					{
						if(PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] == 0)
						{
							SendNotification(playerid, "No se puede eliminar este rango porque es el rango fundador.");
							return 1;
						}
						
						SendFormatNotification(playerid, "~r~[Atención] ~w~Selecciona el nuevo rango para los miembros que actualmente tienen el rango ~r~'%s'.", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_NAME]);
						ShowDialog(playerid, DIALOG_CREW_RANK_DELETE);
					}
				}
			}
			else ShowDialog(playerid, DIALOG_CREW_RANKS);
			return 1;
		}
		case DIALOG_CREW_RANK_MODIFY_NAME:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_RANKS]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(strlen(inputtext) >= 32)
				{
					SendNotification(playerid, "Como máximo puedes introducir un nombre de 32 caracteres.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				new rank_name[32];
				if(sscanf(inputtext, "s[32]", rank_name))
				{
					SendNotification(playerid, "Introduce el nuevo nombre para el rango.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				SendFormatNotification(playerid, "Has modificado el nombre del rango '%s' a '%s'.", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_NAME], rank_name);
				format(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_NAME], 32, "%s", rank_name);
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE crew_ranks SET rank_name = '%e' WHERE id = %d;",  CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
			}
			else ShowDialog(playerid, DIALOG_CREW_RANK_MODIFY);
			return 1;
		}
		case DIALOG_CREW_RANK_CREATE:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_RANKS]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(strlen(inputtext) >= 32)
				{
					SendNotification(playerid, "Como máximo puedes introducir un nombre de 32 caracteres.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				new rank_name[32];
				if(sscanf(inputtext, "s[32]", rank_name))
				{
					SendNotification(playerid, "Introduce un nombre para el rango.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				if(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_VALID])
				{
					SendNotification(playerid, "Parece que alguien ya ha creado un rango en este espacio.");
					return 1;
				}
				
				
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_VALID] = true;
				format(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_NAME], 32, "%s", rank_name);
				
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][CREW_RANK_CHANGE_NAME] =
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][CREW_RANK_INVITE] =
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][CREW_RANK_CAST_MEMBERS] =
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_MEMBERS] =
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_RANKS] =
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][CREW_RANK_ADD_PROPERTIES] =
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][CREW_RANK_DELETE_PROPERTIES] =
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][CREW_RANK_CHANGE_COLOR] =
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][CREW_RANK_LEAVE_TERRITORY] =
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][CREW_RANK_DELETE] = 0;
				RegisterNewCrewRank(PLAYER_TEMP[playerid][pt_CREW_INDEX], PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK]);
								
				SendFormatNotification(playerid, "El rango '%s' ha sido creado, ahora puedes editarlo y darle permisos.", rank_name);
				ShowDialog(playerid, DIALOG_CREW_RANKS);
			}
			else ShowDialog(playerid, DIALOG_CREW_RANKS);
			return 1;
		}
		case DIALOG_CREW_RANK_MODIFY_PERMISS:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_RANKS]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][listitem])
				{
					SendNotification(playerid, "No puedes cambiar este permiso porque tu no tienes este permiso.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				if(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][listitem]) CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][listitem] = false;
				else CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][listitem] = true;
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE crew_ranks SET permission%d = %d WHERE id = %d;", listitem, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][listitem], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				ShowDialog(playerid, dialogid);
			}
			else ShowDialog(playerid, DIALOG_CREW_RANK_MODIFY);
			return 1;
		}
		case DIALOG_CREW_RANK_DELETE:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_RANKS]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] < PI[playerid][ac_CREW_RANK])
				{
					SendNotification(playerid, "No puedes seleccionar este rango porque es superior al tuyo.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				PLAYER_TEMP[playerid][pt_CREW_SELECTED_NEW_RANK] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
				ShowDialog(playerid, DIALOG_CREW_RANK_DELETE_CONFIRM);
			}
			return 1;
		}
		case DIALOG_CREW_RANK_DELETE_CONFIRM:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_RANKS]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET crew_rank = %d WHERE crew = %d AND crew_rank = %d;", PLAYER_TEMP[playerid][pt_CREW_SELECTED_NEW_RANK], PI[playerid][ac_CREW], PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK]);
				mysql_tquery(srp_db, QUERY_BUFFER);

				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM crew_ranks WHERE id = %d;", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
			
				SendFormatNotification(playerid, "Has eliminado el rango '%s'.", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_NAME]);
			
				new message[145];
				format(message, sizeof message, "{%06x}[Banda] {FFFFFF}%s (%s) cambió tu rango al rango '%s'.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, PLAYER_TEMP[playerid][pt_RP_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_NEW_RANK] ][crew_rank_NAME]);
				for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
				{
					if(IsPlayerConnected(i))
					{
						if(PI[i][ac_CREW])
						{
							if(PI[i][ac_CREW] == PI[playerid][ac_CREW])
							{
								if(PI[i][ac_CREW_RANK] == PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK])
								{
									PI[i][ac_CREW_RANK] = PLAYER_TEMP[playerid][pt_CREW_SELECTED_NEW_RANK];
									SendClientMessage(i, -1, message);
								}
							}
						}
					}
				}
				
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_VALID] = false;
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_ID] = 0;
				CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_NAME][0] = EOS;
				for(new i = 0; i != CREW_RANK_SIZE; i ++) CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][i] = 0;
			
			}
			else ShowDialog(playerid, DIALOG_CREW_RANKS);
			return 1;
		}
		case DIALOG_CREW_MODIFY_COLOR:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_CHANGE_COLOR]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_FIGHTING]) return SendNotification(playerid, "No se puede cambiar el color cuando la banda está en combate.");
				
				CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] = RandomColors[listitem];
				
				
				new r, g, b, a,		new_territory_color;
				HexToRGBA(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR], r, g, b, a);
				new_territory_color = RGBAToHex(r, g, b, 135);
				for(new i = 0; i != MAX_TERRITORIES; i ++)
				{
					if(!TERRITORIES[i][territory_VALID]) continue;
					if(!TERRITORIES[i][territory_OCCUPIED]) continue;
					
					if(TERRITORIES[i][territory_CREW_ID] == PI[playerid][ac_CREW])
					{
						TERRITORIES[i][territory_COLOR] = new_territory_color;
						UpdateGangZoneColor(i);
					}
				}
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE crews SET color = %d WHERE id = %d;", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR], CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				SendNotification(playerid, "El color de la banda se ha cambiado.");
				ShowDialog(playerid, DIALOG_CREW_MENU);
			}
			else ShowDialog(playerid, DIALOG_CREW_MENU);
			return 1;
		}
		case DIALOG_CREW_DELETE:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_DELETE]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_FIGHTING]) return SendNotification(playerid, "No se puede borrar la banda cuando está en combate.");
				
				new message[145];
				format(message, sizeof message, "{%06x}[Banda] {FFFFFF}%s (%s) ha eliminado la banda.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, PLAYER_TEMP[playerid][pt_RP_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_NAME]);

				new tmp_crew_id = PI[playerid][ac_CREW];
				new tmp_CREW_INFO[enum_CREW_INFO], old_crew_id = PI[playerid][ac_CREW];
				CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ] = tmp_CREW_INFO;
				
				new tmp_CREW_RANK_INFO[enum_CREW_RANK_INFO];
				for(new i = 0; i != MAX_CREW_RANKS; i ++) CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i] = tmp_CREW_RANK_INFO;
				
				for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
				{
					if(IsPlayerConnected(i))
					{
						if(PI[i][ac_CREW])
						{
							if(PI[i][ac_CREW] == PI[playerid][ac_CREW])
							{
								SetPlayerNormalColor(i);
								HidePlayerGangZones(i);
								if(PLAYER_TEMP[i][pt_LAST_TERRITORY] != INVALID_STREAMER_ID)
								{
									TextDrawHideForPlayer(i, Textdraws[textdraw_TERRITORY_BOX]);
									TextDrawHideForPlayer(i, TERRITORIES[ PLAYER_TEMP[i][pt_LAST_TERRITORY] ][territory_TEXTDRAW]);
								}
								PI[i][ac_CREW] = 0;
								PI[i][ac_CREW_RANK] = 0;
								PLAYER_TEMP[i][pt_CREW_INDEX] = 0;
								SendClientMessage(i, -1, message);
							}
						}
					}
				}
				
				for(new i = 0; i != MAX_TERRITORIES; i ++)
				{
					if(!TERRITORIES[ i ][territory_VALID]) continue;
					if(!TERRITORIES[ i ][territory_OCCUPIED]) continue;
					if(TERRITORIES[ i ][territory_CREW_ID] != old_crew_id) continue;
					
					TERRITORIES[ i ][territory_OCCUPIED] = false;
					TERRITORIES[ i ][territory_CREW_ID] = 0;
					TERRITORIES[ i ][territory_CREW_INDEX] = 0;
					TERRITORIES[ i ][territory_COLOR] = 0xCCCCCC55;
					UpdateGangZoneColor(i);
				}
				
				for(new i = 0; i != MAX_PROPERTIES; i ++)
				{
					if(!PROPERTY_INFO[i][property_VALID]) continue;
					if(!PROPERTY_INFO[i][property_CREW]) continue;
					if(PROPERTY_INFO[i][property_CREW_ID] != old_crew_id) continue;
					
					PROPERTY_INFO[ i ][property_SOLD] = false;
					PROPERTY_INFO[ i ][property_CREW] = false;
					PROPERTY_INFO[ i ][property_OWNER_ID] = 0;
					PROPERTY_INFO[ i ][property_CREW_ID] = 0;
					format(PROPERTY_INFO[ i ][property_NAME], 24, "PROPIEDAD %d", PROPERTY_INFO[ i ][property_ID]);
					
					new info[3];
					UpdateUnnocupiedPropertyLabel(i);
					
					DestroyDynamicPickup(PROPERTY_INFO[ i ][property_EXT_PICKUP_ID]);
					PROPERTY_INFO[ i ][property_EXT_PICKUP_ID] = INVALID_STREAMER_ID;
					
					PROPERTY_INFO[ i ][property_EXT_PICKUP_ID] = CreateDynamicPickup(1273, 1, PROPERTY_INFO[ i ][property_EXT_X], PROPERTY_INFO[ i ][property_EXT_Y], PROPERTY_INFO[ i ][property_EXT_Z], 0, PROPERTY_INFO[ i ][property_EXT_INTERIOR]);
					info[0] = PICKUP_TYPE_PROPERTY;
					info[1] = i; // Index
					info[2] = 2; // Pickup Exterior
					Streamer_SetArrayData(STREAMER_TYPE_PICKUP, PROPERTY_INFO[ i ][property_EXT_PICKUP_ID], E_STREAMER_EXTRA_ID, info);

					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE properties SET id_player = NULL, id_territory = NULL WHERE id = %d;", PROPERTY_INFO[i][property_ID]);
					mysql_tquery(srp_db, QUERY_BUFFER);
				}

				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM crews WHERE id = %d;", tmp_crew_id);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				CallLocalFunction("OnCrewDeleted", "i", tmp_crew_id);
			}
			else ShowDialog(playerid, DIALOG_CREW_MENU);
			return 1;
		}
		case DIALOG_CREW_LEAVE:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			
			if(response)
			{
				if(PI[playerid][ac_CREW_RANK] == 0)
				{
					inline OnCountQueryLoad()
					{
						new rows;
						if(cache_get_row_count(rows))
						{
							if(rows)
							{
								new total;
								cache_get_value_index_int(0, 0, total);
								if(total <= 1) SendNotification(playerid, "No puedes abandonar la banda porque eres el único miembro con el rango fundador.");
								else
								{
									new message[145];
									format(message, sizeof message, "{%06x}[Banda] {FFFFFF}%s (%s) ha abandonado la banda.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, PLAYER_TEMP[playerid][pt_RP_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_NAME]);
									SendMessageToCrewMembers(PI[playerid][ac_CREW], -1, message);
									
									mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET crew = NULL, crew_rank = 0 WHERE id = %d;", ACCOUNT_INFO[playerid][ac_ID]);
									mysql_tquery(srp_db, QUERY_BUFFER);
									
									SetPlayerNormalColor(playerid);
									CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_MEMBERS] --;
									CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_ONLINE_MEMBERS] --;
									HidePlayerGangZones(playerid);
									
									PI[playerid][ac_CREW] = 0;
									PI[playerid][ac_CREW_RANK] = 0;
									PLAYER_TEMP[playerid][pt_CREW_INDEX] = 0;
								}
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d AND crew_rank = 0;", PI[playerid][ac_CREW]);
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
				}
				else
				{
					new message[145];
					format(message, sizeof message, "{%06x}[Banda] {FFFFFF}%s (%s) ha abandonado la banda.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, PLAYER_TEMP[playerid][pt_RP_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_NAME]);
					SendMessageToCrewMembers(PI[playerid][ac_CREW], -1, message);
					
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET crew = NULL, crew_rank = 0 WHERE id = %d;", ACCOUNT_INFO[playerid][ac_ID]);
					mysql_tquery(srp_db, QUERY_BUFFER);
					
					SetPlayerNormalColor(playerid);
					CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_MEMBERS] --;
					CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_ONLINE_MEMBERS] --;
					HidePlayerGangZones(playerid);
					
					PI[playerid][ac_CREW] = 0;
					PI[playerid][ac_CREW_RANK] = 0;
					PLAYER_TEMP[playerid][pt_CREW_INDEX] = 0;
				}
			}
			else ShowDialog(playerid, DIALOG_CREW_MENU);
			return 1;
		}
		case DIALOG_CREW_MODIFY_MEMBERS:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_MEMBERS]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -2) // Siguiente
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d", PI[playerid][ac_CREW]);
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
				}
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -3) //Anterior
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d", PI[playerid][ac_CREW]);
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
				}
				else
				{
					PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
					ShowDialog(playerid, DIALOG_CREW_MODIFY_MEMBER);
				}
			}
			else ShowDialog(playerid, DIALOG_CREW_MENU);
			return 1;
		}
		case DIALOG_CREW_MODIFY_MEMBER:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_MEMBERS]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] < PI[playerid][ac_CREW_RANK])
				{
					SendNotification(playerid, "No puedes dar este rango porque es un rango superior al tuyo.");
					return 1;
				}
				
				inline OnCrewInfoLoad()
				{
					new rows;
					if(cache_get_row_count(rows))
					{
						if(rows)
						{
							new name[24], connected, pid, crew_rank;
							cache_get_value_name(0, "name", name);
							cache_get_value_name_int(0, "connected", connected);
							cache_get_value_name_int(0, "playerid", pid);
							cache_get_value_name_int(0, "crew_rank", crew_rank);

							if(crew_rank < PI[playerid][ac_CREW_RANK]) SendNotification(playerid, "No puedes modificar el rango de este miembro porque es un rango superior al tuyo.");
							else
							{
								if(crew_rank == 0)
								{
									inline OnCountQueryLoad()
									{
										if(cache_get_row_count(rows))
										{
											if(rows)
											{
												new total;
												cache_get_value_index_int(0, 0, total);
												if(total <= 1) SendNotification(playerid, "No se puede modificar el rango de este miembro ya que es el único miembro con el rango fundador.");
												else
												{
													mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET crew_rank = %d WHERE id = %d;", PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem], PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID]);
													mysql_tquery(srp_db, QUERY_BUFFER);

													new message[145];
													if(connected)
													{
														PI[pid][ac_CREW_RANK] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
														format(message, sizeof message, "{%06x}[Banda] {FFFFFF}%s (%s) cambió tu rango al rango '%s'.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, PLAYER_TEMP[playerid][pt_RP_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][crew_rank_NAME]);
														SendClientMessage(pid, -1, message);
													}
													format(message, sizeof message, "Has modificado el rango de '%s' al rango '%s'.", name, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][crew_rank_NAME]);
													SendClientMessage(playerid, -1, message);
												}
											}
										}
									}
									mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d AND crew_rank = 0;", PI[playerid][ac_CREW]);
									MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
								}
								else
								{
									mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET crew_rank = %d WHERE id = %d;", PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem], PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID]);
									mysql_tquery(srp_db, QUERY_BUFFER);

									new message[145];
									if(connected)
									{
										PI[pid][ac_CREW_RANK] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
										format(message, sizeof message, "{%06x}[Banda] {FFFFFF}%s (%s) cambió tu rango al rango '%s'.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, PLAYER_TEMP[playerid][pt_RP_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][crew_rank_NAME]);
										SendClientMessage(pid, -1, message);
									}
									format(message, sizeof message, "Has modificado el rango de '%s' al rango '%s'.", name, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][crew_rank_NAME]);
									SendClientMessage(playerid, -1, message);
								}
							}							
						}
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT name, connected, playerid, crew_rank FROM player WHERE id = %d;", PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID]);
				MySQL_TQueryInline(srp_db, using inline OnCrewInfoLoad, QUERY_BUFFER);
			}
			else ShowDialog(playerid, DIALOG_CREW_MODIFY_MEMBERS);
			return 1;
		}
		case DIALOG_CREW_PROPERTY_CONFIRM:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_ADD_PROPERTIES]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(!TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_VALID]) return SendNotification(playerid, "La propiedad no está dentro de un territorio de tu banda.");
				if(!TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_OCCUPIED]) return SendNotification(playerid, "La propiedad no está dentro de un territorio de tu banda.");
				if(TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_CREW_ID] != PI[playerid][ac_CREW]) return SendNotification(playerid, "La propiedad no está dentro de un territorio de tu banda.");
				if(!IsPointInDynamicArea(TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_AREA], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_X], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_Y], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_Z])) return SendNotification(playerid, "La propiedad no está dentro de un territorio de tu banda.");
				if(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) return SendNotification(playerid, "Esta no es tu propiedad.");
				
				
				new index = PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED];
				PROPERTY_INFO[ index ][property_CREW] = true;
				PROPERTY_INFO[ index ][property_OWNER_ID] = 0;
				PROPERTY_INFO[ index ][property_CREW_ID] = PI[playerid][ac_CREW];
				format(PROPERTY_INFO[ index ][property_NAME], 24, "PROPIEDAD %d", PROPERTY_INFO[ index ][property_ID]);
				
				new label_str[256];
				format
				(
					label_str, 
					sizeof label_str, 
					"\
						Propiedad {"#PRIMARY_COLOR"}#%d\n\n\
						{FFFFFF}Banda: {"#PRIMARY_COLOR"}%s\n\
						{FFFFFF}Presiona {"#PRIMARY_COLOR"}~k~~CONVERSATION_YES~ {FFFFFF}para entrar.\
					", PROPERTY_INFO[index][property_ID], CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_NAME]
				);
				UpdateDynamic3DTextLabelText(PROPERTY_INFO[ index ][property_EXT_LABEL_ID], 0xFFFFFFFF, label_str);
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE properties SET id_player = NULL, id_territory = %d WHERE id = %d;", TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_ID], PROPERTY_INFO[index][property_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				new message[145];
				format(message, sizeof message, "{%06x}[Banda] {FFFFFF}%s (%s) ha añadido una nueva propiedad en %s.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, PLAYER_TEMP[playerid][pt_RP_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_NAME], TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_NAME]);
				SendMessageToCrewMembers(PI[playerid][ac_CREW], -1, message);
			}
			return 1;
		}
		case DIALOG_CREW_LEAVE_TERRITORY:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_LEAVE_TERRITORY]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(!IsPlayerInDynamicArea(playerid, TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_AREA])) return SendNotification(playerid, "No estás en un territorio de tu banda.");
				if(TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_CREW_ID] != PI[playerid][ac_CREW]) return SendNotification(playerid, "No estás en un territorio de tu banda.");			
				if(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_FIGHTING]) return SendNotification(playerid, "No puedes abandonar territorios mientras tu banda está en combate.");
				if(TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_WAR]) return SendNotification(playerid, "No se puede abandonar este territorio cuando está siendo conquistado.");

				TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_OCCUPIED] = false;
				TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_CREW_ID] = 0;
				TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_CREW_INDEX] = 0;
				TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_COLOR] = 0xCCCCCC55;
				UpdateGangZoneColor(PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO]);
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE territories SET id_crew = NULL WHERE id = %d;", TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				CallLocalFunction("OnCrewLeftTerritory", "iiii", PI[playerid][ac_CREW], PLAYER_TEMP[playerid][pt_CREW_INDEX], TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_ID], PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO]);
				
				for(new i = 0; i != MAX_PROPERTIES; i ++)
				{
					if(!PROPERTY_INFO[i][property_VALID]) continue;
					if(!PROPERTY_INFO[i][property_CREW]) continue;
					if(PROPERTY_INFO[i][property_CREW_ID] != PI[playerid][ac_CREW]) continue;
					
					if(IsPointInDynamicArea(TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_AREA], PROPERTY_INFO[i][property_EXT_X], PROPERTY_INFO[i][property_EXT_Y], PROPERTY_INFO[i][property_EXT_Z]))
					{
						PROPERTY_INFO[ i ][property_SOLD] = false;
						PROPERTY_INFO[ i ][property_CREW] = false;
						PROPERTY_INFO[ i ][property_OWNER_ID] = 0;
						PROPERTY_INFO[ i ][property_CREW_ID] = 0;
						format(PROPERTY_INFO[ i ][property_NAME], 24, "PROPIEDAD %d", PROPERTY_INFO[ i ][property_ID]);
						
						new info[3];
						UpdateUnnocupiedPropertyLabel(i);
						
						DestroyDynamicPickup(PROPERTY_INFO[ i ][property_EXT_PICKUP_ID]);
						PROPERTY_INFO[ i ][property_EXT_PICKUP_ID] = INVALID_STREAMER_ID;
						
						PROPERTY_INFO[ i ][property_EXT_PICKUP_ID] = CreateDynamicPickup(1273, 1, PROPERTY_INFO[ i ][property_EXT_X], PROPERTY_INFO[ i ][property_EXT_Y], PROPERTY_INFO[ i ][property_EXT_Z], 0, PROPERTY_INFO[ i ][property_EXT_INTERIOR]);
						info[0] = PICKUP_TYPE_PROPERTY;
						info[1] = i; // Index
						info[2] = 2; // Pickup Exterior
						Streamer_SetArrayData(STREAMER_TYPE_PICKUP, PROPERTY_INFO[ i ][property_EXT_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
							
						mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE properties SET id_player = NULL, id_territory = NULL WHERE id = %d;", PROPERTY_INFO[ i ][property_ID]);
						mysql_tquery(srp_db, QUERY_BUFFER);
					}
				}
				
				new message[145];
				format(message, sizeof message, "{"#PRIMARY_COLOR"}[BANDAS] {FFFFFF}La banda '%s' ha abandonado un territorio en %s.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_NAME], TERRITORIES[ PLAYER_TEMP[playerid][pt_PLAYER_TERRITORY_PRO] ][territory_NAME]);
				for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
				{
					if(IsPlayerConnected(i))
					{
						if(PI[i][ac_CREW])
						{
							SendClientMessage(i, -1, message);
						}
					}
				}
			}
			return 1;
		}
		case DIALOG_CREW_LEAVE_PROPERTY:
		{
			if(!PI[playerid][ac_CREW]) return SendNotification(playerid, "No perteneces a ninguna banda.");
			if(!CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_VALID]) return SendNotification(playerid, "La banda ya no existe.");
			if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_DELETE_PROPERTIES]) return SendNotification(playerid, "No tienes permiso.");
			
			if(response)
			{
				if(!PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_CREW]) return SendNotification(playerid, "Esta no es una propiedad de tu banda.");
				if(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_CREW_ID] != PI[playerid][ac_CREW]) return SendNotification(playerid, "Esta no es una propiedad de tu banda.");
				if(CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_FIGHTING]) return SendNotification(playerid, "No puedes liberar una propiedad cuando tu banda está en combate.");
			
				PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_SOLD] = false;
				PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_CREW] = false;
				PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_OWNER_ID] = 0;
				PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_CREW_ID] = 0;
				format(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_NAME], 24, "PROPIEDAD %d", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_ID]);
				
				new info[3];
				UpdateUnnocupiedPropertyLabel(PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED]);
				
				DestroyDynamicPickup(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_PICKUP_ID]);
				PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_PICKUP_ID] = INVALID_STREAMER_ID;
				
				PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_PICKUP_ID] = CreateDynamicPickup(1273, 1, PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_X], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_Y], PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_Z], 0, PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_INTERIOR]);
				info[0] = PICKUP_TYPE_PROPERTY;
				info[1] = PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED]; // Index
				info[2] = 2; // Pickup Exterior
				Streamer_SetArrayData(STREAMER_TYPE_PICKUP, PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_EXT_PICKUP_ID], E_STREAMER_EXTRA_ID, info);
						
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE properties SET id_player = NULL, id_territory = NULL WHERE id = %d;", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				SendNotification(playerid, "La propiedad ha sido liberada.");
			}
			return 1;
		}
		case DIALOG_POLICE_PENALTY:
		{
			if(response)
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_POLICE_PEN_TIME] + 20) return SendNotification(playerid, "Has tardardo mucho en aceptarlo.");
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_POLICE_PEN_PID])) return SendNotification(playerid, "El policía ya no está en el servidor.");
				if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_POLICE_PEN_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_POLICE_PEN_AID]) return SendNotification(playerid, "El policía ya no está en el servidor.");

				new Float:pos[3]; GetPlayerPos(PLAYER_TEMP[playerid][pt_POLICE_PEN_PID], pos[0], pos[1], pos[2]);
				if(!IsPlayerInRangeOfPoint(playerid, 5.0, pos[0], pos[1], pos[2])) return SendNotification(playerid, "El policía no está cerca tuya.");
				
				if(PLAYER_TEMP[playerid][pt_POLICE_PEN_IM] > PI[playerid][ac_CASH])
				{
					SendNotification(playerid, "No tienes dinero para pagar la multa.");
					SendClientMessage(PLAYER_TEMP[playerid][pt_POLICE_PEN_PID], -1, "{"#SILVER_COLOR"}La persona ha aceptado la multa pero no tiene dinero para pagarla.");
					return 1;
				}
				
				if(GivePlayerCash(playerid, -PLAYER_TEMP[playerid][pt_POLICE_PEN_IM], true, true) && GivePlayerCash(PLAYER_TEMP[playerid][pt_POLICE_PEN_PID], PLAYER_TEMP[playerid][pt_POLICE_PEN_IM]/2, true, false)) {
					SetPlayerWantedLevelEx(playerid, 0);

					SendNotification(playerid, "La multa ha sido pagada.");
					SendClientMessage(PLAYER_TEMP[playerid][pt_POLICE_PEN_PID], -1, "{"#SILVER_COLOR"}La persona ha pagado la multa, la mitad del importe es para ti.");
				}
			
			}
			else
			{
				if(gettime() > PLAYER_TEMP[playerid][pt_POLICE_PEN_TIME] + 20) return 1;
				if(!IsPlayerConnected(PLAYER_TEMP[playerid][pt_POLICE_PEN_PID])) return 1;
				if(ACCOUNT_INFO[ PLAYER_TEMP[playerid][pt_POLICE_PEN_PID] ][ac_ID] != PLAYER_TEMP[playerid][pt_POLICE_PEN_AID]) return 1;
				
				SendClientMessage(PLAYER_TEMP[playerid][pt_POLICE_PEN_PID], -1, "{"#SILVER_COLOR"}La persona se ha negado a pagar la multa.");
				if(PI[playerid][ac_WANTED_LEVEL] < 6) SetPlayerWantedLevelEx(playerid, PI[playerid][ac_WANTED_LEVEL] + 1);
			}
			return 1;
		}
		case DIALOG_SERVER_ECO:
		{
			if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] < 5) return 1;
			
			if(response)
			{
				PLAYER_TEMP[playerid][pt_DIALOG_ECO_SELECTED] = listitem;
				ShowDialog(playerid, DIALOG_SERVER_ECO_MODIFY);
			}
			return 1;
		}
		case DIALOG_SERVER_ECO_MODIFY:
		{
			if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] < 5) return 1;
			
			if(response)
			{
				new Float:multiplier;
				if(sscanf(inputtext, "f", multiplier))
				{
					SendNotification(playerid, "Sintaxis no válida.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				switch(PLAYER_TEMP[playerid][pt_DIALOG_ECO_SELECTED])
				{
					case 0: MULTIPLIER_PROPERITES = multiplier;
					case 1: MULTIPLIER_VEHICLES = multiplier;
					case 2: MULTIPLIER_TOYS = multiplier;
					case 3: MULTIPLIER_SKINS = multiplier;
					case 4: MULTIPLIER_WEAPONS = multiplier;
					case 5: SU_SD_PRICE = floatround(multiplier);
					case 6: SU_WORK_EXTRA_PAY = floatround(multiplier);
					case 7: CHANGE_NAME_SD_PRICE = floatround(multiplier);
				}
				
				WriteEconomyMultipliers();
				UpdateEconomy(PLAYER_TEMP[playerid][pt_DIALOG_ECO_SELECTED]);
				ShowDialog(playerid, DIALOG_SERVER_ECO);
			}
			else ShowDialog(playerid, DIALOG_SERVER_ECO);
			return 1;
		}
		case DIALOG_SU_BUY:
		{
			if(response)
			{
				if(SU_SD_PRICE > PI[playerid][ac_COINS])
				{
					SendFormatNotification(playerid, "Te faltan %d "SERVER_COIN" para poder comprar VIP.", SU_SD_PRICE - PI[playerid][ac_COINS]);
					return 1;
				}
				
				SetPlayerVip(playerid, 1, SU_SD_PRICE, 30);
			}
			return 1;
		}
		case DIALOG_SU:
		{
			if(!response)
			{
				if(SU_SD_PRICE > PI[playerid][ac_COINS])
				{
					SendFormatNotification(playerid, "Te faltan %d "SERVER_COIN" para poder comprar VIP.", SU_SD_PRICE - PI[playerid][ac_COINS]);
					return 1;
				}
				
				PI[playerid][ac_COINS] -= SU_SD_PRICE;
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
								SendClientMessageEx(playerid, -1, "{"#PRIMARY_COLOR"}¡Felicidades! {FFFFFF}Has renovado tu VIP (%d) por 30 días, fecha de caducidad: %s.", ACCOUNT_INFO[playerid][ac_VIP], ACCOUNT_INFO[playerid][ac_VIP_EXPIRE_DATE]);
								SendClientMessage(playerid, -1, "Puedes utilizar {"#PRIMARY_COLOR"}/vip {FFFFFF}para ver el tiempo restante o renovar.");
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT vip_expire_date FROM player WHERE id = %d;", ACCOUNT_INFO[playerid][ac_ID]);
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET coins = %d, vip = %d, vip_expire_date = DATE_ADD(NOW(), INTERVAL 30 DAY) WHERE id = %d;", PI[playerid][ac_COINS], ACCOUNT_INFO[playerid][ac_VIP], ACCOUNT_INFO[playerid][ac_ID]);
				MySQL_TQueryInline(srp_db, using inline OnInfoQueryLoad, QUERY_BUFFER);
			}
			return 1;
		}
		case DIALOG_VOBJECT_MENU:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: ShowDialog(playerid, DIALOG_VOBJECT_OBJECTS);
					case 1: //Comprar
					{
						new slot = GetVehicleFreeObjectSlot(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID]);
						if(!ACCOUNT_INFO[playerid][ac_VIP] && slot >= MAX_NU_VOBJECTS)
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendFormatNotification(playerid, "¡Los jugadores ~y~VIP ~w~pueden tener hasta %d objetos en vehículos! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP.", MAX_SU_VOBJECTS);
							ShowDialog(playerid, dialogid);
							return 1;
						}
						if(slot == -1)
						{
							SendNotification(playerid, "No hay más espacio para objetos en este vehículo.");
							ShowDialog(playerid, dialogid);
							return 1;
						}
						
						PLAYER_TEMP[playerid][pt_TUNING_GARAGE_SHOP] = true;
						
						new Float:p[6];
						GetPlayerCameraPos(playerid, p[0], p[1], p[2]);
						GetPlayerCameraLookAt(playerid, p[3], p[4], p[5]);
						InterpolateCameraPos(playerid, p[0], p[1], p[2], 265.3348, 14.0199, 3.6937, 2000);
						InterpolateCameraLookAt(playerid, p[3], p[4], p[5], 268.2404, 11.4559, 2.2, 2000);
						
						TextDrawShowForPlayer(playerid, Textdraws[textdraw_SHOP_TUNING][0]);
						TextDrawShowForPlayer(playerid, Textdraws[textdraw_SHOP_TUNING][1]);
						TextDrawShowForPlayer(playerid, Textdraws[textdraw_SHOP_TUNING][2]);
						TextDrawShowForPlayer(playerid, Textdraws[textdraw_SHOP_TUNING][3]);
						TextDrawShowForPlayer(playerid, Textdraws[textdraw_SHOP_TUNING][4]);
						PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_TUNING_SHOP]);
						//PLAYER_TEMP[playerid][pt_SELECT_TUNING_OBJECT] = 0;
						
						UpdateTuningShop(playerid);
						
						SendNotification(playerid, "Pulsa ~r~'ESCAPE' ~w~para dejar de comprar objetos.");
						SelectTextDrawEx(playerid, 0xe8d08fFF);
					}
					case 2:
					{
						new slot = GetVehicleFreeObjectSlot(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID]);
						if(!ACCOUNT_INFO[playerid][ac_VIP] && slot >= MAX_NU_VOBJECTS)
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendFormatNotification(playerid, "¡Los jugadores ~y~VIP ~w~pueden tener hasta %d objetos en vehículos! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP.", MAX_SU_VOBJECTS);
							ShowDialog(playerid, dialogid);
							return 1;
						}
						if(slot == -1)
						{
							SendNotification(playerid, "No hay más espacio para objetos en este vehículo.");
							ShowDialog(playerid, dialogid);
							return 1;
						}
						
						if(500 > PI[playerid][ac_CASH])
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar este objeto.", number_format_thousand(500 - PI[playerid][ac_CASH]));
							ShowDialog(playerid, dialogid);
							return 1;
						}
						
						if(GivePlayerCash(playerid, -500, true, true)) {
							VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_VALID] = true;
							VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_TYPE] = VOBJECT_TYPE_TEXT;
							VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_ID] = 0;
							VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_MODELID] = 19327;
							format(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_NAME], 32, "Texto");
							VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_ATTACHED] = true;
							
							format(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_text_TEXT], 32, "Mi texto");
							format(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_text_FONT], 24, "Arial");
							VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_text_FONT_SIZE] = 24;
							VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_text_BOLD] = false;
							VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_text_FONT_COLOR] = 0xFF000000;
							
							RegisterNewVehicleObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], slot);
							UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], slot, true);
							
							SendNotification(playerid, "El objeto de texto ha sido añadido.");
							PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] = slot;
							ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
						}
						else ShowDialog(playerid, dialogid);
					}
					case 3:
					{
						new slot = GetVehicleFreeObjectSlot(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID]);
						if(!ACCOUNT_INFO[playerid][ac_VIP] && slot >= MAX_NU_VOBJECTS)
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendFormatNotification(playerid, "¡Los jugadores ~y~VIP ~w~pueden tener hasta %d objetos en vehículos! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP.", MAX_SU_VOBJECTS);
							ShowDialog(playerid, dialogid);
							return 1;
						}
						if(slot == -1)
						{
							SendNotification(playerid, "No hay más espacio para objetos en este vehículo.");
							ShowDialog(playerid, dialogid);
							return 1;
						}
						
						if(2000 > PI[playerid][ac_CASH])
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar este objeto.", number_format_thousand(2000 - PI[playerid][ac_CASH]));
							ShowDialog(playerid, dialogid);
							return 1;
						}
						
						PLAYER_TEMP[playerid][pt_COUNTRY_TYPE] = 0;
						ShowDialog(playerid, DIALOG_VOBJECT_COUNTRY);
					}
					case 4:
					{
						new slot = GetVehicleFreeObjectSlot(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID]);
						if(!ACCOUNT_INFO[playerid][ac_VIP] && slot >= MAX_NU_VOBJECTS)
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendFormatNotification(playerid, "¡Los jugadores ~y~VIP ~w~pueden tener hasta %d objetos en vehículos! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP.", MAX_SU_VOBJECTS);
							ShowDialog(playerid, dialogid);
							return 1;
						}
						if(slot == -1)
						{
							SendNotification(playerid, "No hay más espacio para objetos en este vehículo.");
							ShowDialog(playerid, dialogid);
							return 1;
						}
						
						if(2000 > PI[playerid][ac_CASH])
						{
							PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
							SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar este objeto.", number_format_thousand(2000 - PI[playerid][ac_CASH]));
							ShowDialog(playerid, dialogid);
							return 1;
						}
						
						PLAYER_TEMP[playerid][pt_COUNTRY_TYPE] = 1;
						ShowDialog(playerid, DIALOG_VOBJECT_COUNTRY);
					}
				}
			}
			else
			{
				PLAYER_TEMP[playerid][pt_IN_TUNING_GARAGE] = false;
				SetVehicleVirtualWorldEx(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], 0);
				SetPlayerVirtualWorld(playerid, 0);
				SetVehiclePosEx(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], 246.551910, 17.474195, 2.363841);
				SetVehicleZAngle(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], 0.0);
				
				GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_PARAMS_ENGINE] = 1;
				UpdateVehicleParams(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID]);
				
				if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) PutPlayerInVehicleEx(playerid, PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], 0);
				
				ShowPlayerSpeedoMeter(playerid);
				SetCameraBehindPlayer(playerid);
			}
			return 1;
		}
		case DIALOG_VOBJECT_COUNTRY:
		{
			if(response)
			{
				new slot = GetVehicleFreeObjectSlot(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID]);
				if(!ACCOUNT_INFO[playerid][ac_VIP] && slot >= MAX_NU_VOBJECTS)
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendFormatNotification(playerid, "¡Los jugadores ~y~VIP ~w~pueden tener hasta %d objetos en vehículos! Usa ~y~/ayuda ~w~si quieres ser ~y~VIP.", MAX_SU_VOBJECTS);
					ShowDialog(playerid, DIALOG_VOBJECT_MENU);
					return 1;
				}
				if(slot == -1)
				{
					SendNotification(playerid, "No hay más espacio para objetos en este vehículo.");
					ShowDialog(playerid, DIALOG_VOBJECT_MENU);
					return 1;
				}
				
				if(2000 > PI[playerid][ac_CASH])
				{
					PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
					SendFormatNotification(playerid, "No tienes dinero suficiente, te faltan ~r~%s$~w~ para poder comprar este objeto.", number_format_thousand(2000 - PI[playerid][ac_CASH]));
					ShowDialog(playerid, DIALOG_VOBJECT_MENU);
					return 1;
				}
				
				if(GivePlayerCash(playerid, -2000, true, true)) {
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_VALID] = true;
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_ID] = 0;
					if(PLAYER_TEMP[playerid][pt_COUNTRY_TYPE] == 0)
					{
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_TYPE] = VOBJECT_TYPE_COUNTRY_FLAG;
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_MODELID] = 19307;
					}
					else if(PLAYER_TEMP[playerid][pt_COUNTRY_TYPE] == 1)
					{
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_TYPE] = VOBJECT_TYPE_COUNTRY_PANEL;
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_MODELID] = 2661;
					}
					format(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_NAME], 32, "Pais");
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_ATTACHED] = true;
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][slot][vobject_COLORS][0] = listitem;
					
					RegisterNewVehicleObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], slot);
					UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], slot, true);
					
					SendNotification(playerid, "El objeto ha sido añadido.");
					PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] = slot;
					ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
				}
				else ShowDialog(playerid, DIALOG_VOBJECT_MENU);
			}
			else ShowDialog(playerid, DIALOG_VOBJECT_MENU);
			return 1;
		}
		case DIALOG_VOBJECT_OBJECTS:
		{
			if(response)
			{
				if(listitem == MAX_SU_VOBJECTS) return ShowDialog(playerid, DIALOG_VOBJECT_DELETE_ALL);
				if(!VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][listitem][vobject_VALID])
				{
					SendNotification(playerid, "Este slot está vacío, aquí puedes comprar un nuevo objeto.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] = listitem;
				ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
			}
			else ShowDialog(playerid, DIALOG_VOBJECT_MENU);
			return 1;
		}
		case DIALOG_VOBJECT_OBJECT:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0:
					{
						if(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ATTACHED])
						{
							SendNotification(playerid, "Objeto guardado.");
							VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ATTACHED] = false;
							UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT]);
							ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
						}
						else
						{
							SendNotification(playerid, "Objeto colocado.");
							VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ATTACHED] = true;
							UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT], true);
							ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
						}
					}
					case 1: ShowDialog(playerid, DIALOG_VOBJECT_OBJECT_CNAME);
					case 2:
					{
						if(!VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ATTACHED])
						{
							SendNotification(playerid, "Debes colocar el objeto primero.");
							ShowDialog(playerid, dialogid);
							return 1;
						}
							
						EditVehicleObject(playerid, PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT]);
					}
					case 3:
					{
						switch(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_TYPE])
						{
							case VOBJECT_TYPE_OBJECT:
							{
								if(!VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ATTACHED])
								{
									SendNotification(playerid, "Debes colocar el objeto primero.");
									ShowDialog(playerid, dialogid);
									return 1;
								}
								ShowDialog(playerid, DIALOG_VOBJECT_OBJECT_COLORS);
							}
							case VOBJECT_TYPE_TEXT:
							{
								if(!VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ATTACHED])
								{
									SendNotification(playerid, "Debes colocar el objeto primero.");
									ShowDialog(playerid, dialogid);
									return 1;
								}
								ShowDialog(playerid, DIALOG_VOBJECT_TEXT_MENU);
							}
							case VOBJECT_TYPE_COUNTRY_FLAG, VOBJECT_TYPE_COUNTRY_PANEL: ShowDialog(playerid, DIALOG_VOBJECT_OBJECT_CDELETE);
						}
					}
					case 4: ShowDialog(playerid, DIALOG_VOBJECT_OBJECT_CDELETE);
				}
			
			}
			else
			{
				UpdateVehicleObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT]);
				ShowDialog(playerid, DIALOG_VOBJECT_OBJECTS);
			}
			return 1;
		}
		case DIALOG_VOBJECT_OBJECT_CNAME:
		{
			if(response)
			{
				if(!strlen(inputtext) || strlen(inputtext) > 32)
				{
					SendNotification(playerid, "El nombre del objeto debe contener de 1 a 32 caracteres.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				format(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_NAME], 32, "%s", inputtext);
				ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
			}
			else ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
			return 1;
		}
		case DIALOG_VOBJECT_OBJECT_COLORS:
		{
			if(response)
			{
				PLAYER_TEMP[playerid][pt_TUNING_EDIT_COLOR_SLOT] = listitem;
				ShowDialog(playerid, DIALOG_VOBJECT_OBJECT_COLOR);
			}
			else ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
			return 1;
		}
		case DIALOG_VOBJECT_OBJECT_COLOR:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0:
					{
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_COLORS][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_COLOR_SLOT] ] = 0;
						UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT], true);
						SendNotification(playerid, "Color actualizado.");
						ShowDialog(playerid, DIALOG_VOBJECT_OBJECT_COLORS);
					}
					case 1:
					{
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_COLORS][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_COLOR_SLOT] ] = RGBAToARGB(VEHICLE_COLORS[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_COLOR_1] ]);
						UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT], true);
						SendNotification(playerid, "Color actualizado.");
						ShowDialog(playerid, DIALOG_VOBJECT_OBJECT_COLORS);
					}
					case 2:
					{
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_COLORS][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_COLOR_SLOT] ] = RGBAToARGB(VEHICLE_COLORS[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_COLOR_2] ]);
						UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT], true);
						SendNotification(playerid, "Color actualizado.");
						ShowDialog(playerid, DIALOG_VOBJECT_OBJECT_COLORS);
					}
					case 3: ShowDialog(playerid, DIALOG_VOBJECT_OBJECT_COLOR_HEX);
				}
			}
			else ShowDialog(playerid, DIALOG_VOBJECT_OBJECT_COLORS);
			return 1;
		}
		case DIALOG_VOBJECT_OBJECT_COLOR_HEX:
		{
			if(response)
			{
				if(strlen(inputtext) < 8) return ShowDialog(playerid, dialogid);
				
				new hexcolor;
				if(sscanf(inputtext, "h", hexcolor))
				{
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_COLORS][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_COLOR_SLOT] ] = hexcolor;
				UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT], true);
				SendNotification(playerid, "Color actualizado.");
				ShowDialog(playerid, DIALOG_VOBJECT_OBJECT_COLORS);
			}
			else ShowDialog(playerid, DIALOG_VOBJECT_OBJECT_COLOR);
			return 1;
		}
		case DIALOG_VOBJECT_OBJECT_CDELETE:
		{
			if(response)
			{
				if(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OBJECT_ID] != INVALID_STREAMER_ID)
				{
					DestroyDynamicObject(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OBJECT_ID]);
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_OBJECT_ID] = INVALID_STREAMER_ID;
				}
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM vobjects WHERE id = %d;", VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				new temp_VEHICLE_OBJECTS[e_VEHICLE_OBJECTS];
				VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ] = temp_VEHICLE_OBJECTS;
				
				SendNotification(playerid, "Objeto eliminado.");
				ShowDialog(playerid, DIALOG_VOBJECT_OBJECTS);
			}
			else ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
			return 1;
		}
		case DIALOG_VOBJECT_DELETE_ALL:
		{
			if(response)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM vobjects WHERE id_vehicle = %d;", PLAYER_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][player_vehicle_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);

				new temp_VEHICLE_OBJECTS[e_VEHICLE_OBJECTS];
				for(new i = 0; i != MAX_SU_VOBJECTS; i ++)
				{
					if(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][i][vobject_OBJECT_ID] != INVALID_STREAMER_ID)
					{
						DestroyDynamicObject(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][i][vobject_OBJECT_ID]);
						VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][i][vobject_OBJECT_ID] = INVALID_STREAMER_ID;
					}
					
					VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][i] = temp_VEHICLE_OBJECTS;
				}
				
				SendNotification(playerid, "Todos los objetos de este vehículo han sido eliminados.");
				ShowDialog(playerid, DIALOG_VOBJECT_OBJECTS);
			}
			else ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
			return 1;
		}
		case DIALOG_VOBJECT_TEXT_MENU:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: ShowDialog(playerid, DIALOG_VOBJECT_TEXT_TEXT);
					case 1: ShowDialog(playerid, DIALOG_VOBJECT_TEXT_FONT);
					case 2: ShowDialog(playerid, DIALOG_VOBJECT_TEXT_FONT_SIZE);
					case 3:
					{
						if(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_text_BOLD])
						{
							VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_text_BOLD] = false;
							SendNotification(playerid, "Negrita deshabilitada.");
						}
						else
						{
							VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_text_BOLD] = true;
							SendNotification(playerid, "Negrita habilitada.");
						}
						UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT], true);
						ShowDialog(playerid, dialogid);
					}
					case 4: ShowDialog(playerid, DIALOG_VOBJECT_TEXT_FONT_COLOR);
				}
			}
			else ShowDialog(playerid, DIALOG_VOBJECT_OBJECT);
			return 1;
		}
		case DIALOG_VOBJECT_TEXT_TEXT:
		{
			if(response)
			{
				if(strlen(inputtext) >= 32)
				{
					SendNotification(playerid, "Como máximo puedes introducir un texto de 32 caracteres.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				if(sscanf(inputtext, "s[32]", inputtext))
				{
					SendNotification(playerid, "Introduce un texto.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				format(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_text_TEXT], 32, "%s", inputtext);
				UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT], true);
				SendNotification(playerid, "Texto actualizado.");
			}
			ShowDialog(playerid, DIALOG_VOBJECT_TEXT_MENU);
			return 1;
		}
		case DIALOG_VOBJECT_TEXT_FONT:
		{
			if(response)
			{
				if(strlen(inputtext) >= 24)
				{
					SendNotification(playerid, "Como máximo puedes introducir una fuente de 24 caracteres.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				
				if(sscanf(inputtext, "s[24]", inputtext))
				{
					SendNotification(playerid, "Introduce un texto.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				format(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_text_FONT], 24, "%s", inputtext);
				UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT], true);
				SendNotification(playerid, "Fuente actualizada.");
			}
			ShowDialog(playerid, DIALOG_VOBJECT_TEXT_MENU);
			return 1;
		}
		case DIALOG_VOBJECT_TEXT_FONT_SIZE:
		{
			if(response)
			{
				if(sscanf(inputtext, "d", inputtext[0]))
				{
					SendNotification(playerid, "Introduce un valor numérico.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				if(inputtext[0] <= 0 || inputtext[0] >= 255)
				{
					SendNotification(playerid, "Introduce un valor entre 1 y 255.");
					ShowDialog(playerid, dialogid);
					return 1;
				}
				VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_text_FONT_SIZE] = inputtext[0];
				UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT], true);
				SendNotification(playerid, "Tamaño de letra actualizado.");
			}
			ShowDialog(playerid, DIALOG_VOBJECT_TEXT_MENU);
			return 1;
		}
		case DIALOG_VOBJECT_TEXT_FONT_COLOR:
		{
			if(response)
			{
				VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_text_FONT_COLOR] = RGBAToARGB(RandomColors[listitem]);
				UpdateVehicleAttachedObject(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT], true);
				SendNotification(playerid, "Color de letra actualizado.");
			}
			ShowDialog(playerid, DIALOG_VOBJECT_TEXT_MENU);
			return 1;
		}
		
		case DIAlOG_BUY_MASO:
		{
			
			if(response)
			{
				
				if(PI[playerid][ac_CASH] < 300) return SendNotification(playerid, "No tienes suficiente dinero.");
				{
								
					if(PI[playerid][ac_Maso])
					{
										
						SendNotification(playerid, "Ya tienes un maso.");
						PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
						return 1;
					}		
					PI[playerid][ac_Maso] = 1;
					SendNotification(playerid, "Compraste un maso por $300.");
					PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);			
					GivePlayerCash(playerid, -300, true, true);	
				}
			}
			return 1;
		}
		
		
		
		case DIALOG_PLAYER_CONFIG:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0:
					{
						if(PI[playerid][ac_CONFIG_AUDIO])
						{
							PI[playerid][ac_CONFIG_AUDIO] = false;
							StopAudioStreamForPlayer(playerid);
						}
						else PI[playerid][ac_CONFIG_AUDIO] = true;
					}
					case 1:
					{
						if(PI[playerid][ac_CONFIG_HUD])
						{
							PI[playerid][ac_CONFIG_HUD] = false;
							for(new i = 0; i < HUD_TEXTDRAWS_SIZE; i ++) {
								TextDrawHideForPlayer(playerid, Textdraws[textdraw_HUD][i]);
							}
							
							for(new i = 0; i < HUD_PTEXTDRAWS_SIZE; i ++) {
								PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][i]);
							}
						}
						else
						{
							PI[playerid][ac_CONFIG_HUD] = true;
							for(new i = 0; i < HUD_TEXTDRAWS_SIZE; i ++) {
								TextDrawShowForPlayer(playerid, Textdraws[textdraw_HUD][i]);	
							}
							
							for(new i = 0; i < HUD_PTEXTDRAWS_SIZE; i ++) {
								PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][i]);
							}
						}
					}
					case 2:
					{
						if(PI[playerid][ac_CONFIG_ADMIN]) PI[playerid][ac_CONFIG_ADMIN] = false;
						else PI[playerid][ac_CONFIG_ADMIN] = true;
					}
					case 3:
					{
						if(PI[playerid][ac_PHONE_VISIBLE_NUMBER]) PI[playerid][ac_PHONE_VISIBLE_NUMBER] = false;
						else PI[playerid][ac_PHONE_VISIBLE_NUMBER] = true;
					}
					case 4:
					{
						if(PI[playerid][ac_CONFIG_SOUNDS]) PI[playerid][ac_CONFIG_SOUNDS] = false;
						else PI[playerid][ac_CONFIG_SOUNDS] = true;
					}
					case 5:
					{
						if(PI[playerid][ac_CONFIG_TIME])
						{
							PI[playerid][ac_CONFIG_TIME] = false;
							TextDrawHideForPlayer(playerid, Textdraws[textdraw_SERVER_TIME]);
						}
						else
						{
							PI[playerid][ac_CONFIG_TIME] = true;
							TextDrawShowForPlayer(playerid, Textdraws[textdraw_SERVER_TIME]);
						}
					}
					case 6:
					{
						if(ACCOUNT_INFO[playerid][ac_DOUBT_CHANNEL]) ACCOUNT_INFO[playerid][ac_DOUBT_CHANNEL] = false;
						else ACCOUNT_INFO[playerid][ac_DOUBT_CHANNEL] = true;
					}
					case 7: {
						PI[playerid][ac_CONFIG_SECURE_LOGIN] = !PI[playerid][ac_CONFIG_SECURE_LOGIN];
					}
				}
				ShowDialog(playerid, dialogid);
			}
			return 1;
		}
		case DIALOG_POLICE_BYC:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0:
					{
						PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] = 10;
						PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
						ShowDialog(playerid, DIALOG_POLICE_BYC_LAST_ALL);
					}
					case 1: ShowDialog(playerid, DIALOG_POLICE_BYC_NAME_FIND);
				}
			}
			return 1;
		}
		case DIALOG_POLICE_BYC_NAME_FIND:
		{
			if(response)
			{
				new name[24];
				if(sscanf(inputtext, "s[24]", name)) return ShowDialog(playerid, dialogid);
				
				inline OnDialogQueryLoad()
				{
					new rows;
					if(cache_get_row_count(rows))
					{
						if(rows)
						{
							cache_get_value_index_int(0, 0, PLAYER_TEMP[playerid][pt_SELECTED_BYC_USER_ID]);
							PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] = 10;
							PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] = 0;
							ShowDialog(playerid, DIALOG_POLICE_BYC_LAST_PLAYER);
						}
						else SendNotification(playerid, "No se ha encontrado a esa persona.");
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM player WHERE name = '%e';", name);
				MySQL_TQueryInline(srp_db, using inline OnDialogQueryLoad, QUERY_BUFFER);
			}
			return 1;
		}
		case DIALOG_POLICE_BYC_LAST_ALL:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -2) // Siguiente
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, "SELECT COUNT(id) FROM police_history;");
				}
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -3) //Anterior
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, "SELECT COUNT(id) FROM police_history;");
				}
				else
				{
					PLAYER_TEMP[playerid][pt_SELECTED_BYC_ID] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
					ShowDialog(playerid, DIALOG_POLICE_DELETE_BYC);
				}
			}
			return 1;
		}
		case DIALOG_POLICE_BYC_LAST_PLAYER:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -2) // Siguiente
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM police_history WHERE id_player = %d;", PLAYER_TEMP[playerid][pt_SELECTED_BYC_USER_ID]);
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
				}
				else if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -3) //Anterior
				{
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
								ShowDialog(playerid, dialogid);
							}
						}
					}
					mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM police_history WHERE id_player = %d;", PLAYER_TEMP[playerid][pt_SELECTED_BYC_USER_ID]);
					MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
				}
				else
				{
					PLAYER_TEMP[playerid][pt_SELECTED_BYC_ID] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
					ShowDialog(playerid, DIALOG_POLICE_DELETE_BYC);
				}
			}
			return 1;
		}
		case DIALOG_POLICE_DELETE_BYC:
		{
			if(response)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM police_history WHERE id = %d;", PLAYER_TEMP[playerid][pt_SELECTED_BYC_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				SendNotification(playerid, "Registro borrado del historial policial.");
			}
			return 1;
		}
		case DIALOG_CHANGE_NAME:
		{
			if(response)
			{
				if(!strlen(inputtext)) return ShowDialog(playerid, dialogid);
				
				new password[64 + 1];
				SHA256_PassHash(inputtext, ACCOUNT_INFO[playerid][ac_SALT], password, sizeof password);
			
				if(!strcmp(password, ACCOUNT_INFO[playerid][ac_PASS], false))
				{
					ShowDialog(playerid, DIALOG_CHANGE_NAME_NAME);
					PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] = 0;
				}
				else // Error
				{
					PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] ++;
					if(PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] > MAX_BAD_LOGIN_ATTEMPS) return Kick(playerid);
					SendFormatNotification(playerid, "Contraseña incorrecta, aviso %d/%d.", PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP], MAX_BAD_LOGIN_ATTEMPS);
				}
			}
			return 1;
		}
		case DIALOG_CHANGE_NAME_NAME:
		{
			if(response)
			{
				new length = strlen(inputtext);
				if(length < 3 || length > 20) return ShowDialog(playerid, dialogid);

				new name[24];
				format(name, 24, "%s", inputtext);
				if(!IsValidRPName(name)) return SendFormatNotification(playerid, "El nombre '%s' no cumple con el formato Nombre_Apellido.", name);
	
				inline OnDialogQueryLoad()
				{
					new rows;
					if(cache_get_row_count(rows))
					{
						if(rows) SendFormatNotification(playerid, "El nombre '%s' está en uso.", name);
						else
						{
							printf("/cname: %d, '%s'", ACCOUNT_INFO[playerid][ac_ID], name);
							if(SetPlayerName(playerid, name) == 1) {
								format(PLAYER_TEMP[playerid][pt_NAME], 24, "%s", name);
								format(ACCOUNT_INFO[playerid][ac_NAME], 24, "%s", name);
								PLAYER_TEMP[playerid][pt_RP_NAME] = RP_GetPlayerName(playerid);
								
								for(new i = 0; i != MAX_PROPERTIES; i ++)
								{
									if(!PROPERTY_INFO[i][property_VALID]) continue;
									if(PROPERTY_INFO[i][property_OWNER_ID] != ACCOUNT_INFO[playerid][ac_ID]) continue;
									
									new label_str[256];
									format
									(
										label_str, 
										sizeof label_str, 
										"\
											Propiedad {"#PRIMARY_COLOR"}#%d\n\n\
											{FFFFFF}Propietario: {"#PRIMARY_COLOR"}%s\n\
											{FFFFFF}Presiona {"#PRIMARY_COLOR"}~k~~CONVERSATION_YES~ {FFFFFF}para entrar.\
										", PROPERTY_INFO[i][property_ID], ACCOUNT_INFO[playerid][ac_NAME]
									);
									UpdateDynamic3DTextLabelText(PROPERTY_INFO[i][property_EXT_LABEL_ID], 0xFFFFFFFF, label_str);
								}

								PI[playerid][ac_COINS] -= CHANGE_NAME_SD_PRICE;

								mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET name = '%e', coins = %d WHERE id = %d;", ACCOUNT_INFO[playerid][ac_NAME], PI[playerid][ac_COINS], ACCOUNT_INFO[playerid][ac_ID]);
								printf(QUERY_BUFFER);
								mysql_tquery(srp_db, QUERY_BUFFER);
								SendFormatNotification(playerid, "Tu nombre ha sido cambiado a '%s' por %d "SERVER_COIN"", name, CHANGE_NAME_SD_PRICE);
							}
							else SendNotification(playerid, "No se ha podido cambiar tu nombre.");
						}
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM player WHERE name = '%e';", name);
 				MySQL_TQueryInline(srp_db, using inline OnDialogQueryLoad, QUERY_BUFFER, "s", name);

			}
			return 1;
		}
		case DIALOG_CHANGE_EMAIL:
		{
			if(response)
			{
				if(!strlen(inputtext)) return ShowDialog(playerid, dialogid);
				
				new password[64 + 1];
				SHA256_PassHash(inputtext, ACCOUNT_INFO[playerid][ac_SALT], password, sizeof password);
			
				if(!strcmp(password, ACCOUNT_INFO[playerid][ac_PASS], false))
				{
					ShowDialog(playerid, DIALOG_CHANGE_EMAIL_EMAIL);
					PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] = 0;
				}
				else // Error
				{
					PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] ++;
					if(PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] > MAX_BAD_LOGIN_ATTEMPS) return Kick(playerid);
					SendFormatNotification(playerid, "Contraseña incorrecta, aviso %d/%d.", PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP], MAX_BAD_LOGIN_ATTEMPS);
				}
			}
			return 1;
		}
		case DIALOG_CHANGE_EMAIL_EMAIL:
		{
			if(response)
			{
				new length = strlen(inputtext);
				if(length > 32) return ShowDialog(playerid, dialogid);

				new email[32];
				format(email, 32, "%s", inputtext);
				if(!IsValidEmail(email)) return SendNotification(playerid, "Correo no válido.");
	
				inline OnDialogQueryLoad()
				{
					new rows;
					if(cache_get_row_count(rows))
					{
						if(rows) SendNotification(playerid, "El correo electrónico introducido está en uso.");
						else
						{
							format(ACCOUNT_INFO[playerid][ac_EMAIL], 32, "%s", email);
							
							mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET email = '%e' WHERE id = %d;", ACCOUNT_INFO[playerid][ac_EMAIL], ACCOUNT_INFO[playerid][ac_ID]);
							mysql_tquery(srp_db, QUERY_BUFFER);
							SendNotification(playerid, "Tu correo electrónico ha sido cambiado correctamente.");
						}
					}
				}
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM player WHERE email = '%e';", email);
				MySQL_TQueryInline(srp_db,  using inline OnDialogQueryLoad, QUERY_BUFFER, "s", email);
			}
			return 1;
		}
		case DIALOG_PROPERTY_CLOSET:
		{
			if(response)
			{
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;
				if(PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == MAX_CLOSET_SLOTS + 20)
				{
					ShowDialog(playerid, DIALOG_PROPERTY_CLOSET_DLT_ALL);
					return 1;
				}
				
				PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
				ShowDialog(playerid, DIALOG_PROPERTY_CLOSET_OPTIONS);
			}
			return 1;
		}
		case DIALOG_PROPERTY_CLOSET_DLT_ALL:
		{
			if(response)
			{
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM property_closet WHERE id_property = %d;", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][property_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				new tmp_PROPERTY_CLOSET[enum_PROPERTY_CLOSET];
				for(new i = 0; i != MAX_CLOSET_SLOTS; i ++) PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][i] = tmp_PROPERTY_CLOSET;
		
				SendNotification(playerid, "Ha eliminado todo de este armario.");
				Auto_SendPlayerAction(playerid, "tira todas las armas de su armario");
			}
			return 1;
		}
		case DIALOG_PROPERTY_CLOSET_OPTIONS:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0:
					{
						if(!PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_VALID]) return SendNotification(playerid, "No hay nada en ese slot.");
						
						new string[128];
						switch(PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_TYPE]) {
							case BOOT_TYPE_WEAPON: {
								new to_slot = WEAPON_INFO[ PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_INT] ][weapon_info_SLOT];
								if(PLAYER_WEAPONS[playerid][to_slot][player_weapon_VALID])
								{
									PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
									SendFormatNotification(playerid, "Para sacar esta arma tienes que deshacerte de tu ~r~\"%s (%d\"~w~ para tener espacio.", WEAPON_INFO[ PLAYER_WEAPONS[playerid][ to_slot ][player_weapon_ID] ][weapon_info_NAME], to_slot);
									return 1;
								}
								GivePlayerWeaponEx(playerid, PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_INT], PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_INT_EXTRA]);
								RegisterNewPlayerWeapon(playerid, to_slot);
								format(string, sizeof string, "saca un(a) %s del armario.", WEAPON_INFO[ PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_INT] ][weapon_info_NAME]);
							}
							case BOOT_TYPE_MEDICINES: {
								PI[playerid][ac_MEDICINE] += PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_INT];
								format(string, sizeof string, "saca medicamentos del armario.");
							}
							case BOOT_TYPE_CANNABIS: {
								PI[playerid][ac_CANNABIS] += PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_INT];
								format(string, sizeof string, "saca marihuana del armario.");
							}
							case BOOT_TYPE_CRACK: {
								PI[playerid][ac_CRACK] += PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_INT];
								format(string, sizeof string, "saca crack del armario.");
							}
						}

						mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM property_closet WHERE id = %d;", PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_OBJECT_ID]);
						mysql_tquery(srp_db, QUERY_BUFFER);

						new tmp_PROPERTY_CLOSET[enum_PROPERTY_CLOSET];
						PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ] = tmp_PROPERTY_CLOSET;
						Auto_SendPlayerAction(playerid, string);
					}
					case 1: ShowDialog(playerid, DIALOG_PROPERTY_CLOSET_DELETE);
				}
			}
			else ShowDialog(playerid, DIALOG_PROPERTY_CLOSET);
			return 1;
		}
		case DIALOG_PROPERTY_CLOSET_DELETE:
		{
			if(response)
			{
				SendNotification(playerid, "Eliminado.");
				
				mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "DELETE FROM property_closet WHERE id = %d;", PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_OBJECT_ID]);
				mysql_tquery(srp_db, QUERY_BUFFER);
				
				new tmp_PROPERTY_CLOSET[enum_PROPERTY_CLOSET];
				PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ] = tmp_PROPERTY_CLOSET;
			}
			else ShowDialog(playerid, DIALOG_PROPERTY_CLOSET_OPTIONS);
			return 1;
		}
	}
	return 0;
}