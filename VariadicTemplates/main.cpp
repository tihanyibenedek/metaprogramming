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

int main()
{
    /// Understanding the need for variadic templates
    {
        using namespace understandingTheNeedForVariadicTemplates;

        std::cout << "min(42, 7)= " << min<int>(42, 7) << std::endl;
        std::cout << "min(1, 5, 3, -4, 9)= " << min<double>(1, 5, 3, -4, 9) << std::endl;
    }


    return 0;
}