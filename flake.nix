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
    in
    {
      packages = forAllSystems (system:
        let
          pkgs = pkgsFor system;
        in {
          open-mp = pkgs.callPackage ./nix/open-mp.nix {};

          sscanf = pkgs.callPackage ./nix/plugins/sscanf.nix {};
          streamer = pkgs.callPackage ./nix/plugins/streamer.nix {};
          mysql = pkgs.callPackage ./nix/plugins/mysql.nix {
            log-core = pkgs.callPackage ./nix/samp-log-core.nix {};
          };
          omp-mapandreas = pkgs.callPackage ./nix/plugins/omp-mapandreas.nix {};
          omp-pawncmd = pkgs.callPackage ./nix/plugins/omp-pawncmd.nix {};
          omp-pawnraknet = pkgs.callPackage ./nix/plugins/omp-pawnraknet.nix {};
          omp-streamer = pkgs.callPackage ./nix/plugins/omp-streamer.nix {};
          ysi = pkgs.callPackage ./nix/plugins/ysi.nix {};

          default = pkgs.callPackage ./nix/package.nix {
            pawncc = compiler-flake.packages.${system}.default;
            open-mp-server = self.packages.${system}.open-mp;
            ysi = self.packages.${system}.ysi;
            plugins = {
              inherit (self.packages.${system})
                sscanf mysql;
            };
            components = {
              inherit (self.packages.${system})
                omp-mapandreas omp-pawncmd omp-pawnraknet omp-streamer;
            };
            src = pkgs.lib.fileset.toSource {
              root = ./.;
              fileset = pkgs.lib.fileset.unions [
                ./gamemodes
                ./filterscripts
                ./scriptfiles
                ./pawno
              ];
            };
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
        in {
          default = pkgs.mkShell {
            name = "samp-server-env";
            buildInputs = [
              compiler-flake.packages.${system}.default
              self.packages.${system}.ysi
            ];
            shellHook = ''
              echo "SA-MP Super Roleplay dev shell"
              echo "Compile: cd gamemodes && pawncc -Dgamemodes -i../pawno/include -i${self.packages.${system}.ysi}/include -d3 -Z \"-(+\" \"-;+\" srp.pwn"
            '';
          };
        }
      );

      nixosModules.default = import ./nix/module.nix;
    };
}
