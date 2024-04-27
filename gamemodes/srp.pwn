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
#pragma option -d2
#pragma option -(+
#pragma option -;+
#pragma warning disable 239

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
#define DEBUG_MODE 0

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
#define YSI_NO_HEAP_MALLOC
#include <YSI_Coding\y_va>
#include <YSI_Coding\y_stringhash>
#include <YSI_Coding\y_timers>
#include <YSI_Game\y_vehicledata>
#include <YSI_Coding\y_inline>
#include <YSI_Extra\y_inline_mysql>

#include <streamer>
#include <sscanf2>
#include <Pawn.CMD>
#include <Pawn.Regex>
#include <Pawn.RakNet>
#include <interpolate_weather>
#include <mapandreas>

#define PP_SYNTAX_FOR_LIST
#include <PawnPlus>

#define SERVER_VERSION	"2.4.8"
#define VERSION_DATE	"30/08/2020"

/* Config */
#define SERVER_NAME			"Super Roleplay"
#define SERVER_SHORT_NAME	"Super"
#define SERVER_GAMEMODE		"SRP2 "SERVER_VERSION" - Roleplay español"
#define SERVER_LANGUAGE		"Español - Spanish"
#define SERVER_WEBSITE		"super-rp.es"
#define SERVER_HOSTNAME 	"Super Roleplay 2"
#define SERVER_COIN			"Coins"

#define MAX_BAD_LOGIN_ATTEMPS 3	// maximos intentos de contraseñas erroneas
#define REP_MULTIPLIER 3	//12 rep * nivel para subir, si es nivel 1 necesita 12 rep para subir a nivel 2
#define TIME_FOR_REP 1500000 // 25 minutos para obtener reputacion
#define REP_FOR_PAYDAY 2	// payday cada 2 reputaciones
#define CMD_LOGGIN 1 // log de comandos

#define PRIMARY_COLOR "fce679"
#define SILVER_COLOR "d1d1d1"
#define RED_COLOR "FF6633"
#define GREEN_COLOR "98f442"
#define BLUE_COLOR "90C3D4"




/* LIMITES JUGADOR */
	// Normal User
#define MAX_NU_VEHICLES		2
#define MAX_NU_PROPERTIES	1
#define MAX_NU_WORKS		1
#define MAX_NU_TOYS			3
#define MAX_NU_VOBJECTS		3
#define MAX_NU_PROPERTY_OBJECTS 150

	// VIP
#define MAX_SU_VEHICLES		4
#define MAX_SU_PROPERTIES	4
#define MAX_SU_WORKS		3
#define MAX_SU_TOYS			6
#define MAX_SU_VOBJECTS		10
#define MAX_SU_PROPERTY_OBJECTS 250

//SECURE LOGIN
#define SECURE_LOGIN_IP_DAYS 30
#define SECURE_LOGIN_REQUEST_URL "api.super-rp.es/auth/auth-request-code"
#define SECURE_LOGIN_CHECK_URL "api.super-rp.es/auth/auth-check-code"
#define SECURE_LOGIN_TIME 180000 //3 min en logear o kick
#define KEY_F								( 16 )
#define callbackp:%0(%1)						forward %0(%1) ; public %0(%1)
//YOUTUBE
#define SEARCH_YT_PATH "yt.super-rp.es/search"
#define DOWNLOAD_YT_PATH "yt.super-rp.es/download"
#define DOWNLOADED_YT_PATH "https://yt.super-rp.es/mp3/"

// -- headers --
#include "core/global_header"

// Player
#include "core/player/player_account_h"
#include "core/player/player_temp_h"
#include "core/player/player_phone_h"
#include "core/player/player_toys_h"
#include "core/player/player_textdraws_h"
#include "core/player/player_vehicles_h"
#include "core/player/player_weapons_h"

#include "core/dialog/dialogs_id_h"
#include "core/world/zones_header"
#include "core/crew/crew_header"

#include "core/impls"

#include "core/world_time"
#include "core/body_weapons"
#include "core/speedo"
#include "core/taximeter"
#include "core/nears_players_dialog"
#include "core/robbery"
#include "core/hotdogs"
#include "core/air_speedo"
#include "core/notifications"
#include "core/air_veh_fuel"
#include "core/attachobjecttoobjectex"
#include "core/info_vars"
#include "core/jobs/jobs_best_employees"
#include "core/furniture_shop"
#include "core/property_objects"
#include "core/key_press"
#include "core/inventary"
#include "core/injured"
#include "core/jobs/works_tutorials"
#include "core/jobs/warehouse/work_warehouse"
#include "core/jobs/deliveryman/work_deliveryman"
#include "core/drop_weapons"
#include "core/visible_items"
#include "core/boat_repair_points"
#include "core/ac_anticbug"
#include "core/ac_afkpos"
#include "core/shield_weapon"
#include "core/taser_gun_weapon"
#include "core/police_pdoor_kick"
#include "core/graffiti"


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
