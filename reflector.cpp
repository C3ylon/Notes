#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <type_traits>
#include <any>
#include <memory>
#include <functional>
#include <iomanip>

// 类型特征检查
template<typename T>
struct is_string : std::false_type {};

template<>
struct is_string<std::string> : std::true_type {};

template<typename T>
struct is_numeric : std::false_type {};

template<>
struct is_numeric<int> : std::true_type {};

template<>
struct is_numeric<double> : std::true_type {};

// 字段信息类
class FieldInfo {
public:
    std::string name;
    std::any value;

    FieldInfo(const std::string& n, const std::any& v) : name(n), value(v) {}
};

// 序列化器接口
class Serializer {
public:
    virtual ~Serializer() = default;
    virtual void startObject(const std::string& name) = 0;
    virtual void endObject() = 0;
    virtual void serializeField(const FieldInfo& field) = 0;
    virtual std::string getResult() = 0;
};

// XML 序列化器
class XMLSerializer : public Serializer {
private:
    std::ostringstream oss;
    int indentLevel = 0;

    void addIndent() {
        for (int i = 0; i < indentLevel; ++i) {
            oss << "  ";
        }
    }

public:
    void startObject(const std::string& name) override {
        addIndent();
        oss << "<" << name << ">\n";
        indentLevel++;
    }

    void endObject() override {
        indentLevel--;
        addIndent();
        oss << "</" << "object" << ">\n";
    }

    void serializeField(const FieldInfo& field) override {
        addIndent();
        oss << "<" << field.name << ">";

        if (field.value.type() == typeid(std::string)) {
            oss << std::any_cast<std::string>(field.value);
        } else if (field.value.type() == typeid(int)) {
            oss << std::any_cast<int>(field.value);
        } else if (field.value.type() == typeid(double)) {
            oss << std::any_cast<double>(field.value);
        }

        oss << "</" << field.name << ">\n";
    }

    std::string getResult() override {
        return oss.str();
    }
};

// HTML 序列化器
class HTMLSerializer : public Serializer {
private:
    std::ostringstream oss;

public:
    void startObject(const std::string& name) override {
        oss << "<!DOCTYPE html>\n<html>\n<head>\n";
        oss << "<title>" << name << "</title>\n";
        oss << "<style>table { border-collapse: collapse; width: 100%; } "
            << "th, td { border: 1px solid black; padding: 8px; text-align: left; } "
            << "th { background-color: #f2f2f2; }</style>\n";
        oss << "</head>\n<body>\n";
        oss << "<h1>" << name << "</h1>\n";
        oss << "<table>\n";
        oss << "<tr><th>Field</th><th>Value</th></tr>\n";
    }

    void endObject() override {
        oss << "</table>\n</body>\n</html>";
    }

    void serializeField(const FieldInfo& field) override {
        oss << "<tr><td>" << field.name << "</td><td>";

        if (field.value.type() == typeid(std::string)) {
            oss << std::any_cast<std::string>(field.value);
        } else if (field.value.type() == typeid(int)) {
            oss << std::any_cast<int>(field.value);
        } else if (field.value.type() == typeid(double)) {
            oss << std::fixed << std::setprecision(2) << std::any_cast<double>(field.value);
        }

        oss << "</td></tr>\n";
    }

    std::string getResult() override {
        return oss.str();
    }
};

// 可序列化接口
class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual std::vector<FieldInfo> getFields() const = 0;
    virtual std::string getName() const = 0;
};

// 序列化上下文
class SerializationContext {
public:
    static std::string serialize(const ISerializable& obj, Serializer& serializer) {
        serializer.startObject(obj.getName());

        for (const auto& field : obj.getFields()) {
            serializer.serializeField(field);
        }

        serializer.endObject();
        return serializer.getResult();
    }
};

// 示例结构体
struct Person : public ISerializable {
    std::string name;
    int age;
    double salary;

    Person(const std::string& n, int a, double s) : name(n), age(a), salary(s) {}

    std::vector<FieldInfo> getFields() const override {
        return {
            FieldInfo("name", name),
            FieldInfo("age", age),
            FieldInfo("salary", salary)
        };
    }

    std::string getName() const override {
        return "Person";
    }
};

struct Product : public ISerializable {
    std::string title;
    int id;
    double price;

    Product(const std::string& t, int i, double p) : title(t), id(i), price(p) {}

    std::vector<FieldInfo> getFields() const override {
        return {
            FieldInfo("title", title),
            FieldInfo("id", id),
            FieldInfo("price", price)
        };
    }

    std::string getName() const override {
        return "Product";
    }
};

int main() {
    // 创建一些示例数据
    Person person("John Doe", 30, 75000.50);
    Product product("Laptop", 12345, 1299.99);

    // 序列化为 XML
    XMLSerializer xmlSerializer;
    std::string xmlResult = SerializationContext::serialize(person, xmlSerializer);
    std::cout << "XML Output:\n" << xmlResult << "\n\n";

    // 序列化为 HTML
    HTMLSerializer htmlSerializer;
    std::string htmlResult = SerializationContext::serialize(product, htmlSerializer);
    std::cout << "HTML Output:\n" << htmlResult << "\n";

    return 0;
}
