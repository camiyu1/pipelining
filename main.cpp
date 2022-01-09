#include <glog/logging.h>
#include <benchmark/benchmark.h>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>

std::mutex g_mutex;
constexpr unsigned kMaxVal = 10000LL;

int accum(int start, int end) {
  int sum = 0;
  for (int i = start; i < end; ++i) {
    sum += i;
  }
  return sum;
}

BENCHMARK_MAIN();

static void BM_THREAD_TEST1(benchmark::State& state) {
  std::atomic<int> accumulate(0);
  auto sum = [&](int start, int end) {
    accumulate += accum(start, end);
  };
  FLAGS_logtostderr = true;

  for (auto _ : state) {
    accumulate = 0;
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
      threads.push_back(std::thread(sum, 0, 1000));
    }
    for (auto &tr : threads) {
      if (tr.joinable()) {
        tr.join();
      }
    }
    if (accumulate != 4995000) {
      LOG(INFO) << "Result: " << accumulate;
    }
  }
}

static void BM_THREAD_TEST2(benchmark::State& state) {
  int accumulate(0);
  auto sum = [&](int start, int end) {
    std::unique_lock<std::mutex> ul(g_mutex);
    accumulate += accum(start, end);
  };
  FLAGS_logtostderr = true;

  for (auto _ : state) {
    accumulate = 0;
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
      threads.push_back(std::thread(sum, 0, 1000));
    }
    for (auto &tr : threads) {
      if (tr.joinable()) {
        tr.join();
      }
    }
    if (accumulate != 4995000) {
      LOG(INFO) << "Result: " << accumulate;
    }
  }
}

BENCHMARK_WITH_UNIT(BM_THREAD_TEST1, benchmark::kNanosecond);
BENCHMARK_WITH_UNIT(BM_THREAD_TEST2, benchmark::kNanosecond);

#if 0
int main(int argc, char **argv) {
  std::atomic<int> accumulate(0);
  auto sum = [&](int start, int end) {
    accumulate += accum(start, end);
  };

  google::InitGoogleLogging(argv[0]);
  FLAGS_logtostderr = true;

  for (int j = 0; j < 10000; ++j) {
    accumulate = 0;
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
      threads.push_back(std::thread(sum, 0, 1000));
    }
    for (auto &tr : threads) {
      if (tr.joinable()) {
        tr.join();
      }
    }
    if (accumulate != 4995000) {
      LOG(INFO) << "Result: " << accumulate;
    }
  }
}
#endif