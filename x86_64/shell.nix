let nixpkgs = import <nixpkgs> { };
in with nixpkgs;
nixpkgs.multiStdenv.mkDerivation {
  name = "nos-env";
  buildInputs = [ bochs glibc_multi libgcc nasm gnumake qemu clang-tools bear tinyxxd python313 gdb libisoburn ];
}
