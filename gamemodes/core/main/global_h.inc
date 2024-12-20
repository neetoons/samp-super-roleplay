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

#if defined global_h_included 
	#endinput
#endif
#define global_h_included


new Float:Driving_School_Points[][] =
{
	{-2046.806640, -78.443099, 35.04331},
	{-2013.774780, -72.396865, 35.04462},
	{-2007.532470, -187.772094, 35.5859},
	{-2008.538696, -286.359100, 35.1949},
	{-2197.762207, -289.570770, 35.1921},
	{-2208.258300, -192.957733, 35.0926},
	{-2346.990966, -187.385894, 35.0468},
	{-2371.669677, -73.981056, 35.15266},
	{-2438.013671, -68.217697, 34.54619},
	{-2496.469482, -65.725486, 25.43449},
	{-2497.072021, 34.770183, 25.335966},
	{-2423.709716, 39.920101, 34.890605},
	{-2414.502197, 232.563766, 34.89061},
	{-2314.661621, 412.145690, 34.89409},
	{-2382.651123, 557.856140, 24.61735},
	{-2195.194335, 562.594299, 34.89065},
	{-2015.883666, 562.195312, 34.89488},
	{-2007.202514, 330.878173, 34.88609},
	{-2143.078613, 322.394317, 35.04595},
	{-2148.475097, 225.861907, 35.04685},
	{-2163.534179, 89.445915, 35.046863},
	{-2164.760986, -67.964546, 35.04682},
	{-2080.382324, -72.316001, 35.04682},
	{-2025.379638, -95.889015, 35.03900}
};

#define SPECIAL_ACTION_PISSING 68

#define WORLD_INTRO 555
new
	INTRO_3[3],
	INTRO_2[6],
	INTRO_1[4];

#define NEARS_PLAYERS_DISTANCE 4.0
enum {
	NEAR_PLAYERS_INV_GENERAL,
	NEAR_PLAYERS_POCKET_GIVE,
	NEAR_PLAYERS_POCKET_SELL,
	NEAR_PLAYERS_WEAPON_GIVE,
	NEAR_PLAYERS_WEAPON_SELL
};
forward OnNearPlayerSelected(playerid, to_player, id);
forward OnPriceSelected(playerid, price, id);
forward OnAmountSelected(playerid, amount, id);
forward OnPlayerEnterInterior(playerid, index);
forward OnPlayerExitInterior(playerid, index);




