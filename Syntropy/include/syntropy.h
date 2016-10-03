
/// \file syntropy.h
/// \brief Common header for Syntropy library.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <type_traits>
#include <cassert>

#define SYN_UNUSED(x) (x)

#ifdef _MSC_VER
#define SYN_NO_INLINE __declspec(noinline)
#else
#define SYN_NO_INLINE 
#endif

#ifdef _DEBUG
#define SYN_DEBUG_ONLY(x) x
#else
#define SYN_DEBUG_ONLY(x)
#endif

#ifdef _DEBUG
#define SYN_RELEASE_ONLY(x) 
#else
#define SYN_RELEASE_ONLY(x) x
#endif

namespace syntropy {




}