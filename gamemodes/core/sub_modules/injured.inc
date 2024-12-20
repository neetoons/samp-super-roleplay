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
#if defined injured_included 
    #endinput
#endif
#define injured_included

#include <YSI_Coding\y_hooks>

new
    pInjuredTimer[MAX_PLAYERS] = {-1, ...},
    bool:pInjuredMoving[MAX_PLAYERS],
    pInjuredMovingStep[MAX_PLAYERS];

hook OnPlayerDisconnect(playerid, reason) {
    if(pInjuredTimer[playerid] != -1) {
        KillTimer(pInjuredTimer[playerid]);
        pInjuredTimer[playerid] = -1;
    }
}

hook OnPlayerSpawn(playerid) {
    if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK) {
        SetPlayerHud(playerid);
        if(PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_DEAD) SetPlayerHealthEx(playerid, 60.0);
        SetCameraBehindPlayer(playerid);
        SendAlertToMedics(playerid);
        TogglePlayerControllableEx(playerid, false);
        KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][3]);
        PLAYER_TEMP[playerid][pt_TIMERS][3] = SetTimerEx("TogglePlayerControl", 2000, false, "ib", playerid, true);
        
        pInjuredMoving[playerid] = false;
        pInjuredMovingStep[playerid] = 0;
        if(pInjuredTimer[playerid] != -1) {
            KillTimer(pInjuredTimer[playerid]);
            pInjuredTimer[playerid] = -1;
        }
        pInjuredTimer[playerid] = SetTimerEx("HealthDown", 3000, false, "i", playerid);
        
        ApplyAnimation(playerid, "SWEET", "Sweet_injuredloop", 4.1, true, 0, 0, 0, 0, 1);
        if(PI[playerid][ac_WANTED_LEVEL] > 0) SendNotification(playerid, "Est�s herido y en b�squeda, espera a que la polic�a venga a por ti.");
        else SendNotification(playerid, "Est�s herido, puedes esperar a un m�dico o usar ~r~/morir~w~.");
    }
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger) {
    if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK)
	{
		new Float:sx, Float:sy, Float:sz;
	    GetPlayerPos(playerid, sx, sy, sz);
        RemovePlayerFromVehicle(playerid);
		SetPlayerPos(playerid, sx, sy, sz);
		ApplyAnimation(playerid, "SWEET", "Sweet_injuredloop", 4.1, true, 0, 0, 0, 0, 1);
		return Y_HOOKS_BREAK_RETURN_1;
	}
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerUpdate(playerid) {
    if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK) {
        new Keys, ud, lr;
        GetPlayerKeys(playerid, Keys, ud, lr);
        if(ud == KEY_UP /*|| ud == KEY_DOWN || lr == KEY_LEFT || lr == KEY_RIGHT*/) {
            if(!pInjuredMoving[playerid]) {
                pInjuredMoving[playerid] = true;
                if(pInjuredMovingStep[playerid] == 0) HealthDown(playerid);
            }
        }
        else {
            if(pInjuredMoving[playerid]) {
                pInjuredMoving[playerid] = false;
                if(pInjuredMovingStep[playerid] == 1) HealthDown(playerid);
            }
        }
        
        new anim = GetPlayerAnimationIndex(playerid);
        if(anim != 1017 && anim != 1537) HealthDown(playerid);
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

forward HealthDown(playerid);
public HealthDown(playerid)
{
    if(pInjuredTimer[playerid] != -1) {
        KillTimer(pInjuredTimer[playerid]);
        pInjuredTimer[playerid] = -1;
    }
    
	if(ACCOUNT_INFO[playerid][ac_STATE] == ROLEPLAY_STATE_CRACK) {
        new Float:rx, Float:rz;
        GetPlayerCameraRotation(playerid, rx, rz);

        GivePlayerHealthEx(playerid, -0.3);
        if(pInjuredMoving[playerid]) {
            switch(pInjuredMovingStep[playerid]) {
                case 0: {
                    ApplyAnimation(playerid, "PED", "CAR_crawloutRHS", 4.1, true, 1, 1, 0, 0, 1);
                    SetPlayerFacingAngle(playerid, rz + 90.0);
                    pInjuredMovingStep[playerid] = 1;
                    pInjuredTimer[playerid] = SetTimerEx("HealthDown", 1000, false, "i", playerid);
                    
                }
                case 1: {
                    ApplyAnimation(playerid, "SWEET", "Sweet_injuredloop", 4.1, true, 0, 0, 0, 0, 1);
                    SetPlayerFacingAngle(playerid, rz);
                    pInjuredMovingStep[playerid] = 0;
                    pInjuredTimer[playerid] = SetTimerEx("HealthDown", 2000, false, "i", playerid);
                }
            }
        }
        else {
            ApplyAnimation(playerid, "SWEET", "Sweet_injuredloop", 4.1, true, 0, 0, 0, 0, 1);
            SetPlayerFacingAngle(playerid, rz);
            pInjuredMovingStep[playerid] = 0;
            pInjuredTimer[playerid] = SetTimerEx("HealthDown", 3000, false, "i", playerid);
        }
    }
	return 1;
}