// Author: Yong Hak Lee (camiyu1@gmail.com)

#include <mutex>
#include <list>

template <typename T>
class Queue {
 public:
  void Push(const T& in);
  T Pop();
  bool Empty();
  int Count();

 private:
  std::list<T> queue_;
  std::mutex lock_;
};

