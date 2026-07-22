{ lib
, clangStdenv
, fetchFromGitHub
, cmake
, ninja
, pkg-config
, openssl
, ghc_filesystem
, sqlite
, writeText
, open-mp-src
}:

let
  pinned = import ./pinned.nix { stdenv = clangStdenv; inherit fetchFromGitHub writeText; };
in
clangStdenv.mkDerivation {
  pname = "open-mp-server";
  version = "1.5.8";

  src = open-mp-src;

  nativeBuildInputs = [
    cmake
    ninja
    pkg-config
  ];

  buildInputs = [
    pinned.nlohmann_json_3_9_1
    openssl
    ghc_filesystem
    sqlite
    pinned.cxxopts_2_2_1
  ];

  prePatch = ''
    mkdir -p nix
    cp ${./conan-override.cmake} nix/conan-override.cmake
  '';

  postPatch = ''
    substituteInPlace lib/CMakeLists.txt \
      --replace-warn 'include(''${CMAKE_SOURCE_DIR}/lib/cmake-conan/conan.cmake)' \
      'include(''${CMAKE_SOURCE_DIR}/nix/conan-override.cmake)' \
      --replace-warn 'include(''${CMAKE_SOURCE_DIR}/lib/cmake-conan/conan-omp.cmake)' \
      '# skipped - using nix override'
  '';

  cmakeFlags = [
    "-GNinja"
    "-DCMAKE_BUILD_TYPE=RelWithDebInfo"
    "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
    "-DTARGET_BUILD_ARCH=x86_64"
    "-DSHARED_OPENSSL=ON"
    "-DBUILD_SERVER=ON"
    "-DBUILD_PAWN_COMPONENT=ON"
    "-DBUILD_LEGACY_COMPONENTS=ON"
    "-DBUILD_SQLITE_COMPONENT=ON"
    "-DBUILD_ABI_CHECK_TOOL=OFF"
    "-DBUILD_UNICODE_COMPONENT=OFF"
    "-DBUILD_FIXES_COMPONENT=OFF"
    "-DBUILD_TEST_COMPONENTS=OFF"
  ];

  enableParallelBuilding = true;

  installPhase = ''
    runHook preInstall
    mkdir -p $out/bin
    mkdir -p $out/lib/open-mp/components
    cp -r Output/RelWithDebInfo/Server/* $out/bin/
    runHook postInstall
  '';

  meta = with lib; {
    description = "open.mp (open multiplayer) - SA-MP game server";
    homepage = "https://github.com/openmultiplayer/open.mp";
    license = licenses.mit;
    platforms = platforms.linux;
  };
}
