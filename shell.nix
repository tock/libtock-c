# Shell expression for the Nix package manager
#
# This nix expression creates an environment with necessary packages installed:
#
#  * `tockloader`
#  * arm-none-eabi toolchain
#  * elf2tab
#
# To use:
#
#  $ nix-shell
#

{ pkgs ? import <nixpkgs> {} }:

with builtins;
let
  inherit (pkgs) stdenv lib;
  pythonPackages = lib.fix' (self: with self; pkgs.python3Packages //
  {

    tockloader = buildPythonPackage rec {
      pname = "tockloader";
      version = "1.6.0";
      name = "${pname}-${version}";

      propagatedBuildInputs = [ argcomplete colorama crcmod pyserial pytoml ];

      src = fetchPypi {
        inherit pname version;
        sha256 = "1aqkj1nplcw3gmklrhq6vxy6v9ad5mqiw4y1svasak2zkqdk1wyc";
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
  with pkgs;
  stdenv.mkDerivation {
    name = "tock-dev";
    buildInputs = [
      elf2tab
      gcc-arm-embedded
      python3Full
      pythonPackages.tockloader
      ];
  }
