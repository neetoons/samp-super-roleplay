{ lib, clangStdenv, cmake, ninja, fetchFromGitHub }:

let
  src = fetchFromGitHub {
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
in
clangStdenv.mkDerivation {
  pname = "omp-mapandreas";
  version = "1.2.0";
  inherit src;
  nativeBuildInputs = [ cmake ninja ];
  prePatch = ''
    rm -rf lib/open.mp-sdk
    cp -r ${sdk-src} lib/open.mp-sdk
    chmod -R u+w .
  '';
  cmakeFlags = [
    "-GNinja"
    "-DCMAKE_BUILD_TYPE=RelWithDebInfo"
    "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
  ];
  installPhase = ''
    runHook preInstall
    mkdir -p $out/lib
    find . -name "mapandreas.so" -exec cp {} $out/lib/ \;
    mkdir -p $out/include
    cp ${src}/mapandreas.inc $out/include/
    runHook postInstall
  '';
  meta.description = "open.mp MapAndreas component";
}
