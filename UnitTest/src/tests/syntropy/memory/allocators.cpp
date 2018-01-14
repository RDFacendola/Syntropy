#include "tests/syntropy/memory/allocators.h"

#include "macro.h"

#include "reflection/class.h"

#include "unit_test/test_runner.h"

/************************************************************************/
/* TEST SYNTROPY MEMORY ALLOCATORS                                      */
/************************************************************************/

syntropy::AutoTestSuite<TestSyntropyMemoryAllocators> suite("syntropy.memory.allocators");

std::vector<syntropy::TestCase> TestSyntropyMemoryAllocators::GetTestCases()
{
    return
    {
        { "memory context", &TestSyntropyMemoryAllocators::TestMemoryContext }
    };
}

TestSyntropyMemoryAllocators::TestSyntropyMemoryAllocators()
{
    // Initialization of the memory manager

    // #TODO This test should not rely on the reflection system and its configuration should not be provided by an external file (since it would most likely change the test result).

    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::LinearSegregatedFitAllocator>());
    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::ExponentialSegregatedFitAllocator>());
    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::TwoLevelSegregatedFitAllocator>());
    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::LayeredAllocator>());

    syntropy::ImportMemoryConfigurationFromJSON("memory.cfg");
}

void TestSyntropyMemoryAllocators::TestMemoryContext()
{
    // #TODO This is not a proper unit test: it resembles an integration test which cannot be verified (since the output is provided to the logging system).

    void* p;
    void* q;
    void* r;

    {
        syntropy::MemoryContext ctx1("MasterAllocator2");

        p = SYNTROPY_MM_ALLOC(23_Bytes);

        {
            syntropy::MemoryContext ctx2("MasterAllocator1");

            q = SYNTROPY_MM_ALLOC(24_KiBytes);
            r = SYNTROPY_MM_ALLOC(2_MiBytes);
        }

        SYNTROPY_MM_FREE(p);        // TODO: quirk! p was allocated by "small", the actual allocator who handled the call was "master2". Incidentally also "master1" references "small", so it will take care of its deallocation. (that's correct but a little bit obscure)
        SYNTROPY_MM_FREE(q);
        SYNTROPY_MM_FREE(r);
    }
}