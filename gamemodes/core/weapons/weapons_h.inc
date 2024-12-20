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

#if defined weapons_h_included 
    #endinput
#endif
#define weapons_h_included

// PLAYER WEAPONS

#define NO_WEAPON 0
enum {
	WEAPON_SHIELD = 300,
	WEAPON_TASER_GUN
};
enum enum_WEAPON_INFO
{
	bool:weapon_info_SPECIAL,
	bool:weapon_info_AC_CBUG,
	bool:weapon_info_ANTI_CHEAT,
	bool:weapon_info_AMMO,
	weapon_info_TYPE,
	weapon_info_NAME[32],
	weapon_info_SLOT,
	weapon_info_SHOT_TIME,
	weapon_info_MODEL,
	bool:weapon_info_DROP
}
new WEAPON_INFO[][enum_WEAPON_INFO] = 
{
	{false, false, false, false, NO_WEAPON, "Puño", 0, 250, -1, false},
	{false, false, true, false, WEAPON_BRASSKNUCKLE, "Manopla", 0, 250, 331, true},
	{false, false, true, false, WEAPON_GOLFCLUB, "Palo de golf", 1, 250, 333, true},
	{false, false, true, false, WEAPON_NITESTICK, "Porra", 1, 250, 334, false},
	{false, false, true, false, WEAPON_KNIFE, "Cuchillo", 1, 250, 335, true},
	{false, false, true, false, WEAPON_BAT, "Bate", 1, 250, 336, true},
	{false, false, true, false, WEAPON_SHOVEL, "Pala", 1, 250, 337, true},
	{false, false, true, false, WEAPON_POOLSTICK, "Taco", 1, 250, 338, true},
	{false, false, true, false, WEAPON_KATANA, "Katana", 1, 250, 339, true},
	{false, false, true, false, WEAPON_CHAINSAW, "Motosierra", 1, 30, 341, true},
	{false, false, true, false, WEAPON_DILDO, "Dildo morado", 10, 250, 321, true},
	{false, false, true, false, WEAPON_DILDO2, "Dildo", 10, 250, 322, true},
	{false, false, true, false, WEAPON_VIBRATOR, "Vibrador", 10, 250, 323, true},
	{false, false, true, false, WEAPON_VIBRATOR2, "Vibrador plateado", 10, 250, 324, true},
	{false, false, true, false, WEAPON_FLOWER, "Flores", 10, 250, 325, true},
	{false, false, true, false, WEAPON_CANE, "Bastón", 10, 250, 326, true},
	{false, false, true, true, WEAPON_GRENADE, "Granada", 8, 500, 342, true},
	{false, false, true, true, WEAPON_TEARGAS, "Gas lacrimógeno", 8, 500, 343, true},
	{false, false, true, true, WEAPON_MOLTOV, "Cocktail Molotov", 8, 500, 344, true},
	{false, false, false, false, NO_WEAPON, "", 0, 0, -1, false},
	{false, false, false, false, NO_WEAPON, "", 0, 0, -1, false},
	{false, false, false, false, NO_WEAPON, "", 0, 0, -1, false},
	{false, true, true, true, WEAPON_COLT45, "9mm", 2, 160, 346, true},
	{false, true, true, true, WEAPON_SILENCED, "9mm silenciada", 2, 120, 347, true},
	{false, true, true, true, WEAPON_DEAGLE, "Desert Eagle", 2, 950, 348, true},
	{false, true, true, true, WEAPON_SHOTGUN, "Escopeta", 3, 800, 349, true},
	{false, true, true, true, WEAPON_SAWEDOFF, "Escopeta recortada", 3, 120, 350, true},
	{false, true, true, true, WEAPON_SHOTGSPA, "Escopeta de combate", 3, 120, 351, true},
	{false, true, true, true, WEAPON_UZI, "Micro UZI", 4, 70, 352, true},
	{false, true, true, true, WEAPON_MP5, "MP5", 4, 70, 353, true},
	{false, true, true, true, WEAPON_AK47, "Ak-47", 5, 80, 355, true},
	{false, true, true, true, WEAPON_M4, "M4", 5, 80, 356, true},
	{false, true, true, true, WEAPON_TEC9, "TEC-9", 4, 70, 372, true},
	{false, true, true, true, WEAPON_RIFLE, "Rifle", 6, 1000, 357, true},
	{false, true, true, true, WEAPON_SNIPER, "Sniper", 6, 1000, 358, true},
	{false, true, true, true, WEAPON_ROCKETLAUNCHER, "Lanzacohetes RPG", 7, 1000, 359, true},
	{false, true, true, true, WEAPON_HEATSEEKER, "Lanzacohetes HS", 7, 1000, 360, true},
	{false, true, true, true, WEAPON_FLAMETHROWER, "Lanzallamas", 7, 500, 361, true},
	{false, true, true, true, WEAPON_MINIGUN, "Minigun", 7, 0, 362, false},
	{false, false, true, true, WEAPON_SATCHEL, "Bomba detonadora", 8, 500, 363, false},
	{false, false, false, false, WEAPON_BOMB, "Detonador", 12, 500, 364, false},
	{false, false, true, true, WEAPON_SPRAYCAN, "Bote de spray", 9, 10, 365, true},
	{false, false, true, true, WEAPON_FIREEXTINGUISHER, "Extintor", 9, 10, 366, true},
	{false, false, true, false, WEAPON_CAMERA, "Cámara", 9, 500, 367, true},
	{false, false, true, false, NO_WEAPON, "Gafas de visión nocturna", 11, 0, 368, false},
	{false, false, true, false, NO_WEAPON, "Gafas térmicas", 11, 0, 369, false},
	{false, false, false, false, WEAPON_PARACHUTE, "Paracaídas", 11, 0, 371, true},
	{true, false, false, false, WEAPON_SHIELD, "Escudo", 13, 0, 18637, false},
	{true, true, true, true, WEAPON_TASER_GUN, "Pistola taser", 2, 120, 347, false}
};