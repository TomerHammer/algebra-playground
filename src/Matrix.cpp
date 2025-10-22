#include "../include/Matrix.h"
#include "../include/MatrixException.h"
#include <iomanip>

int Matrix::index(int row, int col) const {
	if (row < 0 || row >= _rows || col < 0 || col >= _cols) {
		throw MatrixOutOfBounds(_rows, _cols);
	}
	return row * _cols + col;
}

Matrix::Matrix(int rows, int cols, double initValue)
	:_rows(rows), _cols(cols){
	if (rows <= 0 || cols <= 0) {
		throw MatrixInvalidInitialization();
	}

	if (rows * cols >= MATRIX_LIMIT_ERROR )
		throw MatrixTooLarge();

	if (rows * cols >= MATRIX_LIMIT_WARNING )
		std::cerr << "Warning: large matrix may slow down performance.\n";
	_matrix.resize(rows * cols, initValue);
}

Matrix& Matrix::operator=(const Matrix& other) {
	if (this != &other) {
		_rows = other._rows;
		_cols = other._cols;
		_matrix = other._matrix;
	}
	return *this;
}
bool Matrix::operator==(const Matrix& other) const {
	if (_rows != other._rows || _cols != other._cols || _matrix != other._matrix){
		return false;
	}
	return true;
}

bool Matrix::operator!=(const Matrix& other) const {
	return !(*this == other);
}

const double& Matrix::operator()(int row, int column) const {
	return _matrix[index(row, column)];
}

double& Matrix::operator()(int row, int column) {
	return _matrix[index(row, column)];
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
	os << std::fixed << std::setprecision(3);
	const int width = 7;

	for (int row = 0; row < matrix._rows; ++row) {
		os << "|";
		for (int col = 0; col < matrix._cols; ++col) {
			os << std::setw(width) << matrix(row, col) << "|";
		}
		os << '\n';
	}

	return os;
}


Matrix& Matrix::operator-=(const Matrix& other){
	if (_rows != other._rows || _cols != other._cols) {
		throw MatrixDimensionMismatch(_rows, _cols, other._rows, other._cols);
	}
	for (size_t i = 0; i < _matrix.size(); ++i)
		_matrix[i] -= other._matrix[i];
	return *this;
}

Matrix Matrix::operator-(const Matrix& other) const {
	Matrix result(*this);
	result -= other;
	return result;
}

Matrix& Matrix::operator+=(const Matrix& other){
	if (_rows != other._rows || _cols != other._cols) {
		throw MatrixDimensionMismatch(_rows, _cols, other._rows, other._cols);
	}
	for (size_t i = 0; i < _matrix.size(); ++i)
		_matrix[i] += other._matrix[i];
	return *this;
}

Matrix Matrix::operator*(const double& scalar) const {
	Matrix result(*this);
	result *= scalar;
	return result;
}

Matrix Matrix::operator+(const Matrix& other) const {
	Matrix result(*this);
	result += other;
	return result;
}

Matrix operator*(const double& scalar, const Matrix& matrix) {
	Matrix result = matrix * scalar;
	return result;
}

Matrix& Matrix::operator*=(const double& scalar) {
	for (auto& val : _matrix)
		val *= scalar;
	return *this;
}

Matrix Matrix::operator-() const{
	Matrix result = *this;
	return -1 * result;
}

Matrix Matrix::transpose() const{
	Matrix result(_cols, _rows);
	for (int row = 0; row < _rows; row++) {
		for (int col = 0; col < _cols; col++) {
			result(col, row) = (*this)(row, col);
		}
	}
	return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
	if (_cols != other._rows) {
		throw MatrixDimensionMismatch(_rows, _cols, other._rows, other._cols);
	}
	Matrix result(_rows, other._cols, 0.0);
	for (int row = 0; row < _rows; row++) {
		for (int col = 0; col < other._cols; col++) {
			double sum = 0.0;
			for (int k = 0; k < _cols; k++) {
				sum += (*this)(row, k) * other(k, col);
			}
			result(row, col) = sum;
		}
	}
	return result;
}

Matrix& Matrix::operator*=(const Matrix& other) {
	*this = Matrix(*this * other);
	return *this;
}

int Matrix::getRows() const {
	return _rows;
}

int Matrix::getCols() const {
	return _cols;
}

void Matrix::swapRows(int row1, int row2) {
	if (row1 == row2) return;
	int cols = getCols();
	for (int j = 0; j < cols; j++) {
		std::swap((*this)(row1, j), (*this)(row2, j));
	}
}


