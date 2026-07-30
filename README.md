<h1 align="center">
Super Roleplay 3
</h1>

<p align="center">
    <img width="300" src="https://github.com/neetoons/samp-super-roleplay/blob/main/docs/post/WZMe055.png" alt="super-roleplay-logo">
</p>

An OPEN:MP roleplay gamemode originally written and released by [Andri1](https://sampforum.blast.hk/member.php?action=profile&uid=106967), now maintained by Neetoons and contributors.

## Features
- [Admin Commands](https://github.com/neetoons/samp-super-roleplay/blob/main/docs/admin_commands.md)
- [Anticheat List](https://github.com/neetoons/samp-super-roleplay/blob/main/docs/anticheats.md)
- [Gangs](https://github.com/neetoons/samp-super-roleplay/blob/main/docs/bandas.md)

## Specifications & Usage

- [Vehicle item system](https://sampforum.blast.hk/showthread.php?tid=639041)
- [Customizable gangs with 100+ conquerable territories](https://github.com/neetoons/samp-super-roleplay/blob/main/docs/bandas.md)
- To get admin use /modget while logged in as RCON.

## Systems

| Systems     | Normal User | Super User |
| :---------- | :---------: | :--------: |
| Jobs        |      1      |     3      |
| Vehicles    |      2      |     6      |
| Accessories |      1      |     10     |
| Properties  |      1      |     4      |
| Objects     |      1      |     10     |

## Anticheat

24 cheats patched in total. Important: the "Warning" action notifies admins (not the cheater), so they can spectate.
To configure anticheat in-game, be admin level 5 (owner), command is /ac.

<p align="center">
    <img width="300" src="https://github.com/neetoons/samp-super-roleplay/blob/main/docs/post/n5AGwAZm.png" alt="anticheat">
</p>

The "Trigger" column shows how many detections within the given seconds trigger a warning or kick.
For example, the "pos" cheat triggers on 3 detections within 10 seconds.

## Economy Adjustment

Use the /eco command (admin level 5) to adjust in-game economy multipliers.
For example, if the base vehicle price is $1,000 and the vehicle multiplier is set to 2, the price becomes $2,000.

<p align="center">
    <img width="300" src="https://github.com/neetoons/samp-super-roleplay/blob/main/docs/post/AArCW7Hm.png" alt="economy">
</p>

## Flake Usage (Nix)

This repository is a Nix flake. To use it:

### Build the full server package

```bash
nix build
# Result is a fully-configured open.mp server in ./result
```

### Run the server

```bash
nix run
```

### Enter the development shell

```bash
nix develop
```

Inside the dev shell you'll have `pawncc`, YSI includes, and the formatting tools available:

```bash
cd gamemodes
pawncc -Dgamemodes -i../pawno/include -i$YSI_INCLUDE -d3 -Z "-(+" "-;+" srp.pwn
```

To compile the filterscript:

```bash
cd filterscripts
pawncc -Dfilterscripts -i../pawno/include -i$YSI_INCLUDE -Z "-(+" "-;+" maps.pwn
```

### NixOS module

Import the module and enable the server:

```nix
{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    samp-super-roleplay.url = "github:neetoons/samp-super-roleplay";
  };

  outputs = { self, nixpkgs, samp-super-roleplay }: {
    nixosConfigurations.my-server = nixpkgs.lib.nixosSystem {
      modules = [
        samp-super-roleplay.nixosModules.default
        {
          services.samp-server = {
            enable = true;
            hostname = "My RP Server";
            port = 7777;
            database = {
              enable = true;
              password = "my-db-password";
            };
          };
        }
      ];
    };
  };
}
```

Key options:

| Option                          | Default              | Description                          |
| :------------------------------ | :------------------- | :----------------------------------- |
| `services.samp-server.enable`   | `false`              | Enable the server service            |
| `services.samp-server.hostname` | `"Super Roleplay"`   | Server name shown in browser         |
| `services.samp-server.port`     | `7777`               | Game port                            |
| `services.samp-server.database.enable` | `false`        | Spin up a local MariaDB instance     |
| `services.samp-server.database.password` | `""`        | Database password                    |

---

## Development

### Requirements

- San Andreas Multiplayer (SA:MP) / OPEN.MP
- Pawn Compiler 3.10.11
- MySQL

### Plugins

| Plugin/Include                                                        | Version  |
| :-------------------------------------------------------------------- | :------: |
| [MySQL](https://github.com/pBlueG/SA-MP-MySQL)                        |  R41-4   |
| [YSI-Includes](https://github.com/pawn-lang/YSI-Includes)             | 5.10.0006 |
| [Pawn.CMD](https://github.com/katursis/Pawn.CMD)                      |  3.4.0   |
| [Pawn.Regex](https://github.com/katursis/Pawn.Regex)                  |  1.2.3   |
| [Pawn.Raknet](https://github.com/katursis/Pawn.RakNet)                |  1.6.0   |
| [Pawn.Plus](https://github.com/IS4Code/PawnPlus)                      |  1.5.1   |
| [sscanf](https://github.com/Y-Less/sscanf)                            |  2.15.1  |
| [streamer](https://github.com/samp-incognito/samp-streamer-plugin)    |  2.9.6   |
| [MapAndreas](https://github.com/philip1337/samp-plugin-mapandreas)    |    -     |

### Installation

1. Download the [latest release](https://github.com/neetoons/samp-super-roleplay/releases/latest)
2. Compile [maps.pwn](https://github.com/neetoons/samp-super-roleplay/blob/main/gamemodes/mapas.pwn) as a filterscript.
3. Create a MySQL database, for example "srp".
4. Import [srp_db.sql](https://github.com/neetoons/samp-super-roleplay/blob/main/db/srp_db.sql) into the MySQL database.
5. Configure the connection details in [srp_db.ini](https://github.com/neetoons/samp-super-roleplay/blob/main/srp_db.ini)

```ini
hostname = localhost
username = root
database = srp
password = 123
auto_reconnect = false
```

If using XAMPP no password is needed.

## Database Schema

<p align="center">
    <img width="300" src="https://github.com/neetoons/samp-super-roleplay/blob/main/docs/db_schema.png" alt="super-roleplay-db_schema">
</p>

---
### Note
Adri1: in case someone wants to edit it, here were my ideas for future versions:
> - Businesses.
> - House garages.
> - Bank and store robberies with gangs.
