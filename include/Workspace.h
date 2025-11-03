#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include "Matrix.h"

/**
 * @class Workspace
 * @brief Manages a collection of matrices and performs matrix operations.
 *
 * The Workspace class acts as the logical backend for the Algebra Playground project.
 * It stores matrices in memory, supports creation, manipulation, and persistence,
 * and provides safe, exception-aware wrappers for all mathematical operations.
 *
 * Each matrix is identified by a unique string name, and the Workspace offers
 * both interactive (e.g., assignMatrix) and programmatic (e.g., multiplyMatrices)
 * operations. The class is also responsible for loading and saving entire
 * workspaces to and from disk.
 *
 * Design philosophy:
 * - Each method prints user-friendly feedback for CLI integration.
 * - All operations are exception-safe, using try/catch wrappers.
 * - Redundant try/catch and existence checks are unified through helper methods.
 */
class Workspace {
private:
    /**
     * @brief Stores all matrices in the workspace, indexed by their names.
     */
    std::unordered_map<std::string, Matrix> workspace;

public:
    // ========================= CONSTRUCTION =========================

    /**
     * @brief Default constructor.
     */
    Workspace() = default;

    /**
     * @brief Default destructor.
     */
    ~Workspace() = default;

    // ========================= QUERY METHODS =========================

    /**
     * @brief Returns the total number of matrices currently stored.
     * @return The number of matrices in the workspace.
     */
    [[nodiscard]] size_t getMatrixCount() const;

    /**
     * @brief Checks whether a matrix with the given name exists.
     * @param matName The name of the matrix to look for.
     * @return True if the matrix exists, false otherwise (also prints an error).
     */
    [[nodiscard]] bool matrixExists(const std::string& matName) const;

    // ========================= CREATION & BASIC OPS =========================

    /**
     * @brief Creates a new matrix and stores it in the workspace.
     *
     * @param matName The name of the new matrix.
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @param initValue Optional initial value for all elements (default = 0.0).
     * @return True if creation succeeded, false otherwise.
     */
    bool createMatrix(const std::string& matName, int rows, int cols, double initValue = 0.0);

    /**
     * @brief Lists all matrices currently stored in the workspace.
     * Prints each matrix name and its contents.
     * @return True if at least one matrix was listed, false if workspace is empty.
     */
    [[nodiscard]] bool listMatrices() const;

    /**
     * @brief Displays a single matrix by name.
     * @param matName The name of the matrix to display.
     * @return True if the matrix exists and was displayed successfully.
     */
    [[nodiscard]] bool showMatrix(const std::string& matName) const;

    /**
     * @brief Transposes a matrix and replaces its contents with the transposed version.
     * @param matName The name of the matrix to transpose.
     * @return True if transposition succeeded, false otherwise.
     */
    bool transposeMatrix(const std::string& matName);

    /**
     * @brief Allows interactive reassignment of matrix values through console input.
     *
     * The user is prompted for each element sequentially.
     *
     * @param matName The name of the matrix to modify.
     * @return True if assignment completed successfully, false otherwise.
     */
    bool assignMatrix(const std::string& matName);

    /**
     * @brief Deletes a matrix from the workspace.
     * @param matName The name of the matrix to delete.
     * @return True if deletion succeeded, false otherwise.
     */
    bool deleteMatrix(const std::string& matName);

    // ========================= MATHEMATICAL OPERATIONS =========================

    /**
     * @brief Computes and prints the rank of a matrix.
     * @param matName The name of the matrix.
     * @return True if successful, false otherwise.
     */
    [[nodiscard]] bool rankMatrix(const std::string& matName) const;

    /**
     * @brief Computes and prints the determinant of a matrix.
     * @param matName The name of the matrix.
     * @return True if successful, false otherwise.
     */
    [[nodiscard]] bool determinantMatrix(const std::string& matName) const;

    /**
     * @brief Multiplies a matrix by a scalar and stores the result.
     * @param resultName The name of the resulting matrix.
     * @param matName The name of the source matrix.
     * @param scalar The scalar value to multiply by.
     * @return True if successful, false otherwise.
     */
    bool scalarMultiplyMatrix(const std::string& resultName,
                              const std::string& matName,
                              double scalar);

    /**
     * @brief Generic helper for performing binary operations on two matrices.
     *
     * This function checks that both matrices exist, performs the given operation,
     * and stores the result under a new name.
     *
     * @param resultName Name of the resulting matrix.
     * @param mat1Name Name of the first operand.
     * @param mat2Name Name of the second operand.
     * @param op Binary operation (lambda or std::function) returning a new Matrix.
     * @return True if operation succeeded, false otherwise.
     */
    bool binaryMatrixOp(const std::string& resultName,
                        const std::string& mat1Name,
                        const std::string& mat2Name,
                        const std::function<Matrix(const Matrix&, const Matrix&)>& op);

