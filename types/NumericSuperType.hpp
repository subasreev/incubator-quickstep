/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_TYPES_NUMERIC_SUPER_TYPE_HPP_
#define QUICKSTEP_TYPES_NUMERIC_SUPER_TYPE_HPP_

#include <cstddef>
#include <cstring>

#include "types/NullCoercibilityCheckMacro.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief Templatized superclass for Numeric types. Contains code common to all
 *        Numeric types.
 **/
template <typename CppType>
class NumericSuperType : public Type {
 public:
  typedef CppType cpptype;

  std::size_t estimateAverageByteLength() const override {
    return sizeof(CppType);
  }

  bool isCoercibleFrom(const Type &original_type) const override {
    QUICKSTEP_NULL_COERCIBILITY_CHECK();
    return (original_type.getSuperTypeID() == kNumeric);
  }

  TypedValue makeZeroValue() const override {
    return TypedValue(static_cast<CppType>(0));
  }

  bool canCheckEqualityWithMemcmp() const override {
    return true;
  }

  void makeZeroValue(void *value_ptr) const override {
    std::memset(value_ptr, 0, sizeof(cpptype));
  }

  inline std::size_t getHash(const void *value_ptr) const {
    return *static_cast<const CppType *>(value_ptr);
  }

  inline void copyValue(void *dst, const void *src) const {
    *static_cast<CppType *>(dst) = *static_cast<const CppType *>(src);
  }

 protected:
  NumericSuperType(const TypeID type_id, const bool nullable)
      : Type(Type::kNumeric, type_id, nullable, sizeof(CppType), sizeof(CppType)) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(NumericSuperType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_NUMERIC_SUPER_TYPE_HPP_
