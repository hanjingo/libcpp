#include <iostream>
#include <chrono>
#include <thread>

#include <libcpp/math/matrix.hpp>

int main(int argc, char* argv[])
{
    std::cout << "start test mat1>> " << std::endl;
    math::matrix::Matrix2<int> mat1{3, 3, 0};
    auto curr1 = mat1.Curr();
    std::cout << "curr.first=" << curr1.first << ", curr.second=" << curr1.second << std::endl;

    std::cout << "set[1][1]=" << mat1.Set(1, 1, 123) << std::endl;

    std::cout << "get[1][1]=" << mat1.Get(1, 1) << std::endl;

    curr1 = mat1.MoveTo(1, 1);
    std::cout << "movte to (" << curr1.first << ", " << curr1.second << ")" << std::endl;
    curr1 = mat1.Shift(1);
    std::cout << "horizon shift(1) with step=1 to(" << curr1.first << ", " << curr1.second << ")" << std::endl;

    curr1 = mat1.MoveTo(2, 2);
    std::cout << "movte to (" << curr1.first << ", " << curr1.second << ")" << std::endl;
    curr1 = mat1.Shift(1);
    std::cout << "horizon shift(1) with step=1 to(" << curr1.first << ", " << curr1.second << ")" << std::endl;

    curr1 = mat1.MoveTo(0, 0);
    std::cout << "movte to (" << curr1.first << ", " << curr1.second << ")" << std::endl;
    curr1 = mat1.Shift(-1);
    std::cout << "horizon shift(-1) with step=1 to(" << curr1.first << ", " << curr1.second << ")" << std::endl;

    std::cout << "\nstart test mat2>> " << std::endl;
    math::matrix::Matrix2<int> mat2({
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}});
    auto curr2 = mat2.Curr();
    std::cout << "curr.first=" << curr2.first << ", curr.second=" << curr2.second << std::endl;

    std::cout << "set[1][1]=" << mat2.Set(1, 1, 123) << std::endl;

    std::cout << "get[1][1]=" << mat1.Get(1, 1) << std::endl;

    curr2 = mat2.MoveTo({1, 1});
    std::cout << "movte to (" << curr2.first << ", " << curr2.second << ")" << std::endl;
    curr2 = mat2.Shift(1, 1, math::matrix::vertical_shift);
    std::cout << "vertical shift(1) with step=1 to(" << curr2.first << ", " << curr2.second << ")" << std::endl;

    curr2 = mat2.MoveTo(2, 2);
    std::cout << "movte to (" << curr2.first << ", " << curr2.second << ")" << std::endl;
    curr2 = mat2.Shift(1, 1, math::matrix::vertical_shift);
    std::cout << "vertical shift(1) with step=1 to(" << curr2.first << ", " << curr2.second << ")" << std::endl;

    curr2 = mat2.MoveTo(0, 0);
    std::cout << "movte to (" << curr2.first << ", " << curr2.second << ")" << std::endl;
    curr2 = mat2.Shift(-1, 1, math::matrix::vertical_shift);
    std::cout << "vertical shift(-1) with step=1 to(" << curr2.first << ", " << curr2.second << ")" << std::endl;

    std::cout << "\nstart test mat3>> " << std::endl;
    auto mat3 = mat2;
    std::cout << "get mat3[1][1]=" << mat1[1][1] << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(100));

    return 0;
}
