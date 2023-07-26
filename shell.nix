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

  nrf-command-line-tools = stdenvNoCC.mkDerivation {
    pname = "nrf-command-line-tools";
    version = "10.22.1";

    src = builtins.fetchurl {
      url = "https://nsscprodmedia.blob.core.windows.net/prod/software-and-other-downloads/desktop-software/nrf-command-line-tools/sw/versions-10-x-x/10-22-1/nrf-command-line-tools-10.22.1_linux-amd64.tar.gz";
      sha256 = "sha256:0i3dfhp75rizs7kxyfka166k3zy5hmb28c25377pgnzk6w1yx383";
    };

    nativeBuildInputs = with pkgs; [
      autoPatchelfHook
    ];

    propagatedBuildInputs = with pkgs; [
      segger-jlink libusb
    ];

    installPhase = ''
      mkdir -p $out/
      cp -r * $out/
    '';
  };

  pythonPackages = lib.fix' (self: with self; pkgs.python3Packages //
  {
    siphash = buildPythonPackage rec {
      pname = "siphash";
      version = "0.0.1";

      src = fetchPypi {
        inherit pname version;
        sha256 = "sha256-rul/6V4JoplYGcBYpeSsbZZmGomNf+CtVeO3LJox1GE=";
      };
    };

    pynrfjprog = buildPythonPackage {
      pname = "pynrfjprog";
      version = nrf-command-line-tools.version;

      src = nrf-command-line-tools.src;

      preConfigure = ''
        cd ./python
      '';

      format = "pyproject";

      nativeBuildInputs = [
        setuptools
        pkgs.autoPatchelfHook
      ];

      buildInputs = [
        nrf-command-line-tools
      ];

      propagatedBuildInputs = [
        tomli-w
        future
      ];
    };

    tockloader = buildPythonPackage rec {
      pname = "tockloader";
      version = "1.10.0";
      name = "${pname}-${version}";

      propagatedBuildInputs = [
        argcomplete
        colorama
        crcmod
        pyserial
        toml
        tqdm
        questionary
        pycrypto
        siphash
      ] ++ (lib.optional withUnfreePkgs pynrfjprog);

      src = fetchPypi {
        inherit pname version;
        sha256 = "sha256-TFOCtrYp0+I5tfj/m5BE+QIIUrQgIFicyzGKF7KJ0HY=";
      };

      # Dependency checks require unfree software
      doCheck = withUnfreePkgs;
    };
  });
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
      pythonPackages.tockloader
    ] ++ (lib.optionals withUnfreePkgs [
      segger-jlink
      nrf-command-line-tools
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
