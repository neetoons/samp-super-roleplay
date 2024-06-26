#if defined player_account_included 
	#endinput
#endif
#define player_account_included

enum account_Enum
{
	ac_ID,
	ac_IP[16],
	ac_NAME[24],
	ac_EMAIL[32],
	ac_PASS[64 + 1],
	ac_SALT[16],
	ac_LAST_CONNECTION,
	ac_LAST_CONNECTION_TIMESTAMP,
	ac_TIME_PLAYING,
	ac_LEVEL,
	ac_REP,
	ac_STATE,
	ac_REG_DATE[24], //Fecha de registro de la tabla REGISTER_LOG
	ac_DOUBT_CHANNEL,
	ac_TIME_FOR_REP, // En MS
	ac_ADMIN_LEVEL,
	ac_PAYDAY_REP,
	ac_VIP,
	ac_VIP_EXPIRE_DATE[24],
};
new ACCOUNT_INFO[MAX_PLAYERS][account_Enum]; // AI (AccountInfo)

enum enum_PI
{
	ac_LEVEL,
	ac_REP,
	ac_CONNECTED,
	ac_PLAYERID,
	ac_DOUBT_CHANNEL,
	ac_COINS,
	ac_SKIN,
	ac_CASH,
	Float:ac_POS_X,
	Float:ac_POS_Y,
	Float:ac_POS_Z,
	Float:ac_ANGLE,
	ac_STATE,
	ac_INTERIOR,
	ac_LOCAL_INTERIOR,
	ac_FIGHT_STYLE,
	Float:ac_HEALTH,
	Float:ac_ARMOUR,
	ac_GENDER,
	Float:ac_HUNGRY,
	Float:ac_THIRST,
	ac_BLACK_MARKET_LEVEL,
	ac_WANTED_LEVEL,
	ac_POLICE_JAIL_TIME,
	ac_POLICE_DUTY,
	ac_POLICE_JAIL_ID,
	ac_DRIVE_LICENSE_POINTS,
	ac_BANK_ACCOUNT,
	ac_BANK_MONEY,
	ac_PHONE_NUMBER,
	ac_PHONE_STATE,
	ac_PHONE_VISIBLE_NUMBER,
	ac_GPS,
	ac_Maso,
	ac_MP3,
	ac_PHONE_RESOLVER,
	ac_SPEAKERS,
	ac_MECHANIC_PIECES,
	ac_FUEL_DRUM,
	ac_SEED_MEDICINE,
	ac_SEED_CANNABIS,
	ac_SEED_CRACK,
	ac_MEDICINE,
	ac_CANNABIS,
	ac_CRACK,
	ac_CONFIG_SOUNDS,
	ac_CONFIG_AUDIO,
	ac_CONFIG_TIME,
	ac_CONFIG_HUD,
	ac_CONFIG_ADMIN,
	ac_CONFIG_SECURE_LOGIN,
	ac_MUTE,
	ac_PLACA_PD,
	ac_CAN_BUY_BM,
	ac_CREW,
	ac_CREW_RANK,
	ac_MECHANIC_KITS,
	ac_MEDICAL_KITS
};
new PI[MAX_PLAYERS][enum_PI];

