#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

void processCompressedFile(ifstream &file) {
    while (true) {
        
        int count;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));

       
        if (file.eof()) break;
        if (file.gcount() != sizeof(count)) {
            cerr << "wunzip: Read error while reading count" << endl;
            exit(1);
        }

        char ch;
        file.read(&ch, sizeof(ch));

        if (file.gcount() != sizeof(ch)) {
            cerr << "wunzip: Read error while reading character" << endl;
            exit(1);
        }

        for (int i = 0; i < count; ++i) {
            putchar(ch);
        }
    }
}

class threadPool {
public:
    threadPool(int numThreads) : stop(false) {
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
        cerr << "wunzip: file1 [file2 ...]" << endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        ifstream file(argv[i], ios::binary);
        if (!file.is_open()) {
            cerr << "wunzip: Cannot open file " << argv[i] << endl;
            return 1;
        }
        processCompressedFile(file);
    }

    return 0;
}
