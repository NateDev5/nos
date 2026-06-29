let
  nixpkgs = import <nixpkgs> { };
in
with nixpkgs;
nixpkgs.multiStdenv.mkDerivation {
  name = "nos-env";
  buildInputs = [
    clang
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
