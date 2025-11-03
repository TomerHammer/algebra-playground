#include "Workspace.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include "MatrixException.h"

size_t Workspace::getMatrixCount() const{
    return workspace.size();
  }

bool Workspace::matrixExists(const std::string& matName) const {
    if (workspace.find(matName) == workspace.end()) {
        std::cout << "Matrix '" << matName << "' not found in workspace." << std::endl;
        return false;
    }
    return true;
}

bool Workspace::createMatrix(const std::string& matName, const int rows, const int cols, const double initValue) {
    Matrix matrix;
    try {
        matrix = Matrix(rows, cols, initValue);
    } catch (const MatrixException& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    workspace[matName] = matrix;
    std::cout << "Matrix '" << matName << "' created:\n"
                  << "  Dimensions: " << rows << " x " << cols << std::endl;
    return true;
}

bool Workspace::listMatrices() const {
    if (workspace.empty()) {
        return false;
    }
    for (const auto& matrix : workspace) {
        std::cout << "Matrix '" << matrix.first << "':\n" << matrix.second << std::endl;
    }
    return true;
}

bool Workspace::showMatrix(const std::string& matName) const {
    return handleReadOnlyMatrixOp(matName, [&matName](const Matrix& m) {
        std::cout << "Matrix '" << matName << "':\n" << m << std::endl;
    });
}

bool Workspace::transposeMatrix(const std::string& matName) {
    return handleSingleMatrixOp(matName, [](Matrix& m) {
        m = m.transpose();
    });
}

bool Workspace::assignMatrix(const std::string& matName) {
    if (!matrixExists(matName)) return false;
    Matrix& assigned = workspace.at(matName);
    for (int i = 0; i < assigned.getRows(); ++i) {
        for (int j = 0; j < assigned.getCols(); ++j) {
            double value;
            std::string inputStr;
            std::cout << "Assign value for element in (" << i << ", " << j << ")\n";
            std::cout << "> ";
            std::getline(std::cin, inputStr);
            std::istringstream ss(inputStr);
            if (!(ss >> value) || !(ss.eof())) {
                std::cout << "Invalid value assignment" << std::endl;
                --j; // Retry the same element
                continue;
            }
            assigned(i, j) = value;
        }
    }
    return true;
}

bool Workspace::deleteMatrix(const std::string& matName) {
    if (!matrixExists(matName)) return false;
    workspace.erase(matName);
    std::cout << "Matrix '" << matName << "' deleted from workspace." << std::endl;
    return true;
}

bool Workspace::scalarMultiplyMatrix(const std::string& resultName, const std::string& matName, const double scalar)
{
    return handleSingleMatrixOp(matName, [this, &resultName, scalar](Matrix& m) {
        workspace[resultName] = m * scalar;
    });
}

bool Workspace::binaryMatrixOp(const std::string& resultName, const std::string& mat1Name, const std::string& mat2Name,
    const std::function<Matrix(const Matrix&, const Matrix&)>& op) {
    if (workspace.find(mat1Name) == workspace.end()) {
        std::cout << "Matrix '" << mat1Name << "' not found in workspace." << std::endl;
        return false;
    }
    if (workspace.find(mat2Name) == workspace.end()) {
        std::cout << "Matrix '" << mat2Name << "' not found in workspace." << std::endl;
        return false;
    }
    try {
        workspace[resultName] = op(workspace.at(mat1Name), workspace.at(mat2Name));
    } catch (const MatrixDimensionMismatch& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}

bool Workspace::addMatrices(const std::string& resultName,const std::string& mat1Name, const std::string& mat2Name) {
    return binaryMatrixOp(resultName, mat1Name, mat2Name,
        [](const Matrix& a, const Matrix& b) { return a + b; });
}

bool Workspace::subtractMatrices(const std::string& resultName, const std::string& mat1Name, const std::string& mat2Name) {
    return binaryMatrixOp(resultName, mat1Name, mat2Name,
        [](const Matrix& a, const Matrix& b) { return a - b; });
}

bool Workspace::multiplyMatrices(const std::string& resultName, const std::string& mat1Name, const std::string& mat2Name) {
    return binaryMatrixOp(resultName, mat1Name, mat2Name,
        [](const Matrix& a, const Matrix& b) { return a * b; });
}

bool Workspace::saveWorkspaceToFile(const std::string& filename) const {
    const std::string folder = "workspaces/";
    std::filesystem::create_directories(folder);

    std::ofstream ofs(folder + filename);
    if (!ofs.is_open()) {
        std::cout << "Could not open file for writing.\n";
        return false;
    }

    for (const auto& pair : workspace) {
        const std::string& name = pair.first;
        const Matrix& matrix = pair.second;
        ofs << name << " " << matrix.getRows() << " " << matrix.getCols() << "\n";

        for (int r = 0; r < matrix.getRows(); ++r) {
            for (int c = 0; c < matrix.getCols(); ++c) {
                ofs << matrix(r, c) << " ";
            }
            ofs << "\n";
        }

        ofs << "\n";
    }
    std::cout << "Workspace saved successfully as '" << folder + filename << "'.\n";
    return true;
}

bool Workspace::loadWorkspaceFromFile(const std::string& filename) {
    const std::string folder = "workspaces/";
    std::ifstream ifs(folder + filename);
    if (!ifs.is_open()) {
        std::cout << "Could not open workspace file '" << folder + filename << "'.\n";
        return false;
    }

    workspace.clear();

    std::string name;
    int rows, cols;
    while (ifs >> name >> rows >> cols) {
        Matrix matrix;
        try {
            matrix = Matrix(rows, cols);
        } catch (const MatrixException& e) {
            std::cout << e.what() << std::endl;
            return false;
        }

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (!(ifs >> matrix(r, c))) {
                    std::cout << "Failed to read value for matrix ' " << name <<" ' element at ("
                              << r << ", " << c << "). Please check the file format.\n";
                    workspace.clear();
                    return false;
                }
            }
        }

        workspace[name] = matrix;
    }

    std::cout << "Workspace loaded successfully from '" << folder + filename << "'.\n";
    return true;
}

