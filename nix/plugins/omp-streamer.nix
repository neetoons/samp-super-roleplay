{ lib, clangStdenv, cmake, ninja, fetchFromGitHub, eigen, boost }:

let
  src = fetchFromGitHub {
    owner = "OpenSamp"; repo = "omp-streamer-component";
    rev = "0904eec6520f7ad764a3a7e12bbb459cbd7ae182";
    hash = "sha256-Dv/L/DcqX1DIrGIW6qknp0iWrvnEBA1UP5G+szzgaYo=";
  };

  sdk-src = fetchFromGitHub {
    owner = "openmultiplayer"; repo = "open.mp-sdk";
    rev = "473b732b2de159773b2e2908fd11aa99f07db284";
    hash = "sha256-czQOF2MlKWIlqKd6otAPwAgqCSOgb3l+G7RTxaL9Pic=";
    fetchSubmodules = true;
  };

  cmake-modules-src = fetchFromGitHub {
    owner = "AmyrAhmady"; repo = "samp-cmake-modules";
    rev = "c0a898698b0003b83c2b2aeaa8a2950d573cc79b";
    hash = "sha256-kiMyJknIMzXsfCbPLA5WuSIZlL7MObeKyrdcpjP+f6Y=";
  };
in
clangStdenv.mkDerivation {
  pname = "omp-streamer";
  version = "2.9.6";
  inherit src;
  nativeBuildInputs = [ cmake ninja ];
  buildInputs = [ eigen boost ];
  postPatch = ''
    rm -rf lib/open.mp-sdk lib/boost lib/cmake-modules lib/eigen
    cp -r ${sdk-src} lib/open.mp-sdk
    cp -r ${cmake-modules-src} lib/cmake-modules
    chmod -R u+w .

    sed -i 's/-Werror//g' CMakeLists.txt
    sed -i 's/-Werror//g' src/CMakeLists.txt
  '';
  NIX_CFLAGS_COMPILE = "-isystem ${boost.dev}/include";
  cmakeFlags = [
    "-GNinja"
    "-DCMAKE_BUILD_TYPE=RelWithDebInfo"
    "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
    "-DEIGEN3_INCLUDE_DIR=${eigen}/include/eigen3"
  ];
  installPhase = ''
    runHook preInstall
    mkdir -p $out/lib
    find . -name "streamer.so" -exec cp {} $out/lib/ \;
    mkdir -p $out/include
    cp ${src}/streamer.inc $out/include/
    runHook postInstall
  '';
  meta.description = "open.mp Streamer component (x64 compatible)";
}
