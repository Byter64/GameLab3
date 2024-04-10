#include "EngineException.h"
#include "../../extern/StackWalker-master/Main/StackWalker/StackWalker.h"
#include <string>

namespace Engine
{
    class CustomStackWalker : public StackWalker
    {
    public:
        CustomStackWalker() : StackWalker() {}
        std::string GetStackTrace()
        {
            isRecording = false;
            ShowCallstack();
            return stackTrace;
        }

        std::string GetFullStackTrace()
        {
            isRecording = false;
            ShowCallstack();
            return fullStackTrace;
        }
    protected:
        std::string stackTrace{""};
        std::string fullStackTrace{""};
        bool isRecording = false;

        virtual void OnOutput(LPCSTR szText) override
        {
            if(isRecording)
            {
                stackTrace += szText;
                if(strstr(szText, "main.cpp"))
                    isRecording = false;
            }
            else if(strstr(szText, "Engine::EngineException::EngineException"))
                isRecording = true;
            fullStackTrace += szText;
        }
    };

    EngineException::EngineException() : std::exception()
    {
        CustomStackWalker stackWalker;
        stackTrace = stackWalker.GetStackTrace();
        errorName = "EXCEPTION";
        errorDescription = "No description available";
    }

    EngineException::EngineException(std::string errorName, std::string errorDescription) : std::exception()
    {
        CustomStackWalker stackWalker;
        stackTrace = stackWalker.GetStackTrace();
        this->errorName = errorName;
        this->errorDescription = errorDescription;
    }
} // Engine