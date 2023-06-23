#include "wrapper.h"
#include <iostream>

namespace understandingTemplateInstantiation::explicitInstantiation::explicitInstantiationDeclaration
{
    template wrapper<int>;

    void f()
    {
        extern::wrapper<int> a{42};
        std::cout << a.data << std::endl;
    }
}
