#include <iostream>
#include "DiContainer.h"

class A {
private:
    int value = 1;
public:
    void echo() {
        std::cout << "Class A " << this->value << std::endl;
    }

    void setValue(const int &newValue) {
        this->value = newValue;
    }

    int getValue() const {
        return this->value;
    }
};

class C {
private:
    int value = 1;
    int v = 5550;
public:
    void echo() {
        std::cout << "Class C " << this->value << std::endl;
    }
    virtual int e() {
        return v;
    }

    void setValue(const int &newValue) {
        this->value = newValue;
    }

    int getValue() const {
        return this->value;
    }
};

class B {
private:
    int value = 1;
    A *a;
public:
    explicit B(A *a): a(a) {}
    void echo() {
        std::cout << "Class B contains: " << this->value << " ";
        this->a->echo();
    }

    void setValue(const int &newValue) {
        this->value = newValue;
    }

    int getValue() const {
        return this->value;
    }
};

class FactoryA: public Factory<A> {
public:
    A* create(DiContainer *diContainer) const override{
        return new A();
    }
};

class FactoryC: public Factory<C> {
public:
    C* create(DiContainer *diContainer) const override{
        return new C();
    }
};

class FactoryB: public Factory<B> {
public:
    B* create(DiContainer *diContainer) const override {
        return new B(diContainer->get<A>("A"));
    }
};

int main() {

    auto *di = new DiContainer();

    di->set<A>(new FactoryA());
    di->set<A>("A", new FactoryA());
    di->set<B>(new FactoryB());
    di->set<C, FactoryC>();

    auto a = di->get<A>();
    auto a2 = di->get<A>("A");
    auto b = di->get<B>();
    auto c = di->get<C>();
    auto bb = di->get<B>();
    auto bbc = di->get<B>(false);

    b->setValue(5);
    c->echo();
    c->setValue(1009);
    std::cout << c->e() << std::endl;

    std::cout << c->getValue() << std::endl;
    std::cout << b->getValue() << std::endl;
    std::cout << bb->getValue() << std::endl;
    std::cout << bbc->getValue() << std::endl;
    return 0;
}
