{ lib, clangStdenv, cmake, ninja, fetchFromGitHub }:

let
  src = fetchFromGitHub {
    owner = "OpenSamp"; repo = "Pawn.RakNet.OMP";
    rev = "e6d80cec9256581b770492cb8895dc05dc667f96";
    hash = "sha256-aGmU45AOrz2R8OlMpMrsUVXvlAscV0oIqDObd3r2E1E=";
  };

  sdk-src = fetchFromGitHub {
    owner = "openmultiplayer"; repo = "open.mp-sdk";
    rev = "473b732b2de159773b2e2908fd11aa99f07db284";
    hash = "sha256-czQOF2MlKWIlqKd6otAPwAgqCSOgb3l+G7RTxaL9Pic=";
    fetchSubmodules = true;
  };

  cpptoml-src = fetchFromGitHub {
    owner = "skystrife"; repo = "cpptoml";
    rev = "fededad7169e538ca47e11a9ee9251bc361a9a65";
    hash = "sha256-PcPIajifRQE0Qjx1rQX6vPRgq6lSCdZRrlNrmyZtj34=";
  };

  samp-ptl-src = fetchFromGitHub {
    owner = "katursis"; repo = "samp-ptl";
    rev = "06b5e2b9c51879532368dd9e468b5e259ce5d7ce";
    hash = "sha256-aIK/kPt22tHLI3UvflkU0YvToK9+myexUtUgKS/4tRU=";
  };

  samp-cmake-src = fetchFromGitHub {
    owner = "katursis"; repo = "samp-cmake-modules";
    rev = "7fbf1922d750a8dc8dfe5569788c8815369b180e";
    hash = "sha256-kfq0V+NSKMoTausZGRjniSi+29DGKIH+DFlF+8gzlMA=";
  };
in
clangStdenv.mkDerivation {
  pname = "omp-pawnraknet";
  version = "1.6.0";
  inherit src;
  nativeBuildInputs = [ cmake ninja ];
  postPatch = ''
    mkdir -p lib/omp-sdk lib/cpptoml lib/samp-ptl cmake/modules
    rm -rf lib/omp-sdk lib/cpptoml lib/samp-ptl cmake/modules
    cp -r ${sdk-src} lib/omp-sdk
    cp -r ${cpptoml-src} lib/cpptoml
    cp -r ${samp-ptl-src} lib/samp-ptl
    cp -r ${samp-cmake-src} cmake/modules
    chmod -R u+w .

    sed -i 's/-Werror=format//g' lib/omp-sdk/CMakeLists.txt
    sed -i 's/-Werror//g' lib/omp-sdk/CMakeLists.txt
  '';
  cmakeFlags = [
    "-GNinja"
    "-DCMAKE_BUILD_TYPE=RelWithDebInfo"
    "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
    "-DCMAKE_CXX_FLAGS=-Wno-invalid-partial-specialization"
  ];
  installPhase = ''
    runHook preInstall
    mkdir -p $out/lib
    find . -name "pawnraknet.so" -exec cp {} $out/lib/ \;
    mkdir -p $out/include
    cp ${src}/src/Pawn.RakNet.inc $out/include/
    runHook postInstall
  '';
  meta.description = "Pawn.RakNet open.mp component";
}
