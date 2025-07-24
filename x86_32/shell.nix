let nixpkgs = import <nixpkgs> { };
in with nixpkgs;
stdenv.mkDerivation {
  name = "nos-env";
  buildInputs = [ libgcc nasm gnumake qemu clang clang-tools bear ];
}
