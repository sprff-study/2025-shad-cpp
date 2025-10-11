#pragma once

#include <vector>

class Matrix {
public:
    Matrix(int n, int m) : data_(n, std::vector<double>(m, 0.0)) {
    }
    explicit Matrix(int n) : Matrix(n, n) {
    }
    Matrix(std::vector<std::vector<double>> data) : data_(data) {
    }

    size_t Rows() const {
        return data_.size();
    }
    size_t Columns() const {
        if (data_.empty()) {
            return 0;
        }
        return data_[0].size();
    }

    double& operator[](int i, int j) {
        return data_[i][j];
    }
    double operator[](int i, int j) const {
        return data_[i][j];
    }

    Matrix& operator+=(const Matrix& rhs) {
        for (size_t i = 0; i < data_.size(); ++i) {
            for (size_t j = 0; j < data_[0].size(); ++j) {
                data_[i][j] += rhs.data_[i][j];
            }
        }
        return *this;
    }
    Matrix& operator-=(const Matrix& rhs) {
        for (size_t i = 0; i < data_.size(); ++i) {
            for (size_t j = 0; j < data_[0].size(); ++j) {
                data_[i][j] -= rhs.data_[i][j];
            }
        }
        return *this;
    }
    Matrix& operator*=(const Matrix& rhs) {
        *this = *this * rhs;
        return *this;
    }

    friend Matrix operator+(Matrix lhs, const Matrix& rhs) {
        lhs += rhs;
        return lhs;
    }
    friend Matrix operator-(Matrix lhs, const Matrix& rhs) {
        lhs -= rhs;
        return lhs;
    }
    friend Matrix operator*(const Matrix lhs, const Matrix& rhs) {
        const size_t n = lhs.Rows();
        const size_t k = lhs.Columns();
        const size_t m = rhs.Columns();
        Matrix res(n, m);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                for (size_t t = 0; t < k; ++t) {
                    res[i, j] += lhs[i, t] * rhs[t, j];
                }
            }
        }
        return res;
    }

private:
    std::vector<std::vector<double>> data_;
};

Matrix Transpose(const Matrix& a) {
    const size_t n = a.Rows();
    const size_t m = a.Columns();
    Matrix res(m, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            res[j, i] = a[i, j];
        }
    }
    return res;
}

Matrix Identity(int n) {
    Matrix res(n, n);
    for (int i = 0; i < n; ++i) {
        res[i, i] = 1.0;
    }
    return res;
}
