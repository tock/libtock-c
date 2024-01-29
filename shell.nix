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
    rev = "v1.11.0";
    sha256 = "sha256-bPEfpfOZOjOiazqRgn1cnqe4ohLPvocuENKoZx/Qw80=";
  }) { inherit pkgs withUnfreePkgs; };

  elf2tab = pkgs.rustPlatform.buildRustPackage rec {
    name = "elf2tab-${version}";
    version = "0.11.0";

    src = pkgs.fetchFromGitHub {
      owner = "tock";
      repo = "elf2tab";
      rev = "v${version}";
      sha256 = "sha256-cjDFi9vaD9O2oVtGAapvvHrA+yUe17teoVzTso2enpI=";
    };

    cargoSha256 = "sha256-KGPp6Dx1aUX8XILfV8kbiXKinoBVkEmBRxD9mWrsVNk=";
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
    ];

    # Unfortunately, `segger-jlink` has been removed from Nixpkgs due to its
    # hard dependency in Qt4, which has multiple security issues and is
    # deprecated since a few years now. Efforts exist to bring the package back,
    # but for now we don't assume it's available. Once [1] is merged, we can add
    # the following back:
    #
    # buildInputs ++ (lib.optionals withUnfreePkgs [
    #   segger-jlink
    #   tockloader.nrf-command-line-tools
    # ])
    #
    # shellHook = ''
    #   # TODO: This should be patched into the rpath of the respective libraries!
    #   export LD_LIBRARY_PATH=${pkgs.libusb}/lib:${pkgs.segger-jlink}/lib:$LD_LIBRARY_PATH
    # '';
    #
    # [1]: https://github.com/NixOS/nixpkgs/pull/255185
  }
