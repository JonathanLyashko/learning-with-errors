#include "ModInt.hpp"
#include "Matrix.hpp"
#include <gmp.h>
#include <stdexcept> //for unsigned or signed 64b int



// --- CONSTRUCTORS ---
// default constructor, creates a 0x0 matrix
Matrix::Matrix() {
    this->rows_ = 0;
    this->cols_ = 0;
    this->mod_ = mpz_class(1);
    this->data_ = {}; // size == rows_ * cols_ (access using i*cols_ + j)
}



// --- DIMENSIONS ---
// getter functions
size_t Matrix::rows() const noexcept {
    return rows_;
}

size_t Matrix::cols() const noexcept {
    return cols_;
}

std::string Matrix::modulus() const noexcept {
    return mod_.get_str();
}


// --- ELEMENT ACCESS ---
ModInt& Matrix::operator()(size_t i, size_t j) {
    if (i >= rows_) {
        throw std::out_of_range("Matrix row index out of bounds");
    } else if (j >= cols_) {
        throw std::out_of_range("Matrix column index out of bounds");
    } else {
        return data_[i * cols_ + j];
    }
}

const ModInt& Matrix::operator()(size_t i, size_t j) const {
    if (i >= rows_) {
        throw std::out_of_range("Matrix row index out of bounds");
    } else if (j >= cols_) {
        throw std::out_of_range("Matrix column index out of bounds");
    } else {
        return data_[i * cols_ + j];
    }
}


