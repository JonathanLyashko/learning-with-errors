#include "ModInt.hpp"
#include "Matrix.hpp"
#include <gmp.h>
#include <stdexcept> //for unsigned or signed 64b int
#include <sstream>



// --- CONSTRUCTORS ---
// default constructor, creates a 0x0 matrix
Matrix::Matrix() {
    this->rows_ = 0;
    this->cols_ = 0;
    this->mod_ = mpz_class(1);
    this->data_ = {}; // size == rows_ * cols_ (access using i*cols_ + j)
}
// dimension based constructor of size rows x cols (zero filled)
Matrix::Matrix(size_t rows, size_t cols, const std::string& mod_str) {
    this->rows_ = rows;
    this->cols_ = cols;
    this->mod_ = mpz_class(mod_str);
    this->data_ = std::vector<ModInt>(rows * cols, ModInt("0", mod_str));
}; 



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


// --- NTL CONVERSIONS --- 
NTL::Mat<NTL::ZZ> Matrix::toNTL() const {
    NTL::Mat<NTL::ZZ> result; 
    result.SetDims(rows_, cols_);

    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            NTL::ZZ val;
            NTL::conv(val, data_[i * cols_ + j].toString().c_str());

            result(i+1, j+1) = val;
        }
    }

    return result;
}

Matrix Matrix::fromNTL(const NTL::Mat<NTL::ZZ>& M, const std::string& mod_str) {
    size_t numRows = M.NumRows();
    size_t numCols = M.NumCols();
    Matrix result = Matrix(numRows, numCols, mod_str);
    
    for (size_t i = 0; i < numRows; i++) {
        for (size_t j = 0; j < numCols; j++) {
            // convert to string (NTL doesn't have this conversion built in)
            std::stringstream ss;
            ss << M(i+1, j+1);
            std::string str_val = ss.str();

            result(i, j) = ModInt(str_val, mod_str);
        }
    }

    return result;
}

