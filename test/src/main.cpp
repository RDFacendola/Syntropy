
#include <vector>

#include "syntropy/language/macro.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/observer_ptr.h"

#include "syntropy/core/range.h"

int main(int argc, char **argv)
{
    using namespace syntropy::literals;

    auto k = 4_GiBytes;

    auto v = std::vector<char>{ 'H', 'e', 'l' };

    auto r = syntropy::MakeRange(v);

    return 0;
}