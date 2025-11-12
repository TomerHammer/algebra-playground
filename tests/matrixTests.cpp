#include <cassert>
#include <iostream>
#include <cmath>
#include <sstream>
#include "../include/Matrix.h"
#include "../include/MatrixException.h"

// Example test function
void testMatrixInitializationCustom() {
    Matrix mat(5, 5, 5.0);
    for (int i = 0; i < mat.getRows(); ++i) {
        for (int j = 0; j < mat.getCols(); ++j) {
            assert(mat(i, j) == 5.0);
        }
    }

    std::cout << "✅ testMatrixInitialization passed!" << std::endl;
}

void testMatrixInitializationDefault() {
    Matrix mat(3,2);
    for (int i = 0; i < mat.getRows(); ++i) {
        for (int j = 0; j < mat.getCols(); ++j) {
            assert(mat(i, j) == 0.0); // Default value for double is 0.0
        }
    }
    std::cout << "✅ testMatrixInitializationDefault passed!" << std::endl;
}

void testMatrixCopy() {
    Matrix mat1(2,2,3.0);
    Matrix mat2 = mat1; // Copy constructor
    for (int i = 0; i < mat2.getRows(); ++i) {
        for (int j = 0; j < mat2.getCols(); ++j) {
            assert(mat2(i, j) == 3.0);
        }
    }
    std::cout << "✅ testMatrixCopy passed!" << std::endl;
}

void testMatrixAssignmentOperator() {
    Matrix mat1(2,2,3.0);
    Matrix mat2(2,2); // Default value 0.0
    mat2 = mat1; // Assignment operator
    for (int i = 0; i < mat2.getRows(); ++i) {
        for (int j = 0; j < mat2.getCols(); ++j) {
            assert(mat2(i, j) == 3.0);
        }
    }
    std::cout << "✅ testMatrixAssignmentOperator passed!" << std::endl;
}

void testMatrixComparisonOperators() {
    Matrix mat1(2,2,3);
    Matrix mat2(2,2,3);
    Matrix mat3(2,2,4);
    assert(mat1 == mat2);
    assert(mat2 != mat3);
    assert(mat3 != mat1);
    std::cout << "✅ testMatrixComparisonOperators passed!" << std::endl;
}

void testMatrixAccessOperator() {
    Matrix mat(3,3);
    for (int i = 0; i < mat.getRows(); ++i) {
        for (int j = 0; j < mat.getCols(); ++j) {
            mat(i, j) = 5;
        }
    }

    for (int i = 0; i < mat.getRows(); ++i) {
        for (int j = 0; j < mat.getCols(); ++j) {
            assert(mat(i, j) == 5);
        }
    }
    std::cout << "✅ testMatrixAccessOperatorPassed!" << std::endl;
}

void testOutOfBoundsAccess() {
    Matrix mat(3,3);
    try {
        mat(3,0) = 5; // Out of bounds
        assert(false); // Should not reach here
    } catch (const std::exception&) {}


    try {
        double val = mat(0,3); // Out of bounds
        (void)val; // To avoid unused variable warning
        assert(false); // Should not reach here
    } catch (const std::exception&) {}

    std::cout << "✅ testOutOfBoundsAccess passed!" << std::endl;
}

void testDimensionMismatchMinusOperator() {
    Matrix mat1(2,3);
    Matrix mat2(3,2);
    try {
        mat1 - mat2; // Should throw exception
        assert(false); // Should not reach here
    } catch (const std::exception&) {}

    std::cout << "✅ testDimensionMismatchMinusOperator passed!" << std::endl;
}

void testDimensionMismatchAdditionOperator() {
    Matrix mat1(2,3);
    Matrix mat2(3,2);
    try {
        mat1 + mat2; // Should throw exception
        assert(false); // Should not reach here
    } catch (const std::exception&) {}

    std::cout << "✅ testDimensionMismatchAdditionOperator passed!" << std::endl;
}

