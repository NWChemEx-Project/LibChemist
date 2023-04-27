/*
 * Copyright 2023 NWChemEx-Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <catch2/catch.hpp>
#include <chemist/point/point_set.hpp>

using namespace chemist;

/* Testing Notes:
 *
 * We assume the base class has been tested and works correctly. Subject to
 * that assumption, we need to test the ability to access points by reference
 * (since we wrote the implementation), and obtaining the size, but do not need
 * to retest all the ways one can access the points (i.e., operator[], at, and
 * iterators).
 */
TEMPLATE_TEST_CASE("PointSet", "", float, double) {
    using set_type   = PointSet<TestType>;
    using value_type = typename set_type::value_type;

    value_type p0(0.0, 1.0, 2.0);
    value_type p1(3.0, 4.0, 5.0);

    set_type defaulted;
    set_type points{p0, p1, p1}; // <- ensure we can add same point 2x

    // Collect the addresses for testing aliasing/references
    std::vector<TestType*> old_addresses;
    for(std::size_t point_i = 0; point_i < 3; ++point_i) {
        for(std::size_t coord_i = 0; coord_i < 3; ++coord_i) {
            old_addresses.push_back(&points[point_i].coord(coord_i));
        }
    }

    SECTION("Ctor") {
        SECTION("default") { REQUIRE(defaulted.size() == 0); }

        SECTION("initializer_list") {
            REQUIRE(points.size() == 3);
            REQUIRE(points[0] == p0);
            REQUIRE(points[1] == p1);
            REQUIRE(points[2] == p1);
        }

        SECTION("Copy") {
            set_type copy0(defaulted);
            set_type copy1(points);
            REQUIRE(copy0 == defaulted);
            REQUIRE(copy1 == points);
        }

        SECTION("Move") {
            set_type copy0(defaulted);
            set_type move0(std::move(defaulted));
            set_type copy1(points);

            // Do the move
            set_type move1(std::move(points));

            // Check the values
            REQUIRE(copy0 == move0);
            REQUIRE(copy1 == move1);

            // Check that original references are still valid
            for(std::size_t point_i = 0; point_i < 3; ++point_i) {
                for(std::size_t coord_i = 0; coord_i < 3; ++coord_i) {
                    const auto idx = point_i * 3 + coord_i;
                    const auto p   = &move1[point_i].coord(coord_i);
                    REQUIRE(old_addresses[idx] == p);
                }
            }
        }

        SECTION("Copy assignment") {
            set_type copy0;
            auto pcopy0 = &(copy0 = defaulted);
            set_type copy1;
            auto pcopy1 = &(copy1 = points);

            // Check values
            REQUIRE(copy0 == defaulted);
            REQUIRE(copy1 == points);

            // Check returns *this for chaining
            REQUIRE(pcopy0 == &copy0);
            REQUIRE(pcopy1 == &copy1);
        }

        SECTION("Move assignment") {
            set_type copy0(defaulted);
            set_type move0;
            auto pmove0 = &(move0 = std::move(defaulted));
            set_type copy1(points);
            set_type move1;

            // Do the move
            auto pmove1 = &(move1 = std::move(points));

            // Check values
            REQUIRE(copy0 == move0);
            REQUIRE(copy1 == move1);

            // Check returns *this for chaining
            REQUIRE(pmove0 == &move0);
            REQUIRE(pmove1 == &move1);

            // Check that original references are still valid
            for(std::size_t point_i = 0; point_i < 3; ++point_i) {
                for(std::size_t coord_i = 0; coord_i < 3; ++coord_i) {
                    const auto idx = point_i * 3 + coord_i;
                    const auto p   = &move1[point_i].coord(coord_i);
                    REQUIRE(old_addresses[idx] == p);
                }
            }
        }
    }

    SECTION("push_back") {
        defaulted.push_back(p0);
        defaulted.push_back(p1);
        defaulted.push_back(p1);
        REQUIRE(defaulted == points);
    }

    SECTION("at_()") {
        using rtype = decltype(points[0]);
        STATIC_REQUIRE(std::is_same_v<rtype, typename set_type::reference>);

        for(std::size_t point_i = 0; point_i < 3; ++point_i) {
            const auto corr = point_i == 0 ? p0 : p1;
            for(std::size_t coord_i = 0; coord_i < 3; ++coord_i) {
                auto& coord = points[point_i].coord(coord_i);

                REQUIRE(coord == corr.coord(coord_i));
                REQUIRE(old_addresses[point_i * 3 + coord_i] == &coord);
            }
        }
    }

    SECTION("at_() const") {
        using rtype = decltype(std::as_const(points)[0]);
        using corr  = typename set_type::const_reference;
        STATIC_REQUIRE(std::is_same_v<rtype, corr>);

        for(std::size_t point_i = 0; point_i < 3; ++point_i) {
            const auto corr = point_i == 0 ? p0 : p1;
            for(std::size_t coord_i = 0; coord_i < 3; ++coord_i) {
                auto& coord = std::as_const(points)[point_i].coord(coord_i);

                REQUIRE(coord == corr.coord(coord_i));
                REQUIRE(old_addresses[point_i * 3 + coord_i] == &coord);
            }
        }
    }

    SECTION("size_") {
        REQUIRE(defaulted.size() == 0);
        REQUIRE(points.size() == 3);
    }
}