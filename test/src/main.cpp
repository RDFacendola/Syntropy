

#include "syntropy/language/macro.h"

#include "syntropy/memory/observer_ptr.h"

int main(int argc, char **argv)
{
    auto x = int{};

    auto obs = syntropy::MakeObserver(&x);

    return 0;
}