void testInPlaceScalarMultiplication() {
    Matrix mat(2,2,2);
    mat *= 3; // In-place scalar multiplication
    mat = mat * 2; // To ensure the operator works correctly
    for (int i = 0; i < mat.getRows(); ++i) {
        for (int j = 0; j < mat.getCols(); ++j) {
            assert(mat(i, j) == 12); // 2 * 3 * 2 = 12
        }
    }
    std::cout << "✅ testInPlaceScalarMultiplication passed!" << std::endl;
}

void testScalarMultiplication() {
    Matrix mat(2,2,2);
    Matrix result = Matrix(3.5 * mat);// Scalar multiplication
    for (int i = 0; i < result.getRows(); ++i) {
        for (int j = 0; j < result.getCols(); ++j) {
            assert(result(i, j) == 7); // 2 * 3.5 = 7
        }
    }
    std::cout << "✅ testScalarMultiplication passed!" << std::endl;
}

void testMatrixInvalidInitialization() {
    try {
        Matrix mat(0, 3); // Invalid initialization
        assert(false); // Should not reach here
    } catch (const std::exception&) {}

    try {
        Matrix mat(-1, 3); // Invalid initialization
        assert(false); // Should not reach here
    } catch (const std::exception&) {}

    try {
        Matrix mat(3, 0); // Invalid initialization
        assert(false); // Should not reach here
    } catch (const std::exception&) {}

    try {
        Matrix mat(3, -1); // Invalid initialization
        assert(false); // Should not reach here
    } catch (const std::exception&) {}

    std::cout << "✅ testMatrixInvalidInitialization passed!" << std::endl;
}

void testMatrixMultiplicationDimensionMismatch() {
    Matrix mat1(2,3);
    Matrix mat2(4,2);
    try {
        mat1 * mat2; // Should throw exception
        assert(false); // Should not reach here
    } catch (const std::exception& e) {}
    std::cout << "✅ testMatrixMultiplicationDimensionMismatch passed!" << std::endl;
}

void testMatrixMultiplication() {
    Matrix mat1(2,3);
    Matrix mat2(3,2);
    // Initialize mat1
    mat1(0,0) = 1; mat1(0,1) = 2; mat1(0,2) = 3;
    mat1(1,0) = 4; mat1(1,1) = 5; mat1(1,2) = 6;
    // Initialize mat2
    mat2(0,0) = 7;  mat2(0,1) = 8;
    mat2(1,0) = 9;  mat2(1,1) = 10;
    mat2(2,0) = 11; mat2(2,1) = 12;

    Matrix result = mat1 * mat2; // Matrix multiplication
    assert(result.getRows() == 2);
    assert(result.getCols() == 2);
    assert(result(0,0) == 58); // 1*7 + 2*9 + 3*11
    assert(result(0,1) == 64); // 1*8 + 2*10 + 3*12
    assert(result(1,0) == 139); // 4*7 + 5*9 + 6*11
    assert(result(1,1) == 154); // 4*8 + 5*10 + 6*12

    std::cout << "✅ testMatrixMultiplication passed!" << std::endl;
}

void testDeterminant() {
    Matrix mat(2,2);
    mat(0,0) = 4; mat(0,1) = 7;
    mat(1,0) = 2; mat(1,1) = 6;
    double det = mat.determinant();
    // Determinant should be 10.0
    assert(std::abs(det - 10.0) < 1e-9);

    // also test operator== via copy
    Matrix matCopy = mat;
    assert(matCopy == mat);
    assert(!(matCopy != mat));

    // Singular matrix determinant == 0
    Matrix sing(2,2);
    sing(0,0) = 1; sing(0,1) = 2;
    sing(1,0) = 2; sing(1,1) = 4;
    assert(std::abs(sing.determinant() - 0.0) < 1e-9);

    std::cout << "✅ testDeterminant passed!" << std::endl;
}

void testRank() {
    Matrix mat(2,2);
    mat(0,0) = 1; mat(0,1) = 2;
    mat(1,0) = 2; mat(1,1) = 4; // second row is multiple -> rank 1
    assert(mat.rank() == 1);

    Matrix mat2(3,3,0.0);
    // full rank identity-like
    mat2(0,0) = 1; mat2(1,1) = 1; mat2(2,2) = 1;
    assert(mat2.rank() == 3);

    // operator== check with explicit constructed expected
    Matrix expected(3,3,0.0);
    expected(0,0)=1; expected(1,1)=1; expected(2,2)=1;
    assert(mat2 == expected);

    std::cout << "✅ testRank passed!" << std::endl;
}

