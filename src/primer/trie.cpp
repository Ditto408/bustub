//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// trie.cpp
//
// Identification: src/primer/trie.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/trie.h"
#include <string_view>
#include "common/exception.h"

namespace bustub {

/**
 * @brief Get the value associated with the given key.
 * 1. If the key is not in the trie, return nullptr.
 * 2. If the key is in the trie but the type is mismatched, return nullptr.
 * 3. Otherwise, return the value.
 */
template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  auto root = root_;
  if (root == nullptr) return nullptr;

  for (auto c : key) {
    if (root->children_.find(c) == root->children_.end()) {
      return nullptr;
    }
    root = root->children_.at(c);
  }

  const auto *value_node = dynamic_cast<const TrieNodeWithValue<T> *>(root.get());
  if (value_node != nullptr) {
    return value_node->value_.get();
  }
  return nullptr;
}

/**
 * @brief Put a new key-value pair into the trie. If the key already exists, overwrite the value.
 * @return the new trie.
 */

template <class T>
void SolvePut(const std::shared_ptr<bustub::TrieNode> &node, std::string_view key, T value, unsigned len) {
  if (len >= key.size()) return;
  if (node->children_.find(key[len]) != node->children_.end()) {
    if (len == key.size() - 1) {
      std::shared_ptr<T> val_p = std::make_shared<T>(std::move(value));
      auto val_node = std::make_shared<TrieNodeWithValue<T>>(node->children_[key[len]]->children_, std::move(val_p));
      node->children_[key[len]] = val_node;
    } else {
      std::shared_ptr<bustub::TrieNode> next_node = node->children_[key[len]]->Clone();
      SolvePut(next_node, key, std::move(value), len + 1);
      node->children_[key[len]] = std::shared_ptr<const TrieNode>(next_node);
    }
  } else {
    if (len == key.size() - 1) {
      std::shared_ptr<T> val_p = std::make_shared<T>(std::move(value));
      auto val_node = std::make_shared<TrieNodeWithValue<T>>(std::move(val_p));
      node->children_[key[len]] = val_node;
    } else {
      const std::shared_ptr<bustub::TrieNode> next_node = std::make_unique<TrieNode>();
      SolvePut(next_node, key, std::move(value), len + 1);
      node->children_[key[len]] = std::shared_ptr<const TrieNode>(next_node);
    }
  }
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  if (key.empty()) {
    std::shared_ptr<T> val_p = std::make_shared<T>(std::move(value));
    std::unique_ptr<TrieNodeWithValue<T>> val_node = nullptr;
    if (root_->children_.empty()) {
      val_node = std::make_unique<TrieNodeWithValue<T>>(std::move(val_p));
    } else {
      val_node = std::make_unique<TrieNodeWithValue<T>>(root_->children_, std::move(val_p));
    }
    return Trie(std::move(val_node));
  }
  std::shared_ptr<TrieNode> val_node = nullptr;
  if (root_ == nullptr) {
    val_node = std::make_unique<TrieNode>();
  } else {
    val_node = root_->Clone();
  }
  SolvePut<T>(val_node, key, std::move(value), 0);
  return Trie(std::move(val_node));
}

/**
 * @brief Remove the key from the trie.
 * @return If the key does not exist, return the original trie. Otherwise, returns the new trie.
 */

int SolveRemove(const std::shared_ptr<bustub::TrieNode> &node, std::string_view key, unsigned len) {
  if (len >= key.size()) return 1;
  if (node->children_.find(key[len]) != node->children_.end()) {
    if (len == key.size() - 1) {
      if (node->children_[key[len]]->children_.empty()) {
        node->children_.erase(key[len]);
      } else {
        auto new_node = std::make_shared<TrieNode>(node->children_[key[len]]->children_);
        node->children_[key[len]] = new_node;
      }
      return 1;
    } else {
      std::shared_ptr<bustub::TrieNode> next_node = node->children_[key[len]]->Clone();
      if (SolveRemove(next_node, key, len + 1)) {
        if (next_node->children_.empty()) {
          node->children_.erase(key[len]);
        } else {
          node->children_[key[len]] = next_node;
        }
        return 1;
      }
    }
  }
  return 0;
}

auto Trie::Remove(std::string_view key) const -> Trie {
  if (key.empty()) {
    std::unique_ptr<TrieNode> new_node = nullptr;
    if (root_->children_.empty()) {
      new_node = std::make_unique<TrieNode>();
    } else {
      new_node = std::make_unique<TrieNode>(root_->children_);
    }
    return Trie(std::move(new_node));
  }
  std::shared_ptr<TrieNode> new_node = nullptr;
  if (root_ == nullptr) {
    return *this;
  } else {
    new_node = root_->Clone();
  }
  SolveRemove(new_node, key, 0);
  return Trie(std::move(new_node));
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
