#include "ISubject.hpp"
#include "IObserver.hpp"
#include <iostream>
#include <unordered_map>

ISubject::ISubject(){}
ISubject::~ISubject() {}

void ISubject::AddObserver(int bucket, IObserver* observer){
    auto it = m_observers.find(bucket);
    if (it == m_observers.end()){
        m_observers[bucket] = ObserversList();
    }
    
    m_observers[bucket].push_front(observer);
}

void ISubject::RemoveObserver(int bucket, IObserver* observer){

    m_observers[bucket].remove(observer);
}

void ISubject::Notify(int bucket, const std::vector<std::vector<std::string>>& message){
    for (auto o:m_observers[bucket]){
        o->OnNotify(message);
    }
}