#include <iostream>
#include <stdarg.h>
#include <array>
#include <functional>

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

namespace parameterPacks
{
    template <typename T, typename... Args>
    T sum(T a, Args ...args)
    {
        if constexpr (sizeof...(args) == 0)
            return a;
        else 
            return a + sum(args...);
    }

    template <typename... Ts>
    constexpr auto get_type_sizes()
    {
        return std::array<std::size_t, sizeof...(Ts)>{sizeof(Ts)...};
    }

    template <typename... Ts, typename... Us>
    constexpr auto multipacks(Ts... args1, Us... args2)
    {
        std::cout << sizeof...(args1) << ", " << sizeof...(args2) << std::endl;
        static_assert(sizeof...(args1) == sizeof...(args2), "paks must be of equal size");
    }

    template <typename, typename>
    struct func_pair;

    template <typename R1, typename... A1,
              typename R2, typename... A2>
    struct func_pair<R1(A1...) , R2(A2...) >
    {
        std::function<R1(A1...)> f;
        std::function<R2(A2...)> g;
    };
    
    bool twice_as(int a, int b)
    {
        return a >= b*2;
    }

    double sum_and_div(int a, int b, double c)
    {
        return (a+b)/c;
    }

    func_pair<bool(int, int), double(int, int, double)> funcs { twice_as, sum_and_div };
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

    /// Parameter packs
    {
        using namespace parameterPacks;

        std::cout << "sum: " << sum(1,2,3,4,5) << std::endl;

        auto sizes = get_type_sizes<short, int, long, long long>();
        // std::cout << "sizes: " << sizes << std::endl;

        // multipacks(1, 2, 3, 4, 5, 6);
        // multipacks<int>(1, 2, 3, 4, 5, 6);
        // multipacks<int, int>(1, 2, 3, 4, 5, 6);
        multipacks<int, int, int>(1, 2, 3, 4, 5, 6);
        // multipacks<int, int, int, int>(1, 2, 3, 4, 5, 6);
        // multipacks<int, int, int, int, int>(1, 2, 3, 4, 5, 6);
        // multipacks<int, int, int, int, int, int>(1, 2, 3, 4, 5, 6);

        // multipacks<int, int>(1, 2, 4.0, 5.0, 6.0);
        multipacks<int, int, int>(1, 2, 3, 4.0, 5.0, 6.0);

        std::cout << "funcs.f: " << funcs.f(42, 12) << std::endl;
        std::cout << "funcs.g: " << funcs.g(42, 12, 10.0) << std::endl;
    }

    return 0;
}