
namespace understandingTemplateInstantiation::explicitInstantiation::explicitInstantiationDeclaration
{
    template <typename T>
    struct wrapper
    {
        T data;
    };

    extern template wrapper<int>;
}

