#include <fstream>
#include <string>
#include <stdexcept>
#include <memory>

class FileHandler {
public:
    // 构造函数，打开文件
    FileHandler(const std::string& filePath, std::ios::openmode mode)
        : file(std::make_unique<std::fstream>(filePath, mode)) {
        if (!file->is_open()) {
            throw std::runtime_error("Failed to open file: " + filePath);
        }
    }

    // 删除拷贝构造函数和赋值运算符，防止资源不安全地被拷贝
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

    // 移动构造函数和赋值运算符
    FileHandler(FileHandler&& other) noexcept
        : file(std::move(other.file)) {}

    FileHandler& operator=(FileHandler&& other) noexcept {
        if (this != &other) {
            file = std::move(other.file);
        }
        return *this;
    }

    // 写入数据
    void write(const std::string& data) {
        if (!file || !file->is_open()) {
            throw std::runtime_error("File is not open for writing");
        }
        (*file) << data;
    }

    // 读取一行数据
    std::string readLine() {
        if (!file || !file->is_open()) {
            throw std::runtime_error("File is not open for reading");
        }
        std::string line;
        if (!std::getline(*file, line)) {
            throw std::runtime_error("Failed to read line from file");
        }
        return line;
    }

    // 检查文件是否处于打开状态
    bool isOpen() const {
        return file && file->is_open();
    }

    // 析构函数，自动关闭文件
    ~FileHandler() = default;

private:
    std::unique_ptr<std::fstream> file; // 使用unique_ptr管理文件流，确保异常安全
};
