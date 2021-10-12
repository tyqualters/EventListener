#include "eventlistener.hpp"

#include <iostream>

int main()
{
    CreateEventListener(nullptr, "Test", new EventFunction([](SEvent event){
        static int id = 0;
        std::cout << "Test " << ++id << std::endl;

    }));

    PushEvent("Test");
    PushEvent("Test");
    PushEvent("Test");
    PushEvent("Test");
    PushEvent("Test");
    PushEvent("Test");
    PushEvent("Test");
    PushEvent("Test");
    PushEvent("Test");
}
