// dllmain.cpp
// Proxy bink2w64.dll: forwards all exports to bink2w64_real.dll, then injects the mod menu.
//
// IMPORTANT: #include "pch.h" must come before the #pragma comment(linker,...) lines.
// With /Yu (precompiled header), the compiler discards everything before the PCH include,
// so pragmas placed before it are silently ignored — producing a DLL with no exports.
#include "pch.h"

#pragma comment(linker, "/export:BinkAllocateFrameBuffers=bink2w64_real.BinkAllocateFrameBuffers")
#pragma comment(linker, "/export:BinkClose=bink2w64_real.BinkClose")
#pragma comment(linker, "/export:BinkCloseTrack=bink2w64_real.BinkCloseTrack")
#pragma comment(linker, "/export:BinkControlBackgroundIO=bink2w64_real.BinkControlBackgroundIO")
#pragma comment(linker, "/export:BinkCopyToBuffer=bink2w64_real.BinkCopyToBuffer")
#pragma comment(linker, "/export:BinkCopyToBufferRect=bink2w64_real.BinkCopyToBufferRect")
#pragma comment(linker, "/export:BinkCurrentSubtitle=bink2w64_real.BinkCurrentSubtitle")
#pragma comment(linker, "/export:BinkDoFrame=bink2w64_real.BinkDoFrame")
#pragma comment(linker, "/export:BinkDoFrameAsync=bink2w64_real.BinkDoFrameAsync")
#pragma comment(linker, "/export:BinkDoFrameAsyncMulti=bink2w64_real.BinkDoFrameAsyncMulti")
#pragma comment(linker, "/export:BinkDoFrameAsyncWait=bink2w64_real.BinkDoFrameAsyncWait")
#pragma comment(linker, "/export:BinkDoFramePlane=bink2w64_real.BinkDoFramePlane")
#pragma comment(linker, "/export:BinkFindXAudio2WinDevice=bink2w64_real.BinkFindXAudio2WinDevice")
#pragma comment(linker, "/export:BinkFreeGlobals=bink2w64_real.BinkFreeGlobals")
#pragma comment(linker, "/export:BinkGetError=bink2w64_real.BinkGetError")
#pragma comment(linker, "/export:BinkGetFrameBuffersInfo=bink2w64_real.BinkGetFrameBuffersInfo")
#pragma comment(linker, "/export:BinkGetGPUDataBuffersInfo=bink2w64_real.BinkGetGPUDataBuffersInfo")
#pragma comment(linker, "/export:BinkGetKeyFrame=bink2w64_real.BinkGetKeyFrame")
#pragma comment(linker, "/export:BinkGetPlatformInfo=bink2w64_real.BinkGetPlatformInfo")
#pragma comment(linker, "/export:BinkGetRealtime=bink2w64_real.BinkGetRealtime")
#pragma comment(linker, "/export:BinkGetRects=bink2w64_real.BinkGetRects")
#pragma comment(linker, "/export:BinkGetSubtitleByIndex=bink2w64_real.BinkGetSubtitleByIndex")
#pragma comment(linker, "/export:BinkGetSummary=bink2w64_real.BinkGetSummary")
#pragma comment(linker, "/export:BinkGetTrackData=bink2w64_real.BinkGetTrackData")
#pragma comment(linker, "/export:BinkGetTrackID=bink2w64_real.BinkGetTrackID")
#pragma comment(linker, "/export:BinkGetTrackMaxSize=bink2w64_real.BinkGetTrackMaxSize")
#pragma comment(linker, "/export:BinkGetTrackType=bink2w64_real.BinkGetTrackType")
#pragma comment(linker, "/export:BinkGoto=bink2w64_real.BinkGoto")
#pragma comment(linker, "/export:BinkLoadSubtitles=bink2w64_real.BinkLoadSubtitles")
#pragma comment(linker, "/export:BinkLogoAddress=bink2w64_real.BinkLogoAddress")
#pragma comment(linker, "/export:BinkNextFrame=bink2w64_real.BinkNextFrame")
#pragma comment(linker, "/export:BinkOpen=bink2w64_real.BinkOpen")
#pragma comment(linker, "/export:BinkOpenDirectSound=bink2w64_real.BinkOpenDirectSound")
#pragma comment(linker, "/export:BinkOpenTrack=bink2w64_real.BinkOpenTrack")
#pragma comment(linker, "/export:BinkOpenWaveOut=bink2w64_real.BinkOpenWaveOut")
#pragma comment(linker, "/export:BinkOpenWithOptions=bink2w64_real.BinkOpenWithOptions")
#pragma comment(linker, "/export:BinkOpenXAudio2=bink2w64_real.BinkOpenXAudio2")
#pragma comment(linker, "/export:BinkOpenXAudio27=bink2w64_real.BinkOpenXAudio27")
#pragma comment(linker, "/export:BinkOpenXAudio28=bink2w64_real.BinkOpenXAudio28")
#pragma comment(linker, "/export:BinkOpenXAudio29=bink2w64_real.BinkOpenXAudio29")
#pragma comment(linker, "/export:BinkPause=bink2w64_real.BinkPause")
#pragma comment(linker, "/export:BinkRegisterFrameBuffers=bink2w64_real.BinkRegisterFrameBuffers")
#pragma comment(linker, "/export:BinkRegisterGPUDataBuffers=bink2w64_real.BinkRegisterGPUDataBuffers")
#pragma comment(linker, "/export:BinkRequestStopAsyncThread=bink2w64_real.BinkRequestStopAsyncThread")
#pragma comment(linker, "/export:BinkRequestStopAsyncThreadsMulti=bink2w64_real.BinkRequestStopAsyncThreadsMulti")
#pragma comment(linker, "/export:BinkService=bink2w64_real.BinkService")
#pragma comment(linker, "/export:BinkSetError=bink2w64_real.BinkSetError")
#pragma comment(linker, "/export:BinkSetFileOffset=bink2w64_real.BinkSetFileOffset")
#pragma comment(linker, "/export:BinkSetFrameRate=bink2w64_real.BinkSetFrameRate")
#pragma comment(linker, "/export:BinkSetIO=bink2w64_real.BinkSetIO")
#pragma comment(linker, "/export:BinkSetIOSize=bink2w64_real.BinkSetIOSize")
#pragma comment(linker, "/export:BinkSetMemory=bink2w64_real.BinkSetMemory")
#pragma comment(linker, "/export:BinkSetOSFileCallbacks=bink2w64_real.BinkSetOSFileCallbacks")
#pragma comment(linker, "/export:BinkSetPan=bink2w64_real.BinkSetPan")
#pragma comment(linker, "/export:BinkSetSimulate=bink2w64_real.BinkSetSimulate")
#pragma comment(linker, "/export:BinkSetSoundOnOff=bink2w64_real.BinkSetSoundOnOff")
#pragma comment(linker, "/export:BinkSetSoundSystem=bink2w64_real.BinkSetSoundSystem")
#pragma comment(linker, "/export:BinkSetSoundSystem2=bink2w64_real.BinkSetSoundSystem2")
#pragma comment(linker, "/export:BinkSetSoundTrack=bink2w64_real.BinkSetSoundTrack")
#pragma comment(linker, "/export:BinkSetSpeakerVolumes=bink2w64_real.BinkSetSpeakerVolumes")
#pragma comment(linker, "/export:BinkSetVideoOnOff=bink2w64_real.BinkSetVideoOnOff")
#pragma comment(linker, "/export:BinkSetVolume=bink2w64_real.BinkSetVolume")
#pragma comment(linker, "/export:BinkSetWillLoop=bink2w64_real.BinkSetWillLoop")
#pragma comment(linker, "/export:BinkShouldSkip=bink2w64_real.BinkShouldSkip")
#pragma comment(linker, "/export:BinkStartAsyncThread=bink2w64_real.BinkStartAsyncThread")
#pragma comment(linker, "/export:BinkUtilCPUs=bink2w64_real.BinkUtilCPUs")
#pragma comment(linker, "/export:BinkUtilFree=bink2w64_real.BinkUtilFree")
#pragma comment(linker, "/export:BinkUtilMalloc=bink2w64_real.BinkUtilMalloc")
#pragma comment(linker, "/export:BinkUtilMutexCreate=bink2w64_real.BinkUtilMutexCreate")
#pragma comment(linker, "/export:BinkUtilMutexDestroy=bink2w64_real.BinkUtilMutexDestroy")
#pragma comment(linker, "/export:BinkUtilMutexLock=bink2w64_real.BinkUtilMutexLock")
#pragma comment(linker, "/export:BinkUtilMutexLockTimeOut=bink2w64_real.BinkUtilMutexLockTimeOut")
#pragma comment(linker, "/export:BinkUtilMutexUnlock=bink2w64_real.BinkUtilMutexUnlock")
#pragma comment(linker, "/export:BinkUtilSoundGlobalLock=bink2w64_real.BinkUtilSoundGlobalLock")
#pragma comment(linker, "/export:BinkUtilSoundGlobalUnlock=bink2w64_real.BinkUtilSoundGlobalUnlock")
#pragma comment(linker, "/export:BinkWait=bink2w64_real.BinkWait")
#pragma comment(linker, "/export:BinkWaitStopAsyncThread=bink2w64_real.BinkWaitStopAsyncThread")
#pragma comment(linker, "/export:BinkWaitStopAsyncThreadsMulti=bink2w64_real.BinkWaitStopAsyncThreadsMulti")
#pragma comment(linker, "/export:RADTimerRead=bink2w64_real.RADTimerRead")

