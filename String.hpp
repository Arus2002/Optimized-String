#ifndef STRING_HPP
#define STRINH_HPP

#include <iostream>
#include <string>

class String {
public:
    String(const char* str = "");
    String(const String& rhs);
	String(const std::string& str);

    String(String&& rhs) noexcept;
    String& operator=(String&& rhs) noexcept;
    ~String();

    char& operator[](std::size_t index);
    String& operator=(const char* rhs);
    String& operator=(const String& rhs);
    String& operator=(const std::string& rhs);
    String& operator+=(String& rhs);
    String& operator+=(const char rhs);
    friend std::ostream& operator<<(std::ostream &out, const String& str);

public:
    std::size_t size() const;
    bool empty() const;
    char* c_str() const;

private:
    struct dynStr {
         std::size_t m_size; 
         char* m_buff; 
    };
    union {
        dynStr str;
        char onstack[16];
    } string;

private:
    bool fromHeap;
    void resize();
    void resize(std::size_t size);
    void resizeAfterReset();

    void cleanup() noexcept;
    void moveFrom(String& src) noexcept;
};

#endif