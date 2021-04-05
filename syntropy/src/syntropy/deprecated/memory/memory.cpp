#include "syntropy/memory/memory.h"

#include "syntropy/diagnostics/foundation/assert.h"
#include "syntropy/math/math.h"

namespace Syntropy::Memory
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/



    Bytes Gather(const RWByteSpan& destination, InitializerList<ByteSpan> sources)
    {
        auto gather = destination;

        for (auto&& source : sources)
        {
            auto count = Copy(gather, source);

            // gather = PopFront(gather, ToInt(count));
        }

        return Size(ByteSpan{ destination.GetData(), gather.GetData() });
    }

    Bytes Scatter(InitializerList<RWByteSpan> destinations, const ByteSpan& source)
    {
        auto scatter = source;

        for (auto&& destination : destinations)
        {
            auto count = Copy(destination, scatter);

            // scatter = PopFront(scatter, ToInt(count));
        }

        return Size(ByteSpan{ source.GetData(), scatter.GetData() });
    }

}
