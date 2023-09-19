#ifndef NDEBUG
#include "assert.h"
#endif

#ifndef NDEBUG
#define ASSERT(statement) assert(statement);
#else
#define ASSERT(statement)
#endif

#define STATIC_ASSERT(statement) std::static_assert(statement);

#include <memory>

#define CLASS_DECLARE(name) class name;
#define CLASS_SHAREDPTR_DECLARE(name) \
    class name; \
    using name##SPtr = std::shared_ptr<name>;