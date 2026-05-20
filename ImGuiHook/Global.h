#include <windows.h>
#include <stdint.h>
#include <assert.h>
#include <psapi.h>
#include <cstdio>
#include <cstdarg>
#include "..\ImGuiHook\Directories\MinHook\Include\MinHook.h"

static HMODULE gLogModule = NULL;
static const char* gLogTag = "?";

static void Log(const char* fmt, ...) {
    // Build path: <module_dir>/../../logs/modmenu.log  (game root / logs /)
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(gLogModule, path, MAX_PATH);
    wchar_t* slash = wcsrchr(path, L'\\'); if (slash) *slash = L'\0'; // strip filename -> Bin\Win64
    slash = wcsrchr(path, L'\\');          if (slash) *slash = L'\0'; // strip Win64   -> Bin
    slash = wcsrchr(path, L'\\');          if (slash) *slash = L'\0'; // strip Bin     -> game root
    wcscat_s(path, MAX_PATH, L"\\logs");
    CreateDirectoryW(path, NULL);
    wcscat_s(path, MAX_PATH, L"\\modmenu.log");

    FILE* f = nullptr;
    _wfopen_s(&f, path, L"a");
    if (!f) return;

    // Get current process exe filename for easy identification
    wchar_t exePath[MAX_PATH] = {};
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    const wchar_t* exeName = wcsrchr(exePath, L'\\');
    exeName = exeName ? exeName + 1 : exePath;

    SYSTEMTIME st;
    GetLocalTime(&st);
    fprintf(f, "[%02d:%02d:%02d.%03d] [%s pid=%lu exe=%ls] ",
            st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
            gLogTag, GetCurrentProcessId(), exeName);
    va_list args;
    va_start(args, fmt);
    vfprintf(f, fmt, args);
    va_end(args);
    fputc('\n', f);
    fclose(f);
}

#if defined _M_X64
typedef uint64_t uintx_t;
#elif defined _M_IX86
typedef uint32_t uintx_t;
#endif

static uintx_t* MethodsTable = NULL;

struct _DirectXVersion {
	int Unknown = 0;
	int D3D9 = 1;
	int D3D10 = 2;
	int D3D11 = 3;
	int D3D12 = 4;
}DirectXVersion;

bool ChecktDirectXVersion(int _DirectXVersion) {
	if (_DirectXVersion == DirectXVersion.D3D12) {
		if (GetModuleHandle("d3d12.dll") != NULL) {
			return true;
		}
	}

	if (_DirectXVersion == DirectXVersion.D3D11) {
		if (GetModuleHandle("d3d11.dll") != NULL) {
			return true;
		}
	}

	if (_DirectXVersion == DirectXVersion.D3D10) {
		if (GetModuleHandle("d3d10.dll") != NULL) {
			return true;
		}
	}

	if (_DirectXVersion == DirectXVersion.D3D9) {
		if (GetModuleHandle("d3d9.dll") != NULL) {
			return true;
		}
	}

	return false;
}

WNDCLASSEX WindowClass;
HWND WindowHwnd;
// Unique per-DLL window class name — avoids collision when DX11 and DX12 hooks both load
static char gWindowClassName[32] = "MJ";

bool InitWindow() {
	// Use gLogTag ("DX11" or "DX12") to make the class name unique within the process
	wsprintfA(gWindowClassName, "MJ_%s", gLogTag);

	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = DefWindowProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandle(NULL);
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = NULL;
	WindowClass.hbrBackground = NULL;
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = gWindowClassName;
	WindowClass.hIconSm = NULL;
	RegisterClassEx(&WindowClass);
	WindowHwnd = CreateWindow(WindowClass.lpszClassName, "DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL);
	if (WindowHwnd == NULL) {
		return false;
	}
	return true;
}

bool DeleteWindow() {
	DestroyWindow(WindowHwnd);
	UnregisterClass(gWindowClassName, WindowClass.hInstance);
	if (WindowHwnd != NULL) {
		return false;
	}
	return true;
}

bool CreateHook(uint16_t Index, void** Original, void* Function) {
	//assert(_index >= 0 && _original != NULL && _function != NULL);
	void* target = (void*)MethodsTable[Index];
	if (MH_CreateHook(target, Function, Original) != MH_OK || MH_EnableHook(target) != MH_OK) {
		return false;
	}
	return true;
}

void DisableHook(uint16_t Index) {
	assert(Index >= 0);
    MH_DisableHook((void*)MethodsTable[Index]);
}

void DisableAll() {
	MH_DisableHook(MH_ALL_HOOKS);
	free(MethodsTable);
	MethodsTable = NULL;
}

#if defined _M_X64
uint64_t* getMethodsTable()
{
	return MethodsTable;
}
#elif defined _M_IX86
uint32_t* getMethodsTable()
{
	return MethodsTable;
}
#endif
