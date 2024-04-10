#include "BaseException.h"
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
    protected:
        std::string stackTrace{""};
        bool isRecording = false;

        virtual void OnOutput(LPCSTR szText) override
        {
            if(isRecording)
            {
                stackTrace += szText;
                if(strstr(szText, "main.cpp"))
                    isRecording = false;
            }
            else if(strstr(szText, "Engine::BaseException::BaseException"))
                isRecording = true;
        }
    };

    BaseException::BaseException() : std::exception()
    {
        CustomStackWalker stackWalker;
        stackTrace = stackWalker.GetStackTrace();

        errorName = "EXCEPTION";
        errorDescription = "No description available";
    }

    BaseException::BaseException(const char *errorName, const char *errorDescription) : std::exception()
    {
        CustomStackWalker stackWalker;
        stackTrace = stackWalker.GetStackTrace();
        this->errorName = errorName;
        this->errorDescription = errorDescription;
    }
} // Engine