// New test: the specific matrix reported by the user (should have rank 2)
void testRankExampleProvided() {
    Matrix A(3,3);
    A(0,0)=0; A(0,1)=1; A(0,2)=9;
    A(1,0)=0; A(1,1)=1; A(1,2)=7;
    A(2,0)=0; A(2,1)=2; A(2,2)=5;
    assert(A.rank() == 2);
    std::cout << "✅ testRankExampleProvided passed!" << std::endl;
}

void testIdentityAndInverse() {
    Matrix id = Matrix::identity(3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == j) assert(id(i,j) == 1.0);
            else assert(id(i,j) == 0.0);
        }
    }

    // operator==/!= assertions for identity
    Matrix id2 = Matrix::identity(3);
    assert(id == id2);
    assert(!(id != id2));

    // Inverse of known 2x2
    Matrix a(2,2);
    a(0,0)=4; a(0,1)=7; a(1,0)=2; a(1,1)=6;
    Matrix inv = a.inverse();
    // Check a * inv == identity (within tolerance)
    Matrix prod = a * inv;
    for (int i = 0; i < prod.getRows(); ++i) {
        for (int j = 0; j < prod.getCols(); ++j) {
            if (i == j) assert(std::abs(prod(i,j) - 1.0) < 1e-9);
            else assert(std::abs(prod(i,j) - 0.0) < 1e-9);
        }
    }

    // inverse of singular matrix should throw
    Matrix s(2,2);
    s(0,0)=1; s(0,1)=2; s(1,0)=2; s(1,1)=4;
    try {
        Matrix news = s.inverse();
        assert(false);
    } catch (const std::exception& e) {}

    std::cout << "✅ testIdentityAndInverse passed!" << std::endl;
}

void testSolveUniqueNoAndInfinite() {
    // Unique solution example
    Matrix A(2,2);
    A(0,0)=2; A(0,1)=1; A(1,0)=1; A(1,1)=1;
    Matrix b(2,1);
    b(0,0)=1; b(1,0)=1;
    SolveResult res = A.solve(b);
    assert(res.status == SolveStatus::Unique);
    // expected solution [0,1]
    assert(std::abs(res.x(0,0) - 0.0) < 1e-9);
    assert(std::abs(res.x(1,0) - 1.0) < 1e-9);

    // No solution example
    Matrix B(2,2);
    B(0,0)=1; B(0,1)=1; B(1,0)=1; B(1,1)=1;
    Matrix c(2,1);
    c(0,0)=1; c(1,0)=2;
    SolveResult res2 = B.solve(c);
    assert(res2.status == SolveStatus::NoSolution);

    // Infinite solutions example
    Matrix C(2,2);
    C(0,0)=1; C(0,1)=1; C(1,0)=2; C(1,1)=2;
    Matrix d(2,1);
    d(0,0)=2; d(1,0)=4;
    SolveResult res3 = C.solve(d);
    assert(res3.status == SolveStatus::Infinite);

    std::cout << "✅ testSolveUniqueNoAndInfinite passed!" << std::endl;
}

// New: operator-related assertions
void testOperators() {
    // operator<< basic sanity check (check substrings)
    Matrix m(2,2);
    m(0,0)=1; m(0,1)=2; m(1,0)=3; m(1,1)=4;
    std::ostringstream oss;
    oss << m;
    std::string out = oss.str();
    // Expect each row wrapped in | and numbers present
    assert(out.find("|") != std::string::npos);
    assert(out.find("1") != std::string::npos);
    assert(out.find("2") != std::string::npos);
    assert(out.find("3") != std::string::npos);
    assert(out.find("4") != std::string::npos);

    // gaussianElimination basic forward-elimination check
    Matrix g(2,2);
    g(0,0)=2; g(0,1)=1; g(1,0)=4; g(1,1)=3;
    int swaps = 0;
    Matrix ge = g.gaussianElimination(nullptr, true, &swaps);
    // After eliminating below pivot, ge(1,0) should be exactly 0 and ge(1,1)==1
    assert(std::abs(ge(1,0) - 0.0) < 1e-9);
    assert(std::abs(ge(1,1) - 1.0) < 1e-9);

    std::cout << "✅ testOperators passed!" << std::endl;
}

