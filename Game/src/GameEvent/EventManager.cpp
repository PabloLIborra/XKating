#include "EventManager.h"
#include <iostream>


//////////////////////////////////////////////
//             Possible upgrades
//////////////////////////////////////////////
/*
:::>Add event priority queue to events
:::>Add fast sorting of events by multiple byte reading
:::>Add scheduling, by now event manager is useless since it only searches for calls already done in that update, so it just adds innecesary cost
*/
//////////////////////////////////////////////
EventManager& EventManager::getInstance() {
    static EventManager instance;
    return instance;
}

void EventManager::init() {

}

void EventManager::update() {

    while(EventManager::eventQueue.size() != 0){
        Event e = EventManager::eventQueue.front();
        EventManager::eventQueue.pop();
        EventManager::processEvent(e);
    }
}

void EventManager::close() {
    while(!eventQueue.empty()){
        eventQueue.pop();
    }
    eventListenerMap.clear();
}

void EventManager::clearEvents() {
    while(!eventQueue.empty()){
        eventQueue.pop();
    }
}

void EventManager::addEvent(Event event) {
    EventManager::eventQueue.push(event);
}

void EventManager::processEvent(Event event){

    auto auxEventListenerList = EventManager::eventListenerMap.find(event.type);
    
    if(auxEventListenerList != eventListenerMap.end()) {
        auto eventListenerList = auxEventListenerList->second;

        for(auto eventListener : eventListenerList) {
                eventListener.listener(event.data);
        }
    }
}

void EventManager::addListener(EventListener eventListener){
    //Search if the listener already exists
    auto iterator = EventManager::eventListenerMap.find(eventListener.listenerType);

    //Check if there's already a list for our listener and if not, create it
    if(iterator==EventManager::eventListenerMap.end()){
        EventListenerList eventListenerList; //new empty list
        iterator = EventManager::eventListenerMap.insert(Pair(eventListener.listenerType, eventListenerList)).first;
    }

    //Push the function that listens
    iterator->second.push_back(eventListener);

}