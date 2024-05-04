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
#if defined crew_h_included 
	#endinput
#endif
#define crew_h_included

// Crews
#define MAX_CREWS		50
#define MAX_CREW_RANKS	10
#define MAX_CREW_MEMBERS 80

enum enum_CREW_INFO
{
	bool:crew_VALID,
	crew_ID,
	crew_NAME[32],
	crew_COLOR,
	
	bool:crew_FIGHTING,
	crew_LAST_ATTACK,
	crew_LAST_ROBBERY,
	
	crew_MEMBERS,
	crew_ONLINE_MEMBERS,
}
new CREW_INFO[MAX_CREWS][enum_CREW_INFO];

#define MAX_TERRITORIES	100
enum enum_TERRITORIES
{
	bool:territory_VALID,
	territory_ID,
	territory_NAME[32],
	bool:territory_OCCUPIED,
	territory_CREW_ID,
	territory_CREW_INDEX,
	territory_COLOR,
	Float:territory_MIN_X,
	Float:territory_MIN_Y,
	Float:territory_MIN_Z,
	Float:territory_MAX_X,
	Float:territory_MAX_Y,
	Float:territory_MAX_Z,
	
	territory_AREA,
	territory_GANG_ZONE,
	
	bool:territory_WAR,
	territory_ATTACKER_CREW_INDEX,
	territory_WAR_TIME_LEFT,
	Text:territory_TEXTDRAW,
	territory_TIMER,
	territory_LAST_ATTACK,
}
new TERRITORIES[MAX_TERRITORIES][enum_TERRITORIES];


/* 
Permisos de rangos

0 - cambiar nombre
1 - invitar
2 - echar
3 - cambiar rango de miembros
4 - crear rangos y modificar rangos
5 - poner casas personales a casas de banda
6 - vender casas de banda
7 - cambiar color de la banda
8 - abandonar territorio
9 - destruir cbanda
*/

//permisos
enum
{
	CREW_RANK_CHANGE_NAME,
	CREW_RANK_INVITE,
	CREW_RANK_CAST_MEMBERS,
	CREW_RANK_MODIFY_MEMBERS,
	CREW_RANK_MODIFY_RANKS,
	CREW_RANK_ADD_PROPERTIES,
	CREW_RANK_DELETE_PROPERTIES,
	CREW_RANK_CHANGE_COLOR,
	CREW_RANK_LEAVE_TERRITORY,
	CREW_RANK_DELETE,
	CREW_RANK_GRAFFITI,
	
	CREW_RANK_SIZE, // dejar siempre el ultimo
}

new CREW_RANKS_PERMISSIONS[][] =
{
	"cambiar nombre de la banda",	//0
	"invitar jugadores",			//1
	"echar miembros",				//2
	"cambiar rangos de miembros",	//3
	"crear rangos o modificarlos",	//4
	"poner propiedades de banda",	//5
	"liberar propiedades de banda",	//6
	"cambiar color de la banda",	//7
	"abandonar territorios",		//8
	"eliminar banda",				//9
	"crear y eliminar graffitis"	//10
};

enum enum_CREW_RANK_INFO
{
	bool:crew_rank_VALID,
	crew_rank_ID,
	crew_rank_NAME[32],
	crew_rank_PERMISSION[CREW_RANK_SIZE],
}
new CREW_RANK_INFO[MAX_CREWS][MAX_CREW_RANKS][enum_CREW_RANK_INFO];

forward OnTerritoriesLoaded();
forward OnCrewDeleted(crewId);
forward OnCrewCaptureTerritory(crewId, crewIndex, territoryId, territoryIndex);
forward OnCrewLeftTerritory(crewId, crewIndex, territoryId, territoryIndex);
