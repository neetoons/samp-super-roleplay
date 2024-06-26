enum
{
	DIALOG_REGISTER,
	DIALOG_REGISTER_EMAIL,
	DIALOG_LOGIN,
	DIALOG_SECURE_LOGIN_INSERT_CODE,
	DIALOG_INFO,
	DIALOG_CLOTHES,
	DIALOG_FOOD_PIZZA,
	DIALOG_FOOD_CLUCKIN,
	DIALOG_FOOD_BURGER,
	DIALOG_PLAYER_TOYS,
	DIALOG_PLAYER_TOY_MENU,
	DIALOG_PLAYER_TOY_EDIT_NAME,
	DIALOG_PLAYER_TOY_CONFIRMDELETE,
	DIALOG_PLAYER_TOY_EDIT_BONE,
	DIALOG_PLAYER_TOY_SELECT_BONE,
	DIALOG_PLAYER_TOY_COLOR_1,
	DIALOG_PLAYER_TOY_COLOR_2,
	DIALOG_PLAYER_TOY_DELETE_ALL,
	DIALOG_BANK,
	DIALOG_BANK_WITHDRAW,
	DIALOG_BANK_DEPOSIT,
	DIALOG_BANK_TRANSFER,
	DIALOG_BANK_TRANSFER_SEND,
	DIALOG_BANK_TRANSACTIONS,
	DIALOG_CREATE_BANK_ACCOUNT,
	DIALOG_247_LIST,
	DIALOG_PLAYER_POCKET,
	DIALOG_PLAYER_POCKET_OPTIONS,
	DIALOG_PLAYER_POCKET_DELETE_ALL,
	DIALOG_PHONE,
	DIALOG_PHONE_BOOK,
	DIALOG_PHONE_BOOK_ADD_NUMBER,
	DIALOG_PHONE_BOOK_ADD_NAME,
	DIALOG_PHONE_BOOK_OPTIONS,
	DIALOG_PHONE_BOOK_CHANGE_NAME,
	DIALOG_PHONE_BOOK_CONFIRM_DELET,
	DIALOG_PHONE_BOOK_DELETE_ALL,
	DIALOG_PHONE_CALL_NUMBER,
	DIALOG_PHONE_SMS_NUMBER,
	DIALOG_PHONE_SMS_MESSAGE,
	DIALOG_PHONE_BOOK_SEND_MESSAGE,
	DIALOG_PHONE_SENT_MESSAGES,
	DIALOG_PHONE_RECEIVED_MESSAGES,
	DIALOG_CONFIRM_BUY_PROPERTY,
	DIALOG_PLAYER_GPS,
	DIALOG_PLAYER_GPS_PLAYER,
	DIALOG_PLAYER_GPS_PLAYER_ADD,
	DIALOG_PLAYER_GPS_PLAYER_OPTION,
	DIALOG_PLAYER_GPS_CHANGE_NAME,
	DIALOG_PLAYER_GPS_CONFIRM_DELET,
	DIALOG_PLAYER_GPS_SITES,
	DIALOG_PLAYER_GPS_WORKS,
	DIALOG_PLAYER_GPS_DELETE_ALL,
	DIALOG_PLAYER_GPS_SITE_0,
	DIALOG_PLAYER_GPS_SITE_1,
	DIALOG_PLAYER_GPS_SITE_2,
	DIALOG_PLAYER_GPS_SITE_3,
	DIALOG_PLAYER_GPS_SITE_4,
	DIALOG_PLAYER_GPS_SITE_5,
	DIALOG_PLAYER_GPS_SITE_6,
	DIALOG_PLAYER_GPS_SITE_7,
	DIALOG_PLAYER_GPS_SITE_8,
	DIALOG_PLAYER_GPS_PROPERTIES,
	DIALOG_PLAYER_GPS_VEHICLES,
	DIALOG_PROPERTY_OPTIONS,
	DIALOG_PROPERTY_NAME,
	DIALOG_PROPERTY_BANK_SELL,
	DIALOG_PLAYER_MP3,
	DIALOG_PLAYER_MP3_RESULTS,
	DIALOG_BUY_VEHICLE,
	DIALOG_NOTARY,
	DIALOG_NOTARY_SELECT_PROPERTY,
	DIALOG_PROPERTY_SELL_PRICE,
	DIALOG_PROPERTY_SELL_TO_PLAYER,
	DIALOG_SELL_CONFIRM_PROPERTY,
	DIALOG_NOTARY_SELECT_VEHICLE,
	DIALOG_VEHICLE_BANK_SELL,
	DIALOG_VEHICLE_SELL_PRICE,
	DIALOG_VEHICLE_SELL_TO_PLAYER,
	DIALOG_SELL_CONFIRM_VEHICLE,
	DIALOG_SELECT_TRUCK_TYPE,
	DIALOG_TUNING_MENU_PART,
	DIALOG_TUNING_MENU_COMPONENT,
	DIALOG_MECHANIC_MENU,
	DIALOG_MECHANIC_REPAIR_PRICE,
	DIALOG_MECHANIC_COLOR_PRICE,
	DIALOG_MECHANIC_TUNING_PRICE,
	DIALOG_MECHANIC_PAINTJOB_PRICE,
	DIALOG_MECHANIC_REM_COM_PRICE,
	DIALOG_MECHANIC_ACCEPT,
	DIALOG_MECHANIC_SELECT_COL_SLOT,
	DIALOG_MECHANIC_SELECT_COLOR,
	DIALOG_MECHANIC_REMOVE_COMPONEN,
	DIALOG_TUNING_PAINTJOB,
	DIALOG_SEED_LIST,
	DIALOG_SEED_BUY,
	DIALOG_PLANT_PLANTS,
	DIALOG_BLACK_MARKET,
	DIALOG_BLACK_MARKET_WEAPONS,
	DIALOG_BLACK_MARKET_SELECT_WEA,
	DIALOG_BLACK_MARKET_AMMO,
	DIALOG_PLAYER_WEAPONS,
	DIALOG_PLAYER_WEAPONS_OPTIONS,
	DIALOG_PLAYER_WEAPONS_DELETE_A,
	DIALOG_PLAYER_WEAPONS_DELETE,
	DIALOG_ANIMS,
	DIALOG_TRICKS_FOOD,
	DIALOG_TRICKS_MEDICINE,
	DIALOG_TRICKS_CANNABIS,
	DIALOG_TRICKS_CRACK,
	DIALOG_TRICKS_SU,
	DIALOG_TRICKS_WEAPON,
	DIALOG_VEHICLE_BOOT,
	DIALOG_VEHICLE_BOOT_DELETE_ALL,
	DIALOG_VEHICLE_BOOT_OPTIONS,
	DIALOG_VEHICLE_BOOT_DELETE,
	DIALOG_SELECT_POLICE_SKIN,
	DIALOG_POLICE_RADIOS,
	DIALOG_POLICE_LIST,
	DIALOG_POLICE_MODIFY,
	DIALOG_POLICE_SHOP,
	DIALOG_POLICE_SELECT_WEAPON,
	DIALOG_POLICE_AMMO,
	DIALOG_POLICE_ARMOUR,
	DIALOG_POLICE_WEAPONS,
	DIALOG_HELP,
	DIALOG_HELP_GENERAL,
	DIALOG_HELP_INFO,
	DIALOG_HELP_CHARACTER,
	DIALOG_HELP_PROPERTIES,
	DIALOG_HELP_REPORT,
	DIALOG_HELP_VEHICLES,
	DIALOG_HELP_WORKS,
	DIALOG_HELP_CREWS,
	DIALOG_HELP_SU,
	DIALOG_HELP_SECURE_LOGIN,
	DIALOG_ADMIN_LIST,
	DIALOG_ADMIN_MODIFY,
	DIALOG_CHANGE_PASSWORD,
	DIALOG_CHANGE_PASSWORD_PASS,
	DIALOG_ANTI_CHEAT,
	DIALOG_ANTI_CHEAT_MODIFY,
	DIALOG_ANTI_CHEAT_MODIFY_DETECT,
	DIALOG_SHOP_ARTICLE_MODIFY,
	DIALOG_SHOP_ARTICLE_REMOVE,
	DIALOG_SHOP_ADD,
	DIALOG_SHOP_ADD_TEXT,
	DIALOG_SHOP_ADD_PRICE,
	DIALOG_CRANE_SELECT_VEHICLE,
	DIALOG_CREATE_CREW,
	DIALOG_CREATE_CREW_NAME,
	DIALOG_CREATE_CREW_COLOR,
	DIALOG_CREATE_CREW_CONFIRM,
	DIALOG_CREW_MENU,
	DIALOG_CREW_MEMBER_LIST,
	DIALOG_CREW_CHANGE_NAME,
	DIALOG_CREW_MEMBER_LIST_DELETE,
	DIALOG_CREW_CAST_MEMBER_CONFIRM,
	DIALOG_CREW_INVITE_RANK,
	DIALOG_CREW_INVITE_ACCEPT,
	DIALOG_CREW_RANKS,
	DIALOG_CREW_RANK_MODIFY,
	DIALOG_CREW_RANK_MODIFY_NAME,
	DIALOG_CREW_RANK_MODIFY_PERMISS,
	DIALOG_CREW_RANK_DELETE,
	DIALOG_CREW_RANK_DELETE_CONFIRM,
	DIALOG_CREW_RANK_CREATE,
	DIALOG_CREW_MODIFY_COLOR,
	DIALOG_CREW_DELETE,
	DIALOG_CREW_LEAVE,
	DIALOG_CREW_MODIFY_MEMBERS,
	DIALOG_CREW_MODIFY_MEMBER,
	DIALOG_CREW_PROPERTY_CONFIRM,
	DIALOG_CREW_LEAVE_TERRITORY,
	DIALOG_CREW_LEAVE_PROPERTY,
	DIALOG_POLICE_PENALTY,
	DIALOG_SERVER_ECO,
	DIALOG_SERVER_ECO_MODIFY,
	DIALOG_SU_BUY,
	DIALOG_SU,
	DIALOG_VOBJECT_MENU,
	DIALOG_VOBJECT_OBJECTS,
	DIALOG_VOBJECT_OBJECT,
	DIALOG_VOBJECT_OBJECT_CNAME,
	DIALOG_VOBJECT_OBJECT_COLORS,
	DIALOG_VOBJECT_OBJECT_COLOR,
	DIALOG_VOBJECT_OBJECT_COLOR_HEX,
	DIALOG_VOBJECT_OBJECT_CDELETE,
	DIALOG_VOBJECT_DELETE_ALL,
	DIALOG_VOBJECT_TEXT_MENU,
	DIALOG_VOBJECT_TEXT_TEXT,
	DIALOG_VOBJECT_TEXT_FONT,
	DIALOG_VOBJECT_TEXT_FONT_SIZE,
	DIALOG_VOBJECT_TEXT_FONT_COLOR,
	DIALOG_VOBJECT_COUNTRY,
	DIAlOG_BUY_MASO,
	DIALOG_PLAYER_CONFIG,
	DIALOG_WORKS_TELEPORTS,
	DIALOG_POLICE_BYC,
	DIALOG_POLICE_BYC_LAST_ALL,
	DIALOG_POLICE_BYC_NAME_FIND,
	DIALOG_POLICE_DELETE_BYC,
	DIALOG_POLICE_BYC_LAST_PLAYER,
	DIALOG_CHANGE_NAME,
	DIALOG_CHANGE_NAME_NAME,
	DIALOG_CHANGE_EMAIL,
	DIALOG_CHANGE_EMAIL_EMAIL,
	DIALOG_PROPERTY_CLOSET,
	DIALOG_PROPERTY_CLOSET_DLT_ALL,
	DIALOG_PROPERTY_CLOSET_OPTIONS,
	DIALOG_PROPERTY_CLOSET_DELETE,
	DIALOG_INV_ACTIONS,
	DIALOG_NEARS_PLAYERS,
	DIALOG_NEARS_PLAYERS_PRICE,
	DIALOG_NEARS_PLAYERS_AMOUNT,
	DIALOG_HOTDOG,
	DIALOG_FSHOP_SELECT_PROPERTY,
	DIALOG_PROPERTY_OBJECTS_MENU,
	DIALOG_POBJECTS_DELETE_ALL,
	DIALOG_POBJECT_EDIT,
	DIALOG_POBJECT_TEXTURE_INDEX,
	DIALOG_POBJECT_TEXTURE,
	DIALOG_PLANE_OPTIONS,
	DIALOG_PLANE_SELECT_COLOR,
	DIALOG_BOAT_OPTIONS,
	DIALOG_BOAT_SELECT_COLOR,
	DIALOG_GRAFFITI_OPTIONS,
	DIALOG_GRAFFITI_EDIT,
	DIALOG_GRAFFITI_TEXT_OPTIONS,
	DIALOG_GRAFFITI_ETEXT_TEXT,
	DIALOG_GRAFFITI_ETEXT_FONT,
	DIALOG_GRAFFITI_ETEXT_SIZE,
	DIALOG_GRAFFITI_ETEXT_COLOR
}