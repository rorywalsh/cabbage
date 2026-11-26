# Cabbage backend tests

Catch2 suite driving the **real** `Engine` and `CabbageAudioApp` (not mocks) against
embedded `.csd` fixtures. Each `TEST_CASE` is registered as its own ctest test
(`catch_discover_tests`), so a hung case is reported and timed out individually
instead of stalling the whole binary.

> **Maintenance rule:** when you add, remove, rename or materially change the scope of a
> `TEST_CASE`, update this README in the same PR. Every test case must be listed here.

## Suites at a glance

| File | Cases | Tag | What it needs |
|---|---|---|---|
| `CabbageAppTests.cpp` | 16 | `[CabbageApp]`, `[CabbageApp][TrigStructs]`, `[CabbageAudioApp::AudioConfig]`, `[CabbageAudioApp]` | Csound + a vscabbage checkout (widget sources) |
| `JsoncParsingTests.cpp` | 7 | `[JSONC]` | Csound; one case also needs vscabbage |
| `TestCsdFiles.h` | — | — | Embedded `.csd` fixtures (no files needed at runtime) |
| `FileBrowseStub.cpp` | — | — | Stubs the native file dialog |

`CabbageAppTests.cpp` also defines the shared `ensureValidSettingsFileExists()` helper and
the `requireWidgetsResolved()` guard.

## How the tests run

- **`catch_discover_tests`** registers every `TEST_CASE` as a separate ctest test, with
  `EXTRA_ARGS "--reporter compact"`. A hung case hits `ctest --timeout` without blocking
  the rest of the suite.
- **`TEST_VSCABBAGE_DIR`** is a compile definition (`-DTEST_VSCABBAGE_DIR=<path>/src`).
  At runtime `ensureValidSettingsFileExists()` writes it into the Cabbage `settings.json`
  as `jsSourceDir`, so widget descriptors resolve. Without it, the helper warns and skips
  bootstrapping.
- **`requireWidgetsResolved()`** fails cleanly when a content test compiles a CSD but
  resolves zero widgets — that is always a broken test setup (e.g. wrong
  `TEST_VSCABBAGE_DIR`), never a valid result. It prevents a confusing segfault deep in
  `process()`.
- **`FileBrowseStub.cpp`** replaces `cabbage::File::browseForFile()` with a stub returning
  `/test/dummy/path.wav`, so no native dialog is opened in CI.

## Building and running

A `vscabbage` checkout is **required** for widget fixtures; without it the tests run
against the backend placeholder and fail (cleanly) on missing widgets.

```bash
cmake -S . -B build-tests \
  -DCABBAGE_BUILD_TARGET=CabbageTests \
  -DTEST_VSCABBAGE_DIR=<path-to-vscabbage>/src
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure --timeout 180
```

Platform-specific CMake generators/architectures are used in CI, e.g. `-GXcode` on macOS
and `-A ARM64` on Windows ARM.

## CI

`.github/workflows/cabbage3.yaml` runs the suite on macOS, Linux, Windows x64 and
Windows ARM:

1. Check out `cabbage` and a `vscabbage` clone (for widget sources).
2. Configure `CABBAGE_BUILD_TARGET=CabbageTests` with `TEST_VSCABBAGE_DIR`; the configure
   step fails fast if `<dir>/cabbage/widgets` is missing.
3. Build, then `ctest --output-on-failure --timeout 180` (15-minute job timeout).
4. On failure, rerun the binary under `lldb` / `gdb` / `cdb` for a symbolicated backtrace.

---

## `CabbageAppTests.cpp` — 16 cases

### 1. `Basic CabbageApp construction and functionality`
Builds an app with `--file test.csd`. Sections cover: audio-device scanning does not throw;
`setCsoundFile`; `AudioConfig::loadFromJson` accepts a valid file and rejects a missing one;
the message queue accepts/counts commands; `onIdle()` does not throw.

### 2. `Audio device scanning`
Standalone `scanAudioDevices()` must not throw. Present-but-unusable devices (the kind CI
runners expose) must degrade safely rather than crash.

### 3. `Test stdin/stdout pipe communication`
Loads the `rotarySliders` fixture, initialises Cabbage and the stdio connection, then runs
`process()`/`onIdle()` for up to 5 seconds. Verifies the `harmonic1..harmonic8` control
channels read back sanely and that audio iterations complete.

