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

        size_t MSVCBuiltIn::GetLeadingZeroes(uint64_t number) const
        {
            unsigned long index;
            return _BitScanReverse64(&index, number) != 0 ?
                   63 - index :
                   64;
        }

    }
}
