{ lib
, stdenv
, bash
, pawncc
, open-mp-server
, sourceRoot ? ./..
}:

stdenv.mkDerivation {
  pname = "samp-super-roleplay";
  version = "2.4.8";

  src = sourceRoot;

  nativeBuildInputs = [
    pawncc
  ];

  dontConfigure = true;
  dontFixup = true;

  buildPhase = ''
    runHook preBuild

    cd gamemodes
    pawncc -Dgamemodes -i../pawno/include -d3 -Z "-(+" "-;+" srp.pwn
    cd ..

    cd filterscripts
    pawncc -Dfilterscripts -i../pawno/include -Z "-(+" "-;+" maps.pwn
    cd ..

    runHook postBuild
  '';

  installPhase = ''
    runHook preInstall

    mkdir -p $out

    # Server binary from open.mp
    install -Dm755 ${open-mp-server}/bin/omp-server $out/omp-server

    # Server config
    install -Dm644 server.cfg $out/server.cfg

    # Compiled gamemode
    install -Dm644 gamemodes/srp.amx $out/gamemodes/srp.amx

    # Filterscripts
    mkdir -p $out/filterscripts
    for amx in filterscripts/*.amx; do
      [ -f "$amx" ] && install -Dm644 "$amx" "$out/$amx"
    done

    # Plugins (.so only)
    mkdir -p $out/plugins
    for so in plugins/*.so; do
      [ -f "$so" ] && install -Dm755 "$so" "$out/$so"
    done
    for cfg in plugins/*.cfg; do
      [ -f "$cfg" ] && install -Dm644 "$cfg" "$out/$cfg"
    done

    # Scriptfiles
    cp -r scriptfiles $out/scriptfiles

    # Pawn includes
    cp -r pawno/include $out/include

    # Wrapper script
    mkdir -p $out/bin
    cat > $out/bin/samp-server <<WRAPPER
    #!${bash}/bin/bash
    exec ./omp-server
    WRAPPER
    chmod +x $out/bin/samp-server

    runHook postInstall
  '';

  meta = with lib; {
    description = "SA-MP Super Roleplay server (open.mp based)";
    license = licenses.unfree;
    platforms = [ "x86_64-linux" ];
    mainProgram = "samp-server";
  };
}
