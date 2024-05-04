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

#if defined farmer_header_included 
    #endinput
#endif
#define farmer_header_included

enum
{
	TYPE_LEGAL,
	TYPE_ILLEGAL,
}

// AGRICULTOR
enum
{
	PLANT_TYPE_MEDICINE, 	// VIDA
	PLANT_TYPE_CANNABIS,	// YOKSETIOXD
	PLANT_TYPE_CRACK,		//CHALECO
}

enum seed_info_enum
{
	seed_info_TYPE,
	seed_info_PLANT_TYPE,
	seed_info_EXP,
	seed_info_NAME[24],
	seed_info_PRICE,
}
new seed_info[][seed_info_enum] =
{
	{TYPE_LEGAL, PLANT_TYPE_MEDICINE, 0, "Medicina", 50},
	{TYPE_ILLEGAL, PLANT_TYPE_CANNABIS, 100, "Marihuana", 150},
	{TYPE_ILLEGAL, PLANT_TYPE_CRACK, 200, "Coca", 300}
};

enum plant_info_enum
{
	plant_info_SEEDS,
	plant_info_NAME[24],
}
new plant_info[][plant_info_enum] =
{
	{2, "Medicamento"},
	{4, "Marihuana"},
	{10, "Crack"}
};


enum PLANTS_ENUM
{
	bool:plant_VALID,
	bool:plant_GROWING,
	bool:plant_INVISIBLE,
	Float:plant_GROWING_PROGRESS,
	plant_TYPE,
	plant_PLANTED_BY_ACCOUNT_ID,
	plant_PLANTED_BY_NAME[24],
	plant_OBJECT_ID,
	Text3D:plant_LABEL_ID,
	plant_EXPIRE_TIME,
	plant_TIMER,
	plant_IMMUNITY,
}
new PLANTS[MAX_PLANTS][PLANTS_ENUM];

enum
{
	OBJECT_TYPE_NONE,
	OBJECT_TYPE_PLANT,
	OBJECT_TYPE_POLICE,
	OBJECT_TYPE_FURNITURE,
	OBJECT_TYPE_WH_BOX,
	OBJECT_TYPE_GRAFFITI,
	OBJECT_TYPE_TEST
}

GetPlayerPlantedPlants(playerid)
{
	new plants;
	
	for(new i = 0; i != MAX_PLANTS; i ++)
	{
		if(!PLANTS[i][plant_VALID]) continue;
		if(PLANTS[i][plant_PLANTED_BY_ACCOUNT_ID] == ACCOUNT_INFO[playerid][ac_ID]) plants ++;
	}
	return plants;
}