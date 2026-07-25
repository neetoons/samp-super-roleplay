{ lib, stdenv, cmake, ninja, boost, libmysqlclient, log-core, fetchFromGitHub }:

let
  src = fetchFromGitHub {
    owner = "pBlueG"; repo = "SA-MP-MySQL";
    rev = "9524fcc9088004948770ddd6e05a6074581d3095";
    hash = "sha256-FCynVHHQHtgQvAWs2RTIsgaD8EYV2ra949s2dyMxzbc=";
  };

  cmake-modules-src = fetchFromGitHub {
    owner = "samp-forks"; repo = "cmake-modules";
    rev = "61c95893445a4350303b16390988a076de554b5f";
    hash = "sha256-iBUh7qXx/Vu6mHH1PAeLkLuHbawRYz0xm8HpMSBEwwQ=";
  };
  fmt-src = fetchFromGitHub {
    owner = "fmtlib"; repo = "fmt";
    rev = "135ab5cf71ed731fc9fa0653051e7d4884a3652f";
    hash = "sha256-88eonGslDqCl/CJ92IOF2SWRx4PIjigJbzsn0mQ8/5Q=";
  };
  samp-sdk-src = fetchFromGitHub {
    owner = "maddinat0r"; repo = "samp-plugin-sdk";
    rev = "1d2e63238012d6ecac44326ca3a83e739cb783c6";
    hash = "sha256-9xGCFwIdm6o889mnyXUdPj6YJk3kq1A5j5Bk+DjyoH4=";
  };
in
stdenv.mkDerivation {
  pname = "mysql";
  version = "R41-4";
  inherit src;
  nativeBuildInputs = [ cmake ninja ];
  buildInputs = [ boost libmysqlclient libmysqlclient.dev log-core ];
  prePatch = ''
    rm -rf libs/cmake libs/fmt libs/sdk
    cp -r ${cmake-modules-src} libs/cmake
    cp -r ${fmt-src} libs/fmt
    cp -r ${samp-sdk-src} libs/sdk
    chmod -R u+w .
  '';
  postPatch = ''
    sed -i 's/-m32//g' CMakeLists.txt
    find . -name '*.cmake' -exec sed -i 's/-m32//g' {} +
    sed -i 's|cmake_policy(SET CMP0048 OLD)|cmake_policy(SET CMP0048 NEW)|g' libs/fmt/CMakeLists.txt
    sed -i 's|cmake_policy(SET CMP0063 OLD)|cmake_policy(SET CMP0063 NEW)|g' libs/fmt/CMakeLists.txt
  '';
  cmakeFlags = [
    "-GNinja"
    "-DCMAKE_BUILD_TYPE=Release"
    "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
    "-DMYSQLCAPI_INCLUDE_DIR=${libmysqlclient.dev}/include"
    "-DMYSQLCAPI_LIBRARY=${libmysqlclient}/lib/mariadb/libmariadb.so"
    "-DCMAKE_CXX_FLAGS=-fpermissive"
    "-DCMAKE_PREFIX_PATH=${log-core}"
  ];
  installPhase = ''
    runHook preInstall
    mkdir -p $out/lib $out/include
    find . -name "*.so" -exec cp {} $out/lib/ \;
    cp ${src}/src/include/a_mysql.inc $out/include/ 2>/dev/null || true
    runHook postInstall
  '';
  meta.description = "MySQL plugin for SA-MP/open.mp (BlueG)";
}
