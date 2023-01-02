#pragma once

#include <concepts>
#include <iostream>
#include <map>
#include <memory>

namespace autodiff {

template <class T>
concept Dual = requires(T &x) {
                 { x *x } -> std::same_as<T>;
               };

constexpr int_fast32_t factorial_(int_fast32_t n) {
  int_fast32_t ret{1};
  for (int i = 1; i <= n; i++) {
    ret *= i;
  }
  return ret;
}

constexpr int_fast32_t permutation_(int_fast32_t n, int_fast32_t m) {
  int_fast32_t den{1};
  int_fast32_t num{1};
  for (int i = 0; i < m; i++) {
    num *= n - i;
    den *= (i + 1);
  }
  return num / den;
}

template <int Rank = 2, class T = double>
  requires std::floating_point<T>
class dual {
 private:
  using ValType = T;
  using ValueMap =
      std::map<std::map<std::uint_fast16_t, int_fast16_t>, ValType>;

  uint_fast16_t id_;

  inline static std::atomic_uint_fast16_t id_counter{0};

  ValueMap value_;

  // Intermidiate dual has id 0
  dual(ValueMap &&value_) : id_{0}, value_(std::move(value_)) {}

 public:
  // Newly constructed dual has unique id (1-indexed)
  dual(ValType &&value)
      : id_(++id_counter), value_({{{{{id_}, 0}}, value}, {{{{id_}, 1}}, 1}}) {}

  constexpr friend std::ostream &operator<<(std::ostream &os,
                                            const dual<Rank, T> &dual) {
    // std::cout << "value: " << dual.value_.at(0, 0) << " [" << Rank << "]"
    //           << std::endl;
    for (auto &&[div, value] : dual.value_) {
      for (auto &&[rank, dim] : div) {
        std::cout << rank << "," << dim << std::endl;
      }
      std::cout << "\t" << value << std::endl;
    }
    return os;
  }

  // dual(ValType val) {
  //   value_[0] = val;
  //   value_[1] = 1;
  // }

  auto value() { return value_; }

  friend dual<Rank, ValType> operator*(const dual<Rank, ValType> &a,
                                       const dual<Rank, ValType> &b) {
    ValueMap v;

    for (auto &&[a_rank, a_val] : a.value_) {
      if (a_val == 0) {
        continue;
      }
      for (auto &&[b_rank, b_val] : b.value_) {
        if (b_val == 0) {
          continue;
        }
        int rank_counter{0};

        std::map<uint_fast16_t, int_fast16_t> key;

        for (auto &&[a_id, a_dim] : a_rank) {
          key.insert_or_assign(a_id, a_dim);
          rank_counter += a_dim;
        }

        ValType ret_value{1};

        for (auto &&[b_id, b_dim] : b_rank) {
          rank_counter += b_dim;
          std::cout << rank_counter << std::endl;
          if (key.contains(b_id)) {
            key.at(b_id) += b_dim;
            ret_value *= permutation_(key.at(b_id), b_dim);
          } else {
            key.insert_or_assign(b_id, b_dim);
          }
        }

        if (rank_counter > Rank) {
          // Prune
          continue;
        }

        ret_value *= a_val * b_val;

        if (v.contains(key)) {
          v.at(key) += ret_value;
        } else {
          v.insert_or_assign(key, ret_value);
        }
      }
    }

    return {std::move(v)};
  }

};

};  // namespace autodiff
