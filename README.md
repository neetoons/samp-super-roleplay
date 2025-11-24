<h1 align="center">
Super Roleplay 2
</h1>

<p align="center">
    <img width="300" src="https://github.com/neetoons/samp-super-roleplay/blob/main/docs/post/WZMe055.png" alt="super-roleplay-logo">
</p>

Una gamemode roleplay de SA:MP originalmente escrita y liberada por [Andri1](https://sampforum.blast.hk/member.php?action=profile&uid=106967), ahora mantenida por Neetoons y colaboradores.

## Carácteristicas
- [Comandos Administrativos](https://github.com/neetoons/samp-super-roleplay/blob/main/docs/admin_commands.md)
- [Lista de Anticheats](https://github.com/neetoons/samp-super-roleplay/blob/main/docs/anticheats.md)
- [bandas](https://github.com/neetoons/samp-super-roleplay/blob/main/docs/bandas.md)

## Especificaciones y Uso

- [Sistema de objetos en vehículos](https://sampforum.blast.hk/showthread.php?tid=639041)
- [Bandas personalizables con más de 100 territorios para conquistar](https://github.com/neetoons/samp-super-roleplay/blob/main/docs/bandas.md)
- Para obtener admin usar /modget logeado como RCON.

## Sistemas

| Sistemas    | Usuario Normal | Super Usuario |
| :---------- | :------------: | :-----------: |
| Trabajos    |       1        |       3       |
| Vehículos   |       2        |       6       |
| Accesorios  |       1        |      10       |
| Propiedades |       1        |       4       |
| Objetos     |       1        |      10       |

## Anticheat

En total hay 24 cheats parcheados. Importante la acción "Aviso" no es aviso al cheater, es aviso para los admins y así podrán espiarlo.
Para configurar el anticheat IG necesitamos ser admin nivel 5 (dueño), el comando es /ac.

<p align="center">
    <img width="300" src="https://github.com/neetoons/samp-super-roleplay/blob/main/docs/post/n5AGwAZm.png" alt="anticheat">
</p>

La columna accionar indica el numero de detecciones en los segundos indicados para que el anticheat realice la acción de avisar o kickear.
Por ejemplo el cheat "pos" será accionado cuando haya 3 detecciones en menos de 10 segundos.

## Ajustar economía

Puedes utilizar el comando /eco (nivel admin 5) para ajustar los multiplicadores de la economía del servidor IG.
Por ejemplo si el precio base de un vehículo es de 1.000$ si ponemos el multiplicador de vehículos en 2, el precio del vehículo sera de 2.000$.

<p align="center">
    <img width="300" src="https://github.com/neetoons/samp-super-roleplay/blob/main/docs/post/AArCW7Hm.png" alt="economia">
</p>

---

## Desarrollo

### Requisitos

- San Andreas Multiplayer (SA:MP)
- Pawn Compiler 3.10.10
- MySQL

### Plugins

| plugins-includes                                                   |  version  |
| :----------------------------------------------------------------- | :-------: |
| [MySQL](https://github.com/pBlueG/SA-MP-MySQL)                     |   R41-4   |
| [YSI-Includes](https://github.com/pawn-lang/YSI-Includes)          | 5.10.0006 |
| [Pawn.CMD](https://github.com/katursis/Pawn.CMD)                   |   3.4.0   |
| [Pawn.Regex](https://github.com/katursis/Pawn.Regex)               |   1.2.3   |
| [Pawn.Raknet](https://github.com/katursis/Pawn.RakNet)             |   1.6.0   |
| [Pawn.Plus](https://github.com/IS4Code/PawnPlus)                   |   1.5.1   |
| [sscanf](https://github.com/Y-Less/sscanf)                         |  2.13.8   |
| [streamer](https://github.com/samp-incognito/samp-streamer-plugin) |   2.9.6   |
| [MapAndreas](https://github.com/philip1337/samp-plugin-mapandreas) |     -     |

### Instalación

1. Descarga [La última versión](https://github.com/neetoons/samp-super-roleplay/releases/latest)
2. Compila el [maps.pwn](https://github.com/neetoons/samp-super-roleplay/blob/main/gamemodes/mapas.pwn) en filterscript.
3. Crear una base de datos en MySQL, con el nombre por ejemplo "srp".
4. Subir [srp_db.sql](https://github.com/neetoons/samp-super-roleplay/blob/main/db/srp_db.sql) a la base de datos MySQL en srp.
5. Especifica los datos de la conexión [srp_db.ini](https://github.com/neetoons/samp-super-roleplay/blob/main/srp_db.ini)

```ini
hostname = localhost
username = root
database = srp
password = 123
auto_reconnect = false
```

Si estas usando XAMPP no hace falta especificar el password.

## Esquema de la base de datos

<p align="center">
    <img width="300" src="https://github.com/neetoons/samp-super-roleplay/blob/main/docs/db_schema.png" alt="super-roleplay-db_schema">
</p>

---
### Nota
Adri1: por si alguien quiere editarlo o algo mis ideas para próximas versiones eran estas:
> - Negocios.
> - Garajes para las casas.
> - Robos a bancos y tiendas con las bandas.