### 4. `Test cabbageSet.csd with JSON message capture`
Loads the `cabbageSet` fixture and overrides `hostCallback` to capture emitted opcode
messages. Runs audio for ~3 seconds, asserts iterations completed, and logs the captured
channel/type/JSON payloads.

### 5. `Test cabbageJson opcodes`
Loads `cabbageJson` and asserts the classic JSON opcodes: string getters (present, missing,
numeric-as-string, array elements), type queries (`number`, `array`, `null`, `missing`),
builder round-trips through the getters (`99.0`, `hi`, `880.0`), graceful handling of
invalid input, numeric get/has/len/empty cases, and that a trigger variant fires.

### 6. `Test cabbageJson functional syntax`
Loads `cabbageJsonFunc` and asserts the typed-variable functional call style (`res:CabbageNumTrig …`)
independently of the classic syntax: get, len, type, has, freq/new-freq, id, trigger-seen.

### 7. `Test cabbageGetWidgetIds and cabbageGetWidget opcodes`
Loads `widgetIdsAndWidget` and asserts widget enumeration and querying: all ids flattened
(`MainForm`, `widA`–`widC`, `grp`, nested `childKnob`), exact and glob type filters,
channel-id glob, nested dot-notation colour filter, single-widget JSON containing id/type,
and an unknown id returning an empty string. Also covers the deprecated `cabbageGetWidgets`
alias.

### 8. `Struct cabbageGetValue forms follow shared trigger semantics` (`[TrigStructs]`)
Loads `trigStructs` and runs 1500 k-cycles. The `CabbageNumTrig`/`CabbageStrTrig` struct
overloads are the sole readers (the trigger memo is process-wide), checked against
independently computed expectations via non-zero error counters; also asserts the struct
triggers actually fire.

### 9. `Struct trigger overloads for cabbageGet and cabbageJsonGet` (`[TrigStructs]`)
Loads `trigStructsSolo` and runs 1800 k-cycles. The property struct fires on first sight
and once when the text becomes `"beta"`; the JSON struct fires as the document alternates
and its value always tracks the document (zero mismatches).

### 10. `Stress test start/stop/destroy`
Loops `StopAudio`/`KillProcessor`/`InitCabbage` for ~5 seconds and asserts the app is still
functional. **Skipped on macOS** via `SUCCEED` + `return` (not Catch2 `SKIP`) because the
CI runner intermittently fails to create a CoreMIDI client, and per-case ctest treats an
isolated SKIP as a failure.

### 11. `CabbageAudioApp AudioConfig functionality` (`[CabbageAudioApp::AudioConfig]`)
Asserts `AudioConfig` defaults (driver, channels, buffer size, sample rate, MIDI, JS source
dir) and `loadFromJson`: false for a missing file, true for a valid settings file.

### 12. `CabbageAudioApp command line parsing` (`[CabbageAudioApp]`)
Constructs the app with `--file` and with `--portNumber`, asserting both argument forms
parse successfully.

### 13. `File resave - InitCabbage on live session does not crash`
Loads `basicOscillator`, then sends three bare `InitCabbage` commands to an already-live
session (the Ctrl+S resave path). Asserts no throw/heap corruption, the processor remains
valid and the CSD still compiles, then tears down cleanly. Reproduces the Windows
`STATUS_HEAP_CORRUPTION` crash.

### 14. `stdin onFileChanged is handled without crashing`
Exercises the exact stdin payload the extension sends on save:
1. Valid file → exactly one `InitCabbage` queued.
2. Windows-style path (drive letter, backslashes, spaces) → parsed without throwing.
3. Double notification → coalesced to a single `InitCabbage`.
4. Missing file → declined quietly, nothing queued.
5. Malformed/hostile JSON (bad text, empty, missing/typed file name, unknown command) →
   caught without throwing and without queueing.

### 15. `cabbageSetValue/cabbageGetValue value round-trip`
Loads `valueRoundTrip` (nine numeric widget types). `instr 1` writes a distinct in-range
value to each channel with `cabbageSetValue` at init; `instr 2` reads them back with
`cabbageGetValue` at both i-time and k-time and publishes per-rate mismatch counts. Both
`res_iErr` and `res_kErr` must stay zero after ≥600 k-cycles.

