{
  description = "SA-MP super roleplay server";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    compiler-flake.url = "github:neetoons/compiler-flake";
  };

  outputs = { self, nixpkgs, compiler-flake }:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-linux" ];
      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;

      pkgsFor = system: import nixpkgs {
        inherit system;
        config.allowUnfree = true;
      };

      pkgs-i686For = system: import nixpkgs {
        system = "i686-linux";
      };
    in
    {
      packages = forAllSystems (system:
        let
          pkgs = pkgsFor system;
          pkgs-i686 = pkgs-i686For system;
        in {
          default = pkgs.callPackage ./nix/package.nix {
            pawncc = compiler-flake.packages.${system}.default;
            inherit pkgs-i686;
            sourceRoot = ./.;
          };
        }
      );

      apps = forAllSystems (system: {
        default = {
          type = "app";
          program = "${self.packages.${system}.default}/bin/samp-server";
        };
      });

      devShells = forAllSystems (system:
        let
          pkgs = pkgsFor system;
          pkgs-i686 = pkgs-i686For system;
          glibc_32 = pkgs-i686.glibc;
          pawncc = compiler-flake.packages.${system}.default;
        in {
          default = pkgs.mkShell {
            name = "samp-server-env";

            buildInputs = [
              pkgs.patchelf
              pkgs-i686.stdenv.cc.cc.lib
              pkgs-i686.zlib
              pkgs.libuuid
              pawncc
            ];

            shellHook = ''
              export LD_LIBRARY_PATH="${pkgs-i686.stdenv.cc.cc.lib}/lib:${pkgs-i686.zlib}/lib:$LD_LIBRARY_PATH"
              export INTERPRETER_32="${glibc_32}/lib/ld-linux.so.2"
              echo "type patchelf --set-interpreter \$INTERPRETER_32 samp03svr"
            '';
          };
        }
      );

      nixosModules.default = import ./nix/module.nix { inherit self; };
    };
}
