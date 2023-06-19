//
// Created by zhaohe on 2023/5/22.
//

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <stdint.h>
#include "main.h"
#include "ac_tree.h"

enum LIGHT_PATTERN
{

    SLOW_FLASHING = 0,
    FAST_FLASHING = 1,
    BREATHE = 2,
    PULSE_FLASHING = 3
};
class Led
{
public:
    explicit Led(GPIO_TypeDef *port, uint16_t pin, uint16_t pwm_freq = 1000, uint16_t base_freq = 50);
    void setPattern(LIGHT_PATTERN pattern);
    void run();
    void setToHighTrigger();
    void setToLowTrigger();
    AcTreeNode *createIndex();
private:
    void _breathe();
    void _slowFlashing();
    void _fastFlashing();
    void _pulseFlashing();
    void _lightUp();
    void _lightDown();
    uint16_t _getTime();
    bool _high_trigger = false;
    uint16_t _res = 20;
    LIGHT_PATTERN _pattern = FAST_FLASHING;
    GPIO_TypeDef *_port;
    uint16_t _pin;
    uint16_t _count = 0;
    uint16_t _time = 0;
    uint16_t _thr = 0;
};


#endif //__H7_LIGHT_H__
