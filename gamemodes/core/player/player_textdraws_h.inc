/* TEXTDRAWS */
#define MAX_PLAYER_MAP_GPS_POINTS 20
#define HUD_PTEXTDRAWS_SIZE 4
enum Player_Textdraws_Enum
{
	PlayerText:ptextdraw_GUIDE[5],
	PlayerText:ptextdraw_HOSPITAL[3], //Barra recuperando vida despues de morir
	PlayerText:ptextdraw_CLOTHING_SHOP[6], //Tiendas de ropas
	PlayerText:ptextdraw_REGISTER_SKIN[10],
	PlayerText:ptextdraw_TOYS_SHOP[10],
	PlayerText:ptextdraw_PROPERTY_CONSTRUCTOR[6],
	PlayerText:ptextdraw_TRUCK_LOAD[4],
	PlayerText:ptextdraw_RECYCLE_TRASH[3],
	PlayerText:ptextdraw_PLAYER_GPS_MAP[MAX_PLAYER_MAP_GPS_POINTS],
	PlayerText:ptextdraw_WANTED_LEVEL[2],
	PlayerText:ptextdraw_SHOP_BUTTONS[5],
	PlayerText:ptextdraw_SHOP_ARTICLES_PAGE,
	PlayerText:ptextdraw_SHOP_ARTICLES_MODEL[6],
	PlayerText:ptextdraw_SHOP_ARTICLES_PRICE[6],
	PlayerText:ptextdraw_SHOP_ARTICLE_NUMBER,
	PlayerText:ptextdraw_SHOP_ARTICLE_INFO,
	PlayerText:ptextdraw_SHOP_ARTICLE_MODELID,
	PlayerText:ptextdraw_TUNING_SHOP,
	PlayerText:ptextdraw_MESSAGE,
	PlayerText:ptextdraw_HUD[HUD_PTEXTDRAWS_SIZE]
}
new PlayerTextdraws[MAX_PLAYERS][Player_Textdraws_Enum];