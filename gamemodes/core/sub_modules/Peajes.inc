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
#if defined peajes_included 
    #endinput
#endif
#define peajes_included

#include <a_samp>
#include <zcmd>
#include <streamer>
#define FILTERSCRIPT

new Puerta;
new Puerta2;
new Puerta3;

////EJEMPLO:
// new Puertatecsharp;

#define PRESSED(%0) (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))
#define RELEASED(%0) (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))

forward PuertaCheck();
forward Cerrar();
//=============================================================//
public OnFilterScriptInit()
{
    Puerta = CreateObject(975, -19.917915, 2070.967529, 18.555311, 0.000000, 0.000000, 88.300018); //CERRADA// Puerta 1 Punto 1
    Puerta2 = CreateObject(975, -19.730997, 2077.265625, 18.555311, 0.000000, 0.000000, 88.300018);//CERRADA   Puerta 2 Punto 2
    Puerta3 = CreateObject(971, 135.523010, 2021.813842, 13.507822, 0.000000, 0.000000, 0.000000);//ABIERTA    Puerta 1 Punto 3
    return 1;
}
////////////////////////////////////COMIENZO PORTONES /////////////////////////////////////////////////////////////

CMD:porton1(playerid, params[]) //Para abrir porton 1 y cerrar 2
{
if(IsPlayerInRangeOfPoint(playerid, 20.0, -19.8663, 2066.2559, 17.7980)) //Lugar para abrir 1

MoveObject(Puerta, -20.181333, 2062.090087, 18.555311, 3.0); //ABIERTO//
MoveObject(Puerta2, -20.360233, 2056.061523, 18.555311, 3.0); //ABIERTO//
MoveObject(Puerta3, 135.523010, 2021.813842, 21.027832, 3.0); //CERRADO//
SendClientMessage(playerid, -1, "Cabo Scorpion dice: Puede continuar.");
return 1;
}
CMD:porton2(playerid, params[]) //Para abrir porton 1 y cerrar 2
{

if(IsPlayerInRangeOfPoint(playerid, 20.0, 135.2876, 2023.1539, 18.5495)){ //Lugar para abrir 2
MoveObject(Puerta, -19.917915, 2070.967529, 18.555311, 3.0); //CERRADO//
MoveObject(Puerta2, -19.730997, 2077.265625, 18.555311, 3.0); //CERRADO//
MoveObject(Puerta3, 135.523010, 2021.813842, 13.507822, 3.0); //ABIERTO//
}else return SendClientMessage(playerid, -1, "");
return 1;
}

//=============================================================//
public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
if(newkeys & KEY_CROUCH)   //Preciona la Tecla H desde un vehiculo para abrir los portones
    {
	 new params[1];
	 {
	 cmd_porton1(playerid, params);  //Porton 1
	 cmd_porton2(playerid, params);  //Porton 2
	 }
	 return 1;
  }
