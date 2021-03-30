#ifndef DICONTAINER_DICONTAINER_H
#define DICONTAINER_DICONTAINER_H

#include <string>
#include <map>
#include <iostream>

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
    DiContainer *set(const char* id, Factory<T> *factory) {
        this->factories[id] = factory;
        this->aliases[id] = typeid(T).name();

        return this;
    }

    template<class T>
    DiContainer *set(Factory<T> *factory) {
        this->factories[typeid(T).name()] = factory;

        return this;
    }

    template<class T, class F, typename std::enable_if<std::is_base_of<Factory<T>, F>::value>::type* = nullptr>
    DiContainer *set() {
        this->factories[typeid(T).name()] = new F();

        return this;
    }

    template<class T>
    T *get(const bool &shared = true) {
        const char* requestedName = typeid(T).name();

        if (!this->has<T>()) {
            throw std::exception();
        }

        if (shared && (this->instances.find(requestedName) != this->instances.end())) {
            return static_cast<T *>(this->instances[requestedName]);
        }

        auto const *factory = static_cast<Factory<T>*>(this->factories[requestedName]);
        T *instance = factory->create(this);

        if (shared) {
            this->instances[requestedName] = instance;
        }

        return instance;
    }

    template<class T>
    T *get(std::string &id, const bool &shared = true) {

        if (!this->has(id)) {
            throw std::exception();
        }

        if (shared && (this->instances.find(id) != this->instances.end())) {
            return static_cast<T *>(this->instances[id]);
        }

        auto const *factory = static_cast<Factory<T>*>(this->aliases[id]);
        T *instance = factory->create(this);

        if (shared) {
            this->instances[id] = instance;
        }

        return instance;
    }

    template<class T>
    bool has() {
        return this->factories.find(typeid(T).name()) != this->factories.end();
    }

    bool has(const std::string &id) {
        return this->aliases.find(id) != this->aliases.end();
    }
};

#endif //DICONTAINER_DICONTAINER_H
