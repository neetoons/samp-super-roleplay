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

// Compilación
#pragma option -(+
#pragma option -;+
#pragma warning disable 239, 203

// Anti-DeAMX creado por Daniel-Cortez
@___ww___@();
@___ww___@()
{
    #emit    stack    0x7FFFFFFF
    #emit    inc.s    cellmax

    static const ___[][] = {"sls", "422"};

    #emit    retn
    #emit    load.s.pri    ___
    #emit    proc
    #emit    proc
    #emit    fill    cellmax
    #emit    proc
    #emit    stack    1
    #emit    stor.alt    ___
    #emit    strb.i    2
    #emit    switch    0
    #emit    retn
L1:
    #emit    jump    L1
    #emit    zero    cellmin
}

/*
 * Modos de debug:
 *  - 0: No saldrá ningún mensaje de debug ni en la consola ni en el juego.
 *  - 1: Saldrán mensajes de debug solamente en la consola.
 *  - 2: Saldrán mensajes de debug en la consola y en el juego.
*/
#define DEBUG_MODE 2

#if DEBUG_MODE != 0
	#pragma option -d3
	#include <crashdetect>
#else
	#pragma option -O1
#endif

#include <a_samp>
#include <a_http>
//MODIFICAR SEGUN SLOTS DEL SERVIDOR
#undef MAX_PLAYERS
#define MAX_PLAYERS 500

#include <a_mysql>

// YSI-Includes
#define CGEN_MEMORY 20000
#define YSI_NO_HEAP_MALLOC
//#define YSI_NO_MODE_CACHE
//#define YSI_NO_OPTIMISATION_MESSAGE
#include <YSI_Coding\y_va>
#include <YSI_Coding\y_stringhash>
#include <YSI_Coding\y_timers>
#include <YSI_Game\y_vehicledata>
#include <YSI_Coding\y_inline>
#include <YSI_Extra\y_inline_mysql>

#include <logger>
#include <streamer>
#include <sscanf2>
#include <Pawn.CMD>
#include <Pawn.Regex>
#include <Pawn.RakNet>
#include <mapandreas>
#include <interpolate_weather>
#define PP_SYNTAX_FOR_LIST
#include <PawnPlus>

/*

    8888888b.            .d888 d8b                            
    888  "Y88b          d88P"  Y8P                            
    888    888          888                                   
    888    888  .d88b.  888888 888 88888b.   .d88b.  .d8888b  
    888    888 d8P  Y8b 888    888 888 "88b d8P  Y8b 88K      
    888    888 88888888 888    888 888  888 88888888 "Y8888b. 
    888  .d88P Y8b.     888    888 888  888 Y8b.          X88 
    8888888P"   "Y8888  888    888 888  888  "Y8888   88888P' 

*/

#define SERVER_VERSION	"2.4.8"
#define VERSION_DATE	"30/08/2020"

/* Config */
#define SERVER_NAME			"Super Roleplay"
#define SERVER_SHORT_NAME	"Super"
#define SERVER_GAMEMODE		"SRP2 "SERVER_VERSION" - Roleplay español"
#define SERVER_LANGUAGE		"Español - Spanish"
#define SERVER_WEBSITE		"yourwebsite"
#define SERVER_HOSTNAME 	"Super Roleplay 2"
#define SERVER_COIN			"Coins"

#define PRIMARY_COLOR "fce679"
#define SILVER_COLOR "d1d1d1"
#define RED_COLOR "FF6633"
#define GREEN_COLOR "98f442"
#define BLUE_COLOR "90C3D4"

//SECURE LOGIN
#define SECURE_LOGIN_IP_DAYS 30
#define SECURE_LOGIN_REQUEST_URL "api.yourwebsite/auth/auth-request-code"
#define SECURE_LOGIN_CHECK_URL "api.yourwebsite/auth/auth-check-code"
#define SECURE_LOGIN_TIME 180000 //3 min en logear o kick
#define KEY_F								( 16 )
// basura innecesaria, se espera por cambio
#define callbackp:%0(%1)						forward %0(%1) ; public %0(%1)
//YOUTUBE
#define SEARCH_YT_PATH "yt.yourwebsite/search"
#define DOWNLOAD_YT_PATH "yt.yourwebsite/download"
#define DOWNLOADED_YT_PATH "https://yt.yourwebsite/mp3/"

 /*

    888    888                        888                  
    888    888                        888                  
    888    888                        888                  
    8888888888  .d88b.   8888b.   .d88888  .d88b.  888d888 
    888    888 d8P  Y8b     "88b d88" 888 d8P  Y8b 888P"   
    888    888 88888888 .d888888 888  888 88888888 888     
    888    888 Y8b.     888  888 Y88b 888 Y8b.     888     
    888    888  "Y8888  "Y888888  "Y88888  "Y8888  888     

*/                                          

