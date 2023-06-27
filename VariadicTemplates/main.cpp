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

namespace understandingParameterPacksExpansion
{
    // Template Parameter List
    template <typename... T>
    struct outer
    {
        template <T... args>
        struct inner { };
    };
    
    // Template Argument List
    template <typename... T>
    struct tag {};

    template <typename T, typename U, typename... Args>
    void tagger()
    {
        tag<T, U, Args...> t1;
        tag<T, Args..., U> t2;
        tag<Args..., T, U> t3;
        tag<U, T, Args...> t4;
    }

    // Funtion Parameter List
    template <typename... Args>
    void make_it(Args... args)
    { }

    // Function Argument List
    template <typename T>
    T step_it(T value)
    {
        return value+1;
    }

    template <typename... T>
    int sum(T... args)
    {
        return (... + args);
    }

    template <typename ...T>
    void do_sums(T... args)
    {
        auto s1 = sum(args...);
        // sum(1,2,3,4);

        auto s2 = sum(42, args...);
        // sum(42,1,2,3,4)

        auto s3 = sum(step_it(args)...);
        // sum(step_it(1), step_it(2), ...., step_it(4))
    }

    // Parenthesized initializers
    template <typename... T>
    struct sum_wrapper
    {
        sum_wrapper(T... args)
        {
            value = (... + args);
        }

        std::common_type_t<T...> value;
    };

    template <typename ...T>
    void paranthesized(T... args)
    {
        std::array<std::common_type_t<T...>,sizeof...(T)>{args...};
        // std::array<int, 4> {1,2,3,4};

        sum_wrapper sw1(args...);
        // value = 1 + 2 + 3 + 4;

        sum_wrapper sw2(++args...);
        // value = 2 + 3 + 4 + 5;
    }

    // Brace-enclosed initializers
    template <typename... T>
    void brace_enclosed(T... args)
    {
        int arr1[sizeof...(args)+1] = {args..., 0};
        // arr1 = {1,2,3,4,0};

        int arr2[sizeof...(args)] = {step_it(args)...};
        // arr2 = {2,3,4,5}
    }

    // Base specifiers and member initializer lists
    struct A 
    {
        void execute() { std::cout << "A::execuce" << std::endl; }
    };
    struct B 
    {
        void execute() { std::cout << "B::execuce" << std::endl; }
    };
    struct C 
    {
        void execute() { std::cout << "C::execuce" << std::endl; }
    };

    template <typename... Bases>
    struct X : public Bases...
    {
        X(Bases const & ...args) : Bases(args)...
        { }

        using Bases::execute...;
    };

    // Using declarations

    // Lambda captures
    template <typename... T>
    void captures(T... args)
    {
        auto l = [args...] {return sum(step_it(args)...);};
        auto s = l();
    }

    // Fold expressions
        // Examples have already been shown earlier
    
    // Alignment specifier
    template <typename... T>
    struct alignment1
    {
        alignas(T...) char a;
    };

    template <int... args>
    struct alignment2
    {
        alignas(args...) char a;
    };
}

namespace variadicClassTemplates
{
    template <typename T, typename... Ts>
    struct tuple
    {
        tuple(T const& t, Ts const&... ts) :
            value(t), rest(ts...)
        { }

        constexpr int size() const { return 1 + rest.size(); }

        T value;
        tuple<Ts...> rest;
    };

    template <typename T>
    struct tuple<T>
    {
        tuple(const T& t) :
            value(t)
        { }

        constexpr int size() const { return 1; }

        T value;
    };

    template <size_t N, typename T, typename... Ts>
    struct nth_type: nth_type<N-1, Ts...>
    {
        static_assert(N < sizeof...(Ts)+1, "Index out of bounds");
    };
    
    template <typename T, typename... Ts>
    struct nth_type<0, T, Ts...>
    {
        using value_type = T;
    };

    template <size_t N>
    struct getter
    {
        template <typename... Ts>
        static typename nth_type<N, Ts...>::value_type& get(tuple<Ts...>& t)
        {
            return getter<N-1>::get(t.rest);
        }
    };