Matrix Matrix::gaussianElimination(Matrix* right, bool fullReduction, int* swapCount) const {
	Matrix result(*this);
	const int n = result.getRows();
	const int m = result.getCols();
	int localSwapCount = 0;

	int lim = std::min(n, m);
	for (int i = 0; i < lim; i++) {
		// Pivot selection
		double maxEl = std::abs(result(i, i));
		int maxRow = i;
		for (int k = i + 1; k < n; k++) {
			if (std::abs(result(k, i)) > maxEl) {
				maxEl = std::abs(result(k, i));
				maxRow = k;
			}
		}
		// Handle zero (or near-zero) pivot
		if (std::abs(result(maxRow, i)) < EPSILON) {

			if (!right && !fullReduction) { // Determinant / rank calculation can continue
				continue;
			}

			throw MatrixSingular(); // Singular matrix for solving/inversion
		}


		// Swap rows apply to right side too if present
		if (maxRow != i) {
			result.swapRows(i, maxRow);
			if (right) right->swapRows(i, maxRow);
			localSwapCount++;
		}

		// Normalize pivot row if full reduction
		double pivot = result(i, i);
		if (fullReduction) {
			for (int j = 0; j < m; j++) result(i, j) /= pivot;
			if (right)
				for (int j = 0; j < right->getCols(); j++)
					(*right)(i, j) /= pivot;
		}

		// Eliminate below pivot (forward)
		const int rCols = right ? right->getCols() : 0;
		for (int k = i + 1; k < n; k++) {
			double c = -result(k, i) / result(i, i);
			for (int j = i; j < m; j++) result(k, j) += c * result(i, j);
			if (right)
				for (int j = 0; j < rCols; j++)
					(*right)(k, j) += c * (*right)(i, j);
		}


		// Eliminate above pivot, full reduction
		if (fullReduction) {
			for (int k = 0; k < i; k++) {
				double c = -result(k, i);
				for (int j = 0; j < m; j++) result(k, j) += c * result(i, j);
				if (right)
					for (int j = 0; j < right->getCols(); j++)
						(*right)(k, j) += c * (*right)(i, j);
			}
		}
	}

	if (swapCount) // return swap count for determinant sign adjustment
		*swapCount = localSwapCount;

	return result;
}



double Matrix::determinant() const {
	if (_rows != _cols)
		throw MatrixNotSquare();

	int swapCount = 0;
	Matrix result = gaussianElimination(NOT_SOLVE, NO_REDUCTION, &swapCount);

	double det = 1.0;
	for (int i = 0; i < _rows; i++) {
		det *= result(i, i);
	}
	if (std::abs(det) < EPSILON) // consider as zero
		det = 0.0; // Avoid negative zero

	// Adjust sign for row swaps
	if (swapCount % 2 != 0)
		det =  det != 0.0 ? -det : 0.0; // Avoid negative zero

	return det;
}


int Matrix::rank() const {
	Matrix result = this->gaussianElimination(NOT_SOLVE,NO_REDUCTION,NO_DET);
	int rank = 0;
	for (int i = 0; i < result.getRows(); i++) {
		bool nonZeroRow = false;
		for (int j = 0; j < result.getCols(); j++) {
			if (std::abs(result(i, j)) > EPSILON) {
				nonZeroRow = true;
				break;
			}
		}
		if (nonZeroRow) {
			rank++;
		}
	}
	return rank;
}

Matrix Matrix::inverse() const {
	if (_rows != _cols)
		throw MatrixNotSquare();

	Matrix right = Matrix::identity(_rows);

	 // Might throw MatrixSingular
	this->gaussianElimination(&right, FULL_REDUCTION, NO_DET);

	return right; // Now right contains A⁻¹
}


Matrix Matrix::identity(int size) {
	if (size <= 0) {
		throw MatrixInvalidInitialization();
	}
	Matrix result(size, size, 0.0);
	for (int i = 0; i < size; i++) {
		result(i, i) = 1.0;
	}
	return result;
}

Matrix Matrix::augment(const Matrix& right) const {
	if (_rows != right.getRows())
		throw MatrixDimensionMismatch(_rows, _cols, right.getRows(), right.getCols());

	Matrix result(_rows, _cols + right.getCols());
	for (int i = 0; i < _rows; i++) {
		for (int j = 0; j < _cols; j++)
			result(i, j) = (*this)(i, j);
		for (int j = 0; j < right.getCols(); j++)
			result(i, _cols + j) = right(i, j);
	}
	return result;
}

SolveResult Matrix::solve(const Matrix& b) const {
	if (_rows != b.getRows() || b.getCols() != 1)
		throw MatrixDimensionMismatch(_rows, _cols, b.getRows(), b.getCols());

	// Compute rank of A and [A|b]
	int rankA = this->rank();

	// Build augmented matrix [A|b]
	Matrix augmented = this->augment(b);
	int rankAb = augmented.rank();

	// check for no solution or infinite solutions
	if (rankAb > rankA)
		return { SolveStatus::NoSolution, Matrix() };

	if (rankA < _cols)
		return { SolveStatus::Infinite, Matrix() };

	// If reach here, rankA == rankAb == number of variables => unique solution

	Matrix right(b);
	// Perform Gaussian elimination with full reduction to find solution
	this->gaussianElimination(&right, FULL_REDUCTION, NO_DET);

	// right now holds x
	return { SolveStatus::Unique, right };
}

