# kernelx-shim

A minimal `kernelx.dll` stand-in that lets the Xbox One **XG library** (`xg_x.dll`) load and
run on desktop Windows — so tools like [Reclaimer](https://github.com/Gravemind2401/Reclaimer)
can untile Halo 5 Xbox One bitmaps using the real texture-layout code instead of a
reimplementation.

`xg_x.dll` is built for the Xbox One, where the OS kernel is exposed as `kernelx.dll`.
On desktop Windows that DLL does not exist, so the loader fails before any XG function
is reachable.

The imports it needs are almost all ordinary Win32 that `kernel32.dll` already provides.
Only two are Xbox-specific:

| Import | Handling |
|---|---|
| 15 Win32 calls (`EnterCriticalSection`, `GetTickCount`, `Sleep`, …) | Forwarded straight to `kernel32` via the `.def` |
| `XMemAlloc` / `XMemFree` | Implemented in [`kernelx.c`](kernelx.c) over `_aligned_malloc` |

No XG behaviour is reimplemented or patched — the Microsoft
library now works with the kernel calls it expects, but with the real windows' equivalents

## Building

From an **x64 Native Tools Command Prompt for VS 2022**:

```bat
cl /LD /O2 kernelx.c /link /DEF:kernelx.def /OUT:kernelx.dll
```

x64 only — `xg_x.dll` is a 64-bit binary.

## Usage

1. Build `kernelx.dll`, or grab it from [Releases](../../releases).
2. **Supply your own `xg_x.dll`** from an Xbox One XDK/GDK you have access to.
   It is not distributed here — see [Legal](#legal).
3. Rename it to `xg.dll`.
4. Put both next to the tool that loads XG. - Reclaimers case, in the folder with the app's .exe

## Regenerating the .def for a different XG version

The export list here matches `xg.dll` **10.0.16232.1064**. Other XDK/GDK builds may import
a slightly different set, which shows up as a load failure rather than a clear error.
To check your copy:

```bat
dumpbin /imports xg.dll
```

Find the `kernelx.dll` section and compare it against the `EXPORTS` in
[`kernelx.def`](kernelx.def). Anything missing needs a line added — forward it to `kernel32`
if that's where it lives, or stub it in `kernelx.c` if it's Xbox-specific.
[Dependencies](https://github.com/lucasg/Dependencies) shows the same thing with a GUI.

## Legal

This repository contains **only original shim code**. It ships no Microsoft binaries and
no game assets.

`xg_x.dll` is proprietary Microsoft code covered by the Xbox XDK/GDK license and is
deliberately excluded (see `.gitignore`). Please don't open a PR adding it.

The shim performs no decryption and defeats no protection mechanism — it supplies missing
OS imports so an existing library can resolve. Texture untiling is a memory-layout
calculation.

## License

MIT — see [LICENSE](LICENSE).
