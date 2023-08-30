#include "String.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

String::String(const char* str) 
    : string{0, nullptr}
    , fromHeap{false}
{
    std::cout << "ctor with const char*" << std::endl;
    std::size_t size = 0;
    while (str[size] != '\0') {
        ++size;
    }
    if (size >= 15) {
        fromHeap = true;

        string.str.m_size = size + 1;
        string.str.m_buff = new char[string.str.m_size];

        for (int i = 0; i < size; ++i) {
            string.str.m_buff[i] = str[i];
        }
        string.str.m_buff[size] = '\0';
    }
    else {
        for (int i = 0; i < size; ++i) {
            string.onstack[i] = str[i];
        }
        string.onstack[size] = '\0';
    }
}

String::String(const String& str)
    : string{str.string.str.m_size, nullptr}
    , fromHeap{false}
{
    std::cout << "ctor with String" << std::endl;
    if (str.fromHeap && str.string.str.m_size >= 15) {
        fromHeap = true;
        string.str.m_size = str.string.str.m_size;
        string.str.m_buff = new char[string.str.m_size + 1];
        for (int i = 0; str.string.str.m_buff[i] != '\0'; ++i) {
            string.str.m_buff[i] = str.string.str.m_buff[i];
        }
        string.str.m_buff[string.str.m_size] = '\0';
    }
    else {
        int size = 0;
        for (int i = 0; str.string.onstack[i] != '\0'; ++i) {
            string.onstack[i] = str.string.onstack[i];
            ++size;
        } 
        string.onstack[size] = '\0';
    }
}

String::String(const std::string& str) 
    : string{str.size() + 1, nullptr}
    , fromHeap{false} 
{
    std::cout << "ctor with std::string" << std::endl;
    if (str.size() >= 15) {
        fromHeap = true;
        string.str.m_size = str.size();
        string.str.m_buff = new char[string.str.m_size + 1];
        for (int i = 0; i < str.size(); ++i) {
            string.str.m_buff[i] = str[i];
        }
        string.str.m_buff[string.str.m_size] = '\0';
    }
    else {
        int size = 0;
        for (int i = 0; i < str.size(); ++i) {
            string.onstack[i] = str[i];
            ++size;
        } 
        string.onstack[size] = '\0';
    }
}

String::~String() {
    if (fromHeap) {
        delete[] string.str.m_buff;
    }
}

char& String::operator[](std::size_t index) {
    if (fromHeap == true && index < string.str.m_size && index > 0) {
        return string.str.m_buff[index];
    }
    if (index >= 15 || index < 0) {
        throw std::runtime_error("Value is out of the valid range");
    }
    return string.onstack[index];
}

std::ostream& operator<<(std::ostream &out, const String& str) {
    if (str.fromHeap == true) {
        for (int i = 0; str.string.str.m_buff[i] != '\0'; ++i) {
                out << str.string.str.m_buff[i];
        }
    }
    else {
        for (int i = 0; str.string.onstack[i] != '\0'; ++i) {
            out << str.string.onstack[i];
        }
    }
    return out;
}

String& String::operator=(const String& rhs) {
    if (this == &rhs) {
        return *this;
    }
    std::cout << "operator = with String" << std::endl;
    std::size_t size = rhs.size();
    if (size >= 15 && fromHeap) {
        fromHeap = true;
        if (size != string.str.m_size) {
            delete[] string.str.m_buff;
            string.str.m_buff = new char[string.str.m_size];
        }
        if (size >= string.str.m_size) {
            resize();
        }
        for (int i = 0; rhs.string.str.m_buff[i] != '\0'; ++i) {
            string.str.m_buff[i] = rhs.string.str.m_buff[i];
        }
        for (int i = size; i < string.str.m_size; ++i) {
            string.str.m_buff[i] = '\0';
        }
        string.str.m_buff[size] = '\0';
        string.str.m_size = size;
        return *this;
    }
    else if (size < 15 && fromHeap) {
        fromHeap = false;
        for (int i = 0; rhs.string.onstack[i] != '\0'; ++i) {
            string.onstack[i] = rhs.string.onstack[i];
        }
        string.onstack[size] = '\0';
        delete[] string.str.m_buff;
        return *this;
    }
    else if (size >= 15 && !fromHeap) {
        fromHeap = true;
        for (int i = 0; string.onstack[i] != '\0'; ++i) {
            string.onstack[i] = '\0';
        }
        string.str.m_buff = new char[size + 1];
        for (int i = 0; i < size; ++i) {
            string.str.m_buff[i] = rhs.string.str.m_buff[i];
        }
        string.str.m_buff[size] = '\0';
        string.str.m_size = size;
        return *this;
    }
    else if (size < 15 && !fromHeap) {
        fromHeap = false;
        for (int i = 0; rhs.string.onstack[i] != '\0'; ++i) {
            string.onstack[i] = rhs.string.onstack[i];
        }
        string.onstack[size] = '\0';
        return *this;
    }
    throw std::runtime_error("error");
}

