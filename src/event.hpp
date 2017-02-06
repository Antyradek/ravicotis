#pragma once
#include <list>
#include <functional>

namespace rav
{
/// \brief You can bind your own functions, own methods, and other object's methods to this and later call them all. Functions must return void and take no arguments.
class Event
{
public:
    /// \brief Connect a function to be called with signal(). You can use event.connect(std::bind(myStaticFun)), or event.connect(std::bind(&MyClass::someFun, myObj)).
    /// \param fun Function to call. You can use std::bind.
    void connect(const std::function<void()>& fun);

    /// \brief Call all functions in the order they were added.
    void signal();

private:
    std::list<std::function<void()>> funSet;
};
}
