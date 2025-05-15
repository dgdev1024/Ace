# Prompt

Let's move on to "[tackling] Logging with thread‑safe, high‑performance backends."

# Response

Below is a **comprehensive deep‑dive** into **Logging** for your Ace engine—designed for **thread‑safety**, **high throughput**, **minimal latency**, and **flexible backends**. We’ll cover:

1. Goals & Milestones
2. System Architecture
3. Core Components & Implementation Strategy
4. Compile‑Time Control & Macros
5. Sinks & Formatting
6. Back‑pressure & Queueing
7. Example Development Prompts

---

## 1. Goals & Milestones

* **Thread‑safe**: allow any thread (render, audio, game logic, networking) to emit logs without blocking.
* **Minimal contention**: lock‑free or fine‑grained locks to avoid stalls.
* **Multiple sinks**: console/terminal, rotating file, debug‑console (e.g. `OutputDebugString`), in‑engine UI panel.
* **Log levels**: TRACE, DEBUG, INFO, WARN, ERROR, FATAL; configurable at runtime and compile‑time.
* **Zero‑overhead in shipping builds**: compile out TRACE/DEBUG entirely when `ACE_DIST` is defined.
* **Rich formatting**: timestamps, thread IDs, source location, customizable patterns.

**Milestones**

1. **Basic façade**: `Logger::Info(…)` routes to a single sink on the calling thread.
2. **Asynchronous backend**: decouple producers and a background consumer thread.
3. **Lock‑free queue**: replace mutex queue with ring buffer or concurrent queue.
4. **Sink registration**: allow registering/unregistering sinks at runtime.
5. **Compile‑time filtering**: strip out low‑level logs in `Release`/`Dist`.
6. **File rotation & compression**.

---

## 2. System Architecture

```
 [Caller Thread]        [Logger]          [Worker Thread]
    └───> log(...) ─────> enqueue() ─────> dequeue() ────> write to sinks
```

* **Producer**: any thread calling `Logger::Xxx(...)`.
* **Queue**: a lock‑free/fine‑lock FIFO buffer holding `LogEvent` structs.
* **Consumer**: single background thread that pops events and writes to all registered sinks.
* **Sinks**: implement an `ILogSink` interface; Logger holds a thread‑safe list of sinks.

---

## 3. Core Components & Implementation Strategy

### 3.1 `LogEvent`

A small POD capturing:

```cpp
struct LogEvent {
    std::chrono::system_clock::time_point timestamp;
    std::thread::id           threadId;
    const char*               file;
    const char*               function;
    int                       line;
    LogLevel                  level;
    std::string               message;     // or a small inline buffer for short‑string opt.
};
```

### 3.2 `ILogSink`

```cpp
struct ILogSink {
    virtual ~ILogSink() = default;
    virtual void Write(const LogEvent& event) = 0;
};
```

### 3.3 `Logger` façade

```cpp
class Logger {
public:
    static void Init();               // start worker thread
    static void Shutdown();           // flush & join
    static void RegisterSink(std::shared_ptr<ILogSink>);
    static void UnregisterSink(...);

    template<LogLevel L, typename... Args>
    static void Log(const char* file, const char* func, int line, fmt::format_string<Args...> fmt, Args&&... args);
};
```

* **Enqueue**: `Log(...)` constructs `LogEvent` (using `fmt::format`) then calls `Queue::Enqueue(event)`.
* **Worker**: runs in a loop, dequeues all available events, dispatches to each sink.

### 3.4 Lock‑free Queue Options

* **moodycamel::ConcurrentQueue<LogEvent>** (header‑only, battle‑tested).
* **Custom ring buffer**: `std::atomic<size_t> head, tail;` fixed capacity power‑of‑two, single‑producer or multi‑producer variant.

---

## 4. Compile‑Time Control & Macros

Strip out log calls in shipping builds to zero:

