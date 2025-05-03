#include <NTL/ZZ.h>
#include <NTL/mat_ZZ.h>
#include <NTL/ZZ_p.h>
#include <iostream>

using namespace NTL;

int main() {
    ZZ a = conv<ZZ>( (unsigned long)1234567890123456789ULL);
    ZZ b = conv<ZZ>( (unsigned long)9876543210987654321ULL);
    ZZ c = a * b;
    std::cout << "a * b = " << c << std::endl;

    ZZ_p::init(ZZ(17)); // all math now mod 17
    ZZ_p x = conv<ZZ_p>(10);
    ZZ_p y = conv<ZZ_p>(15);
    std::cout << "10 + 15 mod 17 = " << x + y << std::endl;

    return 0;
}