#ifndef NDEBUG
#include "assert.h"
#endif

#ifndef NDEBUG
#define XH_ASSERT(statement) assert(statement);
#else
#define XH_ASSERT(statement)
#endif

#define XH_STATIC_ASSERT(statement) std::static_assert(statement);

#include <memory>

#define CLASS_DECLARE(name) class name;
#define CLASS_SHAREDPTR_DECLARE(name) \
    class name; \
    using name##Handle = std::shared_ptr<name>; \
    using name##Ref = std::weak_ptr<name>;