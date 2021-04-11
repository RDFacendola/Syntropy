/// \file bulk.h
///
/// Define all headers.
///
/// \author Raffaele D. Facendola - February 2021

// =========================================================================  //

// Core.
// =====

#include "syntropy/core/algorithms/swap.h"
#include "syntropy/core/algorithms/compare.h"

#include "syntropy/core/containers/fix_array.h"

#include "syntropy/core/foundation/ordering.h"
#include "syntropy/core/foundation/unique_ptr.h"

#include "syntropy/core/ranges/bidirectional_range.h"
#include "syntropy/core/ranges/contiguous_range.h"
#include "syntropy/core/ranges/forward_range.h"
#include "syntropy/core/ranges/random_access_range.h"
#include "syntropy/core/ranges/range_extensions.h"
#include "syntropy/core/ranges/range_iterator.h"
#include "syntropy/core/ranges/range.h"
#include "syntropy/core/ranges/reverse_range.h"
#include "syntropy/core/ranges/sized_range.h"
#include "syntropy/core/ranges/span.h"
#include "syntropy/core/ranges/zip_range.h"

#include "syntropy/core/records/record.h"
#include "syntropy/core/records/tuple.h"

#include "syntropy/core/reflection/type_id.h"

#include "syntropy/core/strings/string_view.h"
#include "syntropy/core/strings/string.h"

#include "syntropy/core/support/event.h"
#include "syntropy/core/support/scope_guard.h"
#include "syntropy/core/support/visitor.h"

// Diagnostics.
// ============

#include "syntropy/diagnostics/foundation/assert.h"
#include "syntropy/diagnostics/foundation/debugger.h"
#include "syntropy/diagnostics/foundation/source_location.h"

#include "syntropy/diagnostics/unit_test/test_context.h"
#include "syntropy/diagnostics/unit_test/test_case.h"
#include "syntropy/diagnostics/unit_test/test_suite.h"
#include "syntropy/diagnostics/unit_test/auto_test_case.h"
#include "syntropy/diagnostics/unit_test/auto_test_suite.h"
#include "syntropy/diagnostics/unit_test/auto_unit_test.h"

// HAL.
// ====

#include "syntropy/hal/hal_macro.h"

// Language.
// =========

#include "syntropy/language/foundation/conversions.h"
#include "syntropy/language/foundation/definitions.h"
#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/foundation/types.h"

#include "syntropy/language/preprocessor/macro.h"

#include "syntropy/language/support/ignore.h"
#include "syntropy/language/support/initializer_list.h"

#include "syntropy/language/templates/concepts.h"
#include "syntropy/language/templates/diagnostics.h"
#include "syntropy/language/templates/invoke.h"
#include "syntropy/language/templates/math.h"
#include "syntropy/language/templates/priority.h"
#include "syntropy/language/templates/ratio.h"
#include "syntropy/language/templates/sfinae.h"
#include "syntropy/language/templates/templates.h"
#include "syntropy/language/templates/type_traits.h"

// Math.
// =====

#include "syntropy/math/math.h"

// Memory.
// =======

#include "syntropy/memory/allocators/allocator.h"
#include "syntropy/memory/allocators/system_allocator.h"

#include "syntropy/memory/foundation/address.h"
#include "syntropy/memory/foundation/alignment.h"
#include "syntropy/memory/foundation/buffer.h"
#include "syntropy/memory/foundation/byte_span.h"
#include "syntropy/memory/foundation/byte.h"
#include "syntropy/memory/foundation/memory.h"
#include "syntropy/memory/foundation/size.h"


// =========================================================================  //
