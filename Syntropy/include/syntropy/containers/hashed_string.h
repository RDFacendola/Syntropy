
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
        HashedString()
            : HashedString(std::string{})
        {

        }

        /// \brief Copy constructor.
        /// \param other String to copy.
        HashedString(const HashedString& other) noexcept
            : string_(other.string_)
            , hash_(other.hash_)
        {

        }

        /// \brief Create a new hashed string from anything that can construct a TString.
        template <typename TStringArg, typename = std::enable_if_t<std::is_constructible_v<std::string, TStringArg&&>>>
        HashedString(TStringArg string)
        {
            std::tie(hash_, string_) = Atlas::GetInstance().AddEntry(std::move(string));
        }

        /// \brief Unified assignment operator.
        /// \param other Other instance to copy.
        /// \return Returns a reference to this.
        HashedString& operator=(HashedString other) noexcept
        {
            return other.swap(*this);
        }

        /// \brief Get the hash associated to this instance.
        /// \return Returns the hash associated to this instance.
        std::int64_t GetHash() const noexcept
        {
            return hash_;
        }

        /// \brief Get the string associated to this instance.
        /// \brief Returns the string associated to this instance.
        const std::string& GetString() const noexcept
        {
            return *string_;
        }

        /// \brief Get the string associated to this instance.
        /// \brief Returns the string associated to this instance.
        operator const std::string&() const noexcept
        {
            return GetString();
        }

        /// \brief Check whether the string is non-empty.
        /// \return Returns true if the string is non-empty, returns false otherwise.
        constexpr operator bool() const noexcept
        {
            return (*this) != HashedString{};
        }

        /// \brief Equality operator.
        /// \return Returns true if the two hashed strings are identical, returns false otherwise.
        constexpr bool operator==(const HashedString& other) const noexcept
        {
            return other.hash_ == hash_;
        }

        /// \brief Inequality operator.
        /// \return Returns true if the two hashed strings are different, returns false otherwise.
        constexpr bool operator!=(const HashedString& other) const noexcept
        {
            return other.hash_ != hash_;
        }

        /// \brief Swaps two instances.
        /// \param other Object to swap with the current instance.
        HashedString& swap(HashedString& other) noexcept
        {
            std::swap(other.string_, string_);
            std::swap(other.hash_, hash_);
            return *this;
        }

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

        std::int64_t hash_;                                                             ///< \brief String hash.

        const std::string* string_{ nullptr };                                               ///< \brief Pointer to the actual string. Non-owning pointer.
    };

    /// \brief Swaps two hashed strings.
    inline void swap(HashedString& first, HashedString second) noexcept
    {
        first.swap(second);
    }

    /// \brief Stream insertion for HashedString.
    inline std::ostream& operator<<(std::ostream& out, const HashedString& hashed_string)
    {
        out << hashed_string.GetString();
        return out;
    }

    /// \brief Used to sort hashed strings.
    inline bool operator<(const HashedString& first, const HashedString& second)
    {
        return first.GetHash() < second.GetHash();
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

