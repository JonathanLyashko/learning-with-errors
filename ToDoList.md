# Development Flow for Plain LWE Encryption Implementation

This document outlines a step-by-step development workflow to implement a vanilla Learning With Errors (LWE) encryption scheme in C++ using only math libraries. Each stage includes objectives, recommended tasks, and deliverables.

---

## 1. Project Initialization

**Objectives:**
- Scaffold the repository structure.
- Establish coding standards and tooling.

**Tasks:**
1. Create a GitHub repository and clone locally.
2. Initialize C++ project (e.g., with CMake or a simple Makefile).
3. Set up folder structure:
   ```
   /src
     random.hpp        // PRNG & Gaussian sampler
     lwe_params.hpp    // Parameter definitions
     modint.hpp        // mod‑q integer wrapper
     matrix.hpp        // NTL/Eigen wrappers
     keygen.cpp        // KeyGen implementation
     encrypt.cpp       // Encrypt/Encaps implementation
     decrypt.cpp       // Decrypt/Decaps implementation
   /tests
     toy_tests.cpp     // Manual small‑n tests
     fuzz_tests.cpp    // Randomized decrypt checks
   CMakeLists.txt      // or Makefile
   ```
4. Define coding style (e.g., clang-format) and integrate CI (GitHub Actions).

**Deliverables:**
- Repository scaffold with empty source files and build config.
- CI pipeline that compiles and runs an empty test suite.

---

## 2. Core Math & Utilities

**Objectives:**
- Integrate big‑integer and linear algebra libraries.
- Build foundational utilities.

**Tasks:**
1. Add dependencies:
   - GMP (or Boost.Multiprecision)
   - NTL (or Eigen)
2. Implement `modint.hpp`:
   - A thin C++ class wrapping `uint64_t` or `mpz_t` with fast `+`, `-`, `*`, and `mod q` operations.
3. Implement `matrix.hpp`:
   - Wrappers for `NTL::Mat<ZZ>` and `Vec<ZZ>`, including modulable operations.
4. Develop `random.hpp`:
   - Seed PRNG (`std::random_device` → `std::mt19937_64`).
   - Uniform sampler for `[0, q)`.
   - Discrete Gaussian sampler (CDF‑inversion or rejection).
5. Write unit tests for each utility:
   - Validate modular arithmetic (edge cases, large inputs).
   - Statistical tests for Gaussian sampler (mean, variance checks).

**Deliverables:**
- Fully tested `modint.hpp`, `matrix.hpp`, `random.hpp` modules.
- Test coverage report ≥ 90% for utilities.

---

## 3. LWE Core Algorithms

**Objectives:**
- Implement KeyGen, Encrypt/Encaps, and Decrypt/Decaps routines.

**Tasks:**
1. Define `Params` struct in `lwe_params.hpp`:
   ```cpp
   struct Params {
     size_t n;    // secret dimension
     size_t m;    // # of samples
     uint64_t q;  // modulus
     double sigma; // error std. dev.
   };
   ```
2. **KeyGen** (`keygen.cpp`):
   - Sample matrix `A ∈ Z_q^{m×n}`.
   - Sample secret `s` and error `e` vectors from Gaussian.
   - Compute `b = A·s + e (mod q)`.
3. **Encrypt/Encaps** (`encrypt.cpp`):
   - Sample ephemeral `r`, `e1`, `e2`.
   - Compute `u = Aᵀ·r + e1`, `v = bᵀ·r + e2 + encode(m)`.
4. **Decrypt/Decaps** (`decrypt.cpp`):
   - Compute `mu = v - uᵀ·s (mod q)`.
   - Recover `m` via rounding/decoding.
5. Write comprehensive unit tests:
   - Known-answer tests (toy parameters n=4, q=17).
   - Edge-case tests (invalid ciphertexts, malformed input).

**Deliverables:**
- Correctly working encryption primitives.
- Test suite asserting 0% failure on valid inputs.

---

## 4. Testing & Validation

**Objectives:**
- Ensure correctness, quantify failure rates, and assess performance.

**Tasks:**
1. **Statistical testing**:
   - Monte Carlo: run 10⁶ encrypt/decrypt to compute empirical failure rate (should match theoretical bound).
2. **Performance benchmarking**:
   - Measure throughput (operations/sec) for KeyGen, Encaps, Decaps.
   - Profile hot spots using tools like `perf` or `valgrind`.
3. **Fuzz testing**:
   - Randomly mutate ciphertext bits to ensure decrypt either fails gracefully or returns fallback without crashing.
4. **Security audits**:
   - Review code for secret-dependent branches.
   - Add masking or bit‑mask comparisons in critical sections.

**Deliverables:**
- Test reports: failure rates, performance numbers.
- Identified and mitigated side‑channel risks.

---

## 5. Documentation & Packaging

**Objectives:**
- Create user-facing documentation and prepare for release.

**Tasks:**
1. Write `README.md`:
   - Project overview, build instructions, usage examples.
2. API documentation:
   - Document functions and data structures using Doxygen or Markdown.
3. Packaging:
   - Provide a single-header install option or CMake install target.
4. Examples:
   - CLI demo: generate keypair, encrypt a sample message, decrypt and verify.

**Deliverables:**
- Published GitHub release with binaries/source.
- Clear, concise documentation for developers.

---

## 6. Optional Next Steps

- **Optimizations**: hand‑optimize inner loops, enable OpenMP or SIMD.
- **Transition**: refactor into Ring/Module-LWE once plain LWE is stable.
- **Integration**: plug into a TLS library or messaging system.

---

*This dev flow was written by ChatGPT o4-mini-high*

