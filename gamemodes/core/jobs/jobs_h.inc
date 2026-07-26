enum
{
	WORK_TYPE_NONE,
	WORK_TYPE_NORMAL,
	WORK_TYPE_FAMILY,
}
enum _:E_WORKS
{
	WORK_NONE = 0,
	WORK_TAXI = 1,
	WORK_TRUCK = 2,
	WORK_MECHANIC = 3,
	WORK_HARVESTER = 4,
	WORK_FUMIGATOR = 5,
	WORK_TRASH = 6,
	WORK_LUMBERJACK = 7,
	WORK_FARMER = 8,
	WORK_POLICE = 9,
	WORK_PIZZA = 10,
	WORK_MEDIC = 11,
	WORK_WAREHOUSE = 12,
	WORK_DELIVERYMAN = 13,
	WORK_MINERO = 14
};
enum work_info_info
{
	work_info_TYPE,
	work_info_LEVEL,
	work_info_NAME[24],
	bool:work_info_NEED_DUTY, 	//si este trabajo es normal y tiene vehiculos indica si es necesario que el jugador este de servicio para subirse
	work_info_EXTRA_PAY,		//dejar en 0 para no paga extra.......Paga extra de 'work_info_EXTRA_PAY' cada 'work_info_EXTRA_PAY_EXP' con un limite de 'work_info_EXTRA_PAY_LIMIT'
	work_info_EXTRA_PAY_EXP,
	work_info_EXTRA_PAY_LIMIT,	// dejar en 0 para sin limites
	
	//sistame de mejores empleados
	work_info_TOP_MAX_PRIZE,
	work_info_TOP_PRIZE_DECREASE,
	work_info_TOP_MIN_PRIZE,
	work_info_TOP1_COINS_PRIZE
}
new work_info[][work_info_info] =
{
	{WORK_TYPE_NONE, 0, "ninguno",					false, 0, 0, 0,			0, 0, 0, 0},
	{WORK_TYPE_NORMAL, 1, "taxista",				false, 0, 0, 0,			0, 0, 0, 0},
	{WORK_TYPE_NORMAL, 1, "camionero",				false, 0, 50, 1300,		100000, 20000, 3000, 2},
	{WORK_TYPE_NORMAL, 3, "mecánico",				true, 0, 0, 0,			100000, 20000, 3000, 1},
	{WORK_TYPE_NORMAL, 1, "cosechador",				false, 20, 30, 800,		20000, 3000, 1000, 0},
	{WORK_TYPE_NORMAL, 4, "fumigador",				false, 20, 30, 800,		20000, 3000, 1000, 0},
	{WORK_TYPE_NORMAL, 1, "basurero", 				true, 60, 50, 2000,		20000, 3000, 1000, 0},
	{WORK_TYPE_NORMAL, 1, "leñador",				false, 10, 100, 100,	20000, 3000, 1000, 0},
	{WORK_TYPE_NORMAL, 5, "agricultor",				false, 0, 0, 0,			100000, 20000, 3000, 1},
	{WORK_TYPE_FAMILY, 0, "policía",				true, 1000, 1, 0,		0, 0, 0, 0},
	{WORK_TYPE_NORMAL, 1, "pizzero",				true, 60, 20, 800,		0, 0, 0, 0},
	{WORK_TYPE_NORMAL, 5, "médico",					true, 60, 50, 2000,		0, 0, 0, 0},
	{WORK_TYPE_NORMAL, 1, "almacén",				false, 10, 100, 100,	100000, 20000, 3000, 1},
	{WORK_TYPE_NORMAL, 6, "repartidor",				false, 0, 50, 1300,		100000, 20000, 3000, 3},
	{WORK_TYPE_NORMAL, 1, "minero",				false, 0, 50, 1300,		100000, 20000, 3000, 3}
};

forward OnPlayerObtainWork(playerid, work);
enum enum_PLAYER_WORKS
{
	pwork_SET,
	pwork_LEVEL
};
new PLAYER_WORKS[MAX_PLAYERS][sizeof(work_info)][enum_PLAYER_WORKS];

new Float:MedicalBuyKitsCoords[][3] = {
	{-2031.387207, -115.191055, 1035.171875}
};

new male_medic_skins[] = {274, 275, 276};

enum obtain_work_coords_info
{
	bool:obtain_work_AVAILABLE,
	bool:obtain_work_LABELS,
	Float:obtain_work_X,
	Float:obtain_work_Y,
	Float:obtain_work_Z,
	obtain_work_INTERIOR,
	bool:obtain_work_MAP_ICON,
	obtain_work_MAP_ICON_ID,
	Float:obtain_work_MAP_ICON_X,
	Float:obtain_work_MAP_ICON_Y,
	Float:obtain_work_MAP_ICON_Z,
}
new Float:obtain_work_coords[][obtain_work_coords_info] =
{
	{false, false, 0.0, 0.0, 0.0, 0,	false, 0, 0.0, 0.0, 0.0},
	{true, true, 1490.285766, 1305.699340, 1093.296386, 3,	true, 61, 1752.603881, -1894.155883, 13.557376}, //taxista ls
	{true, true, 1297.107543, -65.027748, 1002.498046, 18, true, 61, -510.699890, -539.409118, 25.523437}, //caminero
	{true, true, 1831.917602, -1446.153564, 13.592857, 0, true, 27, 1831.917602, -1446.153564, 13.592857}, //mecanico
	{true, true, -372.126922, -1435.706298, 25.726562, 0,	true, 61, -372.126922, -1435.706298, 25.726562}, //Cosechador
	{true, true, -1342.295898, -2195.076660, 23.591030, 0, true, 61, -1342.295898, -2195.076660, 23.591030}, //Fumigador
	{true, true, -1884.997314, -1636.733642, 21.750000, 0, true, 61, -1884.997314, -1636.733642, 21.750000}, //Basurero
	{true, true, -532.029846, -97.514228, 63.296875, 0, true, 61, -532.029846, -97.514228, 63.296875}, //leñador
	{true, true, 1564.864257, 23.255302, 24.164062, 0, true, 61, 1564.864257, 23.255302, 24.164062},
	{true, true, -510.972015, 324.242736, 2004.585937, 20,	false, 0, 1480.966918, -1772.065673, 18.795755}, // policia
	{true, true, 377.902313, -119.416114, 1001.492187, 5, false, 0, 2105.485107, -1806.400878, 13.554687},
	{true, true, -2033.237304, -117.411125, 1035.171875, 3,	false, 0, 0.0, 0.0, 0.0},
	{true, true, 1944.000366, -1481.420776, 1394.125244, 11, true, 61, -1058.164306, -652.747924, 32.007812}, //almacén
	{true, true, -1014.827697, -594.402160, 32.382198, 0, true, 61, -1014.827697, -594.402160, 32.382198}, //repartidor
	{true, true, 583.2664,192.2517,20.8174, 0, true, 61, 583.2664,192.2517,20.8174} //minero
};
