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
#if defined showdialog_included 
	#endinput
#endif
#define showdialog_included

ShowDialog(playerid, dialogid)
{
	PLAYER_TEMP[playerid][pt_PLAYER_WAITING_MP3_HTTP] = false;
	PLAYER_TEMP[playerid][pt_DIALOG_RESPONDED] = false;
	PLAYER_TEMP[playerid][pt_DIALOG_ID] = dialogid;
	
	switch(dialogid)
	{
		case DIALOG_REGISTER:
		{
			new dialog[140];
			format(dialog, sizeof dialog, "Esta cuenta no está registrada.\nIngrese su contraseña a continuación.\n\nLa contraseña debe contener entre %d-%d caracteres.", MIN_PASS_LENGTH, MAX_PASS_LENGTH);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_PASSWORD, "Registrarse", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_LOGIN: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_PASSWORD, "Ingresar", "Bienvenido de nuevo.\nIngrese su contraseña para acceder.", "Continuar", "Cerrar");
		case DIALOG_SECURE_LOGIN_INSERT_CODE: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Verifica tu cuenta", "Se ha enviado un código verificación\nal correo asociado a esta cuenta.\n\nIntroduce el código para acceder:", "Continuar", "Cerrar");
		case DIALOG_REGISTER_EMAIL: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Correo electrónico", "Ingresa una dirección de correo electrónico válida.\n\nEs importante que tengas acceso al correo ya que\nserá la única manera de recuperar tu contraseña.\n\nTranquilo, no recibirás ningún tipo de SPAM ni suscripciones.", "Continuar", "Cerrar");
		case DIALOG_CLOTHES: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "{FF7373}¿Qué quieres comprar?", "Ropa\nAccesorios", ">>", "Salir");
		case DIALOG_FOOD_PIZZA: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Comida rápida", DIALOG_FOOD_PIZZA_String, "Pedir", "Cerrar");
		case DIALOG_FOOD_CLUCKIN: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Comida rápida", DIALOG_FOOD_CLUCKIN_String, "Pedir", "Cerrar");
		case DIALOG_FOOD_BURGER: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Comida rápida", DIALOG_FOOD_BURGER_String, "Pedir", "Cerrar");
		case DIALOG_PLAYER_TOYS:
		{
			new dialog[96 * (MAX_SU_TOYS + 2)], line_str[64];
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Accesorio\t{"#BLUE_COLOR"}Colocado\n");
			for(new i; i != MAX_SU_TOYS; i ++)
			{
				if(ACCOUNT_INFO[playerid][ac_VIP])
				{
					if(PLAYER_TOYS[playerid][i][player_toy_VALID])
					{
						if(PLAYER_TOYS[playerid][i][player_toy_ATTACHED]) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#BLUE_COLOR"}Sí\n", i + 1, PLAYER_TOYS[playerid][i][player_toy_NAME]);
						else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#BLUE_COLOR"}No\n", i + 1, PLAYER_TOYS[playerid][i][player_toy_NAME]);
					}
					else format(line_str, sizeof line_str, "{666666}%d. Slot vacío\n", i + 1);
				}
				else
				{
					if(i >= MAX_NU_TOYS)
					{
						if(PLAYER_TOYS[playerid][i][player_toy_VALID]) format(line_str, sizeof line_str, "{666666}%d. %s\t{"#BLUE_COLOR"}No\n", i + 1, PLAYER_TOYS[playerid][i][player_toy_NAME]);
						else format(line_str, sizeof line_str, "{666666}%d. Slot vacío\n", i + 1);
					}
					else
					{
						if(PLAYER_TOYS[playerid][i][player_toy_VALID])
						{
							if(PLAYER_TOYS[playerid][i][player_toy_ATTACHED]) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#BLUE_COLOR"}Sí\n", i + 1, PLAYER_TOYS[playerid][i][player_toy_NAME]);
							else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#BLUE_COLOR"}No\n", i + 1, PLAYER_TOYS[playerid][i][player_toy_NAME]);
						}
						else format(line_str, sizeof line_str, "{666666}%d. Slot vacío\n", i + 1);
					}
				}
				strcat(dialog, line_str);
			}
			strcat(dialog, "{c4290d}- Eliminar todo\n");
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Mis accesorios", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_PLAYER_TOY_MENU:
		{
			if(!PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_VALID])
			{
				SendNotification(playerid, "Este slot está vacío, puedes comprar accesorios en cualquier tienda de ropa.");
				return 1;
			}
			
			new caption[48];
			format(caption, sizeof caption, "{"#SILVER_COLOR"}Accesorio '%s'", PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_NAME]);
			
			new dialog[256];
			if(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_ATTACHED]) format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Guardar este accesorio\n{"#SILVER_COLOR"}Cambiar el nombre del accesorio\n{"#SILVER_COLOR"}Configurar el accesorio\n{"#SILVER_COLOR"}Color primario\n{"#SILVER_COLOR"}Color secundario\n{"#SILVER_COLOR"}Eliminar este accesorio");
			else
			{
				if(PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_BONE] == -1)
				{
					SendNotification(playerid, "Este accesorio es nuevo y aún no ha sido configurado, configuralo desde 'Configurar el accesorio'.");
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Colocarte el accesorio\n{"#SILVER_COLOR"}Cambiar el nombre del accesorio\n{"#SILVER_COLOR"}Configurar el accesorio\n{"#SILVER_COLOR"}Color primario\n{"#SILVER_COLOR"}Color secundario\n{"#SILVER_COLOR"}Eliminar este accesorio");
				}
				else format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Colocarte el accesorio\n{"#SILVER_COLOR"}Cambiar el nombre del accesorio\n{"#SILVER_COLOR"}Configurar el accesorio\n{"#SILVER_COLOR"}Color primario\n{"#SILVER_COLOR"}Color secundario\n{"#SILVER_COLOR"}Eliminar este accesorio");
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_PLAYER_TOY_EDIT_NAME:
		{		
			new dialog[145];
			format(dialog, sizeof dialog, "Nombre actual: \"%s\"\nIndica el nuevo nombre:", PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_NAME]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "{"#SILVER_COLOR"}Accesorio - Modificar nombre", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_PLAYER_TOY_CONFIRMDELETE:
		{
			new dialog[145];
			format(dialog, sizeof dialog, "¿Estás seguro que quieres borrar el siguiente accesorio?\n\t\"%s\"", PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_NAME]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "{"#SILVER_COLOR"}Accesorio - Eliminar accesorio", dialog, "Eliminar", "Atrás");
			return 1;
		}
		case DIALOG_PLAYER_TOY_SELECT_BONE:
		{
			SendNotification(playerid, "Selecciona en que parte del cuerpo quieres colocar este accesorio.");
			
			new caption[48];
			format(caption, sizeof caption, "{"#SILVER_COLOR"}Accesorio '%s'", PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_NAME]);
			
			new dialog[45 * sizeof Toys_Bones], line_str[45];
			for(new i = 0; i != sizeof Toys_Bones; i ++)
			{
				format(line_str, sizeof(line_str), "{"#BLUE_COLOR"}%d. %s\n", i + 1, Toys_Bones[i]);
				strcat(dialog, line_str);
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_PLAYER_TOY_EDIT_BONE:
		{
			new caption[48];
			format(caption, sizeof caption, "{"#SILVER_COLOR"}Accesorio '%s'", PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_NAME]);
			
			new dialog[145];
			format(dialog, sizeof(dialog), "¿Quieres cambiar la parte del cuerpo donde se coloca este accesorio?\nActualmente está colocado en: %s", Toys_Bones[ PLAYER_TOYS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_TOY_SLOT] ][player_toy_BONE] - 1 ]);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, caption, dialog, "Sí", "No");
			return 1;
		}
		case DIALOG_PLAYER_TOY_COLOR_1:
		{
			SendNotification(playerid, "Selecciona el color 0 para poner el color original del accesorio.");
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Selecciona color", DIALOG_CREATE_CREW_COLOR_String, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_PLAYER_TOY_COLOR_2:
		{
			SendNotification(playerid, "Selecciona el color 0 para poner el color original del accesorio.");
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Selecciona color", DIALOG_CREATE_CREW_COLOR_String, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_PLAYER_TOY_DELETE_ALL: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Accesorios - Eliminar todo", "¿Estás seguro de que quiere eliminar todos sus accesorios?\nEsta opción no se puede deshacer.", "Eliminar", "Atrás");	
		case DIALOG_BANK:
		{
			if(PI[playerid][ac_BANK_ACCOUNT] == 0) return SendNotification(playerid, "No tienes cuenta bancaria.");
			
			new caption[80];
			format(caption, sizeof caption, "Mi cuenta bancaria: {"#BLUE_COLOR"}%s {FFFFFF}- Balance actual: {"#BLUE_COLOR"}%s$", number_format_thousand(PI[playerid][ac_BANK_ACCOUNT]), number_format_thousand(PI[playerid][ac_BANK_MONEY]));
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, 
				"{"#SILVER_COLOR"}1. Retirar dinero\n{"#SILVER_COLOR"}2. Depositar dinero\n{"#SILVER_COLOR"}3. Transferir dinero\n{"#SILVER_COLOR"}4. Últimas transacciones", 
			"Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_CREATE_BANK_ACCOUNT:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Crear cuenta bancaria", 
				"{"#SILVER_COLOR"}Bienvenido, actualmente no tienes una cuenta bancaria.\n\nPara disfrutar de los beneficios de una cuenta bancaria\npuedes crearla ahora mismo con un depósito minimo de 500$.", 
			"Crear", "Cerrar");
			return 1;
		}
		case DIALOG_BANK_WITHDRAW:
		{
			new dialog[100];
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Balance actual: {"#BLUE_COLOR"}%s$\n{"#SILVER_COLOR"}¿Cuánto quiere retirar?", number_format_thousand(PI[playerid][ac_BANK_MONEY]));
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Banco - Retirar", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_BANK_DEPOSIT:
		{
			new dialog[100];
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Dinero actual: {"#BLUE_COLOR"}%s$\n{"#SILVER_COLOR"}¿Cuánto quiere depositar?", number_format_thousand(PI[playerid][ac_CASH]));
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Banco - Depositar", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_BANK_TRANSFER: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Banco - Transferir", "Indica a que número de cuenta quiere transferir dinero.", "Continuar", "Atrás");
		case DIALOG_BANK_TRANSFER_SEND:
		{
			new dialog[170];
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Transferir a cuenta: {"#BLUE_COLOR"}%s\n{"#SILVER_COLOR"}Balance actual: {"#BLUE_COLOR"}%s$\n\n{"#SILVER_COLOR"}¿Cuánto quiere transferir?", number_format_thousand(PLAYER_TEMP[playerid][pt_SELECT_BANK_TRANSFER_ACCOUNT]), number_format_thousand(PI[playerid][ac_BANK_MONEY]));
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Banco - Transferir", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_BANK_TRANSACTIONS:
		{
			inline OnBankMovementsLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new dialog[2675], caption[40], line_str[128];
					format(caption, sizeof caption, "Banco - Últimas %d transacciones", rows);
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Fecha\t{"#BLUE_COLOR"}Concepto\t{"#SILVER_COLOR"}Importe\n");
					if(rows)
					{
						for(new i = 0; i != rows; i ++)
						{
							new date[24], type, from_id, to_id, amount, from_bankid, to_bankid, bool:insull_from_id, bool:isnull_to_id;
							cache_get_value_name(i, "date", date);
							cache_get_value_name_int(i, "type", type);
							cache_is_value_name_null(i, "from_id", insull_from_id);
							if(!insull_from_id) cache_get_value_name_int(i, "from_id", from_id);
							cache_is_value_name_null(i, "to_id", isnull_to_id);
							if(!isnull_to_id) cache_get_value_name_int(i, "to_id", to_id);
							cache_get_value_name_int(i, "amount", amount);
							cache_get_value_name_int(i, "from_bankid", from_bankid);
							cache_get_value_name_int(i, "to_bankid", to_bankid);

							switch(type)
							{
								case BANK_TRANSACTION_WITHDRAW:
								{
									format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}Retirada efectivo\t{"#SILVER_COLOR"}-%s$\n", date, number_format_thousand(amount));
									strcat(dialog, line_str);
								}
								case BANK_TRANSACTION_DEPOSIT:
								{
									format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}Ingreso efectivo\t{"#SILVER_COLOR"}%s$\n", date, number_format_thousand(amount));
									strcat(dialog, line_str);
								}
								case BANK_TRANSACTION_TRANSFER:
								{
									if(from_id == ACCOUNT_INFO[playerid][ac_ID]) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}Transferencia a %d\t{"#SILVER_COLOR"}-%s$\n", date, to_bankid, number_format_thousand(amount));
									else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}Transferencia de %d\t{"#SILVER_COLOR"}%s$\n", date, from_bankid, number_format_thousand(amount));
									strcat(dialog, line_str);
								}
								case BANK_TRANSACTION_CREATE_ACCOUNT:
								{
									format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}Apertura cuenta\t{"#SILVER_COLOR"}-%s$\n", date, number_format_thousand(amount));
									strcat(dialog, line_str);
								}
								case BANK_TRANSACTION_BUY_PROPERTY:
								{
									format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}Compra de propiedad\t{"#SILVER_COLOR"}-%s$\n", date, number_format_thousand(amount));
									strcat(dialog, line_str);
								}
								case BANK_TRANSACTION_SOLD_PROPERTY:
								{
									format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}Venta de propiedad\t{"#SILVER_COLOR"}%s$\n", date, number_format_thousand(amount));
									strcat(dialog, line_str);
								}
								case BANK_TRANSACTION_BUY_VEHICLE:
								{
									format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}Compra de vehículo\t{"#SILVER_COLOR"}-%s$\n", date, number_format_thousand(amount));
									strcat(dialog, line_str);
								}
								case BANK_TRANSACTION_SOLD_VEHICLE:
								{
									format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}Venta de vehículo\t{"#SILVER_COLOR"}%s$\n", date, number_format_thousand(amount));
									strcat(dialog, line_str);
								}
							}
						}
					}
					else strcat(dialog, "{"#SILVER_COLOR"}No hay transacciones en tu cuenta.");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, caption, dialog, "Cerrar", "Atrás");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT bank_movements.*, player.bank_account AS from_bankid, player2.bank_account AS to_bankid FROM bank_movements LEFT JOIN player ON bank_movements.from_id = player.id LEFT JOIN player AS player2 ON bank_movements.to_id = player2.id WHERE bank_movements.from_id = %d OR bank_movements.to_id = %d ORDER BY date DESC LIMIT %d;", ACCOUNT_INFO[playerid][ac_ID], ACCOUNT_INFO[playerid][ac_ID], MAX_BANK_TRANSACTIONS_DIALOG);
			MySQL_TQueryInline(srp_db, using inline OnBankMovementsLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_247_LIST: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "24/7", DIALOG_247_LIST_String, "Continuar", "Cerrar");
		case DIALOG_PLAYER_POCKET:
		{
			new dialog[50 * (MAX_PLAYER_POCKET_OBJECTS + 2)], line_str[50];
			for(new i = 0; i != MAX_PLAYER_POCKET_OBJECTS; i ++)
			{
				if(PLAYER_POCKET[playerid][i][player_pocket_VALID])
				{
					format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\n", i + 1, PLAYER_POCKET[playerid][i][player_pocket_object_NAME]);
					strcat(dialog, line_str);
				}
				else
				{
					format(line_str, sizeof line_str, "{666666}%d. Slot vacío\n", i + 1);
					strcat(dialog, line_str);
				}
			}
			strcat(dialog, "{c4290d}- Eliminar todo\n");
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Alimentos", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_PLAYER_POCKET_OPTIONS:
		{
			new caption[64];
			format(caption, sizeof caption, "Alimento - %s", PLAYER_POCKET[playerid][PLAYER_TEMP[playerid][pt_POCKET_SLOT_SELECTED]][player_pocket_object_NAME]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, "- Consumir\n- Dar\n- Vender\n- Eliminar", "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_PLAYER_WEAPONS_OPTIONS:
		{
			new caption[64];
			format(caption, sizeof caption, "Arma - %s", WEAPON_INFO[ PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_ID] ][weapon_info_NAME]);

			new dialog_body[1024] = "- Dar\n- Vender\n- Eliminar\n";

			if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_NORMAL || ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK) {
				new vehicleid = GetPlayerCameraTargetVehicle(playerid);
				if(vehicleid != INVALID_VEHICLE_ID && PLAYER_VEHICLES[vehicleid][player_vehicle_VALID] && PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID]) {
					strcat(dialog_body, "- Guardar en el maletero");
				}
			}
			else if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_OWN_PROPERTY) {
				new index = GetPropertyIndexByID(PI[playerid][ac_LOCAL_INTERIOR]);

				if(index != -1) {
					new Float:z_pos = PROPERTY_CLOSET_POS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_closet_Z];
					if(PROPERTY_INFO[index][property_DIS_DEFAULT_INTERIOR]) z_pos += PROPERTY_EMPTY_INTERIOR_Z_OFFSET;
					if(PROPERTY_INFO[index][property_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID] && IsPlayerInRangeOfPoint(playerid, NEARS_PLAYERS_DISTANCE, PROPERTY_CLOSET_POS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_closet_X], PROPERTY_CLOSET_POS[ PROPERTY_INFO[index][property_ID_INTERIOR] ][property_closet_Y], z_pos)) {
						strcat(dialog_body, "- Guardar en el armario");
					}
				}
			}

			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog_body, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_PLAYER_POCKET_DELETE_ALL: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Alimentos - Eliminar todo", "¿Estás seguro de que quiere eliminar todos sus alimentos?\nEsta opción no se puede deshacer y perderás todos lo que haya.", "Eliminar", "Atrás");	
		case DIALOG_PHONE:
		{
			new caption[50];
			format(caption, sizeof caption, "Mi teléfono - %d", PI[playerid][ac_PHONE_NUMBER]);
			
			new dialog[200];
			if(PI[playerid][ac_PHONE_STATE] == PHONE_STATE_ON) format(dialog, sizeof dialog, "{"#SILVER_COLOR"}1. Agenda\n{"#SILVER_COLOR"}2. Llamar\n{"#SILVER_COLOR"}3. Enviar mensaje\n{"#SILVER_COLOR"}4. Ver mensajes recibidos\n{"#SILVER_COLOR"}5. Ver mensajes enviados\n{"#SILVER_COLOR"}6. Tienda de compra y venta\n{"#SILVER_COLOR"}7. Apagar teléfono");
			else format(dialog, sizeof dialog, "{"#SILVER_COLOR"}1. Encender teléfono\n{"#SILVER_COLOR"}2. Agenda\n{"#SILVER_COLOR"}3. Llamar\n{"#SILVER_COLOR"}4. Enviar mensaje\n{"#SILVER_COLOR"}5. Ver mensajes recibidos\n{"#SILVER_COLOR"}6. Ver mensajes enviados\n{"#SILVER_COLOR"}7. Tienda de compra y venta");
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_PHONE_BOOK:
		{
			for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
			
			new dialog[80 * (MAX_PHONE_CONTACTS + 3)];
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Nombre\t{"#BLUE_COLOR"}Número\n");

			new line_str[80], contacts, listitem;
			for(new i = 0; i != MAX_PHONE_CONTACTS; i++ )
			{
				if(!PLAYER_PHONE_BOOK[playerid][i][phone_book_contact_VALID]) continue;
				
				format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#BLUE_COLOR"}%d\n", listitem + 1, PLAYER_PHONE_BOOK[playerid][i][phone_book_contact_NAME], PLAYER_PHONE_BOOK[playerid][i][phone_book_contact_PHONE_NUMBER]);
				strcat(dialog, line_str);
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i;
				
				contacts ++; listitem ++;
			}
			if(contacts < MAX_PHONE_CONTACTS)
			{
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = MAX_PHONE_CONTACTS + 10;
				strcat(dialog, "{"#SILVER_COLOR"}+ Añadir contacto\n"); listitem ++;
			}
			if(contacts > 0)
			{
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = MAX_PHONE_CONTACTS + 20;
				strcat(dialog, "{c4290d}- Eliminar todo\n"); listitem ++;
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Mi teléfono - Agenda", dialog, "Continuar", "Atrás");		
			return 1;
		}
		case DIALOG_PHONE_BOOK_ADD_NUMBER: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Agenda - Añadir contacto", "Está añadiendo un nuevo contacto en su agenda.\nIndique el número de teléfono del nuevo contacto.", "Continuar", "Atrás");
		case DIALOG_PHONE_BOOK_ADD_NAME: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Agenda - Añadir contacto", "Está añadiendo un nuevo contacto en su agenda.\nIndique el nombre del nuevo contacto.", "Continuar", "Atrás");
		case DIALOG_PHONE_BOOK_OPTIONS:
		{
			new caption[40];
			format(caption, sizeof caption, "Contacto - %s", PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_NAME]);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, "{"#SILVER_COLOR"}1. Llamar\n{"#SILVER_COLOR"}2. Enviar mensaje\n{"#SILVER_COLOR"}3. Cambiar nombre del contacto\n{"#SILVER_COLOR"}4. Eliminar este contacto", "Continuar", "Atrás");			
			return 1;
		}
		case DIALOG_PHONE_BOOK_CHANGE_NAME:
		{
			new caption[40];
			format(caption, sizeof caption, "Contacto - %s", PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_NAME]);
			
			new dialog[140];
			format(dialog, sizeof dialog, "Nombre actual: %s\nTeléfono: %d\n\nIndica el nuevo nombre de este contacto.", PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_NAME], PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_PHONE_NUMBER]);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, caption, dialog, "Cambiar", "Atrás");	
			return 1;
		}
		case DIALOG_PHONE_BOOK_CONFIRM_DELET:
		{
			new caption[40];
			format(caption, sizeof caption, "Contacto - %s", PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_NAME]);
			
			new dialog[140];
			format(dialog, sizeof dialog, "¿Estás seguro de que desea eliminar el siguiente contacto?\n\nNombre: %s\nTeléfono: %d", PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_NAME], PLAYER_PHONE_BOOK[playerid][ PLAYER_TEMP[playerid][pt_PLAYER_PHONE_BOOK_SELECTED] ][phone_book_contact_PHONE_NUMBER]);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, caption, dialog, "Eliminar", "Atrás");	
			return 1;
		}
		case DIALOG_PHONE_BOOK_DELETE_ALL: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Agenda - Eliminar todo", "¿Estás seguro de que quiere eliminar todos sus contactos?\nEsta opción no se puede deshacer y perderás todos los contactos guardados.", "Eliminar", "Atrás");	
		case DIALOG_PHONE_CALL_NUMBER: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Mi teléfono - Realizar una llamada", "¿A quién quieres llamar?\nIndique el número al que desea llamar.", "Continuar", "Atrás");
		case DIALOG_PHONE_SMS_NUMBER: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Mi teléfono - Enviar mensaje", "¿A quién quieres enviarle el mensaje?\nIndique el número al que desea enviar el mensaje.", "Continuar", "Atrás");
		case DIALOG_PHONE_SMS_MESSAGE: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Mi teléfono - Enviar mensaje", "Escribe el mensaje que quieres enviar\nCaracteres: 1-64.", "Continuar", "Atrás");
		case DIALOG_PHONE_BOOK_SEND_MESSAGE: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Mi teléfono - Enviar mensaje", "Escribe el mensaje que quieres enviar\nCaracteres: 1-64.", "Continuar", "Atrás");
		case DIALOG_PHONE_SENT_MESSAGES:
		{
			inline OnSentMessagesLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new dialog[1600], caption[40], line_str[128];
					format(caption, sizeof caption, "Últimos %d mensajes enviados", rows);
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Fecha\t{"#BLUE_COLOR"}Enviado a\t{"#SILVER_COLOR"}Mensaje\n");
					if(rows)
					{
						for(new i = 0; i != rows; i ++)
						{
							new message[64], date[24], phone_number, bool:isnull_phone_number;
							cache_get_value_name(i, "message", message);
							cache_get_value_name(i, "date", date);
							cache_is_value_name_null(i, "phone_number", isnull_phone_number);
							if(!isnull_phone_number) cache_get_value_name_int(i, "phone_number", phone_number);

							if(!phone_number) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}---\t{"#SILVER_COLOR"}%s\n", date, message);
							else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}%s\t{"#SILVER_COLOR"}%s\n", date, convertPhoneNumber(playerid, phone_number), message);
							strcat(dialog, line_str);
						}
					}
					else strcat(dialog, "{"#SILVER_COLOR"}No hay mensajes.");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, caption, dialog, "Cerrar", "Atrás");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT pmessages.*, player.phone_number FROM pmessages LEFT JOIN player ON pmessages.to_id = player.id WHERE pmessages.from_id = %d ORDER BY pmessages.date DESC LIMIT 10;", ACCOUNT_INFO[playerid][ac_ID]);
			MySQL_TQueryInline(srp_db, using inline OnSentMessagesLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_PHONE_RECEIVED_MESSAGES:
		{
			inline OnReceivedMessagesLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new dialog[1600], caption[40], line_str[128];
					format(caption, sizeof caption, "Últimos %d mensajes recibidos", rows);
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Fecha\t{"#BLUE_COLOR"}Remitente\t{"#SILVER_COLOR"}Mensaje\n");
					if(rows)
					{
						for(new i = 0; i != rows; i ++)
						{
							new message[64], date[24], phone_number, bool:isnull_phone_number;
							cache_get_value_name(i, "message", message);
							cache_get_value_name(i, "date", date);
							cache_is_value_name_null(i, "phone_number", isnull_phone_number);
							if(!isnull_phone_number) cache_get_value_name_int(i, "phone_number", phone_number);

							if(!phone_number) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}---\t{"#SILVER_COLOR"}%s\n", date, message);
							else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}%s\t{"#SILVER_COLOR"}%s\n", date, convertPhoneNumber(playerid, phone_number), message);
							strcat(dialog, line_str);
						}
					}
					else strcat(dialog, "{"#SILVER_COLOR"}No hay mensajes.");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, caption, dialog, "Cerrar", "Atrás");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT pmessages.*, player.phone_number FROM pmessages LEFT JOIN player ON pmessages.from_id = player.id WHERE pmessages.to_id = %d ORDER BY pmessages.date DESC LIMIT 10;", ACCOUNT_INFO[playerid][ac_ID]);
			MySQL_TQueryInline(srp_db, using inline OnReceivedMessagesLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_CONFIRM_BUY_PROPERTY:
		{
			new dialog[220];
			
			if(PROPERTY_INFO[PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX]][property_EXTRA])
			{
				format(dialog, sizeof dialog, "Coste de la propiedad: %d "SERVER_COIN"\nSeguro: NO\nMonedas "SERVER_COIN" actuales: %d "SERVER_COIN"\nMonedas "SERVER_COIN" tras la compra: %d "SERVER_COIN"\n\n¿De verdad desea comprar esta propiedad?", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX] ][property_EXTRA], PI[playerid][ac_COINS], PI[playerid][ac_COINS] - PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX] ][property_EXTRA]);
			}
			else
			{
				format(dialog, sizeof dialog, "Precio de la propiedad: %s$\nSeguro: %s$\nBalance actual: %s$\nBalance tras la compra: %s$\n\n¿De verdad desea comprar esta propiedad?", number_format_thousand(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX] ][property_PRICE]), number_format_thousand(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX] ][property_PRICE_BASE] / 200), number_format_thousand(PI[playerid][ac_BANK_MONEY]), number_format_thousand(PI[playerid][ac_BANK_MONEY] - PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_BUY_HOUSE_INDEX] ][property_PRICE]));
			}
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Comprar propiedad",  dialog, "Comprar", "Cerrar");
			return 1;
		}
		case DIALOG_PLAYER_GPS:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "GPS",  "- Mis lugares\n- Mis propiedades\n- Mis vehículos\n- Lugares de interés\n- Trabajos\n- Ver mapa", "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_PLAYER_GPS_PLAYER:
		{
			for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
			
			new dialog[90 * (MAX_PLAYER_GPS_SAVES + 2)];
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Lugar\t{FF846A}Distancia\n");
			
			new line_str[90], sites, listitem;
			for(new i = 0; i != MAX_PLAYER_GPS_SAVES; i++ )
			{
				if(!PLAYER_GPS[playerid][i][player_gps_VALID]) continue;
				
				format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#BLUE_COLOR"}%.2f Km\n", listitem + 1, PLAYER_GPS[playerid][listitem][player_gps_NAME], (GetPlayerDistanceFromPoint(playerid, PLAYER_GPS[playerid][listitem][player_gps_X], PLAYER_GPS[playerid][listitem][player_gps_Y], PLAYER_GPS[playerid][listitem][player_gps_Z]) * 0.01));
				strcat(dialog, line_str);
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i;
				
				sites ++; listitem ++;
			}
			if(sites < MAX_PLAYER_GPS_SAVES)
			{
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = MAX_PLAYER_GPS_SAVES + 10;
				strcat(dialog, "{"#SILVER_COLOR"}+ Añadir lugar actual\n"); listitem ++;
			}
			if(sites > 0)
			{
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = MAX_PLAYER_GPS_SAVES + 20;
				strcat(dialog, "{c4290d}- Eliminar todo\n"); listitem ++;
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "GPS - Mis lugares", dialog, "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_PLAYER_ADD:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "GPS - Añadir lugar", "Tu posición actual será guardada en este lugar para ir más tarde.\nIndica el nombre de este lugar para guardarlo.", "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_DELETE_ALL: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Mis lugares - Eliminar todo", "¿Estás seguro de que quiere eliminar todos sus lugares?\nEsta opción no se puede deshacer y perderás todos los lugares guardados.", "Eliminar", "Atrás");	
		case DIALOG_PLAYER_GPS_PLAYER_OPTION:
		{
			new caption[40];
			format(caption, sizeof caption, "GPS - %s", PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_NAME]);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, "{"#SILVER_COLOR"}1. Marcar coordenadas en el GPS\n{"#SILVER_COLOR"}2. Cambiar nombre de este lugar\n{"#SILVER_COLOR"}3. Eliminar este lugar del GPS", "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_CHANGE_NAME:
		{
			new dialog[90];
			format(dialog, sizeof dialog, "Nombre del lugar: %s\nIndica el nuevo nombre para este lugar.", PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_NAME]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "GPS - Cambiar nombre de un lugar", dialog, "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_CONFIRM_DELET:
		{
			new dialog[95];
			format(dialog, sizeof dialog, "Nombre del lugar: %s\n¿Estás seguro de que quiere borrarlo del GPS?", PLAYER_GPS[playerid][ PLAYER_TEMP[playerid][pt_GPS_PLAYER_SELECTED] ][player_gps_NAME]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "GPS - Eliminar un lugar", dialog, "Eliminar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITES:
		{
			new dialog[1024];
			format(dialog, sizeof dialog, "- Hospitales\n- Tiendas de Ropa\n- Tiendas 24/7\n- Restaurantes\n- Bancos\n- Cajeros\n- Gasolineras\n- Concesionarios\n- Gobierno\n- Policías\n- "SERVER_SHORT_NAME" Tuning Club\n- Autoescuela\n- Ikea (tienda de muebles)");
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "GPS - Lugares de interés", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_PLAYER_GPS_WORKS:
		{
			for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
			
			new dialog[85 * (sizeof(work_info) + 1)], listitem;
			format(dialog, sizeof dialog, "Trabajo\tNivel\tDistancia\n");

			for(new i = 1; i != sizeof work_info; i ++)
			{
				if(work_info[i][work_info_TYPE] == WORK_TYPE_FAMILY) continue;
				if(!obtain_work_coords[i][obtain_work_AVAILABLE]) continue;
				
				new line_str[85];
				if(i == WORK_MEDIC)
					format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%c%s\t{"#SILVER_COLOR"}%d\t{"#SILVER_COLOR"}-\n", toupper(work_info[i][work_info_NAME][0]), work_info[i][work_info_NAME][1], work_info[i][work_info_LEVEL]);
				else
					format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%c%s\t{"#SILVER_COLOR"}%d\t{"#SILVER_COLOR"}%.2f Km\n", toupper(work_info[i][work_info_NAME][0]), work_info[i][work_info_NAME][1], work_info[i][work_info_LEVEL], (GetPlayerDistanceFromPoint(playerid, obtain_work_coords[i][obtain_work_MAP_ICON_X], obtain_work_coords[i][obtain_work_MAP_ICON_Y], obtain_work_coords[i][obtain_work_MAP_ICON_Z]) * 0.01));
				strcat(dialog, line_str);
				
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i;
				listitem ++;
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "GPS - Trabajos", dialog, "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_WORKS_TELEPORTS:
		{
			for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;

			new dialog[85 * (sizeof(work_info) + 1)], listitem;
			format(dialog, sizeof dialog, "Trabajo\tNivel\n");

			for(new i = 1; i != sizeof work_info; i ++)
			{
				new line_str[85];
				format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%c%s (id: %d)\t{"#SILVER_COLOR"}%d\n", toupper(work_info[i][work_info_NAME][0]), work_info[i][work_info_NAME][1], i, work_info[i][work_info_LEVEL]);
				strcat(dialog, line_str);

				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i;
				listitem ++;
			}
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Trabajos", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_0: //Hospitales
		{
			new dialog[90 * (sizeof Hospital_Spawn_Positions)], listitem;

			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Lugar\t{"#BLUE_COLOR"}Zona");
			for(new i = 0; i != sizeof(Hospital_Spawn_Positions); i++ )
			{
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i;

				new city[45], zone[45];
				GetPointZone(Hospital_Spawn_Positions[i][hspi_X], Hospital_Spawn_Positions[i][hspi_Y], city, zone);
				format(dialog, sizeof dialog, "%s\n{FFFFFF}Hospital %d\t{"#BLUE_COLOR"}%s, %s", dialog, listitem + 1, city, zone);

				listitem++;
			}
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "GPS - Hospitales", dialog, "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_1: //Tiendas de ropa
		{
			for(new i = 0; i != MAX_LISTITEMS; i ++) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;

			new dialog[90 * (MAX_LISTITEMS + 1)];
			format(dialog, sizeof dialog, "Lugar\tDistancia\n");

			new line_str[90], sites;
			for(new i = 0; i != sizeof(ENTER_EXIT); i++ )
			{
				if(sites >= MAX_LISTITEMS) break;
				if(ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_BINCO && ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_SUBURBAN && ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_PROLAPS && ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_DIDIER_SACHS && ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_VICTIM && ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_ZIP) continue;

				format(line_str, sizeof line_str, "{FFFFFF}%d. %s\t{"#SILVER_COLOR"}%.2f Km\n", sites + 1, ENTER_EXIT[i][ee_NAME], (GetPlayerDistanceFromPoint(playerid, ENTER_EXIT[i][ee_EXT_X], ENTER_EXIT[i][ee_EXT_Y], ENTER_EXIT[i][ee_EXT_Z]) * 0.01));
				strcat(dialog, line_str);

				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][sites] = i;
				sites ++;
			}

			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "GPS - Tiendas de ropa", dialog, "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_2: //Tiendas 24 horas
		{
			for(new i = 0; i != MAX_LISTITEMS; i ++) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;

			new dialog[90 * (MAX_LISTITEMS + 1)];
			format(dialog, sizeof dialog, "Lugar\tDistancia\n");

			new line_str[90], sites;
			for(new i = 0; i != sizeof(ENTER_EXIT); i++ )
			{
				if(sites >= MAX_LISTITEMS) break;
				if(ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_247) continue;

				format(line_str, sizeof line_str, "{FFFFFF}%d. %s\t{"#SILVER_COLOR"}%.2f Km\n", sites + 1, ENTER_EXIT[i][ee_NAME], (GetPlayerDistanceFromPoint(playerid, ENTER_EXIT[i][ee_EXT_X], ENTER_EXIT[i][ee_EXT_Y], ENTER_EXIT[i][ee_EXT_Z]) * 0.01));
				strcat(dialog, line_str);

				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][sites] = i;
				sites ++;
			}

			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "GPS - Tiendas 24/7", dialog, "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_3: //Comida rapida
		{
			for(new i = 0; i != MAX_LISTITEMS; i ++) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;

			new dialog[90 * (MAX_LISTITEMS + 1)];
			format(dialog, sizeof dialog, "Lugar\tDistancia\n");

			new line_str[90], sites;
			for(new i = 0; i != sizeof(ENTER_EXIT); i++ )
			{
				if(sites >= MAX_LISTITEMS) break;
				if(ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_PIZZA && ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_CLUCKIN_BELL && ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_BURGER_SHOT) continue;

				format(line_str, sizeof line_str, "{FFFFFF}%d. %s\t{"#SILVER_COLOR"}%.2f Km\n", sites + 1, ENTER_EXIT[i][ee_NAME], (GetPlayerDistanceFromPoint(playerid, ENTER_EXIT[i][ee_EXT_X], ENTER_EXIT[i][ee_EXT_Y], ENTER_EXIT[i][ee_EXT_Z]) * 0.01));
				strcat(dialog, line_str);

				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][sites] = i;
				sites ++;
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "GPS - Restaurantes", dialog, "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_4: //Bancos
		{
			for(new i = 0; i != MAX_LISTITEMS; i ++) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;

			new dialog[90 * (MAX_LISTITEMS + 1)];
			format(dialog, sizeof dialog, "Lugar\tDistancia\n");

			new line_str[90], sites;
			for(new i = 0; i != sizeof(ENTER_EXIT); i++ )
			{
				if(sites >= MAX_LISTITEMS) break;
				if(ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_BANK_LS && ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_BANK_SF && ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_BANK_LV) continue;

				format(line_str, sizeof line_str, "{FFFFFF}%d. %s\t{"#SILVER_COLOR"}%.2f Km\n", sites + 1, ENTER_EXIT[i][ee_NAME], (GetPlayerDistanceFromPoint(playerid, ENTER_EXIT[i][ee_EXT_X], ENTER_EXIT[i][ee_EXT_Y], ENTER_EXIT[i][ee_EXT_Z]) * 0.01));
				strcat(dialog, line_str);

				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][sites] = i;
				sites ++;
			}

			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "GPS - Bancos", dialog, "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_5: //Cajeros
		{
			new dialog[90 * (sizeof(ATM_BANK) + 1)];
			format(dialog, sizeof dialog, "Lugar\tDistancia\n");

			new line_str[90], sites;
			for(new i = 0; i != sizeof(ATM_BANK); i++ )
			{

				format(line_str, sizeof line_str, "{FFFFFF}Cajero %d\t{"#SILVER_COLOR"}%.2f Km\n", sites + 1, (GetPlayerDistanceFromPoint(playerid, ATM_BANK[i][atm_X], ATM_BANK[i][atm_Y], ATM_BANK[i][atm_Z]) * 0.01));
				strcat(dialog, line_str);

				sites ++;
			}

			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "GPS - Cajeros", dialog, "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_6: //Gasolineras
		{
			new dialog[90 * (sizeof(Fuel_Stations) + 1)];
			format(dialog, sizeof dialog, "Lugar\tDistancia\n");

			new line_str[90], sites;
			for(new i = 0; i != sizeof(Fuel_Stations); i++ )
			{
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][sites] = i;
				format(line_str, sizeof line_str, "{FFFFFF}Gasolinera %d\t%.2f Km\n", sites + 1, (GetPlayerDistanceFromPoint(playerid, Fuel_Stations[i][fs_X], Fuel_Stations[i][fs_Y], Fuel_Stations[i][fs_Z]) * 0.01));
				strcat(dialog, line_str);

				sites ++;
			}

			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "GPS - Gasolineras", dialog, "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_7: //Concesionarios
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "GPS - Concesionarios", DIALOG_PLAYER_GPS_SITE_7_String, "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_SITE_8: //Policias
		{
			for(new i = 0; i != MAX_LISTITEMS; i ++) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;

			new dialog[90 * (MAX_LISTITEMS + 1)];
			format(dialog, sizeof dialog, "Lugar\tDistancia\n");

			new line_str[90], sites;
			for(new i = 0; i != sizeof(ENTER_EXIT); i++ )
			{
				if(sites >= MAX_LISTITEMS) break;
				if(ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_POLICE_LS && ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_POLICE_SF && ENTER_EXIT[i][ee_INTERIOR_TYPE] != INTERIOR_POLICE_LV) continue;

				format(line_str, sizeof line_str, "{FFFFFF}%d. %s\t%.2f Km\n", sites + 1, ENTER_EXIT[i][ee_NAME], (GetPlayerDistanceFromPoint(playerid, ENTER_EXIT[i][ee_EXT_X], ENTER_EXIT[i][ee_EXT_Y], ENTER_EXIT[i][ee_EXT_Z]) * 0.01));
				strcat(dialog, line_str);

				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][sites] = i;
				sites ++;
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "GPS - Policías", dialog, "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_GPS_PROPERTIES:
		{
			inline OnDialogQueryLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new dialog[90 * (MAX_SU_PROPERTIES + 2)], line_str[90];
					format(dialog, sizeof dialog, "Propiedad\tDistancia\n");
					for(new i = 0; i != MAX_SU_PROPERTIES; i ++) PLAYER_TEMP[playerid][pt_PLAYER_GPS_SELECTED_PROPERTY][i] = -1;
					if(rows)
					{
						for(new i = 0; i != rows; i ++)
						{
							if(i >= MAX_SU_PROPERTIES) break;
							new property_name[24], Float:pos[3], bool:isnull_name;
							cache_get_value_name_int(i, "id", PLAYER_TEMP[playerid][pt_PLAYER_GPS_SELECTED_PROPERTY][i]);
							cache_is_value_name_null(i, "name", isnull_name);
							if(!isnull_name) cache_get_value_name(i, "name", property_name);
							cache_get_value_name_float(i, "ext_x", pos[0]);
							cache_get_value_name_float(i, "ext_y", pos[1]);
							cache_get_value_name_float(i, "ext_z", pos[2]);
							format(line_str, sizeof line_str, "{FFFFFF}%d. %s\t{"#SILVER_COLOR"}%.2f Km\n", i + 1, property_name, (GetPlayerDistanceFromPoint(playerid, pos[0], pos[1], pos[2]) * 0.01));
							strcat(dialog, line_str);
						}
					}
					else strcat(dialog, "{"#SILVER_COLOR"}No tienes propiedades");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "GPS - Mis propiedades", dialog, "Continuar", "Atrás");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM properties WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
			MySQL_TQueryInline(srp_db, using inline OnDialogQueryLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_PLAYER_GPS_VEHICLES:
		{
			for(new i = 0; i != MAX_LISTITEMS; i ++) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;

			new dialog[115 * (MAX_SU_VEHICLES + 2)], total_vehicles;
			format(dialog, sizeof dialog, "Vehículo\tMatrícula\tDistancia\n");
			for(new i = 0, j = GetVehiclePoolSize(); i <= j; i ++)
			{
				if(!GLOBAL_VEHICLES[i][gb_vehicle_VALID]) continue;
				if(!PLAYER_VEHICLES[i][player_vehicle_VALID]) continue;
				if(total_vehicles > MAX_SU_VEHICLES) break;

				if(PLAYER_VEHICLES[i][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
				{
					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][total_vehicles] = i;

					new Float:pos[3];
					GetVehiclePos(i, pos[0], pos[1], pos[2]);

					new line_str[115];

					if(!PLAYER_VEHICLES[i][player_vehicle_ACCESSIBLE])
						format(line_str, sizeof line_str, "{666666}%d. %s (%d)\t{666666}%s\t{666666}-\n", total_vehicles + 1, VEHICLE_INFO[ GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME], i, GLOBAL_VEHICLES[i][gb_vehicle_NUMBER_PLATE]);
					else
						format(line_str, sizeof line_str, "{FFFFFF}%d. %s\t{FFFFFF}%s\t{"#SILVER_COLOR"}%.2f Km\n", total_vehicles + 1, VEHICLE_INFO[ GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME], GLOBAL_VEHICLES[i][gb_vehicle_NUMBER_PLATE], (GetPlayerDistanceFromPoint(playerid, pos[0], pos[1], pos[2]) * 0.01));

					strcat(dialog, line_str);
					total_vehicles ++;
				}
			}
			if(total_vehicles == 0) strcat(dialog, "{"#SILVER_COLOR"}No tienes vehículos");

			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "GPS - Mis vehículos", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_PROPERTY_OPTIONS:
		{
			new caption[40];
			format(caption, sizeof caption, "Propiedad - %s", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_NAME]);

			new dialog[512];
			format(dialog, sizeof dialog, "1. Cambiar nombre de la propiedad\n2. Echar a todo del mundo de mi propiedad\n3. Poner música desde tu MP3\n4. Muebles\n");

			if(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_DIS_DEFAULT_INTERIOR]) strcat(dialog, "5. Crear muebles por defecto\n");
			else strcat(dialog, "5. Eliminar muebles por defecto\n");

			if(PI[playerid][ac_CREW])
			{
				if(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_ADD_PROPERTIES])
				{
					strcat(dialog, "6. Pasar a propiedad de banda\n");
				}
			}

			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_PROPERTY_NAME:
		{
			new dialog[85];
			format(dialog, sizeof dialog, "Nombre actual: %s\nIndica el nuevo nombre de la propiedad.", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_NAME]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Propiedad - Cambiar nombre", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_PLAYER_MP3: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "MP3 - Buscar una canción", "Indica el nombre y cantante de la canción que quieres reproducir.\nSe recomienda añadir la palabra 'audio' para canción directa.\n\nPor ejemplo: Lynyrd Skynyrd - Free Bird (Audio)", "Buscar", "Cerrar");
		case DIALOG_BUY_VEHICLE:
		{
			if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][gb_vehicle_VALID]) return 0;
			if(GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][gb_vehicle_TYPE] != VEHICLE_TYPE_SELL) return 0;
			if(!SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_VALID]) return 0;
			
			new dialog[800];
			format(dialog, sizeof dialog, 
				"Vehículo: %s\n\
				Nivel necesario: %d\n\
				Precio del vehículo: %s$\n\
				Velocidad máxima: %d Km/h\n\
				Capacidad depósito: %.1f Litros\n\
				Espacio maletero: %d\n\
				Seguro: %s$\n\n", 
				VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME],
				SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_LEVEL],
				number_format_thousand(SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_PRICE]),
				floatround(VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_MAX_VEL]),
				VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_MAX_GAS],
				VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_BOOT_SLOTS],
				number_format_thousand(VEHICLE_INFO[GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][gb_vehicle_MODELID] - 400][vehicle_info_PRICE] / 100)
			);
			
			new str[145];
			if(SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_LEVEL] > ACCOUNT_INFO[playerid][ac_LEVEL])
			{
				format(str, sizeof str, "{"#RED_COLOR"}- Necesitas ser al menos nivel %d para poder comprar este vehículo.", SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_LEVEL]);
				strcat(dialog, str);
				PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] = INVALID_VEHICLE_ID;
				ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Comprar vehículo", dialog, "Cerrar", "");
				return 1;
			}
			
			if(SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_VIP_LEVEL] > ACCOUNT_INFO[playerid][ac_VIP])
			{
				format(str, sizeof str, "{"#RED_COLOR"}- Necesitas membresía VIP (%d) para poder comprar este vehículo.", SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_VIP_LEVEL]);
				strcat(dialog, str);
				PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] = INVALID_VEHICLE_ID;
				ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Comprar vehículo", dialog, "Cerrar", "");
				return 1;
			}
			
			if(PI[playerid][ac_BANK_ACCOUNT] == 0)
			{
				strcat(dialog, "{"#RED_COLOR"}- Necesitas una cuenta bancaria para poder comprar vehículos.");
				PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] = INVALID_VEHICLE_ID;
				ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Comprar vehículo", dialog, "Cerrar", "");
				return 1;
			}
	
			if(SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_PRICE] > PI[playerid][ac_BANK_MONEY])
			{
				new diff = SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_PRICE] - PI[playerid][ac_BANK_MONEY];
				format(str, sizeof str, "{"#RED_COLOR"}- Te faltan %s$ en tu cuenta bancaria para poder comprar este vehículo.", number_format_thousand(diff));
				strcat(dialog, str);
				PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] = INVALID_VEHICLE_ID;
				ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Comprar vehículo", dialog, "Cerrar", "");
				return 1;
			}
			
			if(SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_EXTRA] > PI[playerid][ac_COINS])
			{
				format(str, sizeof str, "{"#RED_COLOR"}- Te faltan %d "SERVER_COIN" para poder comprar este vehículo.", SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_EXTRA] - PI[playerid][ac_COINS]);
				strcat(dialog, str);
				PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] = INVALID_VEHICLE_ID;
				ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Comprar vehículo", dialog, "Cerrar", "");
				return 1;
			}

			if(PI[playerid][ac_DRIVE_LICENSE_POINTS] == 0)
			{
				strcat(dialog, "{"#RED_COLOR"}- No puedes comprar vehículos sin una licencia.");
				PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] = INVALID_VEHICLE_ID;
				ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Comprar vehículo", dialog, "Cerrar", "");
				return 1;
			}
			
			if(!SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_EXTRA]) format(str, sizeof str, "Balance de tu cuenta bancaria: %s$\nBalance tras la compra: %s$\n\n¿De verdad desea comprar este vehículo?", number_format_thousand(PI[playerid][ac_BANK_MONEY]), number_format_thousand(PI[playerid][ac_BANK_MONEY] - SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_PRICE]));
			else format(str, sizeof str, "Tus monedas: %d "SERVER_COIN"\nMonedas tras la compra: %d "SERVER_COIN"\n\n¿De verdad desea comprar este vehículo?", PI[playerid][ac_COINS], PI[playerid][ac_COINS] - SELL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_VEHICLE_ID] ][sell_vehicle_EXTRA]);
			strcat(dialog, str);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Comprar vehículo", dialog, "Comprar", "Cerrar");
			return 1;
		}
		case DIALOG_NOTARY:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Notaría", "{"#SILVER_COLOR"}Vender una propiedad al banco\n{"#SILVER_COLOR"}Vender un vehículo al banco\n{"#SILVER_COLOR"}Vender una propiedad a una persona\n{"#SILVER_COLOR"}Vender un vehículo a una persona", "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_NOTARY_SELECT_PROPERTY:
		{
			inline OnDialogQueryLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new dialog[45 * (MAX_SU_PROPERTIES + 2)], line_str[45];
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Propiedad\n");
					for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
					if(rows)
					{
						for(new i = 0; i != rows; i ++)
						{
							if(i >= MAX_SU_PROPERTIES) break;
							new property_name[24], bool:isnull_name;
							cache_get_value_name_int(i, "id", PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i]);
							cache_is_value_name_null(i, "name", isnull_name);
							if(!isnull_name) cache_get_value_name(i, "name", property_name);
							format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\n", i + 1, property_name);
							strcat(dialog, line_str);
						}
					}
					else strcat(dialog, "{"#SILVER_COLOR"}No tienes propiedades");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Selecciona la propiedad que quieres vender", dialog, "Continuar", "Atrás");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, name FROM properties WHERE id_player = %d;", ACCOUNT_INFO[playerid][ac_ID]);
			MySQL_TQueryInline(srp_db, using inline OnDialogQueryLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_PROPERTY_BANK_SELL:
		{
			new caption[40];
			format(caption, sizeof caption, "Vender - %s", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_NAME]);
			
			new Float:price, payment;
			price = PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_PRICE_BASE];
			if(ACCOUNT_INFO[playerid][ac_VIP]) payment = floatround( floatmul(price, 0.50) );
			else payment = floatround( floatmul(price, 0.25) );
			
			new dialog[160];
			format(dialog, sizeof dialog, "Precio en mercado: %s$\nNosotros te pagaremos: %s$\n\n¿Estás seguro de que quiere venderla?", number_format_thousand(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_PRICE]), number_format_thousand(payment));
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, caption, dialog, "Vender", "Atrás");
			return 1;
		}
		case DIALOG_PROPERTY_SELL_PRICE:
		{
			new caption[40];
			format(caption, sizeof caption, "Vender - %s", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_NAME]);
			
			new Float:price, min_price, max_price;
			price = PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_PRICE];
			min_price = floatround( floatmul(price, 0.15) );
			max_price = floatround( floatmul(price, 6.0) );
			
			new dialog[250];
			format(dialog, sizeof dialog, 
			"Precio en mercado: %s$\n\nPrecio mínimo de venta: %s$\nPrecio máximo de venta: %s$\n\n¿Por cuánto quieres vender esta propiedad?", 
			number_format_thousand(PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_PRICE]),
			number_format_thousand(min_price),
			number_format_thousand(max_price));
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_PROPERTY_SELL_TO_PLAYER:
		{
			new caption[40];
			format(caption, sizeof caption, "Vender - %s", PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_NAME]);
			
			new dialog[250];
			format(dialog, sizeof dialog, "De acuerdo, quieres vender esta propiedad por %s$.\n\nIndica el nombre o playerid del comprador.\nEl comprador debe estar en la sala y cumplir todos los requisitos.", number_format_thousand(PLAYER_TEMP[playerid][pt_NOTARY_PRICE]));
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_SELL_CONFIRM_PROPERTY:
		{
			new dialog[240];
			format(dialog, sizeof dialog,
				"%s te quiere vender una propiedad.\n\n\
				Propiedad: %d\n\
				Precio: %s$\n\n\
				Balance tras la compra: %s$\n\n\
				¿Quieres comprar esta propiedad?",
				PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] ][pt_RP_NAME],
				PROPERTY_INFO[ PLAYER_TEMP[playerid][pt_PLAYER_PROPERTY_SELECTED] ][property_ID],
				number_format_thousand(PLAYER_TEMP[playerid][pt_NOTARY_PRICE]),
				number_format_thousand( PI[playerid][ac_BANK_MONEY] - PLAYER_TEMP[playerid][pt_NOTARY_PRICE] )
			);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Oferta recibida", dialog, "Comprar", "Cerrar");
			return 1;
		}
		case DIALOG_NOTARY_SELECT_VEHICLE:
		{
			for(new i = 0; i != MAX_LISTITEMS; i ++) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
			
			new dialog[95 * (MAX_SU_VEHICLES + 2)], total_vehicles;
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Vehículo\t{"#BLUE_COLOR"}Matrícula\n");
			for(new i = 0, j = GetVehiclePoolSize(); i <= j; i ++)
			{
				if(!GLOBAL_VEHICLES[i][gb_vehicle_VALID]) continue;
				if(!PLAYER_VEHICLES[i][player_vehicle_VALID]) continue;
				if(total_vehicles > MAX_SU_VEHICLES) break;
				
				if(PLAYER_VEHICLES[i][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
				{
					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][total_vehicles] = i;
					
					new line_str[95];
					format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#BLUE_COLOR"}%s\n", total_vehicles + 1, VEHICLE_INFO[ GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME], GLOBAL_VEHICLES[i][gb_vehicle_NUMBER_PLATE]);
					strcat(dialog, line_str);
					total_vehicles ++;
				}
			}
			if(total_vehicles == 0) strcat(dialog, "{"#SILVER_COLOR"}No tienes vehículos");
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Selecciona el vehículo que quieres vender", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_VEHICLE_BANK_SELL:
		{
			new caption[40];
			format(caption, sizeof caption, "Vender - %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			new Float:price, payment;
			price = VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_MODELID] - 400 ][vehicle_info_PRICE];
			if(ACCOUNT_INFO[playerid][ac_VIP]) payment = floatround( floatmul(price, 0.50) );
			else payment = floatround( floatmul(price, 0.25) );
			
			new dialog[160];
			format(dialog, sizeof dialog, "Precio en mercado: %s$\nNosotros te pagaremos: %s$\n\n¿Estás seguro de que quiere venderlo?", number_format_thousand( VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_MODELID] - 400 ][vehicle_info_PRICE] ), number_format_thousand(payment));
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, caption, dialog, "Vender", "Atrás");
			return 1;
		}
		case DIALOG_VEHICLE_SELL_PRICE:
		{
			new caption[40];
			format(caption, sizeof caption, "Vender - %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			new Float:price, min_price, max_price;
			price = VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_MODELID] - 400 ][vehicle_info_PRICE];
			min_price = floatround( floatmul(price, 0.15) );
			max_price = floatround( floatmul(price, 6.0) );
			
			new dialog[250];
			format(dialog, sizeof dialog, 
			"Precio en mercado: %s$\n\nPrecio mínimo de venta: %s$\nPrecio máximo de venta: %s$\n\n¿Por cuánto quieres vender este vehículo?", 
			number_format_thousand(VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_MODELID] - 400 ][vehicle_info_PRICE]),
			number_format_thousand(min_price),
			number_format_thousand(max_price));
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_VEHICLE_SELL_TO_PLAYER:
		{
			new caption[40];
			format(caption, sizeof caption, "Vender - %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			new dialog[250];
			format(dialog, sizeof dialog, "De acuerdo, quieres vender este vehículo por %s$.\n\nIndica el nombre o playerid del comprador.\nEl comprador debe estar en la sala y cumplir todos los requisitos.", number_format_thousand(PLAYER_TEMP[playerid][pt_NOTARY_PRICE]));
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_SELL_CONFIRM_VEHICLE:
		{
			new dialog[300];
			format(dialog, sizeof dialog,
				"%s te quiere vender un vehículo.\n\n\
				Vehículo: %s\n\
				Matrícula: %s\n\
				Precio: %s$\n\n\
				Balance tras la compra: %s$\n\n\
				¿Quieres comprar este vehículo?",
				PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_NOTARY_TO_PLAYER] ][pt_RP_NAME],
				VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME],
				GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_PLAYER_VEHICLE_SELECTED] ][gb_vehicle_NUMBER_PLATE],
				number_format_thousand(PLAYER_TEMP[playerid][pt_NOTARY_PRICE]),
				number_format_thousand( PI[playerid][ac_BANK_MONEY] - PLAYER_TEMP[playerid][pt_NOTARY_PRICE] )
			);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Oferta recibida", dialog, "Comprar", "Cerrar");
			return 1;
		}
		case DIALOG_SELECT_TRUCK_TYPE:
		{
			new work_extra_payment;
			if(work_info[WORK_TRUCK][work_info_EXTRA_PAY] > 0 && work_info[WORK_TRUCK][work_info_EXTRA_PAY_EXP] > 0)
			{
				work_extra_payment = (work_info[WORK_TRUCK][work_info_EXTRA_PAY] * floatround(floatdiv(PLAYER_WORKS[playerid][WORK_TRUCK][pwork_LEVEL], work_info[WORK_TRUCK][work_info_EXTRA_PAY_EXP])));
				if(work_info[WORK_TRUCK][work_info_EXTRA_PAY_LIMIT] != 0) if(work_extra_payment > work_info[WORK_TRUCK][work_info_EXTRA_PAY_LIMIT]) work_extra_payment = work_info[WORK_TRUCK][work_info_EXTRA_PAY_LIMIT];
				
				if(ACCOUNT_INFO[playerid][ac_VIP]) work_extra_payment += SU_WORK_EXTRA_PAY;
			}
			
			new dialog[105 * (sizeof(Truck_Contents) + 2)], listitem;
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Producto\t{"#BLUE_COLOR"}Beneficios\t{"#SILVER_COLOR"}Distancia\n");
			for(new i = 0; i != sizeof Truck_Contents; i ++)
			{
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i;

				new line_str[105];
				if(Truck_Contents[i][truck_content_EXP] > PLAYER_WORKS[playerid][WORK_TRUCK][pwork_LEVEL])
				{
					format(line_str, sizeof line_str, "{666666}%s\t{666666}%s$\t{666666}%.2f Km\n", Truck_Contents[i][truck_content_NAME], number_format_thousand(Truck_Contents[i][truck_content_MONEY] + work_extra_payment), (GetPlayerDistanceFromPoint(playerid, Truck_Contents[i][truck_content_X], Truck_Contents[i][truck_content_Y], Truck_Contents[i][truck_content_Z]) * 0.01));
					strcat(dialog, line_str);
				}
				else
				{
					format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}%s$\t{"#SILVER_COLOR"}%.2f Km\n", Truck_Contents[i][truck_content_NAME], number_format_thousand(Truck_Contents[i][truck_content_MONEY] + work_extra_payment), (GetPlayerDistanceFromPoint(playerid, Truck_Contents[i][truck_content_X], Truck_Contents[i][truck_content_Y], Truck_Contents[i][truck_content_Z]) * 0.01));
					strcat(dialog, line_str);
				}

				listitem++;
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Selecciona la carga", dialog, "Continuar", "Cancelar");
			return 1;
		}
		case DIALOG_TUNING_MENU_PART:
		{
			inline OnComponentsInfoLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new caption[45], dialog[80 * TOTAL_TUNING_PARTS], line_str[80];
					format(caption, sizeof caption, "Tuning - %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Categoría\n");
					if(rows)
					{
						for(new i = 0; i != rows; i ++)
						{
							if(i >= TOTAL_TUNING_PARTS)
							{
								printf("---> Límite superado en array 'PLAYER_TUNING_MENU, dialog Parts' al intentar cargar de la base de datos.");
								break;
							}

							cache_get_value_index(i, 0, PLAYER_TUNING_MENU[playerid][i][tuning_menu_NAME]);
							format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\n", PLAYER_TUNING_MENU[playerid][i][tuning_menu_NAME]);
							strcat(dialog, line_str);
						}
					}
					else strcat(dialog, "{666666}Este vehículo no se puede tunear");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, caption, dialog, "Continuar", "Atrás");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT vcomponents_info.part FROM vcomponents_info INNER JOIN vcomponents ON vcomponents_info.id = vcomponents.componentid WHERE vcomponents.modelid = %d GROUP BY vcomponents_info.part;", GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID]);
			MySQL_TQueryInline(srp_db, using inline OnComponentsInfoLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_TUNING_MENU_COMPONENT:
		{
			inline OnComponentsInfoLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new caption[45], dialog[75 * MAX_TUNING_PARTS_COMPONENTS], line_str[75];
					format(caption, sizeof caption, "%s > %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME], PLAYER_TEMP[playerid][pt_TUNING_SELECTED_PART]);
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Categoría\t{"#BLUE_COLOR"}Piezas necesarias\t{"#SILVER_COLOR"}Estimación\n");
					if(rows)
					{
						for(new i = 0; i != rows; i ++)
						{
							if(i >= MAX_TUNING_PARTS_COMPONENTS)
							{
								printf("---> Límite superado en array 'PLAYER_TUNING_MENU, dialog Partsnames' al intentar cargar de la base de datos.");
								break;
							}

							cache_get_value_name_int(i, "id", PLAYER_TUNING_MENU[playerid][i][tuning_menu_ID]);
							cache_get_value_name(i, "name", PLAYER_TUNING_MENU[playerid][i][tuning_menu_NAME]);
							cache_get_value_name_int(i, "pieces", PLAYER_TUNING_MENU[playerid][i][tuning_menu_PIECES]);
							format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}%d\t{"#SILVER_COLOR"}%s$\n", PLAYER_TUNING_MENU[playerid][i][tuning_menu_NAME], PLAYER_TUNING_MENU[playerid][i][tuning_menu_PIECES], number_format_thousand(PLAYER_TUNING_MENU[playerid][i][tuning_menu_PIECES] * 50));
							strcat(dialog, line_str);
						}
					}
					else strcat(dialog, "{666666}No se puede tunear");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, caption, dialog, "Continuar", "Atrás");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT vcomponents_info.id, vcomponents_info.name, vcomponents_info.pieces FROM vcomponents_info INNER JOIN vcomponents ON vcomponents_info.id = vcomponents.componentid WHERE vcomponents_info.part = '%e' AND vcomponents.modelid = %d;", PLAYER_TEMP[playerid][pt_TUNING_SELECTED_PART], GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID]);
			MySQL_TQueryInline(srp_db, using inline OnComponentsInfoLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_GRAFFITI_EDIT: {
			ShowPlayerDialog(playerid, DIALOG_GRAFFITI_EDIT, DIALOG_STYLE_LIST, "Graffiti", "1. Cambiar posición\n2. Editar texto\n3. Borrar", "Continuar", "Cerrar");
		}
		case DIALOG_MECHANIC_MENU:
		{
			new caption[60];
			format(caption, sizeof caption, "%s piezas disponibles - %s", number_format_thousand(PI[playerid][ac_MECHANIC_PIECES]), VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			new dialog[450];
			format(dialog, sizeof dialog, 
			"\
				{"#SILVER_COLOR"}Opción\t{"#BLUE_COLOR"}Piezas necesarias\t{"#SILVER_COLOR"}Estimación\n\
				{"#SILVER_COLOR"}Reparación completa\t{"#BLUE_COLOR"}10\t{"#SILVER_COLOR"}500$\n\
			");
			
			for(new i; i != sizeof Mechanic_Areas; i++)
				if(IsPlayerInDynamicArea(playerid, Mechanic_Areas[i]))
				{
					new paintjobs = VehiclePaintjob( GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] );
					strcat(dialog, "{"#SILVER_COLOR"}Pintar vehículo\t{"#BLUE_COLOR"}5\t{"#SILVER_COLOR"}250$\n");
					if(PLAYER_WORKS[playerid][WORK_MECHANIC][pwork_LEVEL] >= 100)
					{
						strcat(dialog, "{"#SILVER_COLOR"}Eliminar componentes\n");
						strcat(dialog, "{"#SILVER_COLOR"}Tunear vehículo\n");
						if(paintjobs > 0) strcat(dialog, "{"#SILVER_COLOR"}Paintjob\t{"#BLUE_COLOR"}15\t{"#SILVER_COLOR"}750$\n");
					}
					else
					{
						strcat(dialog, "{666666}Eliminar componentes\n");
						strcat(dialog, "{666666}Tunear vehículo\n");
						if(paintjobs > 0) strcat(dialog, "{666666}Paintjob\t{666666}15\t{666666}750$\n");
					}
					break;
				}

			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_MECHANIC_REPAIR_PRICE:
		{
			new caption[60];
			format(caption, sizeof caption, "Reparar %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, caption, "Indica el precio de la reparación.\nRango de precio: 0$ - 2.000$", "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_MECHANIC_COLOR_PRICE:
		{
			new caption[60];
			format(caption, sizeof caption, "Pintar %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, caption, "Indica el precio para pintar el vehículo.\nRango de precio: 0$ - 1.750$", "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_MECHANIC_TUNING_PRICE:
		{
			new caption[60], dialog[145];
			format(caption, sizeof caption, "Tunear %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			format(dialog, sizeof dialog, "Indica el precio para tunear el vehículo.\nRango de precio: 0$ - %s$", number_format_thousand((PLAYER_TEMP[playerid][pt_TUNING_SELECTED_PIECES] * 50) + 2000));
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_MECHANIC_PAINTJOB_PRICE:
		{
			new caption[60];
			format(caption, sizeof caption, "Paintjob %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			if(PLAYER_TEMP[playerid][pt_PAINTJOB_SELECTED_ID] == 3)
				ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, caption, "Indica el precio para eliminar el paintjob del vehículo.\nRango de precio: 0$ - 2.750$", "Continuar", "Atrás");
			else
				ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, caption, "Indica el precio para el paintjob del vehículo.\nRango de precio: 0$ - 2.750$", "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_MECHANIC_REM_COM_PRICE:
		{
			new caption[60];
			format(caption, sizeof caption, "Eliminar componente de %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, caption, "Indica el precio para eliminar el componente del vehículo.\nRango de precio: 0$ - 2.750$", "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_MECHANIC_ACCEPT:
		{
			new dialog[300];
			format(dialog, sizeof dialog,
				
				"\
					{"#SILVER_COLOR"}El mecánico %s te ofrece:\n\
						\t%s\n\
					\n\
					{"#SILVER_COLOR"}Precio: %s$\n\
					¿Quieres aceptarlo?\n\
				",
					PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_MECHANIC_PID] ][pt_RP_NAME],
					PLAYER_TEMP[playerid][pt_MECHANIC_TEXT],
					number_format_thousand(PLAYER_TEMP[playerid][pt_MECHANIC_PRICE])
			);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Mecánico", dialog, "Aceptar", "Cancelar");
			return 1;
		}
		case DIALOG_MECHANIC_SELECT_COL_SLOT:
		{
			new caption[60];
			format(caption, sizeof caption, "Pintar %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, "{"#SILVER_COLOR"}Pintar color primario\n{"#SILVER_COLOR"}Pintar color secundario", "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_MECHANIC_SELECT_COLOR, DIALOG_PLANE_SELECT_COLOR, DIALOG_BOAT_SELECT_COLOR:
		{
			new caption[60];
			format(caption, sizeof caption, "Pintar %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			
			new dialog[15 * (sizeof(VEHICLE_COLORS) + 1) ];
			for(new i = 0; i != sizeof VEHICLE_COLORS; i++)
			{
				new line_str[15];
				format(line_str, sizeof(line_str), "{%06x}%d\n", VEHICLE_COLORS[i] >>> 8, i);
				strcat(dialog, line_str);
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_MECHANIC_REMOVE_COMPONEN:
		{
			inline OnComponentsInfoLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new caption[60], dialog[80 * (MAX_VEHICLE_COMPONENTS + 2) ], count, line_str[80];
					format(caption, sizeof caption, "Componentes de %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Componente\t{"#BLUE_COLOR"}Piezas\n");
					if(rows)
					{
						for(new i = 0; i != rows; i ++)
						{
							new id, part[24], name[24];
							cache_get_value_name_int(i, "id", id);
							PLAYER_TUNING_MENU[playerid][count][tuning_menu_ID] = GetVehicleComponentIndexFromId(PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID], id);
							if(PLAYER_TUNING_MENU[playerid][count][tuning_menu_ID] != -1) {
								cache_get_value_name(i, "part", part);
								cache_get_value_name(i, "name", name);
								cache_get_value_name_int(i, "pieces", PLAYER_TUNING_MENU[playerid][count][tuning_menu_PIECES]);
								format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s > %s\t{"#BLUE_COLOR"}+ %d\n", part, name, PLAYER_TUNING_MENU[playerid][count][tuning_menu_PIECES]);
								strcat(dialog, line_str);
								count++;
							}
							else strcat(dialog, "{"#SILVER_COLOR"}Vehículo sin tunear"); 
						}
					}
					else strcat(dialog, "{"#SILVER_COLOR"}Vehículo sin tunear");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, caption, dialog, "Eliminar", "Atrás");
				}
			}

			new components_query[1024], count;
			format(components_query, sizeof components_query, "SELECT * FROM vcomponents_info WHERE");
			for(new i = 0; i != MAX_VEHICLE_COMPONENTS; i ++)
			{
				PLAYER_TUNING_MENU[playerid][i][tuning_menu_ID] = -1;
				if(!GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_COMPONENTS][i]) continue;
				if(count == 0) format(components_query, sizeof components_query, "%s id = %d", components_query, GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_COMPONENTS][i]);
				else format(components_query, sizeof components_query, "%s OR id = %d", components_query, GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_COMPONENTS][i]);
				count ++;
			}
			if(!count) SendNotification(playerid, "Vehículo sin tunear.");
			else MySQL_TQueryInline(srp_db, using inline OnComponentsInfoLoad, components_query);
			return 1;
		}
		case DIALOG_TUNING_PAINTJOB:
		{
			new caption[60], dialog[25 * 4];
			format(caption, sizeof caption, "Paintjob - %s", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			new paintjobs = VehiclePaintjob( GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_SELECTED_MECHANIC_VEHICLE_ID] ][gb_vehicle_MODELID] );
			if(paintjobs == 0)
			{
				SendNotification(playerid, "Este vehículo no soporta paintjobs.");
				ShowDialog(playerid, DIALOG_MECHANIC_MENU);
				return 1;
			}
			
			strcat(dialog, "{"#SILVER_COLOR"}Eliminar paintjob (+ 15 piezas)\n");
			for(new i = 0; i != paintjobs; i ++)
			{
				new line_str[25];
				format(line_str, sizeof line_str, "{"#SILVER_COLOR"}Paintjob %d\n", i + 1);
				strcat(dialog, line_str);
			}
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_SEED_LIST:
		{
			new dialog[64 * (sizeof(seed_info) + 1)];
			format(dialog, sizeof dialog, "Nombre\tPrecio por semilla\n");
			
			for(new i = 0; i != sizeof seed_info; i ++)
			{
				new line_str[64];
				if(PLAYER_WORKS[playerid][WORK_FARMER][pwork_LEVEL] >= seed_info[i][seed_info_EXP])
				{
					if(seed_info[i][seed_info_TYPE] == TYPE_LEGAL) format(line_str, sizeof line_str, "{"#BLUE_COLOR"}%s\t{"#SILVER_COLOR"}%d$\n", seed_info[i][seed_info_NAME], seed_info[i][seed_info_PRICE]);
					else format(line_str, sizeof line_str, "{"#RED_COLOR"}%s\t{"#SILVER_COLOR"}%d$\n", seed_info[i][seed_info_NAME], seed_info[i][seed_info_PRICE]);
				}
				else format(line_str, sizeof line_str, "{666666}%s\t{666666}%d$\n", seed_info[i][seed_info_NAME], seed_info[i][seed_info_PRICE]);
				
				strcat(dialog, line_str);
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Semillas", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_SEED_BUY:
		{
			new dialog[145];
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Vale, quieres comprar semillas de %s.\n{"#SILVER_COLOR"}El precio por semilla es de: %d$\n\n{"#SILVER_COLOR"}¿Cuántas semillas quieres comprar?", seed_info[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_SEED_ID] ][seed_info_NAME], seed_info[ PLAYER_TEMP[playerid][pt_SELECTED_BUY_SEED_ID] ][seed_info_PRICE]);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Semillas", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_PLANT_PLANTS:
		{
			new dialog[95 * (sizeof(seed_info) + 1)];
			format(dialog, sizeof dialog, "Planta\tSemilla requerida\tSemillas necesarias\n");
			
			for(new i = 0; i != sizeof plant_info; i ++)
			{
				new line_str[95];
				
				if(seed_info[i][seed_info_TYPE] == TYPE_LEGAL) format(line_str, sizeof line_str, "{"#BLUE_COLOR"}%s\t{"#SILVER_COLOR"}%s\t{"#SILVER_COLOR"}%d\n", plant_info[i][plant_info_NAME], seed_info[i][seed_info_NAME], plant_info[i][plant_info_SEEDS]);
				else format(line_str, sizeof line_str, "{"#RED_COLOR"}%s\t{"#SILVER_COLOR"}%s\t{"#SILVER_COLOR"}%d\n", plant_info[i][plant_info_NAME], seed_info[i][seed_info_NAME], plant_info[i][plant_info_SEEDS]);

				strcat(dialog, line_str);
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Plantar", dialog, "Continuar", "Cerrar");
		}
		case DIALOG_BLACK_MARKET:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Mercado negro", "{"#SILVER_COLOR"}Comprar armas\n{"#SILVER_COLOR"}Comprar munición para mis armas", "Comprar", "Cerrar");
			return 1;
		}
		case DIALOG_BLACK_MARKET_WEAPONS:
		{
			new dialog[64 * (sizeof(BLACK_MARKT_WEAPONS) + 2)];
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Producto\t{"#SILVER_COLOR"}Precio\n");
			
			for(new i = 0; i != sizeof BLACK_MARKT_WEAPONS; i ++)
			{
				new line_str[64];
				if(BLACK_MARKT_WEAPONS[i][black_market_EXP] > PI[playerid][ac_BLACK_MARKET_LEVEL])
				{
					format(line_str, sizeof line_str, "{666666}%s\t{666666}%s$\n", WEAPON_INFO[ BLACK_MARKT_WEAPONS[i][black_market_WEAPON_ID] ][weapon_info_NAME], number_format_thousand(BLACK_MARKT_WEAPONS[i][black_market_WEAPON_PRICE]));
					strcat(dialog, line_str);
				}
				else
				{
					format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#SILVER_COLOR"}%s$\n", WEAPON_INFO[ BLACK_MARKT_WEAPONS[i][black_market_WEAPON_ID] ][weapon_info_NAME], number_format_thousand(BLACK_MARKT_WEAPONS[i][black_market_WEAPON_PRICE]));
					strcat(dialog, line_str);
				}
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Armas", dialog, "Comprar", "Cerrar");
			return 1;
		}
		case DIALOG_BLACK_MARKET_SELECT_WEA:
		{
			for(new i = 0; i != MAX_LISTITEMS; i ++) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
			
			new dialog[95 * 15], line_str[95], listitem;
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Arma\t{"#BLUE_COLOR"}Munición\t{"#SILVER_COLOR"}Slot\n");
			
			for(new i = 0; i < sizeof PLAYER_WEAPONS[]; i ++)
			{
				if(!PLAYER_WEAPONS[playerid][i][player_weapon_VALID]) continue;
				
				format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}%s\t{"#SILVER_COLOR"}%d\n", WEAPON_INFO[ PLAYER_WEAPONS[playerid][i][player_weapon_ID] ][weapon_info_NAME], number_format_thousand(PLAYER_WEAPONS[playerid][i][player_weapon_AMMO]), i);
				strcat(dialog, line_str);
				
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i; 
				listitem ++;
			}
			if(listitem == 0) strcat(dialog, "{666666}No tienes armas");
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Selecciona la arma a la que quieres comprarle munición", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_BLACK_MARKET_AMMO:
		{
			new dialog[145];
			format(dialog, sizeof dialog, "Arma: %s\nMunición: %s\n\nEl precio por bala es de 3$\n¿Cuánto munición quiere comprar?", WEAPON_INFO[ PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_ID] ][weapon_info_NAME], number_format_thousand(PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_AMMO]));
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Comprar munición", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_PLAYER_WEAPONS:
		{
			for(new i = 0; i != MAX_LISTITEMS; i ++) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
			
			new dialog[95 * 15], line_str[95], listitem;
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Arma\t{"#BLUE_COLOR"}Munición\t{"#SILVER_COLOR"}Slot\n");
			
			for(new i = 0; i < sizeof PLAYER_WEAPONS[]; i ++)
			{
				if(!PLAYER_WEAPONS[playerid][i][player_weapon_VALID]) continue;
				
				format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}%s\t{"#SILVER_COLOR"}%d\n", WEAPON_INFO[ PLAYER_WEAPONS[playerid][i][player_weapon_ID] ][weapon_info_NAME], number_format_thousand(PLAYER_WEAPONS[playerid][i][player_weapon_AMMO]), i);
				strcat(dialog, line_str);
				
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i; 
				listitem ++;
			}
			strcat(dialog, "{c4290d}- Eliminar todo\n");
			PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = 13 + 20;
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Mis armas", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_PLAYER_WEAPONS_DELETE_A:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Mis armas - Eliminar todo", "¿Estás seguro de que quiere eliminar todas sus armas?\nEsta opción no se puede deshacer.", "Eliminar", "Atrás");	
			return 1;
		}
		case DIALOG_PLAYER_WEAPONS_DELETE:
		{
			new dialog[150];
			format(dialog, sizeof dialog, "Arma: %s\nMunición: %s\n\n¿Estás seguro de que quiere eliminar esta arma?\nEsta opción no se puede deshacer.", WEAPON_INFO[ PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_ID] ][weapon_info_NAME], number_format_thousand(PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_AMMO]));
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Mis armas - Eliminar arma", dialog, "Eliminar", "Cerrar");	
			return 1;
		}
		case DIALOG_ANIMS: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Animaciones", DIALOG_ANIMS_String, "Continuar", "X");
		case DIALOG_TRICKS_FOOD:
		{	
			new dialog[150];
			format(dialog, sizeof dialog, "%s te quiere vender algo.\n\nTipo: alimento\nNombre: %s\nPrecio: %s$\n\n¿Quieres comprárselo?",
									PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_RP_NAME],
									PLAYER_POCKET[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA] ][player_pocket_object_NAME],
									number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE])
								);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Ventas", dialog, "Aceptar", "Cancelar");	
			return 1;
		}
		case DIALOG_TRICKS_MEDICINE:
		{	
			new dialog[150];
			format(dialog, sizeof dialog, "%s te quiere vender algo.\n\nTipo: medicamento\nCantidad: %dg\nPrecio: %s$\n\n¿Quieres comprárselo?",
									PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_RP_NAME],
									PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA],
									number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE])
								);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Ventas", dialog, "Aceptar", "Cancelar");	
			return 1;
		}
		case DIALOG_TRICKS_CANNABIS:
		{	
			new dialog[150];
			format(dialog, sizeof dialog, "%s te quiere vender algo.\n\nTipo: marihuana\nCantidad: %dg\nPrecio: %s$\n\n¿Quieres comprárselo?",
									PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_RP_NAME],
									PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA],
									number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE])
								);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Ventas", dialog, "Aceptar", "Cancelar");	
			return 1;
		}
		case DIALOG_TRICKS_CRACK:
		{	
			new dialog[150];
			format(dialog, sizeof dialog, "%s te quiere vender algo.\n\nTipo: crack\nCantidad: %dg\nPrecio: %s$\n\n¿Quieres comprárselo?",
									PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_RP_NAME],
									PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA],
									number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE])
								);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Ventas", dialog, "Aceptar", "Cancelar");	
			return 1;
		}
		case DIALOG_TRICKS_SU:
		{	
			new dialog[150];
			format(dialog, sizeof dialog, "%s te quiere vender %d "SERVER_COIN"\nPrecio: %s$\n\n¿Quieres comprárselo?",
									PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_RP_NAME],
									PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA],
									number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE])
								);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Ventas", dialog, "Aceptar", "Cancelar");	
			return 1;
		}
		case DIALOG_TRICKS_WEAPON:
		{	
			new dialog[180];
			format(dialog, sizeof dialog, "%s te quiere vender algo.\n\nTipo: arma\nNombre: %s\nMunición: %s\nPrecio: %s$\n\n¿Quieres comprárselo?",
									PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_RP_NAME],
									WEAPON_INFO[ PLAYER_WEAPONS[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA] ][player_weapon_ID] ][weapon_info_NAME],
									number_format_thousand(PLAYER_WEAPONS[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][ PLAYER_TEMP[playerid][pt_TRICK_SELLER_EXTRA] ][player_weapon_AMMO]),
									number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE])
								);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Ventas", dialog, "Aceptar", "Cancelar");	
			return 1;
		}
		case DIALOG_VEHICLE_BOOT:
		{
			for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
	
			new max_slots = VEHICLE_INFO[GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][gb_vehicle_MODELID] - 400][vehicle_info_BOOT_SLOTS];
			if(max_slots > MAX_BOOT_SLOTS) max_slots = MAX_BOOT_SLOTS;
			
			new dialog[128 * (MAX_BOOT_SLOTS + 2)], line_str[128], listitem;
			format(dialog, sizeof dialog, "Tipo\tDescripción\n");
			
			for(new i = 0; i != max_slots; i ++)
			{
				if(VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][i][vehicle_boot_VALID])
				{
					switch(VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][i][vehicle_boot_TYPE])
					{
						case BOOT_TYPE_WEAPON:
						{
							format(line_str, sizeof line_str, "%s\t%s (%s balas)\n", GetBootTypeName(VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][i][vehicle_boot_TYPE]), WEAPON_INFO[ VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][i][vehicle_boot_INT] ][weapon_info_NAME], number_format_thousand(VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][i][vehicle_boot_INT_EXTRA]));
							strcat(dialog, line_str);
						}
						case BOOT_TYPE_MEDICINES, BOOT_TYPE_CANNABIS, BOOT_TYPE_CRACK:
						{
							format(line_str, sizeof line_str, "%s\t%s gramos\n", GetBootTypeName(VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][i][vehicle_boot_TYPE]), number_format_thousand(VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][i][vehicle_boot_INT]));
							strcat(dialog, line_str);
						}
					}
					
					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i;
					listitem ++;
				}
			}
			
			new caption[64];
			format(caption, sizeof caption, "Maletero de %s (%d espacios disponibles)", VEHICLE_INFO[GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][gb_vehicle_MODELID] - 400][vehicle_info_NAME], max_slots - listitem);
			
			PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = MAX_BOOT_SLOTS + 20;
			strcat(dialog, "{c4290d}- Eliminar todo\n"); listitem ++;
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, caption, dialog, "Continuar", "Cerrar");
		}
		case DIALOG_VEHICLE_BOOT_DELETE_ALL: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Maletero - Eliminar todo", "¿Estás seguro de que quiere eliminar todo de su maletero?\nEsta opción no se puede deshacer.", "Eliminar", "Cancelar");	
		case DIALOG_VEHICLE_BOOT_OPTIONS:
		{	
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Opciones", "{"#SILVER_COLOR"}1. Sacar\n{"#SILVER_COLOR"}2. Eliminar", "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_VEHICLE_BOOT_DELETE:
		{
			new dialog[190];

			switch(VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_TYPE])
			{
				case BOOT_TYPE_WEAPON: format(dialog, sizeof dialog, "Tipo: %s\nDescripción: %s (%s balas)\n\n¿Estás seguro de que quiere eliminar esto de tu maletero?\nEsta opción no se puede deshacer.", GetBootTypeName(VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_TYPE]), WEAPON_INFO[ VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_INT] ][weapon_info_NAME], number_format_thousand( VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_INT_EXTRA] ));
				case BOOT_TYPE_MEDICINES, BOOT_TYPE_CANNABIS, BOOT_TYPE_CRACK: format(dialog, sizeof dialog, "Tipo: %s\nDescripción: %s gramos\n\n¿Estás seguro de que quiere eliminar esto de tu maletero?\nEsta opción no se puede deshacer.", GetBootTypeName(VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_TYPE]), number_format_thousand( VEHICLE_BOOT[ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE] ][ PLAYER_TEMP[playerid][pt_DIALOG_BOT_VEHICLE_BOOT_SLOT] ][vehicle_boot_INT] ));
			}			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Maletero - Eliminar", dialog, "Eliminar", "Atrás");
			return 1;
		}
		case DIALOG_SELECT_POLICE_SKIN:
		{
			for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
			
			new dialog[60 * sizeof(POLICE_SKINS)], line_str[60], listitem;
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Ropa\t{"#SILVER_COLOR"}Rango necesario\n");
			for(new i = 0; i != sizeof POLICE_SKINS; i ++)
			{
				if(POLICE_SKINS[i][police_skin_TYPE] != PI[playerid][ac_GENDER]) continue;
				
				if(POLICE_SKINS[i][police_skin_RANK] > PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL]) format(line_str, sizeof line_str, "{666666}%d\t{666666}%s\n", POLICE_SKINS[i][police_skin_SKIN], POLICE_RANKS[ POLICE_SKINS[i][police_skin_RANK] ]);
				else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d\t{"#SILVER_COLOR"}%s\n", POLICE_SKINS[i][police_skin_SKIN], POLICE_RANKS[ POLICE_SKINS[i][police_skin_RANK] ]);
				strcat(dialog, line_str);
				
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i;
				listitem ++;
			}
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Selecciona la ropa", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_POLICE_RADIOS:
		{
			new dialog[64 * 6];
			for(new i = 0; i != 6; i ++)
			{
				new players = CountPoliceRadioPlayers(i);
				
				if(PLAYER_TEMP[playerid][pt_POLICE_RADIO] == i) strcat(dialog, "{"#SILVER_COLOR"}");
				else strcat(dialog, "{666666}");
				format(dialog, sizeof dialog, "%sFrecuencia %d:\t%d personas\n", dialog, i, players);
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST, "Selecciona la frecuencia", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_POLICE_LIST:
		{
			inline OnDialogQueryLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new dialog[128 * 15], line_str[128], count, start_pos = PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] + 1;
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Nombre\t{"#SILVER_COLOR"}Rango\t{"#SILVER_COLOR"}Últ. conexión\n");
					for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
					for(new i = 0; i != rows; i ++)
					{
						new level, id, name[24], last_connection[24], connected;
						cache_get_value_name_int(i, "level", level);
						cache_get_value_name_int(i, "id", id);
						cache_get_value_name(i, "name", name);
						cache_get_value_name(i, "last_connection", last_connection);
						cache_get_value_name_int(i, "connected", connected);

						if(connected) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#SILVER_COLOR"}%s\t{"#GREEN_COLOR"}Conectado\n", start_pos + i, name, POLICE_RANKS[level]);
						else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#SILVER_COLOR"}%s\t{"#SILVER_COLOR"}%s\n", start_pos + i, name, POLICE_RANKS[level], last_connection);
						strcat(dialog, line_str);
						PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = id;
					}

					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -2;
					strcat(dialog, "{c9c9c9}- Siguiente\n");
					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -3;
					strcat(dialog, "{c9c9c9}- Anterior\n");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "policías", dialog, "Continuar", "Cerrar");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT pworks.level, player.id, player.name, player.last_connection, player.connected FROM pworks INNER JOIN player ON pworks.id_player = player.id WHERE pworks.id_work = %d AND pworks.`set` = 1 ORDER BY player.connected DESC, pworks.level DESC LIMIT %d, %d;", WORK_POLICE, PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT], PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);
			MySQL_TQueryInline(srp_db, using inline OnDialogQueryLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_POLICE_MODIFY:
		{
			inline OnDialogQueryLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					if(rows)
					{
						new level, name[24];
						cache_get_value_name_int(0, "level", level);
						cache_get_value_name(0, "name", name);

						if(level > PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL]) SendNotification(playerid, "No puedes modificar el rango de este policía porque es un rango superior al tuyo.");
						else
						{
							new caption[45], dialog[45 * sizeof(POLICE_RANKS)], line_str[45];
							format(caption, sizeof caption, "policía - %s", name);
							format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Civil (eliminar)\n");
							for(new i = 1; i != sizeof POLICE_RANKS; i ++)
							{
								if(i > PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL])
								{
									if(level == i) format(line_str, sizeof line_str, "{666666}%s (actual)\n", POLICE_RANKS[i]);
									else format(line_str, sizeof line_str, "{666666}%s\n", POLICE_RANKS[i]);
								}
								else
								{
									if(level == i) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s (actual)\n", POLICE_RANKS[i]);
									else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\n", POLICE_RANKS[i]);
								}
								strcat(dialog, line_str);
							}
							ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Atrás");
						}
					}
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT pworks.level, player.name FROM pworks INNER JOIN player ON pworks.id_player = player.id WHERE pworks.id_player = %d AND pworks.id_work = %d AND pworks.`set` = 1;", PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID], WORK_POLICE);
			MySQL_TQueryInline(srp_db, using inline OnDialogQueryLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_POLICE_SHOP:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Equiparse", "{"#SILVER_COLOR"}Comprar armas\n{"#SILVER_COLOR"}Comprar munición para mis armas\n{"#SILVER_COLOR"}Comprar chaleco antibalas", "Comprar", "Cerrar");
			return 1;
		}
		case DIALOG_POLICE_SELECT_WEAPON:
		{
			for(new i = 0; i != MAX_LISTITEMS; i ++) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
			
			new dialog[95 * 15], line_str[95], listitem;
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Arma\t{"#BLUE_COLOR"}Munición\t{"#SILVER_COLOR"}Slot\n");
			
			for(new i = 0; i < sizeof PLAYER_WEAPONS[]; i ++)
			{
				if(!PLAYER_WEAPONS[playerid][i][player_weapon_VALID]) continue;
				
				format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#BLUE_COLOR"}%s\t{"#SILVER_COLOR"}%d\n", WEAPON_INFO[ PLAYER_WEAPONS[playerid][i][player_weapon_ID] ][weapon_info_NAME], number_format_thousand(PLAYER_WEAPONS[playerid][i][player_weapon_AMMO]), i);
				strcat(dialog, line_str);
				
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i; 
				listitem ++;
			}
			if(listitem == 0) strcat(dialog, "{666666}No tienes armas");
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Selecciona la arma a la que quieres comprarle munición", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_POLICE_AMMO:
		{
			new dialog[145];
			format(dialog, sizeof dialog, "Arma: %s\nMunición: %s\n\nEl precio por bala es de 2$\n¿Cuánto munición quiere comprar?", WEAPON_INFO[ PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_ID] ][weapon_info_NAME], number_format_thousand(PLAYER_WEAPONS[playerid][ PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_WEAPON_SLOT] ][player_weapon_AMMO]));
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Comprar munición", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_POLICE_ARMOUR:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Comprar chaleco", "El precio del chaleco antibalas es de 300$.\n¿Lo quieres comprar?", "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_POLICE_WEAPONS:
		{
			new dialog[64 * (sizeof(POLICE_SHOP_WEAPONS) + 2)];
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Producto\t{"#SILVER_COLOR"}Precio\n");
			
			for(new i = 0; i != sizeof POLICE_SHOP_WEAPONS; i ++)
			{
				new line_str[64];
				if(POLICE_SHOP_WEAPONS[i][police_shop_RANK] > PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL])
				{
					format(line_str, sizeof line_str, "{666666}%s\t{666666}%s$\n", WEAPON_INFO[ POLICE_SHOP_WEAPONS[i][police_shop_WEAPON_ID] ][weapon_info_NAME], number_format_thousand(POLICE_SHOP_WEAPONS[i][police_shop_WEAPON_PRICE]));
					strcat(dialog, line_str);
				}
				else
				{
					format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\t{"#SILVER_COLOR"}%s$\n", WEAPON_INFO[ POLICE_SHOP_WEAPONS[i][police_shop_WEAPON_ID] ][weapon_info_NAME], number_format_thousand(POLICE_SHOP_WEAPONS[i][police_shop_WEAPON_PRICE]));
					strcat(dialog, line_str);
				}
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Armas", dialog, "Comprar", "Cerrar");
			return 1;
		}
		case DIALOG_HELP:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, ""SERVER_NAME" v"SERVER_VERSION"",
			
				"\
					{"#SILVER_COLOR"}- General\n\
					{"#SILVER_COLOR"}- Información\n\
					{"#SILVER_COLOR"}- Personaje\n\
					{"#SILVER_COLOR"}- Propiedades\n\
					{"#SILVER_COLOR"}- Reportar\n\
					{"#SILVER_COLOR"}- Trabajos\n\
					{"#SILVER_COLOR"}- Vehículos\n\
					{"#SILVER_COLOR"}- Bandas\n\
					{"#PRIMARY_COLOR"}- Membresía VIP\n\
					{"#PRIMARY_COLOR"}- Verificación en dos pasos\n\
				"
			
			, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_HELP_GENERAL:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "General",
			
				"\
					{FFFFFF}Un día del juego dura tres horas reales.\n\
					\n\
					Para subir de nivel necesitas rellenar la reputación, puedes verlo gráficamente\n\
					en la barra que aparece en la parte superior derecha que representa tu reputación.\n\
					La reputación se aumenta cada {"#PRIMARY_COLOR"}25 minutos,{FFFFFF} cuando la reputación llegue\n\
					al límite podrás subir al siguiente nivel con el comando {"#SILVER_COLOR"}/comprarnivel.\n\
					{FFFFFF}El pago de juego(payday) se recibe cada tres reputaciones {"#PRIMARY_COLOR"}(75 minutos).\n\
					{FFFFFF}Puedes utilizar {"#SILVER_COLOR"}/csave{FFFFFF} para guardar los datos de tu cuenta.\n\
					\n\
					Utiliza {"#SILVER_COLOR"}/estadisticas{FFFFFF} para ver tus estadísticas, utiliza {"#SILVER_COLOR"}/exp{FFFFFF} para ver tus habilidades.\n\
					En el nivel uno no podrás agredir a otros jugadores, tampoco podrás tener armas\n\
					hasta el nivel tres.\n\
					Utiliza {"#SILVER_COLOR"}/nombre{FFFFFF} para cambiar tu nombre.\n\
					Utiliza {"#SILVER_COLOR"}/correo{FFFFFF} para cambiar tu correo electrónico.\n\
					Utiliza {"#SILVER_COLOR"}/pass{FFFFFF} para cambiar la contraseña de tu cuenta.\n\
					Para ver el tiempo que te queda en la cárcel utiliza {"#SILVER_COLOR"}/tiempo.\n\
					\n\
					{FFFFFF}Para utilizar el canal de dudas utiliza el comando {"#SILVER_COLOR"}/duda{FFFFFF} seguido de tu duda.\n\
					Para deshabilitar o volver a habilitar el canal de dudas usa {"#SILVER_COLOR"}/dudas.\n\
					\n\
					{FFFFFF}Utiliza {"#SILVER_COLOR"}/panel {FFFFFF}para configuraciones básicas de tu cuenta.\n\
					\n\
				"
			, "Cerrar", "");
			return 1;
		}
		case DIALOG_HELP_INFO:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Información", DIALOG_CREDIT_String, "Cerrar", "");
			return 1;
		}
		case DIALOG_HELP_CHARACTER:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Personaje",
			
				"\
					{FFFFFF}Para poder localizar cualquier punto en San Andreas te recomandamos que compres\n\
					un GPS en cualquier tienda {FF846A}24/7.{FFFFFF} Desde el GPS podrás localizarlo todo, tus vehículos,\n\
					tus propiedades, trabajos, lugares de interés, etc.\n\
					\n\
					Utiliza tu teléfono para llamar a la policía, mecánico, taxi... o para hablar con\n\
					alguien, por defecto tendrás varios contactos en la agenda pero puedes eliminarlos\n\
					o añadir más, puedes tener hasta 20 contactos, todo esto se hace desde {"#SILVER_COLOR"}/movil{FFFFFF} o {"#SILVER_COLOR"}/celular.\n\
					\n\
					{FFFFFF}En las tiendas {FF846A}24/7{FFFFFF} puedes comprar guías para saber el número de cualquier jugador.\n\
					Para evitar aparecer en la guía utiliza {"#SILVER_COLOR"}/sguia.\n\
					\n\
					{FFFFFF}Las barras en la esquina superior derecha representan tu nivel de hidratación y de alimentación,\n\
					si los niveles llegan muy abajo tu personaje se empezará a sentir mal y empezarás a perder vida.\n\
					Los niveles bajarán según tu actividad física, corriendo te dará sed más pronto que caminando.\n\
					Para alimentarte ve a un restaurante de cómida rápida o a un {FF846A}24/7{FFFFFF} donde podrás comprar alimentos,\n\
					que podrás consumir el cualquier momento con {"#SILVER_COLOR"}/alimentos{FFFFFF} o {"#SILVER_COLOR"}/consumir.\n\
					\n\
					{FFFFFF}Puedes dar objetos con el comando {"#SILVER_COLOR"}/dar,{FFFFFF} para más información utiliza {"#SILVER_COLOR"}/man dar.\n\
					{FFFFFF}Puedes deshacerte de tus objetos con el comando {"#SILVER_COLOR"}/tirar,{FFFFFF} para más información utiliza {"#SILVER_COLOR"}/man tirar.\n\
					{FFFFFF}Puedes vender objetos con el comando {"#SILVER_COLOR"}/vender, para más información utiliza {"#SILVER_COLOR"}/man vender.\n\
					{FFFFFF}Para consumir consumibles que lleves contigo utiliza {"#SILVER_COLOR"}/consumir.\n\
					\n\
					{FFFFFF}Puedes cambiar la ropa de tu personaje en cualquier tienda de ropa, también\n\
					podrás comprarte accesorios y ponertelos o quitartelos {"#SILVER_COLOR"}(/accesorios).\n\
					\n\
					{FFFFFF}Si tienes nivel de búsqueda o estás arrestado y mueres irás a la cárcel después de recuperarte.\n\
					Cuando te arresten los policías te pueden requisar tus pertenencias ilegales {FF846A}(armas y drogas).\n\
					\n\
					{FFFFFF}Utiliza {"#SILVER_COLOR"}/inv{FFFFFF} o pulsa {"#SILVER_COLOR"}N {FFFFFF}para ver todo lo que llevas encima.\n\
					{FFFFFF}Utiliza {"#SILVER_COLOR"}/estilo{FFFFFF} para cambiar el estilo de tu personaje.\n\
					\n\
				"
			, "Cerrar", "");
			return 1;
		}
		case DIALOG_HELP_PROPERTIES:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Propiedades",
			
				"\
					{FFFFFF}Puedes encontrar diferentes propiedades en todo {FF846A}San Andreas,{FFFFFF} para comprarla tendrás que saber la\n\
					ID de la propiedad e ir a cualquier banco para comprarla, el dinero se descuenta de la cuenta bancaria.\n\
					\n\
					Puedes localizar tus propiedades con el GPS.\n\
					Puedes dejar pasar a cualquiera a tu propiedad, solo tiene que tocar el timbre\n\
					y tu elijes si quieres dejarlo pasar o no.\n\
					\n\
					Si quieres echar a alguien que está dentro de tu propiedad utiliza el comando {"#SILVER_COLOR"}/echar.\n\
					{FFFFFF}Dentro de la propiedad sitúate en la puerta interior y pon {"#SILVER_COLOR"}/casa,{FFFFFF} desde ahí podras\n\
					cambiar el nombre de la propiedad para así localizarla más fácilmente en el GPS.\n\
					{FFFFFF}también podrás echar a todos los huéspedes de tu propiedad.\n\
					Si tienes un MP3 podrás poner música en él y hacer que se reproduzca para toda la gente\n\
					que haya dentro de la propiedad.\n\
					\n\
					Si quieres vender tu propiedad puedes hacerlo en {"#PRIMARY_COLOR"}\"Gobierno San Andreas\"{FFFFFF}, localizaló en el GPS.\n\
					Puedes vendersela al banco al instante, pero te darán solo el 25% de lo que te costó {FF846A}(50% VIP).\n\
					{FFFFFF}también se la puedes vender a otro jugador, dentro de un rango de precio pre-establecido.\n\
					Para vender tu propiedad a un jugador puedes ponerla en {"#SILVER_COLOR"}/tienda.\n\
					\n\
				"
			, "Cerrar", "");
			return 1;
		}
		case DIALOG_HELP_REPORT:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Reportar",
			
				"\
					{FFFFFF}Para un reporte simple utiliza {"#SILVER_COLOR"}/reportar ID{FFFFFF} para reportar a un jugador.\n\
					\n\
					Para reportes más detallado debes publicarlos en el foro aportando imágenes.\n\
					Los reportes de fallos o bugs también deberás hacerlo en el foro junto con imágenes.\n\
					\n\
				"
			, "Cerrar", "");
			return 1;
		}
		case DIALOG_HELP_VEHICLES:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Vehículos",
			
				"\
					{FF846A}GENERAL\n\
					{FFFFFF}Para arrancar o detener un vehículo presiona {"#SILVER_COLOR"}~k~~CONVERSATION_YES~ {FFFFFF}o utiliza {"#SILVER_COLOR"}/motor.\n\
					{FFFFFF}Utiliza {"#SILVER_COLOR"}/luces{FFFFFF} para encender o apagar las luces del vehículos.\n\
					Si tu vehículo se daña tendrás que llamar a un mecánico desde tu móvil para que lo repare.\n\
					\n\
					Cada vehículo tiene su cantidad máxima del depósito de gasolina.\n\
					Para repostar el vehículo debes ir a cualquier gasolinera, puedes localizarlas con el GPS.\n\
					también puedes comprar un bidón de gasolina para {"#SILVER_COLOR"}/vertir{FFFFFF} gasolina en cualquier momento.\n\
					Si tienes un MP3 puedes poner una canción en el vehículo con el comando {"#SILVER_COLOR"}/vmp3.\n\
					\n\
					\n\
					{FF846A}VEHICULOS PERSONALES\n\
					{FFFFFF}Para comprarlos ve a cualquier concesionario.\n\
					El dinero para comprarlo es el de la cuenta bancaria.\n\
					Para localizar tus vehículos utiliza el GPS.\n\
					\n\
					Utiliza {"#SILVER_COLOR"}/abrir{FFFFFF} para abrir las puertes de tu vehículo, utiliza {"#SILVER_COLOR"}/cerrar{FFFFFF} para cerrarlas.\n\
					Puedes estacionar tu vehículo con el comando {"#SILVER_COLOR"}/estacionar.\n\
					{FFFFFF}Cuando tu vehículo sea destruido recibirás uno nuevo en tu último estacionamiento.\n\
					Para pintar, tunear o modificar tu vehículo tendrás que hacerlo en el mecánico.\n\
					\n\
					Puedes guardar cosas en el maletero del vehículo con el comando {"#SILVER_COLOR"}/guardar.\n\
					{FFFFFF}Para sacar cosas del maletero puedes utilizar {"#SILVER_COLOR"}/maletero.\n\
					\n\
					{FFFFFF}Si quieres vender tu vehículo puedes hacerlo en \"Gobierno San Andreas\", localizaló en el GPS.\n\
					Puedes venderselo al banco, pero te darán solo el 25% de lo que te costó (50% VIP).\n\
					también se la puedes vender a otro jugador, dentro de un rango de precio pre-establecido.\n\
					Para vender tu vehículo a un jugador puedes ponerlo en {"#SILVER_COLOR"}/tienda.\n\
					{FFFFFF}Al vender tu vehículo se venderá con todo lo que haya en el maletero.\n\
					\n\
				"
			, "Cerrar", "");
			return 1;
		}
		case DIALOG_HELP_WORKS:
		{
			new dialog[45 * sizeof work_info], line_str[45];
			for(new i = 1; i != sizeof work_info; i ++)
			{
				format(line_str, sizeof line_str, "{"#SILVER_COLOR"}- %c%s\n", toupper(work_info[i][work_info_NAME][0]), work_info[i][work_info_NAME][1]);
				strcat(dialog, line_str);
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Trabajos", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_HELP_CREWS:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Bandas",
			
				"\
					{FFFFFF}Utiliza {"#SILVER_COLOR"}/banda{FFFFFF} para crear una banda o administrar la tuya.\n\
					{FFFFFF}Al entrar a una banda puedes conquistar territorios por todo San Andreas.\n\
					\n\
					{FFFFFF}Visita {FF846A}"SERVER_WEBSITE" {FFFFFF}para más información.\n\
					\n\
				"
			
			, "Cerrar", "");
			return 1;
		}
		case DIALOG_HELP_SU:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Membresía VIP",
			
				"\
					{"#SILVER_COLOR"}Puedes comprar VIP por 30 días con el comando /vip.\n\
					{"#SILVER_COLOR"}Se compra con "SERVER_COIN", para ver como comprar esta moneda visita nuestra web.\n\
					\n\
					{"#PRIMARY_COLOR"}Ventajas\n\
					{"#SILVER_COLOR"}- Tener "#MAX_SU_WORKS" trabajos.\n\
					{"#SILVER_COLOR"}- Tener "#MAX_SU_VEHICLES" vehículos.\n\
					{"#SILVER_COLOR"}- Tener "#MAX_SU_TOYS" accesorios.\n\
					{"#SILVER_COLOR"}- Tener "#MAX_SU_PROPERTIES" propiedades.\n\
					{"#SILVER_COLOR"}- Tener "#MAX_SU_PROPERTY_OBJECTS" muebles por propiedad.\n\
					{"#SILVER_COLOR"}- No pagas seguro de propiedades ni vehículos en el Payday.\n\
					{"#SILVER_COLOR"}- No pierdes chaleco al morir.\n\
					{"#SILVER_COLOR"}- Al vender un vehículo o propiedad al banco recibes un 50%.\n\
					{"#SILVER_COLOR"}- Te recuperas el doble de rápido en el hospital.\n\
					{"#SILVER_COLOR"}- Al recuperarte tu vida es del 100%.\n\
					{"#SILVER_COLOR"}- Reducción de la condena a la mitad.\n\
					{"#SILVER_COLOR"}- Cuando un médico te cura vida al 50%.\n\
					{"#SILVER_COLOR"}- Comprar armas en mercado negro sin pertenecer a banda.\n\
					{"#SILVER_COLOR"}- Comprar armas sin esperar 5 minutos por cada compra.\n\
					{"#SILVER_COLOR"}- Pagas extras de 300$ en algunos trabajos.\n\
					\n\
					{"#SILVER_COLOR"}Visita {"#PRIMARY_COLOR"}"SERVER_WEBSITE" {"#SILVER_COLOR"}para más información.\n\
					\n\
				"
			
			, "Cerrar", "");
			return 1;
		}
		case DIALOG_HELP_SECURE_LOGIN:
		{
			new string[512];
			format(string, sizeof string, 
				"\
					Activa la verificación en dos pasos para proteger tu cuenta.\n\
					Cuando tu IP cambie se enviará un correo electrónico para\n\
					poder verificiar el inicio de sesión\n\
					\n\
					Tu dirección de correo electrónico actual es: %s.\n\
					Puedes cambiar tu correo con el comando /correo.\n\
					\n\
					Para activar o desactivar la verificación en dos pasos utiliza el comando /panel.\
				",
					ACCOUNT_INFO[playerid][ac_EMAIL]
			);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Verificación en dos pasos", string, "Cerrar", "");
			return 1;
		}
		case DIALOG_ADMIN_LIST:
		{
			inline OnDialogQueryLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new dialog[128 * 15], line_str[128], count, start_pos = PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] + 1;
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Nombre\t{"#SILVER_COLOR"}Rango\t{"#SILVER_COLOR"}Últ. conexión\n");
					for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
					for(new i = 0; i != rows; i++)
					{
						new id, name[24], last_connection[24], connected, pid, admin_level;
						cache_get_value_name_int(i, "id", id);
						cache_get_value_name(i, "name", name);
						cache_get_value_name(i, "last_connection", last_connection);
						cache_get_value_name_int(i, "connected", connected);
						cache_get_value_name_int(i, "playerid", pid);
						cache_get_value_name_int(i, "admin_level", admin_level);

						if(connected)
						{
							if(PLAYER_TEMP[pid][pt_ADMIN_SERVICE]) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#SILVER_COLOR"}%s\t{"#GREEN_COLOR"}Duty (%d)\n", start_pos + i, name, ADMIN_LEVELS[admin_level], pid);
							else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#SILVER_COLOR"}%s\t{"#GREEN_COLOR"}Conectado (%d)\n", start_pos + i, name, ADMIN_LEVELS[admin_level], pid);
						}
						else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#SILVER_COLOR"}%s\t{"#SILVER_COLOR"}%s\n", start_pos + i, name, ADMIN_LEVELS[admin_level], last_connection);
						strcat(dialog, line_str);
						PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = id;
					}

					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -2;
					strcat(dialog, "{c9c9c9}- Siguiente\n");
					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -3;
					strcat(dialog, "{c9c9c9}- Anterior\n");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Administradores", dialog, "Continuar", "Cerrar");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, name, last_connection, connected, playerid, admin_level FROM player WHERE admin_level > 0 ORDER BY connected DESC, admin_level DESC LIMIT %d, %d;", PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT], PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);
			MySQL_TQueryInline(srp_db, using inline OnDialogQueryLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_ADMIN_MODIFY:
		{
			inline OnDialogQueryLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					if(rows)
					{
						new name[24], admin_level;
						cache_get_value_name(0, "name", name);
						cache_get_value_name_int(0, "admin_level", admin_level);

						if(admin_level > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL]) SendNotification(playerid, "No puedes modificar el rango de este admin porque es un rango superior al tuyo.");
						else
						{
							new caption[45], dialog[45 * sizeof(ADMIN_LEVELS)], line_str[45];
							format(caption, sizeof caption, "Admin - %s", name);
							format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Normal (eliminar)\n");

							for(new i = 1; i != sizeof ADMIN_LEVELS; i ++)
							{
								if(i > ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL])
								{
									if(admin_level == i) format(line_str, sizeof line_str, "{666666}%s (actual)\n", ADMIN_LEVELS[i]);
									else format(line_str, sizeof line_str, "{666666}%s\n", ADMIN_LEVELS[i]);
								}
								else
								{
									if(admin_level == i) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s (actual)\n", ADMIN_LEVELS[i]);
									else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\n", ADMIN_LEVELS[i]);
								}
								strcat(dialog, line_str);
							}
							
							ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Atrás");
						}
					}
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT name, admin_level FROM player WHERE id = %d;", PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID]);
			MySQL_TQueryInline(srp_db, using inline OnDialogQueryLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_CHANGE_PASSWORD:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_PASSWORD, "Cambiar contraseña", "Introduce tu contraseña actual.", "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_CHANGE_PASSWORD_PASS:
		{
			new dialog[128];
			format(dialog, sizeof dialog, "Introduce tu nueva contraseña.\nLa contraseña debe contener entre %d-%d caracteres.", MIN_PASS_LENGTH, MAX_PASS_LENGTH);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_PASSWORD, "Cambiar contraseña", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_ANTI_CHEAT:
		{
			if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] < 4) return 0;
			
			new dialog[115 * sizeof(ac_Info) + 1], line_str[115];
			dialog = "Tipo\tEstado\tAccionar\tAcción\n";
			
			for(new i = 0; i != sizeof(ac_Info); i ++)
			{
				format(line_str, sizeof line_str, "%02d %s\t%s\t%d:%d\t%s\n", i, ac_Info[i][ac_Name], ac_Info[i][ac_Enabled] ? "Activado" : "Desactivado", ac_Info[i][ac_Detections], ac_Info[i][ac_Interval], ac_Info[i][ac_Kick] ? "Kick" : "Aviso");
				strcat(dialog, line_str);
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Anticheat", dialog, "Continuar", "X");
			return 1;
		}
		case DIALOG_ANTI_CHEAT_MODIFY:
		{
			if(ACCOUNT_INFO[playerid][ac_ADMIN_LEVEL] < 4) return 0;
			
			new caption[45], dialog[90];
			
			format(caption, sizeof caption, "ac %02d %s", PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM], ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Name]);
			format(dialog, sizeof dialog, "Estado\t%s\nAccionar\t(%d:%d)\nAccion\t%s", ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Enabled] ? "Activado" : "Desactivado", ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Detections], ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Interval], ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Kick] ? "Kick" : "Aviso");
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_ANTI_CHEAT_MODIFY_DETECT:
		{
			new caption[45], dialog[200];
			
			format(caption, sizeof caption, "ac %02d %s", PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM], ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Name]);
			format(dialog, sizeof dialog, "Estado actual de accioner en: (%d:%d)\n\nSintaxis: <detecciones>:<segundos>\nDeja segundos en 0 para que sea instantáneo.\n\nEjemplo de 2 detecciones en 10 segundos: 2:10", ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Detections], ac_Info[ PLAYER_TEMP[playerid][pt_SELECTED_AC_LISTITEM] ][ac_Interval]);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_SHOP_ARTICLE_MODIFY: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Mi anuncio", "Inserta el nuevo precio de tu artículo:\n", "Continuar", "Atrás");
		case DIALOG_SHOP_ARTICLE_REMOVE: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Eliminar anuncio", "¿Estás seguro de que quiere eliminar este anuncio?\nEsta opción no se puede deshacer.", "Eliminar", "Atrás");	
		case DIALOG_SHOP_ADD:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Añadir anuncio", "{"#SILVER_COLOR"}Propiedad\n{"#SILVER_COLOR"}Vehículo\n{"#SILVER_COLOR"}Otros", "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_SHOP_ADD_TEXT:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Añadir anuncio", "Introduce una breve descripción para tu anuncio.\nCaracteres: 1-24 [A-Z a-z 0-9]", "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_SHOP_ADD_PRICE:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Añadir anuncio", "Introduce el precio para tu anuncio.\nAl publicar tu anuncio se te cobrará 500$.", "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_CRANE_SELECT_VEHICLE:
		{
			for(new i = 0; i != MAX_LISTITEMS; i ++) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
			
			new dialog[95 * (MAX_SU_VEHICLES + 2)], total_vehicles;
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Vehículo\t{"#BLUE_COLOR"}Matrícula\t{"#SILVER_COLOR"}Precio\n");
			for(new i = 0, j = GetVehiclePoolSize(); i <= j; i ++)
			{
				if(!GLOBAL_VEHICLES[i][gb_vehicle_VALID]) continue;
				if(!PLAYER_VEHICLES[i][player_vehicle_VALID]) continue;
				if(total_vehicles > MAX_SU_VEHICLES) break;
				
				if(PLAYER_VEHICLES[i][player_vehicle_OWNER_ID] == ACCOUNT_INFO[playerid][ac_ID])
				{
					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][total_vehicles] = i;
					
					new line_str[95], veh_money = (VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_PRICE] / 100) + 350;
					format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#BLUE_COLOR"}%s\t{"#SILVER_COLOR"}%s$\n", total_vehicles + 1, VEHICLE_INFO[ GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME], GLOBAL_VEHICLES[i][gb_vehicle_NUMBER_PLATE], number_format_thousand(veh_money));
					strcat(dialog, line_str);
					total_vehicles ++;
				}
			}
			if(total_vehicles == 0) strcat(dialog, "{"#SILVER_COLOR"}No tienes vehículos");
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Selecciona el vehículo que quieres remolcar", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_CREATE_CREW:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Banda", "No perteneces a ninguna banda.\n\n¿Quieres crear una?\nPara crear una banda necesitas ser nivel 5, además cuesta 600.000$.", "Sí", "No");
			return 1;
		}
		case DIALOG_CREATE_CREW_NAME:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Crear banda", "Introduce el nombre de tu banda.\nHasta 32 caracteres.", "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_CREATE_CREW_COLOR:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Selecciona un color para tu banda", DIALOG_CREATE_CREW_COLOR_String, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_CREATE_CREW_CONFIRM:
		{
			new dialog[360];
			format(dialog, sizeof dialog,
					
					"\
						{"#SILVER_COLOR"}Nombre de la banda: %s\n\
						{"#SILVER_COLOR"}Color de la banda: {%06x}color %d\n\
						{"#SILVER_COLOR"}Coste de creación: 600.000$\n\
						\n\
						{"#SILVER_COLOR"}¿Quieres crear esta banda?\n\
						{"#SILVER_COLOR"}Una vez creada podrás crear rangos, modificarlos y mucho más.\n\
						\n\
					", 
						PLAYER_TEMP[playerid][pt_CREATE_CREW_NAME],
						RandomColors[ PLAYER_TEMP[playerid][pt_CREATE_CREW_COLOR] ] >>> 8, PLAYER_TEMP[playerid][pt_CREATE_CREW_COLOR] + 1
			);
					
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Crear banda", dialog, "Crear", "Cerrar");
			return 1;
		}
		case DIALOG_CREW_MENU:
		{
			for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
			
			new caption[64], dialog[360], listitem;
			format(caption, sizeof caption, "Mi banda -  {%06x}'%s'", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_COLOR] >>> 8, CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_NAME]);
			
			format(dialog, sizeof dialog, "Ver miembros\n");
			PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = CREW_RANK_SIZE;
			listitem ++;

			if(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_CHANGE_NAME])
			{
				strcat(dialog, "Cambiar nombre de la banda\n");
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = CREW_RANK_CHANGE_NAME;
				listitem ++;
			}
			if(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_CAST_MEMBERS])
			{
				strcat(dialog, "Echar a un miembro\n");
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = CREW_RANK_CAST_MEMBERS;
				listitem ++;
			}
			if(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_MEMBERS])
			{
				strcat(dialog, "Cambiar rango de un miembro\n");
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = CREW_RANK_MODIFY_MEMBERS;
				listitem ++;
			}
			if(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_MODIFY_RANKS])
			{
				strcat(dialog, "Crear rangos o modificarlos\n");
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = CREW_RANK_MODIFY_RANKS;
				listitem ++;
			}
			if(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_CHANGE_COLOR])
			{
				strcat(dialog, "Cambiar color de la banda\n");
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = CREW_RANK_CHANGE_COLOR;
				listitem ++;
			}
			if(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][CREW_RANK_DELETE])
			{
				strcat(dialog, "Eliminar la banda\n");
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = CREW_RANK_DELETE;
				listitem ++;
			}
			
			strcat(dialog, "Abandonar banda\n");
			PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = CREW_RANK_SIZE + 1;
			listitem ++;
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_CREW_MEMBER_LIST:
		{
			inline OnDialogQueryLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new dialog[128 * 15], caption[64], line_str[128], count, start_pos = PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] + 1;
					format(caption, sizeof caption, "Miembros de la banda '%s'", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_NAME]);
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Nombre\t{"#SILVER_COLOR"}Rango\t{"#SILVER_COLOR"}Últ. conexión\n");
					for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
					for(new i = 0; i != rows; i++)
					{
						new name[24], last_connection[24], connected, crew_rank;
						cache_get_value_name(i, "name", name);
						cache_get_value_name(i, "last_connection", last_connection);
						cache_get_value_name_int(i, "connected", connected);
						cache_get_value_name_int(i, "crew_rank", crew_rank);

						if(connected) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#SILVER_COLOR"}%s\t{"#GREEN_COLOR"}Conectado\n", start_pos + i, name, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ crew_rank ][crew_rank_NAME]);
						else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#SILVER_COLOR"}%s\t{"#SILVER_COLOR"}%s\n", start_pos + i, name, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ crew_rank ][crew_rank_NAME], last_connection);
						strcat(dialog, line_str);
						count++;
					}

					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -2;
					strcat(dialog, "{c9c9c9}- Siguiente\n");
					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -3;
					strcat(dialog, "{c9c9c9}- Anterior\n");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, caption, dialog, "Continuar", "Cerrar");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT name, last_connection, connected, crew_rank FROM player WHERE crew = %d ORDER BY connected DESC, crew_rank ASC LIMIT %d, %d;", PI[playerid][ac_CREW], PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT], PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);
			MySQL_TQueryInline(srp_db, using inline OnDialogQueryLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_CREW_CHANGE_NAME:
		{
			new dialog[128];
			format(dialog, sizeof dialog, "Nombre actual de la banda:\n\t\"%s\"\n\nIndica el nuevo nombre.\nMáximo 32 caracteres.", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_NAME]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Cambiar nombre de la banda", dialog, "Cambiar", "Atrás");
			return 1;
		}
		case DIALOG_CREW_MEMBER_LIST_DELETE:
		{
			inline OnDialogQueryLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new dialog[128 * 15], line_str[128], count, start_pos = PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] + 1;
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Nombre\t{"#SILVER_COLOR"}Rango\t{"#SILVER_COLOR"}Últ. conexión\n");
					for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
					for(new i = 0; i != rows; i++)
					{
						new id, name[24], last_connection[24], connected, crew_rank;
						cache_get_value_name_int(i, "id", id);
						cache_get_value_name(i, "name", name);
						cache_get_value_name(i, "last_connection", last_connection);
						cache_get_value_name_int(i, "connected", connected);
						cache_get_value_name_int(i, "crew_rank", crew_rank);

						if(connected) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#SILVER_COLOR"}%s\t{"#GREEN_COLOR"}Conectado\n", start_pos + i, name, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ crew_rank ][crew_rank_NAME]);
						else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#SILVER_COLOR"}%s\t{"#SILVER_COLOR"}%s\n", start_pos + i, name, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ crew_rank ][crew_rank_NAME], last_connection);
						strcat(dialog, line_str);
						PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = id;
					}

					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -2;
					strcat(dialog, "{c9c9c9}- Siguiente\n");
					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -3;
					strcat(dialog, "{c9c9c9}- Anterior\n");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Echar a un miembro", dialog, "Continuar", "Cerrar");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, name, last_connection, connected, crew_rank FROM player WHERE crew = %d ORDER BY connected DESC, crew_rank ASC LIMIT %d, %d;", PI[playerid][ac_CREW], PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT], PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);
			MySQL_TQueryInline(srp_db, using inline OnDialogQueryLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_CREW_CAST_MEMBER_CONFIRM:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Echar a un miembro", "¿Estás seguro de que quieres echar a este miembro de la banda?", "Echar", "Cancelar");
			return 1;
		}
		case DIALOG_CREW_INVITE_RANK:
		{
			for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
			
			new dialog[64 * MAX_CREW_RANKS], line_str[64], listitem;
			for(new i = 0; i != MAX_CREW_RANKS; i ++)
			{
				if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_VALID]) continue;
				
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i;
				if(i < PI[playerid][ac_CREW_RANK]) format(line_str, sizeof line_str, "{666666}%d. %s\n", listitem + 1, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
				else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\n", listitem + 1, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
				strcat(dialog, line_str);
				
				listitem ++;
			}
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Invitar", dialog, "Invitar", "Cerrar");
			return 1;
		}
		case DIALOG_CREW_INVITE_ACCEPT:
		{
			new dialog[128];
			format(dialog, sizeof dialog, "Te han ofrecio unirte a una banda.\n\nBanda: %s\nRango: %s\n", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INVITE_INFO][1] ][crew_NAME], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INVITE_INFO][1] ][ PLAYER_TEMP[playerid][pt_CREW_INVITE_INFO][2] ][crew_rank_NAME]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Invitación", dialog, "Unirme", "Cerrar");
			return 1;
		}
		case DIALOG_CREW_RANKS:
		{
			new dialog[64 * MAX_CREW_RANKS], line_str[64];
			for(new i = 0; i != MAX_CREW_RANKS; i ++)
			{
				if(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_VALID])
				{
					if(i < PI[playerid][ac_CREW_RANK]) format(line_str, sizeof line_str, "{666666}%d. %s\n", i + 1, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
					else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\n", i + 1, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
				}
				else
				{
					if(i < PI[playerid][ac_CREW_RANK]) format(line_str, sizeof line_str, "{666666}%d. Slot vacío\n", i + 1, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
					else format(line_str, sizeof line_str, "{FFFFFF}%d. Slot vacío\n", i + 1, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
				}
				strcat(dialog, line_str);
			}
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Rangos", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_CREW_RANK_MODIFY:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Modificar rango", "Cambiar nombre del rango\nModificar permisos\nEliminar rango", "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_CREW_RANK_MODIFY_NAME:
		{
			new dialog[145];
			format(dialog, sizeof dialog, "Nombre actual del rango:\n\t\"%s\"\n\nIndica el nuevo nombre del rango.\nMáximo 32 caracteres.", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_NAME]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Modificar rango", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_CREW_RANK_MODIFY_PERMISS:
		{
			new caption[70];
			format(caption, sizeof caption, "Modificar permisos del rango '%s'", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_NAME]);
			
			new dialog[64 * (CREW_RANK_SIZE + 1)], line_str[64];
			format(dialog, sizeof dialog, "Permiso\tActivado\n");
			
			for(new i = 0; i != CREW_RANK_SIZE; i ++)
			{
				if(CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PI[playerid][ac_CREW_RANK] ][crew_rank_PERMISSION][i]) format(line_str, sizeof line_str, "%s\t%s\n", CREW_RANKS_PERMISSIONS[i], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][i] ? "Sí" : "No");
				else format(line_str, sizeof line_str, "{666666}%s\t{666666}%s\n", CREW_RANKS_PERMISSIONS[i], CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_PERMISSION][i] ? "Sí" : "No");
				
				strcat(dialog, line_str);
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, caption, dialog, "Cambiar", "Atrás");
			return 1;
		}
		case DIALOG_CREW_RANK_CREATE:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Crear nuevo rango", "Indica el nombre del rango.\nMáximo 32 caracteres.", "Crear", "Atrás");
			return 1;
		}
		case DIALOG_CREW_RANK_DELETE:
		{
			for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
			
			new dialog[64 * MAX_CREW_RANKS], line_str[64], listitem;
			for(new i = 0; i != MAX_CREW_RANKS; i ++)
			{
				if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_VALID]) continue;
				if(i == PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK]) continue;
				
				PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i;
				if(i < PI[playerid][ac_CREW_RANK]) format(line_str, sizeof line_str, "{666666}%d. %s\n", listitem + 1, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
				else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\n", listitem + 1, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
				strcat(dialog, line_str);
				
				listitem ++;
			}
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Seleccionar nuevo rango", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_CREW_RANK_DELETE_CONFIRM:
		{
			inline OnCrewInfoLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					if(rows)
					{
						new affected_members;
						cache_get_value_index_int(0, 0, affected_members);
						
						new dialog[400];
						format(dialog, sizeof dialog, 
								
							"\
								Rango que se eliminará:\n\
									\t'%s'\n\
								\n\
								Miembros afectados: %d\n\
								El nuevo rango de estos miembros será:\n\
									\t'%s'\n\
								\n\
								¿Seguro que quieres eliminar el rango? Esta opción no se puede deshacer.\n\
								\n\
							",
								CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK] ][crew_rank_NAME],
								affected_members,
								CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ PLAYER_TEMP[playerid][pt_CREW_SELECTED_NEW_RANK] ][crew_rank_NAME]
						);
						ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Eliminar rango", dialog, "Eliminar", "Cancelar");

					}
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d AND crew_rank = %d;", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_ID], PLAYER_TEMP[playerid][pt_CREW_SELECTED_RANK]);
			MySQL_TQueryInline(srp_db, using inline OnCrewInfoLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_CREW_MODIFY_COLOR:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Selecciona el nuevo color para tu banda", DIALOG_CREATE_CREW_COLOR_String, "Cambiar", "Atrás");
			return 1;
		}
		case DIALOG_CREW_DELETE:
		{
			inline OnCrewInfoLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					if(rows)
					{
						new affected_members, crew_territories;
						cache_get_value_index_int(0, 0, affected_members);
						cache_get_value_index_int(0, 0, crew_territories);

						new dialog[450];
						format(dialog, sizeof dialog, 
								
							"\
								Banda: %s\n\
								Miembros: %d\n\
								Territorios: %d\n\
								\n\
								¿De verdad quieres borrar la banda?\n\
								\n\
							",
								CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_NAME],
								affected_members,
								crew_territories
						);
						ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Eliminar banda", dialog, "Eliminar", "Cancelar");
					}
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(player.id), COUNT(territories.id) FROM player, territories WHERE player.crew = %d AND territories.id_crew = %d;", CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_ID], CREW_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][crew_ID]);
			MySQL_TQueryInline(srp_db, using inline OnCrewInfoLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_CREW_LEAVE:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Abandonar banda", "¿Quieres abandonar la banda?\nEsta opción no se puede deshacer.", "Abandonar", "Cancelar");
			return 1;
		}
		case DIALOG_CREW_MODIFY_MEMBERS:
		{
			inline OnDialogQueryLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					new dialog[128 * 15], line_str[128], count, start_pos = PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] + 1;
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Nombre\t{"#SILVER_COLOR"}Rango\t{"#SILVER_COLOR"}Últ. conexión\n");
					for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
					for(new i = 0; i != rows; i++)
					{
						new id, name[24], last_connection[24], connected, crew_rank;
						cache_get_value_name_int(i, "id", id);
						cache_get_value_name(i, "name", name);
						cache_get_value_name(i, "last_connection", last_connection);
						cache_get_value_name_int(i, "connected", connected);
						cache_get_value_name_int(i, "crew_rank", crew_rank);

						if(connected) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#SILVER_COLOR"}%s\t{"#GREEN_COLOR"}Conectado\n", start_pos + i, name, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ crew_rank ][crew_rank_NAME]);
						else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#SILVER_COLOR"}%s\t{"#SILVER_COLOR"}%s\n", start_pos + i, name, CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][ crew_rank ][crew_rank_NAME], last_connection);
						strcat(dialog, line_str);
						PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = id;
					}

					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -2;
					strcat(dialog, "{c9c9c9}- Siguiente\n");
					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -3;
					strcat(dialog, "{c9c9c9}- Anterior\n");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "Modificar rango de un miembro", dialog, "Continuar", "Cerrar");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id, name, last_connection, connected, crew_rank FROM player WHERE crew = %d ORDER BY connected DESC, crew_rank ASC LIMIT %d, %d;", PI[playerid][ac_CREW], PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT], PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);
			MySQL_TQueryInline(srp_db, using inline OnDialogQueryLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_CREW_MODIFY_MEMBER:
		{
			inline OnCrewInfoLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					if(rows)
					{
						new name[24], crew_rank;
						cache_get_value_name(0, "name", name);
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
												new caption[45], dialog[45 * CREW_RANK_SIZE], line_str[45], count;
												format(caption, sizeof caption, "Miembro - %s", name);
												for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
												for(new i = 0; i != CREW_RANK_SIZE; i ++)
												{
													if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_VALID]) continue;
													
													if(i < PI[playerid][ac_CREW_RANK])
													{
														if(crew_rank == i) format(line_str, sizeof line_str, "{666666}%s (actual)\n", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
														else format(line_str, sizeof line_str, "{666666}%s\n", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
													}
													else
													{
														if(crew_rank == i) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s (actual)\n", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
														else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\n", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
													}
													strcat(dialog, line_str);
													PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = i;
												}
												ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Atrás");
											}
										}
									}
								}
								mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT COUNT(id) FROM player WHERE crew = %d AND crew_rank = 0;", PI[playerid][ac_CREW]);
								MySQL_TQueryInline(srp_db, using inline OnCountQueryLoad, QUERY_BUFFER);
							}
							else
							{
								new caption[45], dialog[45 * CREW_RANK_SIZE], line_str[45], count;
								format(caption, sizeof caption, "Miembro - %s", name);
								for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
								for(new i = 0; i != CREW_RANK_SIZE; i ++)
								{
									if(!CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_VALID]) continue;
									
									if(i < PI[playerid][ac_CREW_RANK])
									{
										if(crew_rank == i) format(line_str, sizeof line_str, "{666666}%s (actual)\n", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
										else format(line_str, sizeof line_str, "{666666}%s\n", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
									}
									else
									{
										if(crew_rank == i) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s (actual)\n", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
										else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%s\n", CREW_RANK_INFO[ PLAYER_TEMP[playerid][pt_CREW_INDEX] ][i][crew_rank_NAME]);
									}
									strcat(dialog, line_str);
									PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = i;
								}
								ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Atrás");
							}
						}
					}
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT name, crew_rank FROM player WHERE id = %d;",  PLAYER_TEMP[playerid][pt_SELECTED_DB_AC_ID]);
			MySQL_TQueryInline(srp_db, using inline OnCrewInfoLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_CREW_PROPERTY_CONFIRM:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Propiedad de banda", "¿Estás seguro que quieres pasar esta propiedad a banda?\nEsta opción no se puede deshacer.", "Continuar", "Cancelar");
			return 1;
		}
		case DIALOG_CREW_LEAVE_TERRITORY:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Abandonar territorio", "¿Estás seguro que quieres abandonar este territorio?\nEsta opción no se puede deshacer.", "Continuar", "Cancelar");
			return 1;
		}
		case DIALOG_CREW_LEAVE_PROPERTY:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Propiedad de banda", "¿Estás seguro que quieres liberar esta propiedad?\nEsta opción no se puede deshacer.", "Continuar", "Cancelar");
			return 1;
		}
		case DIALOG_POLICE_PENALTY:
		{
			new dialog[128];
			format(dialog, sizeof dialog, "El policía %s te ha puesto una multa.\nImporte: %s$\n", PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_POLICE_PEN_PID] ][pt_RP_NAME], number_format_thousand(PLAYER_TEMP[playerid][pt_POLICE_PEN_IM]));
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Multa", dialog, "Pagar", "Cerrar");
			return 1;
		}
		case DIALOG_SERVER_ECO:
		{
			new dialog[512];
			format(dialog, sizeof dialog,
				
				"\
					PROPIEDADES\t%.4f\n\
					VEHICULOS\t%.4f\n\
					TOYS\t%.4f\n\
					SKINS\t%.4f\n\
					ARMAS\t%.4f\n\
					VIP\t%d "SERVER_COIN"\n\
					VIP JOB EXTRA PAY\t%d\n\
					CHANGE NAME\t%d "SERVER_COIN"\n\
				",
					MULTIPLIER_PROPERITES,
					MULTIPLIER_VEHICLES,
					MULTIPLIER_TOYS,
					MULTIPLIER_SKINS,		
					MULTIPLIER_WEAPONS,
					SU_SD_PRICE,
					SU_WORK_EXTRA_PAY,
					CHANGE_NAME_SD_PRICE
			);	
				
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST, "Multiplicadores", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_SERVER_ECO_MODIFY:
		{
			new dialog[32];
			switch(PLAYER_TEMP[playerid][pt_DIALOG_ECO_SELECTED])
			{
				case 0: format(dialog, sizeof dialog, "PROPIEDADES: %f", MULTIPLIER_PROPERITES);
				case 1: format(dialog, sizeof dialog, "VEHICULOS: %f", MULTIPLIER_VEHICLES);
				case 2: format(dialog, sizeof dialog, "TOYS: %f", MULTIPLIER_TOYS);
				case 3: format(dialog, sizeof dialog, "SKINS: %f", MULTIPLIER_SKINS);
				case 4: format(dialog, sizeof dialog, "ARMAS: %f", MULTIPLIER_WEAPONS);
				case 5: format(dialog, sizeof dialog, "Coste VIP: %d "SERVER_COIN"", SU_SD_PRICE);
				case 6: format(dialog, sizeof dialog, "VIP JOB EXTRA PAY: %d", SU_WORK_EXTRA_PAY);
				case 7: format(dialog, sizeof dialog, "Coste cambiar nombre: %d "SERVER_COIN"", CHANGE_NAME_SD_PRICE);
			}
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Multiplicador", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_SU_BUY:
		{
			new dialog[128];
			format(dialog, sizeof dialog, "El coste del VIP es de %d "SERVER_COIN"\nDuración: 30 días\n\n¿Comprar?", SU_SD_PRICE);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "VIP", dialog, "Comprar", "Cerrar");
			return 1;
		}
		case DIALOG_SU:
		{
			new dialog[256], current_date[24];
			getDateTime(current_date);
			format(dialog, sizeof dialog, 
				
				"\
					Fecha actual: %s\n\
					Fecha caducidad: %s\n\
					\n\
					Renuevalo 30 días más por %d "SERVER_COIN".\n\
					\n\
				",
					current_date,
					ACCOUNT_INFO[playerid][ac_VIP_EXPIRE_DATE],
					SU_SD_PRICE
			);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "VIP", dialog, "Cerrar", "Renovar");
			return 1;
		}
		case DIALOG_VOBJECT_MENU:
		{
			new caption[45];
			format(caption, sizeof caption, "STC - '%s'", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, "1. Mis objetos\n2. Comprar objeto\n3. Añadir texto (500$)\n4. Añadir bandera de país (2.000$)\n5. Añadir panel de país (2.000$)", "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_VOBJECT_OBJECTS:
		{
			new caption[45], dialog[145 * (MAX_SU_VOBJECTS + 2)], line_str[145];
			format(caption, sizeof caption, "STC - '%s'", VEHICLE_INFO[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_MODELID] - 400 ][vehicle_info_NAME]);
			
			format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Objeto\t{"#BLUE_COLOR"}Colocado\n");
			for(new i = 0; i != MAX_SU_VOBJECTS; i ++)
			{
				if(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][i][vobject_VALID])
				{
					if(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][i][vobject_ATTACHED]) format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#BLUE_COLOR"}Sí\n", i + 1, VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][i][vobject_NAME]);
					else format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#BLUE_COLOR"}No\n", i + 1, VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][i][vobject_NAME]);
				}
				else format(line_str, sizeof line_str, "{666666}%d. Slot vacío\n", i + 1);
				strcat(dialog, line_str);
			}
			strcat(dialog, "{c4290d}- Eliminar todo\n");
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_VOBJECT_OBJECT:
		{
			new caption[45], dialog[256];
			format(caption, sizeof caption, "Objeto '%s'", VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_NAME]);
			
			switch(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_TYPE])
			{
				case VOBJECT_TYPE_OBJECT:
				{
					if(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ATTACHED]) format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Guardar este objeto\n{"#SILVER_COLOR"}Cambiar el nombre del objeto\n{"#SILVER_COLOR"}Editar posición del objeto\n{"#SILVER_COLOR"}Colores\n{"#SILVER_COLOR"}Eliminar este objeto");
					else format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Colocar este objeto\n{"#SILVER_COLOR"}Cambiar el nombre del objeto\n{666666}Editar posición del objeto\n{666666}Colores\n{"#SILVER_COLOR"}Eliminar este objeto");
				}
				case VOBJECT_TYPE_TEXT:
				{
					if(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ATTACHED]) format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Guardar este objeto\n{"#SILVER_COLOR"}Cambiar el nombre del objeto\n{"#SILVER_COLOR"}Editar posición del objeto\n{"#SILVER_COLOR"}Texto\n{"#SILVER_COLOR"}Eliminar este objeto");
					else format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Colocar este objeto\n{"#SILVER_COLOR"}Cambiar el nombre del objeto\n{666666}Editar posición del objeto\n{666666}Texto\n{"#SILVER_COLOR"}Eliminar este objeto");
				}
				case VOBJECT_TYPE_COUNTRY_FLAG, VOBJECT_TYPE_COUNTRY_PANEL:
				{
					if(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_ATTACHED]) format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Guardar este objeto\n{"#SILVER_COLOR"}Cambiar el nombre del objeto\n{"#SILVER_COLOR"}Editar posición del objeto\n{"#SILVER_COLOR"}Eliminar este objeto");
					else format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Colocar este objeto\n{"#SILVER_COLOR"}Cambiar el nombre del objeto\n{666666}Editar posición del objeto\n{"#SILVER_COLOR"}Eliminar este objeto");
				}
			}
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_VOBJECT_OBJECT_CNAME:
		{		
			new dialog[145];
			format(dialog, sizeof dialog, "Nombre actual: \"%s\"\nIndica el nuevo nombre:", VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_NAME]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Objeto - Modificar nombre", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_VOBJECT_OBJECT_COLORS:
		{
			new caption[64];
			format(caption, sizeof caption, "Colores de '%s'", VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_NAME]);
			
			new dialog[45 * MAX_VEHICLE_OBJECTS_INDEXS], line_str[45];
			for(new i = 0; i != MAX_VEHICLE_OBJECTS_INDEXS; i ++)
			{
				if(!VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_COLORS][i]) format(line_str, sizeof line_str, "{FFFFFF}Color #%d (original)\n", i + 1);
				else format(line_str, sizeof line_str, "{%06x}Color #%d\n", ARGBToRGBA(VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_COLORS][i]) >>> 8, i + 1);
				
				strcat(dialog, line_str);
			}
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_VOBJECT_OBJECT_COLOR:
		{
			new caption[64];
			format(caption, sizeof caption, "Color (%d) de '%s'", PLAYER_TEMP[playerid][pt_TUNING_EDIT_COLOR_SLOT], VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_NAME]);
			
			new dialog[145];
			format(dialog, sizeof dialog, 
					
					"\
						Color original\n\
						{%06x}Veh Color 1\n\
						{%06x}Veh Color 2\n\
						{FFFFFF}Otro (ARGB)\n\
					",
					VEHICLE_COLORS[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_COLOR_1] ] >>> 8,
					VEHICLE_COLORS[ GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][gb_vehicle_COLOR_2] ] >>> 8
			);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_VOBJECT_OBJECT_COLOR_HEX:
		{
			new caption[64];
			format(caption, sizeof caption, "Color (%d) de '%s'", PLAYER_TEMP[playerid][pt_TUNING_EDIT_COLOR_SLOT], VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_NAME]);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, caption, "Introduce el color en formato AARRGGBB\nPor ejemplo: FFCCCCCC", "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_VOBJECT_OBJECT_CDELETE:
		{
			new dialog[145];
			format(dialog, sizeof dialog, "¿Estás seguro que quieres borrar el siguiente objeto?\n\t\"%s\"", VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_NAME]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Objeto - Eliminar Objeto", dialog, "Eliminar", "Atrás");
			return 1;
		}
		case DIALOG_VOBJECT_DELETE_ALL: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Objeto - Eliminar todo", "¿Estás seguro de que quiere eliminar todos los objetos de este vehículo?\nEsta opción no se puede deshacer.", "Eliminar", "Atrás");	
		case DIALOG_VOBJECT_TEXT_MENU:
		{
			new caption[64];
			format(caption, sizeof caption, "Texto '%s'", VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_NAME]);

			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, caption, "Editar texto\nCambiar fuente\nTamaño de letra\nNegrita\nColor de fuente", "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_VOBJECT_TEXT_TEXT:
		{
			new dialog[128];
			format(dialog, sizeof dialog, "Texto actual:\n\"%s\"\n\nIndica el nuevo texto (1-32 caracteres)\n", VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_text_TEXT]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Modificar texto", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_VOBJECT_TEXT_FONT:
		{
			new dialog[128];
			format(dialog, sizeof dialog, "Fuente actual:\n\"%s\"\n\nIndica la nueva fuente (1-24 caracteres)\n", VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_text_FONT]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Modificar fuente", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_VOBJECT_TEXT_FONT_SIZE:
		{
			new dialog[128];
			format(dialog, sizeof dialog, "Tamaño actual: %d\n\nIndica el nuevo tamaño\n", VEHICLE_OBJECTS[ PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID] ][ PLAYER_TEMP[playerid][pt_TUNING_EDIT_SLOT] ][vobject_text_FONT_SIZE]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Modificar tamaño de letra", dialog, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_VOBJECT_TEXT_FONT_COLOR:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Selecciona color", DIALOG_CREATE_CREW_COLOR_String, "Continuar", "Atrás");
			return 1;
		}
		case DIAlOG_BUY_MASO:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "{2396FA}Herramientas de minería", "Masa\t\t\t\t\t{16DC45}300${FFFFFF}", "Comprar", "Cerrar");
			return 1;
		}
		case DIALOG_VOBJECT_COUNTRY:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Selecciona país",
				
				"\
					Alemania\n\
					Argentina\n\
					Austria\n\
					Belgica\n\
					Bolivia\n\
					Bulgaria\n\
					Chile\n\
					Colombia\n\
					El Salvador\n\
					España\n\
					Estonia\n\
					Francia\n\
					Guatemala\n\
					Holanda\n\
					Hungrúa\n\
					India\n\
					Indonesia\n\
					Irlanda\n\
					Italia\n\
					Lituania\n\
					Luxemburgo\n\
					Madagascar\n\
					Malta\n\
					Malí\n\
					México\n\
					Nigeria\n\
					Panamá\n\
					Paraguay\n\
					Perú\n\
					Polonia\n\
					Rumanía\n\
					Rusia\n\
					Ucrania\n\
					Venezuela\n\
				"
				
			, "Continuar", "Atrás");
			return 1;
		}
		case DIALOG_PLAYER_CONFIG:
		{
			new dialog[512];
			format(dialog, sizeof dialog, 
				
				"\
					Audios del servidor\t%s\n\
					Barras indicadoras (hud)\t%s\n\
					Ver sanciones, kicks, baneos\t%s\n\
					Número en guía telefónica\t%s\n\
					Sonidos del servidor\t%s\n\
					Mostrar hora del servidor\t%s\n\
					Canal de dudas\t%s\n\
					Verificación en dos pasos\t%s\
				",
					(PI[playerid][ac_CONFIG_AUDIO] ? "Sí" : "No"),
					(PI[playerid][ac_CONFIG_HUD] ? "Sí" : "No"),
					(PI[playerid][ac_CONFIG_ADMIN] ? "Sí" : "No"),
					(PI[playerid][ac_PHONE_VISIBLE_NUMBER] ? "Sí" : "No"),
					(PI[playerid][ac_CONFIG_SOUNDS] ? "Sí" : "No"),
					(PI[playerid][ac_CONFIG_TIME] ? "Sí" : "No"),
					(ACCOUNT_INFO[playerid][ac_DOUBT_CHANNEL] ? "Sí" : "No"),
					(PI[playerid][ac_CONFIG_SECURE_LOGIN] ? "Sí" : "No")
			);
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST, "Panel de configuración del usuario", dialog, "Cambiar", "Cerrar");	
			return 1;
		}
		case DIALOG_POLICE_BYC:
		{
			new caption[128];
			format(caption, sizeof caption, "Placa Nº%d | %c. %s", PI[playerid][ac_PLACA_PD], PLAYER_TEMP[playerid][pt_FIRST_NAME][0], PLAYER_TEMP[playerid][pt_SUB_NAME]);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST, caption, "Ver Últimos registros a BYC\nBuscar BYC de una persona", "Continuar", "Cerrar");	
			return 1;
		}
		case DIALOG_POLICE_BYC_NAME_FIND:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Búsqueda", "Introduce el nombre completo del sospechoso.\n\nUsa el guión bajo para separar nombre y apellido\nPor ejemplo: Nombre_Apellido.", "Buscar", "Cerrar");	
			return 1;
		}
		case DIALOG_POLICE_BYC_LAST_ALL:
		{
			inline OnPoliceHistoryLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
					new dialog[256 * 15], line_str[256], count, start_pos = PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] + 1;
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Sospechoso\t{"#SILVER_COLOR"}Añadido por\t{"#SILVER_COLOR"}Fecha\t{"#SILVER_COLOR"}Razón\n");
					for(new i = 0; i != rows; i ++)
					{
						new id, id_player, text[128], date[24], toname[24], fromname[24], bool:isnull_fromname;
						cache_get_value_name_int(i, "id_player", id);
						cache_get_value_name_int(i, "id_player", id_player);
						cache_get_value_name(i, "text", text);
						cache_get_value_name(i, "date", date);
						cache_get_value_name(i, "toname", toname);
						cache_is_value_name_null(i, "fromname", isnull_fromname);
						if(!isnull_fromname) cache_get_value_name(i, "fromname", fromname);

						format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#SILVER_COLOR"}%s\t{"#SILVER_COLOR"}%s\t{"#SILVER_COLOR"}%s\n", start_pos + i, toname, fromname, date, text);
						strcat(dialog, line_str);
						PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = id;
					}

					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -2;
					strcat(dialog, "{c9c9c9}- Siguiente\n");
					
					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -3;
					strcat(dialog, "{c9c9c9}- Anterior\n");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "BYC", dialog, "Continuar", "Cerrar");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT police_history.*, player.name AS toname, player2.name AS fromname FROM police_history INNER JOIN player ON police_history.id_player = player.id LEFT JOIN player AS player2 ON police_history.by_id = player2.id ORDER BY police_history.date DESC LIMIT %d, %d;", PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT], PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);
			MySQL_TQueryInline(srp_db, using inline OnPoliceHistoryLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_POLICE_BYC_LAST_PLAYER:
		{
			inline OnPoliceHistoryLoad()
			{
				new rows;
				if(cache_get_row_count(rows))
				{
					for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
					new dialog[256 * 15], line_str[256], count, start_pos = PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT] + 1;
					format(dialog, sizeof dialog, "{"#SILVER_COLOR"}Sospechoso\t{"#SILVER_COLOR"}Añadido por\t{"#SILVER_COLOR"}Fecha\t{"#SILVER_COLOR"}Razón\n");
					for(new i = 0; i != rows; i ++)
					{
						new id, id_player, text[128], date[24], toname[24], fromname[24], bool:isnull_fromname;
						cache_get_value_name_int(i, "id", id);
						cache_get_value_name_int(i, "id_player", id_player);
						cache_get_value_name(i, "text", text);
						cache_get_value_name(i, "date", date);
						cache_get_value_name(i, "toname", toname);
						cache_is_value_name_null(i, "fromname", isnull_fromname);
						if(!isnull_fromname) cache_get_value_name(i, "fromname", fromname);

						format(line_str, sizeof line_str, "{"#SILVER_COLOR"}%d. %s\t{"#SILVER_COLOR"}%s\t{"#SILVER_COLOR"}%s\t{"#SILVER_COLOR"}%s\n", start_pos + i, toname, fromname, date, text);
						strcat(dialog, line_str);
						PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = id;
					}

					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -2;
					strcat(dialog, "{c9c9c9}- Siguiente\n");
					
					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][count++] = -3;
					strcat(dialog, "{c9c9c9}- Anterior\n");
					ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, "BYC", dialog, "Continuar", "Cerrar");
				}
			}
			mysql_format(srp_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT police_history.*, player.name AS toname, player2.name AS fromname FROM police_history INNER JOIN player ON police_history.id_player = player.id LEFT JOIN player AS player2 ON police_history.by_id = player2.id WHERE police_history.id_player = %d ORDER BY police_history.date LIMIT %d, %d;", PLAYER_TEMP[playerid][pt_SELECTED_BYC_USER_ID], PLAYER_TEMP[playerid][pt_DIALOG_DB_PAGE] * PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT], PLAYER_TEMP[playerid][pt_DIALOG_DB_LIMIT]);
			MySQL_TQueryInline(srp_db, using inline OnPoliceHistoryLoad, QUERY_BUFFER);
			return 1;
		}
		case DIALOG_POLICE_DELETE_BYC:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "BYC", "¿Borrar esto del historial?", "Borrar", "Cerrar");
			return 1;
		}
		case DIALOG_CHANGE_NAME:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_PASSWORD, "Cambiar nombre", "Introduce tu contraseña actual.", "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_CHANGE_NAME_NAME:
		{
			new dialog[128];
			format(dialog, sizeof dialog, "Introduce tu nuevo nombre.\nEl nombre debe respear el formato Nombre_Apellido.\n\nEl coste por el cambio de nombre es de %d "SERVER_COIN"", CHANGE_NAME_SD_PRICE);
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Cambiar nombre", dialog, "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_CHANGE_EMAIL:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_PASSWORD, "Cambiar correo electrónico", "Introduce tu contraseña actual.", "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_CHANGE_EMAIL_EMAIL:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, "Cambiar correo electrónico", "Introduce tu nuevo correo electrónico.", "Continuar", "Cerrar");
			return 1;
		}
		case DIALOG_PROPERTY_CLOSET:
		{
			for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;

			new max_slots = MAX_CLOSET_SLOTS,
				dialog[128 * (MAX_CLOSET_SLOTS + 2)], line_str[128], listitem;

			format(dialog, sizeof dialog, "Tipo\tDescripción\n");
			
			for(new i = 0; i != max_slots; i ++)
			{
				if(PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][i][property_closet_VALID])
				{
					switch(PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][i][property_closet_TYPE])
					{
						case CLOSET_TYPE_WEAPON:
						{
							format(line_str, sizeof line_str, "%s\t%s (%s balas)\n", GetClosetTypeName(PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][i][property_closet_TYPE]), WEAPON_INFO[ PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][i][property_closet_INT] ][weapon_info_NAME], number_format_thousand(PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][i][property_closet_INT_EXTRA]));
							strcat(dialog, line_str);
						}
						case CLOSET_TYPE_MEDICINES, CLOSET_TYPE_CANNABIS, CLOSET_TYPE_CRACK:
						{
							format(line_str, sizeof line_str, "%s\t%s gramos\n", GetClosetTypeName(PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][i][property_closet_TYPE]), number_format_thousand(PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][i][property_closet_INT]));
							strcat(dialog, line_str);
						}
					}
					
					PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i;
					listitem ++;
				}
			}

			new caption[64];
			format(caption, sizeof caption, "Armario (%d espacios disponibles)", max_slots - listitem);
			
			PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = MAX_CLOSET_SLOTS + 20;
			strcat(dialog, "{c4290d}- Eliminar todo\n"); listitem ++;
			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_TABLIST_HEADERS, caption, dialog, "Continuar", "Cerrar");
		}
		case DIALOG_PROPERTY_CLOSET_DLT_ALL: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Armario - Eliminar todo", "¿Estás seguro de que quiere eliminar todo de su armario?\nEsta opción no se puede deshacer.", "Eliminar", "Cancelar");	
		case DIALOG_PROPERTY_CLOSET_OPTIONS:
		{
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Armario", "{"#SILVER_COLOR"}1. Sacar\n{"#SILVER_COLOR"}2. Eliminar", "Continuar", "Atrás");	
			return 1;
		}
		case DIALOG_PROPERTY_CLOSET_DELETE:
		{
			new dialog[190];
			switch(PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_TYPE])
			{
				case CLOSET_TYPE_WEAPON: format(dialog, sizeof dialog, "Tipo: %s\nDescripción: %s (%s balas)\n\n¿Estás seguro de que quiere eliminar esto de tu maletero?\nEsta opción no se puede deshacer.", GetClosetTypeName(PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_TYPE]), WEAPON_INFO[ PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_INT] ][weapon_info_NAME], number_format_thousand( PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_INT_EXTRA] ));
				case CLOSET_TYPE_MEDICINES, CLOSET_TYPE_CANNABIS, CLOSET_TYPE_CRACK: format(dialog, sizeof dialog, "Tipo: %s\nDescripción: %s gramos\n\n¿Estás seguro de que quiere eliminar esto de tu maletero?\nEsta opción no se puede deshacer.", GetClosetTypeName(PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_TYPE]), number_format_thousand( PROPERTY_CLOSET[ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY] ][ PLAYER_TEMP[playerid][pt_DIALOG_CLOSET_PROPERTY_SLOT] ][property_closet_INT] ));
			}			
			ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_MSGBOX, "Armario - Eliminar", dialog, "Eliminar", "Atrás");
			return 1;
		}
		case DIALOG_PROPERTY_OBJECTS_MENU: return ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_LIST, "Muebles", "1. Editar mueble\n2. Ver muebles guardados\n3. Eliminar todos los muebles", "Continuar", "Cancelar");	
		default: return 0;
	}
	return 1;
}