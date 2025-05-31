{
  description = "Win32 + Linux musl build env";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-24.11";
  };

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      pkgsMusl = import pkgs.path {
        inherit system;
        crossSystem = {
          config = "x86_64-unknown-linux-musl";
        };
      };
    in
    {
      devShell.${system} = pkgs.mkShell {
        buildInputs = [
          pkgs.gcc13
          pkgs.ninja
          pkgs.cmake
          pkgs.vcpkg
        ];
        CMAKE_GENERATOR = "Ninja";
        CMAKE_TOOLCHAIN_FILE = "${pkgs.vcpkg.outPath}/share/vcpkg/scripts/buildsystems/vcpkg.cmake";
      };
    };
}
