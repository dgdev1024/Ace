# Prompt

Before we begin, this is the first time we touch on SIMD. Can you provide a detailed explainer and breakdown on SIMD and how it works, in general and as it pertains to a maths library?

# Response

SIMD (Single Instruction, Multiple Data) is a processor feature that lets you perform the *same* operation on *multiple* data elements in parallel, using wide CPU registers. In a math library—where you do tons of small, repetitive vector and matrix operations—leveraging SIMD can yield **4×** or more speedups over naïve scalar code. Here’s a deep-dive:

---

## 1. The Core Idea

* **Scalar code**: each instruction (e.g. add, multiply) works on one data element:

  ```cpp
  float c = a + b;  // one add per instruction
  ```
* **SIMD code**: one instruction works on a small vector of data (e.g. 4 floats at once on a 128-bit register):

  ```cpp
  __m128 c = _mm_add_ps(a, b);  // adds four floats in parallel
  ```

Modern x86 CPUs support several SIMD ISAs:

* **SSE** (Streaming SIMD Extensions): 128-bit registers (`__m128`) → 4 × 32-bit floats
* **AVX**: 256-bit registers (`__m256`) → 8 × floats
* **AVX-512**: 512-bit registers → 16 × floats

ARM (mobile/console) has **NEON** (128-bit) with similar concepts.

---

## 2. Why It Matters in a Math Library

Game-engine math is dominated by small-vector operations:

* **Dot products**, **cross products**, **vector additions**, **matrix multiplies**, etc.
* You often do **millions** of these per frame: transforming every vertex, computing physics, animating joints.

Using SIMD you can:

1. **Pack** four `float` coordinates into one register.
2. **Execute** the same operation (add, mul) on all four lanes with a single instruction.
3. **Save** instruction-dispatch overhead, improve throughput and utilize SIMD units in the CPU.

---

## 3. Intrinsics vs Auto-Vectorization

### Auto-Vectorization

* Let the compiler translate simple loops into SIMD under the hood.
* **Pros**: no code changes.
* **Cons**: unpredictable; may not vectorize complex expressions or across function calls.

### Intrinsics

* You call functions (intrinsics) that map *directly* to SIMD instructions:

  ```cpp
  __m128 va = _mm_load_ps(&a.x);    // load 4 floats
  __m128 vb = _mm_load_ps(&b.x);
  __m128 vc = _mm_add_ps(va, vb);   // parallel add
  _mm_store_ps(&r.x, vc);
  ```
* **Pros**: precise control, maximal performance.
* **Cons**: more verbose, architecture-specific, manual shuffles for complex data layouts.

---

## 4. Data Layout & Alignment

SIMD loads/stores typically require data to be **aligned** to the register width (16 bytes for SSE). In a math library you’ll often:

```cpp
struct alignas(16) Vec4f {
    float x, y, z, w;
};
```

* Ensures `&v.x` is 16-byte aligned so you can use `_mm_load_ps` safely.
* Alternatively, you can use unaligned loads (`_mm_loadu_ps`), but they’re slightly slower.

---

## 5. Example: Vector Addition

**Scalar**:

```cpp
Vec4f add(const Vec4f& a, const Vec4f& b) {
    return { a.x + b.x,
             a.y + b.y,
             a.z + b.z,
             a.w + b.w };
}
```

**SIMD (SSE)**:

```cpp
Vec4f add(const Vec4f& a, const Vec4f& b) {
    __m128 va = _mm_load_ps(&a.x);
    __m128 vb = _mm_load_ps(&b.x);
    __m128 vr = _mm_add_ps(va, vb);
    Vec4f r;
    _mm_store_ps(&r.x, vr);
    return r;
}
```

This single `_mm_add_ps` does four float-adds at once.

---

## 6. Beyond Simple Ops: Matrix × Vector

A 4×4 matrix times a Vec4 can be done with a few shuffles and multiplies:

```cpp
// assume m.rows are __m128 row0..row3 and v is __m128
__m128 x = _mm_shuffle_ps(v, v, 0x00); // replicate v.x in all lanes
__m128 y = _mm_shuffle_ps(v, v, 0x55); // v.y
__m128 z = _mm_shuffle_ps(v, v, 0xAA); // v.z
__m128 w = _mm_shuffle_ps(v, v, 0xFF); // v.w

__m128 r0 = _mm_add_ps(
               _mm_add_ps(_mm_mul_ps(x, m.row0),
                          _mm_mul_ps(y, m.row1)),
               _mm_add_ps(_mm_mul_ps(z, m.row2),
                          _mm_mul_ps(w, m.row3)));
```

---

## 7. When to Use SIMD

* **High-volume hotspots**: vector loops in skinning, vertex transforms, physics simulation.
* **Large data batches**: arrays of Vec4, matrix operations on large meshes.

**Don’t** bite off more than you can chew: start by vectorizing your `Vec4f` and `Mat4f` operations, benchmark, then extend to more.

---

## 8. Abstraction Strategies

1. **Partial Specialization**

   * Provide a generic template `Vec4<T>`, then a `template<> struct Vec4<float>` specialization using intrinsics.

2. **Expression Templates**

   * Capture whole expressions (`a + b + c`) to fuse into fewer loads/stores.

3. **Portable Wrappers**

   * Wrap intrinsics in an internal header (`simd.h`) so that your math code stays clean—e.g. `simd::load`, `simd::add`.

---

## 9. Summary

* **SIMD** leverages wide registers to do multiple operations per instruction.
* Ideal for math libraries doing many small, uniform computations.
* Choose **intrinsics** for control and performance; use **auto-vectorization** as a fallback.
* Align your data, benchmark your hotspots, and abstract your intrinsics behind clean APIs so the rest of your engine stays readable.

With this understanding, you’re ready to implement and benchmark a SIMD-accelerated `Vec3f`/`Vec4f` specialization that will dramatically speed up your engine’s math workloads.
