#pragma once
#include <iostream>
#include <vector>

struct SolveResult; // Forward declaration

using std::vector;

/**
 * @class Matrix
 * @brief Represents a 2D mathematical matrix supporting arithmetic,
 *        Gaussian elimination, determinant, inverse, and linear system solving.
 *
 * This class is implemented for educational and computational purposes.
 * It supports matrix operations such as addition, subtraction, multiplication,
 * transpose, determinant, rank, and inversion. Exception handling is used
 * to manage invalid operations, mismatched dimensions, or singular matrices.
 */
class Matrix {
private:
    vector<double> _matrix; ///< Flat vector storing matrix elements in row-major order.
    int _rows;              ///< Number of matrix rows.
    int _cols;              ///< Number of matrix columns.

    // ==== Internal constants ====
    static constexpr int MATRIX_LIMIT_ERROR = 10'000'000;   ///< Maximum allowed total elements (throws error).
    static constexpr int MATRIX_LIMIT_WARNING = 1'000'000;  ///< Warning threshold for large matrices.
    static constexpr double EPSILON = 1e-10;                ///< Tolerance for floating-point comparisons.

    // ==== Internal helpers for function overloading ====
    static constexpr Matrix* NOT_SOLVE = nullptr; ///< Helper for Gaussian elimination modes (no right-hand side).
    static constexpr int* NO_DET = nullptr;       ///< Helper for Gaussian elimination modes (no determinant).
    static constexpr bool FULL_REDUCTION = true;  ///< Flag for full (reduced) elimination mode.
    static constexpr bool NO_REDUCTION = false;   ///< Flag for forward-only elimination mode.

    /**
     * @brief Compute the 1D index in the internal vector for a given row and column.
     * @throws MatrixOutOfBounds if indices are invalid.
     */
    [[nodiscard]] int index(int row, int col) const;

    /**
     * @brief Swap two rows of the matrix in place.
     * @param row1 First row index.
     * @param row2 Second row index.
     */
    void swapRows(int row1, int row2);

    /**
     * @brief Augment the current matrix with another matrix on its right side.
     * @param right Matrix to append (must have same number of rows).
     * @return Combined augmented matrix [A | right].
     * @throws MatrixDimensionMismatch if row counts differ.
     */
    [[nodiscard]] Matrix augment(const Matrix& right) const;

public:
    // ==== Constructors & Destructor ====

    /**
     * @brief Default constructor. Creates an empty 0x0 matrix.
     */
    Matrix() = default;

    /**
     * @brief Construct a matrix of given size and initial value.
     * @param rows Number of rows (must be > 0).
     * @param cols Number of columns (must be > 0).
     * @param initValue Optional initial value for all elements (default 0.0).
     * @throws MatrixInvalidInitialization if dimensions are invalid.
     * @throws MatrixTooLarge if matrix exceeds size limit.
     */
    Matrix(int rows, int cols, double initValue = 0.0);

    /**
     * @brief Default destructor.
     */
    ~Matrix() = default;

    /**
     * @brief Copy assignment operator.
     * @param other Matrix to copy from.
     * @return Reference to this matrix.
     */
    Matrix& operator=(const Matrix& other);

    // ==== Comparison Operators ====

    /**
     * @brief Equality operator.
     * @param other Matrix to compare with.
     * @return True if both matrices have equal dimensions and elements.
     */
    bool operator==(const Matrix& other) const;

    /**
     * @brief Inequality operator.
     * @param other Matrix to compare with.
     * @return True if matrices differ in size or content.
     */
    bool operator!=(const Matrix& other) const;

    // ==== Element Access ====

    /**
     * @brief Access an element (const version).
     * @param row Row index (0-based).
     * @param column Column index (0-based).
     * @return Reference to element value (read-only).
     * @throws MatrixOutOfBounds if indices are invalid.
     */
    const double& operator()(int row, int column) const;

    /**
     * @brief Access an element (modifiable version).
     * @param row Row index (0-based).
     * @param column Column index (0-based).
     * @return Reference to element value (modifiable).
     * @throws MatrixOutOfBounds if indices are invalid.
     */
    double& operator()(int row, int column);

    // ==== Scalar Operations ====

    /**
     * @brief Multiply matrix by scalar in place.
     * @param scalar Scalar value.
     * @return Reference to this matrix after modification.
     */
    Matrix& operator*=(const double& scalar);

