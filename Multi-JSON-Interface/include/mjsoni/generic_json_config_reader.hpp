/**
 * Multi JSON Interface
 * Copyright (C) 2019  Mir Drualga
 *
 * This file is part of Multi JSON Interface.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef MJSONI_GENERIC_JSON_CONFIG_READER_HPP_
#define MJSONI_GENERIC_JSON_CONFIG_READER_HPP_

#include <cstdint>
#include <deque>
#include <filesystem>
#include <initializer_list>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace mjsoni {

template<typename DOC, typename OBJ, typename VAL>
class GenericConfigReader {
  using JsonDocument = DOC;
  using JsonObject = OBJ;
  using JsonValue = VAL;

 public:
  GenericConfigReader() = delete;

  explicit GenericConfigReader(
      std::filesystem::path config_file_path
  );

  /* Read and Write */

  bool Read();

  bool Write(int indent_width);

  /* Functions for Generic Types */

  template <typename ...Args>
  bool ContainsKey(
      const Args&... keys
  ) const;

  template <typename ...Args>
  const JsonValue& GetValueRef(
      const Args&... keys
  ) const;

  template <typename Container, typename ...Args>
  Container GetArrayCopy(
      const Args&... keys
  ) const;

  template <typename Iter, typename ...Args>
  void SetArray(
      Iter first,
      Iter last,
      const Args&... keys
  );

  template <typename Iter, typename ...Args>
  void SetDeepArray(
      Iter first,
      Iter last,
      const Args&... keys
  );

  template <typename ...Args>
  void SetValue(
      JsonValue value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepValue(
      JsonValue value,
      const Args&... keys
  );

  /* Functions for bool */

  template <typename ...Args>
  bool GetBool(
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool GetBoolOrDefault(
      bool default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasBool(
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetBool(
      bool value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepBool(
      bool value,
      const Args&... keys
  );

  /* Functions for std::deque */

  template <typename T, typename ...Args>
  std::deque<T> GetDeque(
      const Args&... keys
  ) const;

  template <typename T, typename ...Args>
  std::deque<T> GetDequeOrDefault(
      const std::deque<T>& default_value,
      const Args&... keys
  ) const;

  template <typename T, typename ...Args>
  std::deque<T> GetDequeOrDefault(
      std::deque<T>&& default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasDeque(
      const Args&... keys
  ) const;

  template <typename T, typename ...Args>
  void SetDeque(
      const std::deque<T>& value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeque(
      const std::deque<std::string_view>& value,
      const Args&... keys
  );

  template <typename T, typename ...Args>
  void SetDeque(
      std::deque<T>&& value,
      const Args&... keys
  );

  template <typename T, typename ...Args>
  void SetDeepDeque(
      const std::deque<T>& value,
      const Args&... keys
  );

  template <typename T, typename ...Args>
  void SetDeepDeque(
      std::deque<T>&& value,
      const Args&... keys
  );

  /* Functions for int */

  template <typename ...Args>
  int GetInt(
      const Args&... keys
  ) const;

  template <typename ...Args>
  int GetIntOrDefault(
      int default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasInt(
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetInt(
      int value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepInt(
      int value,
      const Args&... keys
  );

  /* Functions for std::int32_t */

  template <typename ...Args>
  std::int32_t GetInt32(
      const Args&... keys
  ) const;

  template <typename ...Args>
  std::int32_t GetInt32OrDefault(
      std::int32_t default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasInt32(
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetInt32(
      std::int32_t value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepInt32(
      std::int32_t value,
      const Args&... keys
  );

  /* Functions for std::int64_t */

  template <typename ...Args>
  std::int64_t GetInt64(
      const Args&... keys
  ) const;

  template <typename ...Args>
  std::int64_t GetInt64OrDefault(
      std::int64_t default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasInt64(
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetInt64(
      std::int64_t value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepInt64(
      std::int64_t value,
      const Args&... keys
  );

  /* Functions for long */

  template <typename ...Args>
  long GetLong(
      const Args&... keys
  ) const;

  template <typename ...Args>
  long GetLongOrDefault(
      long default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasLong(
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetLong(
      long value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepLong(
      long value,
      const Args&... keys
  );

  /* Functions for long long */

  template <typename ...Args>
  long long GetLongLong(
      const Args&... keys
  ) const;

  template <typename ...Args>
  long long GetLongLongOrDefault(
      long long default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasLongLong(
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetLongLong(
      long long value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepLongLong(
      long long value,
      const Args&... keys
  );

  /* Functions for std::filesystem::path */

  template <typename ...Args>
  std::filesystem::path GetPath(
      const Args&... keys
  ) const;

  template <typename ...Args>
  std::filesystem::path GetPathOrDefault(
      const std::filesystem::path& default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  std::filesystem::path GetPathOrDefault(
      std::filesystem::path&& default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasPath(
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetPath(
      const std::filesystem::path& value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepPath(
      const std::filesystem::path& value,
      const Args&... keys
  );

  /* Functions for std::set */

  template <typename T, typename ...Args>
  std::set<T> GetSet(
      const Args&... keys
  ) const;

  template <typename T, typename ...Args>
  std::set<T> GetSetOrDefault(
      const std::set<T>& default_value,
      const Args&... keys
  ) const;

  template <typename T, typename ...Args>
  std::set<T> GetSetOrDefault(
      std::set<T>&& default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasSet(
      const Args&... keys
  ) const;

  template <typename T, typename ...Args>
  void SetSet(
      const std::set<T>& value,
      const Args&... keys
  );

  template <typename T, typename ...Args>
  void SetSet(
      std::set<T>&& value,
      const Args&... keys
  );

  template <typename T, typename ...Args>
  void SetDeepSet(
      const std::set<T>& value,
      const Args&... keys
  );

  template <typename T, typename ...Args>
  void SetDeepSet(
      std::set<T>&& value,
      const Args&... keys
  );

  /* Functions for std::string */

  template <typename ...Args>
  std::string GetString(
      const Args&... keys
  ) const;

  template <typename ...Args>
  std::string GetStringOrDefault(
      const std::string& default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  std::string GetStringOrDefault(
      std::string&& default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasString(
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetString(
      const std::string& value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetString(
      std::string&& value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepString(
      const std::string& value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepString(
      std::string&& value,
      const Args&... keys
  );

  /* Functions for unsigned int */

  template <typename ...Args>
  unsigned int GetUnsignedInt(
      const Args&... keys
  ) const;

  template <typename ...Args>
  unsigned int GetUnsignedIntOrDefault(
      unsigned int default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasUnsignedInt(
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetUnsignedInt(
      unsigned int value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepUnsignedInt(
      unsigned int value,
      const Args&... keys
  );

  /* Functions for std::uint32_t */

  template <typename ...Args>
  std::uint32_t GetUnsignedInt32(
      const Args&... keys
  ) const;

  template <typename ...Args>
  std::uint32_t GetUnsignedInt32OrDefault(
      std::uint32_t default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasUnsignedInt32(
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetUnsignedInt32(
      std::uint32_t value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepUnsignedInt32(
      std::uint32_t value,
      const Args&... keys
  );

  /* Functions for std::uint64_t */

  template <typename ...Args>
  std::uint64_t GetUnsignedInt64(
      const Args&... keys
  ) const;

  template <typename ...Args>
  std::uint64_t GetUnsignedInt64OrDefault(
      std::uint64_t default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasUnsignedInt64(
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetUnsignedInt64(
      std::uint64_t value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepUnsignedInt64(
      std::uint64_t value,
      const Args&... keys
  );

  /* Functions for unsigned long */

  template <typename ...Args>
  unsigned long GetUnsignedLong(
      const Args&... keys
  ) const;

  template <typename ...Args>
  unsigned long GetUnsignedLongOrDefault(
      unsigned long default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasUnsignedLong(
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetUnsignedLong(
      unsigned long value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepUnsignedLong(
      unsigned long value,
      const Args&... keys
  );

  /* Functions for unsigned long long */

  template <typename ...Args>
  unsigned long long GetUnsignedLongLong(
      const Args&... keys
  ) const;

  template <typename ...Args>
  unsigned long long GetUnsignedLongLongOrDefault(
      unsigned long long default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasUnsignedLongLong(
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetUnsignedLongLong(
      unsigned long long value,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepUnsignedLongLong(
      unsigned long long value,
      const Args&... keys
  );

  /* Functions for std::unordered_set */

  template <typename T, typename ...Args>
  std::unordered_set<T> GetUnorderedSet(
      const Args&... keys
  ) const;

  template <typename T, typename ...Args>
  std::unordered_set<T> GetUnorderedSetOrDefault(
      const std::unordered_set<T>& default_value,
      const Args&... keys
  ) const;

  template <typename T, typename ...Args>
  std::unordered_set<T> GetUnorderedSetOrDefault(
      std::unordered_set<T>&& default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasUnorderedSet(
      const Args&... keys
  ) const;

  template <typename T, typename ...Args>
  void SetUnorderedSet(
      const std::unordered_set<T>& value,
      const Args&... keys
  );

  template <typename T, typename ...Args>
  void SetUnorderedSet(
      std::unordered_set<T>&& value,
      const Args&... keys
  );

  template <typename T, typename ...Args>
  void SetDeepUnorderedSet(
      const std::unordered_set<T>& value,
      const Args&... keys
  );

  template <typename T, typename ...Args>
  void SetDeepUnorderedSet(
      std::unordered_set<T>&& value,
      const Args&... keys
  );

  /* Functions for std::vector */

  template <typename T, typename ...Args>
  std::vector<T> GetVector(
      const Args&... keys
  ) const;

  template <typename T, typename ...Args>
  std::vector<T> GetVectorOrDefault(
      const std::vector<T>& default_value,
      const Args&... keys
  ) const;

  template <typename T, typename ...Args>
  std::vector<T> GetVectorOrDefault(
      std::vector<T>&& default_value,
      const Args&... keys
  ) const;

  template <typename ...Args>
  bool HasVector(
      const Args&... keys
  ) const;

  template <typename T, typename ...Args>
  void SetVector(
      const std::vector<T>& value,
      const Args&... keys
  );

  template <typename T, typename ...Args>
  void SetVector(
      std::vector<T>&& value,
      const Args&... keys
  );

  template <typename T, typename ...Args>
  void SetDeepVector(
      const std::vector<T>& value,
      const Args&... keys
  );

  template <typename T, typename ...Args>
  void SetDeepVector(
      std::vector<T>&& value,
      const Args&... keys
  );

  /* Getter and Setters */

  constexpr const std::filesystem::path& config_file_path() const noexcept {
    return this->config_file_path_;
  }

  constexpr const JsonDocument& json_document() const noexcept {
    return this->json_document_;
  }

 private:
  std::filesystem::path config_file_path_;
  JsonDocument json_document_;

  template <typename ...Args>
  bool ContainsKeyRecursive(
      const JsonObject& object,
      std::string_view current_key,
      const Args&... keys
  ) const;

  template <typename ...Args>
  JsonValue& GetValueRefRecursive(
      JsonObject& object,
      std::string_view current_key,
      const Args&... keys
  );

  template <typename ...Args>
  const JsonValue& GetValueRefRecursive(
      const JsonObject& object,
      std::string_view current_key,
      const Args&... keys
  ) const;

  template <typename ...Args>
  void SetValueRecursive(
      JsonValue value,
      JsonObject& object,
      std::string_view current_key,
      const Args&... keys
  );

  template <typename ...Args>
  void SetDeepValueRecursive(
      JsonValue value,
      JsonObject& object,
      std::string_view current_key,
      const Args&... keys
  );
};

} // namespace mjsoni

#endif // MJSONI_GENERIC_JSON_CONFIG_READER_HPP_
