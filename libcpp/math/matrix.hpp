#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstdint>
#include <vector>
#include <functional>

#include <iostream>

namespace libcpp
{
namespace math
{

template <typename T>
class Matrix;

template <typename T>
struct MatrixIterator;

template <typename T>
struct MatrixVerticalIterator;

template <typename T>
class Matrix
{
public:
    Matrix() :
        row_n_{0},
        col_n_{0}
    {
        buf_ = create_();
    }

    Matrix(const int row_n, const int col_n) :
        row_n_{row_n},
        col_n_{col_n}
    {
        buf_ = create_();
    }

    Matrix(const int row_n, const int col_n, T value) :
        row_n_{row_n},
        col_n_{col_n}
    {
        buf_ = create_();

        copy_n_(value);
    }

    Matrix(const std::vector<std::vector<T>>& buf) :
        row_n_{int(buf.size())},
        col_n_{row_n_ > 0 ? int(buf[0].size()) : 0}
    {
        buf_ = create_();

        copy_from_(buf);
    }

    Matrix(const Matrix& rhs) :
        row_n_{rhs.row_n_},
        col_n_{rhs.col_n_},
        buf_{rhs.buf_}
    {
    }

    ~Matrix()
    {
        clean_(buf_);
    }

    Matrix& operator=(const Matrix& rhs)
    {
        clean_(buf_);
        buf_ = create_(rhs.row_n_, rhs.col_n_);
        copy_from_(rhs);

        row_n_ = rhs.row_n_;
        col_n_ = rhs.col_n_;
        return this;
    }

    inline T* operator[](const int row)
    {
        return buf_[row];
    }

    inline friend bool operator==(const Matrix& a, const Matrix& b)
    {
        return a.col_n_ == b.col_n_ && a.row_n_ == b.row_n_ && a.buf_ == b.buf_;
    }

    inline friend bool operator!=(const Matrix& a, const Matrix& b)
    {
        return !(a == b);
    }

    inline MatrixIterator<T> begin()
    {
        return find(0, 0);
    }

    inline MatrixVerticalIterator<T> vbegin()
    {
        return vfind(0, 0);
    }

    inline MatrixIterator<T> end()
    {
        return MatrixIterator<T>(this, row_n_, 0);
    }

    inline MatrixVerticalIterator<T> vend()
    {
        return MatrixVerticalIterator<T>(this, 0, col_n_);
    }

    inline MatrixIterator<T> find(const int row, const int col)
    {
        return MatrixIterator<T>(this, row, col);
    }

    inline MatrixVerticalIterator<T> vfind(const int row, const int col)
    {
        return MatrixVerticalIterator<T>(this, row, col);
    }

    inline T& at(const int row, const int col)
    {
        return buf_[row][col];
    }

    inline std::pair<int, int> resize(int row_n, int col_n)
    {
        auto old = buf_;
        buf_ = create_(row_n, col_n);
        copy_from_(old, row_n, col_n);
        clean_(old);

        row_n_ = row_n;
        col_n_ = col_n;
        return std::make_pair(std::move(row_n), std::move(col_n));
    }

    inline int64_t size()
    {
        return row_n_ * col_n_;
    }

    inline int row_n()
    {
        return row_n_;
    }

    inline int col_n()
    {
        return col_n_;
    }

    inline T** date()
    {
        return buf_;
    }

private:
    void clean_(T** buf)
    {
        for (auto row = 0; row < row_n_; ++row) {
            delete []buf[row];
        }

        delete []buf;
    }

    T** create_()
    {
        return create_(row_n_, col_n_);
    }

    T** create_(const int row_n, const int col_n)
    {
        T** bak = new T*[col_n];
        for (auto row = 0; row < row_n; ++row) {
            bak[row] = new T[col_n];
        }
        return bak;
    }

    template<typename Container>
    void copy_from_(const Container& rhs)
    {
        copy_from_(rhs, row_n_, col_n_);
    }

    template<typename Container>
    void copy_from_(const Container& rhs, int row_n, int col_n)
    {
        row_n = row_n_ < row_n ? row_n_ : row_n;
        col_n = col_n_ < col_n ? col_n_ : col_n;

        for (int row = 0; row < row_n; ++row) {
            for (int col = 0; col < col_n; ++col) {
                buf_[row][col] = rhs[row][col];
            }
        }
    }

    void copy_n_(const T value, int n = -1)
    {
        n = n > -1 && n <= size() ? n : size();
        for (int row = 0; row < row_n_; ++row) {
            for (int col = 0; col < col_n_; ++col) {
                if (n == 0) {
                    return;
                }

                buf_[row][col] = value;
                n--;
            }
        }
    }

private:
    T** buf_;
    int row_n_;
    int col_n_;
};

template <typename T>
struct MatrixIterator {
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = ptrdiff_t;
    using difference_type   = T;
    using pointer           = const T*;
    using reference         = T*;

