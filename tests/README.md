# Tests

Catch2 suite driving the **real** `Engine` and `CabbageAudioApp` (not mocks)
against embedded `.csd` fixtures. Each `TEST_CASE` is registered as its own
ctest test (`catch_discover_tests` in `CMakeLists.txt`), so a hung case is
reported and timed out individually instead of stalling the whole binary.

## Files

| File | Role |
|---|---|
| `CabbageAppTests.cpp` | All test cases (~50 KB, 13 `TEST_CASE`s, mostly `[CabbageApp]` tag) |
| `TestCsdFiles.h` | Embedded `.csd` fixtures (`TestCsdFiles::basicOscillator`, `rotarySliders`, `cabbageSet`, `cabbageJson`, `cabbageJsonFunc`, `trigStructs`, `trigStructsSolo`) |
| `FileBrowseStub.cpp` | Stub for `cabbage::File::browseForFile()` (returns a dummy path — no native file dialog in CI) |

## What is covered

* **App lifecycle** — construction, CLI arg parsing, `AudioConfig` load/save,
  stress start/stop/destroy.
* **Audio/MIDI environment** — device scanning (present-but-unusable devices
  must not crash; cf. CoreMIDI/`std::terminate` guard in `CabbageAudioApp`).
* **stdin/stdout protocol** — pipe communication and `onFileChanged` handling:
  the Ctrl+S / compile-on-save path, malformed payloads (`REQUIRE_NOTHROW`
  on bad JSON), and double-save coalescing into a single `InitCabbage`.
* **Opcodes** — `cabbageSet` with stdout JSON capture, `cabbageJson` (both
  classic and functional syntax), and struct-trigger overloads for
  `cabbageGet`/`cabbageJsonGet` (`[TrigStructs]` tag).
* **Live resave** — `InitCabbage` on a live session must not crash.

## Running

```bash
# Configure (a vscabbage checkout is REQUIRED for widget fixtures —
# without it tests run against a placeholder and segfault on missing widgets)
cmake -S . -B build-tests -DCABBAGE_BUILD_TARGET=CabbageTests \
  -DTEST_VSCABBAGE_DIR=<path-to-vscabbage>/src
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure --timeout 180
```

`TEST_VSCABBAGE_DIR` is baked in as a compile definition; at runtime the suite
writes it into the test `settings.json` (`jsSourceDir`). The CI configure step
fails fast if `<dir>/cabbage/widgets` is missing.

## Adding a test

1. Add the `.csd` fixture to `TestCsdFiles.h` (raw string in
   `namespace TestCsdFiles`).
2. Add a `TEST_CASE` in `CabbageAppTests.cpp` following the existing
   arrange/compile/assert-on-stdout-JSON pattern.
3. No registration needed — `catch_discover_tests` picks it up automatically.
4. If the test needs a native dialog, extend `FileBrowseStub.cpp`, not the
   production `CabbageUtils_*` files.
