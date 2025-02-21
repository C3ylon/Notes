#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <atomic>
#include <chrono>
#include <sstream>

class AsyncLogger {
public:
    AsyncLogger(const std::string& filename)
        : m_filename(filename), m_running(true), m_logThread(&AsyncLogger::logWorker, this) {}

    ~AsyncLogger() {
        stop();
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_logQueue.push(message);
        m_cv.notify_one();
    }

private:
    void logWorker() {
        std::ofstream logFile(m_filename, std::ios::out | std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << m_filename << std::endl;
            return;
        }

        while (m_running || !m_logQueue.empty()) {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_cv.wait(lock, [this] { return !m_logQueue.empty() || !m_running; });

            while (!m_logQueue.empty()) {
                logFile << m_logQueue.front() << std::endl;
                m_logQueue.pop();
            }
        }

        logFile.close();
    }

    void stop() {
        m_running = false;
        m_cv.notify_one();
        if (m_logThread.joinable()) {
            m_logThread.join();
        }
    }

private:
    std::string m_filename;
    std::queue<std::string> m_logQueue;
    std::mutex m_queueMutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_running;
    std::thread m_logThread;
};

#include <thread>
#include <chrono>

int main() {
    AsyncLogger logger("log.txt");

    // 模拟多线程写日志
    std::thread t1([&logger]() {
        for (int i = 0; i < 10; ++i) {
            std::ostringstream oss;
            oss << "Thread 1 Log Message #" << i;
            logger.log(oss.str());
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

    std::thread t2([&logger]() {
        for (int i = 0; i < 10; ++i) {
            std::ostringstream oss;
            oss << "Thread 2 Log Message #" << i;
            logger.log(oss.str());
            std::this_thread::sleep_for(std::chrono::milliseconds(60));
        }
    });

    t1.join();
    t2.join();

    std::this_thread::sleep_for(std::chrono::seconds(1)); // 确保日志写完

    std::cout << "Logs are written asynchronously." << std::endl;
    return 0;
}

// #include <iostream>
// #include <queue>
// #include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <chrono>

// std::queue<int> buffer;                  // 缓冲区
// std::mutex mtx;                          // 保护缓冲区的互斥锁
// std::condition_variable cv;              // 条件变量

// // 生产者线程
// void producer() {
//     for (int i = 0; i < 10; ++i) {
//         std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟生产时间

//         std::lock_guard<std::mutex> lock(mtx);  // 锁住缓冲区
//         buffer.push(i);
//         std::cout << "Produced: " << i << std::endl;

//         cv.notify_all(); // 通知消费者有新产品
//     }
// }

// // 消费者线程
// void consumer() {
//     while (true) {
//         std::unique_lock<std::mutex> lock(mtx);

//         cv.wait(lock, []{ return !buffer.empty(); }); // 如果缓冲区为空，等待

//         int value = buffer.front();
//         buffer.pop();
//         std::cout << "Consumed: " << value << std::endl;

//         if (buffer.empty()) {
//             std::cout << "Buffer is empty, consumer is waiting..." << std::endl;
//         }

//         lock.unlock(); // 解锁，可以让生产者继续操作
//         std::this_thread::sleep_for(std::chrono::milliseconds(150)); // 模拟消费时间
//     }
// }

// int main() {
//     std::thread prod(producer);
//     std::thread cons(consumer);

//     prod.join();
//     cons.join();

//     return 0;
// }
