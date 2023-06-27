#include <iostream>
#include <type_traits>
#include <memory>
// #include "source1.cpp"
// #include "source2.cpp"
// #include "wrapper.h"
#include <string>
#include <array>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

namespace definingFunctionTemplates
{
    template<typename T>
    T add(T const a, T const b)
    {
        return a + b;
    }

    class foo
    {
        int value;
    public:
        explicit foo (int const i) : value(i)
        { }
        explicit operator int() const { return value; }
    };
    
    foo operator+ (foo const a, foo const b)
    {
        return foo(int(a)+int(b));
    }

    template<typename Input, typename Predicate>
    int count_if(Input start, Input end, Predicate p)
    {
        int total = 0;
        for (Input i = start; i != end; i++)
        {
            if (p(*i))
                total++;
        }
        return total;
    }
}

namespace definingClassTemplates
{
    template <typename T>
    class wrapper
    {
    public:
        wrapper(T const v): value(v)
        {}

        T const& get() const { return value; }

    private:
        T value;
    };

    // template <typename T = int>
    // class wrapper;

    void use_wrapper(wrapper<int>* ptr)
    {
        std::cout << ptr->get() << std::endl;
    }
}

namespace definingMemberFunctionTemplates
{
    template <typename T>
    class composition_v1
    {
    public:
        T add(T const a, T const b)
        {
            return a+b;
        }
    };

    class composition_v2
    {
    public:
        template <typename T>
        T add(T const a, T const b)
        {
            return a + b;
        }
    };

    template <typename T>
    class wrapper
    {
    public:
        wrapper(T const v): value(v)
        {}

        T const& get() const { return value; }

        template <typename U>
        U as() const
        {
            return static_cast<U>(value);
        }
    private:
        T value;
    };
}

namespace understandingTemplateParameters
{
    namespace typeTemplateParameters
    {
        template <typename>
        class wrapper;

        template <typename = int>
        class wrapper;

        // template <typename... T>
        // class wrapper;
    }

    namespace nonTypeTemplateParameters
    {
        template <int V>
        class foo
        { };

        template <int V = 42>
        class foo;

        // template <int... V>
        // class foo;

        template <typename T, size_t S>
        class buffer
        {
            T data_[S];
        public:
            constexpr T const * data() const { return data_; }

            constexpr T& operator[] (size_t const index)
            {
                return data_[index];
            }

            constexpr T const& operator[] (size_t const index) const
            {
                return data_[index];
            }
        };

        // template <>
        // class buffer<int, 10>
        // {
        //     int data_[10];
        // public:
        //     constexpr int* data() const;
        //     constexpr int& operator[] (const size_t index);
        //     constexpr const int& operator[] (const size_t index) const;
        // };

        struct device
        {
            virtual void output() = 0;
            virtual ~device() {}
        };

        template <void (*action) ()>
        struct smart_device : device
        {
            void output() override
            {
                (*action)();
            }
        };

        void say_hello_in_english()
        {
            std::cout << "Hello, World!" << std::endl;
        }

        void say_hello_in_spanish()
        {
            std::cout << "Hola Mundo!" << std::endl;
        }

        template <typename Command, void (Command::*action)()>
        struct smart_device_new : device
        {
            smart_device_new(Command& command): cmd(command) { }
            void output() override
            {
                (cmd.*action)();
            }
        private:
            Command& cmd;
        };

        struct hello_command
        {
            void say_hello_in_english()
            {
                std::cout << "Hello World!" << std::endl;
            }

            void say_hello_in_spanish()
            {
                std::cout << "Hola Mundo!" << std::endl;
            }
        };

        // template <auto... X>
        // struct foo
        // {

        // };
    }
}

namespace understandingTemplateParameters::templateTemplateParameters
{
    template <typename T>
    class simple_wrapper
    {
    public:
        T value;
    };

    template <typename T>
    class fancy_wrapper
    {
    public:
        fancy_wrapper(T const v): value(v) { }

        T const& get() const { return value; }

