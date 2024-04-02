# Super Roleplay 1.15 

<p align="center">
    <img width="200" src="https://github.com/neetoons/samp-super-roleplay/blob/main/docs/post/WZMe055.png" alt="super-roleplay-logo">
</p>

Una gamemode roleplay de SA:MP originalmente escrita y liberada por [Andri1](https://sampforum.blast.hk/member.php?action=profile&uid=106967), ahora mantenida por Neetoons y colaboradores. 

- [Comandos Administrativos](https://github.com/neetoons/samp-super-roleplay/blob/main/docs/admin_commands.md)
- [Lista de Anticheats](https://github.com/neetoons/samp-super-roleplay/blob/main/docs/anticheats.md)
- [bandas](https://github.com/neetoons/samp-super-roleplay/blob/main/docs/bandas.md)

## Anticheat 

En total hay 24 cheats parcheados. Importante la acción "Aviso" no es aviso al cheater, es aviso para los admins y así podrán espiarlo.
Para configurar el anticheat IG necesitamos ser admin nivel 5 (dueño), el comando es /ac.

<p align="center">
    <img width="200" src="https://github.com/neetoons/samp-super-roleplay/blob/main/docs/post/n5AGwAZm.png" alt="anticheat">
</p>

La columna accionar indica el numero de detecciones en los segundos indicados para que el anticheat realice la acción de avisar o kickear.
Por ejemplo el cheat "pos" será accionado cuando haya 3 detecciones en menos de 10 segundos.

## Ajustar economía

Puedes utilizar el comando /eco (nivel admin 5) para ajustar los multiplicadores de la economía del servidor IG.
Por ejemplo si el precio base de un vehículo es de 1.000$ si ponemos el multiplicador de vehículos en 2, el precio del vehículo sera de 2.000$.

<p align="center">
    <img width="200" src="https://github.com/neetoons/samp-super-roleplay/blob/main/docs/post/AArCW7Hm.png" alt="economia">
</p>

## Otros

- Es necesario usar el [compilador](https://github.com/pawn-lang/compiler) 3.10.10 para que compile el GM.
- Sistema de datos: SQLite.
- Para obtener admin usar /modget logeado como RCON.
- Para que el buscador de YouTube (/mp3) funcione subir [search-yb.php](https://github.com/neetoons/samp-super-roleplay/blob/main/api/search-yb.php) 
- [Sistema de objetos en vehículos](https://sampforum.blast.hk/showthread.php?tid=639041)
- [Bandas personalizables con más de 100 territorios para conquistar](https://github.com/neetoons/samp-super-roleplay/blob/main/docs/bandas.md)
- [Changelog completo](https://github.com/neetoons/samp-super-roleplay/blob/main/CHANGELOG.md) (66 días de desarrollo)
No doy soporte ni ayudo por privado (Andri1).
- Versión GM: 1.15.04.

| Sistemas    | Usuario Normal | Super Usuario |
|    :----    |     :----:     |     :----:    | 
| Trabajos    | 1              | 3             |
| Vehículos   | 2              | 6             |
| Accesorios  | 1              | 10            |
| Propiedades | 1              | 4             |
| Objetos     | 1              | 10            |

Por si alguien quiere editarlo o algo mis ideas para próximas versiones eran estas:
- Negocios.
- Garajes para las casas.
- Robos a bancos y tiendas con las bandas.

## Instalación
### Base de datos
Al tener una base de datos SQLite no necesitarás subirla a un servidor MySQL, tendrás que hacer que se cree en la gm en la carpeta scriptfiles ``Super_RolePlay.db`` de la siguiente forma:

1. Incluir el código de [CreateDatabase.pwn](https://github.com/neetoons/samp-super-roleplay/blob/main/gamemodes/CreateDatabase.pwn) a la gamemode [srp.pwn](https://github.com/neetoons/samp-super-roleplay/blob/main/gamemodes/srp.pwn) a lo ultimo del código (también lo puedes hacer con ``#include`` mucho más rápido).

2. En OnGameModeInit colocar la linea ``CreateDatabase();`` debajo de ``ConnectDatabase();``

```c
public OnGameModeInit()
{
	ConnectDatabase();
	CreateDatabase();
	return 1;
}
```

3. Compilar y correr el servidor.

Una vez encendido el servidor deberás cerrarlo y quitar todo este procedemiento. En las siguientes versiones se optimizará esto.

## Mapeos
Debes compilar el [maps.pwn](https://github.com/neetoons/samp-super-roleplay/blob/main/gamemodes/CreateDatabase.pwn) en filterscript.


## Desarrollo
1. Clona el respositorio
```bash
git clone https://github.com/neetoons/samp-super-roleplay.git
```
2. Descarga el [release](https://github.com/neetoons/samp-super-roleplay/releases/latest) respectivo a esta versión y copia los archivos a la carpeta donde clonaste anteriormente el repositorio (no reemplazar ningún archivo).
