{
  stdenv,
  fetchFromGitHub,
  writeText,
}: let
  nlohmann_jsonConfig = writeText "nlohmann_jsonConfig.cmake" ''
    if(NOT TARGET nlohmann_json::nlohmann_json)
      add_library(nlohmann_json::nlohmann_json INTERFACE IMPORTED)
      set_target_properties(nlohmann_json::nlohmann_json PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "''${CMAKE_CURRENT_LIST_DIR}/../../../include"
      )
    endif()
  '';

  nlohmann_jsonConfigVersion = writeText "nlohmann_jsonConfigVersion.cmake" ''
    set(PACKAGE_VERSION "3.9.1")
    if("''${PACKAGE_FIND_VERSION}" VERSION_GREATER "3.9.1")
      set(PACKAGE_VERSION_COMPATIBLE FALSE)
    else()
      set(PACKAGE_VERSION_COMPATIBLE TRUE)
      if("''${PACKAGE_FIND_VERSION}" VERSION_EQUAL "3.9.1")
        set(PACKAGE_VERSION_EXACT TRUE)
      endif()
    endif()
  '';

  cxxoptsConfig = writeText "cxxoptsConfig.cmake" ''
    if(NOT TARGET cxxopts::cxxopts)
      add_library(cxxopts::cxxopts INTERFACE IMPORTED)
      set_target_properties(cxxopts::cxxopts PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "''${CMAKE_CURRENT_LIST_DIR}/../../../include"
      )
    endif()
  '';

  cxxoptsConfigVersion = writeText "cxxoptsConfigVersion.cmake" ''
    set(PACKAGE_VERSION "2.2.1")
    if("''${PACKAGE_FIND_VERSION}" VERSION_GREATER "2.2.1")
      set(PACKAGE_VERSION_COMPATIBLE FALSE)
    else()
      set(PACKAGE_VERSION_COMPATIBLE TRUE)
      if("''${PACKAGE_FIND_VERSION}" VERSION_EQUAL "2.2.1")
        set(PACKAGE_VERSION_EXACT TRUE)
      endif()
    endif()
  '';
in {
  nlohmann_json_3_9_1 = stdenv.mkDerivation {
    pname = "nlohmann_json";
    version = "3.9.1";
    src = fetchFromGitHub {
      owner = "nlohmann";
      repo = "json";
      rev = "v3.9.1";
      hash = "sha256-THordDPdH2qwk6lFTgeFmkl7iDuA/7YH71PTUe6vJCs=";
    };
    dontConfigure = true;
    dontBuild = true;
    installPhase = ''
      mkdir -p $out/include $out/lib/cmake/nlohmann_json
      cp -r include/* $out/include/
      cp ${nlohmann_jsonConfig} $out/lib/cmake/nlohmann_json/nlohmann_jsonConfig.cmake
      cp ${nlohmann_jsonConfigVersion} $out/lib/cmake/nlohmann_json/nlohmann_jsonConfigVersion.cmake
    '';
  };

  cxxopts_2_2_1 = stdenv.mkDerivation {
    pname = "cxxopts";
    version = "2.2.1";
    src = fetchFromGitHub {
      owner = "jarro2783";
      repo = "cxxopts";
      rev = "v2.2.1";
      hash = "sha256-Ct4MuSn2/pDkmDSkVF/s16+MEGjdpsGomjxATQ85fjQ=";
    };
    dontConfigure = true;
    dontBuild = true;
    installPhase = ''
      mkdir -p $out/include $out/lib/cmake/cxxopts
      cp -r include/* $out/include/
      cp ${cxxoptsConfig} $out/lib/cmake/cxxopts/cxxoptsConfig.cmake
      cp ${cxxoptsConfigVersion} $out/lib/cmake/cxxopts/cxxoptsConfigVersion.cmake
    '';
  };
}
