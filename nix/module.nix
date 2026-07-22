{ self }:

{ config, lib, pkgs, ... }:

let
  cfg = config.services.samp-server;

  serverCfg = pkgs.writeText "server.cfg" ''
    echo Executing Server Config...

    # Server
    hostname ${cfg.hostname}
    language ${cfg.language}
    mapname ${cfg.mapname}
    weburl ${cfg.weburl}
    announce ${if cfg.announce then "1" else "0"}

    # Console
    rcon_password ${cfg.rcon.password}
    rcon ${if cfg.rcon.enable then "1" else "0"}
    output 1

    # Scripts
    filterscripts ${lib.concatStringsSep " " cfg.filterscripts}
    gamemode0 ${cfg.gamemode} ${toString cfg.gamemodeSlots}
    plugins ${lib.concatStringsSep " " cfg.plugins}

    # Browser
    maxplayers ${toString cfg.maxPlayers}
    maxnpc ${toString cfg.maxNpc}

    # Networking
    port ${toString cfg.port}
    conncookies ${if cfg.connCookies then "1" else "0"}
    sleep 1

    # Logging
    logtimeformat (${cfg.logTimeFormat})
    chatlogging ${if cfg.chatLogging then "1" else "0"}
    timestamp ${if cfg.timestamp then "1" else "0"}
    db_logging ${if cfg.dbLogging then "1" else "0"}
    db_log_queries ${if cfg.dbLogQueries then "1" else "0"}

    # Client
    onfoot_rate ${toString cfg.onfootRate}
    incar_rate ${toString cfg.incarRate}
    weapon_rate ${toString cfg.weaponRate}
    lagcompmode ${toString cfg.lagcompMode}
    stream_distance ${cfg.streamDistance}
    stream_rate ${toString cfg.streamRate}

    # Profiler
    long_call_time 0
    profiler_gamemode main
    profiler_outputformat html
    profiler_callgraph 0

    ${cfg.extraConfig}
  '';

  dbCfg = pkgs.writeText "srp_db.ini" ''
    hostname = ${cfg.database.hostname}
    username = ${cfg.database.username}
    database = ${cfg.database.name}
    password = ${cfg.database.password}
    auto_reconnect = ${if cfg.database.autoReconnect then "true" else "false"}
  '';

  # SQL to set user password on first startup
  dbInitScript = pkgs.writeText "samp-db-init.sql" ''
    ALTER USER '${cfg.database.username}'@'${cfg.database.hostname}' IDENTIFIED BY '${cfg.database.password}';
    FLUSH PRIVILEGES;
  '';
