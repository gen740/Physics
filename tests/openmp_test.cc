#include <gtest/gtest.h>
#include <omp.h>

#include <array>
#include <chrono>
#include <complex>

std::array<int, 100'000'000> test_arr;

using namespace std::chrono_literals;
using std::chrono::nanoseconds;
using std::chrono::steady_clock;

TEST(OpenMP, Threading) {
  EXPECT_GT(omp_get_max_threads(), 1);
  for (int i = 0; i < 100'000'000; ++i) {
    test_arr[i] = 0;
  }
  auto start = steady_clock::now();
  for (int i = 0; i < 10'000'000; ++i) {
    if (i % 2 != 0) {
      test_arr[i] = 1 / std::sqrt(i);
    }
  }
  auto end = steady_clock::now();

  auto elapsed_time =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

  start = steady_clock::now();
  std::cout << elapsed_time.count() << std::endl;
#pragma omp parallel for
  for (int i = 0; i < 10'000'000; ++i) {
    if (i % 2 != 0) {
      test_arr[i] = 1 / std::sqrt(i);
    }
  }
  end = steady_clock::now();
  elapsed_time =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  std::cout << elapsed_time.count() << std::endl;
}
