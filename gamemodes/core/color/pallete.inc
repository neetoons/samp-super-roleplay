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

#if defined pallete_included 
    #endinput
#endif
#define pallete_included

/* Colores */
#define COLOR_FADE1 	0xE6E6E6E6
#define COLOR_FADE2 	0xC8C8C8C8
#define COLOR_FADE3 	0xAAAAAAAA
#define COLOR_FADE4 	0x8C8C8C8C
#define COLOR_FADE5 	0x6E6E6E6E
#define PLAYER_COLOR	0xCCCCCC00

stock HexToRGBA(colour, &r, &g, &b, &a) //By Betamaster
{
    r = (colour >> 24) & 0xFF;
    g = (colour >> 16) & 0xFF;
    b = (colour >> 8) & 0xFF;
    a = colour & 0xFF;
}

stock RGBAToHex(r, g, b, a) //By Betamaster
{
    return (r<<24 | g<<16 | b<<8 | a);
}

stock RGBAToARGB(col)
{
	return ((((col) << 24) & 0xFF000000) | (((col) >>> 8) & 0xFFFFFF));
}

stock ARGBToRGBA(col)
{
	return ((((col) << 8) & 0xFFFFFF00) | (((col) >>> 24) & 0xFF));
}