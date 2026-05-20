# Anno1800ModMenu

DLL-injection mod menu for Anno 1800, using Dear ImGui over DirectX 11/12 via MinHook vtable hooking.

## Build

Requires Visual Studio build tools available via `PATH`. MSBuild is found via `$env:VSHOME\MSBuild\Current\Bin\MSBuild.exe`.

The project files target toolset `v143` (VS 2022), but the installed compiler is VS 2026 (`v145`). Always override the toolset:

```powershell
# Hook DLLs (Anno1800ModMenuDX11.dll + Anno1800ModMenuDX12.dll)
& "$env:VSHOME\MSBuild\Current\Bin\MSBuild.exe" Anno1800ImGui.sln /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v145 /m

# Proxy DLL (bink2w64.dll) — must use the .sln, not .vcxproj directly
& "$env:VSHOME\MSBuild\Current\Bin\MSBuild.exe" ProxyDLL.sln /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v145 /m
```

Output goes to `Build\`:
- `Build\Anno1800ModMenuDX11.dll`
- `Build\Anno1800ModMenuDX12.dll`
- `Build\bink2w64.dll`

## Deployment

The proxy DLL is `bink2w64.dll` (switched from `TextShaping.dll` — the game stopped importing it after an update, and `version.dll` is a Windows Known DLL and cannot be proxied from the app directory).

`bink2w64.dll` (RAD Bink video codec) is already in `Bin\Win64\`. The proxy forwards all 80 of its exports to `bink2w64_real.dll` (the renamed original).

Deploy steps (once, to set up):

1. In `S:\SteamLibrary\steamapps\common\Anno 1800\Bin\Win64\`:
   - Rename the existing **`bink2w64.dll`** → **`bink2w64_real.dll`**
   - Delete the old `TextShaping.dll` and `version.dll` if present (both are inert)

2. Copy build outputs to the same directory:
   - `Build\bink2w64.dll`
   - `Build\Anno1800ModMenuDX11.dll`
   - `Build\Anno1800ModMenuDX12.dll`

On subsequent rebuilds only the three files in step 2 need to be recopied.

The log file appears at `S:\SteamLibrary\steamapps\common\Anno 1800\logs\modmenu.log` after first game launch.

## Usage

Press **F7** while the Anno 1800 window is in the foreground to toggle the mod menu overlay.

## ESET NOD32 — Required Exclusions

The hook DLLs use vtable patching and MinHook trampolines, which ESET NOD32 heuristics detect as `Win64/GenKryptik_AGen.CFZ` (false positive). NOD32 silently deletes the DLLs on file creation or game load, which is the most likely cause if F7 stops working.

Add real-time protection exclusions for:
- The build output directory: `D:\xep\2see\Anno1800ModMenu.My\Build\`
- The game directory: `S:\SteamLibrary\steamapps\common\Anno 1800\Bin\Win64\`

Without these exclusions, the freshly built or deployed DLLs will be deleted before they can be used.

## Key Implementation Notes

### DX version detection
Anno1800.exe statically imports BOTH `d3d11.dll` AND `d3d12.dll`, so `GetModuleHandle("d3d12.dll")` always returns non-NULL regardless of rendering mode. The proxy reads `<Documents>\Anno 1800\config\engine.ini` (JSON, `"DeviceType": "DX11"` or `"DX12"`) to determine which hook DLL to load.

### Proxy DLL must NOT free itself
`bink2w64.dll` is a static import of Anno1800.exe. Do NOT call `FreeLibraryAndExitThread` on the proxy's own module handle — unloading a static-import DLL mid-process corrupts the loader's module list and crashes the game. The Init thread simply returns after loading the hook DLL.

### #pragma exports and precompiled headers
`#pragma comment(linker, "/export:Name=dll.Name")` in `ProxyDLL\dllmain.cpp` **must come after** `#include "pch.h"`. With `/Yu` (use precompiled header), the compiler discards everything before the PCH include — placing the pragmas before it produces a DLL with no export table and the game fails to start immediately with no diagnostic.

### Build via .sln not .vcxproj
Building `ProxyDLL.vcxproj` directly sets `$(SolutionDir)` to the `.vcxproj`'s own directory, sending output to `ProxyDLL\Build\` instead of the shared `Build\`. Always build via `ProxyDLL.sln` to get output in the correct `Build\` directory.

### WholeProgramOptimization disabled in ProxyDLL
The ProxyDLL Release|x64 configuration has `WholeProgramOptimization` set to `false`. With LTCG enabled, the compiler emits IR object files (ANONYMOUS OBJECT format) where `#pragma comment(linker,...)` directives are silently dropped, again producing no exports.
