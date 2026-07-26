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

#if defined macros_included 
	#endinput
#endif
#define macros_included

#define PRESSED(%0) \
	(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

#define RELEASED(%0) \
	(((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))

#define PRESSING(%0,%1) \
	(%0 & (%1))

#define HOLDING(%0) \
	((newkeys & (%0)) == (%0))


#define MAX_BAD_LOGIN_ATTEMPS 3	// maximos intentos de contraseñas erroneas
#define REP_MULTIPLIER 3	//12 rep * nivel para subir, si es nivel 1 necesita 12 rep para subir a nivel 2
#define TIME_FOR_REP 1500000 // 25 minutos para obtener reputacion
#define REP_FOR_PAYDAY 2	// payday cada 2 reputaciones
#define CMD_LOGGIN 1 // log de comandos

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


#define MAX_PLANTS	500

#define MAX_VEHICLE_COMPONENTS	14
#define MIN_VEHICLE_HEALTH		350.0
forward Float:GetDistanceBetweenPoints3D(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2);
forward Float:GetVehicleSpeed(vehicleid);
forward Float:GetVehicleAirSpeed(vehicleid);
forward Float:frandom(Float:max, Float:min, dp);


#define MAX_TIMERS_PER_PLAYER	(22)
#define MAX_LISTITEMS			(35)
/* Timers
0 = Kick
1 = Gangzone daño Rojo
2 = Añadir reputacion
3 = un/freeze player
4 = recuperar vida
5 = restart hambre sed
6 = esperando llamada
7 = esperar arrancar veh
9 = cargando camion
10 = reciclando
11 = talando
12 = plantando
13 = gps mapa
14 = esposar
15 = jail time
16 = health down
17 = msg td
18 = intro 
19 = anti cj
20 = terminó descarga dl
21 = ocultar textdraw login ciudad extra
*/

#define MAX_PLAYER_MAP_GPS_POINTS 20
#define HUD_PTEXTDRAWS_SIZE 4

#define MAX_ROUTES	5
#define MAX_OBJECTS_PER_ROUTE	20

#define MIN_TIME_BETWEEN_DOUBT	60
#define IP_SPAM_MUTE_TIME		5 //minutos


#define INTERVAL_HUNGRY_THIRST_DOWN	2 // 2 segundos de timer
#define THIRST_HOURS_STOPPED	9 // 9 horas parado si agua esta a 100 va a 0
#define HUNGRY_HOURS_STOPPED	12 // 12 horas parado si alimentacion esta a 100 va a 0



#define MAX_INTERIOR_ACTORS 10
enum Interior_Actor_Enum
{
	bool:ia_VALID,
	ia_SKIN,
	Float:ia_POS_X,
	Float:ia_POS_Y,
	Float:ia_POS_Z,
	Float:ia_POS_A,
}

const CRASHER_PACKET = 228;

#define NECESSARY_TIME_BETWEEN_WORKS	600 //segundos

#define lc_limit_X_WEST 4000.0
#define lc_limit_X_EAST 7500.0
#define lc_limit_Y_NORTH 6900.0
#define lc_limit_Y_SOUTH 4150.0

#define MIN_SECONDS_BETWEEN_COMMANDS 1 // Deben pasar al menos 1 segundos entre comando y comando.

#define PlayerTask_Interval_Seconds 10
/*
ANTI AIMBOT no funciona bn, se atraviesan los players
const PLAYER_SYNC = 207;
IPacket:PLAYER_SYNC(playerid, BitStream:bs)
{
    new onFootData[PR_OnFootSync];

    BS_IgnoreBits(bs, 8); 
    BS_ReadOnFootSync(bs, onFootData);

    onFootData[PR_specialAction] = SPECIAL_ACTION_ENTER_VEHICLE;

    BS_SetWriteOffset(bs, 8);
    BS_WriteOnFootSync(bs, onFootData);
    return 1;
}
*/

const RPC_VehicleDestroy = 136;

#define MAX_VEHICLE_OBJECTS_INDEXS	5

#define map_td_X 3.0
#define map_td_Y 134.0
#define map_td_SIZE_X 175.0
#define map_td_SIZE_Y 190.0

#define HUD_TEXTDRAWS_SIZE 7
#define LOAD_SCREEN_TEXTDRAWS_SIZE 4

#define MAX_BANK_TRANSACTIONS_DIALOG	20