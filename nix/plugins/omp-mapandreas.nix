{ lib, clangStdenv, cmake, ninja, fetchurl, fetchFromGitHub, unzip }:

let
  omp-src = fetchFromGitHub {
    owner = "OpenSamp"; repo = "omp-component-mapandreas";
    rev = "8ffc603c394df9540df4ad27d522847fffeebb42";
    hash = "sha256-arBf6u56uNphxzcsQiM+KW0WBRcQE/Tr2b8rBv7mGkE=";
  };

  sdk-src = fetchFromGitHub {
    owner = "openmultiplayer"; repo = "open.mp-sdk";
    rev = "473b732b2de159773b2e2908fd11aa99f07db284";
    hash = "sha256-czQOF2MlKWIlqKd6otAPwAgqCSOgb3l+G7RTxaL9Pic=";
    fetchSubmodules = true;
  };

  hmap-src = fetchurl {
    url = "https://github.com/philip1337/samp-plugin-mapandreas/releases/download/v1.2.1/MapAndreas-linux-x86.zip";
    hash = "sha256-Aopz5C8sOXq7ywTRx14YtaZuTwWCFUTC0gvjntNjwFs=";
  };
in
clangStdenv.mkDerivation {
  pname = "omp-mapandreas";
  version = "1.2.0";
  src = omp-src;

  nativeBuildInputs = [ cmake ninja unzip ];

  # The open.mp port changed the legacy constant values (MODE_FULL=3 instead of 2,
  # MODE_NOBUFFER=4 instead of 3) and added MODE_MEDIUM=2.  Restore the original
  # SA-MP MapAndreas values so the gamemode (compiled with the original include)
  # passes the correct mode number.
  prePatch = ''
    rm -rf lib/open.mp-sdk
    cp -r ${sdk-src} lib/open.mp-sdk
    chmod -R u+w .

    sed -i 's/#define MAP_ANDREAS_MODE_MEDIUM 2 \/\/ currently unused/#define MAP_ANDREAS_MODE_MEDIUM 5 \/\/ currently unused/' src/MapAndreas.hpp
    sed -i 's/#define MAP_ANDREAS_MODE_FULL 3/#define MAP_ANDREAS_MODE_FULL 2/' src/MapAndreas.hpp
    sed -i 's/#define MAP_ANDREAS_MODE_NOBUFFER 4/#define MAP_ANDREAS_MODE_NOBUFFER 3/' src/MapAndreas.hpp
  '';

  cmakeFlags = [
    "-GNinja"
    "-DCMAKE_BUILD_TYPE=RelWithDebInfo"
    "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
  ];

  installPhase = ''
    runHook preInstall

    mkdir -p $out/lib $out/include $out/data

    find . -name "mapandreas.so" -exec cp {} $out/lib/ \;

    cp ${omp-src}/mapandreas.inc $out/include/
    unzip -j ${hmap-src} "linux/scriptfiles/SAfull.hmap" -d $out/data/

    runHook postInstall
  '';
  meta.description = "open.mp MapAndreas component";
}
