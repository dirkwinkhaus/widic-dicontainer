#include "DiContainer.h"

template<class T>
widic::DiContainer *widic::DiContainer::set(const char *id, Factory<T> *factory) {
    this->factories[id] = factory;
    this->aliases[id] = typeid(T).name();

    return this;
}

template<class T>
widic::DiContainer *widic::DiContainer::set(Factory<T> *factory) {
    this->factories[typeid(T).name()] = factory;

    return this;
}

template<class T, class F, typename std::enable_if<std::is_base_of<widic::Factory<T>, F>::value>::type*>
widic::DiContainer *widic::DiContainer::set() {
    this->factories[typeid(T).name()] = new F();

    return this;
}

template<class T>
T *widic::DiContainer::get(const bool &shared) {
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
T *widic::DiContainer::get(std::string &id, const bool &shared) {

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
bool widic::DiContainer::has() {
    return this->factories.find(typeid(T).name()) != this->factories.end();
}

bool widic::DiContainer::has(const std::string &id) {
    return this->aliases.find(id) != this->aliases.end();
}