#include "core/misc/macros_h"
#include "core/db/db_h"


// Admin 
#include "core/administration/admin_levels_h"

//anticheat
#include "core/anticheat/anticheat_h"

// world 
#include "core/world/enter_exits_h"
#include "core/world/extra_h"
#include "core/world/world_h"

//Bank
#include "core/bank/atm_h"


// Player
#include "core/player/player_h" 
#include "core/player/player_account_h" // se tiene que modular
#include "core/player/player_temp_h"
#include "core/player/player_phone_h"
#include "core/player/player_toys_h"
#include "core/player/player_character_h"
#include "core/player/player_textdraws_h"
#include "core/player/player_vehicles_h"
#include "core/player/player_weapons_h"
#include "core/player/player_gps_h"
#include "core/player/player_skins_h"

#include "core/door/door_h"
#include "core/properties/hospital_h"
#include "core/properties/fuel_station_h"
#include "core/vehicles/tunning_h"
#include "core/global/textdraws_h"
#include "core/main/global_h"
#include "core/police/police_h"

//jobs
#include "core/jobs/jobs_h"
#include "core/jobs/farmer/farmer_h"

#include "core/vehicles/vehicles_h"
#include "core/dialog/dialogs_id_h"
#include "core/world/zones_h"
#include "core/crew/crew_h"
#include "core/furniture/properties_h"
#include "core/key_areas/key_areas_h"
#include "core/weapons/weapons_h"
#include "core/directives/rules_h" 
/*

8888888888                                  
888                                         
888                                         
8888888 888  888 88888b.   .d8888b .d8888b  
888     888  888 888 "88b d88P"    88K      
888     888  888 888  888 888      "Y8888b. 
888     Y88b 888 888  888 Y88b.         X88 
888      "Y88888 888  888  "Y8888P  88888P' 

*/

#include "core/color/pallete" 

// se tiene que modular
#include "core/main/callbacks"
#include "core/main/functions"

/*

 .d8888b.                     888          
d88P  Y88b                    888          
888    888                    888          
888        88888b.d88b.   .d88888 .d8888b  
888        888 "888 "88b d88" 888 88K      
888    888 888  888  888 888  888 "Y8888b. 
Y88b  d88P 888  888  888 Y88b 888      X88 
 "Y8888P"  888  888  888  "Y88888  88888P' 
                                           
*/

// Administration

#include "core/administration/admin/admin_cmds"
#include "core/administration/developer/developer_cmds"
#include "core/administration/moderator/moderator_cmds"
#include "core/administration/mod_helper/mod_helper_cmds"
#include "core/administration/helper/helper_cmds"

#include "core/main/commands" // se tiene que modular

/*
    888b     d888          d8b          
    8888b   d8888          Y8P          
    88888b.d88888                       
    888Y88888P888  8888b.  888 88888b.  
    888 Y888P 888     "88b 888 888 "88b 
    888  Y8P  888 .d888888 888 888  888 
    888   "   888 888  888 888 888  888 
    888       888 "Y888888 888 888  888 
*/

#include "core/dialog/showDialog" // se tiene que modular
#include "core/dialog/response" // se tiene que modular


/*

888b     d888               888          888                   
8888b   d8888               888          888                   
88888b.d88888               888          888                   
888Y88888P888  .d88b.   .d88888 888  888 888  .d88b.  .d8888b  
888 Y888P 888 d88""88b d88" 888 888  888 888 d8P  Y8b 88K      
888  Y8P  888 888  888 888  888 888  888 888 88888888 "Y8888b. 
888   "   888 Y88..88P Y88b 888 Y88b 888 888 Y8b.          X88 
888       888  "Y88P"   "Y88888  "Y88888 888  "Y8888   88888P' 
                                                               

*/

