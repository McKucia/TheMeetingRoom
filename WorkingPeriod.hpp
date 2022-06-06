#pragma once

struct WorkingPeriod
{
    WorkingPeriod(float _start, float _finish) : start(_start), finish(_finish) {}
    const float getTime() const { return finish - start; }
    float start, finish;
};