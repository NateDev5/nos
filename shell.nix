{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell.override
  {
    stdenv = pkgs.multiStdenv;
  }
  {
    packages = with pkgs; [
      clang
	  clang-tools
      bochs
      glibc_multi
      libgcc
      nasm
      gnumake
      qemu
      bear
      tinyxxd
      python313
      gdb
      libisoburn
    ];
  }
