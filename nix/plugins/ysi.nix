{ lib, stdenv, fetchFromGitHub }:

stdenv.mkDerivation {
  pname = "ysi-includes";
  version = "nightly-56a553d";

  src = fetchFromGitHub {
    owner = "pawn-lang";
    repo = "YSI-Includes";
    rev = "56a553dc0a795a216f3150b4df43233945f53afe";
    hash = "sha256-wuN+9RF1ZKIymmuIE1WhNliIRsslJiVZo5+LouoLGhw=";
  };

  dontBuild = true;
  dontConfigure = true;
  dontFixup = true;

  installPhase = ''
    runHook preInstall
    mkdir -p $out/include
    cp -r YSI_Coding YSI_Core YSI_Data YSI_Extra YSI_Game YSI_Players YSI_Server YSI_Storage YSI_Visual $out/include/
    runHook postInstall
  '';

  meta = with lib; {
    description = "YSI Includes for Pawn (SA-MP/open.mp)";
    license = licenses.mit;
    platforms = platforms.unix;
  };
}
