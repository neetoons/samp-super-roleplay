{ lib, stdenv, src }:

stdenv.mkDerivation {
  pname = "pawnplus";
  version = "1.5.1";
  inherit src;

  preBuild = ''
    cd plugins
    sed -i 's/-m32//g' makefile
  '';

  buildFlags = [ "CC=${stdenv.cc}/bin/cc" "CXX=${stdenv.cc}/bin/c++" ];

  installPhase = ''
    runHook preInstall
    mkdir -p $out/lib $out/include
    cp PawnPlus.so $out/lib/ 2>/dev/null || true
    find . -name "PawnPlus.so" -exec cp {} $out/lib/ \;
    cp ${src}/pawno/include/* $out/include/ 2>/dev/null || true
    runHook postInstall
  '';

  meta.description = "PawnPlus plugin for SA-MP/open.mp";
}
