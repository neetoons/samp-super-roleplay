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
#if defined info_vars_included 
	#endinput
#endif
#define info_vars_included

#include <YSI_Coding\y_hooks>

new
    JobsPointsLastReset,
	WarehouseOrderNum
;

forward OnInfoVarsLoaded();

hook OnDatabaseConnected() {
    LoadInfoVars();
}

LoadInfoVars()
{
	new File:file = fopen("SERVER/INFO.txt", io_read), string[256];
	if(file)
	{
		fread(file, string);
		sscanf(string, "p<,>dd", JobsPointsLastReset, WarehouseOrderNum);
		fclose(file);
	}
	CallLocalFunction("OnInfoVarsLoaded", "");
}

SaveInfoVars()
{
	new File:file = fopen("SERVER/INFO.txt", io_write), string[256];
	if(file)
	{
		format(string, sizeof string, "%d,%d", JobsPointsLastReset, WarehouseOrderNum);
		fwrite(file, string);
		fclose(file);
	}
	return 1;
}