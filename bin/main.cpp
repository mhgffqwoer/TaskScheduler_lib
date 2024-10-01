#include "TTaskScheduler.hpp"

float func(std::pair<float, float> a) { return -4 * a.first * a.second; }

int main() {
    float a = 1;
    float b = -2;
    float c = 0;

    TTaskScheduler ts;

    auto id0 = ts.add([](std::pair<float, float> a){ return func(a); }, std::make_pair(a, c));
    auto id1 = ts.add([](float b, float v){ return b * b + v; }, b, ts.getFutureResult<float>(id0));
    auto id2 = ts.add([](float b, float d){ return -b + std::sqrt(d); }, b, ts.getFutureResult<float>(id1));
    auto id3 = ts.add([](float b, float d){ return -b - std::sqrt(d); }, b, ts.getFutureResult<float>(id1));
    auto id4 = ts.add([](float a, float v){ return v / (2 * a); }, a, ts.getFutureResult<float>(id2));
    auto id5 = ts.add([](float a, float v){ return v / (2 * a); }, a, ts.getFutureResult<float>(id3));
    
    std::cout << "x1 = " << ts.getResult<float>(id4) << std::endl;
    std::cout << "x2 = " << ts.getResult<float>(id5) << std::endl;

    a = 4;

    ts.Reset();
    ts.executeAll();

    std::cout << "x1 = " << ts.getResult<float>(id4) << std::endl;
    std::cout << "x2 = " << ts.getResult<float>(id5) << std::endl;

    ts.PrintDependencies();
}