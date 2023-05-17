#include "component.h"

class Tone: public Component {

    public:
    Tone(int _pin): Component(_pin){}
    void iniciar();
    void _tone(int frequency, long duration);
    void _tone(int frequency);
    void _noTone();
};