String& String::operator=(const char* rhs) {
    std::cout << "operator = with const char*" << std::endl;
    int size = 0;
    for (int i = 0; rhs[i] != '\0'; ++i) {
        ++size;
    }
    if (size >= 15 && fromHeap) {
        if (size != string.str.m_size) {
            delete[] string.str.m_buff;
            string.str.m_buff = new char[string.str.m_size];
        }
        fromHeap = true;
        if (size >= string.str.m_size) {
            resize();
        }
        for (int i = 0; rhs[i] != '\0'; ++i) {
            string.str.m_buff[i] = rhs[i];
        }
        for (int i = size; i < string.str.m_size; ++i) {
            string.str.m_buff[i] = '\0';
        }
        string.str.m_buff[size] = '\0';
        string.str.m_size = size;
        return *this;
    }
    else if (size < 15 && fromHeap) {
        fromHeap = false;
        for (int i = 0; rhs[i] != '\0'; ++i) {
            string.onstack[i] = rhs[i];
        }
        string.onstack[size] = '\0';
        delete[] string.str.m_buff;
        return *this;
    }
    else if (size >= 15 && !fromHeap) {
        fromHeap = true;
        for (int i = 0; string.onstack[i] != '\0'; ++i) {
            string.onstack[i] = '\0';
        }
        string.str.m_buff = new char[size + 1];
        for (int i = 0; i < size; ++i) {
            string.str.m_buff[i] = rhs[i];
        }
        string.str.m_buff[size] = '\0';
        string.str.m_size = size;
        return *this;
    }
    else if (size < 15 && !fromHeap) {
        fromHeap = false;
        for (int i = 0; rhs[i] != '\0'; ++i) {
            string.onstack[i] = rhs[i];
        }
        string.onstack[size] = '\0';
        return *this;
    }
    throw std::runtime_error("error");
}

String& String::operator=(const std::string& rhs) {
    std::cout << "operator = with std::string" << std::endl;
    if (rhs.size() >= 15 && fromHeap) {
        if (rhs.size() != string.str.m_size) {
            delete[] string.str.m_buff;
            string.str.m_buff = new char[string.str.m_size];
        }
        fromHeap = true;
        if (rhs.size() >= string.str.m_size) {
            resize();
        }
        for (int i = 0; rhs[i] != '\0'; ++i) {
            string.str.m_buff[i] = rhs[i];
        }
        for (int i = rhs.size(); i < string.str.m_size; ++i) {
            string.str.m_buff[i] = '\0';
        }
        string.str.m_buff[rhs.size()] = '\0';
        string.str.m_size = rhs.size();
        return *this;
    }
    else if (rhs.size() < 15 && fromHeap) {
        fromHeap = false;
        for (int i = 0; rhs[i] != '\0'; ++i) {
            string.onstack[i] = rhs[i];
        }
        string.onstack[rhs.size()] = '\0';
        delete[] string.str.m_buff;
        return *this;
    }
    else if (rhs.size() >= 15 && !fromHeap) {
        fromHeap = true;
        for (int i = 0; string.onstack[i] != '\0'; ++i) {
            string.onstack[i] = '\0';
        }
        string.str.m_buff = new char[rhs.size() + 1];
        for (int i = 0; i < rhs.size(); ++i) {
            string.str.m_buff[i] = rhs[i];
        }
        string.str.m_buff[rhs.size()] = '\0';
        string.str.m_size = rhs.size();
        return *this;
    }
    else if (rhs.size() < 15 && !fromHeap) {
        fromHeap = false;
        for (int i = 0; rhs[i] != '\0'; ++i) {
            string.onstack[i] = rhs[i];
        }
        string.onstack[rhs.size()] = '\0';
        return *this;
    }
    throw std::runtime_error("error");
}

String::String(String&& rhs) noexcept 
    : fromHeap(rhs.fromHeap)
{
    if (fromHeap) {
        moveFrom(rhs);
    }
    else {
        for (std::size_t i = 0; i < sizeof(string.onstack); ++i) {
            string.onstack[i] = rhs.string.onstack[i];
        }
    }
    std::cout << "Move Constructor" << std::endl;
}

String& String::operator=(String&& rhs) noexcept {
    if (this == &rhs) {
        return *this;
    }

    cleanup();

    fromHeap = rhs.fromHeap;
    if (fromHeap) {
        moveFrom(rhs);
    }
    else {
        for (std::size_t i = 0; i < sizeof(string.onstack); ++i) {
            string.onstack[i] = rhs.string.onstack[i];
        }
    }
    std::cout << "Move Assignment Operator" << std::endl;

    return *this;
}