enum e_WOOD
{
 w_mina,
 w_tipo,
	w_nivel,
	w_object,					// Objeto de �rbol
	w_zone,						// �rea de corte de madera
	bool:w_use,
	bool:w_drop,
	w_count,
	w_time,						// Tiempo en minutos despu�s de que aparece un nuevo mineral.
	Float:RocaX,
	Float:RocaY,
	Float:RocaZ,
	Float:RocaRX,
	Float:RocaRY,
	Float:RocaRZ
}
new MinaInfo[][e_WOOD] =
{
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,610.39233, -47.76829, 790.50641,   0.00000, 0.00000, 0.00000},//Mina A
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,606.03064, -47.09977, 789.21698,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,599.66766, -48.23664, 790.64520,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,590.76660, -47.61083, 789.36621,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,575.97437, -47.33935, 789.36621,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,570.38605, -46.50241, 788.28552,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,567.97473, -47.92181, 790.54022,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,563.96191, -47.74914, 790.54022,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,564.46478, -46.56983, 788.70111,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,556.74707, -46.38679, 788.70111,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,578.94592, -46.94428, 788.64117,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,611.53491, -35.71266, 790.50641,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,607.08044, -36.77654, 789.21545,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,601.40051, -34.83024, 790.99805,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,600.36865, -36.90084, 788.81354,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,577.89111, -36.06913, 788.64117,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,574.87720, -34.55756, 790.44891,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,569.88464, -34.70601, 790.44891,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,573.16309, -36.22608, 788.51862,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,565.51819, -35.34485, 789.69727,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,559.88641, -34.59278, 790.94946,   0.00000, 0.00000, 0.00000},
{1,0,1,INVALID_STREAMER_ID,0,false,false,0,0,559.22162, -36.39666, 788.70349,   0.00000, 0.00000, 0.00000},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,532.00146, -46.38299, 789.71643,   0.00000, 0.00000, -75.66001},//Mina B
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,527.26044, -47.68201, 791.53900,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,527.29822, -45.65039, 788.53070,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,514.06604, -47.88220, 791.80389,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,509.64182, -47.40130, 791.04303,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,512.00665, -45.85838, 788.85992,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,504.79193, -47.74235, 791.82697,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,505.34189, -46.33300, 789.41119,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,512.53528, -31.70577, 791.04303,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,514.47626, -27.11552, 794.74695,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,507.97513, -29.21132, 791.04303,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,497.18716, -27.76551, 791.81317,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,500.64111, -30.38756, 788.55493,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,491.68948, -28.55555, 789.99164,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,483.54611, -27.82957, 789.99164,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,462.99756, -29.04277, 789.99164,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,458.28204, -27.50038, 792.43390,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,452.73999, -27.66210, 792.43390,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,458.34930, -29.47113, 789.50555,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,449.85306, -29.21712, 790.31580,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,464.13443, -44.78816, 790.31580,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,467.07550, -43.52385, 788.26117,   0.00000, 0.00000, -75.66001},
{2,0,1,INVALID_STREAMER_ID,0,false,false,0,0,472.44730, -45.62525, 791.25409,   0.00000, 0.00000, -75.66001},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,419.98120, -33.80698, 789.50409,   0.00000, 0.00000, 0.00000},//MINA C
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,426.30368, -33.98831, 789.50409,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,423.86832, -35.09768, 787.98218,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,423.39349, -33.16940, 790.96008,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,423.64609, -44.43587, 787.98218,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,410.99600, -44.43304, 787.98218,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,409.52292, -46.25862, 790.75452,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,406.21722, -44.67914, 788.55347,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,401.82690, -44.39228, 788.55347,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,396.31995, -45.24484, 790.21490,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,392.14819, -43.50412, 787.89630,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,385.12445, -44.95032, 790.60315,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,379.59647, -44.49262, 790.60315,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,373.71295, -43.79356, 789.52234,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,370.36969, -45.25928, 792.31360,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,377.10602, -29.79018, 790.60315,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,372.33008, -28.25301, 792.48926,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,370.94150, -30.03618, 789.43732,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,366.32056, -30.01953, 789.43732,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,362.72391, -28.49834, 791.83203,   0.00000, 0.00000, 0.00000},
{3,0,1,INVALID_STREAMER_ID,0,false,false,0,0,358.46240, -29.17857, 790.35583,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 318.18515, -29.81613, 790.58728,   0.00000, 0.00000, 0.00000},//Mina D
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 315.14682, -31.77250, 788.45221,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 309.65030, -30.39302, 789.98218,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 303.82379, -28.51960, 791.76721,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 304.16400, -31.62576, 788.62860,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 297.78802, -31.73440, 788.62860,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 289.71585, -30.48142, 789.86328,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 284.26672, -31.67255, 788.65167,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 275.91968, -31.46796, 788.65167,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 279.84027, -29.30080, 790.87689,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 270.35858, -31.54268, 788.65088,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 273.00266, -28.01749, 792.24829,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 257.51425, -31.61290, 788.65088,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 252.38326, -29.49479, 791.91168,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 249.05457, -32.27688, 788.79932,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 243.96979, -29.66706, 791.81964,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 238.27789, -31.11380, 789.93671,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 231.98598, -31.05524, 789.93671,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 231.73611, -45.95616, 789.93671,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 238.01927, -46.52688, 793.11572,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 240.94666, -44.30827, 790.62018,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 248.18039, -43.73589, 790.62018,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 252.84819, -42.67445, 788.95435,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 255.68855, -44.42095, 791.51270,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 258.87372, -43.33083, 789.53448,   0.00000, 0.00000, 0.00000},
{4,0,1,INVALID_STREAMER_ID,0,false,false,0,0, 264.57803, -43.53697, 789.53448,   0.00000, 0.00000, 0.00000}
};
new TimerMensaje[MAX_PLAYERS],PlayerText:MensajeTD[MAX_PLAYERS],TieneRoca[MAX_PLAYERS],TimerMinando[MAX_PLAYERS],TimerSonido[MAX_PLAYERS],ProcesoPicando[MAX_PLAYERS];


