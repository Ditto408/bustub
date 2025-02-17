//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// trie_store.cpp
//
// Identification: src/primer/trie_store.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/trie_store.h"
#include "common/exception.h"

namespace bustub {

/**
 * @brief This function returns a ValueGuard object that holds a reference to the value in the trie. If
 * the key does not exist in the trie, it will return std::nullopt.
 */
template <class T>
auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<T>> {
  std::lock_guard<std::mutex> root_lock(root_lock_);
  Trie trie = root_;
  auto val = trie.Get<T>(key);
  if (val != nullptr) {
    return ValueGuard<T>(trie, *val);
  } else {
    return std::nullopt;
  }
}

/**
 * @brief This function will insert the key-value pair into the trie. If the key already exists in the
 * trie, it will overwrite the value.
 */
template <class T>
void TrieStore::Put(std::string_view key, T value) {
  std::lock_guard<std::mutex> write_lock(write_lock_);
  Trie new_trie = root_.Put<T>(key, std::move(value));
  std::lock_guard<std::mutex> root_lock(root_lock_);
  root_ = new_trie;
}

/** @brief This function will remove the key-value pair from the trie. */
void TrieStore::Remove(std::string_view key) {
  std::lock_guard<std::mutex> write_lock(write_lock_);
  Trie new_trie = root_.Remove(key);
  std::lock_guard<std::mutex> root_lock(root_lock_);
  root_ = new_trie;
}

// Below are explicit instantiation of template functions.

template auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<uint32_t>>;
template void TrieStore::Put(std::string_view key, uint32_t value);

template auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<std::string>>;
template void TrieStore::Put(std::string_view key, std::string value);

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<Integer>>;
template void TrieStore::Put(std::string_view key, Integer value);

template auto TrieStore::Get(std::string_view key) -> std::optional<ValueGuard<MoveBlocked>>;
template void TrieStore::Put(std::string_view key, MoveBlocked value);

}  // namespace bustub