String& String::operator+=(String& rhs) {
    std::size_t sizeRhs = rhs.size();
    std::size_t size = this -> size();
    if (size + sizeRhs >= 15) {
        if (sizeRhs >= 15 && fromHeap) {
            fromHeap = true;
            if (string.str.m_size <= size + sizeRhs) {
                resize(size + sizeRhs);
            }
            for (int i = 0; rhs.string.str.m_buff[i] != '\0'; ++i) {
                string.str.m_buff[size + i] = rhs.string.str.m_buff[i];
            }
            string.str.m_buff[size + sizeRhs] = '\0';
            return *this;
        }
        else if (sizeRhs < 15 && fromHeap) {
            fromHeap = true;
            if (string.str.m_size <= string.str.m_size + sizeRhs) {
                resize(string.str.m_size + sizeRhs);
            }
            for (int i = 0; rhs.string.onstack[i] != '\0'; ++i) {
                string.str.m_buff[size + i] = rhs.string.onstack[i];
            }
            string.str.m_buff[size + sizeRhs] = '\0';
            return *this;
        }
        else if (sizeRhs >= 15 && !fromHeap) {
            fromHeap = true;
            std::string tmp;
            for (int i = 0; i < size; ++i) {
                tmp += string.onstack[i];
            }
            string.str.m_buff = new char[size + sizeRhs + 1];
            for (int i = 0; tmp[i] != '\0'; ++i) {
                string.str.m_buff[i] = tmp[i];
            }
            for (int i = 0; rhs.string.str.m_buff[i] != '\0' ; ++i) {
                string.str.m_buff[size + i] = rhs.string.str.m_buff[i];
            }
            string.str.m_buff[size + sizeRhs] = '\0';
            return *this;
        }
        else if (sizeRhs < 15 && !fromHeap) {
            fromHeap = true;
            std::string tmp;
            for (int i = 0; i < size; ++i) {
                tmp += string.onstack[i];
            }
            string.str.m_buff = new char[size + sizeRhs + 1];
            for (int i = 0; tmp[i] != '\0'; ++i) {
                string.str.m_buff[i] = tmp[i];
            }
            for (int i = 0; rhs.string.onstack[i] != '\0'; ++i) {
                string.str.m_buff[size + i] = rhs.string.onstack[i];
            }
            string.str.m_buff[size + sizeRhs] = '\0';
            return *this;
        }
    }
    if (size + sizeRhs < 15) {
        for (int i = 0; rhs.string.onstack[i] != '\0'; ++i) {
            string.onstack[size + i] += rhs.string.onstack[i];
        }
        string.onstack[size + sizeRhs] = '\0';
        return *this;
    }
    throw std::runtime_error("error");
}

String& String::operator+=(const char rhs) {

    std::size_t size = this -> size();

    if (size + 1 >= 15) {
        if (fromHeap) {
            fromHeap = true;
            if (string.str.m_size <= size + 1) {
                resize(size + 1);
            }
            string.str.m_buff[size] = rhs;
            string.str.m_buff[size + 1] = '\0';
            return *this;
        }
        else {
            fromHeap = true;
            std::string tmp;
            for (int i = 0; i < size; ++i) {
                tmp += string.onstack[i];
            }
            string.str.m_buff = new char[size + 1];
            for (int i = 0; tmp[i] != '\0'; ++i) {
                string.str.m_buff[i] = tmp[i];
            }
            string.str.m_buff[size] = rhs;
            string.str.m_buff[size + 1] = '\0';
            return *this;
        }
    }
    else {
        string.onstack[size] = rhs;
        string.onstack[size + 1] = '\0';
        return *this;
    }
    throw std::runtime_error("error");
}

void String::resize() {
    char* tmp = new char[string.str.m_size * 2];
    for (int i = 0; string.str.m_buff[i] != '\0'; ++i) {
        tmp[i] = string.str.m_buff[i];
    }
    string.str.m_size *= 2;
    delete[] string.str.m_buff;
    string.str.m_buff = tmp;
}

void String::resize(std::size_t size) {
    if (string.str.m_size == 0) {
        resize();
    }
    if (size <= string.str.m_size) {
        return;
    }
    char* tmp = new char[size + 1];
    for (int i = 0; i < string.str.m_buff[i] != '\0'; ++i) {
        tmp[i] = string.str.m_buff[i];
    }
    string.str.m_size = size + 1;
    delete[] string.str.m_buff;
    string.str.m_buff = tmp;
}

void String::cleanup() noexcept {
    if (fromHeap) {
        delete[] string.str.m_buff;
        string.str.m_buff = nullptr;
        string.str.m_size = 0;
    }
}

void String::moveFrom(String& src) noexcept {
    if (fromHeap) {
        string.str.m_buff = src.string.str.m_buff;
        string.str.m_size = src.string.str.m_size;

        src.string.str.m_buff = nullptr;
        src.string.str.m_size = 0;
        return;
    }
    for (int i = 0; string.onstack[i] != '\0'; ++i) {
        string.onstack[i] = src.string.onstack[i];
        src.string.onstack[i] = '\0';
    }
}

std::size_t String::size() const {
    int size = 0;
    if (fromHeap) {
        for (int i = 0; string.str.m_buff[i] != '\0'; ++i) {
            ++size;
        }
    }
    else {
        for (int i = 0; string.onstack[i] != '\0'; ++i) {
            ++size;
        }
    }
    return size;
}

bool String::empty() const {
    std::size_t size = this -> size();
    if (size == 1) {
        return true; 
    }
    return false;
}

char* String::c_str() const {
    if (fromHeap) {
        return string.str.m_buff;
    }
    return (char*)string.onstack;
}