new RandomColors[] =
{
	0xFFFFFFFF,
	0x000000FF,
    0x99CC00FF,
    0xFF9900FF,
	0x6699CCFF,
	0x0099FFFF,
	0xCCCC99FF,
	
	0xFF8C13FF,	0xC715FFFF,	0x20B2AAFF,	0xDC143CFF,	0x6495EDFF,	0xf0e68cFF,	0x778899FF,	0xFF1493FF,	0xF4A460FF,
	0xEE82EEFF,	0xFFD720FF,	0x8b4513FF,	0x4949A0FF,	0x148b8bFF,	0x14ff7fFF,	0x556b2fFF,	0x0FD9FAFF,	0x10DC29FF,
	0x534081FF,	0x0495CDFF,	0xEF6CE8FF,	0xBD34DAFF,	0x247C1BFF,	0x0C8E5DFF,	0x635B03FF,	0xCB7ED3FF,	0x65ADEBFF,
	0x5C1ACCFF,	0xF2F853FF,	0x11F891FF,	0x7B39AAFF,	0x53EB10FF,	0x54137DFF,	0x275222FF,	0xF09F5BFF,	0x3D0A4FFF,
	0x22F767FF,	0xD63034FF,	0x9A6980FF,	0xDFB935FF,	0x3793FAFF,	0x90239DFF,	0xE9AB2FFF,	0xAF2FF3FF,	0x057F94FF,
	0xB98519FF,	0x388EEAFF,	0x028151FF,	0xA55043FF,	0x0DE018FF,	0x93AB1CFF,	0x95BAF0FF,	0x369976FF,	0x18F71FFF,
	0x4B8987FF,	0x491B9EFF,	0x829DC7FF,	0xBCE635FF,	0xCEA6DFFF,	0x20D4ADFF,	0x2D74FDFF,	0x3C1C0DFF,	0x12D6D4FF,
	0x48C000FF,	0x2A51E2FF,	0xE3AC12FF,	0xFC42A8FF,	0x2FC827FF,	0x1A30BFFF,	0xB740C2FF,	0x42ACF5FF,	0x2FD9DEFF,
	0xFAFB71FF,	0x05D1CDFF,	0xC471BDFF,	0x94436EFF,	0xC1F7ECFF,	0xCE79EEFF,	0xBD1EF2FF,	0x93B7E4FF,	0x3214AAFF,
	0x184D3BFF,	0xAE4B99FF,	0x7E49D7FF,	0x4C436EFF,	0xFA24CCFF,	0xCE76BEFF,	0xA04E0AFF,	0x9F945CFF,	0xDCDE3DFF,
	0x10C9C5FF,	0x70524DFF,	0x0BE472FF,	0x8A2CD7FF,	0x6152C2FF,	0xCF72A9FF,	0xE59338FF,	0xEEDC2DFF,	0xD8C762FF,
	0xD8C762FF,	0xFF8C13FF,	0xC715FFFF,	0x20B2AAFF,	0xDC143CFF,	0x6495EDFF,	0xf0e68cFF,	0x778899FF,	0xFF1493FF,
	0xF4A460FF,	0xEE82EEFF,	0xFFD720FF,	0x8b4513FF,	0x4949A0FF,	0x148b8bFF,	0x14ff7fFF,	0x556b2fFF,	0x0FD9FAFF,
	0x10DC29FF,	0x534081FF,	0x0495CDFF,	0xEF6CE8FF,	0xBD34DAFF,	0x247C1BFF,	0x0C8E5DFF,	0x635B03FF,	0xCB7ED3FF,
	0x65ADEBFF,	0x5C1ACCFF,	0xF2F853FF,	0x11F891FF,	0x7B39AAFF,	0x53EB10FF,	0x54137DFF,	0x275222FF,	0xF09F5BFF,
	0x3D0A4FFF,	0x22F767FF,	0xD63034FF,	0x9A6980FF,	0xDFB935FF,	0x3793FAFF,	0x90239DFF,	0xE9AB2FFF,	0xAF2FF3FF,
	0x057F94FF,	0xB98519FF,	0x388EEAFF,	0x028151FF,	0xA55043FF,	0x0DE018FF,	0x93AB1CFF,	0x95BAF0FF,	0x369976FF,
	0x18F71FFF,	0x4B8987FF,	0x491B9EFF,	0x829DC7FF,	0xBCE635FF,	0xCEA6DFFF,	0x20D4ADFF,	0x2D74FDFF,	0x3C1C0DFF,
	0x12D6D4FF,	0x48C000FF,	0x2A51E2FF,	0xE3AC12FF,	0xFC42A8FF,	0x2FC827FF,	0x1A30BFFF,	0xB740C2FF,	0x42ACF5FF,
	0x2FD9DEFF,	0xFAFB71FF,	0x05D1CDFF,	0xC471BDFF,	0x94436EFF,	0xC1F7ECFF,	0xCE79EEFF,	0xBD1EF2FF,	0x93B7E4FF,
	0x3214AAFF,	0x184D3BFF,	0xAE4B99FF,	0x7E49D7FF,	0x4C436EFF,	0xFA24CCFF,	0xCE76BEFF,	0xA04E0AFF,	0x9F945CFF,
	0xDCDE3DFF,	0x10C9C5FF,	0x70524DFF,	0x0BE472FF,	0x8A2CD7FF,	0x6152C2FF,	0xCF72A9FF,	0xE59338FF,	0xEEDC2DFF,
	0xD8C762FF,	0xD8C762FF
};