        template <typename U>
        U as() const
        {
            return static_cast<U>(value);
        }
    private:
        T value;
    };

    template <typename T, typename U, template<typename> typename W>
    class wrapping_pair
    {
    public:
        wrapping_pair(T const a, U const b) :
            item1(a),
            item2(b)
            { }
        
        W<T> item1;
        W<U> item2;
    };
}

namespace understandingTemplateParameters::defaultTemplateArguments
{
    template <typename T = int>
    class foo
    { };

    template <typename T = int, typename U = double>
    class bar 
    { };

    // template <typename T = int, typename U> /// Error
    // class foo3
    // { };
    
    template <typename T = int, typename U>
    void func() {}

    template <typename T, typename U = double>
    struct baz;

    template <typename T = int, typename U>
    struct baz;

    template <typename T, typename U>
    struct baz
    {   
        T a;
        U b;
    };
}

namespace understandingTemplateInstantiation::implicitInstantiation
{
    template <typename T>
    struct foo
    {
        void f() {}
        void g() {int a = 42;}
    };

    template<>
    struct foo<int>
    {
        inline void f() {}
    };

    template <typename T>
    struct control
    { };

    template <typename T>
    struct button : public control<T>
    { };

    void show(button<int>* ptr)
    {
        control<int>*c = ptr;
    }

    template <typename T>
    struct bar  
    {
        static T data;
    };
    
    template <typename T> T bar<T>::data = 0;
}

namespace understandingTemplateInstantiation::explicitInstantiation
{
    namespace ns
    {
        template <typename T>
        struct wrapper
        {
            T value;
        };

        template struct wrapper<int>;
    }
    template struct ns::wrapper<double>;

    namespace ns 
    {
        template <typename T>
        T add(T const a, T const b)
        {
            return a+b;
        }

        template int add(int, int);
    }
    template double ns::add(double, double);

    template <typename T>
    class foo
    {
        struct bar {};

        T f(bar const arg)
        {
            return {};
        }
    };
    template int foo<int>::f(foo<int>::bar);


}

namespace understandingTemplateSpecialization::explicitSpecialization
{
    template <typename T>
    struct is_floating_point
    {
        constexpr static bool value = false;
    };

    template <>
    struct is_floating_point<float>
    {
        constexpr static bool value = true;
    };

    template <>
    struct is_floating_point<double>
    {
        constexpr static bool value = true;
    };

    template <>
    struct is_floating_point<long double>
    {
        constexpr static bool value = true;
    };

    template <typename T>
    struct foo
    {
        static T value;
    };
    template <typename T> T foo<T>::value = 0;
    template <> int foo<int>::value = 42;
    
    namespace incompleteType
    {
        template <typename>
        struct foo {}; // primary template

        template <>
        struct foo<int>; // explicit specialization template
    }

    namespace deduceTemplateArguemnt
    {
        template <typename T>
        struct foo {};

        template <typename T>
        void func(foo<T>)
        {
            std::cout << "primary template" << std::endl;
        }

        template <>
        void func<int>(foo<int>) // void func(foo<int>)
        {
            std::cout << "int specialization" << std::endl;
        }
    }

    namespace multipleArguments
    {
        template <typename T, typename U>
        void func(T a, U b)
        {
            std::cout << "primary template" << std::endl;
        }

        template <>
        void func(int a, int b)
        {
            std::cout << "int - int specialization" << std::endl;
        }

        template <>
        void func(int a, double b)
        {
            std::cout << "int - double specialization" << std::endl;
        }
    }
}
namespace understandingTemplateSpecialization::partialSpecialization
{
    template <typename T, int S> // two template arguments: a type template argument and a non-type template argument
    struct collection
    {
        void operator()()
        {
            std::cout << "primary template" << std::endl;
        }
    };

    template <typename T>
    struct collection<T, 10>
    {
        void operator()()
        {
            std::cout << "partial specialization <T, 10>" << std::endl;
        }
    };

