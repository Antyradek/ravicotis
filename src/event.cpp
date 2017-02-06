#include "event.hpp"

using namespace rav;

void Event::connect(const std::function<void()>& fun)
{
    funSet.push_back(fun);
}

void Event::signal()
{
    for(std::function<void()> fun : funSet)
    {
        fun();
    }
}