forward OnSanAndreasVehicleLoad(vehicleid);
forward OnSanAndreasVehiclesLoad();


/* RESULT_YOUTUBE */
#define MAX_RESULTS	10
enum yt_result
{
	videoID[11 + 1],
	yt_title[100 + 1],
}

new RESULT_YOUTUBE[MAX_RESULTS][yt_result],
	PLAYER_DIALOG_MP3_RESULT[MAX_PLAYERS][MAX_RESULTS][yt_result];


/* SYSTEM - PROPERTIES */
forward OnDefaultPFurChanged(index);




#define MAX_CLOSET_SLOTS	10


//Taxis
enum TAXI_METER_VEHICLE_Info
{
	bool:veh_taxi_meter_ENABLED,
	veh_taxi_meter_PRICE,
	veh_taxi_meter_PAYMENT,
	Float:veh_taxi_meter_OLD_X,
	Float:veh_taxi_meter_OLD_Y,
	Float:veh_taxi_meter_OLD_Z,
	Float:veh_taxi_meter_DISTANCE,
	veh_taxi_meter_TIMER,
}
new TAXI_METER_VEHICLE[MAX_VEHICLES][TAXI_METER_VEHICLE_Info];



//Pizzero
new Float:PIZZA_DELIVERY_POINT[][] = 
{
	{2232.147460, -1785.597656, 13.560449, 82.756179    },
	{2290.242675, -1796.022583, 13.546875, 267.344024   },
	{1739.314575, -1582.890747, 14.158246, 174.783370   },
	{1694.923339, -1583.478881, 13.546875, 179.829818   },
	{1241.816406, -2032.996093, 60.041099, 263.972076   },
	{1187.268798, -1666.487915, 13.634441, 172.332916   },
	{998.998962, -1868.162231, 12.820312, 333.677734    },
	{891.677551, -1695.211669, 13.546875, 182.046417    },
	{507.502838, -1606.101684, 16.247840, 359.563903    },
	{305.462341, -1748.736816, 4.546875, 356.093139     },
	{159.034683, -1906.259521, 3.773437, 0.044234       },
	{387.668762, -2083.981689, 7.835937, 11.638330      },
	{902.655456, -930.055786, 42.601562, 185.516662     },
	{1809.700439, -1166.832885, 24.226562, 168.451568   },
	{2007.953857, -985.501953, 33.888450, 203.231872    },
	{2580.753906, -969.438110, 81.363517, 50.412715     },
	{2796.151855, -1088.867187, 30.719810, 234.631011   },
	{2819.019287, -1614.678588, 11.079871, 245.630599   },
	{2756.354492, -1979.312011, 13.547579, 270.360687   },
	{931.654541, -1231.159545, 16.928218, 262.568939    },
	{952.439208, -913.921447, 45.729900, 180.787841     },
	{824.417663, -2031.578857, 12.867187, 331.166229    },
	{1714.394531, -1629.683593, 20.203384, 332.709594   }
};

enum PIZZA_VEHICLE_Info
{
	bool:pizza_vehicle_STARTED,
	pizza_vehicle_DRIVER_AID,
	pizza_vehicle_PIZZAS,
	pizza_vehicle_PIZZA_POINT,
	bool:pizza_vehicle_PIZZA_POINT_DELI,
}
new PIZZA_VEHICLE[MAX_VEHICLES][PIZZA_VEHICLE_Info];

//Camioneros
new Float:LoadTrucksPoints[][] =
{
	{-520.48822, -502.04059, 25.26037, 0.0},
	{-529.98822, -502.04059, 25.26040, 0.0},
	{-557.48822, -502.04059, 25.26040, 0.0},

	{4471.7319, 5898.2646, 44.1051, 180.0},
	{4477.1011, 5898.2646, 44.1051, 180.0},
	{4481.8916, 5898.2646, 44.1051, 180.0}
};

enum TRUCK_VEHICLE_Info
{
	bool:truck_vehicle_LOADED,
	bool:truck_vehicle_LOADING,
	bool:truck_vehicle_UNLOADING,
	bool:truck_vehicle_DELIVERED,
	truck_vehicle_DRIVER_USER_ID,
	truck_vehicle_POINT,
	truck_vehicle_CURRENT_POINT,
	truck_vehicle_TOTAL_POINTS,
	Float:truck_vehicle_TOTAL_DISTANCE
}
new TRUCK_VEHICLE[MAX_VEHICLES][TRUCK_VEHICLE_Info];

