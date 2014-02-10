// Copyright (c) 2012, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#ifndef VM_ASSEMBLER_H_
#define VM_ASSEMBLER_H_

#include "platform/assert.h"
#include "vm/allocation.h"
#include "vm/globals.h"
#include "vm/growable_array.h"
#include "vm/object.h"

namespace dart {

#if defined(TARGET_ARCH_ARM) || defined(TARGET_ARCH_MIPS)
DECLARE_FLAG(bool, use_far_branches);
#endif

// Forward declarations.
class Assembler;
class AssemblerFixup;
class AssemblerBuffer;
class MemoryRegion;


// External labels keep a function pointer to allow them
// to be called from code generated by the assembler.
class ExternalLabel : public ValueObject {
 public:
  ExternalLabel(const char* name, uword address)
      : name_(name), address_(address) {
    ASSERT(name != NULL);
  }

  const char* name() const { return name_; }
  bool is_resolved() const { return address_ != 0; }
  uword address() const {
    ASSERT(is_resolved());
    return address_;
  }

 private:
  const char* name_;
  const uword address_;
};


// Assembler fixups are positions in generated code that hold relocation
// information that needs to be processed before finalizing the code
// into executable memory.
class AssemblerFixup : public ZoneAllocated {
 public:
  virtual void Process(const MemoryRegion& region, intptr_t position) = 0;

  virtual bool IsPointerOffset() const = 0;

  // It would be ideal if the destructor method could be made private,
  // but the g++ compiler complains when this is subclassed.
  virtual ~AssemblerFixup() { UNREACHABLE(); }

 private:
  AssemblerFixup* previous_;
  intptr_t position_;

  AssemblerFixup* previous() const { return previous_; }
  void set_previous(AssemblerFixup* previous) { previous_ = previous; }

  intptr_t position() const { return position_; }
  void set_position(intptr_t position) { position_ = position; }

  friend class AssemblerBuffer;
};


// Assembler buffers are used to emit binary code. They grow on demand.
class AssemblerBuffer : public ValueObject {
 public:
  AssemblerBuffer();
  ~AssemblerBuffer();

  // Basic support for emitting, loading, and storing.
  template<typename T> void Emit(T value) {
    ASSERT(HasEnsuredCapacity());
    *reinterpret_cast<T*>(cursor_) = value;
    cursor_ += sizeof(T);
  }

  template<typename T> void Remit() {
    ASSERT(Size() >= static_cast<intptr_t>(sizeof(T)));
    cursor_ -= sizeof(T);
  }

  template<typename T> T Load(intptr_t position) {
    ASSERT(position >= 0 &&
           position <= (Size() - static_cast<intptr_t>(sizeof(T))));
    return *reinterpret_cast<T*>(contents_ + position);
  }

  template<typename T> void Store(intptr_t position, T value) {
    ASSERT(position >= 0 &&
           position <= (Size() - static_cast<intptr_t>(sizeof(T))));
    *reinterpret_cast<T*>(contents_ + position) = value;
  }

  const ZoneGrowableArray<intptr_t>& pointer_offsets() const {
#if defined(DEBUG)
    ASSERT(fixups_processed_);
#endif
    return *pointer_offsets_;
  }

  // Emit an object pointer directly in the code.
  void EmitObject(const Object& object);

  // Emit a fixup at the current location.
  void EmitFixup(AssemblerFixup* fixup) {
    fixup->set_previous(fixup_);
    fixup->set_position(Size());
    fixup_ = fixup;
  }

  // Count the fixups that produce a pointer offset, without processing
  // the fixups.
  intptr_t CountPointerOffsets() const;

  // Get the size of the emitted code.
  intptr_t Size() const { return cursor_ - contents_; }
  uword contents() const { return contents_; }

  // Copy the assembled instructions into the specified memory block
  // and apply all fixups.
  void FinalizeInstructions(const MemoryRegion& region);

  // To emit an instruction to the assembler buffer, the EnsureCapacity helper
  // must be used to guarantee that the underlying data area is big enough to
  // hold the emitted instruction. Usage:
  //
  //     AssemblerBuffer buffer;
  //     AssemblerBuffer::EnsureCapacity ensured(&buffer);
  //     ... emit bytes for single instruction ...

#if defined(DEBUG)
  class EnsureCapacity : public ValueObject {
   public:
    explicit EnsureCapacity(AssemblerBuffer* buffer);
    ~EnsureCapacity();

   private:
    AssemblerBuffer* buffer_;
    intptr_t gap_;

    intptr_t ComputeGap() { return buffer_->Capacity() - buffer_->Size(); }
  };

  bool has_ensured_capacity_;
  bool HasEnsuredCapacity() const { return has_ensured_capacity_; }
#else
  class EnsureCapacity : public ValueObject {
   public:
    explicit EnsureCapacity(AssemblerBuffer* buffer) {
      if (buffer->cursor() >= buffer->limit()) buffer->ExtendCapacity();
    }
  };

  // When building the C++ tests, assertion code is enabled. To allow
  // asserting that the user of the assembler buffer has ensured the
  // capacity needed for emitting, we add a dummy method in non-debug mode.
  bool HasEnsuredCapacity() const { return true; }
#endif

  // Returns the position in the instruction stream.
  intptr_t GetPosition() const { return cursor_ - contents_; }

 private:
  // The limit is set to kMinimumGap bytes before the end of the data area.
  // This leaves enough space for the longest possible instruction and allows
  // for a single, fast space check per instruction.
  static const intptr_t kMinimumGap = 32;

  uword contents_;
  uword cursor_;
  uword limit_;
  AssemblerFixup* fixup_;
  ZoneGrowableArray<intptr_t>* pointer_offsets_;
#if defined(DEBUG)
  bool fixups_processed_;
#endif

  uword cursor() const { return cursor_; }
  uword limit() const { return limit_; }
  intptr_t Capacity() const {
    ASSERT(limit_ >= contents_);
    return (limit_ - contents_) + kMinimumGap;
  }

  // Process the fixup chain.
  void ProcessFixups(const MemoryRegion& region);

  // Compute the limit based on the data area and the capacity. See
  // description of kMinimumGap for the reasoning behind the value.
  static uword ComputeLimit(uword data, intptr_t capacity) {
    return data + capacity - kMinimumGap;
  }

  void ExtendCapacity();

  friend class AssemblerFixup;
};

}  // namespace dart


#if defined(TARGET_ARCH_IA32)
#include "vm/assembler_ia32.h"
#elif defined(TARGET_ARCH_X64)
#include "vm/assembler_x64.h"
#elif defined(TARGET_ARCH_ARM)
#include "vm/assembler_arm.h"
#elif defined(TARGET_ARCH_MIPS)
#include "vm/assembler_mips.h"
#else
#error Unknown architecture.
#endif

#endif  // VM_ASSEMBLER_H_
