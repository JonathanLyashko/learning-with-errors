# Setting Up GMP and NTL on WSL with g++ and Makefile

This guide explains how to add **GMP** and **NTL** to your C++ project using **g++**, **WSL**, and a **Makefile**.

---

## 1. Install GMP and NTL

Run the following in your WSL terminal:

```bash
sudo apt update
sudo apt install libgmp-dev libntl-dev
```

These install:
- `libgmp`: High-performance big integer arithmetic
- `libntl`: Number theory library with modular arithmetic and matrix support

You can verify the installation with:
```bash
dpkg -l | grep libgmp
dpkg -l | grep libntl
```

---

## 2. Include in Your Code

In your `.cpp` or `.hpp` files:

```cpp
#include <NTL/ZZ.h>
#include <NTL/mat_ZZ.h>
#include <NTL/vec_ZZ.h>
#include <NTL/ZZ_p.h>
#include <gmp.h> // Optional for raw GMP
```

Use:
- `NTL::ZZ` for big integers
- `NTL::ZZ_p` for modular integers (after setting modulus)

---

## 3. Link Libraries in Makefile

Add the following to your `Makefile`:

```makefile
LIBS = -lntl -lgmp
```

Use this when linking your target:

```makefile
$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
```

---

## 4. Sanity Check File

Create a simple test file to verify everything works:

```cpp
// src/test_gmp_ntl.cpp
#include <NTL/ZZ.h>
#include <NTL/mat_ZZ.h>
#include <NTL/ZZ_p.h>
#include <iostream>

using namespace NTL;

int main() {
    ZZ a = conv<ZZ>(1234567890123456789ULL);
    ZZ b = conv<ZZ>(9876543210987654321ULL);
    ZZ c = a * b;
    std::cout << "a * b = " << c << std::endl;

    ZZ_p::init(ZZ(17)); // All math now mod 17
    ZZ_p x = conv<ZZ_p>(10);
    ZZ_p y = conv<ZZ_p>(15);
    std::cout << "10 + 15 mod 17 = " << x + y << std::endl;

    return 0;
}
```

Build and run it with:

```bash
g++ -std=c++17 -o test src/test_gmp_ntl.cpp -lntl -lgmp
./test
```

Expected output:
```
a * b = 12193263113702179522618503273362292369
10 + 15 mod 17 = 8
```

---

## Summary Table

| Task                     | Command / Code                           |
|--------------------------|-------------------------------------------|
| Install libs             | `sudo apt install libgmp-dev libntl-dev` |
| Include in code          | `#include <NTL/...>`                      |
| Link in Makefile         | `LIBS = -lntl -lgmp`                      |
| Test build & run         | `g++ -std=c++17 ... -lntl -lgmp`          |

*This guide was written by ChatGPT 4o*
