#include <iostream>
#include <stdarg.h>

namespace understandingTheNeedForVariadicTemplates
{
    template <typename T>
    int min(int count, ...)
    {
        va_list args;
        va_start(args, count);

        int val = va_arg(args, T);
        for (int i = 1; i < count; i++)
        {
            int n = va_arg(args, T);
            if (n < val)
                val = n;
        }
        va_end(args);
        return val;
    }
}

namespace variadicFunctionTemplates
{
    template <typename T>
    T min(T a, T b)
    {
        return a < b ? a : b;
    }

    template <typename T, typename... Args>
    T min(T a, Args... args)
    {
        return min(a, min(args...));
    }

    namespace prettyPrint
    {
        template <typename T>
        T min(T a, T b)
        {
            #if defined (__clang__) || defined (__GNUC__) || defined (__GNUG__)
                std::cout << __PRETTY_FUNCTION__ << std::endl;
            #elif defined(_MSVC_VER)
                std::cout << __FUNCSIG__ << std::endl;
            #endif
                return a < b ? a : b;
        }

        template <typename T, typename... Args>
        T min(T a, Args... args)
        {
            #if defined (__clang__) || defined (__GNUC__) || defined (__GNUG__)
                std::cout << __PRETTY_FUNCTION__ << std::endl;
            #elif defined(_MSVC_VER)
                std::cout << __FUNCSIG__ << std::endl;
            #endif
                return min(a, min(args...));
        }
    }
}

int main()
{
    /// Understanding the need for variadic templates
    {
        using namespace understandingTheNeedForVariadicTemplates;

        std::cout << "min(42, 7)= " << min<int>(42, 7) << std::endl;
        std::cout << "min(1, 5, 3, -4, 9)= " << min<double>(1, 5, 3, -4, 9) << std::endl;
    }

    /// Variadic function templates
    {
        using namespace variadicFunctionTemplates;

        std::cout << "min(42.0, 75)= " << min(42.0, 7.5) << std::endl;
        std::cout << "min(1, 5, 3, -4, 9)= " << min(1, 5, 3, -4, 9) << std::endl;

        prettyPrint::min(1, 5, 3, -4, 9);
    }

    return 0;
}