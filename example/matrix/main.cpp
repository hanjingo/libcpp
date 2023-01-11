#include <iostream>
#include <chrono>
#include <thread>

#include <libcpp/math/matrix.hpp>

using namespace libcpp::math;

int main(int argc, char* argv[])
{
    std::cout << "Test Matrix(const std::vector<std::vector<T>>& data)>>" << std::endl;
    Matrix<int> mat{{
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        }};
    for (auto itr = mat.begin(); itr != mat.end(); ++itr) {
        std::cout << *itr << ", ";
        if (itr.pos().second == (mat.col_n() - 1)) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;

    auto itr = mat.begin();
    std::cout << "mat.begin=" << *itr << std::endl;
    std::cout << "mat[0][0]=" << mat[0][0] << std::endl;

    itr = mat.end();
    std::cout << "mat.end(" << itr.pos().first << ", " << itr.pos().second << ")" << std::endl;

    itr = mat.find(1, 1);
    std::cout << "mat.find(1, 1)=" << *itr << std::endl;
    std::cout << "mat[1][1]=" << mat[1][1] << std::endl;

    std::cout << "mat.at(1, 2)=" << mat.at(1, 2) << std::endl;
    mat.at(1, 2) = 1;
    std::cout << "mat.at(1, 2)=" << mat.at(1, 2) << std::endl;

    std::cout << "mat.size()=" << mat.size() << std::endl;

    std::cout << "mat.row_n()=" << mat.row_n() << std::endl;

    std::cout << "mat.col_n()=" << mat.col_n() << std::endl;

    auto data = mat.date();
    std::cout << "mat.data[1][1]=" << data[1][1] << std::endl;

    ///////////////////////////////////////////////////////////////////

    std::cout << "\nTest Matrix(const int row_n, const int col_n, T value)>>" << std::endl;
    Matrix<int> mat1{3, 3, 0};
    int n = 0;
    for (auto row = 0; row < mat1.row_n(); ++row) {
        for (auto col = 0; col < mat1.col_n(); ++col) {
            n++;
            mat1.at(row, col) = n;
        }
    }
    for (auto itr = mat1.begin(); itr != mat1.end(); ++itr) {
        std::cout << *itr << ", ";
        if (itr.pos().second == (mat1.col_n() - 1)) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;

    auto itr1 = mat1.begin();
    std::cout << "mat1.begin=" << *itr1 << std::endl;
    std::cout << "mat1[0][0]=" << mat1[0][0] << std::endl;

    itr1 = mat1.end();
    std::cout << "mat1.end(" << itr.pos().first << ", " << itr.pos().second << ")" << std::endl;

    itr1 = mat1.find(1, 1);
    std::cout << "mat1.find(1, 1)=" << *itr1 << std::endl;
    std::cout << "mat1[1][1]=" << mat1[1][1] << std::endl;

    std::cout << "mat1.at(1, 2)=" << mat1.at(1, 2) << std::endl;

    std::cout << "mat1.size()=" << mat1.size() << std::endl;

    std::cout << "mat1.row_n()=" << mat1.row_n() << std::endl;

    std::cout << "mat1.col_n()=" << mat1.col_n() << std::endl;

    auto data1 = mat1.date();
    std::cout << "mat1.data[1][1]=" << data1[1][1] << std::endl;

    std::cout << "mat == mat ? " << (mat == mat) << std::endl;
    std::cout << "mat == mat1 ? " << (mat == mat1) << std::endl;
    std::cout << "mat1 == mat1 ? " << (mat1 == mat1) << std::endl;

    ///////////////////////////////////////////////////////////////////

    std::cout << "\nTest MatrixIterator>>" << std::endl;
    Matrix<int> mat2{{
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        }};
    for (auto itr = mat2.begin(); itr != mat2.end(); ++itr) {
        std::cout << *itr << ", ";
        if (itr.pos().second == (mat2.col_n() - 1)) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;

    auto itr2 = mat2.begin();
    std::cout << "*itr2 =" << *itr2 << std::endl;
    std::cout << "*(++itr2) =" << *(++itr2) << std::endl;
    std::cout << "*(itr2++) =" << *(itr2++) << std::endl;
    std::cout << "*(itr2+=3) =" << *(itr2 += 3) << std::endl;
    std::cout << "*(itr2-=3) =" << *(itr2 -= 3) << std::endl;
    std::cout << "*(--itr2) =" << *(--itr2) << std::endl;
    std::cout << "*(itr2--) =" << *(itr2--) << std::endl;

    itr2 = mat2.begin();
    std::cout << "*itr2 = mat2.begin()" << std::endl;
    std::cout << "itr2 == mat2.begin() ? " << (itr2 == mat2.begin()) << std::endl;
    std::cout << "itr2 == mat2.end() ? " << (itr2 == mat2.end()) << std::endl;
    std::cout << "itr2 < mat2.begin() ? " << (itr2 < mat2.begin()) << std::endl;
    std::cout << "itr2 < mat2.find(1, 1) ? " << (itr2 < mat2.find(1, 1)) << std::endl;
    std::cout << "itr2 < mat2.end() ? " << (itr2 < mat2.end()) << std::endl;
    std::cout << "itr2 <= mat2.begin() ? " << (itr2 <= mat2.begin()) << std::endl;
    std::cout << "itr2 <= mat2.find(1, 1) ? " << (itr2 <= mat2.find(1, 1)) << std::endl;
    std::cout << "itr2 <= mat2.end() ? " << (itr2 <= mat2.end()) << std::endl;

    itr2 = mat2.end();
    std::cout << "*itr2 = mat2.end()" << std::endl;
    std::cout << "itr2 > mat2.begin() ? " << (itr2 > mat2.begin()) << std::endl;
    std::cout << "itr2 > mat2.find(1, 1) ? " << (itr2 > mat2.find(1, 1)) << std::endl;
    std::cout << "itr2 > mat2.end() ? " << (itr2 > mat2.end()) << std::endl;
    std::cout << "itr2 >= mat2.begin() ? " << (itr2 >= mat2.begin()) << std::endl;
    std::cout << "itr2 >= mat2.find(1, 1) ? " << (itr2 >= mat2.find(1, 1)) << std::endl;
    std::cout << "itr2 >= mat2.end() ? " << (itr2 >= mat2.end()) << std::endl;

    ///////////////////////////////////////////////////////////////////

    std::cout << "\nTest MatrixVerticalIterator>>" << std::endl;
    Matrix<int> mat3{{
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        }};
    for (auto itr = mat3.begin(); itr != mat3.end(); ++itr) {
        std::cout << *itr << ", ";
        if (itr.pos().second == (mat3.col_n() - 1)) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;

    auto itr3 = mat3.begin();
    std::cout << "*itr3 =" << *itr3 << std::endl;
    std::cout << "*(++itr3) =" << *(++itr3) << std::endl;
    std::cout << "*(itr3++) =" << *(itr3++) << std::endl;
    std::cout << "*(itr3+=3) =" << *(itr3 += 3) << std::endl;
    std::cout << "*(itr3-=3) =" << *(itr3 -= 3) << std::endl;
    std::cout << "*(--itr3) =" << *(--itr3) << std::endl;
    std::cout << "*(itr3--) =" << *(itr3--) << std::endl;

    itr3 = mat3.begin();
    std::cout << "*itr3 = mat3.begin()" << std::endl;
    std::cout << "itr3 == mat3.begin() ? " << (itr3 == mat3.begin()) << std::endl;
    std::cout << "itr3 == mat3.end() ? " << (itr3 == mat3.end()) << std::endl;
    std::cout << "itr3 < mat3.begin() ? " << (itr3 < mat3.begin()) << std::endl;
    std::cout << "itr3 < mat3.find(1, 1) ? " << (itr3 < mat3.find(1, 1)) << std::endl;
    std::cout << "itr3 < mat3.end() ? " << (itr3 < mat3.end()) << std::endl;
    std::cout << "itr3 <= mat3.begin() ? " << (itr3 <= mat3.begin()) << std::endl;
    std::cout << "itr3 <= mat3.find(1, 1) ? " << (itr3 <= mat3.find(1, 1)) << std::endl;
    std::cout << "itr3 <= mat3.end() ? " << (itr3 <= mat3.end()) << std::endl;

    itr3 = mat3.end();
    std::cout << "*itr3 = mat3.end()" << std::endl;
    std::cout << "itr3 > mat3.begin() ? " << (itr3 > mat3.begin()) << std::endl;
    std::cout << "itr3 > mat3.find(1, 1) ? " << (itr3 > mat3.find(1, 1)) << std::endl;
    std::cout << "itr3 > mat3.end() ? " << (itr3 > mat3.end()) << std::endl;
    std::cout << "itr3 >= mat3.begin() ? " << (itr3 >= mat3.begin()) << std::endl;
    std::cout << "itr3 >= mat3.find(1, 1) ? " << (itr3 >= mat3.find(1, 1)) << std::endl;
    std::cout << "itr3 >= mat3.end() ? " << (itr3 >= mat3.end()) << std::endl;


    ///////////////////////////////////////////////////////////////////

    std::cout << "\nTest Matrix Algorithm>>" << std::endl;
    Matrix<int> mat5{{
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
            {0, 0, 0}
        }};
    for (auto itr = mat5.begin(); itr != mat5.end(); ++itr) {
        std::cout << *itr << ", ";
        if (itr.pos().second == (mat5.col_n() - 1)) {
            std::cout << std::endl;
        }
    }

    std::cout << "distance(mat5.begin(), mat5.end()) = "
              << distance(mat5.begin(), mat5.end()) << std::endl;
    std::cout << "distance(mat5.find(1, 1), mat5.find(2, 2)) = "
              << distance(mat5.find(1, 1), mat5.find(2, 2)) << std::endl;
    std::cout << "distance(mat5.find(2, 2), mat5.end()) = "
              << distance(mat5.find(2, 2), mat5.end()) << std::endl;
    std::cout << "distance(mat5.begin(), mat5.find(2, 2)) = "
              << distance(mat5.begin(), mat5.find(2, 2)) << std::endl;

    std::cout << "distance(mat5.vbegin(), mat5.vend()) = "
              << distance(mat5.vbegin(), mat5.vend()) << std::endl;
    std::cout << "distance(mat5.vfind(1, 1), mat5.vfind(2, 2)) = "
              << distance(mat5.vfind(1, 1), mat5.vfind(2, 2)) << std::endl;
    std::cout << "distance(mat5.vfind(2, 2), mat5.vend()) = "
              << distance(mat5.vfind(2, 2), mat5.vend()) << std::endl;
    std::cout << "distance(mat5.vbegin(), mat5.vfind(2, 2)) = "
              << distance(mat5.vbegin(), mat5.vfind(2, 2)) << std::endl;


    std::cin.get();
    return 0;
}
