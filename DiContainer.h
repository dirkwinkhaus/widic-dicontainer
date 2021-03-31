#ifndef WIDIC_DICONTAINER_DICONTAINER_H
#define WIDIC_DICONTAINER_DICONTAINER_H

#include <string>
#include <map>
#include <iostream>

namespace widic {

    class DiContainer;

    template<class T>
    class Factory {
    public:
        virtual T *create(DiContainer *diContainer) const = 0;
    };

    class DiContainer {
    private:
        std::map<std::string, std::string> aliases;
        std::map<std::string, void *> factories;
        std::map<std::string, void *> instances;
    public:
        template<class T>
        DiContainer *set(const char *id, Factory<T> *factory);

        template<class T>
        DiContainer *set(Factory<T> *factory);

        template<class T, class F, typename std::enable_if<std::is_base_of<Factory<T>, F>::value>::type * = nullptr>
        DiContainer *set();

        template<class T>
        T *get(const bool &shared = true);

        template<class T>
        T *get(std::string &id, const bool &shared = true);

        template<class T>
        bool has();

        bool has(const std::string &id);
    };
}

#include "DiContainer.ipp"

#endif
