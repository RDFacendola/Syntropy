
/// \file compiler.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains the definition of compiler-specific functionalities.
///
/// IMPORTANT: Include this header as a part of a .cpp file. Do not include this header in any other header (other than private ones).
///
/// \author Raffaele D. Facendola - 2016

#pragma once

// List of supported compilers

#define SYNTROPY_COMPILER_INCLUDE_GUARD

#ifdef _MSC_VER

#include "msvc.h"

#endif

#undef SYNTROPY_COMPILER_INCLUDE_GUARD