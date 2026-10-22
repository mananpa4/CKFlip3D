// ---------------------------------------------------------------------------
// Deciding which windows belong in the switcher.  The result is the list the
// whole cascade is built from, so anything that slips through here shows up as
// a tile the user did not expect.
//
// Copyright © 2026 Karol Cymerman (CYMERKAROL) — https://github.com/CYMERKAROL/CKFlip3D
// ---------------------------------------------------------------------------
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
#include <string>

struct WindowInfo {
    HWND         hwnd;
    std::wstring title;
    RECT         rect;      // Window rect in screen coordinates
    bool         minimized; // Was minimized at scan time
};

namespace WindowScanner {

/// Enumerate all alt-tab-eligible application windows (including minimized).
/// Excludes our own windows (identified by ownerPid).
std::vector<WindowInfo> Enumerate(DWORD ownerPid);

/// Find the desktop background window (Progman or WorkerW).
HWND FindDesktopWindow();

/// True when the thread behind `hwnd` is not answering messages — the
/// "(Not Responding)" state, or the first seconds of it.
///
/// Ask before anything that SENDS to the window and waits for it:
/// ShowWindow(SW_RESTORE) on a minimized window, PrintWindow and a
/// cross-thread SetWindowPos all block for as long as the window is stuck.
///
/// IsHungAppWindow is free but only turns true after five silent seconds; the
/// WM_NULL probe covers that gap.  Any failure other than a timeout (UIPI, a
/// window that just closed) counts as responsive.  Costs up to
/// `probeTimeoutMs` only for a window that really is silent.
///
/// Only responsiveness is checked, never content: a fullscreen game or a
/// protected window can capture as nothing and still be alive.
bool IsNotResponding(HWND hwnd, UINT probeTimeoutMs);

} // namespace WindowScanner
