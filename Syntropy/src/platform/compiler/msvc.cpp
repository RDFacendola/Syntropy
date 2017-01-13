#include "platform/compiler/msvc.h"

namespace syntropy
{
    namespace platform
    {

        //////////////// MSVC BUILTIN ////////////////

        platform::BuiltIn& MSVCBuiltIn::GetInstance()
        {
            static MSVCBuiltIn instance;
            return instance;
        }

        uint64_t MSVCBuiltIn::GetMostSignificantBit(uint64_t number) const
        {
            unsigned long index;
            _BitScanReverse64(&index, number);
            return static_cast<uint64_t>(index);
        }

    }
}
