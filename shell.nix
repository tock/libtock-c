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
    version = "0.13.0";

    src = pkgs.fetchFromGitHub {
      owner = "tock";
      repo = "elf2tab";
      rev = "v${version}";
      sha256 = "sha256-tVxYdokrHzlA8SqZN/+kpznvnnjx7X8z2Du4p4nU0V4=";
    };

    cargoHash =
      if lib.versionAtLeast lib.version "25.05pre" then
        "sha256-5bQVk77gMtMfnRXf7rlRf/ABr73K9ZosB6E/gGZFpz4="
      else
        "sha256-A2w3nYw0A+qcZbVLC+C7ZLsWFcEaP8tc7XVBmuwsIgM=";
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
