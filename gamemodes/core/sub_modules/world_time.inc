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
#if defined worl_time_included 
	#endinput
#endif
#define worl_time_included

new
	sa_weathers[] = {0,1},
	sa_weather,
	last_weather_update;

#define UPDATE_WEATHER_SECONDS 10800 //cada 3 horas

public OnMyWorldTimeUpdate()
{
	if(gettime() > last_weather_update + UPDATE_WEATHER_SECONDS)
	{
		sa_weather = sa_weathers[random(sizeof(sa_weathers))];
		for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(IsPlayerConnected(i) && PLAYER_TEMP[i][pt_GAME_STATE] > GAME_STATE_CONNECTED)
			{
				InterpolatePlayerWeather(i, sa_weather);
			}
		}
		last_weather_update = gettime();
	}
	new td_str[16]; format(td_str, sizeof td_str, "%02d:%02d", GetMyWorldHour(), GetMyWorldMinute());
	TextDrawSetString(Textdraws[textdraw_SERVER_TIME], td_str);
	return 1;
}

SetPlayerCityWeather(playerid)
{
	SetPlayerMyTime(playerid, -1, -1); //volver a la hora del servidor
	SetPlayerMyWeather(playerid, sa_weather);
	return 1;
}