#include <cstdio>
#include <cstdarg>
#include <shlobj.h>
#pragma comment(lib, "Shell32.lib")

static HMODULE hm;

static void Log(const char* fmt, ...) {
    // <module_dir>/../../logs/modmenu.log  (game root / logs /)
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(hm, path, MAX_PATH);
    wchar_t* slash = wcsrchr(path, L'\\'); if (slash) *slash = L'\0'; // strip filename -> Bin\Win64
    slash = wcsrchr(path, L'\\');          if (slash) *slash = L'\0'; // strip Win64   -> Bin
    slash = wcsrchr(path, L'\\');          if (slash) *slash = L'\0'; // strip Bin     -> game root
    wcscat_s(path, MAX_PATH, L"\\logs");
    CreateDirectoryW(path, NULL);
    wcscat_s(path, MAX_PATH, L"\\modmenu.log");

    FILE* f = nullptr;
    _wfopen_s(&f, path, L"a");
    if (!f) return;

    wchar_t exePath[MAX_PATH] = {};
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    const wchar_t* exeName = wcsrchr(exePath, L'\\');
    exeName = exeName ? exeName + 1 : exePath;

    SYSTEMTIME st;
    GetLocalTime(&st);
    fprintf(f, "[%02d:%02d:%02d.%03d] [Proxy pid=%lu exe=%ls] ",
            st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
            GetCurrentProcessId(), exeName);
    va_list args;
    va_start(args, fmt);
    vfprintf(f, fmt, args);
    va_end(args);
    fputc('\n', f);
    fclose(f);
}