    template <int S>
    struct collection<int, S>
    {
        void operator()()
        {
            std::cout << "partial specializtaio <int, S>" << std::endl;
        }
    };

    template <typename T, int S>
    struct collection<T*, S>
    {
        void operator()()
        {
            std::cout << "partial specialization <T*, S" << std::endl;
        }
    };

    namespace realWorldExample
    {
        template <typename T, size_t S>
        std::ostream& pretty_print(std::ostream& os, std::array<T,S> const& arr)
        {
            os << '[';
            if (S > 0)
            {
                size_t i = 0;
                for (; i < S -1; ++i)
                    os << arr[i] << ',';
                os << arr[S-1];
            }
            os << ']';
            return os;
        }

        template <size_t S>
        std::ostream& pretty_print(std::ostream& os, std::array<char, S> const& arr)
        {
            os << '[';
            for (auto const& e : arr)
                os << e;
            os << ']';
            return os;
        }
    }
}

namespace definingVariableTemplates
{
    template <typename T>
    constexpr T PI = T(3.1415926535897932385L);

    template <typename T>
    T sphere_volume(T const r)
    {
        return 4 * PI<T> * r * r * r / 3;
    }

    template <typename T>
    constexpr T SEPARATOR = '\n';

    template <>
    constexpr wchar_t SEPARATOR<wchar_t> = L'\n';

    template <typename T>
    std::basic_ostream<T>& show_parts(std::basic_ostream<T>& s, std::basic_string_view<T> const& str)
    {
        using size_type = typename std::basic_string_view<T>::size_type;
        size_type start = 0;
        size_type end;

        do
        {
            end = str.find(SEPARATOR<T>, start);
            s << '[' << str.substr(start, end - start) << ']' << SEPARATOR<T>;
            start = end+1;
        } while (end != std::string::npos);
        return s;
    }

    struct match_constant
    {
        template<class T>
        static const T PI;  // not static constexpr
    };
    
    template <class T>
    const T match_constant::PI = T(3.1415926535897932385L);

    template <typename T>
    struct is_floating_point
    {
        constexpr static bool value = false;
    };

    template <typename T>
    inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
}

namespace definingAliasTemplates
{
    template <typename T>
    using customer_address_t = std::map<int, std::vector<T>>;

    struct delivery_address_t {};
    struct invoice_address_t {};

    using customer_delivery_adress_t = customer_address_t<delivery_address_t>;
    using customer_invoice_address_t = customer_address_t<invoice_address_t>;
}

namespace exploringGenericLambdasAndLambdaTemplates
{

}