### 16. `cabbageSet/cabbageGet property round-trip`
Loads `propertyRoundTrip`. `instr 1` sets `bounds.left` (numeric) and `label.text` (string)
properties with `cabbageSet` at init; `instr 2` reads them back with `cabbageGet` at i-time
(numeric + string) and k-time (numeric), publishing per-rate mismatch counts that must stay
zero. String property reads are i-time (the plain `S` form); k-time string reads use the
`Sk`/struct trigger forms, covered separately by `trigStructsSolo`.

---

## `JsoncParsingTests.cpp` — 7 cases

JSON-with-comments support across three parsing paths: widget descriptors read from JS,
the `<Cabbage>` section of a CSD, and widget routing for CSD loading.

### 1. `extractPropsFromJS accepts comments inside the props object`
Parses a widget descriptor containing line and block comments between properties and
inside nested `channels`/`range`.

### 2. `extractPropsFromJS ignores braces in comments and strings`
The brace-counting extractor must ignore `{`/`}` inside comments and inside string values.

### 3. `extractPropsFromJS still rejects malformed props`
An unbalanced/malformed `this.props` object returns JSON `null` rather than a partial parse.

### 4. `parseCabbageSection accepts comments in the CSD JSON`
A temp CSD with comments in `widgets` and `channelConfig` parses; asserts form size and
named channel configuration.

### 5. `parseCsdForWidgets loads inline JSONC with a commented form`
Comments between a key, colon and value parse correctly; asserts both the form and the
rotary slider, including the slider channel.

### 6. `parseCsdForWidgets still follows an external include`
A `<Cabbage>` section containing only `#include "widgets.json"` resolves the external file
and loads its form widget.

### 7. `the shipped CustomWidgetTemplate descriptor parses`
When `TEST_VSCABBAGE_DIR` is defined, parses the real
`cabbage/widgets/CustomWidgetTemplate.js`; if the file is unavailable, records an explicit
`SUCCEED` (skip). Asserts `type == "customWidget"` and a channel array.

---

## Fixtures (`TestCsdFiles.h`)

| Fixture | Used by |
|---|---|
| `basicOscillator` | stress, resave, stdin `onFileChanged` |
| `rotarySliders` | stdin/stdout pipe communication |
| `cabbageSet` | `cabbageSet` JSON message capture |
| `cabbageJson` | classic JSON opcodes |
| `cabbageJsonFunc` | functional-syntax JSON opcodes |
| `widgetIdsAndWidget` | `cabbageGetWidgetIds`/`cabbageGetWidget` |
| `trigStructs` | struct `cabbageGetValue` triggers |
| `trigStructsSolo` | struct `cabbageGet`/`cabbageJsonGet` triggers |
| `valueRoundTrip` | `cabbageSetValue`/`cabbageGetValue` round-trip |
| `propertyRoundTrip` | `cabbageSet`/`cabbageGet` property round-trip |

## Adding a test

1. Add any new `.csd` fixture to `TestCsdFiles.h` inside `namespace TestCsdFiles`
   (raw string).
2. Add a `TEST_CASE` in `CabbageAppTests.cpp` or `JsoncParsingTests.cpp` following the
   existing arrange → initialise → process/assert pattern.
3. No registration is needed — `catch_discover_tests` picks it up.
4. If the test needs a native dialog, extend `FileBrowseStub.cpp`, not the production
   `CabbageUtils_*` files.
5. **Add the case to this README.** Keep test names ASCII-only: ctest re-invokes the
   binary with the case name, and non-UTF8 Windows console encodings mangle non-ASCII
   names so no cases match.

## Known caveats

- A `vscabbage` checkout is required for widget fixtures; without it, content tests fail
  cleanly via `requireWidgetsResolved()` rather than segfaulting.
- The macOS MIDI stress case is intentionally skipped with `SUCCEED`, not `SKIP`.
- Each case is its own ctest test; use `ctest --timeout` so a hung case cannot stall the
  suite.
- The suite links the real Csound, so Csound 7 must be installed and discoverable.

## Currently not covered

- Plugin (VST3/CLAP/AU) hosting and ARA behaviour.
- Export/encryption (Pro) paths.
- Audio-thread performance/dropout checks.
- Visual/webview behaviour — covered by the `vscabbage` unit + integration suites.
