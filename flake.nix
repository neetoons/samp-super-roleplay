{
  description = "SA-MP super roleplay server (open.mp based)";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    compiler-flake.url = "github:neetoons/compiler-flake";
  };

  outputs = { self, nixpkgs, compiler-flake }:
    let
      supportedSystems = [ "x86_64-linux" ];
      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;

      pkgsFor = system: import nixpkgs {
        inherit system;
        config.allowUnfree = true;
      };

      open-mp-source = builtins.fetchGit {
        url = "https://github.com/openmultiplayer/open.mp";
        rev = "f8058db80410b70f84c9089ec214530ca9784517";
        submodules = true;
      };
    in
    {
      packages = forAllSystems (system:
        let
          pkgs = pkgsFor system;
        in {
          open-mp = pkgs.callPackage ./nix/open-mp.nix {
            open-mp-src = open-mp-source;
          };

          default = pkgs.callPackage ./nix/package.nix {
            pawncc = compiler-flake.packages.${system}.default;
            open-mp-server = self.packages.${system}.open-mp;
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
          pawncc = compiler-flake.packages.${system}.default;
        in {
          default = pkgs.mkShell {
            name = "samp-server-env";

            buildInputs = [
              pawncc
            ];

            shellHook = ''
              echo "SA-MP Super Roleplay dev shell"
              echo "Compile: cd gamemodes && pawncc -Dgamemodes -i../pawno/include -d3 -Z \"-(+\" \"-;+\" srp.pwn"
            '';
          };
        }
      );

      nixosModules.default = import ./nix/module.nix { inherit self; };
    };
}