    MatrixIterator()
        : mat_{0}, row_{0}, col_{0}
    {}
    MatrixIterator(const MatrixIterator& itr)
        : mat_{itr.mat_}, row_{itr.row_}, col_{itr.col_}
    {}
    explicit MatrixIterator(Matrix<T>* mat, const std::pair<int, int> pos)
        : mat_{mat}, row_{pos.first}, col_{pos.second}
    {}
    explicit MatrixIterator(Matrix<T>* mat, const int row, const int col)
        : mat_{mat}, row_{row}, col_{col}
    {}

    inline MatrixIterator& operator=(MatrixIterator& itr)
    {
        mat_   = itr.mat_;
        row_   = itr.row_;
        col_   = itr.col_;
        return *this;
    }

    inline T& operator*()
    {
        return mat_->at(row_, col_);
    }

    inline MatrixIterator& operator++() // ++itr
    {
        seek(1);
        return *this;
    }

    inline MatrixIterator<T>& operator++(int offset) // itr++
    {
        seek(1);
        return *this;
    }

    inline MatrixIterator& operator+=(ptrdiff_t offset) // itr += n
    {
        seek(offset);
        return *this;
    }

    inline MatrixIterator& operator--() // --itr
    {
        seek(-1);
        return *this;
    }

    inline MatrixIterator& operator--(int offset) // itr--
    {
        seek(-1);
        return *this;
    }

    inline MatrixIterator& operator-=(ptrdiff_t offset) // itr -= n
    {
        seek(-offset);
        return *this;
    }

    inline friend bool operator==(const MatrixIterator& a, const MatrixIterator& b)
    {
        return a.mat_ == b.mat_ && a.row_ == b.row_ && a.col_ == b.col_;
    }

    inline friend bool operator!=(const MatrixIterator& a, const MatrixIterator& b)
    {
        return !(a == b);
    }

    inline friend bool operator<(const MatrixIterator& a, const MatrixIterator& b)
    {
        return (a.row_ < b.row_) || (a.row_ == b.row_ && a.col_ < b.col_);
    }

    inline friend bool operator<=(const MatrixIterator& a, const MatrixIterator& b)
    {
        return (a.row_ < b.row_) || (a.row_ == b.row_ && a.col_ <= b.col_);
    }

    inline friend bool operator>(const MatrixIterator& a, const MatrixIterator& b)
    {
        return (a.row_ > b.row_) || (a.row_ == b.row_ && a.col_ > b.col_);
    }

    inline friend bool operator>=(const MatrixIterator& a, const MatrixIterator& b)
    {
        return (a.row_ > b.row_) || (a.row_ == b.row_ && a.col_ >= b.col_);
    }

    void seek(const int row, const int col)
    {
        row_   = row;
        col_   = col;
    }

    void seek(ptrdiff_t offset)
    {
        int step = offset < 0 ? -1 : 1;
        while (true) {
            while (true) {
                if (col_ + step > -1 && col_ + step < mat_->col_n()) {
                    col_   += step;
                    offset -= step;
                    if (offset == 0) {
                        return;
                    }
                    continue;
                }
                break;
            }

            if (row_ + step < 0 && row_ + step >= mat_->row_n()) {
                return;
            }
            row_   += step;
            col_    = step > 0 ? 0 : mat_->col_n() - 1;
            offset -= step;
            if (offset == 0) {
                return;
            }
        }
    }

    inline std::pair<int, int> pos()
    {
        int row = row_;
        int col = col_;
        return std::make_pair<int, int>(std::move(row), std::move(col));
    }

    inline int row()
    {
        return row_;
    }

    inline int col()
    {
        return col_;
    }

    inline Matrix<T>* matrix()
    {
        return mat_;
    }

private:
    Matrix<T>* mat_;
    int row_;
    int col_;
};

template <typename T>
struct MatrixVerticalIterator {
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = ptrdiff_t;
    using difference_type   = T;
    using pointer           = const T*;
    using reference         = T*;

    MatrixVerticalIterator()
        : mat_{0}, row_{0}, col_{0}
    {}
    MatrixVerticalIterator(const MatrixVerticalIterator& itr)
        : mat_{itr.mat_}, row_{itr.row_}, col_{itr.col_}
    {}
    explicit MatrixVerticalIterator(Matrix<T>* mat, const std::pair<int, int> pos)
        : mat_{mat}, row_{pos.first}, col_{pos.second}
    {}
    explicit MatrixVerticalIterator(Matrix<T>* mat, const int row, const int col)
        : mat_{mat}, row_{row}, col_{col}
    {}

