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


// copy constructor ()