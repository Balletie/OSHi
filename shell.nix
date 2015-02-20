let pkgs = import <nixpkgs> {};
    stdenv = pkgs.stdenv;
in {
  cDevEnv = stdenv.mkDerivation {
    name = "c-dev-env";
    src = ./.;
    buildInputs = [pkgs.gcc pkgs.valgrind];
  }; 
}
