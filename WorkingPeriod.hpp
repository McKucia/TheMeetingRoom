#pragma once

struct WorkingPeriod
{
    WorkingPeriod(float _start, float _finish) : start(_start), finish(_finish) {}
    float start, finish;
};