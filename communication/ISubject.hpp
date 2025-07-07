#include <forward_list>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>


#pragma once

class IObserver;

class ISubject{
    public:
        ISubject();
        virtual ~ISubject();

        virtual void AddObserver(int bucket, IObserver* observer);
        virtual void RemoveObserver(int bucket, IObserver* observer);

        virtual void Notify(int bucket, const std::vector<std::vector<std::string>>& message);

    private:
        typedef std::forward_list<IObserver*> ObserversList;
        typedef std::map<int, ObserversList> ObserversMap;

        ObserversMap m_observers;
};