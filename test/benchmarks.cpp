#include <chrono>
#include <iostream>
#include <random>
#include <catch2/catch_test_macros.hpp>

#include "catch2-addons.hpp"
#include "xprec/ddouble.hpp"
#include "xprec/random.hpp"

using xprec::DDouble;
using my_clock = std::chrono::high_resolution_clock;
using duration = std::chrono::duration<double>;

TEST_CASE("exp speed", "[bench]")
{
    std::minstd_rand rng;
    std::uniform_real_distribution<DDouble> dist(-700, 700);
    std::vector<DDouble> x;
    for (int i = 0; i < 1000000; ++i) {
        x.push_back(dist(rng));
    }

    DDouble r = 1.0;
    auto start = my_clock::now();
    for (auto xi : x) {
        r *= xi;
        r *= xi;
    }
    auto stop = my_clock::now();
    duration ref_duration = stop - start;

    r = 1.0;
    start = my_clock::now();
    for (auto xi : x) {
        r *= exp(xi);
        r *= exp(-xi);
    }
    stop = my_clock::now();
    duration my_duration = stop - start;
    duration diff = my_duration - ref_duration;
    REQUIRE_THAT(r, WithinRel<DDouble>(1.0, 1e-16));

    std::cerr << "Rate of exp = "
              << diff.count()/(2*x.size())*1e9
              << " ns/invoc" << std::endl;
}
