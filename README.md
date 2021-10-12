# EventListener
C++ Event Listener

Important note #1: The functions will not delete themselves. Shouldn't be a problem unless you're automatically generating functions to create events.

Important note #2: There will not be an error thrown for invalid events. What will happen is PushEvent will return 0 if no listener fit the criteria.
