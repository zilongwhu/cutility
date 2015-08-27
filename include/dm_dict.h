#ifndef __CUTILITY_DM_DICT_H__
#define __CUTILITY_DM_DICT_H__

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include "datrie/trie.h"

class DmDict
{
  public:
    struct Entry
    {
      int32_t beg;
      uint32_t len;
      TrieData data;
    };
  private:
    DmDict(const DmDict &);
    DmDict &operator =(const DmDict &);
  public:
    DmDict();
    ~DmDict();
  public:
    bool Load(FILE *fp);
    bool Save(FILE *fp) const;
  public:
    bool Add(const char *key, TrieData data);
    bool AddWithMerge(const char *key, TrieData data);
  public:
    bool MatchAll(const char *str, std::vector<Entry> &entries) const;
    bool MatchAny(const char *str, Entry &entry) const;
  private:
    bool TryInit();
  private:
    Trie *trie_;
};

#endif