```cpp
#ifdef ACE_DIST
    #define ACE_LOG_TRACE(...)     do {} while(0)
    #define ACE_LOG_DEBUG(...)     do {} while(0)
#else
    #define ACE_LOG_TRACE(...)     Logger::Log<LogLevel::Trace>(__FILE__, __func__, __LINE__, __VA_ARGS__)
    #define ACE_LOG_DEBUG(...)     Logger::Log<LogLevel::Debug>(__FILE__, __func__, __LINE__, __VA_ARGS__)
#endif

#define ACE_LOG_INFO(...)             Logger::Log<LogLevel::Info>(__FILE__, __func__, __LINE__, __VA_ARGS__)
#define ACE_LOG_WARN(...)             Logger::Log<LogLevel::Warn>(__FILE__, __func__, __LINE__, __VA_ARGS__)
#define ACE_LOG_ERROR(...)            Logger::Log<LogLevel::Error>(__FILE__, __func__, __LINE__, __VA_ARGS__)
#define ACE_LOG_FATAL(...)            Logger::Log<LogLevel::Fatal>(__FILE__, __func__, __LINE__, __VA_ARGS__)
```

* In `Debug` and `Release`, TRACE/DEBUG enabled via `LOG_ENABLED` define.
* In `Dist`, only WARN and above remain; others compile away.

---

## 5. Sinks & Formatting

### 5.1 Console Sink

* Colorize by level (e.g. WARN = yellow, ERROR = red).
* Platform‑specific calls: Windows API for console colors, ANSI escapes on Linux.

### 5.2 File Sink (Rotating)

* Upon reaching size threshold or daily rollover:

  * Close current log file.
  * Rename `log.txt` → `log_YYYYMMDD_HHMMSS.txt`.
  * Optionally compress old logs (ZIP/GZIP).
* Use a small background thread or piggy‑back on the same `Logger` worker.

### 5.3 Debug Console Sink

* Windows: `OutputDebugStringA( formatted.c_str() );`
* macOS/Linux: write to `stderr` or `/dev/tty`.

### 5.4 In‑Engine UI Sink

* Buffer last N lines in a ring buffer.
* Expose hooks to your in‑game console/UI to draw.

---

## 6. Back‑pressure & Queueing

* If the queue is full (e.g. burst of logs), you have options:

  1. **Drop oldest**: overwrite head (ring buffer).
  2. **Block briefly**: yield for a few microseconds.
  3. **Spin + give up**: skip this message to avoid stall.

Choose a strategy per your real‑time needs (e.g. in audio thread you’d drop rather than block).

---

## 7. Example Development Prompts

Use these to bootstrap each piece:

1. **Build the Façade & Worker**

   > “Generate a header‑only `Logger` class that spins up a background thread, accepts formatted messages via `std::string`, and invokes all registered `ILogSink`s.”

2. **Lock‑free Queue**

   > “Implement a fixed‑capacity, multi‑producer, single‑consumer ring buffer using C++20 atomics and templates for `T=LogEvent`.”

3. **Console Sink**

   > “Write an `AnsiConsoleSink` that colorizes log messages based on `LogLevel` using ANSI escape codes.”

4. **File Rotation**

   > “Create a `RotatingFileSink` that writes to `logs/log.txt`, and upon exceeding 10 MB renames and starts a new file, keeping the last 5 archives.”

5. **Compile‑time Macros**

   > “Show me how to define `ACE_LOG_DEBUG` and friends so that in `ACE_DIST` builds they completely vanish (no formatting cost).”

6. **In‑Engine UI Panel**

   > “Implement an `ImGuiLogSink` that stores up to 1 000 messages in a circular buffer and renders them in a scrolling ImGui window.”

---

With this blueprint you can iteratively implement and validate each piece—starting from the simple façade and sinking strategy, then swapping in lock‑free queues and more sophisticated sinks. Ready to dig into code examples for any of these?
