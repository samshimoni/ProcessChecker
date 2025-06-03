#pragma once
#include <string>
#include "ISubject.hpp"


class IObserver{
    public:
        virtual ~IObserver(){};
        virtual void OnNotify(const std::unordered_map<std::string, int>& message)=0;
};