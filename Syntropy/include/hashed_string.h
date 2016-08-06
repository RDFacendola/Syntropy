
/// \file hashed_string.h
/// \brief This file contains the classes used to handle hashed strings.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstdint>
#include <utility>
#include <functional>
#include <type_traits>

#include "syntropy.h"
#include "fnv1.h"

namespace syntropy {

    /// \brief Represents an hashed string.
    /// \author Raffaele D. Facendola.
    template <typename THashFunction, typename THash>
    class HashedStringT {

    public:

        /// \brief Create an empty hashed string.
        HashedStringT();

        /// \brief Copy constructor.
        HashedStringT(const HashedStringT<THashFunction, THash>& other);

        /// \brief Move constructor.
        HashedStringT(HashedStringT<THashFunction, THash>&& other);

        /// \brief Create a new hashed string from any type that can be used to initialize a string.
        /// \param string The string used to create the tag.
        /// \remarks The SFINAE trick here is used because whenever this overload is preferred over the more correct copy constructor,
        ///          a compilation error occurs.
        template <typename TString>
        HashedStringT(TString&& string, 
                      typename std::enable_if<!std::is_same<std::decay_t<TString>,
                                                            HashedStringT<THashFunction, THash>>::value>::type* = nullptr);
        
        /// \brief Get the hash associated to this instance.
        /// \return Returns the hash associated to this instance.
        THash GetHash() const;

        /// \brief Get the string associated to this instance.
        /// \brief Returns the string associated to this instance.
        const std::string& GetString() const;

        /// \brief Equality operator.
        /// \return Returns true if the two hashed strings are identical, returns false otherwise.
        bool operator==(const HashedStringT<THashFunction, THash>& other) const;

        /// \brief Inequality operator.
        /// \return Returns true if the two hashed strings are different, returns false otherwise.
        bool operator!=(const HashedStringT<THashFunction, THash>& other) const;

        /// \brief Less-equal operator.
        /// \return Returns true if the current instance is less than or equal to the provided hashed string, returns false otherwise.
        bool operator<=(const HashedStringT<THashFunction, THash>& other) const;

        /// \brief Greater-equal operator.
        /// \return Returns true if the current instance is greater than or equal to the provided hashed string, returns false otherwise.
        bool operator>=(const HashedStringT<THashFunction, THash>& other) const;

        /// \brief Less-than operator.
        /// \return Returns true if the current instance is less than the provided hashed string, returns false otherwise.
        bool operator<(const HashedStringT<THashFunction, THash>& other) const;

        /// \brief Greater-than operator.
        /// \return Returns true if the current instance is greater than the provided hashed string, returns false otherwise.
        bool operator>(const HashedStringT<THashFunction, THash>& other) const;

    private:

        std::string string_;	        ///< \brief Plain string.

        THash hash_;                    ///< \brief Hash of the string.
        
    };

    /// \brief Default hashed string type.
    using HashedString = HashedStringT<FNV1a, uint64_t>;
    
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

    //////////////// HASHED STRING ////////////////

    template <typename THashFunction, typename THash>
    inline HashedStringT<THashFunction, THash>::HashedStringT()
        : hash_(THashFunction{}("")) {}

    template <typename THashFunction, typename THash>
    HashedStringT<THashFunction, THash>::HashedStringT(const HashedStringT<THashFunction, THash>& other)
        : string_(other.string_)
        , hash_(other.hash_) {}

    template <typename THashFunction, typename THash>
    HashedStringT<THashFunction, THash>::HashedStringT(HashedStringT<THashFunction, THash>&& other)
        : string_(std::move(other.string_))
        , hash_(other.hash_) {}

    template <typename THashFunction, typename THash>
    template <typename TString>
    inline HashedStringT<THashFunction, THash>::HashedStringT(TString&& string, typename std::enable_if<!std::is_same<std::decay_t<TString>, HashedStringT<THashFunction, THash>>::value>::type*)
        : string_(std::forward<TString>(string))
        , hash_(THashFunction{}(string_)) {}
    
    template <typename THashFunction, typename THash>
    inline THash HashedStringT<THashFunction, THash>::GetHash() const {

        return hash_;

    }
   
    template <typename THashFunction, typename THash>
    inline const std::string& HashedStringT<THashFunction, THash>::GetString() const {

        return string_;

    }

    template <typename THashFunction, typename THash>
    inline bool HashedStringT<THashFunction, THash>::operator==(const HashedStringT<THashFunction, THash>& other) const {

        return hash_ == other.hash_;

    }

    template <typename THashFunction, typename THash>
    inline bool HashedStringT<THashFunction, THash>::operator!=(const HashedStringT<THashFunction, THash>& other) const {

        return hash_ != other.hash_;

    }

    template <typename THashFunction, typename THash>
    inline bool HashedStringT<THashFunction, THash>::operator<=(const HashedStringT<THashFunction, THash>& other) const {

        return hash_ <= other.hash_;

    }

    template <typename THashFunction, typename THash>
    inline bool HashedStringT<THashFunction, THash>::operator>=(const HashedStringT<THashFunction, THash>& other) const {

        return hash_ >= other.hash_;

    }

    template <typename THashFunction, typename THash>
    inline bool HashedStringT<THashFunction, THash>::operator<(const HashedStringT<THashFunction, THash>& other) const {

        return hash_ < other.hash_;

    }

    template <typename THashFunction, typename THash>
    inline bool HashedStringT<THashFunction, THash>::operator>(const HashedStringT<THashFunction, THash>& other) const {

        return hash_ > other.hash_;

    }

}