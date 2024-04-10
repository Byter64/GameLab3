#include <windows.h>
#include "CrashHandler.h"
#include "Engine.h"
#include <fstream>
#include <errhandlingapi.h>
#include "../../extern/WimXML-master/source/APIHook.h"

namespace Engine::CrashHandler
{
    static std::unique_ptr<std::ofstream> logStream;

    static LONG WINAPI CustomCrashHandler(EXCEPTION_POINTERS *exceptionInfo);
    static const char *ExceptionCodeToString(DWORD code);

    void SetupCrashHandler()
    {
        if (!std::filesystem::exists(Engine::Files::LOGS))
            std::filesystem::create_directory(Engine::Files::LOGS);
        logStream = std::make_unique<std::ofstream>(Engine::Files::LOG);
        *logStream << "LOG: (Hopefully nothing bad happens ): " << std::endl;
        std::cout.rdbuf(logStream->rdbuf());

#ifdef _WIN32
        ::SetUnhandledExceptionFilter(CustomCrashHandler);
#else
        std::cout << "Program seems to running on something else than windows. No crash reports available" << std::endl;
#endif
    }

    static LONG WINAPI CustomCrashHandler(EXCEPTION_POINTERS *exceptionInfo)
    {
        std::cout << "\n";

        int depth = 0;
        for (EXCEPTION_RECORD *record = exceptionInfo->ExceptionRecord; record != nullptr; record = record->ExceptionRecord)
        {

            for (int i = 0; i < depth; i++) std::cout << "\t";
            std::cout << "(" << record->ExceptionCode << ")" << ExceptionCodeToString(record->ExceptionCode) << " occurred at address (PC): " << record->ExceptionAddress << std::endl;
            if (record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
            {
                for (int i = 0; i < depth; i++) std::cout << "\t";
                switch (record->ExceptionInformation[0])
                {
                    case 0:
                        std::cout << "Failed reading at address " << record->ExceptionInformation[1];
                        break;
                    case 1:
                        std::cout << "Failed writing at address " << record->ExceptionInformation[1];
                        break;
                    case 8:
                        std::cout << "Data was tried to be interpreted as code at address " << record->ExceptionInformation[1] << ". How did you even do this!?!?!?";
                        break;
                }
                std::cout << std::endl;
            }


            if (record->ExceptionRecord != nullptr)
            {
                for (int i = 0; i < depth; i++) std::cout << "\t";
                std::cout << "Has nested exception:" << std::endl;
            }
            depth++;
        }

        return EXCEPTION_EXECUTE_HANDLER;
    }

    static const char *ExceptionCodeToString(DWORD code)
    {
        switch (code)
        {
            case EXCEPTION_ACCESS_VIOLATION:return "EXCEPTION_ACCESS_VIOLATION";
            case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
            case EXCEPTION_BREAKPOINT:return "EXCEPTION_BREAKPOINT";
            case EXCEPTION_DATATYPE_MISALIGNMENT:return "EXCEPTION_DATATYPE_MISALIGNMENT";
            case EXCEPTION_FLT_DENORMAL_OPERAND:return "EXCEPTION_FLT_DENORMAL_OPERAND";
            case EXCEPTION_FLT_DIVIDE_BY_ZERO:return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
            case EXCEPTION_FLT_INEXACT_RESULT:return "EXCEPTION_FLT_INEXACT_RESULT";
            case EXCEPTION_FLT_INVALID_OPERATION:return "EXCEPTION_FLT_INVALID_OPERATION";
            case EXCEPTION_FLT_OVERFLOW:return "EXCEPTION_FLT_OVERFLOW";
            case EXCEPTION_FLT_STACK_CHECK:return "EXCEPTION_FLT_STACK_CHECK";
            case EXCEPTION_FLT_UNDERFLOW:return "EXCEPTION_FLT_UNDERFLOW";
            case EXCEPTION_ILLEGAL_INSTRUCTION:return "EXCEPTION_ILLEGAL_INSTRUCTION";
            case EXCEPTION_IN_PAGE_ERROR:return "EXCEPTION_IN_PAGE_ERROR";
            case EXCEPTION_INT_DIVIDE_BY_ZERO:return "EXCEPTION_INT_DIVIDE_BY_ZERO";
            case EXCEPTION_INT_OVERFLOW:return "EXCEPTION_INT_OVERFLOW";
            case EXCEPTION_INVALID_DISPOSITION:return "EXCEPTION_INVALID_DISPOSITION";
            case EXCEPTION_NONCONTINUABLE_EXCEPTION:return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
            case EXCEPTION_PRIV_INSTRUCTION:return "EXCEPTION_PRIV_INSTRUCTION";
            case EXCEPTION_SINGLE_STEP:return "EXCEPTION_SINGLE_STEP";
            case EXCEPTION_STACK_OVERFLOW:return "EXCEPTION_STACK_OVERFLOW";
            default: return "UNHANDLED EXCEPTION (WHAT IS GOING ON HERE!?!?! );";
        }
    }

} // Engine::CrashHandler