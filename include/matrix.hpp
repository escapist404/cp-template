#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstddef>
#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <utility>

template <typename T, size_t R = 0, size_t C = 0>
class Matrix;

template <typename T>
struct MatrixRankZero {
	bool operator()(const T& value) const { return value == T{}; }
};

template <typename T, size_t R, size_t C, typename IsZero>
int matrix_rank_impl(Matrix<T, R, C> matrix, IsZero is_zero);

template <typename T>
class Matrix<T, 0, 0> {
private:
	size_t R, C;
	std::vector<T> data;

public:
	Matrix(size_t row, size_t column) : R(row), C(column), data(row * column) {
		if (R == 0 || C == 0) {
			throw std::invalid_argument("Dynamic matrix must have non-zero size");
		}
	}

	Matrix(size_t row, size_t column, std::initializer_list<T> info) : R(row), C(column), data(row * column) {
		if (R == 0 || C == 0) {
			throw std::invalid_argument("Dynamic matrix must have non-zero size");
		}
		if (R * C != info.size()) {
			throw std::invalid_argument("Initializer list size does not match matrix size");
		}
		size_t c = 0;
		for (const auto& i : info) {
			data[c] = i;
			++c;
		}
	}

	size_t rows() const { return R; }
	size_t columns() const { return C; }

	template <typename IsZero = MatrixRankZero<T>>
	int rank(IsZero is_zero = IsZero{}) const {
		return matrix_rank_impl(*this, is_zero);
	}

	void resize(size_t row, size_t column) {
		const size_t new_R = row ? row : R;
		const size_t new_C = column ? column : C;
		if (new_R == R && new_C == C) return;
		std::vector<T> next(new_R * new_C);
		for (size_t i = 0; i < std::min(R, new_R); ++i)
			for (size_t j = 0; j < std::min(C, new_C); ++j)
				next[i * new_C + j] = data[i * C + j];
		R = new_R;
		C = new_C;
		data = std::move(next);
	}

	T* operator[](size_t index) {
		return data.data() + index * C;
	}
	const T* operator[](size_t index) const {
		return data.data() + index * C;
	}

	T& at(size_t row, size_t column) {
		if (row >= R || column >= C) throw std::out_of_range("Matrix index out of range");
		return data.at(row * C + column);
	}
	const T& at(size_t row, size_t column) const {
		if (row >= R || column >= C) throw std::out_of_range("Matrix index out of range");
		return data.at(row * C + column);
	}

	Matrix& operator*=(T lambda) {
		for (size_t  i = 0; i < R; ++i) {
			for (size_t j = 0; j < C; ++j) {
				data[i * C + j] *= lambda;
			}
		}
		return *this;
	}

	Matrix& operator+=(const Matrix& A) {
		if (R != A.R || C != A.C) {
			throw std::invalid_argument("Matrix sizes do not match for addition");
		}
		for (size_t i = 0; i < R; ++i) {
			for (size_t j = 0; j < C; ++j) {
				data[i * C + j] += A[i][j];
			}
		}
		return *this;
	}

	Matrix& operator*=(const Matrix& A) {
		if (C != A.R) {
			throw std::invalid_argument("Matrix sizes do not match for multiplication");
		}

		Matrix G(R, A.C);
		for (size_t i = 0; i < R; ++i) {
			T *out = G[i];
			const T *left = data.data() + i * C;
			for (size_t k = 0; k < C; ++k) {
				const T value = left[k];
				const T *right = A[k];
				for (size_t j = 0; j < A.C; ++j) out[j] += value * right[j];
			}
		}

		return *this = G, *this;
	}

	friend Matrix operator*(const Matrix& A, T lambda) {
		auto B = A;
		B *= lambda;
		return B;
	}

	friend Matrix operator+(const Matrix& A, const Matrix& B) {
		auto G = A;
		G += B;
		return G;
	}

	friend Matrix operator*(const Matrix& A, const Matrix& B) {
		auto G = A;
		G *= B;
		return G;
	}
};

template <typename T, size_t R, size_t C>
class Matrix {
	std::array<std::array<T, C>, R> data;

public:
	static_assert(R != 0 && C != 0, "Static matrix must have non-zero size");

	Matrix() : data() {}
	Matrix(std::initializer_list<T> info) : data() {
		assert(info.size() == R * C);
		int c = 0;
		for (const auto& i : info) {
			data[c / C][c % C] = i;
			++c;
		}
	}

	size_t rows() const { return R; }
	size_t columns() const { return C; }

