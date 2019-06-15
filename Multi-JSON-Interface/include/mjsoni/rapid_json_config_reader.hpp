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

#ifndef MJSONI_RAPID_JSON_CONFIG_READER_HPP_
#define MJSONI_RAPID_JSON_CONFIG_READER_HPP_

#include <cstdarg>
#include <fstream>
#include <string_view>
#include <utility>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include "generic_json_config_reader.hpp"

namespace mjsoni {

using RapidJsonConfigReader = GenericConfigReader<rapidjson::Document, rapidjson::Value, rapidjson::Value>;

/* Constructors and Destructors */

template <>
RapidJsonConfigReader::GenericConfigReader(
    std::filesystem::path config_file_path
) : config_file_path_(std::move(config_file_path)) {
}

/* Functions for Generic Types */

template <>
template <typename ...Args>
bool RapidJsonConfigReader::ContainsKey(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  return this->ContainsKeyRecursive(
      this->json_document(),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
const RapidJsonConfigReader::JsonValue&
RapidJsonConfigReader::GetValueRef(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  return this->GetValueRefRecursive(
      this->json_document_,
      first_key,
      additional_keys...
  );
}

template <>
template <typename Container, typename ...Args>
Container RapidJsonConfigReader::GetArrayCopy(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  const rapidjson::Value::ConstArray& array_ref = value_ref.GetArray();

  Container container;
  if constexpr (std::is_same<Container::value_type, std::string>::value
      || std::is_same<Container::value_type, std::string_view>::value) {
    for (rapidjson::Value::ConstValueIterator it = array_ref.begin(); it != array_ref.end(); it++) {
      container.insert(
          container.end(),
          it->GetString()
      );
    }
  } else {
    for (rapidjson::Value::ConstValueIterator it = array_ref.begin(); it != array_ref.end(); it++) {
      container.insert(
          container.end(),
          it->template Get<Container::value_type>()
      );
    }
  }

  return container;
}

template <>
template <typename Iter, typename ...Args>
void RapidJsonConfigReader::SetArray(
    Iter first,
    Iter last,
    std::string_view first_key,
    const Args&... additional_keys
) {
  rapidjson::Value json_array(rapidjson::kArrayType);

  if constexpr (std::is_same<Iter::value_type, std::string>::value
      || std::is_same<Iter::value_type, std::string_view>::value) {
    for (Iter it = first; it != last; it += 1) {
      json_array.PushBack(
          rapidjson::Value(it->data(), this->json_document_.GetAllocator()),
          this->json_document_.GetAllocator()
      );
    }
  } else if constexpr (std::is_same<Iter::value_type, char*>::value
      || std::is_same<Iter::value_type, const char*>::value) {
    for (Iter it = first; it != last; it += 1) {
      json_array.PushBack(
          rapidjson::Value(*it, this->json_document_.GetAllocator()),
          this->json_document_.GetAllocator()
      );
    }
  } else {
    for (Iter it = first; it != last; it += 1) {
      json_array.PushBack(*it, this->json_document_.GetAllocator());
    }
  }

  this->SetValue(
      std::move(json_array),
      first_key,
      additional_keys...
  );
}

template <>
template <typename Iter, typename ...Args>
void RapidJsonConfigReader::SetDeepArray(
    Iter first,
    Iter last,
    std::string_view first_key,
    const Args&... additional_keys
) {
  rapidjson::Value json_array(rapidjson::kArrayType);

  if constexpr (std::is_same<Iter::value_type, char*>::value
      || std::is_same<Iter::value_type, const char*>::value
      || std::is_same<Iter::value_type, std::string>::value
      || std::is_same<Iter::value_type, std::string_view>::value) {
    for (Iter it = first; it != last; it++) {
      json_array.PushBack(
          rapidjson::Value(it->data(), this->json_document_.GetAllocator()),
          this->json_document_.GetAllocator()
      );
    }
  } else if constexpr (std::is_same<Iter::value_type, char*>::value
      || std::is_same<Iter::value_type, const char*>::value) {
    for (Iter it = first; it != last; it++) {
      json_array.PushBack(
          rapidjson::Value(*it, this->json_document_.GetAllocator()),
          this->json_document_.GetAllocator()
      );
    }
  } else {
    for (Iter it = first; it != last; it++) {
      json_array.PushBack(*it, this->json_document_.GetAllocator());
    }
  }

  this->SetDeepValue(
      std::move(json_array),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetValue(
    RapidJsonConfigReader::JsonValue value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValueRecursive(
      std::move(value),
      this->json_document_,
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepValue(
    RapidJsonConfigReader::JsonValue value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValueRecursive(
      std::move(value),
      this->json_document_,
      first_key,
      additional_keys...
  );
}

/* Functions for bool */

template <>
template <typename ...Args>
bool RapidJsonConfigReader::GetBool(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.GetBool();
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::GetBoolOrDefault(
    bool default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasBool(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetBool(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasBool(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return false;
  }

  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.IsBool();
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetBool(
    bool value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepBool(
    bool value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

/* Functions for std::deque */

template <>
template <typename T, typename ...Args>
std::deque<T> RapidJsonConfigReader::GetDeque(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  return this->GetArrayCopy<std::deque<T>>(
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
std::deque<T> RapidJsonConfigReader::GetDequeOrDefault(
    const std::deque<T>& default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasDeque(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetDeque<T>(first_key, additional_keys...);
}

template <>
template <typename T, typename ...Args>
std::deque<T> RapidJsonConfigReader::GetDequeOrDefault(
    std::deque<T>&& default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasDeque(first_key, additional_keys...)) {
    return std::move(default_value);
  }

  return this->GetDeque<T>(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasDeque(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return false;
  }

  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.IsArray();
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetDeque(
    const std::deque<T>& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetArray(
      value.cbegin(),
      value.cend(),
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetDeque(
    std::deque<T>&& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetArray(
      std::make_move_iterator(value.begin()),
      std::make_move_iterator(value.end()),
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetDeepDeque(
    const std::deque<T>& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepArray(
      value.cbegin(),
      value.cend(),
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetDeepDeque(
    std::deque<T>&& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepArray(
      std::make_move_iterator(value.begin()),
      std::make_move_iterator(value.end()),
      first_key,
      additional_keys...
  );
}

/* Functions for int */

template <>
template <typename ...Args>
int RapidJsonConfigReader::GetInt(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.GetInt();
}

template <>
template <typename ...Args>
int RapidJsonConfigReader::GetIntOrDefault(
    int default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasInt(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetInt(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasInt(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return false;
  }

  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.IsInt();
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetInt(
    int value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepInt(
    int value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

/* Functions for std::int32_t */

template <>
template <typename ...Args>
std::int32_t RapidJsonConfigReader::GetInt32(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.GetInt();
}

template <>
template <typename ...Args>
std::int32_t RapidJsonConfigReader::GetInt32OrDefault(
    std::int32_t default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasInt32(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetInt32(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasInt32(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return false;
  }

  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.IsInt();
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetInt32(
    std::int32_t value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepInt32(
    std::int32_t value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

/* Functions for std::int64_t */

template <>
template <typename ...Args>
std::int64_t RapidJsonConfigReader::GetInt64(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.GetInt64();
}

template <>
template <typename ...Args>
std::int64_t RapidJsonConfigReader::GetInt64OrDefault(
    std::int64_t default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasInt64(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetInt64(first_key, additional_keys...);
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetInt64(
    std::int64_t value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepInt64(
    std::int64_t value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

/* Functions for long */

template <>
template <typename ...Args>
long RapidJsonConfigReader::GetLong(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.GetInt();
}

template <>
template <typename ...Args>
long RapidJsonConfigReader::GetLongOrDefault(
    long default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasLong(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetLong(first_key, additional_keys...);
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetLong(
    long value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepLong(
    long value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

/* Functions for long long */

template <>
template <typename ...Args>
long long RapidJsonConfigReader::GetLongLong(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.GetInt64();
}

template <>
template <typename ...Args>
long long RapidJsonConfigReader::GetLongLongOrDefault(
    long long default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasLongLong(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetLongLong(first_key, additional_keys...);
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetLongLong(
    long long value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepLongLong(
    long long value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

/* Functions for std::filesystem::path */

template <>
template <typename ...Args>
std::filesystem::path RapidJsonConfigReader::GetPath(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  std::string value = this->GetString(
      first_key,
      additional_keys...
  );

  return std::filesystem::u8path(std::move(value));
}

template <>
template <typename ...Args>
std::filesystem::path RapidJsonConfigReader::GetPathOrDefault(
    const std::filesystem::path& default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasPath(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetPath(first_key, additional_keys...);
}

template <>
template <typename ...Args>
std::filesystem::path RapidJsonConfigReader::GetPathOrDefault(
    std::filesystem::path&& default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasPath(first_key, additional_keys...)) {
    return std::move(default_value);
  }

  return this->GetPath(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasPath(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  return this->HasString(
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetPath(
    const std::filesystem::path& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetString(
      value.u8string(),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepPath(
    const std::filesystem::path& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepString(
      value.u8string(),
      first_key,
      additional_keys...
  );
}

/* Functions for std::set */

template <>
template <typename T, typename ...Args>
std::set<T> RapidJsonConfigReader::GetSet(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  return this->GetArrayCopy<std::set<T>>(
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
std::set<T> RapidJsonConfigReader::GetSetOrDefault(
    const std::set<T>& default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasSet(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetSet<T>(first_key, additional_keys...);
}

template <>
template <typename T, typename ...Args>
std::set<T> RapidJsonConfigReader::GetSetOrDefault(
    std::set<T>&& default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasSet(first_key, additional_keys...)) {
    return std::move(default_value);
  }

  return this->GetSet<T>(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasSet(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return false;
  }

  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.IsArray();
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetSet(
    const std::set<T>& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetArray(
      value.cbegin(),
      value.cend(),
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetSet(
    std::set<T>&& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetArray(
      std::make_move_iterator(value.begin()),
      std::make_move_iterator(value.end()),
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetDeepSet(
    const std::set<T>& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepArray(
      value.cbegin(),
      value.cend(),
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetDeepSet(
    std::set<T>&& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepArray(
      std::make_move_iterator(value.begin()),
      std::make_move_iterator(value.end()),
      first_key,
      additional_keys...
  );
}

/* Functions for std::string */

template <>
template <typename ...Args>
std::string RapidJsonConfigReader::GetString(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.GetString();
}

template <>
template <typename ...Args>
std::string RapidJsonConfigReader::GetStringOrDefault(
    const std::string& default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasString(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetString(first_key, additional_keys...);
}

template <>
template <typename ...Args>
std::string RapidJsonConfigReader::GetStringOrDefault(
    std::string&& default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasString(first_key, additional_keys...)) {
    return std::move(default_value);
  }

  return this->GetString(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasString(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return false;
  }

  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.GetString();
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetString(
    const std::string& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValue(
      rapidjson::Value(value.data(), this->json_document_.GetAllocator()),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetString(
    std::string&& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValue(
      rapidjson::Value(value.data(), this->json_document_.GetAllocator()),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepString(
    const std::string& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValue(
      rapidjson::Value(value.data(), this->json_document_.GetAllocator()),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepString(
    std::string&& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValue(
      rapidjson::Value(value.data(), this->json_document_.GetAllocator()),
      first_key,
      additional_keys...
  );
}

/* Functions for unsigned int */

template <>
template <typename ...Args>
unsigned int RapidJsonConfigReader::GetUnsignedInt(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.GetUint();
}

template <>
template <typename ...Args>
unsigned int RapidJsonConfigReader::GetUnsignedIntOrDefault(
    unsigned int default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasUnsignedInt(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetUnsignedInt(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasUnsignedInt(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return false;
  }

  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.IsUint();
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetUnsignedInt(
    unsigned int value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepUnsignedInt(
    unsigned int value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

/* Functions for std::uint32_t */

template <>
template <typename ...Args>
std::uint32_t RapidJsonConfigReader::GetUnsignedInt32(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.GetUint();
}

template <>
template <typename ...Args>
std::uint32_t RapidJsonConfigReader::GetUnsignedInt32OrDefault(
    std::uint32_t default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasUnsignedInt32(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetUnsignedInt32(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasUnsignedInt32(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return false;
  }

  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.IsUint();
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetUnsignedInt32(
    std::uint32_t value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepUnsignedInt32(
    std::uint32_t value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

/* Functions for std::uint64_t */

template <>
template <typename ...Args>
std::uint64_t RapidJsonConfigReader::GetUnsignedInt64(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.GetInt64();
}

template <>
template <typename ...Args>
std::uint64_t RapidJsonConfigReader::GetUnsignedInt64OrDefault(
    std::uint64_t default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasUnsignedInt64(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetUnsignedInt64(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasUnsignedInt64(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return false;
  }

  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.IsUint64();
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetUnsignedInt64(
    std::uint64_t value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepUnsignedInt64(
    std::uint64_t value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

/* Functions for unsigned long */

template <>
template <typename ...Args>
unsigned long RapidJsonConfigReader::GetUnsignedLong(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.GetUint();
}

template <>
template <typename ...Args>
unsigned long RapidJsonConfigReader::GetUnsignedLongOrDefault(
    unsigned long default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasUnsignedLong(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetUnsignedLong(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasUnsignedLong(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return false;
  }

  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.IsUint();
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetUnsignedLong(
    unsigned long value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepUnsignedLong(
    unsigned long value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

/* Functions for unsigned long long */

template <>
template <typename ...Args>
unsigned long long RapidJsonConfigReader::GetUnsignedLongLong(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.GetUint64();
}

template <>
template <typename ...Args>
unsigned long long RapidJsonConfigReader::GetUnsignedLongLongOrDefault(
    unsigned long long default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasUnsignedLongLong(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetUnsignedLongLong(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasUnsignedLongLong(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return false;
  }

  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.IsUint64();
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetUnsignedLongLong(
    unsigned long long value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepUnsignedLongLong(
    unsigned long long value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepValue(
      rapidjson::Value(value),
      first_key,
      additional_keys...
  );
}

/* Functions for std::unordered_set */

template <>
template <typename T, typename ...Args>
std::unordered_set<T> RapidJsonConfigReader::GetUnorderedSet(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  return this->GetArrayCopy<std::unordered_set<T>>(
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
std::unordered_set<T> RapidJsonConfigReader::GetUnorderedSetOrDefault(
    const std::unordered_set<T>& default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasUnorderedSet(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetUnorderedSet<T>(first_key, additional_keys...);
}

template <>
template <typename T, typename ...Args>
std::unordered_set<T> RapidJsonConfigReader::GetUnorderedSetOrDefault(
    std::unordered_set<T>&& default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->HasUnorderedSet(first_key, additional_keys...)) {
    return std::move(default_value);
  }

  return this->GetUnorderedSet<T>(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasUnorderedSet(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return false;
  }

  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.IsArray();
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetUnorderedSet(
    const std::unordered_set<T>& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetArray(
      value.cbegin(),
      value.cend(),
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetUnorderedSet(
    std::unordered_set<T>&& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetArray(
      std::make_move_iterator(value.begin()),
      std::make_move_iterator(value.end()),
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetDeepUnorderedSet(
    const std::unordered_set<T>& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepArray(
      value.cbegin(),
      value.cend(),
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetDeepUnorderedSet(
    std::unordered_set<T>&& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepArray(
      std::make_move_iterator(value.begin()),
      std::make_move_iterator(value.end()),
      first_key,
      additional_keys...
  );
}

/* Functions of std::vector */

template <>
template <typename T, typename ...Args>
std::vector<T> RapidJsonConfigReader::GetVector(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  return this->GetArrayCopy<std::vector<T>>(
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
std::vector<T> RapidJsonConfigReader::GetVectorOrDefault(
    const std::vector<T>& default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return default_value;
  }

  return this->GetVector<T>(first_key, additional_keys...);
}

template <>
template <typename T, typename ...Args>
std::vector<T> RapidJsonConfigReader::GetVectorOrDefault(
    std::vector<T>&& default_value,
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return std::move(default_value);
  }

  return this->GetVector<T>(first_key, additional_keys...);
}

template <>
template <typename ...Args>
bool RapidJsonConfigReader::HasVector(
    std::string_view first_key,
    const Args&... additional_keys
) const {
  if (!this->ContainsKey(first_key, additional_keys...)) {
    return false;
  }

  const rapidjson::Value& value_ref = this->GetValueRef(
      first_key,
      additional_keys...
  );

  return value_ref.IsArray();
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetVector(
    const std::vector<T>& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetArray(
      value.cbegin(),
      value.cend(),
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetVector(
    std::vector<T>&& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetArray(
      std::make_move_iterator(value.begin()),
      std::make_move_iterator(value.end()),
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetDeepVector(
    const std::vector<T>& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepArray(
      value.cbegin(),
      value.cend(),
      first_key,
      additional_keys...
  );
}

template <>
template <typename T, typename ...Args>
void RapidJsonConfigReader::SetDeepVector(
    std::vector<T>&& value,
    std::string_view first_key,
    const Args&... additional_keys
) {
  this->SetDeepArray(
      std::make_move_iterator(value.begin()),
      std::make_move_iterator(value.end()),
      first_key,
      additional_keys...
  );
}

/* Private Helper Functions */

template <>
template <typename ...Args>
bool RapidJsonConfigReader::ContainsKeyRecursive(
    const rapidjson::Value& object,
    std::string_view current_key,
    const Args&... additional_keys
) const {
  // Check for the existence of the key-value.
  if (!object.HasMember(current_key.data())) {
    return false;
  }

  // If this is the destination key, then return the value. Otherwise, recurse
  // one level down.
  if constexpr (sizeof...(additional_keys) <= 0) {
    return true;
  } else {
    const rapidjson::Value& value_ref = object[current_key.data()];

    return this->ContainsKeyRecursive(
        value_ref,
        additional_keys...
    );
  }
}

template <>
template <typename ...Args>
rapidjson::Value& RapidJsonConfigReader::GetValueRefRecursive(
    rapidjson::Value& object,
    std::string_view current_key,
    const Args&... additional_keys
) {
  // If this is the destination key, then return the value. Otherwise, recurse
  // one level down.
  rapidjson::Value& value_ref = object[current_key.data()];

  if constexpr (sizeof...(additional_keys)) {
    return value_ref;
  } else {
    return this->GetValueRef(
        value_ref,
        additional_keys...
    );
  }
}

template <>
template <typename ...Args>
const rapidjson::Value& RapidJsonConfigReader::GetValueRefRecursive(
    const rapidjson::Value& object,
    std::string_view current_key,
    const Args&... additional_keys
) const {
  // If this is the destination key, then return the value. Otherwise, recurse
  // one level down.
  const rapidjson::Value& value_ref = object[current_key.data()];

  if constexpr (sizeof...(additional_keys) <= 0) {
    return value_ref;
  } else {
    return this->GetValueRefRecursive(
        value_ref,
        additional_keys...
    );
  }
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetValueRecursive(
    rapidjson::Value value,
    rapidjson::Value& object,
    std::string_view current_key,
    const Args&... additional_keys
) {
  // If this is the destination key, then set the value. Otherwise, recurse
  // one level down.
  rapidjson::Value& value_ref = object[current_key.data()];

  if constexpr (sizeof...(additional_keys) <= 0) {
    // Check for the existence of the key-value and add the value if this is the
    // destination key.
    if (object.HasMember(current_key.data())) {
      rapidjson::Value& value_ref = object[current_key.data()];
      value_ref = std::move(value);
    } else {
      rapidjson::Value copy_key(
          current_key.data(),
          this->json_document_.GetAllocator()
      );

      object.AddMember(
          copy_key,
          std::move(value),
          this->json_document_.GetAllocator()
      );
    }
  } else {
    this->SetValue(
        std::move(value),
        value_ref,
        additional_keys...
    );
  }
}

template <>
template <typename ...Args>
void RapidJsonConfigReader::SetDeepValueRecursive(
    rapidjson::Value value,
    rapidjson::Value& object,
    std::string_view current_key,
    const Args&... additional_keys
) {
    // If this is the destination key, then set the value. Otherwise, recurse
  // one level down.
  rapidjson::Value& value_ref = object[current_key.data()];

  if constexpr (sizeof...(additional_keys) <= 0) {
    // Check for the existence of the key-value and add the value if this is the
    // destination key.
    if (object.HasMember(current_key.data())) {
      rapidjson::Value& value_ref = object[current_key.data()];
      value_ref = std::move(value);
    } else {
      rapidjson::Value copy_key(
          current_key.data(),
          this->json_document_.GetAllocator()
      );

      object.AddMember(
          copy_key,
          std::move(value),
          this->json_document_.GetAllocator()
      );
    }
  } else {
    // Check for the existence of the key-value and add an object if the
    // object does not exist.
    if (!object.HasMember(current_key.data())) {
      rapidjson::Value copy_key(
          current_key.data(),
          this->json_document_.GetAllocator()
      );

      object.AddMember(
          copy_key,
          rapidjson::Value(rapidjson::kObjectType),
          this->json_document_.GetAllocator()
      );
    }

    this->SetDeepValueRecursive(
        std::move(value),
        value_ref,
        additional_keys...
    );
  }
}

template <>
bool RapidJsonConfigReader::Read() {
  // Create the config file if it doesn't exist.
  if (!std::filesystem::exists(this->config_file_path())) {
    if (std::ofstream config_stream(this->config_file_path());
        config_stream)
    config_stream << u8"{}" << std::endl;
  } else {
    return false;
  }

  // Parse the config.
  if (std::ifstream config_stream(this->config_file_path());
      config_stream) {
    rapidjson::IStreamWrapper config_stream_wrapper(config_stream);
    this->json_document_.ParseStream(config_stream_wrapper);
  } else {
    return false;
  }

  return true;
}

template <>
bool RapidJsonConfigReader::Write(int indent_width) {
  // Write to the config file any new default values.
  if (std::ofstream config_stream(this->config_file_path());
      config_stream) {
    rapidjson::OStreamWrapper config_stream_wrapper(config_stream);
    rapidjson::PrettyWriter pretty_config_writer(config_stream_wrapper);
    pretty_config_writer.SetIndent(' ', indent_width);

    this->json_document().Accept(pretty_config_writer);

    return true;
  } else {
    return false;
  }
}

} // namespace mjsoni

#endif // MJSONI_RAPID_JSON_CONFIG_READER_HPP_
