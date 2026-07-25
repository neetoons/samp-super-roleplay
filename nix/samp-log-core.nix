{ lib, stdenv, cmake, ninja, yaml-cpp, fetchFromGitHub }:

let
  src = fetchFromGitHub {
    owner = "maddinat0r"; repo = "samp-log-core";
    rev = "5ecc2e7083eae36bb8f715a65cc847a6121924af";
    hash = "sha256-S5SwMjgmognJm2qXhnNSPt1nOZSLbIrmQSVXrkmtwEA=";
  };

  cmake-modules-src = fetchFromGitHub {
    owner = "samp-forks"; repo = "cmake-modules";
    rev = "61c95893445a4350303b16390988a076de554b5f";
    hash = "sha256-iBUh7qXx/Vu6mHH1PAeLkLuHbawRYz0xm8HpMSBEwwQ=";
  };
  fmt-src = fetchFromGitHub {
    owner = "fmtlib"; repo = "fmt";
    rev = "9e554999ce02cf86fcdfe74fe740c4fe3f5a56d5";
    hash = "sha256-ZD8IJJxYK5jtnRh6YzUP++lHvxPQRcAms4XiopLRicI=";
  };
  tinydir-src = fetchFromGitHub {
    owner = "cxong"; repo = "tinydir";
    rev = "6a487f5896fcf1f1c0e9b16632a8da125d6dd725";
    hash = "sha256-qdVM1MQGKJrJrH50AQRQqxTJ95vHEFOkewihMHHfqcc=";
  };
in
stdenv.mkDerivation {
  pname = "samp-log-core";
  version = "0.4";
  inherit src;
  nativeBuildInputs = [ cmake ninja ];
  buildInputs = [ yaml-cpp ];
  prePatch = ''
    rm -rf libs/cmake libs/fmt libs/tinydir
    cp -r ${cmake-modules-src} libs/cmake
    cp -r ${fmt-src} libs/fmt
    cp -r ${tinydir-src} libs/tinydir
    chmod -R u+w .
  '';
  postPatch = ''
    find . -name '*.cmake' -exec sed -i 's/-m32//g' {} +
    sed -i 's/-m32//g' CMakeLists.txt
    sed -i 's/-m32//g' src/CMakeLists.txt
    sed -i '/#include "amx.h"/a #include <stdint.h>' src/amx/amx.c
    sed -i 's|goto \*\*cip|goto *(void*)(uintptr_t)*cip|g' src/amx/amx.c
    sed -i 's|(cell)cip-(cell)code|(cell)((uintptr_t)cip-(uintptr_t)code)|g' src/amx/amx.c
    find libs/fmt -name 'CMakeLists.txt' -exec sed -i 's|cmake_policy(SET CMP0048 OLD)|cmake_policy(SET CMP0048 NEW)|g' {} +
    find libs/fmt -name 'CMakeLists.txt' -exec sed -i 's|cmake_policy(SET CMP0063 OLD)|cmake_policy(SET CMP0063 NEW)|g' {} +
  '';
  cmakeFlags = [
    "-GNinja"
    "-DCMAKE_BUILD_TYPE=Release"
    "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
    "-DCMAKE_CXX_FLAGS=-fpermissive"
  ];
  meta.description = "SAMP logging library for MySQL plugin";
}
