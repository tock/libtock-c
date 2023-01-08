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

{ pkgs ? import <nixpkgs> {}, disableRiscvToolchain ? false }:

with builtins;
let
  inherit (pkgs) stdenv lib;
  pythonPackages = lib.fix' (self: with self; pkgs.python3Packages //
  {

    tockloader = buildPythonPackage rec {
      pname = "tockloader";
      version = "1.7.0";
      name = "${pname}-${version}";

      propagatedBuildInputs = [ argcomplete colorama crcmod pyserial pytoml tqdm ];

      src = fetchPypi {
        inherit pname version;
        sha256 = "05ygljkpdympkq13rbnpz3i1h8xdsrxz0cj2i1bkbs0aswq4sc8b";
      };
    };
  });
  elf2tab = pkgs.rustPlatform.buildRustPackage rec {
    name = "elf2tab-${version}";
    version = "0.7.0";

    src = pkgs.fetchFromGitHub {
      owner = "tock";
      repo = "elf2tab";
      rev = "v${version}";
      sha256 = "16k8i03p3hbmrgz9xvv5wm3azrqbq2j7858f75b8yrm3w93dwlrv";
    };

    cargoSha256 = "14z6564jmxd2627m5zjsnc3qjsxy5fymnxlmz0fjhi4gkwyiygjk";
  };
in
  pkgs.mkShell {
    name = "tock-dev";

    buildInputs = with pkgs; [
      elf2tab
      gcc-arm-embedded
      python3Full
      pythonPackages.tockloader
    ] ++ (lib.optional (!disableRiscvToolchain) (
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
    '';
  }
