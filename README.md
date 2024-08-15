# parser
The Parser library is a lightweight, header-only C++ library that enables developers to efficiently parse JSON strings into structured data, all while maintaining a simple integration process and eliminating the need for any external dependencies, making it an ideal choice for applications requiring seamless JSON handling.

To use this library, consider the following example:

```c++
#include "parser.hpp"

int main() {
    JsonParser parser;
    std::string jsonString = R"({"name": "John", "age": 30, "isStudent": false})";

    try {
        auto result = parser.parse(jsonString);
        // Use the parsed JSON data here
        std::cout << "JSON parsed successfully!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }

    return 0;
}
```
