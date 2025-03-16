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

// ====================================================================

#include <iostream>
#include <fstream>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <atomic>
#include <cstring>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class LogBuffer {
public:
    explicit LogBuffer(size_t size = 4 * 1024 * 1024)  // 默认4MB
        : buffer_(size), write_pos_(0) {}

    size_t available() const { return buffer_.size() - write_pos_; }
    size_t length() const { return write_pos_; }
    const char* data() const { return buffer_.data(); }

    void append(const char* data, size_t len) {
        if (len > available()) len = available();
        std::memcpy(buffer_.data() + write_pos_, data, len);
        write_pos_ += len;
    }

    void reset() { write_pos_ = 0; }

private:
    std::vector<char> buffer_;
    size_t write_pos_;
};

class AsyncLogger {
public:
    AsyncLogger(const std::string& file_base_name, size_t roll_size)
        : file_base_name_(file_base_name),
          roll_size_(roll_size),
          written_bytes_(0),
          running_(true) {
        current_buffer_ = std::make_unique<LogBuffer>();
        next_buffer_ = std::make_unique<LogBuffer>();
        backend_thread_ = std::thread(&AsyncLogger::backend_thread_func, this);
    }

    ~AsyncLogger() {
        running_ = false;
        cond_.notify_all();
        backend_thread_.join();
        flush_all_buffers();
    }

    void append(const char* data, size_t len) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (current_buffer_->available() > len) {
            current_buffer_->append(data, len);
        } else {
            buffers_to_write_.push_back(std::move(current_buffer_));

            if (next_buffer_) {
                current_buffer_ = std::move(next_buffer_);
            } else {
                current_buffer_ = std::make_unique<LogBuffer>();
            }

            current_buffer_->append(data, len);
            cond_.notify_one();
        }
    }

private:
    void backend_thread_func() {
        roll_file();
        std::vector<std::unique_ptr<LogBuffer>> buffers_to_write;
        buffers_to_write.reserve(16);

        while (running_) {
            {
                std::unique_lock<std::mutex> lock(mutex_);
                cond_.wait_for(lock, std::chrono::seconds(3),
                              [this] { return !buffers_to_write_.empty() || !running_; });

                buffers_to_write.swap(buffers_to_write_);
                if (!next_buffer_) {
                    next_buffer_ = std::move(current_buffer_);
                    current_buffer_ = std::make_unique<LogBuffer>();
                }
            }

            if (buffers_to_write.empty()) {
                continue;
            }

            write_buffers(buffers_to_write);
            buffers_to_write.clear();

            // Recycle buffers
            {
                std::lock_guard<std::mutex> lock(mutex_);
                if (buffers_to_write_.capacity() < 8) {
                    buffers_to_write_.reserve(8);
                }
            }
        }

        flush_all_buffers();
    }

    void write_buffers(const std::vector<std::unique_ptr<LogBuffer>>& buffers) {
        for (const auto& buffer : buffers) {
            output_file_.write(buffer->data(), buffer->length());
            written_bytes_ += buffer->length();

            if (written_bytes_ >= roll_size_) {
                roll_file();
                written_bytes_ = 0;
            }
        }

        if (output_file_.fail()) {
            std::cerr << "Write log file failed!" << std::endl;
            output_file_.clear();
        }
    }

    void flush_all_buffers() {
        std::vector<std::unique_ptr<LogBuffer>> buffers_to_write;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            buffers_to_write.swap(buffers_to_write_);
            buffers_to_write.push_back(std::move(current_buffer_));
            if (next_buffer_) {
                buffers_to_write.push_back(std::move(next_buffer_));
            }
        }

        write_buffers(buffers_to_write);
        output_file_.flush();
    }

    void roll_file() {
        if (output_file_.is_open()) {
            output_file_.close();
        }

        std::string filename = file_base_name_ + get_time_stamp() + ".log";
        output_file_.open(filename, std::ios::app);

        if (!output_file_.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        }
    }

    std::string get_time_stamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm tm;

        {
            static std::mutex time_mutex;
            std::lock_guard<std::mutex> lock(time_mutex);
            tm = *std::localtime(&t);
        }

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
        return oss.str();
    }

    std::string file_base_name_;
    size_t roll_size_;
    size_t written_bytes_;
    std::ofstream output_file_;

    std::unique_ptr<LogBuffer> current_buffer_;
    std::unique_ptr<LogBuffer> next_buffer_;
    std::vector<std::unique_ptr<LogBuffer>> buffers_to_write_;

    std::mutex mutex_;
    std::condition_variable cond_;
    std::atomic<bool> running_;
    std::thread backend_thread_;
};

class LogMessage {
public:
    LogMessage(LogLevel level, const char* file, int line)
        : level_(level), file_(file), line_(line) {
        stream_ << get_time() << " [" << level_to_string(level_) << "] "
                << file_ << ":" << line_ << " - ";
    }

    ~LogMessage() {
        stream_ << "\n";
        LogMessage::instance().append(stream_.str().data(), stream_.str().size());
    }

    std::ostringstream& stream() { return stream_; }

    static AsyncLogger& instance() {
        static AsyncLogger logger("async_log", 100 * 1024 * 1024);  // 100MB roll size
        return logger;
    }

private:
    std::string get_time() {
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch()) %
                  1000;
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm tm;

        {
            static std::mutex time_mutex;
            std::lock_guard<std::mutex> lock(time_mutex);
            tm = *std::localtime(&t);
        }

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "."
            << std::setw(3) << std::setfill('0') << ms.count();
        return oss.str();
    }

    const char* level_to_string(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO:  return "INFO";
            case LogLevel::WARN:  return "WARN";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    LogLevel level_;
    const char* file_;
    int line_;
    std::ostringstream stream_;
};

#define LOG_DEBUG LogMessage(LogLevel::DEBUG, __FILE__, __LINE__).stream()
#define LOG_INFO  LogMessage(LogLevel::INFO,  __FILE__, __LINE__).stream()
#define LOG_WARN  LogMessage(LogLevel::WARN,  __FILE__, __LINE__).stream()
#define LOG_ERROR LogMessage(LogLevel::ERROR, __FILE__, __LINE__).stream()

// 使用示例
int main() {
    for (int i = 0; i < 1000; ++i) {
        LOG_INFO << "Test log message " << i << " with value: " << i*2;
    }
    return 0;
}
