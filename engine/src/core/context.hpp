#ifndef CONTEXT_H
#define CONTEXT_H

#include "platform.hpp"


struct PlatformDeleter
{
    void operator()(Platform *p);
};

class Context{
public:
    static Platform* GetWindowsInstance();

private:
    Context() = default;
	Context(const Context& other) = delete;
	Context& operator=(const Context&) = delete;
	~Context(){};

    static std::unique_ptr<Platform, PlatformDeleter> instance;
};


#endif //CONTEXT_H
