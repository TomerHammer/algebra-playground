#pragma once
#include <stdexcept>
#include <string>

class MatrixException : public std::runtime_error {
public:
    explicit MatrixException(const std::string& message)
        : std::runtime_error(message) {}
};

class MatrixOutOfBounds : public MatrixException {
public:
    MatrixOutOfBounds(int length, int width)
        : MatrixException("Out of matrix bounds. Dimensions are " +
                  std::to_string(length) + "x" + std::to_string(width)) {}
};

class MatrixDimensionMismatch : public MatrixException {
public:
    MatrixDimensionMismatch(int firstRows, int firstCols,
                 int secondRows, int secondCols)
        : MatrixException("Sizes do not match. First matrix dimensions: " +
                  std::to_string(firstRows) + "x" + std::to_string(firstCols) +
                  ", second matrix dimensions: " +
                  std::to_string(secondRows) + "x" + std::to_string(secondCols)) {}
};

class MatrixInvalidInitialization : public MatrixException {
public:
    MatrixInvalidInitialization()
        : MatrixException("Matrix dimensions must be positive integers.") {}
};

class MatrixTooLarge : public MatrixException {
public:
    MatrixTooLarge()
        : MatrixException("Matrix too large - exceeds 10 million elements.") {}
};

class MatrixNotSquare : public MatrixException {
public:
    MatrixNotSquare():
    MatrixException("Matrix must be square for the desired operation.") {}
};

class MatrixSingular : public MatrixException {
public:
    MatrixSingular():
    MatrixException("Matrix is singular and cannot be inverted.") {}
};