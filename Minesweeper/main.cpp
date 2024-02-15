/*===============================
* minesweeper++ - file main.cpp
* ===============================
* author: ben staehle
* date:	  2/7/23
* this program (and file) use the GNU GPL v3 License
* see COPYRIGHT.txt for license related information
* ==================================================*/
#include<iostream>
#include<stdexcept>
#include<string>
#include"engine.h"
#include<cstdlib>
#include<windows.h>

DWORD ExceptionFilter(EXCEPTION_POINTERS* pointers, DWORD dwException)
{
    return EXCEPTION_EXECUTE_HANDLER;
}

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
LPCWSTR GetLastErrorStdStr(DWORD error)
{
    switch (error) {
    case EXCEPTION_ACCESS_VIOLATION:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_ACCESS_VIOLATION";
        break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
        break;
    case EXCEPTION_BREAKPOINT:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_BREAKPOINT";
        break;
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_DATATYPE_MISALIGNMENT";
        break;
    case EXCEPTION_FLT_DENORMAL_OPERAND:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_FLT_DENORMAL_OPERAND";
        break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_FLT_DIVIDE_BY_ZERO";
        break;
    case EXCEPTION_FLT_INEXACT_RESULT:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_FLT_INEXACT_RESULT";
        break;
    case EXCEPTION_FLT_INVALID_OPERATION:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_FLT_INVALID_OPERATION";
        break;
    case EXCEPTION_FLT_OVERFLOW:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_FLT_OVERFLOW";
        break;
    case EXCEPTION_FLT_STACK_CHECK:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_FLT_STACK_CHECK";
        break;
    case EXCEPTION_FLT_UNDERFLOW:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_FLT_UNDERFLOW";
        break;
    case EXCEPTION_GUARD_PAGE:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_GUARD_PAGE";
        break;
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_ILLEGAL_INSTRUCTION";
        break;
    case EXCEPTION_IN_PAGE_ERROR:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_IN_PAGE_ERROR";
        break;
    case EXCEPTION_INT_OVERFLOW:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_INT_OVERFLOW";
        break;
    case EXCEPTION_INVALID_DISPOSITION:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_INVALID_DISPOSITION";
        break;
    case EXCEPTION_INVALID_HANDLE:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_INVALID_HANDLE";
        break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_NONCONTINUABLE_EXCEPTION";
        break;
    case EXCEPTION_PRIV_INSTRUCTION:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_PRIV_INSTRUCTION";
        break;
    case EXCEPTION_SINGLE_STEP:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_SINGLE_STEP";
        break;
    case EXCEPTION_STACK_OVERFLOW:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_EXCEPTION_STACK_OVERFLOW";
        break;
    case STATUS_UNWIND_CONSOLIDATE:
        return L"A Visual C++ Runtime Exception has occured\nWIN32_Exception STATUS_UNWIND_CONSOLIDATE";
        break;
    default:
        return L"Visual C++ Runtime Exception WIN32_NO_EXCEPTION";
    }
}


void run(const char* title) {
    engine::GameEngine ge(20, 20, title, true);

    #ifndef _DEBUG
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    #endif

    ge.run();
}

int main()
{
    const char* title;
    #ifdef _DEBUG
        title = "MINES_DEBUG_";
    #else
        title = "Minesweeper++";
    #endif

    __try {
        run(title);
    }
    __except (ExceptionFilter(GetExceptionInformation(), GetExceptionCode())) {
        if (GetExceptionCode() == STATUS_FLOAT_MULTIPLE_FAULTS) {
            return EXIT_SUCCESS;
        }
        int msgBoxID = MessageBox(
            NULL,
            (LPCWSTR)GetLastErrorStdStr(GetExceptionCode()),
            (LPCWSTR)L"oh my god what did you do",
            MB_ICONERROR | MB_OK
        );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}