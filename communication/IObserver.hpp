#pragma once
#include <string>
#include "ISubject.hpp"


class IObserver{
    public:
        virtual ~IObserver(){};
        virtual void OnNotify(const std::vector<std::vector<std::string>>& message)=0;
};