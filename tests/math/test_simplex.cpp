#include <cassert>
#include <cmath>
#include "../../include/simplex.hpp"

int main() {
    Matrix<double, 0, 0> A(3, 2, {1, 1, 1, 0, 0, 1});
    auto lp = simplex(A, {1, 1, 1}, {1, 1});
    assert(std::fabs(lp.first - 1) < 1E-9);
    assert(std::fabs(lp.second[0] + lp.second[1] - 1) < 1E-9);
    Matrix<double, 0, 0> U(1, 2, {1, 0});
    auto unbounded = simplex(U, {1}, {0, 1});
    assert(unbounded.first > 1E300);
    Matrix<double, 0, 0> F(2, 1, {1, -1});
    auto infeasible = simplex(F, {0, -1}, {1});
    assert(infeasible.first < -1E300);
}