	template <typename IsZero = MatrixRankZero<T>>
	int rank(IsZero is_zero = IsZero{}) const {
		return matrix_rank_impl(*this, is_zero);
	}

	std::array<T, C>& operator[](size_t index) { return data[index]; }
	const std::array<T, C>& operator[](size_t index) const { return data[index]; }

	Matrix& operator*=(T lambda) {
		for (size_t i = 0; i < R; ++i) {
			for (size_t j = 0; j < C; ++j) data[i][j] *= lambda;
		}
		return *this;
	}

	Matrix<T, R, C>& operator+=(const Matrix<T, R, C>& A) {
		for (size_t i = 0; i < R; ++i) {
			for (size_t j = 0; j < C; ++j) data[i][j] += A[i][j];
		}
		return *this;
	}

	Matrix<T, R, C>& operator*=(const Matrix<T, C, C>& A) {
		Matrix<T, R, C> G;
		for (size_t i = 0; i < R; ++i) {
			for (size_t k = 0; k < C; ++k) {
				for (size_t j = 0; j < C; ++j) {
					G[i][j] += data[i][k] * A[k][j];
				}
			}
		}
		return *this = G, *this;
	}

	friend Matrix<T, R, C> operator+(const Matrix<T, R, C>& A, const Matrix<T, R, C>& B) {
		auto G = A;
		G += B;
		return G;
	}

	template <size_t F>
	friend Matrix<T, R, F> operator*(const Matrix<T, R, C>& A, const Matrix<T, C, F>& B) {
		Matrix<T, R, F> G;
		for (size_t i = 0; i < R; ++i) {
			for (size_t k = 0; k < C; ++k) {
				for (size_t j = 0; j < F; ++j) {
					G[i][j] += A[i][k] * B[k][j];
				}
			}
		}
		return G;
	}
};

template <typename T, size_t R, size_t C, typename IsZero>
int matrix_rank_impl(Matrix<T, R, C> matrix, IsZero is_zero) {
	int n = matrix.rows(), m = matrix.columns(), rank = 0;
	for (int col = 0; col < m && rank < n; ++col) {
		int pivot = rank;
		while (pivot < n && is_zero(matrix[pivot][col])) ++pivot;
		if (pivot == n) continue;
		for (int j = col; j < m; ++j)
			std::swap(matrix[rank][j], matrix[pivot][j]);
		T inverse = T(1) / matrix[rank][col];
		for (int j = col; j < m; ++j) matrix[rank][j] *= inverse;
		for (int i = rank + 1; i < n; ++i) {
			T factor = matrix[i][col];
			if (is_zero(factor)) continue;
			for (int j = col; j < m; ++j)
				matrix[i][j] -= factor * matrix[rank][j];
		}
		++rank;
	}
	return rank;
}

template <typename T, size_t R, size_t C>
Matrix<T, 0, 0> operator*(const Matrix<T, R, C>& A,
							const Matrix<T, 0, 0>& B) {
	if (C != B.rows())
		throw std::invalid_argument("Matrix sizes do not match for multiplication");

	Matrix<T, 0, 0> result(R, B.columns());
	for (size_t i = 0; i < R; ++i)
		for (size_t k = 0; k < C; ++k)
			for (size_t j = 0; j < B.columns(); ++j)
				result[i][j] += A[i][k] * B[k][j];
	return result;
}

template <typename T, size_t R, size_t C>
Matrix<T, 0, 0> operator*(const Matrix<T, 0, 0>& A,
							const Matrix<T, R, C>& B) {
	if (A.columns() != R)
		throw std::invalid_argument("Matrix sizes do not match for multiplication");

	Matrix<T, 0, 0> result(A.rows(), C);
	for (size_t i = 0; i < A.rows(); ++i)
		for (size_t k = 0; k < A.columns(); ++k)
			for (size_t j = 0; j < C; ++j)
				result[i][j] += A[i][k] * B[k][j];
	return result;
}

template <typename T, size_t R, size_t C>
std::istream& operator>>(std::istream& is, Matrix<T, R, C>& A) {
	for (size_t i = 0; i < A.rows(); ++i) {
		for (size_t j = 0; j < A.columns(); ++j) {
			is >> A[i][j];
		}
	}
	return is;
}

template <typename T, size_t R, size_t C>
std::ostream& operator<<(std::ostream& os, const Matrix<T, R, C>& A) {
	for (size_t i = 0; i < A.rows(); ++i) {
		for (size_t j = 0; j < A.columns(); ++j) {
			os << A[i][j] << " \n"[j + 1 == A.columns()];
		}
	}
	return os;
}

#endif // MATRIX_HPP
