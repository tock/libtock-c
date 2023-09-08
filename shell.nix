# Shell expression for the Nix package manager
#
# This nix expression creates an environment with necessary packages installed:
#
#  * `tockloader`
#  * arm-none-eabi toolchain
#  * elf2tab
#  * (optionally) riscv32-embedded toolchain
#
# To use:
#
#  $ nix-shell
#
# The RISC-V toolchain can be disabled optionally. This will further
# prevent RISC-V specific environment variables from being set in the
# Nix shell environment:
#
#  $ nix-shell shell.nix --arg disableRiscvToolchain true

{ pkgs ? import <nixpkgs> {}, disableRiscvToolchain ? false, withUnfreePkgs ? false }:

with builtins;
let
  inherit (pkgs) stdenv stdenvNoCC lib;

  # Tockloader v1.11.0pre-git
  tockloader = import (pkgs.fetchFromGitHub {
    owner = "tock";
    repo = "tockloader";
    rev = "df8823545cbdd3ef49ce3d255404b7adaef5fcfc";
    sha256 = "sha256-gl+uz+JrzZ6RRIu2r7xALtstKzhfiUENbKeNhuSNXAQ=";
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
      nrf-command-line-tools
      elf2tab
      gcc-arm-embedded
      python3Full
      tockloader
    ] ++ (lib.optionals withUnfreePkgs [
      segger-jlink
      tockloader.nrf-command-line-tools
    ]) ++ (lib.optional (!disableRiscvToolchain) (
      pkgsCross.riscv32-embedded.buildPackages.gcc.override (oldCc: {
        cc = (pkgsCross.riscv32-embedded.buildPackages.gcc.cc.override (oldCcArgs: {
          libcCross = oldCcArgs.libcCross.overrideAttrs (oldNewlibAttrs: {
            configureFlags = oldNewlibAttrs.configureFlags ++ [
              "--enable-libssp"
            ];
          });
        })).overrideAttrs (oldCcAttrs: {
          configureFlags = oldCcAttrs.configureFlags ++ [
            "--without-headers"
            "--disable-shared"
            "--disable-libssp"
            "--disable-multilib"
            "--with-newlib"
          ];
          gcc_cv_libc_provides_ssp = "yes";
        });
      })
    ));

    shellHook = ''
      ${if (!disableRiscvToolchain) then ''
        export RISCV=1
      '' else ""}

      # TODO: This should be patched into the rpath of the respective libraries!
      export LD_LIBRARY_PATH=${pkgs.libusb}/lib:${pkgs.segger-jlink}/lib:$LD_LIBRARY_PATH
    '';
  }
