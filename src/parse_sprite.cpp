#include "parse_sprite.hpp"
#include "input_buffer.hpp"
#include "parse_base.hpp"
#include "logging.hpp"
#include "resource_manager.hpp"

namespace ces
{
  //------------------------------------------------------------------------------
  bool SkipToken(InputBuffer& buf, char token)
  {
    buf.SkipWhitespace();
    if (!buf.Expect(token))
      return false;
    buf.SkipWhitespace();
    return true;
  }

  //------------------------------------------------------------------------------
  struct KeywordParser
  {
    KeywordParser(InputBuffer& input) : input(input) {}
    bool Run()
    {
      input.SkipWhitespace();
      while (!input.Eof())
      {
        input.SkipWhitespace();
        string keyword;
        CHECKED_OP(ParseIdentifier(input, &keyword, true));
        input.SkipWhitespace();

        auto it = evals.find(keyword);
        if (it == evals.end())
        {
          LOG_ERROR("Unknown keyword found: ", keyword);
          return false;
        }

        if (!it->second())
          return false;

        CHECKED_OP(SkipToken(input, ';'));
      }
      return true;
    }

    void AddEval(const string& keyword, const function<bool()>& fn)
    {
      evals[keyword] = fn;
    }

    unordered_map<string, function<bool()>> evals;
    InputBuffer& input;
  };

  //------------------------------------------------------------------------------
  bool ParseSpriteSheetSettings(InputBuffer& buf, SpriteSheetSettings* settings)
  {
    KeywordParser k(buf);
    k.AddEval("filename", [&]() {
      return ParseIdentifier(buf, &settings->filename, false);
    });
    k.AddEval("size", [&]() {
      return ParseVec2(buf, &settings->size);
    });

    return k.Run();
  }

  //------------------------------------------------------------------------------
  bool ParseSingleSprite(InputBuffer& buf, vector<SpriteSettings>* sprites)
  {
    string name;
    string sub;
    Vector2 offset = Vector2{0, 0};
    Vector2 size = Vector2{0, 0};
    int repeat = 1;
    int spacing = 0;

    KeywordParser k(buf);
    k.AddEval("name", [&]() { return ParseIdentifier(buf, &name, false); });
    k.AddEval("sub", [&]() { return ParseIdentifier(buf, &sub, false); });
    k.AddEval("offset", [&]() { return ParseVec2(buf, &offset); });
    k.AddEval("size", [&]() { return ParseVec2(buf, &size); });
    k.AddEval("repeat", [&]() { return ParseInt(buf, &repeat); });
    k.AddEval("spacing", [&]() { return ParseInt(buf, &spacing); });

    if (!k.Run())
      return false;

    // create the sprite settings from the input
    Vector2 pos = offset;
    for (int i = 0; i < repeat; ++i)
    {
      sprites->push_back(SpriteSettings{name, sub, pos, size});
      pos.x += size.x + spacing;
      pos.y += size.y + spacing;
    }

    return true;
  }

  //------------------------------------------------------------------------------
  bool ParseSpriteSheet(const char* filename, SpriteSheetSettings* sheet)
  {
    vector<char> buf;
    if (!ResourceManager::Instance().LoadFile(filename, &buf))
      return false;

    InputBuffer input(buf);
    KeywordParser k(input);

    InputBuffer inner;

    k.AddEval("sprite", [&]() {
      input.InnerScope("{}", false, &inner);
      return ParseSingleSprite(inner, &sheet->sprites);
    });
    
    k.AddEval("sheet", [&]() {
      input.InnerScope("{}", false, &inner);
      return ParseSpriteSheetSettings(inner, sheet);
    });
    
    return k.Run();
  }
}
