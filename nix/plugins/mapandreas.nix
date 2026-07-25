{ lib, stdenv, cmake, ninja, fetchFromGitHub }:

let
  src = fetchFromGitHub {
    owner = "philip1337"; repo = "samp-plugin-mapandreas";
    rev = "92b93aa44d2640249d97adde3ec7798ce4f36b79";
    hash = "sha256-L/VuwxK/JZT7ByB6ZrreMWkQ3LErtohV8au98MULeLc=";
  };

  cmake-modules-src = fetchFromGitHub {
    owner = "samp-forks"; repo = "cmake-modules";
    rev = "61c95893445a4350303b16390988a076de554b5f";
    hash = "sha256-iBUh7qXx/Vu6mHH1PAeLkLuHbawRYz0xm8HpMSBEwwQ=";
  };
  samp-sdk-src = fetchFromGitHub {
    owner = "maddinat0r"; repo = "samp-plugin-sdk";
    rev = "1d2e63238012d6ecac44326ca3a83e739cb783c6";
    hash = "sha256-9xGCFwIdm6o889mnyXUdPj6YJk3kq1A5j5Bk+DjyoH4=";
  };
in
stdenv.mkDerivation {
  pname = "mapandreas";
  version = "1.2.1";
  inherit src;
  nativeBuildInputs = [ cmake ninja ];
  prePatch = ''
    mkdir -p external
    rm -rf external/sampcmake external/sampsdk
    cp -r ${cmake-modules-src} external/sampcmake
    cp -r ${samp-sdk-src} external/sampsdk
    chmod -R u+w external
    touch external/sampsdk/plugin.h
  '';
  postPatch = ''
    chmod -R u+w .
    find . -name '*.cmake' -exec sed -i 's/-m32//g' {} +
  '';
  cmakeFlags = [
    "-GNinja"
    "-DCMAKE_BUILD_TYPE=Release"
    "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
    "-DCMAKE_CXX_FLAGS=-fpermissive"
  ];
  installPhase = ''
    runHook preInstall
    mkdir -p $out/lib $out/include
    find . -name "*.so" -exec cp {} $out/lib/ \;
    cp ${src}/mapandreas.inc $out/include/ 2>/dev/null || true
    runHook postInstall
  '';
  meta.description = "MapAndreas plugin for SA-MP/open.mp";
}
