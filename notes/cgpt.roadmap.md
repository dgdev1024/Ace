# Prompt

I wish to write a fast and efficient general-purpose game engine, which can be used for the 
development of 2D, 3D and hybrid games for PC and mobile. Can you provide a detailed, exhaustive 
roadmap which I can follow to achieve this? Include multiple detailed prompts which I can use to 
implement each step provided in a fast, efficient and thread-safe manner.

Include the following subjects in your roadmap:

* Project Setup and Scaffolding (using Premake5)
* Logging
* Event Handling and Management
* Asset Management
* Maths
* Windowing and Input
* Graphics and Rendering
* Audio
* Scripting
* Networking
* Editor Development
* Tools
* Platform Abstraction
* Anything else you can think of to include...

Dedicate your subsequent, thorough responses to this prompt to each one of the above subjects, in
order as listed, throwing in "Anything else you can think of to include..." wherever needed/desired.

The engine will be written in C++, using features from C++11 through C++20, and emphasizing use of 
the Standard Template Library (STL).

# Response

Below is a **master roadmap** for building your C++ game‑engine—from initial setup through advanced editor tooling—all optimized for speed, efficiency and thread‑safety. For each major phase you’ll find:

1. **Goals & Milestones**
2. **Core Tasks**
3. **Example Development Prompts** you can “ask yourself” (or ChatGPT) to generate starter code, patterns or tests.

---

## 1. Project Setup & Scaffolding (Premake5)

**Goals & Milestones**

* Establish a reproducible build system for multiple platforms (Windows, Linux, macOS, iOS, Android).
* Define core library targets (EngineCore, Renderer, Audio, Scripting, Tools, Editor) and sample game project.

**Core Tasks**

* Install Premake5 and configure a root `premake5.lua`.
* Define workspace, projects, configurations (Debug/Release), platforms (x86/x64/ARM).
* Add common include dirs, compiler flags (C++17/20), warnings-as-errors, sanitizer options.

**Example Development Prompts**

* “Generate a `premake5.lua` that builds a static `EngineCore` library and a dynamic `GameSample` executable, with separate Debug/Release flags, using C++20 and enabling all warnings.”
* “Show me how to configure Premake5 to produce Android NDK builds alongside desktop targets.”
* “Create CI scripts (GitHub Actions) that run Premake5 and compile both Windows and Linux in Release mode.”

---

## 2. Logging

**Goals & Milestones**

* Thread‑safe, lock‑free or minimal‑lock logging backend.
* Multiple sinks: console, file (rotating), platform‑specific debug outputs.
* Support log levels, formatting, and conditional compile‑out in shipping builds.

**Core Tasks**

* Choose or implement a lock‑free queue (e.g. moodycamel::ConcurrentQueue).
* Design `Logger` façade: `Logger::Info(…)`, `Warn(…)`, `Error(…)`.
* Add compile‑time `LOG_ENABLED` macro and log‑level filtering.
* Implement file rotation based on size or date.

**Example Development Prompts**

* “Write a thread‑safe `Logger` using a lock‑free ring buffer, with back‑pressure fallback.”
* “Demonstrate how to implement daily file rotation and compress old logs.”
* “Show me unit tests that verify concurrent log writes from multiple threads produce ordered, complete messages.”

---

## 3. Event Handling & Management

**Goals & Milestones**

* Decouple systems with an efficient publish/subscribe event bus.
* Minimal dynamic allocations, high throughput for input, GUI, game events.
* Support synchronous and deferred (queued) dispatch.

**Core Tasks**

* Design a templated `EventBus` that maps event types to handler vectors.
* Use static registration or reflection to avoid manual wiring.
* Implement priority-based processing and one-shot vs persistent subscriptions.

**Example Development Prompts**

* “Implement a header-only `EventBus` that uses `std::unordered_map<type_index, vector<Handler>>` and allows safe removal during dispatch.”
* “Optimize event dispatch to avoid heap allocations: show usage of `std::pmr::vector`.”
* “Create stress tests sending 1M events to multiple subscribers in parallel.”

---

## 4. Asset Management

**Goals & Milestones**

* Load and cache textures, meshes, audio clips, scripts.
* Background streaming of large assets (e.g. texture atlas paging).
* Hot‑reloading in editor and debug builds.

**Core Tasks**

* Define an `IAssetLoader` interface; implement PNG, WAV, FBX loaders.
* Create an `AssetRegistry` with weak‑ptr caching.
* Integrate a file‑watcher (e.g. `inotify`/`ReadDirectoryChangesW`) for reloads.

**Example Development Prompts**

* “Write a generic `AssetHandle<T>` that holds a `std::shared_ptr<T>` but invalidates on unload.”
* “Show how to implement background loading threads with a `std::future`‑based queue.”
* “Demonstrate hot‑reload: detecting file change, unloading old asset, notifying dependent systems.”

---

## 5. Maths

**Goals & Milestones**

* High‑performance vector/matrix/quaternion types.
* Expression templates or SIMD‑accelerated paths.
* Support for physics, animation, camera and spatial queries.

**Core Tasks**

* Define `Vec2/Vec3/Vec4`, `Mat3/Mat4`, `Quat` with operator overloads.
* Integrate SIMD via intrinsics or libraries (DirectXMath, glm simd).
* Provide common utilities: lerp, slerp, lookAt, projection matrices.

**Example Development Prompts**

