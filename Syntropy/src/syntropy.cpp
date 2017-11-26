#include "syntropy.h"

#include <codecvt>

namespace syntropy
{

    /************************************************************************/
    /* STRING CONVERSION                                                    */
    /************************************************************************/

    std::wstring to_wstring(const std::string& string)
    {

        return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(string);

    }

    std::string to_string(const std::wstring& wstring)
    {

        return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(wstring);

    }

}