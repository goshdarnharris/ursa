{
  description = "A Nix-flake-based C/C++ development environment";

  inputs =
    {
      nixpkgs.url = "https://flakehub.com/f/NixOS/nixpkgs/0"; # stable Nixpkgs
      flake-utils.url = "github:numtide/flake-utils";
      openocd-luminaire = {
        #Need to figure out how to make this work with a tag instead of a commit hash.
        url = "git+https://github.com/goshdarnharris/openocd?rev=140465617519720fee8764ac895e6ef3f2d9260d&submodules=1#";
        flake = true;
      };
      roast = {
        url = "git+ssh://git@bitbucket.org/luminairecoffee/roast?rev=e67bb84d09c1e1d32c76b8c6cd799334eadd99c1";
        flake = true;
      };
      roast-utils = {
        url = "git+ssh://git@bitbucket.org/luminairecoffee/environment?rev=0bf388387f1bc31236ede1dd58f774a54ecba27f";
        flake = true;
      };
      lumi = {
        url = "git+ssh://git@bitbucket.org/luminairecoffee/lumi?rev=508dc4d0d25db622916b0fe46ac5f44840ab5004";
        flake = true;
      };
    };

  outputs =
    { self, ... }@inputs:

    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
      ];
      forEachSupportedSystem =
        f:
        inputs.nixpkgs.lib.genAttrs supportedSystems (
          system:
          f {
            pkgs = import inputs.nixpkgs { inherit system; };
          }
        );
    in
    {
      devShells = forEachSupportedSystem (
        { pkgs }:
        {
          default =
            pkgs.mkShell
              {
                packages =
                  with pkgs;
                  [
                    gcc-arm-embedded-13
                    gcc16
                    llvmPackages_22.clangNoLibc.cc
                    tup
                    inputs.openocd-luminaire.packages.${system}.default
                    inputs.roast.packages.${system}.default
                    inputs.roast-utils.packages.${system}.default
                    inputs.lumi.packages.${system}.default
                  ];

                #Ideally this environment variable would be set as part of installing roast-utils,
                #but I haven't been able to figure out how to do that in a way that persists after the install.
                shellHook = ''
                  export PYTHONPATH="${inputs.roast-utils.outPath}/:$PYTHONPATH"
                '';
              };
        }
      );
    };
}