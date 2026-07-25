{ config, lib, pkgs, ... }:

let
  cfg = config.services.samp-server;

  configJson = pkgs.writeText "config.json" (builtins.toJSON {
    name = cfg.hostname;
    language = cfg.language;
    announce = cfg.announce;
    website = cfg.weburl;

    network = {
      port = cfg.port;
      stream_radius = cfg.streamDistance;
      stream_rate = cfg.streamRate;
      cookie_reseed_time = if cfg.connCookies then 300000 else 0;
    };

    max_players = cfg.maxPlayers;
    max_bots = cfg.maxNpc;

    game = {
      lag_compensation_mode = cfg.lagcompMode;
      mode = cfg.gamemode;
      map = cfg.mapname;
    };

    artwork = {
      enable = false;
    };

    pawn = {
      legacy_plugins = cfg.legacyPlugins;
      main_scripts = [ "${cfg.gamemode} ${toString cfg.gamemodeSlots}" ];
      side_scripts = map (fs: "filterscripts/${fs}") cfg.filterscripts;
    };

    rcon = {
      enable = cfg.rcon.enable;
      password = cfg.rcon.password;
    };

    logging = {
      enable = true;
      log_chat = cfg.chatLogging;
      log_connection_messages = true;
      log_deaths = true;
      log_queries = cfg.dbLogQueries;
      timestamp_format = "[${cfg.logTimeFormat}]";
    };

    sleep = cfg.sleep;
  });

  dbCfg = pkgs.writeText "srp_db.ini" ''
    hostname = ${cfg.database.hostname}
    username = ${cfg.database.username}
    database = ${cfg.database.name}
    password = ${cfg.database.password}
    auto_reconnect = ${if cfg.database.autoReconnect then "true" else "false"}
  '';

  dbInitScript = pkgs.writeText "samp-db-init.sql" ''
    CREATE USER IF NOT EXISTS '${cfg.database.username}'@'localhost' IDENTIFIED BY '${cfg.database.password}';
    CREATE USER IF NOT EXISTS '${cfg.database.username}'@'%' IDENTIFIED BY '${cfg.database.password}';
    GRANT ALL PRIVILEGES ON ${cfg.database.name}.* TO '${cfg.database.username}'@'localhost';
    GRANT ALL PRIVILEGES ON ${cfg.database.name}.* TO '${cfg.database.username}'@'%';
    FLUSH PRIVILEGES;
  '';
in
{
  options.services.samp-server = {
    enable = lib.mkEnableOption "open.mp server";

    package = lib.mkOption {
      type = lib.types.package;
      description = "open.mp server package to use.";
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

      legacyPlugins = lib.mkOption {
        type = lib.types.listOf lib.types.str;
        default = [
          "sscanf"
          "mysql"
        ];
        description = "Legacy SA-MP plugins to load from the plugins/ directory.";
      };

    rcon = {
      enable = lib.mkOption {
        type = lib.types.bool;
        default = false;
        description = "Enable RCON (remote console).";
      };

      password = lib.mkOption {
        type = lib.types.str;
        default = "changeme";
        description = "RCON password.";
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
      description = "Log time format (strftime format).";
    };

    chatLogging = lib.mkOption {
      type = lib.types.bool;
      default = true;
      description = "Enable chat logging.";
    };

    dbLogQueries = lib.mkOption {
      type = lib.types.bool;
      default = false;
      description = "Log individual database queries.";
    };

    lagcompMode = lib.mkOption {
      type = lib.types.int;
      default = 1;
      description = "Lag compensation mode (0=off, 1=on, 2=position only).";
    };

    streamDistance = lib.mkOption {
      type = lib.types.float;
      default = 200.0;
      description = "Streamer distance (50.0-400.0).";
    };

    streamRate = lib.mkOption {
      type = lib.types.ints.positive;
      default = 1000;
      description = "Streamer update rate in ms (500-5000).";
    };

    sleep = lib.mkOption {
      type = lib.types.float;
      default = 5.0;
      description = "Main thread sleep time in ms. Lower = better sync, higher CPU.";
    };

    openFirewall = lib.mkOption {
      type = lib.types.bool;
      default = true;
      description = "Open the game port in the NixOS firewall.";
    };

    extraConfig = lib.mkOption {
      type = lib.types.attrs;
      default = {};
      description = "Extra attributes merged into config.json.";
    };

    dataDir = lib.mkOption {
      type = lib.types.path;
      default = "/var/lib/samp-server";
      description = "Persistent data directory for scriptfiles and logs.";
    };

    database = {
      enable = lib.mkEnableOption "MariaDB database server";

      hostname = lib.mkOption {
        type = lib.types.str;
        default = "127.0.0.1";
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
      settings.mysqld = {
        bind-address = "127.0.0.1";
        port = cfg.database.port;
      };
    };

    systemd.services.samp-server = {
      description = "open.mp Super Roleplay Server";
      after = [ "network.target" ] ++ lib.optional cfg.database.enable "mysql.service";
      wantedBy = [ "multi-user.target" ];
      requires = lib.optional cfg.database.enable "mysql.service";

      preStart = ''
        mkdir -p ${cfg.dataDir}/scriptfiles
        mkdir -p ${cfg.dataDir}/scriptfiles/YSI/fixes
        mkdir -p ${cfg.dataDir}/scriptfiles/YSI/temp
        mkdir -p ${cfg.dataDir}/logs

        if [ -z "$(ls -A ${cfg.dataDir}/scriptfiles 2>/dev/null)" ]; then
          cp -r ${cfg.package}/share/samp/scriptfiles/* ${cfg.dataDir}/scriptfiles/ || true
        fi

        ln -sf ${configJson} ${cfg.dataDir}/config.json
        ln -sf ${dbCfg} ${cfg.dataDir}/srp_db.ini

        ln -sfn ${cfg.package}/share/samp/gamemodes ${cfg.dataDir}/gamemodes
        ln -sfn ${cfg.package}/share/samp/filterscripts ${cfg.dataDir}/filterscripts
        ln -sfn ${cfg.package}/share/samp/components ${cfg.dataDir}/components
        ln -sfn ${cfg.package}/share/samp/plugins ${cfg.dataDir}/plugins
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
