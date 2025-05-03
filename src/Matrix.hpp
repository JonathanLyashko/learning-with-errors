#pragma once 

#include <gmpxx.h>
#include <cstddef>

#include <string>
#include <vector>

#include <NTL/mat_ZZ.h>
#include <NTL/ZZ.h>

#include <stdexcept>
#include "ModInt.hpp" // our custom modular arithmetic class

class Matrix {

    public: 
        // --- CONSTRUCTORS ---
        Matrix(); // default constructor creates a 0x0 matrix
        Matrix(size_t rows, size_t cols, const std::string& mod_str); // dimension based constructor of size rows x cols (zero filled)
        Matrix(const Matrix& other); // copy constructor
        Matrix(Matrix&& other) noexcept; // move constructor
        Matrix& operator=(const Matrix& other); // copy assignment
        Matrix& operator=(Matrix&& other) noexcept; // move assignment
        ~Matrix(); // destructor

        // --- DIMENSIONS ---
        size_t rows() const noexcept;
        size_t cols() const noexcept;
        std::string modulus() const noexcept;


        // --- ELEMENT ACCESS ---
        ModInt& operator()(size_t i, size_t j); // called on a non-const Matrix (allow modifications to entries)
        const ModInt& operator()(size_t i, size_t j) const; // called on a const Matrix (doesn't allow modifications to entries)

        // --- NTL CONVERSIONS --- 
        NTL::Mat<NTL::ZZ> toNTL() const;
        static Matrix fromNTL(const NTL::Mat<NTL::ZZ>& M, const std::string& mod_str);


    private: 

        size_t rows;
        size_t cols_;
        mpz_class mod_;
        std::vector<ModInt> data_; // size == rows_ * cols_ (access using i*cols_ + j)

};