    inline MatrixVerticalIterator& operator=(MatrixVerticalIterator& itr)
    {
        mat_   = itr.mat_;
        row_   = itr.row_;
        col_   = itr.col_;
        return *this;
    }

    inline T& operator*()
    {
        return mat_->at(row_, col_);
    }

    inline MatrixVerticalIterator& operator++() // ++itr
    {
        seek(1);
        return *this;
    }

    inline MatrixVerticalIterator<T>& operator++(int offset) // itr++
    {
        seek(1);
        return *this;
    }

    inline MatrixVerticalIterator& operator+=(ptrdiff_t offset) // itr += n
    {
        seek(offset);
        return *this;
    }

    inline MatrixVerticalIterator& operator--() // --itr
    {
        seek(-1);
        return *this;
    }

    inline MatrixVerticalIterator& operator--(int offset) // itr--
    {
        seek(-1);
        return *this;
    }

    inline MatrixVerticalIterator& operator-=(ptrdiff_t offset) // itr -= n
    {
        seek(-offset);
        return *this;
    }

    inline friend bool operator==(const MatrixVerticalIterator& a, const MatrixVerticalIterator& b)
    {
        return a.mat_ == b.mat_ && a.row_ == b.row_ && a.col_ == b.col_;
    }

    inline friend bool operator!=(const MatrixVerticalIterator& a, const MatrixVerticalIterator& b)
    {
        return !(a == b);
    }

    inline friend bool operator<(const MatrixVerticalIterator& a, const MatrixVerticalIterator& b)
    {
        return (a.col_ < b.col_) || (a.col_ == b.col_ && a.row_ < b.row_);
    }

    inline friend bool operator<=(const MatrixVerticalIterator& a, const MatrixVerticalIterator& b)
    {
        return (a.col_ < b.col_) || (a.col_ == b.col_ && a.row_ <= b.row_);
    }

    inline friend bool operator>(const MatrixVerticalIterator& a, const MatrixVerticalIterator& b)
    {
        return (a.col_ > b.col_) || (a.col_ == b.col_ && a.row_ > b.row_);
    }

    inline friend bool operator>=(const MatrixVerticalIterator& a, const MatrixVerticalIterator& b)
    {
        return (a.col_ > b.col_) || (a.col_ == b.col_ && a.row_ >= b.row_);
    }

    void seek(const int row, const int col)
    {
        row_   = row;
        col_   = col;
    }

    void seek(ptrdiff_t offset)
    {
        int step = offset < 0 ? -1 : 1;
        while (true) {
            while (true) {
                if (row_ + step > -1 && row_ + step < mat_->row_n()) {
                    row_   += step;
                    offset -= step;
                    if (offset == 0) {
                        return;
                    }
                    continue;
                }
                break;
            }

            if (col_ + step < 0 && col_ + step >= mat_->col_n()) {
                return;
            }
            col_   += step;
            row_    = step > 0 ? 0 : mat_->row_n() - 1;
            offset -= step;
            if (offset == 0) {
                return;
            }
        }
    }

    inline std::pair<int, int> pos()
    {
        int row = row_;
        int col = col_;
        return std::make_pair<int, int>(std::move(row), std::move(col));
    }

    inline int row()
    {
        return row_;
    }

    inline int col()
    {
        return col_;
    }

    inline Matrix<T>* matrix()
    {
        return mat_;
    }

private:
    Matrix<T>* mat_;
    int row_;
    int col_;
};

template <typename T>
ptrdiff_t distance(MatrixIterator<T>& a, MatrixIterator<T>& b)
{
    if (a.matrix() != b.matrix()) {
        return 0;
    }

    auto row_n = a.matrix()->row_n();
    auto col_n = a.matrix()->col_n();

    auto offset = a.row() == b.row() ? 0 : (b.row() - a.row() - 1) * col_n;
    if (a < b) {
        offset += (col_n - a.col() + b.col());
    } else {
        offset -= (col_n - b.col() + a.col());
    }
    return offset;
}

template <typename T>
ptrdiff_t distance(MatrixVerticalIterator<T>& a, MatrixVerticalIterator<T>& b)
{
    if (a.matrix() != b.matrix()) {
        return 0;
    }

    auto row_n = a.matrix()->row_n();
    auto col_n = a.matrix()->col_n();

    auto offset = a.col() == b.col() ? 0 : (b.col() - a.col() - 1) * row_n;
    if (a < b) {
        offset += (row_n - a.row() + b.row());
    } else {
        offset -= (row_n - b.row() + a.row());
    }
    return offset;
}

}
}

#endif
