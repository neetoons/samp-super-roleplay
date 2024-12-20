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
#if defined police_h_included 
	#endinput
#endif
#define police_h_included
 
/* Rangos Policias */
enum enum_POLICE_SHOP_WEAPONS
{
	police_shop_RANK,
	police_shop_WEAPON_ID,
	police_shop_WEAPON_PRICE,
}
new POLICE_SHOP_WEAPONS[][enum_POLICE_SHOP_WEAPONS] = 
{
	{1, 3, 50},		
	{1, 22, 300},
	{2, 23, 500},
	{2, 17, 50},
	{2, 24, 600},
	{2, 25, 800},
	{2, 27, 1400},
	{2, 28, 1250},
	{2, 29, 1750},
	{2, 30, 1800},
	{2, 31, 1900},
	{2, 33, 900},
	{3, 34, 3500},
	{3, 48, 750},
	{9, 47, 1000}
};


enum enum_POLICE_SKINS
{
	police_skin_TYPE,
	police_skin_SKIN,
	police_skin_RANK,
}
new POLICE_SKINS[][enum_POLICE_SKINS] =
{
	{SEX_MALE, 71, 1},
	{SEX_MALE, 280, 2},
	{SEX_MALE, 265, 2},
	{SEX_MALE, 266, 2},
	{SEX_MALE, 267, 2},
	{SEX_MALE, 284, 4},
	{SEX_MALE, 281, 6},
	{SEX_MALE, 2, 6},
	{SEX_MALE, 46, 6},
	{SEX_MALE, 60, 6},
	{SEX_MALE, 186, 6},
	{SEX_MALE, 285, 9},
	{SEX_MALE, 282, 10},
	{SEX_MALE, 283, 11},
	{SEX_MALE, 286, 11},
	{SEX_MALE, 288, 12},
	
	{SEX_FEMALE, 306, 1},
	{SEX_FEMALE, 307, 1},
	{SEX_FEMALE, 309, 1},
	{SEX_FEMALE, 190, 6},
	{SEX_FEMALE, 192, 6},
	{SEX_FEMALE, 193, 6}
};

new POLICE_RANKS[][] =
{
	/* 0 */ "Civil",
	/* 1 */ "Cadete",
	/* 2 */ "Oficial I",	
	/* 3 */ "Oficial II",	
	/* 4 */ "Oficial III",	
	/* 5 */ "Oficial III+",
	/* 6 */ "Detective I",
	/* 7 */ "Detective II",
	/* 8 */ "Detective III",
	/* 9 */ "Sargento I",
	/* 10 */ "Sargento II",
	/* 11 */ "Capitán",
	/* 12 */ "Jefe Policía"
};

#define MAX_POLICE_OBJECTS	300
enum enum_POLICE_OBJECTS
{
	bool:police_object_VALID,
	police_object_USER[24],
	police_object_OBJECT_ID,
}
new POLICE_OBJECTS[MAX_POLICE_OBJECTS][enum_POLICE_OBJECTS];

enum enum_POLICE_GARAGE_DOORS
{
	police_gdoor_MODELID,
	Float:police_gdoor_CLOSED_X,
	Float:police_gdoor_CLOSED_Y,
	Float:police_gdoor_CLOSED_Z,
	Float:police_gdoor_CLOSED_RX,
	Float:police_gdoor_CLOSED_RY,
	Float:police_gdoor_CLOSED_RZ,
	Float:police_gdoor_OPENED_X,
	Float:police_gdoor_OPENED_Y,
	Float:police_gdoor_OPENED_Z,
	Float:police_gdoor_OPENED_RX,
	Float:police_gdoor_OPENED_RY,
	Float:police_gdoor_OPENED_RZ,
	Float:police_gdoor_MOVE_SPEED,
	bool:police_gdoor_CLOSING,
	police_gdoor_OBJECT_ID,
	police_gdoor_TIMER,
}
new POLICE_GARAGE_DOORS[][enum_POLICE_GARAGE_DOORS] =
{
	{3055, 1588.48853, -1637.85852, 14.57500, 0.0, 0.0, 0.0, 1588.48853, -1637.85852, 16.45880, 90.0, 0.0, 0.0, 1.3, false, INVALID_STREAMER_ID, -1},
	{3055, 2294.04102, 2498.81396, 4.47113,   0.00000, 0.00000, 90.00000, 2294.0410, 2498.8140, 9.0059, 0.0, 0.0, 90.0, 1.6, false, INVALID_STREAMER_ID, -1},
	{3055, 2335.13086, 2443.66895, 6.97370,   0.00000, 0.00000, 60.00000, 2335.1309, 2443.6689, 11.6876, 0.0, 0.0, 60.0, 1.6, false, INVALID_STREAMER_ID, -1},
	{10184, -1631.69556, 688.56641, 8.70960,   0.00000, 0.00000, 90.00000, -1631.6956, 688.5664, 13.2064, 0.0, 0.0, 90.0, 1.6, false, INVALID_STREAMER_ID, -1}
};

enum enum_POLICE_DOORS
{
	police_door_MODELID,
	Float:police_door_X,
	Float:police_door_Y,
	Float:police_door_Z,
	Float:police_door_RZ,
	police_door_INTERIOR,
	police_door_WORLD,
	bool:police_door_CLOSED,
	police_door_OBJECT_ID,
	police_door_TIMER,
}
new POLICE_DOORS[][enum_POLICE_DOORS] =
{
	{1495, 1582.6055, -1638.0713, 12.3384, 0.0, 0, 0, true, INVALID_STREAMER_ID, -1},//lspd
	{1495, 245.62910, 72.53300, 1002.54999, 0.0, 6, 0, true, INVALID_STREAMER_ID, -1},//lspd
	{1495, 250.65849, 62.58540, 1002.54999, 90.0, 6, 0, true, INVALID_STREAMER_ID, -1},//lspd
	{1495, 226.16409, 71.01760, 1003.95050, 90.0, 6, 0, true, INVALID_STREAMER_ID, -1},//lspd
	{1497, 222.14630, 70.52970, 1003.95050, -90.0, 6, 0, true, INVALID_STREAMER_ID, -1},//lspd
	{1495, -1619.70764, 688.41058, 6.17220, 180.00000, 0, 0, true, INVALID_STREAMER_ID, -1},//sfpd
	{1495, 239.58211, 116.84430, 1002.20319, 90.00000, 10, 0, true, INVALID_STREAMER_ID, -1},//sfpd
	{1495, 253.19209, 108.35700, 1002.20319, 90.00000, 10, 0, true, INVALID_STREAMER_ID, -1},//sfpd
	{1497, 266.59210, 112.56570, 1003.60992, 180.0, 10, 0, true, INVALID_STREAMER_ID, -1},//sfpd
	{1495, 228.27240, 150.46510, 1002.01813, 90.0, 3, 0, true, INVALID_STREAMER_ID, -1},//lvpd
	{1495, 228.27240, 160.45010, 1002.01813, 90.0, 3, 0, true, INVALID_STREAMER_ID, -1},//lvpd
	{1495, 232.21030, 169.83350, 1002.01813, 180.0, 3, 0, true, INVALID_STREAMER_ID, -1},//lvpd
	{1495, 2294.03516, 2492.91528, 2.26740, 90.0, 0, 0, true, INVALID_STREAMER_ID, -1},//lvpd
	{1497, 221.95570, 181.21680, 1002.02142, 0.0, 3, 0, true, INVALID_STREAMER_ID, -1}//lvpd
};