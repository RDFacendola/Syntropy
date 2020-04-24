
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
        HashedString() = default;

        /// \brief Copy constructor.
        HashedString(const HashedString& other) noexcept = default;

        /// \brief Create a new hashed string from a string.
        HashedString(std::string string);

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

        /// \brief Used to index hashed strings.
        /// Static member variables are avoided to prevent static initialization order fiasco (some hashed strings may be created during static initialization).
        class Atlas
        {
        public:

            /// \brief Get the singleton instance.
            static Atlas& GetInstance()
            {
                static Atlas instance;
                return instance;
            }

            /// \brief Register a new entry to the atlas.
            /// \return Returns the hash-string pair.
            std::tuple<std::int64_t, const std::string*> AddEntry(std::string string)
            {
                auto hash = Hash::FastHash64(syntropy::ConstMemoryRange{ string.data(), Bytes(string.size()) });

                if (auto it = registry_.find(hash); it != std::end(registry_))
                {
                    return { hash, it->second.get() };                                                          // The string was already registered to the atlas.
                }
                else
                {
                    auto string_ptr = std::make_unique<std::string>(std::move(string));

                    return { hash, registry_.emplace(hash, std::move(string_ptr)).first->second.get() };        // Add a new string to the atlas.
                }
            }

        private:

            /// \brief Default constructor.
            Atlas() = default;

            std::unordered_map<std::int64_t, std::unique_ptr<std::string>> registry_;         ///< \brief Associate hashes with their respective strings.
        };

        /// \brief String hash.
        std::int64_t hash_;                                                             ///< \brief String hash.

        const std::string* string_{ nullptr };                                               ///< \brief Pointer to the actual string. Non-owning pointer.
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

    inline HashedString::HashedString(std::string string)
    {
        std::tie(hash_, string_) = Atlas::GetInstance().AddEntry(std::move(string));
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
        return *string_;
    }

    inline HashedString::operator bool() const noexcept
    {
        return (*this) != HashedString{};
    }

    inline void HashedString::Swap(HashedString& other) noexcept
    {
        using std::swap;

        swap(other.string_, string_);
        swap(other.hash_, hash_);
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

