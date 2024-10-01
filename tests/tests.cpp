#include <gtest/gtest.h>

#include <sstream>

#include "TTaskScheduler.hpp"

TEST(ShedulerTest, Test_1) {
    std::stringstream out;

	TTaskScheduler ts;

    auto id0 = ts.add([](float a, float c){ return -4 * a * c; }, 1, 0);
    auto id1 = ts.add([](float b, float v){ return b * b + v; }, -2, ts.getFutureResult<float>(id0));
    auto id2 = ts.add([](float b, float d){ return -b + std::sqrt(d); }, -2, ts.getFutureResult<float>(id1));
    auto id3 = ts.add([](float b, float d){ return -b - std::sqrt(d); }, -2, ts.getFutureResult<float>(id1));
    auto id4 = ts.add([](float a, float v){ return v / (2 * a); }, 1, ts.getFutureResult<float>(id2));
    auto id5 = ts.add([](float a, float v){ return v / (2 * a); }, 1, ts.getFutureResult<float>(id3));
    
    out << ts.getResult<float>(id4) << ts.getResult<float>(id5);

    ASSERT_EQ(out.str(), "20");
}

TEST(ShedulerTest, Test_2) {
    std::stringstream out;

	TTaskScheduler ts;

	float a = 1;
	float b = -2;
	float c = 0;

    auto id0 = ts.add([](float a, float c){ return -4 * a * c; }, a, c);
    auto id1 = ts.add([](float b, float v){ return b * b + v; }, b, ts.getFutureResult<float>(id0));
    auto id2 = ts.add([](float b, float d){ return -b + std::sqrt(d); }, b, ts.getFutureResult<float>(id1));
    auto id3 = ts.add([](float b, float d){ return -b - std::sqrt(d); }, b, ts.getFutureResult<float>(id1));
    auto id4 = ts.add([](float a, float v){ return v / (2 * a); }, a, ts.getFutureResult<float>(id2));
    auto id5 = ts.add([](float a, float v){ return v / (2 * a); }, a, ts.getFutureResult<float>(id3));

	ts.executeAll();

    out << ts.getResult<float>(id4) << ts.getResult<float>(id5);

    ASSERT_EQ(out.str(), "20");
}

TEST(ShedulerTest, Test_3) {
    std::stringstream out;

	TTaskScheduler ts;

	float a = 1;
	float b = -2;
	float c = 0;

    auto id0 = ts.add([](float a, float c){ return -4 * a * c; }, a, c);
    auto id1 = ts.add([](float b, float v){ return b * b + v; }, b, ts.getFutureResult<float>(id0));
    auto id2 = ts.add([](float b, float d){ return -b + std::sqrt(d); }, b, ts.getFutureResult<float>(id1));
    auto id3 = ts.add([](float b, float d){ return -b - std::sqrt(d); }, b, ts.getFutureResult<float>(id1));
    auto id4 = ts.add([](float a, float v){ return v / (2 * a); }, a, ts.getFutureResult<float>(id2));
    auto id5 = ts.add([](float a, float v){ return v / (2 * a); }, a, ts.getFutureResult<float>(id3));

	ts.executeAll();

    out << ts.getResult<float>(id4) << ts.getResult<float>(id5);

	c = -8;

	ts.Reset();
	ts.executeAll();
	out << ts.getResult<float>(id4) << ts.getResult<float>(id5);

    ASSERT_EQ(out.str(), "204-2");
}