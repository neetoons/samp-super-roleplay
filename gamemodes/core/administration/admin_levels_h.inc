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

#if defined levels_header_included 
    #endinput
#endif
#define levels_header_included


new ADMIN_LEVEL_AC_IMMUNITY = 3; //nivel 3 en adelante

enum
{
	TYPE_WARNING,
	TYPE_KICK,
	TYPE_BAN,
	TYPE_TEMP_BAN,
	TYPE_UNBAN
}

enum
{
	CMD_USER,
	CMD_HELPER,
	CMD_HELPER_MODERATOR,
	CMD_MODERATOR,
	CMD_ADMINISTRATOR,
	CMD_DEVELOPER,
	CMD_DISABLED,
}
new ADMIN_LEVELS[][] = 
{
	"Ciudadano",
	"Ayudante",
	"Moderador",
	"Operador",
	"Administrador",
	"Desarrollador"
};

