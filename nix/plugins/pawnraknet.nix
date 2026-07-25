{ lib, stdenv, cmake, ninja, fetchFromGitHub }:

let
  src = fetchFromGitHub {
    owner = "katursis"; repo = "Pawn.RakNet";
    rev = "7e9ecc2d24445e81b43f07fe77906af276b52ae9";
    hash = "sha256-UnV/gTsr06Z84ou0dVszE+mM6iIcwKUuYvWCt0UZZwA=";
  };

  urmem-src = fetchFromGitHub {
    owner = "katursis"; repo = "urmem";
    rev = "16767778340d15a1dff9c477eecbe3085754ac84";
    hash = "sha256-5MHYkeFkjoGqJEM/LSjGZq/EGaQmbVpHxPgy2yw0QxE=";
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
  cmake-modules-src = fetchFromGitHub {
    owner = "katursis"; repo = "samp-cmake-modules";
    rev = "7fbf1922d750a8dc8dfe5569788c8815369b180e";
    hash = "sha256-kfq0V+NSKMoTausZGRjniSi+29DGKIH+DFlF+8gzlMA=";
  };
in
stdenv.mkDerivation {
  pname = "pawnraknet";
  version = "1.6.0";
  inherit src;
  nativeBuildInputs = [ cmake ninja ];
  prePatch = ''
    rm -rf lib/urmem lib/cpptoml lib/samp-ptl cmake/modules
    cp -r ${urmem-src} lib/urmem
    cp -r ${cpptoml-src} lib/cpptoml
    cp -r ${samp-ptl-src} lib/samp-ptl
    mkdir -p cmake
    cp -r ${cmake-modules-src} cmake/modules
    chmod -R u+w .
  '';
  postPatch = ''
    sed -i 's/-m32//g' CMakeLists.txt
    find . -name '*.cmake' -exec sed -i 's/-m32//g' {} +
  '';
  cmakeFlags = [ "-GNinja" "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_POLICY_VERSION_MINIMUM=3.5" "-DCMAKE_CXX_FLAGS=-fpermissive" ];
  installPhase = ''
    runHook preInstall
    mkdir -p $out/lib $out/include
    find . -name "*.so" -exec cp {} $out/lib/ \;
    find . -name "*.inc" -path "*/include/*" -exec cp {} $out/include/ \; 2>/dev/null || true
    runHook postInstall
  '';
  meta.description = "Pawn.RakNet plugin for SA-MP/open.mp";
}
