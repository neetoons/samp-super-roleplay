# Nix override for conan-omp.cmake
# Replaces Conan-based dependency fetching with Nix-provided packages.
# This file is included INSTEAD of lib/cmake-conan/conan.cmake and lib/cmake-conan/conan-omp.cmake

# No-op: conan_check is not needed
function(conan_check)
endfunction()

# Override: create CONAN_PKG::* INTERFACE targets from Nix-provided find_package results
function(_conan_omp_install_package pkg_name pkg_version pkg_options)
    get_property(_already_added GLOBAL PROPERTY CONAN_OMP_ADDED_LIBS)
    if(_already_added)
        list(FIND _already_added "${pkg_name}" _already_added_index)
        if(NOT _already_added_index EQUAL -1)
            return()
        endif()
    endif()

    # Map conan package names to cmake package names and target names
    if(pkg_name STREQUAL "nlohmann_json")
        find_package(nlohmann_json CONFIG REQUIRED)
        set(_targets nlohmann_json::nlohmann_json)
    elseif(pkg_name STREQUAL "openssl")
        find_package(OpenSSL REQUIRED)
        set(_targets OpenSSL::SSL OpenSSL::Crypto)
    elseif(pkg_name STREQUAL "ghc-filesystem")
        find_package(ghc_filesystem CONFIG REQUIRED)
        set(_targets ghcFilesystem::ghc_filesystem)
    elseif(pkg_name STREQUAL "sqlite3")
        find_package(SQLite3 REQUIRED)
        set(_targets SQLite::SQLite3)
    elseif(pkg_name STREQUAL "cxxopts")
        find_package(cxxopts CONFIG REQUIRED)
        set(_targets cxxopts::cxxopts)
    elseif(pkg_name STREQUAL "icu")
        find_package(ICU REQUIRED COMPONENTS uc i18n data)
        set(_targets ICU::uc ICU::i18n ICU::data)
    elseif(pkg_name STREQUAL "libelfin")
        find_package(libelfin REQUIRED)
        set(_targets libelfin::elfin libelfin::dwarf)
    else()
        message(FATAL_ERROR "Nix override: unknown package '${pkg_name}'")
    endif()

    if(NOT TARGET "CONAN_PKG::${pkg_name}")
        add_library("CONAN_PKG::${pkg_name}" INTERFACE IMPORTED GLOBAL)
    endif()

    target_link_libraries("CONAN_PKG::${pkg_name}" INTERFACE ${_targets})

    set_property(GLOBAL APPEND PROPERTY CONAN_OMP_ADDED_LIBS "${pkg_name}")
endfunction()

function(conan_omp_add_lib_opt pkg_name pkg_version pkg_options)
    _conan_omp_install_package("${pkg_name}" "${pkg_version}" "${pkg_options}")
endfunction()

function(conan_omp_add_lib pkg_name pkg_version)
    conan_omp_add_lib_opt("${pkg_name}" "${pkg_version}" "")
endfunction()
