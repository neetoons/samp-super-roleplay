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
#if defined world_h_included 
	#endinput
#endif
#define world_h_included
new 
	Club_Strippers_Actor[2],
	Club_Bar_Actor,
	Club_Actors[7];
	
new RandomStripAnimations[][] =
{
	"STR_A2B",
	"strip_G",
	"STR_B2C",
	"STR_C1",
	"STR_C2"
};

new RandomPayStripAnimations[][] =
{
	"PLY_CASH",
	"PUN_CASH",
	"PUN_HOLLER",
	"PUN_LOOP"
};