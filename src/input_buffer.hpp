#pragma once

namespace ces
{
#define CHECKED_OP(x) do { if (!(x)) return false; } while(false);

  struct InputBuffer
  {
    InputBuffer();
    InputBuffer(const char* buf, size_t len);
    InputBuffer(const vector<char>& buf);
    bool Peek(char* res);
    void Rewind(size_t len);
    bool Get(char* res);
    bool OneOf(const char* str, size_t len, char* res);
    bool Expect(char ch);
    bool SkipUntil(char ch, bool consume, size_t* end = nullptr);
    bool SkipWhile(const function<bool(char)>& fn, size_t* end);
    bool Satifies(const function<bool(char)>& fn, char* ch);
    void SkipWhitespace();
    bool SubStr(size_t start, size_t len, string* res);

    static bool IsDigit(char ch);
    static bool IsAlphaNum(char ch);

    bool Consume();
    bool ConsumeIf(char ch, bool* res);
    bool Eof();

    bool InnerScope(const char* delim, bool keepDelims, InputBuffer* scope);
    
    const char* _buf;
    size_t _idx;
    size_t _len;
  };

}
