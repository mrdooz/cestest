#pragma once

namespace ces
{
  struct ResourceManager
  {
    static bool Create();
    static void Destroy();
    static ResourceManager& Instance();

    bool Init();
    bool LoadFile(const char* filename, vector<char>* buf);
    bool LoadFileAsText(const char* filename, vector<char>* buf);

    static ResourceManager* _instance;

    string _appRoot;
  };

#define RESOURCE_MANAGER ResourceManager::Instance()

}