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
#if defined properties_h_included 
	#endinput
#endif
#define properties_h_included

#define MAX_PROPERTIES	2000
enum Property_Info_Enum
{
	property_VALID,
	property_ID,
	Float:property_EXT_X,
	Float:property_EXT_Y,
	Float:property_EXT_Z,
	Float:property_EXT_ANGLE,
	property_EXT_INTERIOR,
	property_EXT_FREEZE,
	property_ID_INTERIOR,
	
	bool:property_SOLD,
	bool:property_CREW,
	property_OWNER_ID,
	property_CREW_ID,
	property_NAME[24],
	property_PRICE,
	property_LEVEL,
	property_EXTRA,
	property_VIP_LEVEL,
	
	Text3D:property_EXT_LABEL_ID,
	Text3D:property_INT_LABEL_ID,
	property_EXT_PICKUP_ID,
	property_INT_PICKUP_ID,
	
	property_PRICE_BASE,
	bool:property_DIS_DEFAULT_INTERIOR,

	bool:property_POLICE_FORCING,
	bool:property_POLICE_FORCED,
	property_POLICE_FORCE_TIMER
}
new PROPERTY_INFO[MAX_PROPERTIES][Property_Info_Enum];


/* PROPERTY CONSTRUCTOR */
#define PROPERTY_EMPTY_INTERIOR_Z_OFFSET 20.0
new TOTAL_PROPERTIES_LOADED;
enum PROPERTY_INTERIOR_ENUM
{
	property_creator_LEVEL,
	property_creator_PRICE,
	Float:property_INT_X,
	Float:property_INT_Y,
	Float:property_INT_Z,
	Float:property_INT_ANGLE,
	property_INT_INTERIOR,
	bool:property_INT_FREEZE,
	bool:property_EMPTY_INTERIOR
}
new PROPERTY_INTERIORS[][PROPERTY_INTERIOR_ENUM] =
{
	{6, 180000, 140.249008, 1366.197509, 1083.859375, 0.0, 5, false, true},
	{6, 160000, 2324.530029, -1149.540039, 1050.710083, 0.0, 12, false, true},
	{6, 270000, 225.679992, 1021.450012, 1084.017700, 0.0, 7, false, true},
	{6, 190000, 234.190002, 1063.729980, 1084.212280, 0.0, 6, false, true},
	{6, 120000, 226.300003, 1114.239990, 1080.992919, 270.0, 5, false, true},
	{6, 180000, 2317.889892, -1026.760009, 1050.217773, 0.0, 9, false, true},
	{6, 120000, 2495.979980, -1692.079956, 1014.742187, 180.0, 3, false, true},
	{6, 90000,  2196.626464, -1204.321411, 1049.023437, 90.0, 6, false, true},
	{6, 105000, 2270.143554, -1210.490600, 1047.562500, 90.0, 10, false, true},
	{4, 90000,  2365.199951, -1135.595825, 1050.882568, 0.0, 8, false, true},
	{3, 75000,  2237.590087, -1081.640014, 1049.023437, 0.0, 2, false, true},
	{4, 65000,  2468.840087, -1698.239990, 1013.507812, 90.0, 2, false, false},
	{3, 45000,  2282.928955, -1140.004516, 1050.898437, 0.0, 11, false, true},
	{4, 50000,  2308.775878, -1212.731689, 1049.023437, 0.0, 6, false, true},
	{6, 135000, 2233.681396, -1114.973510, 1050.882812, 0.0, 5, false, true},
	{6, 230000, 2218.162597, -1076.148315, 1050.484375, 90.0, 1, false, true},
	{6, 220000, 266.654327, 304.961791, 999.148437, 270.0, 2, false, true},
	{6, 30000,  243.720001, 304.910003, 999.148437, 270.0, 1, false, true},
	{6, 65000,  2259.440673, -1135.905883, 1050.640258, 270.0, 10, false, true},
	{12, 3000000, 1260.910034, -785.320068, 1091.906250, 280.0, 5, false, false}
};

enum PROPERTY_CLOSET_POS_ENUM
{
	Float:property_closet_X,
	Float:property_closet_Y,
	Float:property_closet_Z
}
new PROPERTY_CLOSET_POS[][PROPERTY_CLOSET_POS_ENUM] = //de momento posicion de la puerte se cambiara porxiamn
{
	{145.103408, 1384.249877, 1088.367187},
	{2337.974609, -1141.684082, 1054.304687},
	{243.130554, 1046.244384, 1084.000000},
	{235.600509, 1079.485351, 1087.812622},
	{239.347656, 1116.808593, 1084.992187},
	{2326.974365, -1015.419189, 1054.711059},
	{2492.362060, -1708.568969, 1018.336791},
	{2198.682128, -1219.331787, 1049.023437},
	{2258.639892, -1220.260253, 1049.023437},
	{2363.767333, -1127.419433, 1050.882568},
	{2235.885986, -1073.944335, 1049.023437},
	{2450.338867, -1706.199340, 1013.507812},
	{2286.247314, -1137.693359, 1050.898437},
	{2319.417480, -1212.880981, 1049.023437},
	{2231.736328, -1112.198730, 1050.882812},
	{2215.857910, -1074.698974, 1050.484375},
	{270.320709, 303.413482, 999.155761},
	{248.817687, 303.795257, 999.148437},
	{2263.338378, -1132.665161, 1050.632812},
	{1271.444091, -822.371704, 1089.937500}
};

enum
{
	CLOSET_TYPE_WEAPON,
	CLOSET_TYPE_MEDICINES,
	CLOSET_TYPE_CANNABIS,
	CLOSET_TYPE_CRACK
};
enum enum_PROPERTY_CLOSET
{
	bool:property_closet_VALID,
	property_closet_OBJECT_ID,
	property_closet_TYPE,
	property_closet_INT,
	property_closet_INT_EXTRA
};
new PROPERTY_CLOSET[MAX_PROPERTIES][MAX_CLOSET_SLOTS][enum_PROPERTY_CLOSET];

enum PLAYER_PROPERTY_CONSTRUCTOR_ENU
{
	bool:player_property_creator_ENABLED,
	player_property_creator_INTERIO,
	Float:player_property_creator_EXT_X,
	Float:player_property_creator_EXT_Y,
	Float:player_property_creator_EXT_Z,
	Float:player_property_creator_EXT_ANG,
	player_property_creator_EXT_INT,
}
new PLAYER_PROPERTY_CONSTRUCTOR[MAX_PLAYERS][PLAYER_PROPERTY_CONSTRUCTOR_ENU];