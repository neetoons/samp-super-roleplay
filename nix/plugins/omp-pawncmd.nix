{ clangStdenv, cmake, ninja, fetchFromGitHub, python3 }:

let
  src = fetchFromGitHub {
    owner = "devbluen"; repo = "Pawn.CMD";
    rev = "bf8de0a1a091c9f38868c134dcc3526b0046eb7e";
    hash = "sha256-PxrBdblogDxF1BUNx3A6iAMYAvJnAmwlyTKgmgkueyk=";
    fetchSubmodules = true;
  };

  sdk-src = fetchFromGitHub {
    owner = "openmultiplayer"; repo = "open.mp-sdk";
    rev = "473b732b2de159773b2e2908fd11aa99f07db284";
    hash = "sha256-czQOF2MlKWIlqKd6otAPwAgqCSOgb3l+G7RTxaL9Pic=";
    fetchSubmodules = true;
  };
in
clangStdenv.mkDerivation {
  pname = "omp-pawncmd";
  version = "3.4.0";
  inherit src;
  nativeBuildInputs = [ cmake ninja python3 ];
  prePatch = ''
    rm -rf lib/omp-sdk
    cp -r ${sdk-src} lib/omp-sdk
    chmod -R u+w .
  '';
  postPatch = ''
    sed -i 's/-Werror=format//g' lib/omp-sdk/CMakeLists.txt
    sed -i 's/-Werror//g' lib/omp-sdk/CMakeLists.txt
    sed -i 's/-Werror=format//g' CMakeLists.txt
    sed -i 's/-Werror//g' CMakeLists.txt
    sed -i '/target_compile_options/s/)/ -Wno-invalid-partial-specialization)/g' lib/omp-sdk/CMakeLists.txt

    python3 ${../patches/omp-pawncmd-64bit.py}
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
    find . -name "pawncmd.so" -exec cp {} $out/lib/ \;
    mkdir -p $out/include
    cp ${src}/src/Pawn.CMD.inc $out/include/
    runHook postInstall
  '';
  meta.description = "Pawn.CMD open.mp component";
}
