// Author: Yong Hak Lee (camiyu1@gmail.com)

#include <vector>
#include <utility>
#include <exception>
#include "util.hpp"

template <typename T>
void Queue<T>::Push(const T& in) {
  std::unique_lock<std::mutex> l(lock_);
  queue_.push_back(in);
}

template <typename T>
T Queue<T>::Pop() {
  std::unique_lock<std::mutex> l(lock_);
  if (queue_.empty() == true) {
    throw std::exception();
  }
  T ret = queue_.front();
  queue_.pop_front();
  return ret;
}

template <typename T>
int Queue<T>::Count() {
  std::unique_lock<std::mutex> l(lock_);
  return queue_.size();
}

template <typename T>
bool Queue<T>::Empty() {
  std::unique_lock<std::mutex> l(lock_);
  return queue_.empty();
}

// Instantiation
template class Queue<std::pair<int, std::vector<int>>>;