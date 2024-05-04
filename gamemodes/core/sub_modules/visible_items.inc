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
#if defined visible_items_included 
    #endinput
#endif
#define visible_items_included

#include <YSI_Coding\y_hooks>

#define DEFAULT_VISIBLE_ITEMS 500

hook OnPlayerConnect(playerid) {
    Streamer_SetVisibleItems(STREAMER_TYPE_OBJECT, DEFAULT_VISIBLE_ITEMS, playerid);
}

hook OnPlayerEnterInterior(playerid, index) {
    new items = DEFAULT_VISIBLE_ITEMS;
    
    if(ENTER_EXIT[index][ee_VISIBLE_ITEMS] >= 0) items = ENTER_EXIT[index][ee_VISIBLE_ITEMS];
    Streamer_SetVisibleItems(STREAMER_TYPE_OBJECT, items, playerid);
}

hook OnPlayerExitInterior(playerid, index) {
    Streamer_SetVisibleItems(STREAMER_TYPE_OBJECT, DEFAULT_VISIBLE_ITEMS, playerid);
}