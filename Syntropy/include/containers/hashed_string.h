
/// \file hashed_string.h
/// \brief This header is part of the syntropy containers. It contains classes used to handle hashed strings.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <utility>
#include <memory>
#include <unordered_map>
#include <string>
#include <type_traits>
#include <cstdint>

#include "math/hash.h"

namespace syntropy
{
    /************************************************************************/
    /* HASHED STRING T                                                      */
    /************************************************************************/

    /// \brief Represents an immutable string optimized for fast comparison.
    /// \tparam TString Type of the underlying string.
    /// \tparam THash Functor used to compute the string hash.
    /// \author Raffaele D. Facendola.
    template <typename TString, typename THash>
    class HashedStringT
    {
    public:

        /// \brief Type of the hashing function return value.
        using THashValue = decltype(std::declval<THash>()(std::declval<TString>()));

        /// \brief Create an empty hashed string.
        constexpr HashedStringT()
            : HashedStringT(TString{})
        {

        }

        /// \brief Copy constructor.
        /// \param other String to copy.
        HashedStringT(const HashedStringT<TString, THash>& other) noexcept
            : string_(other.string_)
            , hash_(other.hash_)
        {

        }

        /// \brief Create a new hashed string from anything that can construct a TString.
        template <typename TStringArg, typename = std::enable_if_t<std::is_constructible_v<TString, TStringArg&&>>>
        HashedStringT(TStringArg&& string)
        {
            std::tie(hash_, string_) = Atlas::GetInstance().AddEntry(std::forward<TStringArg>(string));
        }

        /// \brief Unified assignment operator.
        /// \param other Other instance to copy.
        /// \return Returns a reference to this.
        HashedStringT<TString, THash>& operator=(HashedStringT<TString, THash> other) noexcept
        {
            return other.swap(*this);
        }

        /// \brief Get the hash associated to this instance.
        /// \return Returns the hash associated to this instance.
        THashValue GetHash() const noexcept
        {
            return hash_;
        }

        /// \brief Get the string associated to this instance.
        /// \brief Returns the string associated to this instance.
        const TString& GetString() const noexcept
        {
            return *string_;
        }

        /// \brief Get the string associated to this instance.
        /// \brief Returns the string associated to this instance.
        operator const TString&() const noexcept
        {
            return GetString();
        }

        /// \brief Check whether the string is non-empty.
        /// \return Returns true if the string is non-empty, returns false otherwise.
        constexpr operator bool() const noexcept
        {
            return (*this) != HashedStringT<TString, THash>();
        }

        /// \brief Equality operator.
        /// \return Returns true if the two hashed strings are identical, returns false otherwise.
        constexpr bool operator==(const HashedStringT<TString, THash>& other) const noexcept
        {
            return other.hash_ == hash_;
        }

        /// \brief Inequality operator.
        /// \return Returns true if the two hashed strings are different, returns false otherwise.
        constexpr bool operator!=(const HashedStringT<TString, THash>& other) const noexcept
        {
            return other.hash_ != hash_;
        }

        /// \brief Swaps two instances.
        /// \param other Object to swap with the current instance.
        HashedStringT<TString, THash>& swap(HashedStringT<TString, THash>& other) noexcept
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
            template <typename TAtlasString>
            std::tuple<THashValue, const TString*> AddEntry(TAtlasString&& string)
            {
                auto hash = THash{}(static_cast<const TAtlasString&>(string));

                if (auto it = registry_.find(hash); it != std::end(registry_))
                {
                    return { hash, it->second.get() };                                                          // The string was already registered to the atlas.
                }
                else
                {
                    auto string_ptr = std::make_unique<TString>(std::forward<TAtlasString>(string));

                    return { hash, registry_.emplace(hash, std::move(string_ptr)).first->second.get() };        // Add a new string to the atlas.
                }
            }

        private:

            /// \brief Default constructor.
            Atlas() = default;

            std::unordered_map<THashValue, std::unique_ptr<TString>> registry_;         ///< \brief Associate hashes with their respective strings.
        };

        THashValue hash_;                                                               ///< \brief String hash.

        const TString* string_{ nullptr };                                              ///< \brief Pointer to the actual string. Non-owning pointer.
    };

    /// \brief Default hashed string type.
    using HashedString = HashedStringT<std::string, StringHasher32<std::string>>;

    /// \brief Swaps two hashed strings.
    template <typename TString, typename THash>
    void swap(HashedStringT<TString, THash>& first, HashedStringT<TString, THash>& second) noexcept
    {
        first.swap(second);
    }

    /// \brief Stream insertion for HashedString.
    template <typename THashFunction, typename THash>
    std::ostream& operator<<(std::ostream& out, const HashedStringT<THashFunction, THash>& hashed_string)
    {
        out << hashed_string.GetString();
        return out;
    }

    /// \brief Used to sort hashed strings.
    template <typename TString, typename THash>
    bool operator<(const HashedStringT<TString, THash>& first, const HashedStringT<TString, THash>& second)
    {
        return first.GetHash() < second.GetHash();
    }

}

namespace std
{
    /// \param Template specialization of std::hash for HashedStrings.
    template <typename TString, typename THash>
    struct hash<syntropy::HashedStringT<TString, THash>>
    {
        using argument_type = TString;
        using result_type = typename syntropy::HashedStringT<TString, THash>::THashValue;

        result_type operator()(const argument_type& hashed_string) const
        {
            return THash{}(hashed_string);
        }

    };
}