* “Generate a templated `Vec3<T>` with union for `[x,y,z]` and a SIMD specialization for `float`.”
* “Implement quaternion SLERP and test accuracy vs timestep.”
* “Profile matrix‑vector multiply on 10M operations with and without `_mm_` intrinsics.”

---

## 6. Windowing & Input

**Goals & Milestones**

* Cross‑platform window creation and input events.
* Abstract over GLFW/SDL/Win32/Cocoa.
* High‑precision timers and raw input support.

**Core Tasks**

* Wrap GLFW window and context creation behind `IWindow` interface.
* Capture keyboard, mouse, touch, gamepad events.
* Normalize DPI scaling, multi-monitor.

**Example Development Prompts**

* “Write a `GLFWWindow` class implementing `IWindow`, exposing `OnResize`, `OnKey`, `OnMouseMove` callbacks.”
* “Demonstrate raw mouse delta capture on Windows for high‑precision camera control.”
* “Implement gamepad detection and vibration using XInput and SDL as fallbacks.”

---

## 7. Graphics & Rendering

**Goals & Milestones**

* Flexible rendering architecture: immediate + retained modes.
* Abstraction over Vulkan, DirectX12, Metal, OpenGL ES.
* Thread‑friendly command submission, resource barriers.

**Core Tasks**

* Create a `Renderer` interface; backends for Vulkan and D3D12.
* Command buffer pooling per thread, ring allocators for upload buffers.
* Material/Shader system with reflection-based resource binding.

**Example Development Prompts**

* “Generate code to create a Vulkan command pool per thread and allocate secondary command buffers.”
* “Implement a shader hot‑reload pipeline: compile HLSL/GLSL to SPIR‑V at runtime.”
* “Write sample code for clustered forward+ lighting with compute pass.”

---

## 8. Audio

**Goals & Milestones**

* Low‑latency mixing, 3D spatialization.
* Cross‑platform backends: OpenAL, FMOD, or custom WASAPI/ALSA.
* DSP support: reverb, filters.

**Core Tasks**

* Wrap platform audio output in `IAudioDevice`.
* Design `AudioSource`, `AudioListener` and 3D attenuation model.
* Implement streaming for large audio and voice chat buffers.

**Example Development Prompts**

* “Implement a lock‑free multi‑channel audio mixer that mixes N sources into output buffer.”
* “Demonstrate 3D panning and doppler effect formulas in code.”
* “Show how to integrate with third‑party FMOD for advanced DSP.”

---

## 9. Scripting

**Goals & Milestones**

* Expose engine API to a high‑level language (Lua, AngelScript, or Python).
* Hot‑reloadable scripts, safe sandboxing.

**Core Tasks**

* Embed Lua with sol2 or LuaBridge.
* Register engine types (Vec3, Entity, Transform) for script use.
* Create binding code generator via reflection or macros.

**Example Development Prompts**

* “Generate C++ glue code to expose `Entity::Translate(Vec3)` to Lua with sol2.”
* “Implement script-driven event handler so Lua functions receive engine events.”
* “Test hot‑reload by modifying a Lua script and re‑executing without restart.”

---

## 10. Networking

**Goals & Milestones**

* Low‑latency UDP messaging, reliable channel on top.
* Support client/server and peer‑to‑peer topologies.
* State replication, delta‑compression, snapshot interpolation.

**Core Tasks**

* Build a `NetworkManager` using `asio` or ENet.
* Define message framing, reliability layers, encryption (optional).
* Implement entity synchronization with time‑warp/extrapolation.

**Example Development Prompts**

* “Write a lock‑free outgoing packet queue and async send/recv using `boost::asio`.”
* “Demonstrate delta‑compression: serializing only changed fields of a struct.”
* “Generate code for client‑side prediction and server reconciliation.”

---

## 11. Editor Development & Tools

**Goals & Milestones**

* Integrated scene editor, asset browser, property panels.
* Dockable UI, undo/redo, plugin support.

**Core Tasks**

* Use ImGui + docking branch or Qt for UI.
* Implement scene graph view, transform gizmos (translate/rotate/scale).
* Build serialization for scenes, prefabs, and undo stack.

**Example Development Prompts**

* “Create an ImGui-based entity hierarchy panel with drag‑and‑drop reparenting.”
* “Implement a generic `PropertyInspector` that reflects C++ types via metadata.”
* “Show undo/redo system storing diffs of component states.”

---

## 12. Platform Abstraction & Anything Else

**Goals & Milestones**

* Abstract file I/O, threading, timers behind platform layer.
* Mobile‑specific input (touch, accelerometer), battery and permissions APIs.
* Integrate analytics, crash reporting, packaging, patching.

**Core Tasks**

* Create `Platform` interface for `FileSystem`, `Thread`, `Time`, `Process`.
* Add mobile plugins for iOS/Android signing and deployment.
* Hook in services like Google Play Services, Game Center.

**Example Development Prompts**

* “Define a `ThreadPool` wrapper that uses `std::thread` or platform threads under the hood.”
* “Implement `IFileSystem::ReadAllText` with UTF‑8 BOM detection.”
* “Show how to integrate a crash‑reporting callback that captures stack trace on Windows.”

---

### Next Steps

We’ll now deep‑dive **one subject at a time**. Let me know which module you’d like to tackle first (or I’ll start with Project Setup & Scaffolding), and I’ll provide a thorough, step‑by‑step implementation outline plus sample code prompts.
