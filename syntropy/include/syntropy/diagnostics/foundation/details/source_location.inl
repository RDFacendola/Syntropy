
/// \file source_location.inl
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#include "syntropy/language/foundation/preprocessor.h"
#include "syntropy/core/toolset/swap.h"
#include "syntropy/memory/foundation/size.h"
#include "syntropy/memory/foundation/byte_span.h"
#include "syntropy/memory/foundation/memory.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* MACROS                                                               */
    /************************************************************************/

    #undef SYNTROPY_HERE
    #define SYNTROPY_HERE \
        Syntropy::SourceLocation{ SYNTROPY_FILE, \
                                  SYNTROPY_FUNCTION, \
                                  SYNTROPY_LINE }

    /************************************************************************/
    /* SOURCE LOCATION                                                      */
    /************************************************************************/

    inline
    SourceLocation
    ::SourceLocation(String file_name, String function_name, Int line) noexcept
        : file_name_(Move(file_name))
        , function_name_(Move(function_name))
        , line_(line)
    {

    }

    template <typename TCharacter, Int TSize, Int USize>
    inline
    SourceLocation
    ::SourceLocation(StringLiteralType<TCharacter, TSize> file_name,
                     StringLiteralType<TCharacter, USize> function_name,
                     Int line)
    noexcept
    {
        // #TODO @rfacendola This is terrible! There's no guarantee that
        //       a narrow multibyte string is UTF8-encoded, as it depends on
        //       the locale.
        //       C++20 has the multibyte-to-utf8 mbrtoc8() function but it is
        //       not yet fully supported by clang.
        //       This overload is needed while waiting for full support of
        //       standard's std::source_location.

        static_assert(Memory::IsSameSize<TCharacter, char8_t>);

        char8_t file_name_utf8[TSize];
        char8_t function_name_utf8[USize];

        Memory::Copy(MakeByteSpan(file_name_utf8),
                     MakeByteSpan(file_name));

        Memory::Copy(MakeByteSpan(function_name_utf8),
                     MakeByteSpan(function_name));

        file_name_ = file_name_utf8;
        function_name_ = function_name_utf8;
        line_ = line;
    }

    inline Immutable<String>
    SourceLocation
    ::GetFileName() const noexcept
    {
        return file_name_;
    }

    inline Immutable<String>
    SourceLocation
    ::GetFunctionName() const noexcept
    {
        return function_name_;
    }

    inline Int
    SourceLocation
    ::GetLine() const noexcept
    {
        return line_;
    }

    inline void
    SourceLocation
    ::Swap(Mutable<SourceLocation> other) noexcept
    {
        Toolset::Swap(file_name_, other.file_name_);
        Toolset::Swap(function_name_, other.function_name_);
        Toolset::Swap(line_, other.line_);
    }

}

// ===========================================================================
