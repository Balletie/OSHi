{ stdenv ? (import <nixpkgs> {}).stdenv }:

stdenv.mkDerivation {
    name = "OSHi";
    src = ./.;
}
