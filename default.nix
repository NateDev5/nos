let nixpkgs = import <nixpkgs> { };
in with nixpkgs;
stdenv.mkDerivation {
  name = "nox-env-vscode";
  buildInputs = [ clang clang-tools ];
}
