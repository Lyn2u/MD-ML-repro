// By Boshi Yuan

#ifndef MD_ML_LINEAR_ALGEBRA_H
#define MD_ML_LINEAR_ALGEBRA_H


#include <vector>
#include <algorithm>

// ===== macOS / libc++ compatibility for <execution> algorithms =====
// ===== macOS / libc++ safe transform wrappers =====
// NOTE: libc++ does NOT reliably support execution policies (par_unseq).
// We therefore ALWAYS fall back to sequential std::transform.

#define MDML_TRANSFORM(first, last, result, op) \
    std::transform((first), (last), (result), (op))

#define MDML_TRANSFORM2(first1, last1, first2, result, op) \
    std::transform((first1), (last1), (first2), (result), (op))

// ================================================================


#include <concepts>

#include <Eigen/Core>

namespace md_ml {

// For matrix addtion, subtraction, and scalar product, we can use
// 1. std::transform
// 2. std::ranges::transform (more modern)
// 3. Eigen's implementation of matrix operations
// 4. naive for loop
//
// I found that std::transform is the fastest, so I decided to use std::transform
// Also, we can use MDML_EXEC_POLICY to parallelize the computation
// However, the parallelization is only available
// on Linux gcc and Windows MinGW gcc, but not on macOS Apple Clang.
// so we detect it using macros to avoid compilation errors on macOS.
//
// For matrix multiplication, we use Eigen's implementation.
//
// WARNING:
// To improve efficiency, the dimensions are not checked in these functions,
// instead, they are checked in the constructor of the gate classes,
// so the checks are carried out prior to the online phase.


template <std::integral T>
inline
std::vector<T> matrixAdd(const std::vector<T>& x, const std::vector<T>& y) {
    std::vector<T> output(x.size());

MDML_TRANSFORM2(x.begin(), x.end(), y.begin(), output.begin(), std::plus<T>());

    return output;
}


template <std::integral T>
inline
void matrixAddAssign(std::vector<T>& x, const std::vector<T>& y) {
MDML_TRANSFORM2(x.begin(), x.end(), y.begin(), x.begin(), std::plus<T>());
}


template <std::integral T1, std::integral T2>
inline
std::vector<T1> matrixAddConstant(const std::vector<T1>& x, T2 constant) {
    std::vector<T1> output(x.size());

MDML_TRANSFORM(x.begin(), x.end(), output.begin(),
               [constant](T1 val) { return val + constant; });

    return output;
}


template <std::integral T>
inline
std::vector<T> matrixSubtract(const std::vector<T>& x, const std::vector<T>& y) {
    std::vector<T> output(x.size());

    MDML_TRANSFORM2(x.begin(), x.end(), y.begin(), output.begin(), std::minus<T>());

    return output;
}


template <std::integral T>
inline
void matrixSubtractAssign(std::vector<T>& x, const std::vector<T>& y) {
    MDML_TRANSFORM2(x.begin(), x.end(), y.begin(), x.begin(), std::minus<T>());

}


// matrix scalar product
template <std::integral T>
inline
std::vector<T> matrixScalar(const std::vector<T>& x, T scalar) {
    std::vector<T> output(x.size());

MDML_TRANSFORM(x.begin(), x.end(), output.begin(),
               [scalar](T val) { return scalar * val; });

    return output;
}

template <std::integral T>
inline
void matrixScalarAssign(std::vector<T>& x, T scalar) {
MDML_TRANSFORM(x.begin(), x.end(), x.begin(),
               [scalar](T val) { return scalar * val; });

}


template <std::integral T>
inline
std::vector<T> matrixElemMultiply(std::vector<T>& x, std::vector<T>& y) {
    std::vector<T> output(x.size());

    MDML_TRANSFORM2(x.begin(), x.end(), y.begin(), output.begin(),
                    std::multiplies<T>());

    return output;
}



template <std::integral T>
inline
void matrixMultiply(const T* lhs, const T* rhs, T* output,
                    std::size_t dim_row, std::size_t dim_mid, std::size_t dim_col) {
    using MatrixType = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

    Eigen::Map<const MatrixType> matrix_lhs(lhs, dim_row, dim_mid);
    Eigen::Map<const MatrixType> matrix_rhs(rhs, dim_mid, dim_col);
    Eigen::Map<MatrixType> matrix_output(output, dim_row, dim_col);

    matrix_output = matrix_lhs * matrix_rhs;
}


template <std::integral T>
inline
std::vector<T> matrixMultiply(const std::vector<T>& lhs, const std::vector<T>& rhs,
                              std::size_t dim_row, std::size_t dim_mid, std::size_t dim_col) {
    std::vector<T> output(dim_row * dim_col);
    matrixMultiply(lhs.data(), rhs.data(), output.data(), dim_row, dim_mid, dim_col);
    return output;
}


} // namespace md_ml


#endif //MD_ML_LINEAR_ALGEBRA_H
