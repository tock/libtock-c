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
    rev = "v1.13.0";
    sha256 = "sha256-NRcCPTrLFZLubI5KzMmDkKJdvCdbnW97JMZSmedAQ8s=";
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

    cargoHash =
      if lib.versionAtLeast lib.version "25.05pre" then
        "sha256-C1hg2/y557jRLkSBvFLxYKH+t8xEJudDvU72kO9sPug="
      else
        "sha256-UHAwk1fBcabRqy7VMhz4aoQuIur+MQshDOhC7KFyGm4=";
  };

  # The formatting scripts require a specific version of uncrustify:
  uncrustify-0_75_1 = stdenv.mkDerivation rec {
    pname = "uncrustify";
    version = "0.75.1";

    src = pkgs.fetchFromGitHub {
      owner = "uncrustify";
      repo = "uncrustify";
      rev = "uncrustify-${version}";
      sha256 = "sha256-wLzj/KcqXlcTsOJo7T166jLcWi1KNLmgblIqqkj7/9c=";
    };

    nativeBuildInputs = with pkgs; [
      cmake
      python3
    ];
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
      unzip
      openocd
      uncrustify-0_75_1
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
