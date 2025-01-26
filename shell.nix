# Shell expression for the Nix package manager
#
# This nix expression creates an environment with necessary packages installed:
#
#  * `tockloader`
#  * arm-none-eabi toolchain
#  * elf2tab
#  * riscv32-embedded toolchain
#
# To use:
#
#  $ nix-shell

{ pkgs ? import <nixpkgs> {}, withUnfreePkgs ? false }:

with builtins;
let
  inherit (pkgs) stdenv stdenvNoCC lib;

  tockloader = import (pkgs.fetchFromGitHub {
    owner = "tock";
    repo = "tockloader";
    rev = "v1.12.0";
    sha256 = "sha256-VgbAKDY/7ZVINDkqSHF7C0zRzVgtk8YG6O/ZmUpsh/g=";
  }) { inherit pkgs withUnfreePkgs; };

  elf2tab = pkgs.rustPlatform.buildRustPackage rec {
    name = "elf2tab-${version}";
    version = "0.12.0";

    src = pkgs.fetchFromGitHub {
      owner = "tock";
      repo = "elf2tab";
      rev = "v${version}";
      sha256 = "sha256-+VeWLBI6md399Oaumt4pJrOkm0Nz7fmpXN2TjglUE34=";
    };

    cargoHash = "sha256-UHAwk1fBcabRqy7VMhz4aoQuIur+MQshDOhC7KFyGm4=";
  };
in
  pkgs.mkShell {
    name = "tock-dev";

    buildInputs = with pkgs; [
      elf2tab
      gcc-arm-embedded
      python3Full
      tockloader
      pkgsCross.riscv32-embedded.buildPackages.gcc
      uncrustify
      unzip
      openocd
    ] ++ (lib.optionals withUnfreePkgs [
      segger-jlink
      tockloader.nrf-command-line-tools
    ]);

    shellHook = ''
      # TODO: This should be patched into the rpath of the respective libraries!
      export LD_LIBRARY_PATH=${pkgs.libusb1}/lib:$LD_LIBRARY_PATH
    '' + (lib.optionalString withUnfreePkgs ''
      export LD_LIBRARY_PATH=${pkgs.segger-jlink}/lib:$LD_LIBRARY_PATH
    '');
  }
