#include <iostream>
#include <type_traits>
#include <memory>
// #include "source1.cpp"
// #include "source2.cpp"
// #include "wrapper.h"

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

    return 0;
}