#ifndef COMPONENT_H
#define COMPONENT_H
#pragma once
class Component {

    protected:
        int pin;
        bool estado;

    public:
        Component(){};
        Component(int _pin) : pin(_pin){};
        virtual void iniciar();
        void iniciar(int mode, bool analogic);
};
#endif