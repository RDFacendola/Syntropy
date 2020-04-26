
#include <vector>
#include <string>

#include "syntropy/language/macro.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/observer_ptr.h"

#include "syntropy/diagnostics/assert.h"

#include "syntropy/core/range.h"

int main(int argc, char **argv)
{
    using namespace syntropy::literals;

    auto fun = std::string(SYNTROPY_FUNCTION);

    auto aof = syntropy::AlignmentOf(fun);

    SYNTROPY_ASSERT(false);

    auto k = 4_GiBytes;

    auto v = std::vector<char>{ 'H', 'e', 'l' };

    auto r = syntropy::MakeRange(v);

    return 0;
}