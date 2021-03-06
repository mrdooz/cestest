#include "input_buffer.hpp"
using namespace ces;

//-----------------------------------------------------------------------------
InputBuffer::InputBuffer()
: _buf(nullptr)
, _idx(0)
, _len(0)
{
}

//-----------------------------------------------------------------------------
InputBuffer::InputBuffer(const char* buf, size_t len)
: _buf(buf)
, _idx(0)
, _len(len)
{
}

//-----------------------------------------------------------------------------
InputBuffer::InputBuffer(const vector<char>& buf)
: _buf(buf.data())
, _idx(0)
, _len(buf.size())
{
}

//-----------------------------------------------------------------------------
bool InputBuffer::Peek(char* res)
{
  if (Eof())
    return false;

  *res = _buf[_idx];
  return true;
}

//-----------------------------------------------------------------------------
void InputBuffer::Rewind(size_t len)
{
  _idx = len < _idx ? _idx - len : 0;
}

//-----------------------------------------------------------------------------
bool InputBuffer::Get(char* res)
{
  if (Eof())
    return false;

  *res = _buf[_idx++];
  // Because idx is incremented, we compare against <=, and not just <
  return _idx <= _len;
}

//-----------------------------------------------------------------------------
bool InputBuffer::Consume()
{
  if (Eof())
    return false;

  ++_idx;
  return true;
}

//-----------------------------------------------------------------------------
bool InputBuffer::ConsumeIf(char ch, bool* res)
{
  char tmp;
  CHECKED_OP(Peek(&tmp));

  bool localRes = ch == tmp;
  if (res)
    *res = localRes;

  return localRes ? Consume() : true;
}

//-----------------------------------------------------------------------------
bool InputBuffer::Eof()
{
  return _idx == _len;
}

//-----------------------------------------------------------------------------
bool InputBuffer::OneOf(const char* str, size_t len, char* res)
{
  char ch;
  *res = 0;
  CHECKED_OP(Peek(&ch));

  *res = -1;
  for (size_t i = 0; i < len; ++i)
  {
    if (ch == str[i])
    {
      CHECKED_OP(Consume());
      *res = ch;
      break;
    }
  }

  return true;
}

//-----------------------------------------------------------------------------
bool InputBuffer::Satifies(const function<bool(char)>& fn, char* res)
{
  char ch;
  CHECKED_OP(Peek(&ch));

  if (res)
    *res = ch;

  bool tmp = fn(ch);
  if (!tmp)
    return false;

  return Consume();
}

//-----------------------------------------------------------------------------
void InputBuffer::SkipWhitespace()
{
  while (true)
  {
    char ch = _buf[_idx];
    if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
    {
      _idx++;
      if (_idx == _len)
        return;
    }
    else
    {
      return;
    }
  }
}

//-----------------------------------------------------------------------------
bool InputBuffer::Expect(char ch)
{
  char tmp;
  return Get(&tmp) && tmp == ch;
}

//-----------------------------------------------------------------------------
bool InputBuffer::IsDigit(char ch)
{
  return !!isdigit(ch);
}

//-----------------------------------------------------------------------------
bool InputBuffer::IsAlphaNum(char ch)
{
  return !!isalnum(ch) || ch == '_';
}

//-----------------------------------------------------------------------------
bool InputBuffer::SkipUntil(char ch, bool consume, size_t* end)
{
  char tmp;
  while (Get(&tmp))
  {
    if (tmp == ch)
    {
      if (!consume)
        Rewind(1);
      if (end)
        *end = _idx;
      return true;
    }
  }
  return false;
}

//-----------------------------------------------------------------------------
bool InputBuffer::SkipWhile(const function<bool(char)>& fn, size_t* end)
{
  char ch;
  while (Get(&ch))
  {
    if (!fn(ch))
    {
      Rewind(1);
      *end = _idx;
      return true;
    }
  }
  return false;
}

//-----------------------------------------------------------------------------
bool InputBuffer::SubStr(size_t start, size_t len, string* res)
{
  if (start >= _len || start + len > _len)
    return false;

  res->assign(&_buf[start], len);
  return true;
}

//-----------------------------------------------------------------------------
bool InputBuffer::InnerScope(const char* delim, bool keepDelims, InputBuffer* innerScope)
{
  char open = delim[0];
  char close = delim[1];

  const char* start = &_buf[_idx];
  const char* end = &_buf[_idx + _len];
  const char* cur = start;

  // find opening delimiter
  while (cur < end)
  {
    if (*cur++ == open)
      break;
  }

  if (!keepDelims)
    cur++;

  if (cur >= end)
    return false;

  innerScope->_buf = cur - 1;
  innerScope->_idx = 0;

  // find closing
  int cnt = 0;
  while (cur < end)
  {
    char ch = *cur;
    if (ch == close)
    {
      if (cnt == 0)
      {
        // advance the current scope passed the inner scope
        _idx += cur - start + 1;

        if (!keepDelims)
          cur--;
        // found the scope
        innerScope->_len = cur - innerScope->_buf + 1;
        return true;
      }
      else
      {
        --cnt;
      }
    }
    else if (ch == open)
    {
      ++cnt;
    }
    ++cur;
  }
  return false;
}
