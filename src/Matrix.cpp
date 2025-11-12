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
	bool throwOnZeroPivot = (right != nullptr) || fullReduction;

	// Perform forward elimination and count swaps
	Matrix result = this->forwardElimination(right, throwOnZeroPivot, swapCount);

	if (!fullReduction) {
		// Return the upper-triangular
		return result;
	}

	// Full reduction (: normalize pivot rows and eliminate above
	const int n = result.getRows();
	const int m = result.getCols();
	const int rCols = right ? right->getCols() : 0;

	for (int i = 0; i < n; ++i) {
		// Find pivot column in this row
		int pivotCol = -1;
		for (int j = 0; j < m; ++j) {
			if (std::abs(result(i, j)) > EPSILON) { pivotCol = j; break; }
		}
		if (pivotCol == -1) continue; // zero row

		double pivot = result(i, pivotCol);
		// Normalize pivot row
		for (int j = pivotCol; j < m; ++j)
			result(i, j) /= pivot;

		if (right) {
			for (int j = 0; j < rCols; ++j)
				(*right)(i, j) /= pivot;
		}

		// Eliminate other rows using this pivot
		for (int k = 0; k < n; ++k) {
			if (k == i) continue;
			double c = result(k, pivotCol);
			if (std::abs(c) < EPSILON) continue;

			for (int j = pivotCol; j < m; ++j)
				result(k, j) -= c * result(i, j);

			if (right) {
				for (int j = 0; j < rCols; ++j)
					(*right)(k, j) -= c * (*right)(i, j);
			}
		}
	}

	return result;
}


Matrix Matrix::forwardElimination(Matrix* right, bool throwOnZeroPivot, int* swapCount) const {
	Matrix result(*this);
	const int n = result.getRows();
	const int m = result.getCols();
	int localSwapCount = 0;
	int lim = std::min(n, m);

	for (int i = 0; i < lim; ++i) {
		// partial pivoting
		double maxEl = std::abs(result(i, i));
		int maxRow = i;
		for (int k = i + 1; k < n; ++k) {
			double v = std::abs(result(k, i));
			if (v > maxEl) { maxEl = v; maxRow = k; }
		}

		// Handle near-zero pivot
		if (std::abs(result(maxRow, i)) < EPSILON) {
			if (throwOnZeroPivot)
				throw MatrixSingular();
			// Skip this column/pivot; continue to next column
			continue;

		}

		// Swap rows if needed (and apply same to right)
		if (maxRow != i) {
			result.swapRows(i, maxRow);
			if (right) right->swapRows(i, maxRow);
			localSwapCount++;
		}

		// Eliminate below pivot
		for (int k = i + 1; k < n; ++k) {
			double c = -result(k, i) / result(i, i);
			for (int j = i; j < m; ++j) result(k, j) += c * result(i, j);
			if (right) for (int j = 0; j < right->getCols(); ++j)
				(*right)(k, j) += c * (*right)(i, j);
		}
	}

	if (swapCount) *swapCount = localSwapCount;
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

	Matrix echelon = this->forwardElimination(NOT_SOLVE, false, NO_DET);
	const int rows = echelon.getRows();
	const int cols = echelon.getCols();

	// Count non-zero rows
	int nonZeroRows = 0;
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (std::abs(echelon(i, j)) > EPSILON) { ++nonZeroRows; break; }
		}
	}

	// Count non-zero columns
	int nonZeroCols = 0;
	for (int j = 0; j < cols; ++j) {
		for (int i = 0; i < rows; ++i) {
			if (std::abs(echelon(i, j)) > EPSILON) { ++nonZeroCols; break; }
		}
	}

	// return the minimum of non-zero rows and columns
	return std::min(nonZeroRows, nonZeroCols);
}

Matrix Matrix::inverse() const {
	if (_rows != _cols)
		throw MatrixNotSquare();

	// Build augmented matrix [A | I]
	Matrix augmented = this->augment(Matrix::identity(_rows));

	// Perform full reduction on the augmented matrix. This will throw
	// MatrixSingular if the matrix is not invertible.
	Matrix reduced = augmented.gaussianElimination(NOT_SOLVE, FULL_REDUCTION, NO_DET);

	// Extract right half as the inverse
	Matrix inv(_rows, _cols);
	for (int i = 0; i < _rows; ++i) {
		for (int j = 0; j < _cols; ++j) {
			inv(i, j) = reduced(i, _cols + j);
		}
	}

	return inv;
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

Matrix Matrix::XRotationMatrix(double angleDegrees) {
	Matrix rotation(3,3);
	double angleRadians = angleDegrees * M_PI / 180.0;
	rotation(0,0) = 1; rotation(0,1) = 0;                     rotation(0,2) = 0;
	rotation(1,0) = 0; rotation(1,1) = cos(angleRadians);     rotation(1,2) = -sin(angleRadians);
	rotation(2,0) = 0; rotation(2,1) = sin(angleRadians);     rotation(2,2) = cos(angleRadians);
	return rotation;
}

Matrix Matrix::YRotationMatrix(double angleDegrees) {
	Matrix rotation(3,3);
	double angleRadians = angleDegrees * M_PI / 180.0;
	rotation(0,0) = cos(angleRadians);     rotation(0,1) = 0; rotation(0,2) = sin(angleRadians);
	rotation(1,0) = 0;                     rotation(1,1) = 1; rotation(1,2) = 0;
	rotation(2,0) = -sin(angleRadians);    rotation(2,1) = 0; rotation(2,2) = cos(angleRadians);
	return rotation;
}

Matrix Matrix::ZRotationMatrix(double angleDegrees) {
	Matrix rotation(3,3);
	double angleRadians = angleDegrees * M_PI / 180.0;
	rotation(0,0) = cos(angleRadians);     rotation(0,1) = -sin(angleRadians);    rotation(0,2) = 0;
	rotation(1,0) = sin(angleRadians);     rotation(1,1) = cos(angleRadians);     rotation(1,2) = 0;
	rotation(2,0) = 0;                     rotation(2,1) = 0;                     rotation(2,2) = 1;
	return rotation;
}

Matrix Matrix::createRotationMatrix(double angleDegreesX, double angleDegreesY, double angleDegreesZ){
	Matrix rotationX = XRotationMatrix(angleDegreesX);
	Matrix rotationY = YRotationMatrix(angleDegreesY);
	Matrix rotationZ = ZRotationMatrix(angleDegreesZ);
	Matrix rotation = rotationZ * rotationY * rotationX;
	return rotation;
}

Matrix Matrix::rotate3D(double angleDegreesX, double angleDegreesY, double angleDegreesZ) const {
	Matrix rotation = createRotationMatrix(angleDegreesX, angleDegreesY, angleDegreesZ);
	return rotation * (*this);
}