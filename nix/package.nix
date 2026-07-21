{ lib
, stdenv
, bash
, patchelf
, pawncc
, pkgs-i686
, sourceRoot ? ./..
}:

let
  glibc32 = pkgs-i686.glibc;
  libstdcxx32 = pkgs-i686.stdenv.cc.cc.lib;
  zlib32 = pkgs-i686.zlib;
  libPath32 = "${libstdcxx32}/lib:${zlib32}/lib:${glibc32}/lib";
in
stdenv.mkDerivation {
  pname = "samp-super-roleplay";
  version = "2.4.8";

  src = sourceRoot;

  nativeBuildInputs = [
    patchelf
    pawncc
  ];

  # Only Linux 32-bit binaries and data — no .dll/.exe
  dontConfigure = true;
  dontFixup = true;

  buildPhase = ''
    runHook preBuild

    export LD_LIBRARY_PATH="${libPath32}"

    # Compile gamemode (run from gamemodes/ so -D and -i resolve correctly)
    cd gamemodes
    pawncc -Dgamemodes -i../pawno/include -d3 -Z "-(+" "-;+" srp.pwn
    cd ..

    # Compile filterscript
    cd filterscripts
    pawncc -Dfilterscripts -i../pawno/include -Z "-(+" "-;+" maps.pwn
    cd ..

    # Patch the server binary (use the unpatched .temp if present, else samp03svr)
    if [ -f samp03svr.temp ]; then
      cp samp03svr.temp samp03svr_bin
    else
      cp samp03svr samp03svr_bin
    fi

    patchelf \
      --set-interpreter "${glibc32}/lib/ld-linux.so.2" \
      --set-rpath "${libstdcxx32}/lib:${zlib32}/lib:${glibc32}/lib" \
      samp03svr_bin

    # Patch announce if present
    if [ -f announce ]; then
      patchelf \
        --set-interpreter "${glibc32}/lib/ld-linux.so.2" \
        --set-rpath "${libstdcxx32}/lib:${zlib32}/lib:${glibc32}/lib" \
        announce
    fi

    # Patch all .so plugins (shared libraries only need rpath, not interpreter)
    for so in plugins/*.so log-core.so; do
      if [ -f "$so" ]; then
        patchelf \
          --set-rpath "${libstdcxx32}/lib:${zlib32}/lib:${glibc32}/lib:$out" \
          "$so"
      fi
    done

    runHook postBuild
  '';

  installPhase = ''
    runHook preInstall

    mkdir -p $out

    # Server binary
    install -Dm755 samp03svr_bin $out/samp03svr

    # Announce utility
    if [ -f announce ]; then
      install -Dm755 announce $out/announce
    fi

    # Server config
    install -Dm644 server.cfg $out/server.cfg

    # Log-core shared library (needed by mysql.so)
    install -Dm755 log-core.so $out/log-core.so

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

    # Pawn includes (useful for recompilation)
    cp -r pawno/include $out/include

    # Wrapper script
    mkdir -p $out/bin
    cat > $out/bin/samp-server <<WRAPPER
    #!${bash}/bin/bash
    export LD_LIBRARY_PATH="${libstdcxx32}/lib:${zlib32}/lib:${glibc32}/lib:$out:\$LD_LIBRARY_PATH"
    exec ./samp03svr
    WRAPPER
    chmod +x $out/bin/samp-server

    runHook postInstall
  '';

  meta = with lib; {
    description = "SA-MP Super Roleplay server";
    license = licenses.unfree;
    platforms = [ "x86_64-linux" "i686-linux" ];
    mainProgram = "samp-server";
  };
}