new Float:Truck_Return_Positions[3] =
{
	-481.952117, -521.362060, 25.691177
};

enum Truck_Content_Info
{
	truck_content_EXP,
	truck_content_NAME[32],
	truck_content_MONEY,
	Float:truck_content_X,
	Float:truck_content_Y,
	Float:truck_content_Z
}
new Truck_Contents[][Truck_Content_Info] =
{
	{0, "Productos agr�colas", 950, -185.473693, -280.008880, 1.603357},
	{0, "Productos industriales", 1250, 2168.514404, -2275.264892, 13.128628},
	{0, "Madera y le�a", 1700, -2096.593017, -2243.229492, 30.352083},
	{0, "Ropa y complementos", 1975, 2834.454833, 958.061462, 10.479191},
	{25, "Productos de limpieza", 2250, 2524.153076, 2816.919189, 10.547390},
	{50, "Productos de pesca", 2760, -1789.979858, 1421.842895, 6.914581},
	{50, "Productos qu�micos", 3560, -1862.919433, -202.232727, 18.125516},
	{80, "Productos tecnol�gicos", 3850, 1039.057983, 2132.961425, 10.547390}
};

/* Mec�nico */
new Float:MechanicStartWorkingCoords[1][3] =
{
	{1833.056762, -1443.123413, 13.593692}
};

new Float:MechanicBuyPiecesCoords[1][3] =
{
	{1825.932373, -1447.018798, 13.588473}
};

new Float:MechanicBuyKitsCoords[][3] = {
	{1834.046508, -1436.194213, 13.594635}
};

/* Le�ador */
enum
{
	LUMBER_TREE_STATE_NORMAL,
	LUMBER_TREE_STATE_CUTTING,
	LUMBER_TREE_STATE_CUTTED,
}
enum LUMBER_TREES_Info
{
	lumber_tree_MODELID,
	Float:lumber_tree_X,
	Float:lumber_tree_Y,
	Float:lumber_tree_Z,
	Float:lumber_tree_RX,
	Float:lumber_tree_RY,
	Float:lumber_tree_RZ,
	lumber_tree_OBJECT_ID,
	Text3D:lumber_tree_LABEL,
	lumber_tree_STATE,
	lumber_tree_GROW_TIMER,
	lumber_tree_GROW_COUNTER,
}
new LUMBER_TREES[][LUMBER_TREES_Info] =
{
	{657, -467.31256, -47.68515, 59.00023,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -480.88443, -53.57695, 59.39736,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -491.79199, -45.14817, 59.24156,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -493.84619, -54.35930, 59.48059,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -473.43958, -40.64830, 59.00023,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -468.32245, -61.54378, 58.98302,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -458.69937, -75.32365, 58.80302,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -453.17416, -65.31260, 58.98302,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -444.92609, -78.80465, 57.97836,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -465.78470, -85.49714, 58.96302,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -454.24377, -86.58199, 58.46635,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -440.20041, -92.70150, 57.60720,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -484.98972, -65.79082, 59.82284,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -460.57928, -95.55928, 59.25615,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -521.20569, -44.74903, 60.62712,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -534.94348, -50.98387, 61.66861,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -533.45081, -41.45306, 61.64992,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -495.74197, -64.77782, 59.94119,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -519.74005, -72.02750, 61.19377,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1},
	{657, -518.47144, -57.24505, 60.76774,   0.00000, 0.00000, 0.00000, INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, LUMBER_TREE_STATE_NORMAL, -1, -1}
};

/* Basurero */



enum Trash_Object_Info
{
	bool:trash_route_VALID,
	trash_route_OBJECTS,
	Float:trash_route_DISTANCE,
	trash_route_MONEY,
	trash_object_MODELID[MAX_OBJECTS_PER_ROUTE],
	Float:trash_object_X[MAX_OBJECTS_PER_ROUTE],
	Float:trash_object_Y[MAX_OBJECTS_PER_ROUTE],
	Float:trash_object_Z[MAX_OBJECTS_PER_ROUTE],
	Float:trash_object_RX[MAX_OBJECTS_PER_ROUTE],
	Float:trash_object_RY[MAX_OBJECTS_PER_ROUTE],
	Float:trash_object_RZ[MAX_OBJECTS_PER_ROUTE],
}
new TRASH_OBJECTS[MAX_ROUTES][Trash_Object_Info];
new TRASH_PLAYER_OBJECTS[MAX_PLAYERS][MAX_OBJECTS_PER_ROUTE];

