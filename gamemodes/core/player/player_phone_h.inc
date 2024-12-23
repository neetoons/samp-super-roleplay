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

#if defined player_phone_included 
	#endinput
#endif
#define player_phone_included

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

