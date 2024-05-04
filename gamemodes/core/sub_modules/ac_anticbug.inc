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
#if defined ac_anticbug_included 
    #endinput
#endif
#define ac_anticbug_included

#include <YSI_Coding\y_hooks>

new
    bool:pCBugging[MAX_PLAYERS],
    pCBugLastFiredWeapon[MAX_PLAYERS];

hook OnPlayerDisconnect(playerid, reason) {
    pCBugging[playerid] = false;
    pCBugLastFiredWeapon[playerid] = 0;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if(
        ac_Info[CHEAT_CBUG][ac_Enabled] &&
        !pCBugging[playerid] &&
        GetPlayerState(playerid) == PLAYER_STATE_ONFOOT
    ) {
        new weaponid = GetPlayerWeapon(playerid);
        if(weaponid > 0 && WEAPON_INFO[weaponid][weapon_info_AC_CBUG]) {
            if(newkeys & KEY_FIRE) {
                pCBugLastFiredWeapon[playerid] = gettime();
            }
            else if(newkeys & KEY_CROUCH) {
                new now = gettime();
                if(now > PLAYER_AC_INFO[playerid][CHEAT_CBUG][p_ac_info_IMMUNITY])
                {
                    if((now - pCBugLastFiredWeapon[playerid]) < 1) {
                        if(!ac_Info[CHEAT_CBUG][ac_Interval]) {
                            OnPlayerCheatDetected(playerid, CHEAT_CBUG);
                            SetCBugPlayerInfo(playerid);
                        }
                        else
                        {
                            if(now - PLAYER_AC_INFO[playerid][CHEAT_CBUG][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_CBUG][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_CBUG][p_ac_info_DETECTIONS] = 0;
                            else PLAYER_AC_INFO[playerid][CHEAT_CBUG][p_ac_info_DETECTIONS] ++;
                            
                            PLAYER_AC_INFO[playerid][CHEAT_CBUG][p_ac_info_LAST_DETECTION] = now;
                            if(PLAYER_AC_INFO[playerid][CHEAT_CBUG][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_CBUG][ac_Detections]) {
                                OnPlayerCheatDetected(playerid, CHEAT_CBUG);
                                SetCBugPlayerInfo(playerid);
                            }
                        }
                    }
                }
            }
        }
    }
}

SetCBugPlayerInfo(playerid) {
    new Float:sx, Float:sy, Float:sz;
    GetPlayerPos(playerid, sx, sy, sz);
    SetPlayerPos(playerid, sx, sy, sz);
    TogglePlayerControllableEx(playerid, false);
    KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][19]);
    PLAYER_TEMP[playerid][pt_TIMERS][19] = SetTimerEx("FinishCBug", 2000, false, "i", playerid);
    SetTimerEx("ResyncPlayer", 0, false, "d", playerid);
    SendNotification(playerid, "Evita hacer CBug.");
}

forward FinishCBug(playerid);
public FinishCBug(playerid) {
    pCBugging[playerid] = false;
    pCBugLastFiredWeapon[playerid] = 0;
    TogglePlayerControllableEx(playerid, true);
}