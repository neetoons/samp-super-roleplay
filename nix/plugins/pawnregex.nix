{ lib, stdenv, cmake, ninja, fetchFromGitHub }:

let
  src = fetchFromGitHub {
    owner = "katursis"; repo = "Pawn.Regex";
    rev = "92b3d72cb95d914a907fd239cf14b5ba497e2122";
    hash = "sha256-w7/5uGfxLiwnjkeKo1BXHABnkCDa7a0y7NtYASgpOag=";
  };

  samp-ptl-src = fetchFromGitHub {
    owner = "katursis"; repo = "samp-ptl";
    rev = "dfee372920f52f7b4e846e164e15497a6b040a3a";
    hash = "sha256-UhhSrfAiUKtqKucnf6QSWj72MSMRi25e4wtj8eacuZ8=";
  };
  cpptoml-src = fetchFromGitHub {
    owner = "skystrife"; repo = "cpptoml";
    rev = "fededad7169e538ca47e11a9ee9251bc361a9a65";
    hash = "sha256-PcPIajifRQE0Qjx1rQX6vPRgq6lSCdZRrlNrmyZtj34=";
  };
  cmake-modules-src = fetchFromGitHub {
    owner = "katursis"; repo = "samp-cmake-modules";
    rev = "7fbf1922d750a8dc8dfe5569788c8815369b180e";
    hash = "sha256-kfq0V+NSKMoTausZGRjniSi+29DGKIH+DFlF+8gzlMA=";
  };
in
stdenv.mkDerivation {
  pname = "pawnregex";
  version = "1.2.3";
  inherit src;
  nativeBuildInputs = [ cmake ninja ];
  prePatch = ''
    rm -rf lib/samp-ptl lib/cpptoml cmake/modules
    cp -r ${samp-ptl-src} lib/samp-ptl
    cp -r ${cpptoml-src} lib/cpptoml
    mkdir -p cmake
    cp -r ${cmake-modules-src} cmake/modules
    chmod -R u+w .
  '';
  postPatch = ''
    sed -i 's/-m32//g' CMakeLists.txt
    find . -name '*.cmake' -exec sed -i 's/-m32//g' {} +
    sed -i '1i #include <limits>' lib/cpptoml/include/cpptoml.h
  '';
  cmakeFlags = [ "-GNinja" "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_POLICY_VERSION_MINIMUM=3.5" "-DCMAKE_CXX_FLAGS=-fpermissive" ];
  installPhase = ''
    runHook preInstall
    mkdir -p $out/lib $out/include
    find . -name "*.so" -exec cp {} $out/lib/ \;
    find . -name "*.inc" -path "*/include/*" -exec cp {} $out/include/ \; 2>/dev/null || true
    runHook postInstall
  '';
  meta.description = "Pawn.Regex plugin for SA-MP/open.mp";
}
