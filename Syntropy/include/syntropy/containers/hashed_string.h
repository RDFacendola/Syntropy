
/// \file hashed_string.h
/// \brief This header is part of the syntropy containers. It contains classes used to handle hashed strings.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <utility>
#include <memory>
#include <unordered_map>
#include <type_traits>
#include <cstdint>

#include "syntropy/types/string.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/math/hash.h"

namespace syntropy
{
    /************************************************************************/
    /* HASHED STRING                                                        */
    /************************************************************************/

    /// \brief Represents an immutable string optimized for fast comparison.
    /// \author Raffaele D. Facendola.
    class HashedString
    {
    public:

        /// \brief Create an empty hashed string.
        HashedString();

        /// \brief Copy constructor.
        HashedString(const HashedString& other) noexcept = default;

        /// \brief Create a new hashed string from a string.
        HashedString(const std::string& string);

        /// \brief Create a new hashed string from a null-terminated string.
        HashedString(const char* string);

        /// \brief Assignment operator.
        HashedString& operator=(HashedString other) noexcept;

        /// \brief Get the string hash used for comparison.
        std::int64_t GetHash() const noexcept;

        /// \brief Get the underlying string.
        const std::string& GetString() const noexcept;

        /// \brief Check whether the string is non-empty.
        /// \return Returns true if the string is non-empty, returns false otherwise.
        operator bool() const noexcept;

        /// \brief Swaps two hashed string.
        void Swap(HashedString& other) noexcept;

    private:

        /// \brief Get a dictionary of all registered hashed strings.
        static std::unordered_map<std::int64_t, std::string>& GetDictionary();

        /// \brief String hash.
        std::int64_t hash_;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Equality comparison.
    bool operator==(const HashedString& lhs, const HashedString& rhs) noexcept;

    /// \brief Inequality comparison.
    bool operator!=(const HashedString& lhs, const HashedString& rhs) noexcept;

    /// \brief Swaps two hashed strings.
    void swap(HashedString& lhs, HashedString& rhs) noexcept;

    /// \brief Stream insertion for HashedString.
    std::ostream& operator<<(std::ostream& out, const HashedString& rhs);

    /// \brief Sort hashed string by hash.
    bool operator<(const HashedString& lhs, const HashedString& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // HashedString.

    inline HashedString::HashedString()
        : HashedString("")
    {

    }

    inline HashedString::HashedString(const std::string& string)
        : hash_(Hash::FastHash64(ConstMemoryRange{ string.data(), Bytes{ static_cast<std::int64_t>(string.size()) } }))
    {
        GetDictionary().insert_or_assign(hash_, string);
    }

    inline HashedString::HashedString(const char* string)
        : HashedString(std::string(string))
    {

    }

    inline HashedString& HashedString::operator=(HashedString other) noexcept
    {
        other.Swap(*this);

        return *this;
    }

    inline std::int64_t HashedString::GetHash() const noexcept
    {
        return hash_;
    }

    inline const std::string& HashedString::GetString() const noexcept
    {
        return GetDictionary()[hash_];
    }

    inline HashedString::operator bool() const noexcept
    {
        return (*this) != HashedString{};
    }

    inline void HashedString::Swap(HashedString& other) noexcept
    {
        using std::swap;

        swap(other.hash_, hash_);
    }

    inline std::unordered_map<std::int64_t, std::string>& HashedString::GetDictionary()
    {
        static auto dictionary = std::unordered_map<std::int64_t, std::string>{};

        return dictionary;
    }

    // Non-member functions.

    inline bool operator==(const HashedString& lhs, const HashedString& rhs) noexcept
    {
        return lhs.GetHash() == rhs.GetHash();
    }

    inline bool operator!=(const HashedString& lhs, const HashedString& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    inline void swap(HashedString& lhs, HashedString& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

    inline std::ostream& operator<<(std::ostream& out, const HashedString& rhs)
    {
        out << rhs.GetString();

        return out;
    }

    inline bool operator<(const HashedString& lhs, const HashedString& rhs)
    {
        return lhs.GetHash() < rhs.GetHash();
    }
}

namespace std
{
    /// \param Template specialization of std::hash for HashedStrings.
    template <>
    struct hash<syntropy::HashedString>
    {
        using argument_type = syntropy::HashedString;
        using result_type = std::int64_t;

        result_type operator()(const argument_type& hashed_string) const
        {
            return hashed_string.GetHash();
        }
    };
}

