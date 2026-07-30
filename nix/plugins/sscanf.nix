{ lib, stdenv, cmake, ninja, fetchFromGitHub }:

let
  src = fetchFromGitHub {
    owner = "Y-Less"; repo = "sscanf";
    rev = "27f0b725095bf40ed9c4d772acd7300b02be632b";
    hash = "sha256-13w2qv6pgdk9pm7k2q6hpnvibcqj167vyp8zga6a23i8s6jlbc3q";
  };

  subhook-src = fetchFromGitHub {
    owner = "tianocore"; repo = "edk2-subhook";
    rev = "83d4e1ebef3588fae48b69a7352cc21801cb70bc";
    hash = "sha256-0X9JrYh+g39tJxVn8ofTITet9Is3Oe1avWFzdsRkM2Q=";
  };
  glm-src = fetchFromGitHub {
    owner = "g-truc"; repo = "glm";
    rev = "cc98465e3508535ba8c7f6208df934c156a018dc";
    hash = "sha256-o5SevXlaK/hK9zHkMNQnU5j3HKq86A5grx1XoCcG4Tg=";
  };
  robin-hood-hashing-src = fetchFromGitHub {
    owner = "martinus"; repo = "robin-hood-hashing";
    rev = "fb1483621fda28d4afb31c0097c1a4a457fdd35b";
    hash = "sha256-0irA6suhGcxG+ZE6FQNU6CcJaMuWsS2CmISQd+JOv0k=";
  };
  string-view-lite-src = fetchFromGitHub {
    owner = "martinmoene"; repo = "string-view-lite";
    rev = "6e90d372626effce61053044f18516c3b96870f9";
    hash = "sha256-OnXFuMZmOzVnkUoQwWvBM49GSUgAWMZ3+/1xuTPHfYg=";
  };
  span-lite-src = fetchFromGitHub {
    owner = "martinmoene"; repo = "span-lite";
    rev = "0248244006ad3cf31b2430b6c93bd87277e46c03";
    hash = "sha256-1cp9KV/iR7a31r9EeDYkaWnj55aQWHYsjwrzkTlwMyY=";
  };
  openmp-sdk-src = fetchFromGitHub {
    owner = "openmultiplayer"; repo = "open.mp-sdk";
    rev = "b5c1677981dfc63b4c7850276a513d8290fe756d";
    hash = "sha256-C/4JxwjnE3YsVC4oOAWQrWQmE0/OoKrqr3eNtXBut0M=";
  };
in
stdenv.mkDerivation {
  pname = "sscanf";
  version = "2.15.1";
  inherit src;
  nativeBuildInputs = [ cmake ninja ];
  prePatch = ''
    rm -rf subhook glm robin-hood-hashing string-view-lite span-lite open.mp
    cp -r ${subhook-src} subhook
    cp -r ${glm-src} glm
    cp -r ${robin-hood-hashing-src} robin-hood-hashing
    cp -r ${string-view-lite-src} string-view-lite
    cp -r ${span-lite-src} span-lite
    cp -r ${openmp-sdk-src} open.mp
    chmod -R u+w .
  '';
  postPatch = ''
    sed -i 's/-m32 //g' CMakeLists.txt
  '';
  cmakeFlags = [ "-GNinja" "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_POLICY_VERSION_MINIMUM=3.5" ];
  installPhase = ''
    runHook preInstall
    mkdir -p $out/lib $out/include
    find . -name "libsscanf.so" -exec cp {} $out/lib/ \;
    mv $out/lib/libsscanf.so $out/lib/sscanf.so 2>/dev/null || true
    cp ${src}/sscanf2.inc $out/include/ 2>/dev/null || true
    runHook postInstall
  '';
  meta.description = "sscanf plugin for SA-MP/open.mp";
}