enum TRASH_VEHICLES_INFO
{
	bool:trash_vehicle_JOB_STARTED, 
	trash_vehicle_ROUTE,
	trash_vehicle_CP,
	trash_vehicle_DRIVER_ID,
	trash_vehicle_PASSENGER_ID,
}
new TRASH_VEHICLES[MAX_VEHICLES][TRASH_VEHICLES_INFO];

/* SYSTEM - PLAYERTEMP */
enum
{
	PED_STOPPED,
	PED_WALKING,
	PED_RUNNING,
	PED_SPRINTING,
	PED_BYCICLING,
}

#define MAX_ADS_PER_PLAYER	3
enum
{
	PLAYER_SHOP_STATE_ALL,
	PLAYER_SHOP_STATE_AD,
	PLAYER_SHOP_STATE_MY_ADS,
}

enum
{
	SHOP_ARTICLE_TYPE_PROPERTY,
	SHOP_ARTICLE_TYPE_VEHICLE,
	SHOP_ARTICLE_TYPE_OTHER,
}

enum
{
	MECHANIC_OPTION_REPAIR,
	MECHANIC_OPTION_COLOR_1,
	MECHANIC_OPTION_COLOR_2,
	MECHANIC_OPTION_TUNING,
	MECHANIC_OPTION_REMOVE_COMPONEN,
	MECHANIC_OPTION_PAINTJOB,
}



enum enum_JAIL_POSITIONS
{
	jail_INT,
	Float:jail_X,
	Float:jail_Y,
	Float:jail_Z,
	Float:jail_ANGLE,
	jail_INTERIOR,
}
new JAIL_POSITIONS[][enum_JAIL_POSITIONS] = 
{
	{INTERIOR_POLICE_LS, 265.070129, 77.518280, 1001.039062, 270.0, 6}, //jaills
	{INTERIOR_POLICE_SF, 215.681945, 110.177680, 999.015625, 0.0, 10}, //jailsf
	{INTERIOR_POLICE_LV, 197.925323, 174.997573, 1003.023437, 0.0, 3} //jaillv
};

enum BUY_PROPERTIES_SITES_INFO
{
	site_INTERIOR_TYPE,
	Float:site_X,
	Float:site_Y,
	Float:site_Z,
	site_WORLD,
	site_INTERIOR,
}
new BUY_PROPERTIES_SITES[][BUY_PROPERTIES_SITES_INFO] =
{
	{INTERIOR_BANK_LS, 1396.613403, 1320.551635, 1501.090698, 5, 5},
	{INTERIOR_BANK_SF, 1116.351684, 1540.468750, 1451.827148, 10, 10},
	{INTERIOR_BANK_LV, 2681.990966, -615.470520, -71.658203, 15, 15}
};



enum
{
	PRODUCT_TYPE_NONE,
	PRODUCT_TYPE_FOOD,
	PRODUCT_TYPE_PHONE,
	PRODUCT_TYPE_GPS,
	PRODUCT_TYPE_MP3,
	PRODUCT_TYPE_PHONE_RESOLVER,
	PRODUCT_TYPE_SPEAKERS,
}
enum Supermarket_Enum
{
	product_TYPE,
	product_NAME[24],
	product_PRICE,
	Float:product_HUNGRY,
	Float:product_THIRST,
	product_DRUNK,
}
new Supermarket_Product_List[][Supermarket_Enum] =  
{
	{PRODUCT_TYPE_PHONE, "Tel�fono", 350, 0.0, 0.0, 0},
	{PRODUCT_TYPE_PHONE_RESOLVER, "Gu�a telef�nica", 200, 0.0, 0.0, 0},
	{PRODUCT_TYPE_GPS, "Localizador GPS", 300, 0.0, 0.0, 0},
	{PRODUCT_TYPE_MP3, "Reproductor MP3", 200, 0.0, 0.0, 0},
	{PRODUCT_TYPE_SPEAKERS, "Altavoces", 200, 0.0, 0.0, 0},
	{PRODUCT_TYPE_FOOD, "Botella de agua mineral", 80, 0.0, 25.0, 0},
	{PRODUCT_TYPE_FOOD, "Lata de refresco Sprunk", 40, 0.0, 12.0, 0},
	{PRODUCT_TYPE_FOOD, "Lata de refresco cola", 30, 0.0, 10.0, 0},
	{PRODUCT_TYPE_FOOD, "Barrita energetica", 80, 15.0, 0.0, 0},
	{PRODUCT_TYPE_FOOD, "Porcion de pizza", 60, 20.0, 0.0, 0},
	{PRODUCT_TYPE_FOOD, "Ensalada", 50, 15.0, 2.0, 0},
	{PRODUCT_TYPE_FOOD, "Sandwich", 45, 10.0, 2.0, 0},
	{PRODUCT_TYPE_FOOD, "Lata de cerveza", 40, 0.0, 3.0, 1000},
	{PRODUCT_TYPE_FOOD, "Vodka", 80, 0.0, 3.0, 2500},
	{PRODUCT_TYPE_FOOD, "Whisky", 200, 0.0, 3.0, 3000}
};


