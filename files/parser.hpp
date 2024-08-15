#ifndef INCLUDE_PARSER_HPP
#define INCLUDE_PARSER_HPP

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <cctype>
#include <stdexcept>
#include <memory>
#include <functional>

class JsonParser {
public:
    using JsonValue = std::variant<std::nullptr_t, bool, double, std::string, std::vector<JsonValue>, std::unordered_map<std::string, JsonValue>>;

    JsonParser() = default;
    ~JsonParser() = default;

    JsonValue parse(const std::string& json) noexcept(false) {
        this->json = json;
        index = 0;
        return parseValue();
    }

private:
    std::string json;
    size_t index = 0;

    auto skipWhitespace = [this]() {
        while (index < json.size() && std::isspace(json[index])) {
            ++index;
        }
    };

    JsonValue parseValue() {
        skipWhitespace();
        if (index >= json.size()) throw std::runtime_error("Unexpected end of input");

        if (json[index] == '{') return parseObject();
        if (json[index] == '[') return parseArray();
        if (json[index] == '"') return parseString();
        if (std::isdigit(json[index]) || json[index] == '-') return parseNumber();
        if (json.compare(index, 4, "true") == 0) { index += 4; return true; }
        if (json.compare(index, 5, "false") == 0) { index += 5; return false; }
        if (json.compare(index, 4, "null") == 0) { index += 4; return nullptr; }

        throw std::runtime_error("Invalid JSON value");
    }

    JsonValue parseObject() {
        std::unordered_map<std::string, JsonValue> object;
        ++index; // skip '{'
        skipWhitespace();

        while (index < json.size() && json[index] != '}') {
            std::string key = parseString();
            skipWhitespace();
            if (index >= json.size() || json[index] != ':') throw std::runtime_error("Expected ':' after key");
            ++index; // skip ':'
            JsonValue value = parseValue();
            object[key] = value;
            skipWhitespace();
            if (index < json.size() && json[index] == ',') ++index; // skip ','
            skipWhitespace();
        }
        if (index >= json.size() || json[index] != '}') throw std::runtime_error("Expected '}'");
        ++index; // skip '}'
        return object;
    }

    JsonValue parseArray() {
        std::vector<JsonValue> array;
        ++index; // skip '['
        skipWhitespace();

        while (index < json.size() && json[index] != ']') {
            array.push_back(parseValue());
            skipWhitespace();
            if (index < json.size() && json[index] == ',') ++index; // skip ','
            skipWhitespace();
        }
        if (index >= json.size() || json[index] != ']') throw std::runtime_error("Expected ']'");
        ++index; // skip ']'
        return array;
    }

    std::string parseString() {
        if (json[index] != '"') throw std::runtime_error("Expected '\"'");
        ++index; // skip '"'
        std::string result;
        while (index < json.size() && json[index] != '"') {
            if (json[index] == '\\') {
                ++index; // skip '\'
                if (index >= json.size()) throw std::runtime_error("Unexpected end of input in string escape");
                switch (json[index]) {
                    case '"': result += '"'; break;
                    case '\\': result += '\\'; break;
                    case '/': result += '/'; break;
                    case 'b': result += '\b'; break;
                    case 'f': result += '\f'; break;
                    case 'n': result += '\n'; break;
                    case 'r': result += '\r'; break;
                    case 't': result += '\t'; break;
                    default: throw std::runtime_error("Invalid escape sequence");
                }
            } else {
                result += json[index];
            }
            ++index;
        }
        if (index >= json.size() || json[index] != '"') throw std::runtime_error("Expected '\"'");
        ++index; // skip '"'
        return result;
    }

    JsonValue parseNumber() {
        size_t start = index;
        if (json[index] == '-') ++index;
        while (index < json.size() && std::isdigit(json[index])) ++index;
        if (index < json.size() && json[index] == '.') {
            ++index;
            while (index < json.size() && std::isdigit(json[index])) ++index;
        }
        if (index < json.size() && (json[index] == 'e' || json[index] == 'E')) {
            ++index;
            if (index < json.size() && (json[index] == '+' || json[index] == '-')) ++index;
            while (index < json.size() && std::isdigit(json[index])) ++index;
        }
        return std::stod(json.substr(start, index - start));
    }
};

#endif // INCLUDE_PARSER_HPP
