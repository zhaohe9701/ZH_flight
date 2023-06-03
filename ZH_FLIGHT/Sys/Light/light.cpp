//
// Created by zhaohe on 2023/5/22.
//

#include "light.h"
#include "sys.h"

Led::Led(GPIO_TypeDef *port, uint16_t pin, uint16_t pwm_freq, uint16_t base_freq)
{
    _port = port;
    _pin = pin;
    _res = pwm_freq / base_freq;
}

void Led::setPattern(LIGHT_PATTERN pattern)
{
    _pattern = pattern;
    _thr = 0;
    _time = 0;
    _count = 0;
}

void Led::setToHighTrigger()
{
    _high_trigger = GPIO_PIN_SET;
}

void Led::setToLowTrigger()
{
    _high_trigger = GPIO_PIN_RESET;
}

void Led::_breathe()
{
    if (_count > _thr)
    {
        _lightUp();
    } else
    {
        _lightDown();
    }
    _count++;
    if (_count >= _res)
    {
        _count = 0;
    }
    uint16_t time = _getTime();
    if (time % 80 == 0)
    {
        if (time < 1600)
        {
            _thr++;
        } else if (time < 2000)
        {
        } else
        {
            _thr--;
        }
        if (0 == _thr)
        {
            _time = 0;
        }
    }
}

void Led::_slowFlashing()
{
    uint16_t time = _getTime();
    if (time % 1000 == 0)
    {
        if (time == 0)
        {
            _lightUp();
        } else if (time == 1000)
        {
            _lightDown();
        } else
        {
            _time = 0;
        }
    }
}

void Led::_fastFlashing()
{
    uint16_t time = _getTime();
    if (time % 300 == 0)
    {
        if (time == 0)
        {
            _lightUp();
        } else if (time == 300)
        {
            _lightDown();
        } else
        {
            _time = 0;
        }
    }
}

void Led::_pulseFlashing()
{
    uint16_t time = _getTime();
    if (time % 100 == 0)
    {
        if (time == 0)
        {
            _lightUp();
        } else if (time == 100)
        {
            _lightDown();
        } else if (time < 1000)
        {

        } else
        {
            _time = 0;
        }
    }
}

void Led::_lightUp()
{
    if (_high_trigger)
    {
        HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_SET);
    } else
    {
        HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET);
    }
}

void Led::_lightDown()
{
    if (_high_trigger)
    {
        HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET);
    } else
    {
        HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_SET);
    }
}

uint16_t Led::_getTime()
{
    return _time++;
}

void Led::run()
{
    switch (_pattern)
    {
        case LIGHT_PATTERN::BREATHE:
            _breathe();
            break;
        case LIGHT_PATTERN::SLOW_FLASHING:
            _slowFlashing();
            break;
        case LIGHT_PATTERN::FAST_FLASHING:
            _fastFlashing();
            break;
        case LIGHT_PATTERN::PULSE_FLASHING:
            _pulseFlashing();
            break;
        default:
            break;
    }
}

AcTreeNode *Led::createIndex()
{
    AcTreeNode *root = new AcTreeNode();
    AcTreeNode *node = nullptr;

    root->addData(nullptr, AC_STRUCT, "led", 0);

    node = new AcTreeNode();
    node->addData((void *) &_pattern, AC_INT8, "light_pattern", 1);
    AcTree::addNode(root, node);

    return root;
}
