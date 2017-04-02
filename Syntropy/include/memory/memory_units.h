
/// \file memory_units.h
/// \brief This header is part of the syntropy memory management system. It contains utilities to convert between different memory units.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

namespace syntropy
{

    /// \brief Exposes utility method for memory unit conversions.
    /// \author Raffaele D. Facendola - April 2017
    class MemoryUnits
    {
    public:

        // Constants

        static constexpr size_t kByte = 0x1;                        ///< \brief Number of Bytes in a Byte.

        // Binary

        static constexpr size_t kKibiByte = kByte << 10;            ///< \brief Number of Bytes in a KibiByte.

        static constexpr size_t kMebiByte = kKibiByte << 10;        ///< \brief Number of Bytes in a MebiByte.

        static constexpr size_t kGibiByte = kMebiByte << 10;        ///< \brief Number of Bytes in a GibiByte.

        static constexpr size_t kTebiByte = kGibiByte << 10;        ///< \brief Number of Bytes in a TebiByte.

        // Decimal

        static constexpr size_t kKiloByte = kByte * 1000;           ///< \brief Number of Bytes in a KiloByte.

        static constexpr size_t kMegaByte = kKiloByte * 1000;       ///< \brief Number of Bytes in a MegaByte.

        static constexpr size_t kGigaByte = kMegaByte * 1000;       ///< \brief Number of Bytes in a GigaByte.

        static constexpr size_t kTeraByte = kGigaByte * 1000;       ///< \brief Number of Bytes in a TeraByte.

        // Conversions

        /// \brief Get the number of Bytes in a number of KibiByte.
        /// \param number Number to convert, in KibiByte.
        /// \return Returns the number of Bytes in number KibiByte. 
        static constexpr size_t FromKibiBytes(size_t number);

        /// \brief Get the number of Bytes in a number of MebiByte.
        /// \param number Number to convert, in MebiByte.
        /// \return Returns the number of Bytes in number MebiByte.
        static constexpr size_t FromMebiBytes(size_t number);

        /// \brief Get the number of Bytes in a number of GibiByte.
        /// \param number Number to convert, in GibiByte.
        /// \return Returns the number of Bytes in number GibiByte.
        static constexpr size_t FromGibiBytes(size_t number);

        /// \brief Get the number of Bytes in a number of TebiByte.
        /// \param number Number to convert, in TebiByte.
        /// \return Returns the number of Bytes in number TebiByte.
        static constexpr size_t FromTebiBytes(size_t number);

    };

}

// Unit conversion

/// \brief User-defined literal used to convert a number from Bytes to Bytes.
/// This method is only used for clarity: Foo(78_Bytes) is better than Foo(78).
/// \param number Number to convert.
constexpr size_t operator "" _Bytes(size_t number)
{
    return number;
}

/// \brief User-defined literal used to convert a number from KibiBytes to Bytes.
/// \param number Number to convert.
constexpr size_t operator "" _KiBytes(size_t number)
{
    return syntropy::MemoryUnits::FromKibiBytes(number);
}

/// \brief User-defined literal used to convert a number from MebiBytes to Bytes.
/// \param number Number to convert.
constexpr size_t operator "" _MiBytes(size_t number)
{
    return syntropy::MemoryUnits::FromMebiBytes(number);
}

/// \brief User-defined literal used to convert a number from GibiBytes to Bytes.
/// \param number Number to convert.
constexpr size_t operator "" _GiBytes(size_t number)
{
    return syntropy::MemoryUnits::FromGibiBytes(number);
}

/// \brief User-defined literal used to convert a number from TebiBytes to Bytes.
/// \param number Number to convert.
constexpr size_t operator "" _TiBytes(size_t number)
{
    return syntropy::MemoryUnits::FromTebiBytes(number);
}

namespace syntropy
{

    /************************************************************************/
    /* MEMORY UNITS                                                         */
    /************************************************************************/

    constexpr size_t MemoryUnits::FromKibiBytes(size_t number)
    {
        return number * kKibiByte;
    }

    constexpr size_t MemoryUnits::FromMebiBytes(size_t number)
    {
        return number * kMebiByte;
    }

    constexpr size_t MemoryUnits::FromGibiBytes(size_t number)
    {
        return number * kGibiByte;
    }

    constexpr size_t MemoryUnits::FromTebiBytes(size_t number)
    {
        return number * kTebiByte;
    }

}
