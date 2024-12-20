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
#if defined tunning_h_included 
	#endinput
#endif
#define tunning_h_included

/* TUNING SYSTEM */
#define TOTAL_TUNING_PARTS	14
#define MAX_TUNING_PARTS_COMPONENTS	20
enum PLAYER_TUNING_MENU_Info
{
	tuning_menu_NAME[24], // o part
	tuning_menu_ID,
	tuning_menu_PIECES,
}
new PLAYER_TUNING_MENU[MAX_PLAYERS][MAX_TUNING_PARTS_COMPONENTS][PLAYER_TUNING_MENU_Info];

VehiclePaintjob(modelid)
{
	switch(modelid)
	{
		case 483: return 1;
		case 534..536: return 3;
		case 558..562: return 3;
		case 565, 567: return 3;
		case 575: return 2;
		case 576: return 3;
	}
	return false;
}