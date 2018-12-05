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
  inherit (pkgs) stdenv;
  pythonPackages = stdenv.lib.fix' (self: with self; pkgs.python3Packages //
  {

    tockloader = buildPythonPackage rec {
      pname = "tockloader";
      version = "1.3.1";
      name = "${pname}-${version}";

      propagatedBuildInputs = [ argcomplete colorama crcmod pyserial pytoml ];

      src = fetchPypi {
        inherit pname version;
        sha256 = "1gralnhvl82xr7rkrmxj0c1rxn1y9dlbmkkrklcdjahragbknivn";
      };
    };
  });
  elf2tab = pkgs.rustPlatform.buildRustPackage rec {
    name = "elf2tab-${version}";
    version = "0.3.0";

    src = pkgs.fetchFromGitHub {
      owner = "tock";
      repo = "elf2tab";
      rev = "v${version}";
      sha256 = "1sf3ccqmr6d70shgf0zzxaiqfgk28yahvwdbhalkwaf2sg3yivpk";
    };

    cargoSha256 = "14w5haqbkd9pwdwc2rqknn4zzqkk93hf4ihdvh0q4lnih74wf7mj";
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