    template <>
    struct getter<0>
    {
        template <typename T, typename... Ts>
        static T& get(tuple<T, Ts...>& t)
        {
            return t.value;
        }
    };

    template <size_t N, typename... Ts>
    typename nth_type<N, Ts...>::value_type &
    get(tuple<Ts...>& t)
    {
        return getter<N>::get(t);
    }
}

namespace foldExpressions
{
    namespace previously
    {
        template <typename T>
        T sum(T a)
        {
            return a;
        }

        template <typename T, typename... Args>
        T sum(T a, Args... args)
        {
            return a + sum(args...);
        }
    }

    template <typename... T>
    int sum(T... args)
    {
        return (... + args);
    }

    template <typename... T>
    int sum_from_zero(T... args)
    {
        return (0 + ... + args);
    }

    template <typename... T>
    void printl(T... args)
    {
        (...,(std::cout << args)) << std::endl;
    }

    template <typename... T>
    void printr(T... args)
    {
        ((std::cout << args), ...) << std::endl;
    }

    template <typename T, typename... Args>
    void push_back_many(std::vector<T>& v, Args&&... args)
    {
        (v.push_back(args),...);
    }
}

namespace variadicAliasTemplates
{
    template <typename T, typename... Args>
    struct foo
    { };

    template <typename... Args>
    using int_foo = foo<int, Args...>;

    template <typename T, T... Ints>
    struct integer_sequence
    { };

    template <std::size_t... Ints>
    using index_sequence = integer_sequence<std::size_t, Ints...>;

    template <typename T, std::size_t N, T... Is>
    struct make_integer_sequence
        : make_integer_sequence<T, N-1, N-1, Is...>
        { };
    
    template <typename T, T... Is>
    struct make_integer_sequence<T, 0, Is...> :
        integer_sequence<T, Is...>
    { };
    
    template <std::size_t N>
    using make_index_sequence = make_integer_sequence<std::size_t, N>;

    template <typename... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;
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

    /// Understanding parameter packs expansion
    {
        using namespace understandingParameterPacksExpansion;

        // outer<int, float, char[5]> a; 

        make_it(42);
        make_it(42, 'a');

        do_sums(1,2,3,4);

        paranthesized(1,2,3,4);

        brace_enclosed(1,2,3,4);

        A a;
        B b;
        C c;
        X x(a, b, c);

        x.A::execute();
        x.B::execute();
        x.C::execute();

        captures(1,2,3,4);

        alignment1<int, double> al1;
        alignment2<1, 4, 8> sl2;
    }

    /// Variadic class templates
    {
        using namespace variadicClassTemplates;

        tuple<int> one(42);
        tuple<int, double> two(42, 42.0);
        tuple<int, double, char> three(42, 42.0, 'a');

        std::cout << one.value << std::endl;
        std::cout << two.value << ", " << two.rest.value << std::endl;
        std::cout << three.value << ", " << three.rest.value << ", " << three.rest.rest.value << std::endl;

        tuple<int, double, char> three2 (42, 42.0, 'a');
        std::cout << "get<2>: " << get<2>(three2) << std::endl;
    }
    
    /// Fold expressions
    {
        using namespace foldExpressions;

        std::cout << sum(1) << std::endl;
        std::cout << sum(1, 2) << std::endl;
        std::cout << sum(1, 2, 3, 4, 5, 6) << std::endl;

        // sum();
        sum_from_zero();

        printl('d', 'o', 'g');
        printr('d', 'o', 'g');

        std::vector<int> v;
        push_back_many(v, 1, 2, 3, 4, 5); 
    }

    /// Variadic alias templates
    {
        using namespace variadicAliasTemplates;

        foo<double, char, int> f1;
        foo<int, char, double> f2;
        int_foo<char, double> f3;

        static_assert(std::is_same_v<decltype(f2), decltype(f3)>);
    }

    return 0;
}