void testVectorRotation(){
    Matrix rot(3,1);
    rot(0,0) = 1; rot(1,0) = 0; rot(2,0) = 0; // x-axis unit vector
    Matrix rotated = rot.rotate3D(0,0,90); // Rotate 90 degrees around Z-axis
    // Expected result is (0,1,0)
    assert(std::abs(rotated(0,0) - 0.0) == 0.0);
    assert(std::abs(rotated(1,0) - 1.0) == 0.0);
    assert(std::abs(rotated(2,0) - 0.0) == 0.0);

    rot(0,0) = 3; rot(1,0) = 1; rot(2,0) = 2; // A vector
    rotated = rot.rotate3D(90,0,0); // Rotate 90 degrees around X-axis
    // Expected result is (3,-2,1)
    assert(std::abs(rotated(0,0) - 3.0) == 0.0);
    assert(std::abs(rotated(1,0) - -2.0) == 0.0);
    assert(std::abs(rotated(2,0) - 1.0) == 0.0);

    std::cout << "✅ test3DRotation passed!" << std::endl;
}

void testE2E() {
    Matrix mat1(2,2,1.0);
    Matrix mat2(2,2,1.0);
    Matrix mat3(2,2,2.0);
    assert(mat1 + mat2 == mat3);
    assert(mat3 - mat2 == mat1);
    mat1 *= 3.0;
    assert(mat1(0,0) == 3.0 && mat1(1,1) == 3.0);
    Matrix mat4 = mat1 * mat3; // (3*2) = 6
    assert(mat4(0,0) == 12.0 && mat4(1,1) == 12.0);
    Matrix mat5 = mat4 * 0.5; // (12*0.5) = 6.0
    assert(mat5(0,0) == 6.0 && mat5(1,1) == 6.0);
    assert(mat5 != mat1);
    mat5(1,0) = 3.0;
    Matrix mat6 = mat5.transpose();
    Matrix mat7 = -mat5;
    assert(mat7 == -mat5);
    assert(mat6.transpose() == mat5);
    mat6(0,1) = 10.0; mat6(1,1) = 11.0;
    mat6 -= mat5;
    assert(mat6(0,0) == 0.0 && mat6(0,1) == 4.0);
    mat6 += mat5;
    assert(mat6(0,0) == 6.0 && mat6(0,1) == 10.0);
    mat6 *= mat5;
    // No specific assert for multiplication result, just print
    Matrix mat8(2,4,1.0);
    Matrix mat9(4,10,3.0);
    Matrix mat10 = mat8 * mat9;
    assert(mat10.getRows() == 2 && mat10.getCols() == 10);
    for (int i = 0; i < mat10.getRows(); ++i) {
        for (int j = 0; j < mat10.getCols(); ++j) {
            assert(mat10(i, j) == 12.0); // Each element should be 1*3 + 1*3 + 1*3 + 1*3 = 12
        }
    }
    std::cout << "✅ testE2E passed!" << std::endl;
}

int main() {
    testMatrixInitializationCustom();
    testMatrixInitializationDefault();
    testMatrixCopy();
    testMatrixAssignmentOperator();
    testMatrixComparisonOperators();
    testMatrixAccessOperator();
    testOutOfBoundsAccess();
    testDimensionMismatchMinusOperator();
    testDimensionMismatchAdditionOperator();
    testInPlaceScalarMultiplication();
    testScalarMultiplication();
    testMatrixInvalidInitialization();
    testMatrixMultiplicationDimensionMismatch();
    testMatrixMultiplication();
    testDeterminant();
    testRank();
    testRankExampleProvided();
    testIdentityAndInverse();
    testSolveUniqueNoAndInfinite();
    testOperators();
    testVectorRotation();
    testE2E();
    return 0;
}
