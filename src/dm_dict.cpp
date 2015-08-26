#include <string.h>
#include "dm_dict.h"
#include "slog_manager.h"

extern "C"
{
  AlphaMap *trie_map(Trie *trie);
  AlphaChar alpha_map_trie_to_char(const AlphaMap *alpha_map, TrieChar tc);     
  AlphaChar *alpha_map_trie_to_char_str (const AlphaMap *alpha_map, const TrieChar *str); 
}

DmDict::DmDict()
{
  trie_ = NULL;
}

DmDict::~DmDict()
{
  if (trie_) {
    trie_free(trie_);
    trie_ = NULL;
  }
}

bool DmDict::TryInit()
{
  if (trie_) return true;
  AlphaMap *am = alpha_map_new();
  if (!am) {
    P_WARNING("failed to create alpha map");
    return false;
  }
  int ret = alpha_map_add_range(am, 0x01, 0xFF);
  if (ret < 0) {
    P_WARNING("failed to add [1,255] to alpha map");
    alpha_map_free(am);
    return false;
  }
  trie_ = trie_new(am);
  if (!trie_) {
    P_WARNING("failed to new trie");
    alpha_map_free(am);
    return false;
  }
  return true;
}

bool DmDict::Load(FILE *fp)
{
  if (!fp) {
    P_WARNING("fp is NULL");
    return false;
  }
  Trie *trie = trie_fread(fp);
  if (!trie) {
    P_WARNING("failed to load trie from fp");
    return false;
  }
  if (trie_) {
    trie_free(trie_);
  }
  trie_ = trie;
  return true;
}

bool DmDict::Save(FILE *fp) const
{
  if (!fp) {
    P_WARNING("fp is NULL");
    return false;
  }
  if (!trie_) {
    P_WARNING("trie_ is NULL");
    return false;
  }
  int ret = trie_fwrite(trie_, fp);
  if (ret < 0) {
    P_WARNING("failed to save trie from fp");
    return false;
  }
  return true;
}

bool DmDict::Add(const char *key, TrieData data)
{
  if (!key) return false;
  if (!this->TryInit()) return false;
  AlphaMap *am = trie_map(trie_);
  AlphaChar *ac = alpha_map_trie_to_char_str(am, (const TrieChar *)key);
  if (!ac) {
    P_WARNING("failed to get alpha chars: %s", key);
    return false;
  }
  bool ret = trie_store(trie_, ac, data);
  ::free(ac);
  return ret;
}

bool DmDict::AddWithMerge(const char *key, TrieData data)
{
  if (!key) return false;
  if (!this->TryInit()) return false;
  AlphaMap *am = trie_map(trie_);
  AlphaChar *ac = alpha_map_trie_to_char_str(am, (const TrieChar *)key);
  if (!ac) {
    P_WARNING("failed to get alpha chars: %s", key);
    return false;
  }
  TrieData old_data = 0;
  trie_retrieve(trie_, ac, &old_data);
  data |= old_data;
  bool ret = trie_store(trie_, ac, data);
  ::free(ac);
  return ret;
}

bool DmDict::MatchAll(const char *str, std::vector<Entry> &entries) const
{
  entries.clear();
  if (!str) return true;
  if (!trie_) return false;
  int len = strlen(str);
  AlphaMap *am = trie_map(trie_);
  AlphaChar *ac = alpha_map_trie_to_char_str(am, (const TrieChar *)str);
  TrieState *st = trie_root(trie_);
  if (!st) {
    ::free(ac);
    P_WARNING("trie_root failed");
    return false;
  }
  TrieState *bak = trie_state_clone(st);
  if (!bak) {
    trie_state_free(st);
    ::free(ac);
    P_WARNING("trie_state_clone failed");
    return false;
  }
  Entry entry;
  for (int i = 0; i < len; ++i) {
    entry.beg = i;
    trie_state_rewind(st);
    for (int j = i; j < len; ++j) {
      if (trie_state_is_walkable(st, ac[j])) {
        trie_state_walk(st, ac[j]);
      } else {
        break;
      }
      if (trie_state_is_terminal(st)) {
        trie_state_copy(bak, st);
        entry.len = j - i + 1;
        entry.data = trie_state_get_terminal_data(bak);
        entries.push_back(entry);
      }
    }
  }
  trie_state_free(bak);
  trie_state_free(st);
  ::free(ac);
  return true;
}