// se tiene que modular
#include "core/sub_modules/notifications"
#include "core/sub_modules/graffiti"
#include "core/sub_modules/taximeter"
#include "core/sub_modules/world_time"
#include "core/sub_modules/body_weapons"
#include "core/sub_modules/speedo"
#include "core/sub_modules/nears_players_dialog"
#include "core/sub_modules/robbery"
#include "core/sub_modules/hotdogs"
#include "core/sub_modules/air_speedo"
#include "core/sub_modules/notifications"
#include "core/sub_modules/air_veh_fuel"
#include "core/sub_modules/attachobjecttoobjectex"
#include "core/sub_modules/info_vars"
#include "core/sub_modules/furniture_shop"
#include "core/sub_modules/property_objects"
#include "core/sub_modules/key_press"
#include "core/sub_modules/inventary"
#include "core/sub_modules/injured"
#include "core/sub_modules/drop_weapons"
#include "core/sub_modules/visible_items"
#include "core/sub_modules/boat_repair_points"
#include "core/sub_modules/ac_anticbug"
#include "core/sub_modules/ac_afkpos"
#include "core/sub_modules/shield_weapon"
#include "core/sub_modules/taser_gun_weapon"
#include "core/sub_modules/police_pdoor_kick"

//jobs
#include "core/jobs/jobs_best_employees"
#include "core/jobs/works_tutorials"
#include "core/jobs/warehouse/work_warehouse"
#include "core/jobs/deliveryman/work_deliveryman"

public OnGameModeInit()
{
	SetGameModeText(SERVER_GAMEMODE);
    SendRconCommand("hostname "SERVER_HOSTNAME"...");
    SendRconCommand("language "SERVER_LANGUAGE"");
	SendRconCommand("weburl "SERVER_WEBSITE"");
	SendRconCommand("minconnectiontime 0");
    SendRconCommand("ackslimit 8000");
    SendRconCommand("messageslimit 100");
    SendRconCommand("conncookies 1");
	SendRconCommand("cookielogging 0");
	SendRconCommand("chatlogging 0");
	SendRconCommand("sleep 1");
	SetTimer("minconnecttime", 60000, 0);
	SetTimer("BajarSegundos", 1000, true );


 	for( new i; i < sizeof MinaInfo; i++ )
		{
			MinaInfo[i][w_zone] = CreateDynamicSphere(MinaInfo[i][RocaX], MinaInfo[i][RocaY], MinaInfo[i][RocaZ], 1.5 );
			MinaInfo[i][w_object] = CreateDynamicObject(1207, MinaInfo[i][RocaX], MinaInfo[i][RocaY], MinaInfo[i][RocaZ], MinaInfo[i][RocaRX], MinaInfo[i][RocaRY], MinaInfo[i][RocaRZ]);
			SetMinaRoca(MinaInfo[i][w_mina],i);
			SetTexuraRoca(MinaInfo[i][w_object],MinaInfo[i][w_tipo]);
			MinaInfo[i][w_use] =
			MinaInfo[i][w_drop] = false;
			MinaInfo[i][w_count] = 100;
			MinaInfo[i][w_time] = 0;
		}

	ConnectDatabase();

	SetWorldMinutesForDay(180); // 3 horas reales = 24 horas en juego
	DisableInteriorEnterExits();
	ShowPlayerMarkers(PLAYER_MARKERS_MODE_GLOBAL);
	EnableStuntBonusForAll(false);
	ManualVehicleEngineAndLights();
	FormatDialogStrings();
	UsePlayerPedAnims();
	MapAndreas_Init(MAP_ANDREAS_MODE_FULL);
	return 1;
}

public OnGameModeExit()
{
	print("\n\n\n----> Saliendo\n\n");
	
	new date[24];
	getDateTime(date);
	for(new i, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i) && PLAYER_TEMP[i][pt_USER_LOGGED])
		{
			format(ACCOUNT_INFO[i][ac_LAST_CONNECTION], 24, "%s", date);
			SavePlayerData(i);
			SavePlayerVehicles(i, false);
			SavePlayerToys(i);
			SavePlayerWeaponsData(i);
			SavePlayerWorkLevels(i);
			PLAYER_TEMP[i][pt_SAVE_ACCOUNT_TIME] = gettime();
		}
	}
	mysql_close(srp_db);
	/*
     * Pendiente por borrar o cambiar, esto me parece que es innecesario
	for(new i, j = GetPlayerPoolSize(); i <= j; i++) // Cerrar conexión a todos al cerrar el server
	{
		if(IsPlayerConnected(i))
		{
				new BitStream:bs = BS_New();

				BS_WriteValue(
					bs,
					PR_UINT8, 33
				);

				BS_Send(bs, i);
				BS_Delete(bs);
		}
	}
	*/
	return 1;
}
