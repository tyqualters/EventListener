# EventListener
A basic C++ event listener somewhat based off of Node.js' event listener system.

License: GPLv3 (SEE LICENSE FILE FOR MORE DETAILS)

# Documentation
## CreateEventListener
#### void CreateEventListener(void *objAddress, const char *eventName, EventFunction<arguments...> *pfn)
This method will create the event listener. 

It first takes a pointer to the object to attach to. E.G. let's say you have an object called *client*, and you only want events to be handled for *client* - you would use the address of *client* as the first parameter. You may need to cast to a `void*`. If you wish to make the listener global, just set this value to `0` or `nullptr`.

The second argument is the event name. It is a `const char *` or just a normal string literal in C++ (`"Example String"`). This is going to be the name of the event to listen for.

The last argument is the static function pointer. It's a little confusing, but here is a viable example:

```cpp
CreateEventListener(nullptr, "Example", new EventFunction<int, const char*>([&](SEvent event, int a, const char* b){
  static int id = 0;
  std::cout << "Round " << ++id << ' ' << a << ' ' << b << std::endl;
}));
```

The example above creates an EventListener named "Example" that will receive an `int` and `const char *`. The function itself keeps a static variable `id` that is only accessible in the function itself and it will increment on each run. It will then output "Round (#) (a) (b)".

## DeleteEventListener
#### int DeleteEventListener(int id)
Will delete the event listener associated with a specific ID. This ID is not the index in the global array. Instead it is the ID generated whenever the event listener was first created and it's incremental. Which means that the first event listener will get the ID of 1 and the second one will get the ID of 2. The only other way to get this ID is by reading the `SEvent` object when it is passed to the event listener function as established in `CreateEventListener`.

## DeleteEventListeners (1/2)
#### int DeleteEventListeners(void *objAddress)
This will take the object address associated with all event listeners (ref. `CreateEventListener`) and delete all event listeners associated with it.

## DeleteEventListeners (2/2)
#### int DeleteEventListeners(const char *eventName)
This will take the event name associated with all event listeners (ref. `CreateEventListener`) and delete all event listeners associated with it.

## EventFunction
#### using EventFunction = std::function<void(SEvent, arguments...)>;
This is just essentially a `typedef` for a function meant to be used for an event. In other words, it's simply a cast to make life easier (ref. `CreateEventListener`).

## PushEvent (1/2)
#### int PushEvent(void *objAddress, const char *eventName, Args... args)
This will push an event to all event listeners with the same object address and event name.

You may push the specific parameters. Just keep in mind that if you have the same event associated with the same object address, but the arguments do not match, it will throw an error which will be caught internally and not reported unless you compile with `-D __DEBUG` as mentioned under **Important Notes**.

```cpp
// Example pushing an event with an address, int and string
// Address in this case is global
PushEvent(0, "Example", 51, std::string("Test 3"));
```

## PushEvent (2/2)
#### int PushEvent(const char *eventName, Args... args)
Same thing as the first `PushEvent` except it does not check the object address.

## SEvent
#### struct SEvent { int id; uintptr_t address; std::string name; };
This is the normal response object you will get from a standard event listener. It has the event's ID (ref. `CreateEventListener`), the object address, and the name of the event called.

## Any other methods, objects and variables used...
These are not and will not be documented but are pretty self-explanatory. They aren't very useful to know unless you plan on modifying things.

# Important Notes

Important note #1: The functions will not delete themselves. Shouldn't be a problem unless you're automatically generating functions to create events.

Important note #2: There will not be an error thrown for invalid events. What will happen is `PushEvent` will return `0` if no listener fit the criteria.

Important note #3: Listener events that throw an exception will be caught and ignored unless `__DEBUG` flag is defined. If using CLang or GCC you can compile like so:

`g++ example.cpp -o example -D __DEBUG`

`clang++ example.cpp -o example -D __DEBUG`

# Special Thanks
Thank you zero9178#6333 for helping me with figuring out the template issues with this project!
