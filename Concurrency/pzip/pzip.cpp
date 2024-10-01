#include <atomic>
#include <condition_variable>
#include <cstring>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <sched.h>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>
using namespace std;

class syncWrite {
public:
  syncWrite(int total_) {
    current = 0;
    total = total_;
    mp.resize(total);
    worker = thread([this]() { this->write(); });
  }

  void set(vector<pair<int, char>> v, int id) {
    {
      unique_lock<std::mutex> lock(m);
      mp[id] = v;
    }
    cv.notify_one();
  }

  ~syncWrite() { worker.join(); }

private:
  void write() {
    while (current < total) {
      vector<pair<int, char>> v;
      {
        unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this] { return mp[current].size() > 0; });
        v = mp[current];
      }

      {
        // cout<<"Writing "<<current<<endl;
        for (auto &p : v) {
          fwrite(&p.first, sizeof(p.first), 1, stdout);
          fwrite(&p.second, sizeof(p.second), 1, stdout);
        }
        current++;
      }
    }
  }

  vector<vector<pair<int, char>>> mp;
  int current;
  int total;
  mutex m;
  condition_variable cv;
  thread worker;
};

syncWrite *sw = nullptr;

void processFile(shared_ptr<ifstream> file, int id) {

  // cout<<"Processing File "<<id<<endl;
  char prevChar = 0;
  int count = 0;
  vector<pair<int, char>> v;

  while (true) {
    char currentChar;
    file->get(currentChar);

    if (file->eof()) {
      break;
    }

    if (currentChar == prevChar) {
      count++;
    } else {
      if (count > 0) {
        v.push_back({count, prevChar});
      }
      prevChar = currentChar;
      count = 1;
    }
  }

  if (count > 0) {
    v.push_back({count, prevChar});
  }
  sw->set(v, id - 1);
}

class threadPool {
public:
  threadPool(int numThreads) {
    stop = false;
    for (int i = 0; i < numThreads; i++) {
      threads.emplace_back([this] { executeTask(); });
    }
  }

  void addTask(function<void()> task) {
    {
      unique_lock<std::mutex> lock(m);
      functionQueue.push(std::move(task));
    }
    cv.notify_one();
  }

  ~threadPool() {
    {
      unique_lock<mutex> lock(m);
      stop = true;
    }
    cv.notify_all();
    for (auto &thread : threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }

private:
  void executeTask() {
    while (true) {
      function<void()> task;
      {
        unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this] { return stop || !functionQueue.empty(); });
        if (stop && functionQueue.empty())
          return;
        task = std::move(functionQueue.front());
        functionQueue.pop();
      }
      task();
    }
  }

  vector<std::thread> threads;
  queue<function<void()>> functionQueue;
  condition_variable cv;
  mutex m;
  bool stop;
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "wzip: file1 [file2 ...]" << endl;
    return 1;
  }

  threadPool threadpool_(sysconf(_SC_NPROCESSORS_CONF));
  sw = new syncWrite(argc - 1);
  for (int i = 1; i < argc; ++i) {
    auto file = make_shared<ifstream>(argv[i], ios::binary);
    if (!file->is_open()) {
      cerr << "wzip: Cannot open file " << argv[i] << endl;
      return 1;
    }
    threadpool_.addTask([file, i]() mutable { processFile(file, i); });
  }

  delete sw;
  return 0;
}
