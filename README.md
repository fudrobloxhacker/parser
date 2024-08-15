# parser
Parser serves as a lightweight, header-only C++ library for parsing JSON strings into structured data without any external dependencies. It efficiently handles fundamental JSON types and provides robust error handling.

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
