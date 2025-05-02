#pragma once 
#include <gmp.h>
#include <cstdint>
#include <stdexcept>

class ModInt {

    public: 
        // constructor takes value and modulus as strings in base 10 
        ModInt(const std::string& value_str, const std::string& modulus_str);

        ModInt(const ModInt& other); // copy constructor 
        ModInt& operator=(const ModInt& other); // copy assignment 
        ~ModInt(); // destructor 


        // modular arithmetic operations
        ModInt operator+(const ModInt& rhs) const;
        ModInt operator-(const ModInt& rhs) const;
        ModInt operator*(const ModInt& rhs) const;

        std::string toString() const; // convert internal value to human readable decimal string

    private: 
        bool is_large; // true if we need to use the GMP library (mpz_t types) and false if we're using uint64_t (64 bit unisgned integer)

        // used when modulus fits into 64 bits
        uint64_t val_small;
        uint64_t mod_small;

        // used when modulus is bigger than 64 bits
        mpz_t val_big;
        mpz_t mod_big;
};