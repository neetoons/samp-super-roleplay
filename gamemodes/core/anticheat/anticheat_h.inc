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

#if defined anticheat_header_included 
    #endinput
#endif
#define anticheat_header_included

enum
{
	CHEAT_WEAPON,
	CHEAT_AMMO,
	CHEAT_CAR_MOD,
	CHEAT_VEHICLE_SPEED_HACK,
	CHEAT_POS,
	CHEAT_TEXT_SPAMMER,
	CHEAT_COMMAND_SPAMMER,
	CHEAT_DEATH_SPAMMER,
	CHEAT_STATE_SPAMMER,
	CHEAT_VEHICLE_ENTER_SPAMMER,
	CHEAT_JETPACK,
	CHEAT_PLAYER_SPEED,
	CHEAT_PLAYER_HEALTH,
	CHEAT_PLAYER_ARMOUR,
	CHEAT_VEHICLE_HEALTH,
	CHEAT_PLAYER_VEHICLE_WORK, //Entra a un vehiculo que no es de su trabajo
	CHEAT_PLAYER_VEHICLE_DOORS,
	CHEAT_VEHICLE_NOFUEL,
	CHEAT_SPECTATE,
	CHEAT_UNDETECTED_WEAPON,
	CHEAT_DRIVE_BY,
	CHEAT_UNOCCUPIED_VEHICLE_TP,
	CHEAT_HIGH_POS,
	CHEAT_CAR_JACKING,
	CHEAT_FAKE_TRAILER_SYNC,
	CHEAT_DESTROY_VEHICLE,
	CHEAT_CBUG,
	CHEAT_AFKPOS
}

enum e_ac_Info
{
	ac_Name[24],
	bool:ac_Enabled,
	bool:ac_Kick, // 1 = kickea, 0 = avisa a admins
	ac_Detections, //enviar aviso cuando llege a este num en los seg de abajo
	ac_Interval,
}
new ac_Info[][e_ac_Info] = /* Valores por defecto si no se carga de AC.txt */
{
	{"armas",					true, true,		1, 0},
	{"municion",				true, true,		1, 0},
	{"mod car",					true, true,		1, 0},
	{"vehicle speed",			true, true,		1, 0},
	{"pos",						true, false,	3, 10},
	{"text spammer",			true, true,		10, 5},
	{"command spammer",			true, true,		5, 5},
	{"death spammer",			true, false,	1, 0},
	{"state change spammer",	true, false,	1, 0},
	{"vehicle enter spammer",	true, false,	1, 0},
	{"jetpack",					true, true,		1, 0},
	{"player speed",			true, true,		1, 0},
	{"player health",			true, true,		1, 0},
	{"player armour",			true, true,		1, 0},
	{"vehicle health",			true, true,		1, 0},
	{"pwork != vwork",			true, true,		1, 0},
	{"enter closed vehicle",	true, false,	1, 0},
	{"vehicle nofuel cs",		true, false,	50, 10},
	{"player spectate",			true, false,	1, 0},
	{"pweapon != sweapon",		true, false,	1, 0},
	{"drive by",				true, false,	1, 0},
	{"unoccupied veh tp",		true, true,		1, 0},
	{"high pos",				true, true,		1, 0},
	{"car jacking",				true, true,		1, 0},
	{"fake trailer sync",		true, true,		1, 0},
	{"destroy vehicle",			true, false,	1, 0},
	{"cbug",					true, false,	1, 0},
	{"afk pos",					true, true,		1, 0}
};

enum e_PLAYER_AC_INFO
{
	p_ac_info_DETECTIONS,
	p_ac_info_LAST_DETECTION,
	p_ac_info_IMMUNITY, //gettime + seconds
}
new PLAYER_AC_INFO[MAX_PLAYERS][sizeof(ac_Info)][e_PLAYER_AC_INFO];