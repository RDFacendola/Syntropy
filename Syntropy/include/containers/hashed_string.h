
/// \file hashed_string.h
/// \brief This file contains the classes used to handle hashed strings.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstdint>
#include <utility>
#include <functional>
#include <type_traits>
#include <unordered_map>
#include <memory>

#include "math/fnv1.h"

#include "syntropy.h"

namespace syntropy {

    /// \brief Represents an hashed string.
    /// \author Raffaele D. Facendola.
    template <typename THashFunction, typename THash>
    class HashedStringT {

    public:

        using hash_t = typename THash;

        /// \brief Create an empty hashed string.
        HashedStringT() noexcept;

        /// \brief Copy constructor.
        HashedStringT(const HashedStringT<THashFunction, THash>& other) noexcept;

        /// \brief Create a new hashed string from hash.
        HashedStringT(hash_t hash);
        
        /// \brief Create a new hashed string from anything that can initialize a std::string.
        template <typename TString>
        HashedStringT(TString&& string, typename std::enable_if<!std::is_same<std::decay_t<TString>, HashedStringT<THashFunction, THash>>::value>::type* = nullptr);

        /// \brief Unified assignment operator.
        /// \param other Other instance to copy.
        /// \return Returns a reference to this.
        HashedStringT<THashFunction, THash>& operator=(HashedStringT<THashFunction, THash> other) noexcept;

        /// \brief Get the hash associated to this instance.
        /// \return Returns the hash associated to this instance.
        THash GetHash() const noexcept;

        /// \brief Get the string associated to this instance.
        /// \brief Returns the string associated to this instance.
        const std::string& GetString() const noexcept;

        /// \brief Equality operator.
        /// \return Returns true if the two hashed strings are identical, returns false otherwise.
        bool operator==(const HashedStringT<THashFunction, THash>& other) const noexcept;

        /// \brief Inequality operator.
        /// \return Returns true if the two hashed strings are different, returns false otherwise.
        bool operator!=(const HashedStringT<THashFunction, THash>& other) const noexcept;

        /// \brief Less-equal operator.
        /// \return Returns true if the current instance is less than or equal to the provided hashed string, returns false otherwise.
        bool operator<=(const HashedStringT<THashFunction, THash>& other) const noexcept;

        /// \brief Greater-equal operator.
        /// \return Returns true if the current instance is greater than or equal to the provided hashed string, returns false otherwise.
        bool operator>=(const HashedStringT<THashFunction, THash>& other) const noexcept;

        /// \brief Less-than operator.
        /// \return Returns true if the current instance is less than the provided hashed string, returns false otherwise.
        bool operator<(const HashedStringT<THashFunction, THash>& other) const noexcept;

        /// \brief Greater-than operator.
        /// \return Returns true if the current instance is greater than the provided hashed string, returns false otherwise.
        bool operator>(const HashedStringT<THashFunction, THash>& other) const noexcept;

        /// \brief Swaps two instances.
        /// \param other Object to swap with the current instance.
        HashedStringT<THashFunction, THash>& swap(HashedStringT<THashFunction, THash>& other) noexcept;

    private:

        class Pool;

        mutable const std::string* string_;	        ///< \brief Pointer to the actual string. Initialized lazily. Non-owning pointer.

        THash hash_;                                ///< \brief Hash of the string.

    };

    /// \brief Singleton containing the hashed strings registered so far.
    /// \author Raffaele D. Facendola - February 2017.
    template <typename THashFunction, typename THash>
    class HashedStringT<THashFunction, THash>::Pool
    {

    public:

        /// \brief Get the singleton instance.
        static Pool& GetInstance();

        /// \brief Get a string from hash.
        /// \param hash Hash of the string to get.
        /// \return Returns a pointer to the string matching the given hash. If no such string exists, returns nullptr.
        const std::string* GetStringFromHash(THash hash) const;

        /// \brief Store a string in the pool.
        /// \param string String to store.
        /// \return Returns a pair containing the hash of the provided string and a pointer to the string itself.
        template <typename TString>
        std::pair<THash, const std::string*> StoreString(TString&& string);
        
    private:

        using TStringMap = std::unordered_map<THash, std::unique_ptr<std::string>>;

        /// \brief Default private constructor.
        Pool() = default;

        TStringMap string_map_;             ///< \brief Maps the hash of the strings with the strings themselves.

    };

    /// \brief Default hashed string type.
    using HashedString = HashedStringT<FNV1a, uint64_t>;

    /// \brief Stream insertion for HashedString.
    template <typename THashFunction, typename THash>
    std::ostream& operator<<(std::ostream& out, const HashedStringT<THashFunction, THash>& hashed_string);
    
    /// \brief Used to sort hashed strings.
    template <typename THashFunction, typename THash>
    bool operator<(const HashedStringT<THashFunction, THash>& first, const HashedStringT<THashFunction, THash>& second);

}

namespace std {

    /// \param Custom specialization of std::hash for HashedStrings.
    /// \author Raffaele D. Facendola.
    template <typename THashFunction, typename THash>
    struct hash<syntropy::HashedStringT<THashFunction, THash>> {

        using argument_type = syntropy::HashedStringT<THashFunction, THash>;
        using result_type = THash;

        result_type operator()(const argument_type& hashed_string) const {

            return hashed_string.GetHash();

        }

    };

}

