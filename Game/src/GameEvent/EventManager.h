#pragma once

#include "Event.h"
#include <map>
#include <queue>

typedef std::pair<EventType,EventListenerList> Pair;

class EventManager{

public: 

    //Constructor and destructor
    EventManager() {}
    ~EventManager() {}

    //Static class getter
    static EventManager& getInstance();

    //Initilizer
    void init();

    //Updater - It processes and reacts to all events
    void update();

    //Close
    void close();

    //Clear events
    void clearEvents();

    //==================================
    // Event related functions
    //==================================

    //Add an event to the event queue
    void addEvent(Event);

    //Process an event from the queue
    void processEvent(Event);

    //==================================
    // Listener related functions
    //==================================

    //Add a listener to a EventListenerList of the eventListenerMap
    void addListener(EventListener);

private: 

    std::queue<Event> eventQueue;
    std::map<EventType, EventListenerList> eventListenerMap;


};