bool Workspace::rankMatrix(const std::string& matName) const {
    return handleReadOnlyMatrixOp(matName, [&matName](const Matrix& m) {
        std::cout << "Rank of matrix '" << matName << "' is: " << m.rank() << std::endl;
    });
}

bool Workspace::determinantMatrix(const std::string& matName) const {
    return handleReadOnlyMatrixOp(matName, [&matName](const Matrix& m) {
        double det = m.determinant();
        std::cout << "Determinant of matrix '" << matName
                  << "' is: " << det << std::endl;
    });
}


bool Workspace::inverseMatrix(const std::string& resultName,
                              const std::string& matName)
{
    return handleSingleMatrixOp(matName, [this, &resultName](Matrix& m) {
        workspace[resultName] = m.inverse();
    });
}

bool Workspace::solveMatrix(const std::string& resultName, const std::string& A, const std::string& b) {
    if (!matrixExists(A)) return false;
    if (!matrixExists(b)) return false;
    SolveResult result;

    try {
        result = workspace.at(A).solve(workspace.at(b));
    } catch (const MatrixException& e) {
        std::cout << e.what() << std::endl;
        return false;
    }

    switch (result.status) {
        case SolveStatus::NoSolution:
            std::cout << "The system has no solution." << std::endl;
            return true;
        case SolveStatus::Infinite:
            std::cout << "The system has infinite solutions." << std::endl;
            return true;
        case SolveStatus::Unique:
            workspace[resultName] = result.x;
            std::cout << "The system has a unique solution, saved as '" << resultName << "'." << std::endl;
            return true;
        default:
            std::cout << "Unknown solve status." << std::endl;
            return false;
    }

}

bool Workspace::handleSingleMatrixOp(
    const std::string& matName,
    const std::function<void(Matrix&)>& op)
{
    if (!matrixExists(matName)) return false;
    try {
        op(workspace.at(matName));
    } catch (const MatrixException& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}

bool Workspace::handleReadOnlyMatrixOp(
    const std::string& matName,
    const std::function<void(const Matrix&)>& op) const
{
    if (!matrixExists(matName)) return false;
    try {
        op(workspace.at(matName));
    } catch (const MatrixException& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}



bool Workspace::rotate3DVector(const std::string& vecName,
                        double angleDegreesX,
                        double angleDegreesY,
                        double angleDegreesZ) {
    return handleSingleMatrixOp(vecName,[this, vecName,angleDegreesX, angleDegreesY, angleDegreesZ](Matrix& vec) {
            workspace[vecName] = vec.rotate3D(angleDegreesX, angleDegreesY, angleDegreesZ);
    });
}
