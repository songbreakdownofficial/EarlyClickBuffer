# Early Click Buffer (GD 2.2081, Windows)

A **source-code prototype** for a personal Geode mod, not a compiled `.geode` package.

## Behavior
- Cube mode only.
- An early press while airborne is remembered for 1–16 player update calls.
- If you land during the window, a fresh jump press is simulated.
- Normal inputs still go through unchanged.
- Settings: Enable Jump Buffer and Buffer Frames (default 8).

## Build
1. Install the Geode CLI, a Geode SDK supporting GD 2.2081, CMake, and the Windows C++ build tools.
2. Set the `GEODE_SDK` environment variable to your SDK folder.
3. **Match `mod.json`'s `geode` version to your installed SDK's major/minor version** (currently a placeholder v5.0.0).
4. From this folder, run `geode build` (or configure/build using CMake).
5. Copy the resulting `.geode` package to your Geometry Dash `geode/mods` folder.

## Important limitations
- **Not compiled or tested.** Geode bindings may use different names for `PlayerButton::Jump`, `m_isCube`, or `m_isOnGround`; adapt those to your installed SDK if the compiler reports errors.
- The landing check occurs during `PlayerObject::update`; game physics ordering may make the replay occur one update late.
- This prototype does not handle dual mode, all input edge cases, or resetting on death. It should be tested on a local copy of a level.
- The count measures player update calls, not necessarily fixed 240-Hz ticks or rendered frames.

## GitHub automatic build
Upload the *contents of this folder* to the root of a GitHub repository (including `.github/workflows/build.yml`). Open Actions -> Build Geode mod (Windows) -> Run workflow. Check workflow logs and download any generated artifact.

**Prototype warning:** The C++ hooks and Geode SDK compatibility have not been compiled or verified. A failing build may require API fixes. The intended behavior is a cube-only jump buffer, not a guarantee that it works in-game.
