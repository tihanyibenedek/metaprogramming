#include "wrapper.h"
#include <iostream>

namespace understandingTemplateInstantiation::explicitInstantiation::explicitInstantiationDeclaration
{
    void g()
    {
        wrapper<int> a {100};
        std::cout << a.data << std::endl;

    }
}