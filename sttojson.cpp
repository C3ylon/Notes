// #include <iostream>
// #include <string>
// #include <sstream>
// #include <type_traits>
// #include <map>

// // 简单模拟 JSON 对象
// class JsonObject {
// public:

//     JsonObject& operator[](const std::pair<std::string, std::string>& pair) {
//         data[pair.first] = pair.second;
//         return *this;
//     }

//     template<typename T>
//     JsonObject& operator[](const std::pair<std::string, T>& pair) {
//         data[pair.first] = std::to_string(pair.second);
//         return *this;
//     }

//     std::string dump(int indent = 0) const {
//         std::ostringstream oss;
//         oss << "{\n";
//         for (auto it = data.begin(); it != data.end(); ++it) {
//             for (int i = 0; i < indent + 2; ++i) oss << " ";
//             oss << "\"" << it->first << "\": " << "\"" << it->second << "\"";
//             if (std::next(it) != data.end()) oss << ",";
//             oss << "\n";
//         }
//         oss << "}";
//         return oss.str();
//     }

// private:
//     std::map<std::string, std::string> data;
// };

// // 提供默认的 `to_json` 特化方法
// template<typename T>
// JsonObject to_json(const T &obj) {
//     JsonObject json;
//     json = obj.to_json();
//     return json;
// }

// // 特化一个通用的结构体成员转 JSON 的方法
// #define TO_JSON_FIELD(field) json[std::make_pair(std::string(#field), field)]

// // 用于结构体成员转换为 JSON
// template<typename T>
// JsonObject to_json_impl(const T& obj, std::true_type) {
//     JsonObject json;
//     // 自动生成 JSON
//     return json;
// }

// // 模拟的结构体
// struct Person {
//     std::string name;
//     int age;
//     double height;

//     // 转换为 JSON
//     JsonObject to_json() const {
//         JsonObject json;
//         TO_JSON_FIELD(name);
//         TO_JSON_FIELD(age);
//         TO_JSON_FIELD(height);
//         return json;
//     }
// };

// int main() {
//     Person p{"John", 30, 175.5};

//     // 转换 Person 对象到 JSON 格式
//     JsonObject json = to_json(p);

//     std::cout << json.dump(4) << std::endl;

//     return 0;
// }




// // #include <iostream>
// // #include <string>
// // #include <sstream>
// // #include <type_traits>
// // #include <map>
// // #include <vector>

// // // 用于转换基本类型和 std::string 到 JSON
// // template <typename T>
// // std::string to_json_value(const T& value) {
// //     std::ostringstream oss;
// //     oss << value;
// //     return oss.str();
// // }

// // template <>
// // std::string to_json_value(const std::string& value) {
// //     return "\"" + value + "\"";  // 对于字符串，加上双引号
// // }

// // // JSON 对象的输出函数
// // std::string dump_json_object(const std::map<std::string, std::string>& json_data) {
// //     std::ostringstream oss;
// //     oss << "{\n";
// //     for (auto it = json_data.begin(); it != json_data.end(); ++it) {
// //         oss << "  \"" << it->first << "\": " << it->second;
// //         if (std::next(it) != json_data.end()) oss << ",";
// //         oss << "\n";
// //     }
// //     oss << "}";
// //     return oss.str();
// // }

// // // 用于将结构体转换为 JSON 的模板函数
// // template <typename T>
// // std::string to_json(const T& obj);

// // // 宏用于定义结构体成员到 JSON 的映射
// // #define TO_JSON_FIELD(field) json_data[#field] = to_json_value(obj.field);

// // // 结构体模板特化的实现
// // template <typename T>
// // std::string to_json(const T& obj) {
// //     std::map<std::string, std::string> json_data;

// //     // 对每个成员字段进行转换
// //     obj.to_json(json_data);

// //     return dump_json_object(json_data);
// // }

// // // 示例结构体 Person
// // struct Person {
// //     std::string name;
// //     int age;
// //     double height;

// //     void to_json(std::map<std::string, std::string>& json_data) const {
// //         TO_JSON_FIELD(name);
// //         TO_JSON_FIELD(age);
// //         TO_JSON_FIELD(height);
// //     }
// // };

// // // 示例结构体 Car
// // struct Car {
// //     std::string make;
// //     std::string model;
// //     int year;

// //     void to_json(std::map<std::string, std::string>& json_data) const {
// //         TO_JSON_FIELD(make);
// //         TO_JSON_FIELD(model);
// //         TO_JSON_FIELD(year);
// //     }
// // };

// // int main() {
// //     Person p{"John", 30, 175.5};
// //     Car c{"Toyota", "Camry", 2020};

// //     // 输出结构体转换后的 JSON 字符串
// //     std::cout << "Person JSON: " << to_json(p) << std::endl;
// //     std::cout << "Car JSON: " << to_json(c) << std::endl;

// //     return 0;
// // }


#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

// 用于转换基本类型和 std::string 到 JSON
template <typename T>
std::string to_json_value(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template <>
std::string to_json_value(const std::string& value) {
    return "\"" + value + "\"";  // 对于字符串，加上双引号
}

// JSON 对象的输出函数
std::string dump_json_object(const std::vector<std::pair<std::string, std::string>>& json_data) {
    std::ostringstream oss;
    oss << "{\n";
    for (size_t i = 0; i < json_data.size(); ++i) {
        oss << "  \"" << json_data[i].first << "\": " << json_data[i].second;
        if (i != json_data.size() - 1) oss << ",";  // 添加逗号，除了最后一项
        oss << "\n";
    }
    oss << "}";
    return oss.str();
}

// 结构体模板特化的实现
template <typename T>
std::string to_json(const T& obj);

// 示例结构体 Person
struct Person {
    std::string name;
    int age;
    double height;

    // 结构体成员的 JSON 转换函数
    void to_json(std::vector<std::pair<std::string, std::string>>& json_data) const {
        json_data.push_back({"name", to_json_value(name)});
        json_data.push_back({"age", to_json_value(age)});
        json_data.push_back({"height", to_json_value(height)});
    }
};

// 示例结构体 Car
struct Car {
    std::string make;
    std::string model;
    int year;

    // 结构体成员的 JSON 转换函数
    void to_json(std::vector<std::pair<std::string, std::string>>& json_data) const {
        json_data.push_back({"make", to_json_value(make)});
        json_data.push_back({"model", to_json_value(model)});
        json_data.push_back({"year", to_json_value(year)});
    }
};

// 通过 to_json 函数输出 JSON
template <typename T>
std::string to_json(const T& obj) {
    std::vector<std::pair<std::string, std::string>> json_data;
    obj.to_json(json_data); // 调用结构体的 to_json 方法
    return dump_json_object(json_data); // 使用 dump_json_object 输出 JSON 字符串
}

int main() {
    Person p{"John", 30, 175.5};
    Car c{"Toyota", "Camry", 2020};

    // 输出结构体转换后的 JSON 字符串
    std::cout << "Person JSON: " << to_json(p) << std::endl;
    std::cout << "Car JSON: " << to_json(c) << std::endl;

    return 0;
}
