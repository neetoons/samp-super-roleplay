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
#if defined hospital_h_included 
	#endinput
#endif
#define hospital_h_included

enum E_HOSPITAL_SPAWN_INFO
{
	Float:hspi_X,
	Float:hspi_Y,
	Float:hspi_Z,
	Float:hspi_ANGLE,
}

new Hospital_Spawn_Positions[][E_HOSPITAL_SPAWN_INFO] =  
{ 
	{2034.197875, -1403.040039, 17.294845, 180.0},
	{1172.832763, -1323.269531, 15.400051, 270.0},
	{1242.235595, 328.113281, 19.755510, 0.0},
	{-2201.566162, -2307.610839, 30.625000, 320.0},
	{-2655.108886, 639.278625, 14.453125, 180.0},
	{-318.812774, 1050.359130, 20.340259, 0.0},
	{-1514.855590, 2520.366699, 55.909828, 0.0},
	{1583.311401, 1768.952758, 10.820312, 90.0}
};

new Float:Hospital_Cameras_Positions[][] =
{
	{2007.3301, -1442.3809, 26.7332, 2008.0043, -1441.6445, 26.4982},
	{1209.2427, -1314.4705, 23.4945, 1208.5284, -1315.1681, 23.3195},
	{1260.2025, 342.6251, 22.3913, 1259.4681, 341.9478, 22.2512},
	{-2200.5859, -2316.3057, 31.7932, -2200.8340, -2315.3367, 31.6932},
	{-2530.8450, 568.0942, 29.4603, -2531.6973, 568.6146, 29.4553},
	{-305.5014, 1068.6696, 24.2657, -306.1943, 1067.9514, 24.0607},
	{-1504.2892, 2553.1243, 61.1113, -1504.5568, 2552.1619, 60.8414},
	{1624.5881, 1837.4691, 12.5740, 1623.9382, 1836.7124, 12.4690}
};