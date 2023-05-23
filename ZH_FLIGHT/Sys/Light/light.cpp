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

void Led::SetPattern(LIGHT_PATTERN pattern)
{
    _pattern = pattern;
    _thr = 0;
    _time = 0;
    _count = 0;
}

void Led::SetToHighTrigger()
{
    _high_trigger = GPIO_PIN_SET;
}

void Led::SetToLowTrigger()
{
    _high_trigger = GPIO_PIN_RESET;
}

void Led::_Breathe()
{
    if (_count > _thr)
    {
        _LightUp();
    } else
    {
        _LightDown();
    }
    _count++;
    if (_count >= _res)
    {
        _count = 0;
    }
    uint16_t time = _GetTime();
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

void Led::_SlowFlashing()
{
    uint16_t time = _GetTime();
    if (time % 1000 == 0)
    {
        if (time == 0)
        {
            _LightUp();
        } else if (time == 1000)
        {
            _LightDown();
        } else
        {
            _time = 0;
        }
    }
}

void Led::_FastFlashing()
{
    uint16_t time = _GetTime();
    if (time % 300 == 0)
    {
        if (time == 0)
        {
            _LightUp();
        } else if (time == 300)
        {
            _LightDown();
        } else
        {
            _time = 0;
        }
    }
}

void Led::_PulseFlashing()
{
    uint16_t time = _GetTime();
    if (time % 100 == 0)
    {
        if (time == 0)
        {
            _LightUp();
        } else if (time == 100)
        {
            _LightDown();
        } else if (time < 1000)
        {

        } else
        {
            _time = 0;
        }
    }
}

void Led::_LightUp()
{
    if (_high_trigger)
    {
        HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_SET);
    } else
    {
        HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET);
    }
}

void Led::_LightDown()
{
    if (_high_trigger)
    {
        HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET);
    } else
    {
        HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_SET);
    }
}

uint16_t Led::_GetTime()
{
    return _time++;
}

void Led::Run()
{
    switch (_pattern)
    {
        case LIGHT_PATTERN::BREATHE:
            _Breathe();
            break;
        case LIGHT_PATTERN::SLOW_FLASHING:
            _SlowFlashing();
            break;
        case LIGHT_PATTERN::FAST_FLASHING:
            _FastFlashing();
            break;
        case LIGHT_PATTERN::PULSE_FLASHING:
            _PulseFlashing();
            break;
        default:
            break;
    }
}

AcTreeNode *Led::CreateIndex()
{
    AcTreeNode *root = new AcTreeNode();
    AcTreeNode *node = nullptr;

    root->AddData(nullptr, AC_STRUCT, "led", 0);

    node = new AcTreeNode();
    node->AddData((void*)&_pattern, AC_INT8, "light_pattern", 1);
    AcTree::AddNode(root, node);

    return root;
}