in
{
  options.services.samp-server = {
    enable = lib.mkEnableOption "SA-MP server";

    package = lib.mkOption {
      type = lib.types.package;
      default = self.packages.${pkgs.system}.default;
      defaultText = lib.literalExpression "self.packages.\${pkgs.system}.default";
      description = "SA-MP server package to use.";
    };

    hostname = lib.mkOption {
      type = lib.types.str;
      default = "Super Roleplay";
      description = "Server hostname displayed in the server browser.";
    };

    language = lib.mkOption {
      type = lib.types.str;
      default = "Español";
      description = "Server language.";
    };

    mapname = lib.mkOption {
      type = lib.types.str;
      default = "San Andreas";
      description = "Map name.";
    };

    weburl = lib.mkOption {
      type = lib.types.str;
      default = "";
      description = "Server website URL.";
    };

    announce = lib.mkOption {
      type = lib.types.bool;
      default = true;
      description = "Announce the server in the master server list.";
    };

    port = lib.mkOption {
      type = lib.types.port;
      default = 7777;
      description = "Game port.";
    };

    maxPlayers = lib.mkOption {
      type = lib.types.ints.positive;
      default = 100;
      description = "Maximum number of players.";
    };

    maxNpc = lib.mkOption {
      type = lib.types.ints.unsigned;
      default = 0;
      description = "Maximum number of NPCs.";
    };

    gamemode = lib.mkOption {
      type = lib.types.str;
      default = "srp";
      description = "Gamemode name (without .amx extension).";
    };

    gamemodeSlots = lib.mkOption {
      type = lib.types.ints.unsigned;
      default = 1;
      description = "Gamemode parameter passed to OnGameModeInit.";
    };

    filterscripts = lib.mkOption {
      type = lib.types.listOf lib.types.str;
      default = [ "maps" ];
      description = "List of filterscript names (without .amx extension).";
    };

    plugins = lib.mkOption {
      type = lib.types.listOf lib.types.str;
      default = [
        "crashdetect.so"
        "streamer.so"
        "sscanf.so"
        "mysql.so"
        "pawncmd.so"
        "pawnregex.so"
        "pawnraknet.so"
        "MapAndreas.so"
        "PawnPlus.so"
      ];
      description = "List of plugin files to load.";
    };

    rcon = {
      enable = lib.mkOption {
        type = lib.types.bool;
        default = false;
        description = "Enable RCON (remote console).";
      };

      password = lib.mkOption {
        type = lib.types.str;
        default = "";
        description = "RCON password. Leave empty to disable.";
      };
    };

    connCookies = lib.mkOption {
      type = lib.types.bool;
      default = false;
      description = "Enable connection cookies.";
    };

    logTimeFormat = lib.mkOption {
      type = lib.types.str;
      default = "%d/%m/%Y %H:%M:%S";
      description = "Log time format.";
    };

    chatLogging = lib.mkOption {
      type = lib.types.bool;
      default = true;
      description = "Enable chat logging.";
    };

    timestamp = lib.mkOption {
      type = lib.types.bool;
      default = true;
      description = "Show timestamps in chat.";
    };

    dbLogging = lib.mkOption {
      type = lib.types.bool;
      default = true;
      description = "Enable database logging.";
    };

    dbLogQueries = lib.mkOption {
      type = lib.types.bool;
      default = false;
      description = "Log individual database queries.";
    };

    onfootRate = lib.mkOption {
      type = lib.types.ints.positive;
      default = 40;
      description = "On-foot sync rate (ms).";
    };

    incarRate = lib.mkOption {
      type = lib.types.ints.positive;
      default = 40;
      description = "In-car sync rate (ms).";
    };

    weaponRate = lib.mkOption {
      type = lib.types.ints.positive;
      default = 40;
      description = "Weapon sync rate (ms).";
    };

    lagcompMode = lib.mkOption {
      type = lib.types.int;
      default = 1;
      description = "Lag compensation mode (0=off, 1=on, 2=full).";
    };

    streamDistance = lib.mkOption {
      type = lib.types.str;
      default = "200.0";
      description = "Streamer distance.";
    };

    streamRate = lib.mkOption {
      type = lib.types.ints.positive;
      default = 1000;
      description = "Streamer update rate (ms).";
    };

    openFirewall = lib.mkOption {
      type = lib.types.bool;
      default = true;
      description = "Open the game port in the NixOS firewall.";
    };

    extraConfig = lib.mkOption {
      type = lib.types.lines;
      default = "";
      description = "Extra lines appended to server.cfg.";
    };

    dataDir = lib.mkOption {
      type = lib.types.path;
      default = "/var/lib/samp-server";
      description = "Persistent data directory for scriptfiles and logs.";
    };

    database = {
      enable = lib.mkEnableOption "MariaDB database server for SA-MP";

      hostname = lib.mkOption {
        type = lib.types.str;
        default = "localhost";
        description = "MariaDB/MySQL server hostname.";
      };

      port = lib.mkOption {
        type = lib.types.port;
        default = 3306;
        description = "MariaDB/MySQL server port.";
      };

      username = lib.mkOption {
        type = lib.types.str;
        default = "root";
        description = "Database username.";
      };

      name = lib.mkOption {
        type = lib.types.str;
        default = "srp_db";
        description = "Database name.";
      };

      password = lib.mkOption {
        type = lib.types.str;
        default = "";
        description = "Database password.";
      };

      autoReconnect = lib.mkOption {
        type = lib.types.bool;
        default = false;
        description = "Automatically reconnect to the database.";
      };
    };
  };

  config = lib.mkIf cfg.enable {
    services.mysql = lib.mkIf cfg.database.enable {
      enable = true;
      package = pkgs.mariadb;
      initialDatabases = [
        {
          name = cfg.database.name;
          schema = ./../db/srp_db.sql;
        }
      ];
      initialScript = dbInitScript;
      ensureUsers = [
        {
          name = cfg.database.username;
          ensurePermissions = {
            "${cfg.database.name}.*" = "ALL PRIVILEGES";
          };
        }
      ];
      settings.mysqld = {
        bind-address = cfg.database.hostname;
        port = cfg.database.port;
      };
    };

    systemd.services.samp-server = {
      description = "SA-MP Super Roleplay Server";
      after = [ "network.target" ] ++ lib.optional cfg.database.enable "mysql.service";
      wantedBy = [ "multi-user.target" ];
      requires = lib.optional cfg.database.enable "mysql.service";

      preStart = ''
        # Ensure data directory exists
        mkdir -p ${cfg.dataDir}/scriptfiles
        mkdir -p ${cfg.dataDir}/logs

        # Copy scriptfiles if empty
        if [ -z "$(ls -A ${cfg.dataDir}/scriptfiles 2>/dev/null)" ]; then
          cp -r ${cfg.package}/scriptfiles/* ${cfg.dataDir}/scriptfiles/ || true
        fi

        # Symlink server config and database config
        ln -sf ${serverCfg} ${cfg.dataDir}/server.cfg
        ln -sf ${dbCfg} ${cfg.dataDir}/srp_db.ini

        # Symlink gamemodes, filterscripts, plugins
        ln -sfn ${cfg.package}/gamemodes ${cfg.dataDir}/gamemodes
        ln -sfn ${cfg.package}/filterscripts ${cfg.dataDir}/filterscripts
        ln -sfn ${cfg.package}/plugins ${cfg.dataDir}/plugins

        # Symlink server binary
        ln -sfn ${cfg.package}/omp-server ${cfg.dataDir}/omp-server
      '';

      serviceConfig = {
        ExecStart = "${cfg.package}/bin/samp-server";
        WorkingDirectory = cfg.dataDir;
        Restart = "on-failure";
        RestartSec = 5;
        StateDirectory = "samp-server";
        ProtectHome = true;
        NoNewPrivileges = true;
      };
    };

    networking.firewall = lib.mkIf cfg.openFirewall {
      allowedTCPPorts = [ cfg.port ];
      allowedUDPPorts = [ cfg.port ];
    };
  };
}