enum Clothing_Shop_Info
{
	clothing_shop_INTERIOR_TYPE,
	clothing_shop_NAME[24],
	Float:clothing_shop_X,
	Float:clothing_shop_Y,
	Float:clothing_shop_Z,
	Float:clothing_shop_ANGLE,
	clothing_shop_INTERIOR,
}
new Clothing_Shop_Positions[][Clothing_Shop_Info] =  
{ 
	{INTERIOR_BINCO, "Binco", 207.733657, -100.633468, 1005.257812, 180.0, 15}, //Binco
	{INTERIOR_SUBURBAN, "SubUrban", 203.905395, -43.450450, 1001.804687, 180.0, 1}, //SubUrban
	{INTERIOR_PROLAPS, "ProLaps", 207.049148, -129.177581, 1003.507812, 180.0, 3}, //prolaps
	{INTERIOR_DIDIER_SACHS, "Didier Sachs", 204.348281, -159.493728, 1000.523437, 180.0, 14}, //Didier
	{INTERIOR_VICTIM, "Victim", 206.374328, -7.241514, 1001.210937, 270.0, 5}, //Victim
	{INTERIOR_ZIP, "ZIP", 161.443634, -83.589271, 1001.804687, 180.0, 18} //Zip
};


enum Fast_Food_Info
{
	fast_food_INTERIOR_TYPE,
	fast_food_NAME[24],
	Float:fast_food_X,
	Float:fast_food_Y,
	Float:fast_food_Z,
	fast_food_INTERIOR,
}
new Fast_Food_Positions[][Fast_Food_Info] =
{
	{INTERIOR_BURGER_SHOT, "Burger Shot", 375.913238, -67.435272, 1001.507812, 10}, // BurgerShot
	{INTERIOR_PIZZA, "Well Stacked Pizza", 375.254730, -118.804710, 1001.499511, 5}, //Pizza
	{INTERIOR_CLUCKIN_BELL, "Cluckin Bell", 368.757904, -6.018908, 1001.851562, 9} //Cluckin Bell
};


enum Food_Info
{
	food_NAME[24],
	food_PRICE,
	Float:food_HUNGRY,
	Float:food_THIRST,
}
new PIZZA_FOOD[][Food_Info] =
{
	{"Pizza de verduras", 25, 10.0, -0.1},
	{"Pizza de jam�n y queso", 60, 16.0, -0.1},
	{"Pizza barbacoa", 40, 15.0, -0.1},
	{"Pizza 4 quesos", 55, 14.0, -0.1},
	{"Ensalada", 20, 10.0, 5.0},
	{"Refresco de Sprunk", 30, 0.0, 12.0},
	{"Refresco de Cola", 50, 0.0, 13.5},
	{"Agua mineral", 30, 0.0, 19.0}
};
new BURGER_SHOT_FOOD[][Food_Info] =
{
	{"Hamburguesa de ternera", 60, 16.0, -0.1},
	{"Hamburguesa de pollo", 80, 10.0, -0.1},
	{"Hamburguesa especial", 60, 12.0, -3.0},
	{"Ensalada", 20, 10.0, 5.0},
	{"Refresco de Sprunk", 30, 0.0, 12.0},
	{"Refresco de Cola", 50, 0.0, 13.5},
	{"Agua mineral", 30, 0.0, 19.0}
};
new CLUCKIN_BELL_FOOD[][Food_Info] =
{
	{"Alitas de pollo", 60, 16.0, -0.1},
	{"Plato especial", 80, 18.0, -2.0},
	{"Ensalada", 20, 10.0, 5.0},
	{"Refresco de Sprunk", 30, 0.0, 12.0},
	{"Refresco de Cola", 50, 0.0, 13.5},
	{"Agua mineral", 30, 0.0, 19.0}
};

