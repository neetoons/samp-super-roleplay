/* SYSTEM - PHONE */
enum
{
	CALL_GAME_STATE_NONE,
	CALL_STATE_WAITING_RESPONSE,
	CALL_STATE_INCOMING_CALL,
	CALL_STATE_ESTABLISHED,
}

enum
{
	PHONE_STATE_ON,
	PHONE_STATE_OFF
}

#define MAX_PHONE_CONTACTS	20
enum Phone_Book_Enum
{
	phone_book_contact_VALID,
	phone_book_contact_ID,
	phone_book_contact_NAME[24],
	phone_book_contact_PHONE_NUMBER
}
new PLAYER_PHONE_BOOK[MAX_PLAYERS][MAX_PHONE_CONTACTS][Phone_Book_Enum]; //PB = PhoneBook