// Returns true if engine.ini specifies DX12. Defaults to false (DX11) on any read failure.
static bool IsGameDX12() {
    wchar_t docsPath[MAX_PATH] = {};
    if (FAILED(SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, docsPath)))
        return false;

    wchar_t cfgPath[MAX_PATH] = {};
    swprintf_s(cfgPath, MAX_PATH, L"%s\\Anno 1800\\config\\engine.ini", docsPath);

    FILE* f = nullptr;
    _wfopen_s(&f, cfgPath, L"r");
    if (!f) return false;

    char buf[512] = {};
    fread(buf, 1, sizeof(buf) - 1, f);
    fclose(f);
    return strstr(buf, "DX12") != nullptr;
}

DWORD WINAPI Init(LPVOID) {
    Log("Init thread started, waiting for d3d11.dll");
    while (GetModuleHandle(L"d3d11.dll") == NULL) {
        Sleep(1);
    }
    Log("d3d11.dll loaded");

    bool dx12 = IsGameDX12();
    Log("Renderer from engine.ini: %s", dx12 ? "DX12" : "DX11");

    HMODULE loaded;
    if (!dx12) {
        loaded = LoadLibrary(L"Anno1800ModMenuDX11.dll");
        Log("LoadLibrary(Anno1800ModMenuDX11.dll) -> 0x%p%s", loaded, loaded ? "" : " FAILED");
    } else {
        loaded = LoadLibrary(L"Anno1800ModMenuDX12.dll");
        Log("LoadLibrary(Anno1800ModMenuDX12.dll) -> 0x%p%s", loaded, loaded ? "" : " FAILED");
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        hm = hModule;
        Log("--- process attached ---");
        CreateThread(NULL, 0, Init, NULL, 0, NULL);
    }
    return TRUE;
}
