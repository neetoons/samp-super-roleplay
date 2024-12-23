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
#if defined atem_h_included 
	#endinput
#endif
#define atem_h_included

enum atm_bank_info
{
	atm_modelid,
	Float:atm_X,
	Float:atm_Y,
	Float:atm_Z,
	Float:atm_RX,
	Float:atm_RY,
	Float:atm_RZ
}
new ATM_BANK[][atm_bank_info] = 
{
	{2754, 2833.38672, 2402.91553, 10.93546,   0.00000, 0.00000, 131.44171},
	{2754, 2088.78442, 1510.00574, 10.69234,   0.00000, 0.00000, 45.39682},
	{2754, 1662.92346, 1723.03162, 10.53258,   0.00000, 0.00000, 90.27153},
	{2754, -16.17070, 1219.33813, 19.17369,   0.00000, 0.00000, 179.53238},
	{2754, -1505.91687, 2612.09058, 55.63910,   0.00000, 0.00000, 178.85609},
	{2754, -2540.67188, 2340.96143, 4.79166,   0.00000, 0.00000, 90.97625},
	{2754, -1690.23547, 1327.11902, 7.04427,   0.00000, 0.00000, 43.44909},
	{2754, -1980.48608, 155.24883, 27.49554,   0.00000, 0.00000, 359.06995},
	{2754, -2631.60693, -6.86652, 5.97444,   0.00000, 0.00000, 181.99303},
	{2754, -2146.54395, -2332.90283, 30.43888,   0.00000, 0.00000, 232.48808},
	{2754, 574.90155, -1419.96509, 14.22211,   0.00000, 0.00000, 283.89526},
	{2754, 1227.24524, -1428.54846, 13.29063,   0.00000, 0.00000, 269.20294},
	{2754, 1810.57153, -1873.97852, 13.38759,   0.00000, 0.00000, 180.44339},
	{2754, 1493.07471, -1022.08618, 23.63997,   0.00000, 0.00000, 91.19178},
	{2754, 2330.78149, 81.79469, 26.33336,   0.00000, 0.00000, 269.20883},
	{2754, 711.71503, -542.69287, 16.04271,   0.00000, 0.00000, 180.17184},
	{2754, 242.68755, -54.72442, 1.31352,   0.00000, 0.00000, 0.00000},
	{2754, 1381.12219, 233.81693, 19.28907,   0.00000, 0.00000, 154.79193}
};

enum
{
	BANK_TRANSACTION_WITHDRAW,
	BANK_TRANSACTION_DEPOSIT,
	BANK_TRANSACTION_TRANSFER,
	BANK_TRANSACTION_CREATE_ACCOUNT,
	BANK_TRANSACTION_BUY_PROPERTY,
	BANK_TRANSACTION_SOLD_PROPERTY,
	BANK_TRANSACTION_BUY_VEHICLE,
	BANK_TRANSACTION_SOLD_VEHICLE
};
enum Bank_Interior_Info
{
	bank_INTERIOR_TYPE,
	Float:bank_X,
	Float:bank_Y,
	Float:bank_Z,
	bank_WORLD,
	bank_INTERIOR,
}
new Bank_Interior_Positions[][Bank_Interior_Info] =
{
	{INTERIOR_BANK_LS, 1407.356201, 1320.171142, 1501.083984, 5, 5},
	{INTERIOR_BANK_SF, 1105.033935, 1529.565795, 1452.480957, 10, 10},
	{INTERIOR_BANK_LV, 2690.869628, -610.943664, -71.658203, 15, 15}
};