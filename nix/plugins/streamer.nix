{ lib, stdenv, cmake, ninja, fetchFromGitHub }:

let
  src = fetchFromGitHub {
    owner = "samp-incognito"; repo = "samp-streamer-plugin";
    rev = "afca207b148da6e0dea03693042a1e9665457403";
    hash = "sha256-Uyex+zS0O/VZN5rRP9REM9AN/wN/qg6X+8/782N2ook=";
  };

  boost-headers-src = fetchFromGitHub {
    owner = "scipy"; repo = "boost-headers-only";
    rev = "d8626c9d2d937abf6a38a844522714ad72e63281";
    hash = "sha256-O7ypSgrVRiwTUuM9Agh9NnaLPkYwu8tzASP2CLSZcTQ=";
  };
  cmake-modules-src = fetchFromGitHub {
    owner = "samp-forks"; repo = "cmake-modules";
    rev = "61c95893445a4350303b16390988a076de554b5f";
    hash = "sha256-iBUh7qXx/Vu6mHH1PAeLkLuHbawRYz0xm8HpMSBEwwQ=";
  };
  eigen-src = fetchFromGitHub {
    owner = "eigenteam"; repo = "eigen-git-mirror";
    rev = "c3327783ac21591d0e87cca487c61a4cdea8dc4d";
    hash = "sha256-jDGPHzRCNvK1n/9z0yADXhYj6PoKqQpF1lfoq2Ohwhk=";
  };
  samp-sdk-src = fetchFromGitHub {
    owner = "maddinat0r"; repo = "samp-plugin-sdk";
    rev = "1d2e63238012d6ecac44326ca3a83e739cb783c6";
    hash = "sha256-9xGCFwIdm6o889mnyXUdPj6YJk3kq1A5j5Bk+DjyoH4=";
  };
in
stdenv.mkDerivation {
  pname = "streamer";
  version = "2.9.6";
  inherit src;
  nativeBuildInputs = [ cmake ninja ];
  prePatch = ''
    rm -rf lib/boost lib/cmake-modules lib/eigen lib/samp-plugin-sdk
    cp -r ${boost-headers-src} lib/boost
    cp -r ${cmake-modules-src} lib/cmake-modules
    cp -r ${eigen-src} lib/eigen
    cp -r ${samp-sdk-src} lib/samp-plugin-sdk
    chmod -R u+w .
  '';
  postPatch = ''
    find . -name '*.cmake' -exec sed -i 's/-m32//g' {} +
    sed -i '/#error Unsupported architecture/d' src/sampgdk.h
    sed -i 's/-Werror//g' CMakeLists.txt
    find . -name '*.cmake' -exec sed -i 's/-Werror//g' {} +
  '';
  NIX_CFLAGS_COMPILE = "-Wno-template-body -fpermissive";
  cmakeFlags = [
    "-GNinja"
    "-DCMAKE_BUILD_TYPE=Release"
    "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
  ];
  installPhase = ''
    runHook preInstall
    mkdir -p $out/lib $out/include
    find . -name "*.so" -exec cp {} $out/lib/ \;
    cp ${src}/streamer.inc $out/include/ 2>/dev/null || true
    runHook postInstall
  '';
  meta.description = "Streamer plugin for SA-MP/open.mp";
}