    /**
     * @brief Adds two matrices and stores the result.
     * @param resultName The name of the resulting matrix.
     * @param mat1Name Name of the first operand.
     * @param mat2Name Name of the second operand.
     * @return True if addition succeeded, false otherwise.
     */
    bool addMatrices(const std::string& resultName,
                     const std::string& mat1Name,
                     const std::string& mat2Name);

    /**
     * @brief Subtracts one matrix from another and stores the result.
     * @param resultName The name of the resulting matrix.
     * @param mat1Name Name of the first operand.
     * @param mat2Name Name of the second operand.
     * @return True if subtraction succeeded, false otherwise.
     */
    bool subtractMatrices(const std::string& resultName,
                          const std::string& mat1Name,
                          const std::string& mat2Name);

    /**
     * @brief Multiplies two matrices and stores the result.
     * @param resultName The name of the resulting matrix.
     * @param mat1Name Name of the first operand.
     * @param mat2Name Name of the second operand.
     * @return True if multiplication succeeded, false otherwise.
     */
    bool multiplyMatrices(const std::string& resultName,
                          const std::string& mat1Name,
                          const std::string& mat2Name);

    /**
     * @brief Calculates and stores the inverse of a given matrix.
     * @param resultName The name of the resulting matrix.
     * @param matName The name of the source matrix.
     * @return True if inversion succeeded, false otherwise.
     */
    bool inverseMatrix(const std::string& resultName, const std::string& matName);

    /**
     * @brief Solves a linear system of equations Ax = b.
     *
     * Depending on the system's properties, the method reports:
     *  - Unique solution → stores result under resultName.
     *  - No solution → prints message.
     *  - Infinite solutions → prints message.
     *
     * @param resultName Name of the matrix to store the solution (if unique).
     * @param A Name of the coefficient matrix.
     * @param b Name of the column vector (right-hand side).
     * @return True if the operation executed successfully.
     */
    bool solveMatrix(const std::string& resultName,
                     const std::string& A,
                     const std::string& b);

    // ========================= FILE OPERATIONS =========================

    /**
     * @brief Saves the entire workspace (all matrices) to a text file.
     *
     * Each matrix is written as:
     * ```
     * <name> <rows> <cols>
     * <values row by row>
     * ```
     *
     * @param filename The target file name (stored inside "workspaces/" folder).
     * @return True if the workspace was saved successfully.
     */
    [[nodiscard]] bool saveWorkspaceToFile(const std::string& filename) const;

    /**
     * @brief Loads a workspace from a text file.
     *
     * Clears the current workspace and reconstructs it from the file contents.
     * The file must follow the format produced by saveWorkspaceToFile().
     *
     * @param filename The source file name (from "workspaces/" folder).
     * @return True if loading succeeded, false otherwise.
     */
    bool loadWorkspaceFromFile(const std::string& filename);

    // ========================= OPERATION HELPERS =========================

    /**
     * @brief Safely executes a mutable operation on a single matrix.
     *
     * Wraps exception handling and matrix existence checks to avoid repetition.
     *
     * @param matName Name of the matrix to modify.
     * @param op Operation to perform, taking a modifiable Matrix reference.
     * @return True if operation succeeded, false otherwise.
     */
    bool handleSingleMatrixOp(const std::string& matName,
                              const std::function<void(Matrix&)>& op);

    /**
     * @brief Safely executes a read-only operation on a single matrix.
     *
     * Similar to handleSingleMatrixOp, but guarantees const access and no modification.
     *
     * @param matName Name of the matrix to inspect.
     * @param op Operation to perform, taking a const Matrix reference.
     * @return True if operation succeeded, false otherwise.
     */
    bool handleReadOnlyMatrixOp(const std::string& matName,
                                const std::function<void(const Matrix&)>& op) const;


    /**
     * @brief Rotates a 3D vector matrix around the X, Y, and Z axes respectively.
     *
     * @param vecName  the name of the 3D vector matrix to rotate
     * @param angleDegreesX the rotation angle around the X axis in degrees
     * @param angleDegreesY the rotation angle around the Y axis in degrees
     * @param angleDegreesZ the rotation angle around the Z axis in degrees
     * @return  True if rotation succeeded, false otherwise
     */
    bool rotate3DVector(const std::string& vecName,
                        double angleDegreesX,
                        double angleDegreesY,
                        double angleDegreesZ);
};
