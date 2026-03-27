#ifdef _DEBUG
#if defined(_MSC_VER)
#include <crtdbg.h>
#endif

#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT
#include "support/test_headers.h"

#if defined(_MSC_VER)
static bool runCrtManagerLeakCheck()
{
    _CrtMemState startState;
    _CrtMemState endState;
    _CrtMemState diffState;

    _CrtMemCheckpoint(&startState);
    {
        Manager manager;
        PriceInfo price(10.0, false);

        manager.addItem(new PrintBook("CRT Print", 100, 5.0, EASY, "Author 1", price));
        manager.addItem(new AudioBook("CRT Audio", 120, 6.0, MEDIUM, "Narrator 1", price));
        manager.removeItem(0);
        manager.addItem(new PrintBook("CRT Print 2", 150, 7.0, HARD, "Author 2", price));
    }
    _CrtMemCheckpoint(&endState);

    if (_CrtMemDifference(&diffState, &startState, &endState) != 0)
    {
        _CrtMemDumpStatistics(&diffState);
        _CrtMemDumpAllObjectsSince(&startState);
        return true;
    }

    return false;
}
#endif

int main(int argc, char **argv)
{
    int result = 0;

#if defined(_MSC_VER)
    int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flags |= _CRTDBG_ALLOC_MEM_DF;
    flags &= ~_CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flags);

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
#endif

    {
        doctest::Context context;
        context.applyCommandLine(argc, argv);
        result = context.run();
    }

#if defined(_MSC_VER)
    std::cerr << "\nCRT memory check: ";
    if (runCrtManagerLeakCheck())
    {
        std::cerr << "leaks detected.\n";
    }
    else
    {
        std::cerr << "no leaks detected.\n";
    }
#endif

    return result;
}
#endif