    /**
     * @brief Unary negation (returns -A).
     * @return New matrix with all elements negated.
     */
    Matrix operator-() const;

    // ==== Basic Information ====

    /**
     * @brief Get the number of rows.
     * @return Row count.
     */
    [[nodiscard]] int getRows() const;

    /**
     * @brief Get the number of columns.
     * @return Column count.
     */
    [[nodiscard]] int getCols() const;

    // ==== I/O ====

    /**
     * @brief Stream insertion operator for printing matrices.
     *
     * Prints the matrix in a row-by-row format.
     * Example:
     * @code
     * |1|2|3|
     * |4|5|6|
     * @endcode
     */
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

    // ==== Matrix Arithmetic ====

    Matrix operator*(const Matrix& other) const;      ///< Matrix multiplication.
    Matrix operator+(const Matrix& other) const;      ///< Matrix addition.
    Matrix operator-(const Matrix& other) const;      ///< Matrix subtraction.
    Matrix& operator+=(const Matrix& other);          ///< In-place addition.
    Matrix& operator-=(const Matrix& other);          ///< In-place subtraction.
    Matrix& operator*=(const Matrix& other);          ///< In-place multiplication.
    Matrix operator*(const double& scalar) const;     ///< Matrix-scalar multiplication.

    // ==== Advanced Operations ====

    /**
     * @brief Transpose the matrix (rows become columns).
     * @return Transposed matrix.
     */
    [[nodiscard]] Matrix transpose() const;

    /**
     * @brief Perform Gaussian elimination.
     *
     * Used internally for determinant, inversion, and system solving.
     * Supports both partial pivoting and full reduction (RREF).
     *
     * @param right Optional right-hand side matrix (for Ax=b solving).
     * @param fullReduction If true, perform full reduction (RREF).
     * @param swapCount Optional pointer to record number of row swaps (for determinant).
     * @return Resulting upper-triangular or reduced matrix.
     * @throws MatrixSingular if the matrix is singular and full reduction is required.
     */
    Matrix gaussianElimination(Matrix* right,
                               bool fullReduction,
                               int* swapCount = nullptr) const;

    /**
     * @brief Compute the determinant of the matrix.
     * @return Determinant value.
     * @throws MatrixNotSquare if matrix is not square.
     */
    [[nodiscard]] double determinant() const;

    /**
     * @brief Compute the matrix rank.
     * @return Rank of the matrix.
     */
    [[nodiscard]] int rank() const;

    /**
     * @brief Compute the inverse of the matrix (A⁻¹).
     * @return Inverse matrix.
     * @throws MatrixNotSquare if not square.
     * @throws MatrixSingular if determinant is zero.
     */
    [[nodiscard]] Matrix inverse() const;

    /**
     * @brief Create an identity matrix of given size.
     * @param size Dimension (n×n).
     * @return Identity matrix.
     * @throws MatrixInvalidInitialization if size <= 0.
     */
    static Matrix identity(int size);

    /**
     * @brief Solve a linear system Ax = b using Gaussian elimination.
     *
     * Determines whether the system has a unique, infinite, or no solution.
     * If a unique solution exists, it is returned in SolveResult.x.
     *
     * @param b Column vector (matrix with one column).
     * @return SolveResult structure with solution and status.
     * @throws MatrixDimensionMismatch if dimensions are invalid.
     */
    [[nodiscard]] SolveResult solve(const Matrix& b) const;
};

/**
 * @brief Scalar-matrix multiplication (scalar * matrix).
 * @param scalar Scalar multiplier.
 * @param matrix Matrix operand.
 * @return New matrix where each element is scaled.
 */
Matrix operator*(const double& scalar, const Matrix& matrix);

/**
 * @brief Status of a linear system solution.
 *
 * Used in SolveResult to indicate whether Ax = b has a unique,
 * infinite, or no solution.
 */
enum class SolveStatus {
    Unique,     ///< A unique solution exists.
    Infinite,   ///< Infinitely many solutions exist.
    NoSolution  ///< No valid solution exists.
};

/**
 * @brief Structure representing the result of solving a linear system.
 *
 * Contains both the solution status and the resulting vector (if unique).
 */
struct SolveResult {
    SolveStatus status; ///< Solution status.
    Matrix x;           ///< Solution matrix (valid only if status == Unique).
};