return 1;
}
//=============================================================//
public OnPlayerCommandText(playerid, cmdtext[])
{/*
////Puerta 1////
if (strcmp("/irsssf1", cmdtext, true) == 0) //cerrar
{

if(IsPlayerInRangeOfPoint(playerid, 3.0, 67.6197, -1526.4147, 4.6018)){ //Lugar para abrir 1 ))//si el jugador no esta en un radio de 15.0 a la puerta no lo dejara usar el comando. (Reemplazar las coordenadas por las coordenadas de nuestra puerta (CERRADA))
MoveObject(Puerta, 44.394412, -1528.334960, 4.349106, 3.0); //CERRADO//mueve nuestra puerta hasta las coordenadas de la puerta abierta. (Reemplazar las coordenadas por las coordenadas de nuestra puerta
MoveObject(Puerta4, 60.757301, -1537.011962, 4.199104, 3.0); //ABIERTO//mueve nuestra puerta hasta las coordenadas de la puerta abierta. (Reemplazar las coordenadas por las coordenadas de nuestra puerta
}else return SendClientMessage(playerid, -1, "");
}
////PuertaS 2////
if (strcmp("/irllls2", cmdtext, true) == 0) //para abrir la puerta
{

if(IsPlayerInRangeOfPoint(playerid, 3.0, 55.7807, -1539.2687, 4.8389)){ //si el jugador no esta en un radio de 15.0 a la puerta no lo dejara usar el comando. (Reemplazar las coordenadas por las coordenadas de nuestra puerta (CERRADA))
if(GetPlayerMoney(playerid) < 100 ) return SendClientMessage(playerid, -1, "No tienes dinero para Pasar por el {FFC400}Peaje");

GivePlayerMoney(playerid, -100 );
MoveObject(Puerta2, 42.274993, -1542.324584, 4.349106, 3.0); //CERRADA//mueve nuestra puerta hasta las coordenadas de la puerta abierta. (Reemplazar las coordenadas por las coordenadas de nuestra puerta
MoveObject(Puerta3, 60.927021, -1537.573364, 4.239104, 3.0); //ABIERTO//mueve nuestra puerta hasta las coordenadas de la puerta abierta. (Reemplazar las coordenadas por las coordenadas de nuestra puerta
return SendClientMessage(playerid, -1, "{B381FF}Que tenga un buen Viaje!");
}else return SendClientMessage(playerid, -1, "");
}

if (strcmp("/irllls1", cmdtext, true) == 0) //cerrar
{

if(IsPlayerInRangeOfPoint(playerid, 3.0, 37.9863, -1536.8518, 4.9599)){ //si el jugador no esta en un radio de 15.0 a la puerta no lo dejara usar el comando. (Reemplazar las coordenadas por las coordenadas de nuestra puerta (CERRADA))
MoveObject(Puerta2, 43.665660, -1534.892700, 4.349106, 3.0); //ABIERTO//mueve nuestra puerta hasta las coordenadas de la puerta abierta. (Reemplazar las coordenadas por las coordenadas de nuestra puerta
MoveObject(Puerta3, 59.670658, -1544.287719, 4.239104, 3.0); //CERRADO//mueve nuestra puerta hasta las coordenadas de la puerta abierta. (Reemplazar las coordenadas por las coordenadas de nuestra puerta
}else return SendClientMessage(playerid, -1, "");
}
////Puerta 3////
if (strcmp("/apuerta3", cmdtext, true) == 0) //para abrir la puerta
{

if(IsPlayerInRangeOfPoint(playerid, 15.0, 0.0000,0.0000,0.0000)){ //si el jugador no esta en un radio de 15.0 a la puerta no lo dejara usar el comando. (Reemplazar las coordenadas por las coordenadas de nuestra puerta (CERRADA))
MoveObject(Puerta3, 0.0000,0.0000,0.0000, 3.0); //mueve nuestra puerta hasta las coordenadas de la puerta abierta. (Reemplazar las coordenadas por las coordenadas de nuestra puerta (ABIERTA))
return SendClientMessage(playerid, 0x28F943FF, "Server: Has abierto la puerta");
}else return SendClientMessage(playerid, 0xFF0000FF, "ERROR: Debes estar cerca de la puerta para usar este comando");
}

if (strcmp("/cpuerta3", cmdtext, true) == 0) //cerrar
{

if(IsPlayerInRangeOfPoint(playerid, 15.0, 0.0000,0.0000,0.0000)){ //si el jugador no esta en un radio de 15.0 a la puerta no lo dejara usar el comando. (Reemplazar las coordenadas por las coordenadas de nuestra puerta (CERRADA))
MoveObject(Puerta3, 0.0000,0.0000,0.0000, 3.0); //mueve nuestra puerta hasta las coordenadas de la puerta abierta. (Reemplazar las coordenadas por las coordenadas de nuestra puerta (CERRADA))
return SendClientMessage(playerid, 0x28F943FF, "Server: Has cerrado la puerta");
}else return SendClientMessage(playerid, 0xFF0000FF, "ERROR: Debes estar cerca de la puerta para usar este comando");
}
////Puerta 4////
if (strcmp("/apuerta4", cmdtext, true) == 0) //para abrir la puerta
{

if(IsPlayerInRangeOfPoint(playerid, 15.0, 0.0000,0.0000,0.0000)){ //si el jugador no esta en un radio de 15.0 a la puerta no lo dejara usar el comando. (Reemplazar las coordenadas por las coordenadas de nuestra puerta (CERRADA))
MoveObject(Puerta4, 0.0000,0.0000,0.0000, 3.0); //mueve nuestra puerta hasta las coordenadas de la puerta abierta. (Reemplazar las coordenadas por las coordenadas de nuestra puerta (ABIERTA))
return SendClientMessage(playerid, 0x28F943FF, "Server: Has abierto la puerta");
}else return SendClientMessage(playerid, 0xFF0000FF, "ERROR: Debes estar cerca de la puerta para usar este comando");
}

if (strcmp("/cpuerta4", cmdtext, true) == 0) //cerrar
{

if(IsPlayerInRangeOfPoint(playerid, 15.0, 0.0000,0.0000,0.0000)){ //si el jugador no esta en un radio de 15.0 a la puerta no lo dejara usar el comando. (Reemplazar las coordenadas por las coordenadas de nuestra puerta (CERRADA))
MoveObject(Puerta4, 0.0000,0.0000,0.0000, 3.0); //mueve nuestra puerta hasta las coordenadas de la puerta abierta. (Reemplazar las coordenadas por las coordenadas de nuestra puerta (CERRADA))
return SendClientMessage(playerid, 0x28F943FF, "Server: Has cerrado la puerta");
}else return SendClientMessage(playerid, 0xFF0000FF, "ERROR: Debes estar cerca de la puerta para usar este comando");
*/
return 0;
}