namespace syntropy{

    /************************************************************************/
    /* HASHED STRING T                                                      */
    /************************************************************************/

    template <typename THashFunction, typename THash>
    inline HashedStringT<THashFunction, THash>::HashedStringT() noexcept
        : HashedStringT("")
    {
    
    }

    template <typename THashFunction, typename THash>
    HashedStringT<THashFunction, THash>::HashedStringT(const HashedStringT<THashFunction, THash>& other) noexcept
        : string_(other.string_)
        , hash_(other.hash_) 
    {
    
    }

    template <typename THashFunction, typename THash>
    HashedStringT<THashFunction, THash>::HashedStringT(hash_t hash)
        : string_(nullptr)
        , hash_(hash)
    {

    }

    template <typename THashFunction, typename THash>
    template <typename TString>
    HashedStringT<THashFunction, THash>::HashedStringT(TString&& string, typename std::enable_if<!std::is_same<std::decay_t<TString>, HashedStringT<THashFunction, THash>>::value>::type*)
    {
        auto result = Pool::GetInstance().StoreString(std::forward<TString>(string));

        hash_ = result.first;
        string_ = result.second;
    }

    template <typename THashFunction, typename THash>
    HashedStringT<THashFunction, THash>& HashedStringT<THashFunction, THash>::operator=(HashedStringT<THashFunction, THash> other) noexcept 
    {
        return other.swap(*this);
    }

    template <typename THashFunction, typename THash>
    THash HashedStringT<THashFunction, THash>::GetHash() const noexcept 
    {
        return hash_;
    }
   
    template <typename THashFunction, typename THash>
    const std::string& HashedStringT<THashFunction, THash>::GetString() const noexcept 
    {
        static std::string kUnknown;

        if (!string_)
        {
            string_ = Pool::GetInstance().GetStringFromHash(hash_);          // Resolve the string value.
        }

        return string_ ? *string_ : kUnknown;
    }

    template <typename THashFunction, typename THash>
    bool HashedStringT<THashFunction, THash>::operator==(const HashedStringT<THashFunction, THash>& other) const noexcept 
    {
        return hash_ == other.hash_;
    }

    template <typename THashFunction, typename THash>
    bool HashedStringT<THashFunction, THash>::operator!=(const HashedStringT<THashFunction, THash>& other) const noexcept 
    {
        return hash_ != other.hash_;
    }

    template <typename THashFunction, typename THash>
    bool HashedStringT<THashFunction, THash>::operator<=(const HashedStringT<THashFunction, THash>& other) const noexcept 
    {
        return hash_ <= other.hash_;
    }

    template <typename THashFunction, typename THash>
    bool HashedStringT<THashFunction, THash>::operator>=(const HashedStringT<THashFunction, THash>& other) const noexcept 
    {
        return hash_ >= other.hash_;
    }

    template <typename THashFunction, typename THash>
    bool HashedStringT<THashFunction, THash>::operator<(const HashedStringT<THashFunction, THash>& other) const noexcept 
    {
        return hash_ < other.hash_;
    }

    template <typename THashFunction, typename THash>
    bool HashedStringT<THashFunction, THash>::operator>(const HashedStringT<THashFunction, THash>& other) const noexcept 
    {
        return hash_ > other.hash_;
    }

    template <typename THashFunction, typename THash>
    HashedStringT<THashFunction, THash>& HashedStringT<THashFunction, THash>::swap(HashedStringT<THashFunction, THash>& other) noexcept 
    {
        std::swap(other.string_, string_);
        std::swap(other.hash_, hash_);
        return *this;
    }

    template <typename THashFunction, typename THash>
    std::ostream& operator<<(std::ostream& out, const HashedStringT<THashFunction, THash>& hashed_string) 
    {
        out << hashed_string.GetString();
        return out;
    }

    template <typename THashFunction, typename THash>
    bool operator<(const HashedStringT<THashFunction, THash>& first, const HashedStringT<THashFunction, THash>& second)
    {
        return first.GetHash() < second.GetHash();
    }

    /************************************************************************/
    /* HASHED STRING T :: POOL                                              */
    /************************************************************************/

    template <typename THashFunction, typename THash>
    typename HashedStringT<THashFunction, THash>::Pool& HashedStringT<THashFunction, THash>::Pool::GetInstance()
    {
        static Pool instance;
        return instance;
    }

    template <typename THashFunction, typename THash>
    const std::string* HashedStringT<THashFunction, THash>::Pool::GetStringFromHash(THash hash) const
    {
        auto it = string_map_.find(hash);

        return it != string_map_.end() ?
               it->second.get() :
               nullptr;
    }

    template <typename THashFunction, typename THash>
    template <typename TString>
    std::pair<THash, const std::string*> HashedStringT<THashFunction, THash>::Pool::StoreString(TString&& string)
    {
        std::pair<THash, const std::string*> result;

        result.first = THashFunction{}(string);
        result.second = GetStringFromHash(result.first);

        if (!result.second)
        {
            // Add a new string to the map
            auto emplace_result = string_map_.emplace(result.first, std::make_unique<std::string>(std::forward<TString>(string)));      // std::pair<bool, iterator>
            result.second = emplace_result.first->second.get();                                                                         // std::pair<hash, std::unique_ptr<std::string>>
        }
        
        return result;
    }

}