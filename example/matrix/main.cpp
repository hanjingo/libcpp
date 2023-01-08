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
    auto itr = mat.begin();
    std::cout << "mat.begin=" << *itr << std::endl;
    std::cout << "mat[0][0]=" << mat[0][0] << std::endl;

    itr = mat.end();
    std::cout << "mat.end=" << *itr << std::endl;
    std::cout << "mat[2][2]=" << mat[2][2] << std::endl;

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
    auto itr1 = mat1.begin();
    std::cout << "mat.begin=" << *itr1 << std::endl;
    std::cout << "mat[0][0]=" << mat1[0][0] << std::endl;

    itr1 = mat1.end();
    std::cout << "mat1.end=" << *itr1 << std::endl;
    std::cout << "mat1[2][2]=" << mat1[2][2] << std::endl;

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
    Matrix<int> mat3{{
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        }};
    auto itr3 = mat3.begin();
    std::cout << "*itr3 =" << *itr3 << std::endl;
    std::cout << "*(++itr3) =" << *(++itr3) << std::endl;
    std::cout << "*(itr++3) =" << *(itr3++) << std::endl;
    std::cout << "*(itr3+=3) =" << *(itr3 += 3) << std::endl;
    std::cout << "*(itr3-=3) =" << *(itr3 -= 3) << std::endl;
    std::cout << "*(--itr3) =" << *(--itr3) << std::endl;
    std::cout << "*(itr3--) =" << *(itr3--) << std::endl;

    itr3 = mat3.begin();
    std::cout << "*itr3 =" << *itr3 << std::endl;
    std::cout << "itr3 == mat3.begin() ? " << (itr3 == mat3.begin()) << std::endl;

    std::cin.get();
    return 0;
}
