
#include <mutex>
#include <memory>

#include "core/context.hpp"
#include "core/define.hpp"
#include "platform/win32.hpp"


std::once_flag flag;

void PlatformDeleter::operator()(Platform *p)
{
    delete p;
}

Platform* Context::GetWindowsInstance(){
    std::call_once(flag, []() {instance.reset(new Win32(WIDTH, HEIGHT)); });
	return instance.get();
}

std::unique_ptr<Platform, PlatformDeleter> Context::instance;


