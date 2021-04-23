/// \file bulk_core.h
///
/// Include all core headers.
///
/// \author Raffaele D. Facendola - April 2021

// ========================================================================= //

#include "syntropy/core/toolset/swap.h"
#include "syntropy/core/toolset/swap_extensions.h"
#include "syntropy/core/comparisons/compare.h"

#include "syntropy/core/containers/fix_array.h"

#include "syntropy/core/comparisons/ordering.h"
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
#include "syntropy/core/records/record_extensions.h"
#include "syntropy/core/records/tuple.h"

#include "syntropy/core/reflection/type_id.h"

#include "syntropy/core/strings/string_extensions.h"
#include "syntropy/core/strings/string_view.h"
#include "syntropy/core/strings/string.h"

#include "syntropy/core/support/event.h"
#include "syntropy/core/support/scope_guard.h"
#include "syntropy/core/support/visitor.h"

// ========================================================================= //