int main()
{
    /// "Defining function templates"
    {
        using namespace definingFunctionTemplates;

        auto a = add(42, 21);
        auto b = add<int>(42, 21);
        auto c = add<>(42, 21);

        auto e = add<short>(42, 21);

        // auto f = add(42.0, 21);  template argument deduction/substitution failed: 
        auto f = add<double>(42.0, 21);

        auto g = add(foo(42), foo(21));

        int arr[] {1,1,2,3,5,8,11};
        int odds = count_if(std::begin(arr), std::end(arr), [](int const n){
            return n % 2 == 1; });
        
        // std::cout << "odds: " << odds << std::endl;
    }

    /// Defining class templates
    {
        using namespace definingClassTemplates;

        wrapper a(42);
        wrapper<int> b(42);
        wrapper<short> c(42);
        wrapper<double> d(42.0);
        wrapper e("42");

        use_wrapper(&a);
    }

    // Defining member function templates
    {
        using namespace definingMemberFunctionTemplates;

        composition_v1<int> c1;
        c1.add(42, 21);

        composition_v2 c2;
        c2.add(42, 21); // c2.add<int>(42, 21);

        wrapper wda(42.0); // wrapper<double> wda<42.0>
        auto d2 = wda.get();
        auto d3 = wda.as<int>();
    }

    /// Understanding template parameters
    {
        using namespace understandingTemplateParameters::nonTypeTemplateParameters;

        buffer<int, 10> b1;
        buffer<int, 2*5> b2;
        buffer<int, 3*5> b3;

        static_assert(std::is_same_v<decltype(b1), decltype(b2)>);
        static_assert(!std::is_same_v<decltype(b1), decltype(b3)>);

        auto w1 = std::make_unique<smart_device<&say_hello_in_english>>();
        w1->output();

        auto w2 = std::make_unique<smart_device<&say_hello_in_spanish>>();
        w2->output();

        static_assert(!std::is_same_v<decltype(w1), decltype(w2)>);

        std::unique_ptr<device> w11 = std::make_unique<smart_device<&say_hello_in_english>>();
        w11->output();

        std::unique_ptr<device> w22 = std::make_unique<smart_device<&say_hello_in_spanish>>();
        w22->output();

        static_assert(std::is_same_v<decltype(w11), decltype(w22)>);

        hello_command cmd;

        auto ww1 = std::make_unique<smart_device_new<hello_command, &hello_command::say_hello_in_english>>(cmd);
        ww1->output();
        auto ww2 = std::make_unique<smart_device_new<hello_command, &hello_command::say_hello_in_spanish>>(cmd);
        ww2->output();

        // foo<42, 42.0, false, 'X'> f;
    }

    /// Template template parameters
    {
        using namespace understandingTemplateParameters::templateTemplateParameters;
        
        wrapping_pair<int, double, fancy_wrapper> p1(42, 42.0);
        std::cout << p1.item1.get() << " " << p1.item2.get() << std::endl;

        // wrapping_pair<int, double, simple_wrapper> p2(42, 42.0);
        // std::cout << p2.item1.value << " " << p2.item2.value << std::endl;
    }

    /// Default template arguments
    {

    }

    
    /// Understanding template instantiation

    // Implicit instantiation
    {
        using namespace understandingTemplateInstantiation::implicitInstantiation;

        foo<int> x;
        x.f();

        foo<int>* p;
        foo<double>* q;
        q->g();

        bar<int> a;
        bar<double> b;
        bar<double> c;

        std::cout << a.data << std::endl;
        std::cout << b.data << std::endl;
        std::cout << c.data << std::endl;

        b.data = 42;

        std::cout << a.data << std::endl;
        std::cout << b.data << std::endl;
        std::cout << c.data << std::endl;

    }

    // Explicit instantiation
    {
        // using namespace understandingTemplateInstantiation::explicitInstantiation::explicitInstantiationDeclaration;

        // wrapper<int> a{0};
    }


    /// Understanding template specialization

    // Explicit specialization
    {
        using namespace understandingTemplateSpecialization::explicitSpecialization;

        std::cout << is_floating_point<int>::value << std::endl;
        std::cout << is_floating_point<float>::value << std::endl;
        std::cout << is_floating_point<double>::value << std::endl;
        std::cout << is_floating_point<long double>::value << std::endl;
        std::cout << is_floating_point<std::string>::value << std::endl;

        foo<double> a, b;
        foo<int> c;

        std::cout << a.value << std::endl;
        std::cout << b.value << std::endl;
        std::cout << c.value << std::endl;

        a.value = 100;

        std::cout << a.value << std::endl;
        std::cout << b.value << std::endl;
        std::cout << c.value << std::endl;

        incompleteType::foo<double> ia;
        incompleteType::foo<int*> ib;
        // incompleteType::foo<int> ic; // error, foo<int> incomplete type

        multipleArguments::func(1, 2); // int - int spec
        multipleArguments::func(1, 2.0); // int - double spec
        multipleArguments::func(1.0, 2.0); // primary 

    }

    // Partial specialization
    {
        using namespace understandingTemplateSpecialization::partialSpecialization;

        collection<char, 42> a; // primary template
        collection<int, 42> b; // partial specialization <int, S>
        collection<char, 10> c; // partial specialization <T, 10>
        collection<int*, 20> d; // partial specialization <T*, S>

        // collection<int, 10> e; // error: collection<T,10> or collection<int,S>
        // collection<int*, 10> f;  // error: collection<T,10> or collection<T*,S>

        std::array<int,9> arr {1,1,2,3,5,8,13,21};
        realWorldExample::pretty_print(std::cout, arr);
        std::array<char, 9> str;
        std::strcpy(str.data(), "template");
        realWorldExample::pretty_print(std::cout, str);
    }
    
    /// Defining variable templates
    {
        using namespace definingVariableTemplates;

        show_parts<char>(std::cout, "one\ntwo\nthree");
        show_parts<wchar_t>(std::wcout, L"one line");

        std::cout << is_floating_point<float>::value << std::endl;
        std::cout << is_floating_point_v<float> << std::endl;
    }
    
    /// Defining variable templates
    {
        using namespace definingAliasTemplates;
    }

    /// Defining alias templates
    {
        using index_t = int;
        using NameValueList = std::vector<std::pair<int, std::string>>;
        using fn_ptr = int (*)(int, char);

    }

    /// Exploring generic lambdas and lambda templates
    {
        int arr[] = {1,6,3,8,4,2,9};
        std::sort(std::begin(arr), std::end(arr), 
        [](int const a, int const b){ return b > a; });

        int pivot = 5;
        auto count = std::count_if(std::begin(arr), std::end(arr), [pivot](int const a){ return a > pivot;});

        struct __lambda_1
        {
            inline bool operator() (const int a, const int b ) const 
            {
                return a > b;
            }
        };

        struct __lambda_2
        {
            __lambda_2(int& _pivot) : pivot(_pivot) { }

            inline bool operator() (const int a) const
            {
                return a > pivot;
            }
        private:
            int pivot;
        };

        auto l1 =  [](int a) { return a + a; }; // C++11, regular lambda
        std::cout << "regular lambda: " <<  l1(1) << std::endl;
        auto l2 = [](auto a ){ return a + a; };  // C++14, generic lambda
        std::cout << "generic lambda: " <<  l2(2) << std::endl;
        auto l3 = []<typename T, typename U>(T a, U b) { return a + b; }; // C++20, template lambda
        std::cout << "template lambda: " <<  l3(3, 1) << std::endl;
        auto l5 = []<typename T>(T a, T b){ return a + b; };
        auto l4 = [](auto a, decltype(a)b) { return a + b; };

        auto v1 = l1(42);
        auto v2 = l1(42.0); // warning
        // auto v3 = l1(std::stirng("42")); // error

        auto v4 = l2(42);
        auto v5 = l2(42.0);
        auto v6 = l2(std::string("42"));

        auto v7 = l3(42, 1);
        auto v8 = l3(42.0, 1.0);
        auto v9 = l3(std::string("42"), '1');

        auto v10 = l5(42, 1);
        // auto v11 = l5(42, 1.0); // error
        auto v12 = l5(42.0, 1.0);
        // auto v13 = l5(42.0, false); // error
        auto v14 = l5(std::string{"42"}, std::string{"1.0"}); 
        // auto v15 = l5(std::string{"42"}, '1'); // error

        auto v15 = l4(42.0, 1);
        auto v16 = l4(42, 1.0);
        // auto v17 = l4(std::string{"42"}, '1'); // error 

        auto l = []<typename T, size_t N>(std::array<T,N> const& arr)
        {
            return std::accumulate(arr.begin(), arr.end(), static_cast<T>(0));
        };

        // auto v20 = l(1); // error 
        auto v21 = l(std::array<int, 3>{1,2,3});

        std::function<int(int)> factorial;
        factorial = [&factorial](int const n) {
            if (n < 2) return 1;
            else return n * factorial(n-1);
        };

        std::cout << "factorial: " << factorial(5) << std::endl;

        auto factorial_t = [](auto f, int const n) {
            if (n < 2) return 1;
            else return n * f(f, n-1);
        };

        std::cout << "factorial_t: " << factorial_t(factorial_t, 5) << std::endl;
    }

    return 0;
}