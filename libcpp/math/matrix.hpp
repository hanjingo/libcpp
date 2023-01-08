#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstdint>
#include <vector>
#include <functional>

namespace libcpp
{
namespace math
{

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
        create_();
    }

    Matrix(const int row_n, const int col_n) :
        row_n_{row_n},
        col_n_{col_n}
    {
        create_();
    }

    Matrix(const int row_n, const int col_n, T value) :
        row_n_{row_n},
        col_n_{col_n}
    {
        create_();

        copy_n_(value);
    }

    Matrix(const std::vector<std::vector<T>>& data) :
        row_n_{int(data.size())},
        col_n_{row_n_ > 0 ? int(data[0].size()) : 0}
    {
        create_();

        copy_from_(data);
    }

    Matrix(const Matrix& rhs) :
        row_n_{rhs.row_n_},
        col_n_{rhs.col_n_},
        data_{rhs.data_}
    {
    }

    ~Matrix()
    {
        clean_();
    }

    Matrix& operator=(const Matrix& rhs)
    {
        row_n_ = rhs.row_n_;
        col_n_ = rhs.col_n_;

        clean_();
        create_();
        copy_from_(rhs);
        return this;
    }

    inline T* operator[](const int row)
    {
        return data_[row];
    }

    inline friend bool operator==(const Matrix& a, const Matrix& b)
    {
        return a.col_n_ == b.col_n_ && a.row_n_ == b.row_n_ && a.data_ == b.data_;
    }

    inline friend bool operator!=(const Matrix& a, const Matrix& b)
    {
        return !(a == b);
    }

    inline MatrixIterator<T> begin()
    {
        return find(0, 0);
    }

    inline MatrixIterator<T> end()
    {
        return find(row_n_ - 1, col_n_ - 1);
    }

    inline MatrixIterator<T> find(const int row, const int col)
    {
        return MatrixIterator<T>(this, row, col);
    }

    inline T& at(const int row, const int col)
    {
        return data_[row][col];
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
        return data_;
    }

private:
    void clean_()
    {
        for (auto row = 0; row < row_n_; ++row) {
            delete []data_[row];
        }

        delete []data_;
    }

    void create_()
    {
        data_ = new T*[col_n_];
        for (auto row = 0; row < row_n_; ++row) {
            data_[row] = new T[col_n_];
        }
    }

    template<typename Container>
    void copy_from_(const Container& rhs)
    {
        for (int row = 0; row < row_n_; ++row) {
            for (int col = 0; col < col_n_; ++col) {
                data_[row][col] = rhs[row][col];
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

                data_[row][col] = value;
                n--;
            }
        }
    }

private:
    T** data_;
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
        return a.row_ <= b.row_ && a.col_ < b.col_;
    }

    inline friend bool operator<=(const MatrixIterator& a, const MatrixIterator& b)
    {
        return a.row_ <= b.row_ && a.col_ <= b.col_;
    }

    inline friend bool operator>(const MatrixIterator& a, const MatrixIterator& b)
    {
        return a.row_ >= b.row_ && a.col_ > b.col_;
    }

    inline friend bool operator>=(const MatrixIterator& a, const MatrixIterator& b)
    {
        return a.row_ >= b.row_ && a.col_ >= b.col_;
    }

    virtual void seek(const int row, const int col)
    {
        row_   = row;
        col_   = col;
    }

    virtual void seek(ptrdiff_t offset)
    {
        // default horizon seek
        int step    = offset < 0 ? -1 : 1;
        int try_col = col_;
        int try_row = row_;
        while (true) {
            while (true) {
                try_col = col_ + step;
                if (offset == 0 || try_col < 0 || try_col >= mat_->row_n()) {
                    return;
                }
                col_    = try_col;
                offset -= step;
            }

            try_row = row_ + step;
            if (offset == 0 || try_row < 0 || try_row >= mat_->row_n()) {
                return;
            }
            row_ = try_row;
            col_ = step > 0 ? 0 : mat_->col_n() - 1;
        }
    }

    inline std::pair<int, int> pos()
    {
        int row = row_;
        int col = col_;
        return std::make_pair<int, int>(std::move(row), std::move(col));
    }

private:
    Matrix<T>* mat_;
    int row_;
    int col_;
};

}
}

#endif
