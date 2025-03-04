#include <iostream>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <stdexcept>

// 消息基类
class BaseMessage {
public:
    virtual ~BaseMessage() = default;
    virtual std::type_index getType() const = 0;
};

// 示例消息类型1
class MessageA : public BaseMessage {
public:
    int id;
    std::string content;

    MessageA(int i, const std::string& s) : id(i), content(s) {}

    std::type_index getType() const override {
        return typeid(MessageA);
    }
};

// 示例消息类型2
class MessageB : public BaseMessage {
public:
    double value;
    bool flag;

    MessageB(double v, bool f) : value(v), flag(f) {}

    std::type_index getType() const override {
        return typeid(MessageB);
    }
};

class MessageProcessor {
public:
    MessageProcessor() = default;

    ~MessageProcessor() {
        stop();
    }

    // 注册消息处理函数
    template<typename T>
    void registerHandler(std::function<void(const T&)> handler) {
        std::lock_guard<std::mutex> lock(handlersMutex_);
        auto type = std::type_index(typeid(T));
        handlers_[type] = [handler](const BaseMessage& msg) {
            try {
                const auto& derived = dynamic_cast<const T&>(msg);
                handler(derived);
            } catch (const std::bad_cast&) {
                std::cerr << "消息类型和处理函数不匹配!" << std::endl;
            }
        };
    }

    // 启动处理线程
    void start() {
        running_ = true;
        workerThread_ = std::thread(&MessageProcessor::processMessages, this);
    }

    // 停止处理线程
    void stop() {
        {
            std::lock_guard<std::mutex> lock(queueMutex_);
            running_ = false;
        }
        cv_.notify_all();
        if (workerThread_.joinable()) {
            workerThread_.join();
        }
    }

    // 发送消息到队列
    void sendMessage(std::shared_ptr<BaseMessage> msg) {
        {
            std::lock_guard<std::mutex> lock(queueMutex_);
            queue_.push(msg);
        }
        cv_.notify_one();
    }

private:
    void processMessages() {
        while (true) {
            std::shared_ptr<BaseMessage> msg;
            {
                std::unique_lock<std::mutex> lock(queueMutex_);
                cv_.wait(lock, [this]() { return !queue_.empty() || !running_; });

                if (!running_ && queue_.empty()) {
                    break;
                }

                if (!queue_.empty()) {
                    msg = queue_.front();
                    queue_.pop();
                }
            }

            if (msg) {
                std::lock_guard<std::mutex> lock(handlersMutex_);
                auto it = handlers_.find(msg->getType());
                if (it != handlers_.end()) {
                    try {
                        it->second(*msg);
                    } catch (const std::exception& e) {
                        std::cerr << "处理消息时发生异常: " << e.what() << std::endl;
                    }
                } else {
                    std::cerr << "未注册的处理类型: " << typeid(*msg).name() << std::endl;
                }
            }
        }
    }

    std::thread workerThread_;
    bool running_ = false;

    std::queue<std::shared_ptr<BaseMessage>> queue_;
    std::mutex queueMutex_;
    std::condition_variable cv_;

    std::unordered_map<std::type_index, std::function<void(const BaseMessage&)>> handlers_;
    std::mutex handlersMutex_;
};

int main() {
    MessageProcessor processor;

    // 注册处理函数
    processor.registerHandler<MessageA>([](const MessageA& msg) {
        std::cout << "处理MessageA - ID: " << msg.id
                  << ", 内容: " << msg.content << std::endl;
    });

    processor.registerHandler<MessageB>([](const MessageB& msg) {
        std::cout << "处理MessageB - 值: " << msg.value
                  << ", 标志: " << (msg.flag ? "真" : "假") << std::endl;
    });

    processor.start();

    // 发送测试消息
    processor.sendMessage(std::make_shared<MessageA>(1, "测试消息A"));
    processor.sendMessage(std::make_shared<MessageB>(3.14, true));
    processor.sendMessage(std::make_shared<MessageA>(2, "另一个消息A"));

    // 等待处理完成
    std::this_thread::sleep_for(std::chrono::seconds(1));
    processor.stop();

    return 0;
}