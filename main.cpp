// Author: Yong Hak Lee (camiyu1@gmail.com)

#include <glog/logging.h>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <condition_variable>
#include <atomic>
#include <utility>
#include "util.hpp"

std::mutex g_mutex;
std::condition_variable g_cv;

constexpr unsigned kMaxVal = 10000LL;

int accum(int start, int end) {
  int sum = 0;
  for (int i = start; i < end; ++i) {
    sum += i;
  }
  return sum;
}

constexpr int kNumIterations = 50;
int main(int argc, char **argv) {
  Queue<std::pair<int, std::vector<int>>> in_queue;
  std::atomic<bool> inputed = false;
  std::atomic<int> count(0);

  auto stepone = [&](int num_iter) {
    std::lock_guard<std::mutex> lg(g_mutex);
    LOG(INFO) << "Push " << num_iter << " case";
    std::vector<int> input;
    for (int i = 0; i < 10 * num_iter; ++i) {
      input.push_back(i);
    }
    in_queue.Push(std::make_pair(num_iter, input));
    inputed = true;
    g_cv.notify_one();
  };

  int sum;
  auto steptwo = [&]() {
    LOG(INFO) << "Step Two";
    while (count < kNumIterations) {
      std::unique_lock<std::mutex> ul(g_mutex);
      g_cv.wait(ul, [&](){return inputed == true;});
      while (!in_queue.Empty()) {
        auto [num, pop_input] = in_queue.Pop();
        sum = 0;
        for (int i : pop_input) {
          sum += i;
        }
        count++;
        LOG(INFO) << "case " << num << " Result : " << sum;
      }
      inputed = false;
      LOG(INFO) << "Count : " << count << " " << in_queue.Count();
    }
  };

  auto t2 = std::thread(steptwo);
  std::vector<std::thread> threads;
  for (int i = 0; i < kNumIterations; ++i) {
    LOG(INFO) << i << " case inputed";
    threads.push_back(std::thread(stepone, i));
  }

  for (auto& tr : threads) {
    tr.join();
  }

  t2.join();
}
