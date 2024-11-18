#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;

void test_recursive_lock() {
    mtx.lock();
    std::cout << "First lock acquired." << std::endl;

    try {
        mtx.lock(); // 第二次加锁
        std::cout << "Second lock acquired." << std::endl;
    } catch (...) {
        std::cout << "Error: Recursive locking not allowed!" << std::endl;
    }

    mtx.unlock();
    std::cout << "First lock released." << std::endl;
}

int main() {
    std::thread t1(test_recursive_lock);
    t1.join();
    return 0;
}
