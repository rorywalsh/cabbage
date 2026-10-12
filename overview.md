# Cabbage3 — Repository Overview

This repo holds the **Cabbage 3 backend**: the audio engine, the Csound opcode
extensions, and every native binary the VS Code frontend drives. The editor UI
itself lives in the [`vscabbage`](https://github.com/rorywalsh/vscabbage)
repo (the `vscabbage` VS Code extension). The engine core is
transport-agnostic — `cabbage::Engine::processWebViewCommand()` handles
incoming UI messages identically — and it reaches a frontend through one of
two transports depending on the host:

## The two halves

| | This repo (`cabbage`) | `vscabbage` repo (frontend) |
|---|---|---|
| Language | C++20 (`src/`), CMake | TypeScript extension + JS widgets (`src/`, `src/cabbage/widgets`) |
| Produces | `CabbageApp` binary, plugin binaries (`.vst3`, `.clap`, `.component`), Csound opcodes | VS Code extension (`.vsix`), instrument webview UI |
| Role at runtime | Compiles/runs `.csd` files, renders audio, owns widget state | Spawns `CabbageApp`, sends commands, renders widgets from backend JSON |

> Transports: (1) **Dev flow** — newline-delimited JSON over the
> `CabbageApp` process's stdin (frontend → backend) / stdout
> (backend → frontend). `CabbageAudioApp` runs a dedicated stdin-reader thread and
> `commands.ts → sendMessageToCabbageApp()` is the frontend's only send path.
> (2) **Plugin flow** (VST3/CLAP/AUv2 in a DAW) — no pipes: the plugin hosts
> its UI in an embedded webview (lattice/choc bridge,
> `postMessage` JS function name) and messages travel in-process via
> `CabbageProcessor::onMessageFromWebView()` /
> `sendWebViewMessage()`. Same JSON, same central handler.

## How the pieces fit (runtime)

```
.csd file  (Csound code + <Cabbage> JSON widget section)
    │
    ├─ Dev flow ──▶ vscabbage extension ──spawn──▶ CabbageApp ──embeds──▶ Csound 7
    │                     │    ▲                         │  ▲
    │                     │    │ JSON lines              │  │ cabbage opcodes
    │                     │    │ over stdin/stdout       │  │ (Set/Get/Json/State/…)
    │                     ▼    │                         ▼  │
    │              instrument webview ◀──widget JSON─────┘  └─▶ audio/MIDI (RtAudio/RtMidi)
    │              (VS Code panel,               + VU/meter data
    │               JS widgets)                   (.wav record via CabbageAudioRecorder)
    │
    └─ Plugin flow ──▶ DAW loads .vst3/.clap/.component (own Csound inside)
                              │  ▲
                              │  │ same cabbage opcodes
                    embedded webview ◀──▶ CabbageProcessor
                    (in-process,            │  onMessageFromWebView /
                     choc/lattice bridge)  │  sendWebViewMessage()
                                           ▼
                                     host audio/MIDI
```

1. **Authoring** — instruments are `.csd` files whose `<Cabbage>` section holds
   widget definitions as JSON. The frontend edits/formats this section;
   `CabbageParser` (`src/CabbageParser.*`) parses it (plus `channelConfig`)
   into the backend's widget model.
2. **Launch** — the extension spawns the per-OS `CabbageApp` binary shipped
   inside the `.vsix` (`src/CabbageBinaries/`, packed by CI) and tells it which
   `.csd` to load over stdin.
3. **Engine** — `cabbage::Engine` (`src/Cabbage.*`) owns the Csound instance,
   the `std::vector<nlohmann::json>` widget store, parameter channels, preset
   save/load, and `processWebViewCommand()` for incoming JSON.
4. **Csound bridge** — `src/opcodes/` (`cabbageSet`/`cabbageGet`,
   JSON/state/file/profiler/ARA opcodes) lets running Csound code read and push
   widget state; results flow back through `CabbageProcessor::hostCallback` —
   to `CabbageAudioApp::hostCallback` → `sendJsonMessage()` on stdout in the
   dev flow, or to `sendWebViewMessage()` into the embedded webview in plugins
   (the code comments this split explicitly: the Engine owns no transport).
5. **UI updates** — dev flow: the frontend parses stdout JSON and re-renders
   the instrument webview (JS widgets under `vscabbage/src/cabbage/`),
   including ~20 Hz VU-meter data. Plugin flow: the same widget JSON goes
   straight to the embedded webview in-process; the DAW never sees a pipe.

## Build targets (`CMakeLists.txt`, `CABBAGE_BUILD_TARGET`)

Each target configures (and optionally builds) in its own `build/<Target>/`
directory; `BUILD_ALL_TARGETS=True` loops over all of them.

| Target | Output | Notes |
|---|---|---|
| `CabbageApp` | standalone service binary | Links RtAudio/RtMidi; what the extension spawns |
| `CabbagePluginEffect` / `CabbagePluginSynth` | VST3 + CLAP | Via lattice "clap-first" (`make_clapfirst_plugins`) |
| `…EffectARA` / `…SynthARA` | ARA-enabled VST3 | `LATTICE_ENABLE_ARA=ON`, ARA SDK sources merged in |
| `…EffectAUv2` / `…SynthAUv2` / `…MidiFxAUv2` | AUv2 components | macOS only (`aumi`/`aumf`/`aufx` types) |
| `CabbageTests` | Catch2 `CabbageTests` binary | Each `TEST_CASE` registered individually with ctest |

Key core files: `CabbageProcessor.*` (lattice-based audio/MIDI processor,
+ `CabbageARAProcessor.*` for ARA), `CabbageParser.*` (`.csd`/JSON parsing),
`CabbageUtils*` (per-OS helpers: `_mac.mm` / `_win.cpp` / `_linux.cpp`),
`CabbagePluginInfo.h` (product metadata), `CabbageAudioApp/` (service binary:
`main.cpp` arg parsing, audio/MIDI device setup, stdin loop, recorder),
`LinuxWebviewProcess/` (Linux webview helper process).

## Dependencies (`cmake/Dependencies.cmake`, FetchContent)

* [`lattice`](https://github.com/rorywalsh/lattice) (`main`) — plugin
  framework (CLAP/VST3/AUv2 abstraction, webview support); its own
  `cmake/Dependencies.cmake` is included after ours.
* `concurrentqueue` (`master`) — lock-free queues for audio/UI messaging.
* `Catch2` (`v3.5.2`) — tests only.
* `RtAudio` (`6.0.1`) + `RtMidi` (`master`) — `CabbageApp`/`CabbageTests` only.
* **Csound 7** — not fetched: found via `cmake/FindCSOUND.cmake` (CI installs
  `CSOUND_TAG`, currently `7.0.0-beta.18`, per OS before configuring).
* `cmake/patches/` + `ChocPatches.cmake` / `ClapWrapperPatches.cmake` —
  patches applied to fetched deps after download.

## Tests (`tests/`, CI)

* `tests/CabbageAppTests.cpp` (+ `TestCsdFiles.h`, `FileBrowseStub.cpp`) —
  Catch2 suite driving the real `Engine`/`CabbageAudioApp` against embedded
  `.csd` fixtures, including stdin-driven (`onFileChanged`) resave paths.
* Tests bootstrap shared fixtures from a `vscabbage` checkout via
  `-DTEST_VSCABBAGE_DIR=<path-to-vscabbage/src>` (CI clones it next door).
* `.github/workflows/cabbage3.yaml` (runs on pushes to `main`/`develop`):
  per-OS job installs Csound → configures `CabbageTests` → builds → `ctest`
  (with debugger backtrace reruns: `lldb`/`gdb`/`cdb` on failure) → Release
  build of all targets → zips `CabbageBundle-<os>.zip` → uploads artifacts.
  No release/tag step — output is test results plus downloadable bundles,
  which the `vscabbage` packaging workflow later embeds.

## Branches (repo layout)

* `develop` (default) — active development.
* `main` — release line, updated at release points.
* `archive/cabbage2/*` — frozen Cabbage 2 (`main` = old mainline, plus old
  feature branches); final v2 state also tagged `v2-master-final` /
  `v2-develop-final`.

## Build & test commands

Prerequisite: Csound 7 installed where `cmake/FindCSOUND.cmake` can find it
(CI installs `env.CSOUND_TAG` per OS before configuring).

* Single target (quickest iteration):
  `cmake -S . -B build -DCABBAGE_BUILD_TARGET=CabbageApp`
  (any target name from the table above; add `-DCMAKE_BUILD_TYPE=Release`.)
* All targets: each configures into its own `build/<Target>/` dir:
  `cmake -S . -B build -DBUILD_ALL_TARGETS=True -DCMAKE_BUILD_TYPE=Release`
* macOS: the Xcode generator is mandatory
  (`-G Xcode`, enforced by `FATAL_ERROR` otherwise); deployment target
  defaults to 10.15.
* Tests (needs a `vscabbage` checkout for widget fixtures):
  `cmake -S . -B build-tests -DCABBAGE_BUILD_TARGET=CabbageTests
  -DTEST_VSCABBAGE_DIR=<path-to-vscabbage>/src`
  then build and `ctest --test-dir build-tests --output-on-failure --timeout 180`.
  Without `TEST_VSCABBAGE_DIR` tests run against a backend placeholder and
  segfault on missing widgets — the CI configure step fails fast on this.

## Message protocol (JSON, `command`-dispatched)

Every UI→backend message has a `command` string, handled centrally by
`Engine::processWebViewCommand()` (`src/Cabbage.cpp:2126`). Core vocabulary:

* `cabbageIsReadyToLoad` — sent by the UI when booted; NOT handled by the
  Engine (returns false) — the processor uses it to flush queued messages
  (notably table data) that arrived before the UI was connected.
* `widgetStateUpdate` — full/partial widget state via `updateWidgetState()`.
* `controlData` — `{command, channel, value, gesture}` for UI gestures; routed
  by `parameterIndex`: automatable channels become parameter changes (with
  begin/value/end gestures for DAW automation), others recurse as
  `channelData` (`{channel, stringData|floatData}`).
* Lifecycle commands from the extension, e.g. `{command: "stopAudio"}` and
  file-change notifications (`sendMessageToCabbageApp()`, `commands.ts`).

Backend→UI messages are widget JSON (same schema as the `<Cabbage>` section)
plus meter data; transport is `sendJsonMessage()` (stdout, dev flow) or
`sendWebViewMessage()` (embedded webview, plugins).

## Where to change what

* New Csound opcode → new `Cabbage*Opcode.{h,cpp}` pair in `src/opcodes/`,
  registered in `CABBAGE_OPCODE_SOURCES` (`CMakeLists.txt`). Reuse the
  `CabbageOpcodes<>` helpers (`getSafeString`, dot-notation JSON); never hold
  per-instance `std::string` state (see Conventions below).
* New UI→backend command → add a branch in `processWebViewCommand()`, plus the
  sender: stdin dispatch (`CabbageAudioApp.cpp`, ~line 287) for dev flow and/or
  `CabbageProcessor::onMessageFromWebView()` for plugins.
* New widget type → backend (`Engine` store + `Parser`) AND frontend
  (`vscabbage/src/cabbage/widgets/`); both sides must understand the JSON.
* New build target → `CABBAGE_BUILD_TARGETS` list, a `project()` block, and a
  CI job in `cabbage3.yaml`.
* Csound version bump → `env.CSOUND_TAG` at the top of `cabbage3.yaml`.

## Conventions & gotchas

* C++20, `nlohmann::json` is the lingua franca (widget store, messages, `.csd`
  sections). Standard headers must precede Csound includes (`#undef _CR`
  workaround, `CabbageOpcodes.h`).
* Csound allocates opcode instances as raw memory — C++ constructors never
  run. Keep only trivial state per instance (process-wide memos like
  `TriggerMemo`, content-keyed, are the established pattern).
* `CABBAGE_PRO_*_PLACEHOLDER` markers in `CMakeLists.txt` are injection points
  for the closed Pro variant — do not remove or rename them.
* `lattice` and `concurrentqueue` track moving pins (`main`/`master`); a fresh
  configure can pull new upstream code. Everything else is version-pinned.
* AUv2 targets are macOS-only; on Linux the `WebViewProcess` target must build
  first (ordered explicitly via `add_dependencies`).
