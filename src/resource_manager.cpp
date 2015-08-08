#include "resource_manager.hpp"
#include "path_utils.hpp"
#include "utils.hpp"

using namespace ces;

ResourceManager* ResourceManager::_instance;

//------------------------------------------------------------------------------
bool ResourceManager::Create()
{
  assert(!_instance);
  _instance = new ResourceManager();
  return _instance->Init();
}

bool ResourceManager::Init()
{
  _appRoot = "/Users/dooz/projects/cestest";

  return true;
}

//------------------------------------------------------------------------------
bool ResourceManager::LoadFile(const char* filename, vector<char>* buf)
{
  string fullPath = Path::Join(_appRoot, filename);
  return ces::LoadFile(fullPath.c_str(), false, buf);
}

//------------------------------------------------------------------------------
bool ResourceManager::LoadFileAsText(const char* filename, vector<char>* buf)
{
  string fullPath = Path::Join(_appRoot, filename);
  return ces::LoadFile(fullPath.c_str(), true, buf);
}

//------------------------------------------------------------------------------
ResourceManager& ResourceManager::Instance()
{
  return *_instance;
}