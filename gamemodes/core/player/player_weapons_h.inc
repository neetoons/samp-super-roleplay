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

#if defined player_weapons_included 
	#endinput
#endif
#define player_weapons_included

enum enum_PLAYER_WEAPONS
{
	bool:player_weapon_VALID,
	player_weapon_DB_ID,
	player_weapon_ID,
	player_weapon_AMMO,
}
new PLAYER_WEAPONS[MAX_PLAYERS][20][enum_PLAYER_WEAPONS];