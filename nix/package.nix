{ lib
, stdenv
, makeWrapper
, writeText
, pawncc
, open-mp-server
, plugins
, components ? {}
, ysi
, src
}:

let
  share = "share/samp";

  defaultConfig = builtins.toJSON {
    name = "Super Roleplay";
    language = "Español";
    announce = true;
    website = "";

    network = {
      port = 7777;
      stream_radius = 200.0;
      stream_rate = 1000;
      cookie_reseed_time = 300000;
    };

    max_players = 100;
    max_bots = 0;

    game = {
      lag_compensation_mode = 1;
      mode = "srp";
      map = "San Andreas";
    };

    artwork = {
      enable = false;
    };

    pawn = {
      legacy_plugins = [
        "sscanf"
        "mysql"
      ];
      main_scripts = [ "srp 1" ];
      side_scripts = [];
    };

    rcon = {
      enable = false;
      password = "changeme";
    };

    logging = {
      enable = true;
      log_chat = true;
      log_connection_messages = true;
      log_deaths = true;
      log_queries = false;
      timestamp_format = "[%d/%m/%Y %H:%M:%S]";
    };

    sleep = 5.0;
  };

  configFile = writeText "config.json" defaultConfig;
in
stdenv.mkDerivation {
  pname = "samp-super-roleplay";
  version = "2.4.8";

  inherit src;

  nativeBuildInputs = [
    pawncc
    makeWrapper
  ];

  dontConfigure = true;
  dontFixup = true;

  buildPhase = ''
    runHook preBuild

    mkdir -p gamemodes filterscripts

    if [ -f gamemodes/srp.pwn ]; then
      cd gamemodes
      pawncc -Dgamemodes -i../pawno/include -i${ysi}/include -d3 -Z "-(+" "-;+" srp.pwn
      cd ..
    fi

    if [ -f filterscripts/maps.pwn ]; then
      cd filterscripts
      pawncc -Dfilterscripts -i../pawno/include -i${ysi}/include -Z "-(+" "-;+" maps.pwn
      cd ..
    fi

    runHook postBuild
  '';

  installPhase = ''
    runHook preInstall

    mkdir -p $out/${share}
    BASE=$out/${share}

    install -Dm755 ${open-mp-server}/bin/omp-server $BASE/omp-server

    mkdir -p $BASE/components
    for so in ${open-mp-server}/lib/open-mp/components/*.so; do
      [ -f "$so" ] && install -Dm755 "$so" "$BASE/components/$(basename "$so")"
    done

    mkdir -p $BASE/plugins
    ${lib.concatStringsSep "\n" (map (name: ''
      if [ -d "${plugins.${name}}/lib" ]; then
        for so in ${plugins.${name}}/lib/*.so; do
          [ -f "$so" ] && install -Dm755 "$so" "$BASE/plugins/$(basename "$so")"
        done
      fi
    '') (builtins.attrNames plugins))}

    ${lib.concatStringsSep "\n" (map (name: ''
      if [ -d "${components.${name}}/lib" ]; then
        for so in ${components.${name}}/lib/*.so; do
          [ -f "$so" ] && install -Dm755 "$so" "$BASE/components/$(basename "$so")"
        done
      fi
    '') (builtins.attrNames components))}

    mkdir -p $BASE/gamemodes $BASE/filterscripts
    [ -f gamemodes/srp.amx ] && cp gamemodes/srp.amx $BASE/gamemodes/

    if compgen -G "filterscripts/*.amx" > /dev/null; then
      cp filterscripts/*.amx $BASE/filterscripts/
    fi

    [ -d scriptfiles ] && cp -r scriptfiles $BASE/

    cp ${configFile} $BASE/config.json

    touch $BASE/components/pawncmd.cfg
    touch $BASE/components/pawnraknet.cfg

    mkdir -p $BASE/pawno/include
    cp -rn pawno/include/* $BASE/pawno/include/ 2>/dev/null || true

    if [ -d "${ysi}/include" ]; then
      for dir in ${ysi}/include/YSI_*; do
        [ -d "$dir" ] && cp -r "$dir" "$BASE/pawno/include/"
      done
    fi

    ${lib.concatStringsSep "\n" (map (name: ''
      if [ -d "${plugins.${name}}/include" ]; then
        for inc in ${plugins.${name}}/include/*.inc; do
          [ -f "$inc" ] && [ ! -f "$BASE/pawno/include/$(basename "$inc")" ] && cp "$inc" "$BASE/pawno/include/"
        done
      fi
    '') (builtins.attrNames plugins))}

    ${lib.concatStringsSep "\n" (map (name: ''
      if [ -d "${components.${name}}/include" ]; then
        for inc in ${components.${name}}/include/*.inc; do
          [ -f "$inc" ] && [ ! -f "$BASE/pawno/include/$(basename "$inc")" ] && cp "$inc" "$BASE/pawno/include/"
        done
      fi
    '') (builtins.attrNames components))}

    makeWrapper $BASE/omp-server $out/bin/samp-server

    runHook postInstall
  '';

  meta = with lib; {
    description = "SA-MP Super Roleplay server (open.mp based)";
    license = licenses.unfree;
    platforms = [ "x86_64-linux" ];
    mainProgram = "samp-server";
  };
}
