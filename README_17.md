# Session 17 — Proxy DLL Fix & UX Improvements

## Problem: Game Stopped Starting After Mod Deployment

After a game update, Anno 1800 stopped launching entirely when the mod was installed. No splash screen, no error dialog — the process exited silently within milliseconds.

### Root Cause Chain

**1. Original proxy target became invalid**

The mod previously injected via `TextShaping.dll`, which Anno 1800 used to import. A game update removed it from the executable's import table, so dropping a replacement `TextShaping.dll` in the game directory had no effect. `version.dll` was considered as an alternative but is a Windows *Known DLL* — the OS always loads it from `System32` regardless of what sits in the application directory, so it cannot be proxied this way.

The solution was to switch to `bink2w64.dll` (the RAD Bink video codec), which Anno 1800 statically imports and which is already present in `Bin\Win64\`. The original DLL was renamed to `bink2w64_real.dll` and our proxy forwards all 79 of its exports to the real one.

**2. Proxy DLL had no exports (silent #pragma loss)**

After switching to the bink2w64 approach, the game still refused to start. `dumpbin /exports bink2w64.dll` revealed **zero exported functions** — the proxy was a blank DLL that satisfied none of the game's import requirements, causing immediate load failure.

Two independent build bugs caused this:

- **Precompiled header ordering**: `#pragma comment(linker, "/export:...")` lines were placed *before* `#include "pch.h"`. With `/Yu` (use precompiled header), the MSVC compiler silently discards everything that precedes the PCH include. Moving `#include "pch.h"` to the very first line fixed this.

- **Whole Program Optimization / LTCG**: The Release|x64 configuration had `WholeProgramOptimization` enabled. With LTCG active, the compiler emits IR object files in ANONYMOUS OBJECT format, and `.drectve` sections (which carry `#pragma comment(linker,...)` directives) are silently dropped by the linker. Disabling LTCG in `ProxyDLL.vcxproj` for Release|x64 restored the exports.

**3. Game crashed seconds after loading**

With exports fixed, the game started but crashed shortly after the proxy's init thread ran. The thread was calling `FreeLibraryAndExitThread(hm, 0)` on its own module handle after loading the hook DLL. Because `bink2w64.dll` is a *static import* of `Anno1800.exe`, unloading it mid-process corrupts the Windows loader's internal module list, which Ubisoft Connect's integrity check detects — causing a crash. The fix was to simply `return 0` from the init thread, leaving the proxy resident.

**4. Wrong hook DLL loaded (DX11 vs DX12 detection)**

The proxy was loading the DX12 hook even when the game was running in DX11 mode. The original detection used `GetModuleHandle("d3d12.dll")`, which always returns non-NULL because Anno 1800 statically imports *both* `d3d11.dll` and `d3d12.dll` regardless of the active renderer.

The fix reads the actual renderer setting from `<Documents>\Anno 1800\config\engine.ini`, which contains `"DeviceType": "DX11"` or `"DX12"`.

**5. ChecktDirectXVersion always returned true**

In `Global.h`, the `ChecktDirectXVersion` function used `=` (assignment) instead of `==` (comparison) in all four `if` conditions. This meant the function always evaluated the last branch as true, regardless of which DirectX version was being checked.

---

## Improvements Made

### Proxy DLL (`ProxyDLL/dllmain.cpp`)

- Forwards all 79 `bink2w64` exports to `bink2w64_real.dll`
- Reads `engine.ini` to select the correct hook DLL (DX11 or DX12)
- Writes a timestamped log to `<game root>\logs\modmenu.log` on each key event, including process ID and exe filename for easy identification in multi-process scenarios
- Init thread returns cleanly without freeing itself

### Hook DLLs (`Global.h`, `DirectX11ImGui.cpp`, `DirectX12ImGui.cpp`)

- Fixed the `=` vs `==` bug in `ChecktDirectXVersion`
- Added structured logging throughout: DLL attach, F7 detection, window focus acquisition, hook installation results
- Log tag (`DX11` / `DX12`) included in every line so proxy and hook output can be told apart in the shared log file
- Window class name made unique per DLL (`MJ_DX11` / `MJ_DX12`) to avoid `RegisterClassEx` collision when both hook DLLs load into the same process
- Replaced unsafe `wcscat`/`_wfopen` with their safe `_s` variants

### Items Combo (`Menu.cpp`)

- **First-letter keyboard navigation**: pressing a letter key while the Items tree is open jumps the selection to the first item whose name starts with that letter. Pressing the same letter again cycles to the next match, wrapping around. Disabled automatically while a number input field is being edited (`WantTextInput` guard).
- **Scroll-to-selected**: the combo list now scrolls to keep the selected item visible after a keyboard jump. Implemented via `BeginCombo` + `ImGuiListClipper` (replacing the simple `Combo` call) with `SetScrollHereY` triggered on selection change.

### UI Scale (`Menu.cpp`)

- A **UI Scale** slider (0.5× – 3.0×) added at the top of the menu window. Adjusts `ImGui::GetIO().FontGlobalScale` in real time, making the entire overlay readable on 2K/4K displays without requiring a rebuild.

### Project hygiene

- `.gitignore` added: excludes `Build/`, intermediate `x64/` directories, `*.obj`, `*.pdb`, `*.dll`, `*.exp`, MSBuild tracking files, and Visual Studio IDE files. The DirectX SDK's precompiled `.lib` files are intentionally kept tracked.
- `CLAUDE.md` added: documents build commands, deployment steps, ESET NOD32 exclusion requirements, and the four non-obvious build pitfalls discovered during this session.
