{
  description = "Linux & MacOS build env";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-25.05";
  };

  outputs =
    { self, nixpkgs }:
    let
      linuxSystem = "x86_64-linux";
      darwinSystem = "x86_64-darwin";

      pkgsLinux = import nixpkgs { system = linuxSystem; };
      pkgsDarwin = import nixpkgs { system = darwinSystem; };
    in
    {
      devShells = {
        ${linuxSystem} = {
          default = pkgsLinux.mkShell {
            buildInputs = [
              pkgsLinux.gcc13
              pkgsLinux.ninja
              pkgsLinux.cmake
              pkgsLinux.vcpkg
            ];
            CMAKE_GENERATOR = "Ninja";
            CMAKE_TOOLCHAIN_FILE = "${pkgsLinux.vcpkg.outPath}/share/vcpkg/scripts/buildsystems/vcpkg.cmake";
          };
        };

        ${darwinSystem} = {
          default = pkgsDarwin.mkShell {
            buildInputs = [
              pkgsDarwin.gcc
              pkgsDarwin.ninja
              pkgsDarwin.cmake
              pkgsDarwin.vcpkg
            ];
            CMAKE_GENERATOR = "Ninja";
            CMAKE_TOOLCHAIN_FILE = "${pkgsDarwin.vcpkg.outPath}/share/vcpkg/scripts/buildsystems/vcpkg.cmake";
          };
        };
      };
    };
}
