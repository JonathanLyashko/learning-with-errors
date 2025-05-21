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

// copy constructor
Matrix::Matrix(const Matrix& other) {
    this->rows_ = other.rows_;
    this->cols_ = other.cols_;
    this->mod_ = other.mod_;
    this->data_ = other.data_;
};

// copy assignment
Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) return *this;

    this->rows_ = other.rows_;
    this->cols_ = other.cols_;
    this->mod_ = other.mod_;

    // std vector handles this memory management with this assignment
    this->data_ = other.data_;

}; 

// destructor
Matrix::~Matrix() {
    // no real destructor needed since std vector handles deallocation
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


// --- MATRIX OPERATIONS ---
// arithmetic
Matrix Matrix::operator+(const Matrix& rhs) const {

    if (rows_ != rhs.rows_ || cols_ != rhs.cols_) {
        throw std::invalid_argument("Matrix dimensions do not match. This is required for matrix addition");
    }

    if (mod_ != rhs.mod_) {
        throw std::invalid_argument("Matrix moduli do not match. This is required for modular matrix addition");
    }

    NTL::Mat<NTL::ZZ> left = this->toNTL(); 
    NTL::Mat<NTL::ZZ> right = rhs.toNTL();
    NTL::Mat<NTL::ZZ> sum;
    NTL::add(sum, left, right);
    Matrix result = fromNTL(sum, mod_.get_str());
    return result;
};

Matrix Matrix::operator-(const Matrix& rhs) const {
    if (rows_ != rhs.rows_ || cols_ != rhs.cols_) {
        throw std::invalid_argument("Matrix dimensions do not match. This is required for matrix subtraction");
    }

    if (mod_ != rhs.mod_) {
        throw std::invalid_argument("Matrix moduli do not match. This is required for modular matrix subtraction");
    }

    NTL::Mat<NTL::ZZ> left = this->toNTL(); 
    NTL::Mat<NTL::ZZ> right = rhs.toNTL();
    NTL::Mat<NTL::ZZ> difference;
    NTL::sub(difference, left, right);
    Matrix result = fromNTL(difference, mod_.get_str());
    return result;
};

// matrix mulitplication
Matrix Matrix::operator*(const Matrix& rhs) const {
    if (cols_ != rhs.rows_) {
        throw std::invalid_argument("Matrix dimensions do not match. The number of columns in the left matrix must equal the number of rows in the right matrix");
    }

    if (mod_ != rhs.mod_) {
        throw std::invalid_argument("Matrix moduli do not match. This is required for modular matrix multiplication");
    }

    NTL::Mat<NTL::ZZ> left = this->toNTL(); 
    NTL::Mat<NTL::ZZ> right = rhs.toNTL();
    NTL::Mat<NTL::ZZ> product;
    NTL::mul(product, left, right);
    Matrix result = fromNTL(product, mod_.get_str());
    return result;
};




// --- DEBUGGING TOOLS ---
void Matrix::print() {
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            std::cout << data_[i * cols_ + j].toString() << " ";
        }
        std::cout << std::endl;
    }
};