enum
{
	PICKUP_TYPE_NONE = 0,
	PICKUP_TYPE_ENTER_EXIT,
	PICKUP_TYPE_ATM,
	PICKUP_TYPE_PROPERTY,
	PICKUP_TYPE_HOTDOG
};

new Intro_Skins[][] =
{
	{ // HOMBRE 	
		170,
		158,
		184,
		220,
		183,
		73
	},
	
	{ // MUJER
		192,
		40, 
		226,
		41, 
		13, 
		298,
	}
};

enum E_Intro_Info {
	Float:intro_info_CAM_X,
	Float:intro_info_CAM_Y,
	Float:intro_info_CAM_Z,
	Float:intro_info_CAM_RX,
	Float:intro_info_CAM_RY,
	Float:intro_info_CAM_RZ,
	Float:intro_info_CAM_LOOK_AT_X,
	Float:intro_info_CAM_LOOK_AT_Y,
	Float:intro_info_CAM_LOOK_AT_Z,
	Float:intro_info_CAM_LOOK_AT_RX,
	Float:intro_info_CAM_LOOK_AT_RY,
	Float:intro_info_CAM_LOOK_AT_RZ,
	intro_info_HOUR,
	intro_info_WEATHER,
};

new Intro_Info[][E_Intro_Info] = {
	{313.236328, 1128.530761, 9.520181, 318.766510, 1130.892089, 10.157177, 310.896820, 1132.935791, 9.169703, 314.989013, 1134.086425, 9.431639, 5, 1},
	{408.495666, 2512.417968, 18.166431, 409.920318, 2505.000732, 17.877164, 413.448150, 2512.634277, 17.513612, 414.895172, 2505.338134, 17.507190, 20, 1},
	{49.753929, -240.485778, 2.406555, 53.778308, -237.900802, 3.012162, 45.922225, -237.391876, 1.542942, 49.144725, -236.385101, 1.901962, 16, 1}
};

new Intro_Audios[][] = {
	"https://files.yourwebsite/audio/intro/intro1.mp3",
	"https://files.yourwebsite/audio/intro/intro2.mp3",
	"https://files.yourwebsite/audio/intro/intro3.mp3"
};






// CONCEPTOS	
enum RuleInfo
{
	Rule_Title[64],
	Rule_Info[512],
	Rule_Pages,
}


#define MIN_PASS_LENGTH	6
#define MAX_PASS_LENGTH	18


forward OnPlayerRegister(playerid);
forward OnPlayerLogin(playerid);


#define MAX_PLAYER_POCKET_OBJECTS 10
enum Player_Pocket_Enum
{
	bool:player_pocket_VALID,
	player_pocket_object_ID,
	player_pocket_object_NAME[24],
	Float:player_pocket_object_HUNGRY,
	Float:player_pocket_object_THIRST,
	player_pocket_object_DRUNK,
}
new PLAYER_POCKET[MAX_PLAYERS][MAX_PLAYER_POCKET_OBJECTS][Player_Pocket_Enum]; // PP (PlayerPocket)


new 
	DIALOG_FOOD_PIZZA_String[800],
	DIALOG_FOOD_CLUCKIN_String[800],
	DIALOG_FOOD_BURGER_String[800],
	DIALOG_247_LIST_String[34 + 55 * sizeof Supermarket_Product_List],
	DIALOG_ANIMS_String[1000],
	DIALOG_PLAYER_GPS_SITE_7_String[90 * (sizeof(SELL_VEHICLES_SHOPS_SPAWN) + 1)],
	DIALOG_CREATE_CREW_COLOR_String[4200],
	DIALOG_CREDIT_String[2000]
;

#define HidePlayerDialog(%1)		ShowPlayerDialog(%1, -1, 0, " ", " ", " ", " ")



enum enum_BLACK_MARKT_WEAPONS
{
	black_market_EXP,
	black_market_WEAPON_ID,
	black_market_WEAPON_PRICE_BASE,
	black_market_WEAPON_PRICE,
}
new BLACK_MARKT_WEAPONS[][enum_BLACK_MARKT_WEAPONS] = 
{
	{0, 4, 100, 0},
	{0, 5, 100, 0},
	{0, 10, 50, 0},
	{0, 22, 600, 0},
	{0, 23, 650, 0},
	{100, 24, 1200, 0},
	{100, 33, 1500, 0},
	{100, 25, 1600, 0},
	{200, 26, 2200, 0},
	{200, 27, 2800, 0},
	{200, 28, 2500, 0},
	{200, 32, 2800, 0},
	{250, 29, 3500, 0},
	{250, 30, 3600, 0},
	{250, 31, 3800, 0},
	{300, 34, 6500, 0}
};



