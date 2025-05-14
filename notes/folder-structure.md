# Ace Engine Folder Structure

```
Ace/                            # Root Folder
|   build/                      # Out-of-source build artifacts
|   |   linux-x64/
|   |   |   Debug/
|   |   |   Release/
|   |   |   Distribute/
|   |   windows-x64/...
|   assets/                     # Assets bundled by asset-bundler
|   engine/
|   |   Ace/
|   |   |   System/
|   |   |   Window/
|   |   |   Graphics/
|   |   |   Audio/
|   |   |   Scripting/
|   |   |   Networking/
|   |   |   Platform/
|   |   Common.hpp
|   examples/
|   |   Sandbox/
|   external/                   # Vendored libraries (if any, via `git submoudle`)
|   tools/                      # Command-line tools, including `premake5`, asset-bundlers, etc.
|   scripts/                    # Helper scripts (CI, installers, build scripts, etc.)
|   docs/                       # Design documents, API references
|   notes/                      # Development notes, AI prompts/responses
|   premake5.lua                # Root `premake5` script
|   .gitignore
|   README.md
|   LICENSE
```
