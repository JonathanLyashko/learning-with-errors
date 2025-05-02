#include "ModInt.h"
#include <gmp.h>
#include <stdexcept>


// constructor: check the value modulus, choose if we need small or big
ModInt::ModInt(const std::string& value_str, const std::string& modulus_str) {
    mpz_t tmp_val;
    mpz_t tmp_mod; 
    mpz_inits(tmp_val, tmp_mod, nullptr); 

    if (mpz_set_str(tmp_mod, modulus_str.c_str(), 10) != 0) {
        throw std::invalid_argument("Invalid modulus string");
    } if (mpz_set_str(tmp_val, value_str.c_str(), 10) != 0) {
        throw std::invalid_argument("Invalid value string");
    } if (mpz_cmp_ui(tmp_mod, 0) == 0) {
        throw std::invalid_argument("Modulus cannot be zero");
    }

    // if mod fits into an unsigned long, use 64 bit arithmetic
    if (mpz_fits_ulong_p(tmp_mod)) {
        is_large = false;
        mod_small = mpz_get_ui(tmp_mod);
        val_small = mpz_get_ui(tmp_val) % mod_small;
    } else {
        is_large = true;
        mpz_inits(val_big, mod_big, nullptr);
        mpz_set(mod_big, tmp_mod); 
        mpz_mod(val_big, tmp_val, mod_big);
    }

    mpz_clears(tmp_val, tmp_mod, nullptr); // clear memory since mpz stores data-word size chunks of the number (dynamic)
}


// copy constructor
ModInt::ModInt(const ModInt& other) {
    is_large = other.is_large;

    if (is_large) {
        mpz_inits(val_big, mod_big, nullptr);
        mpz_set(val_big, other.val_big);
        mpz_set(mod_big, other.mod_big);
    } else {
        val_small = other.val_small;
        mod_small = other.mod_small;
    }
}


// copy assignment
ModInt& ModInt::operator=(const ModInt& other) {
    if (this == &other) return *this;

    //clear old big state if needed
    if (is_large) {
        mpz_clears(val_big, mod_big, nullptr);
    }

    is_large = other.is_large;
    if (is_large) {
        mpz_inits(val_big, mod_big, nullptr);
        mpz_set(val_big, other.val_big);
        mpz_set(mod_big, other.mod_big);
    } else {
        val_small = other.val_small;
        mod_small = other.mod_small;
    }

    return *this;
}


// destructor 
ModInt::~ModInt() {
    if (is_large) {
        mpz_clears(val_big, mod_big, nullptr);
    }
}

// addition 
ModInt ModInt::operator+(const ModInt& rhs) const {
    if (is_large != rhs.is_large) {
        throw std::invalid_argument("Modulus sizes do not match");
    }
    
    if (!is_large) {
        uint64_t sum = val_small + rhs.val_small;
        return ModInt(std::to_string(sum % mod_small), std::to_string(mod_small));
    } else {
        mpz_t res; 
        mpz_init(res);
        mpz_add(res, val_big, rhs.val_big);
        mpz_mod(res, res, mod_big);

        char* res_str = mpz_get_str(nullptr, 10, res);
        char* mod_str = mpz_get_str(nullptr, 10, mod_big);
        ModInt out(res_str, mod_str);

        mpz_clear(res);
        free(res_str);
        free(mod_str);
        return out;
    }
}



// subtraction
ModInt ModInt::operator-(const ModInt& rhs) const {
    if (is_large != rhs.is_large) {
        throw std::invalid_argument("Modulus sizes do not match");
    }
    
    if (!is_large) {
        uint64_t diff = (val_small + mod_small - rhs.val_small) % mod_small;
        return ModInt(std::to_string(diff), std::to_string(mod_small));
    } else {
        mpz_t res;
        mpz_init(res);
        mpz_sub(res, val_big, rhs.val_big);
        mpz_mod(res, res, mod_big);

        char*  res_str = mpz_get_str(nullptr, 10, res);
        char*  mod_str = mpz_get_str(nullptr, 10, mod_big);
        ModInt out(res_str, mod_str);

        mpz_clear(res);
        free(res_str);
        free(mod_str);
        return out;
    }
}


// multiplication
ModInt ModInt::operator*(const ModInt& rhs) const {
    if (is_large != rhs.is_large)
        throw std::invalid_argument("Mismatched modulus sizes");

    if (!is_large) {
        uint64_t prod = (val_small * rhs.val_small) % mod_small;
        return ModInt(std::to_string(prod),
                      std::to_string(mod_small));
    } else {
        mpz_t res;
        mpz_init(res);
        mpz_mul(res, val_big, rhs.val_big);
        mpz_mod(res, res, mod_big);

        char*  res_str = mpz_get_str(nullptr, 10, res);
        char*  mod_str = mpz_get_str(nullptr, 10, mod_big);
        ModInt out(res_str, mod_str);

        mpz_clear(res);
        free(res_str);
        free(mod_str);
        return out;
    }
}


// convert current value to decimal string (base 10) 
std::string ModInt::toString() const {
    if (!is_large) {
        return std::to_string(val_small);
    } else {
        // convert the large value to a string representation
        char* value_cstr = mpz_get_str(nullptr, 10, val_big);
        std::string value_str(value_cstr);

        // free the allocated memory for the C-style string
        free(value_cstr);

        // return the string representation
        return value_str;
    }
}