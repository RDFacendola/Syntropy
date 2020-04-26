
/// \file label.h
/// \brief This header is part of syntropy types. It contains classes used to handle hashed strings.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy/containers/map.h"

#include "syntropy/types/string.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/math/hash.h"

namespace syntropy
{
    /************************************************************************/
    /* LABEL                                                                */
    /************************************************************************/

    /// \brief Represents an immutable string optimized for fast comparison.
    /// \author Raffaele D. Facendola.
    class Label
    {
    public:

        /// \brief Create an empty label.
        Label();

        /// \brief Copy constructor.
        Label(const Label& other) noexcept = default;

        /// \brief Create a new label from a string.
        Label(const String& string);

        /// \brief Create a new label from a null-terminated string.
        Label(const char* string);

        /// \brief Assignment operator.
        Label& operator=(Label other) noexcept;

        /// \brief Get the string hash used for comparison.
        std::int64_t GetHash() const noexcept;

        /// \brief Get the underlying string.
        const String& GetString() const noexcept;

        /// \brief Check whether the string is non-empty.
        /// \return Returns true if the string is non-empty, returns false otherwise.
        operator bool() const noexcept;

        /// \brief Swaps two label.
        void Swap(Label& other) noexcept;

    private:

        /// \brief Get a dictionary of all registered labels.
        static Map<std::int64_t, String>& GetDictionary();

        /// \brief String hash.
        std::int64_t hash_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Equality comparison.
    bool operator==(const Label& lhs, const Label& rhs) noexcept;

    /// \brief Inequality comparison.
    bool operator!=(const Label& lhs, const Label& rhs) noexcept;

    /// \brief Swaps two labels.
    void swap(Label& lhs, Label& rhs) noexcept;

    /// \brief Stream insertion for labels.
    std::ostream& operator<<(std::ostream& out, const Label& rhs);

    /// \brief Sort labels by hash.
    bool operator<(const Label& lhs, const Label& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Label.

    inline Label::Label()
        : Label("")
    {

    }

    inline Label::Label(const String& string)
        : hash_(Hash::FastHash64(ConstMemoryRange{ string.data(), Bytes{ static_cast<std::int64_t>(string.size()) } }))
    {
        GetDictionary().insert_or_assign(hash_, string);
    }

    inline Label::Label(const char* string)
        : Label(String(string))
    {

    }

    inline Label& Label::operator=(Label other) noexcept
    {
        other.Swap(*this);

        return *this;
    }

    inline std::int64_t Label::GetHash() const noexcept
    {
        return hash_;
    }

    inline const String& Label::GetString() const noexcept
    {
        return GetDictionary()[hash_];
    }

    inline Label::operator bool() const noexcept
    {
        return (*this) != Label{};
    }

    inline void Label::Swap(Label& other) noexcept
    {
        using std::swap;

        swap(other.hash_, hash_);
    }

    inline Map<std::int64_t, String>& Label::GetDictionary()
    {
        static auto dictionary = Map<std::int64_t, String>{};

        return dictionary;
    }

    // Non-member functions.

    inline bool operator==(const Label& lhs, const Label& rhs) noexcept
    {
        return lhs.GetHash() == rhs.GetHash();
    }

    inline bool operator!=(const Label& lhs, const Label& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    inline void swap(Label& lhs, Label& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

    inline std::ostream& operator<<(std::ostream& out, const Label& rhs)
    {
        out << rhs.GetString();

        return out;
    }

    inline bool operator<(const Label& lhs, const Label& rhs)
    {
        return lhs.GetHash() < rhs.GetHash();
    }
}

namespace std
{
    /// \param Template specialization of std::hash for HashedStrings.
    template <>
    struct hash<syntropy::Label>
    {
        using argument_type = syntropy::Label;
        using result_type = std::int64_t;

        result_type operator()(const argument_type& hashed_string) const
        {
            return hashed_string.GetHash();
        }
    };
}

