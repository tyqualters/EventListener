#include "eventlistener.hpp"

#include <iostream>

int main()
{
    CreateEventListener(nullptr, "Example", new EventFunction<int, const char*>([&](SEvent event, int a, const char* b){
        static int id = 0;
        std::cout << "Round " << ++id << ' ' << a << ' ' << b << std::endl;

    }));

    // Example pushing an event with no address, int and string
    PushEvent("Example", 50, "Test 1");

    // Example pushing an event with an address, int and string
    PushEvent(0, "Example", 51, std::string("Test 2"));
}
