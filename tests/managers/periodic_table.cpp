/*
 * Copyright 2022 NWChemEx-Project
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

#include "chemist/managers/periodic_table.hpp"
#include <catch2/catch.hpp>
#include <string>

using namespace chemist;

using Catch::Matchers::Message;
void load_elements(PeriodicTable& pt) {
    pt.insert(0, Atom("Ez", 0ul, 0.0, 0.0, 0.0, 0.0));

    pt.insert(1, Atom("H", 1ul, 1837.4260218693814, 0.0, 0.0, 0.0));
    pt.add_isotope(1, 1, Atom("H", 1ul, 1837.1526472934618, 0.0, 0.0, 0.0));
    pt.add_isotope(1, 2, Atom("H", 1ul, 3671.4829413173247, 0.0, 0.0, 0.0));
    pt.add_elec_config(1, {1, 0, 0, 0});

    pt.insert(2, Atom("He", 2ul, 7296.297100609073, 0.0, 0.0, 0.0));
    pt.add_isotope(2, 3, Atom("He", 2ul, 5497.885121445487, 0.0, 0.0, 0.0));
    pt.add_isotope(2, 4, Atom("He", 2ul, 7296.299386693523, 0.0, 0.0, 0.0));
    pt.add_elec_config(2, {2, 0, 0, 0});
}

TEST_CASE("PeriodicTable Copy/Move") {
    PeriodicTable pt;
    load_elements(pt);

    SECTION("Copy Ctor") {
        PeriodicTable pt2(pt);

        REQUIRE(pt == pt2);
    }

    SECTION("Move Ctor") {
        PeriodicTable pt2(std::move(pt));
        PeriodicTable corr;
        std::string msg = "PIMPL is not set";

        load_elements(corr);

        // Compare to copy with same data values
        REQUIRE(pt2 == corr);

        // pt should no longer have a PIMPL, and will throw
        REQUIRE_THROWS_MATCHES(pt == pt2, std::runtime_error, Message(msg));
    }

    SECTION("Copy Assignment") {
        PeriodicTable pt2;

        pt2 = pt;

        REQUIRE(pt == pt2);
    }

    SECTION("Move Assignment") {
        PeriodicTable pt2;
        PeriodicTable corr;
        std::string msg = "PIMPL is not set";

        load_elements(corr);

        pt2 = std::move(pt);

        // Compare to copy with same data values
        REQUIRE(pt2 == corr);

        // pt should no longer have a PIMPL, and will throw
        REQUIRE_THROWS_MATCHES(pt == pt2, std::runtime_error, Message(msg));
    }
}

TEST_CASE("PeriodicTable Comparison") {
    PeriodicTable pt;
    PeriodicTable pt2;

    SECTION("Blank equivalence") {
        std::string msg = "PIMPL is not set";

        // Empty PeriodicTables; same object
        REQUIRE_THROWS_MATCHES(pt == pt, std::runtime_error, Message(msg));
        REQUIRE_THROWS_MATCHES(pt2 == pt2, std::runtime_error, Message(msg));

        // Empty PeriodicTables; different objects
        REQUIRE_THROWS_MATCHES(pt == pt2, std::runtime_error, Message(msg));
        REQUIRE_THROWS_MATCHES(pt2 == pt, std::runtime_error, Message(msg));
    }

    SECTION("Filled and blank") {
        std::string msg = "PIMPL is not set";

        load_elements(pt2);

        REQUIRE_THROWS_MATCHES(pt != pt2, std::runtime_error, Message(msg));
        REQUIRE_THROWS_MATCHES(pt2 != pt, std::runtime_error, Message(msg));
    }

    SECTION("Filled with same data") {
        load_elements(pt);
        load_elements(pt2);

        // Filled PeriodicTables; same object
        REQUIRE(pt == pt);
        REQUIRE(pt2 == pt2);

        // Filled PeriodicTables; different objects
        REQUIRE(pt == pt2);
        REQUIRE(pt2 == pt);
    }

    SECTION("Filled with different elements") {
        load_elements(pt);
        pt2.insert(0, Atom("Ez", 0ul, 0.0, 0.0, 0.0, 0.0));

        REQUIRE(pt != pt2);
        REQUIRE(pt2 != pt);
    }

    SECTION("Filled with same element; different isotopes") {
        pt.insert(1, Atom("H", 1ul, 1837.4260218693814, 0.0, 0.0, 0.0));
        pt.add_isotope(1, 1, Atom("H", 1ul, 1837.1526472934618, 0.0, 0.0, 0.0));

        pt2.insert(1, Atom("H", 1ul, 1837.4260218693814, 0.0, 0.0, 0.0));
        pt2.add_isotope(1, 2,
                        Atom("H", 1ul, 3671.4829413173247, 0.0, 0.0, 0.0));

        REQUIRE(pt != pt2);
        REQUIRE(pt2 != pt);
    }

    SECTION("Filled with different density matrices") {
        load_elements(pt);
        load_elements(pt2);
        pt.add_atom_dm(3, "6-31G", {1.0, 1.0, 1.0, 1.0});

        REQUIRE(pt != pt2);
        REQUIRE(pt2 != pt);
    }

    SECTION("Filled with different configs") {
        load_elements(pt);
        load_elements(pt2);
        pt.add_elec_config(3, {3, 0, 0, 0});

        REQUIRE(pt != pt2);
        REQUIRE(pt2 != pt);
    }
}

TEST_CASE("PeriodicTable::insert") {
    PeriodicTable pt;
    load_elements(pt);

    SECTION("Repeat elements") {
        // Element with no isotopes
        REQUIRE_THROWS_MATCHES(
          pt.insert(0, Atom("Ez", 0ul, 0.0, 0.0, 0.0, 0.0)), std::runtime_error,
          Message("Element already exists with Z = 0"));
        // Element with isotopes
        REQUIRE_THROWS_MATCHES(
          pt.insert(1, Atom("H", 1ul, 1837.4260218693814, 0.0, 0.0, 0.0)),
          std::runtime_error, Message("Element already exists with Z = 1"));
    }
}

TEST_CASE("PeriodicTable::add_isotope") {
    PeriodicTable pt;
    load_elements(pt);

    SECTION("Repeat isotopes") {
        REQUIRE_THROWS_MATCHES(
          pt.add_isotope(1, 1,
                         Atom("H", 1ul, 1837.1526472934618, 0.0, 0.0, 0.0)),
          std::runtime_error, Message("Isotope Z = 1, A = 1 already exists"));
    }

    SECTION("No element") {
        REQUIRE_THROWS_MATCHES(
          pt.add_isotope(3, 6,
                         Atom("Li", 3ul, 10964.898253742283, 0.0, 0.0, 0.0)),
          std::runtime_error, Message("Element does not exist with Z = 3"));
    }
}

TEST_CASE("PeriodicTable::sym_2_Z") {
    PeriodicTable pt;
    load_elements(pt);

    SECTION("No element") {
        REQUIRE_THROWS_MATCHES(pt.sym_2_Z("Li"), std::out_of_range,
                               Message("Unrecognized atomic symbol: Li"));
    }

    SECTION("Correct usage") {
        PeriodicTable::size_type corr = 2;

        REQUIRE(corr == pt.sym_2_Z("He"));

        // Ensure case insensitivity
        REQUIRE(corr == pt.sym_2_Z("HE"));
        REQUIRE(corr == pt.sym_2_Z("he"));
    }
}

TEST_CASE("PeriodicTable::isotopes") {
    PeriodicTable pt;
    load_elements(pt);

    SECTION("No element") {
        REQUIRE_THROWS_MATCHES(pt.isotopes(3), std::out_of_range,
                               Message("Element does not exist with Z = 3"));
    }

    SECTION("No isotopes") {
        auto corr = PeriodicTable::isotope_list();

        REQUIRE(corr == pt.isotopes(0));
    }

    SECTION("Isotopes exist") {
        auto corr = PeriodicTable::isotope_list{1, 2};

        REQUIRE(corr == pt.isotopes(1));
    }
}

TEST_CASE("PeriodicTable::get_atom") {
    PeriodicTable pt;
    load_elements(pt);

    SECTION("No element") {
        REQUIRE_THROWS_MATCHES(pt.get_atom(3), std::out_of_range,
                               Message("Element does not exist with Z = 3"));
    }

    SECTION("Element exists") {
        auto corr = Atom("Ez", 0ul, 0.0, 0.0, 0.0, 0.0);

        REQUIRE(corr == pt.get_atom(0));
    }
}

TEST_CASE("PeriodicTable::get_atom_dm") {
    PeriodicTable pt;
    load_elements(pt);
    pt.add_atom_dm(1, "6-31G",
                   {0.18269721, 0.28443345, 0.28443345,
                    0.44282224}); // add precalculated density matrix with the
                                  // basis set 6-31G for H
    pt.add_atom_dm(2, "6-31G",
                   {0.70112023, 0.60816932, 0.60816932,
                    0.52754136}); // add precalculated density matrix with the
                                  // basis set 6-31G for He

    SECTION("No density matrix 1") {
        REQUIRE_THROWS_MATCHES(
          pt.get_atom_dm(3, "6-31G"), std::out_of_range,
          Message("Density matrix does not exist for Z = 3/6-31G"));
    }

    SECTION("No density matrix 2") {
        REQUIRE_THROWS_MATCHES(
          pt.get_atom_dm(1, "3-21G"), std::out_of_range,
          Message("Density matrix does not exist for Z = 1/3-21G"));
    }

    SECTION("Add existing density matrix") {
        REQUIRE_THROWS_MATCHES(
          pt.add_atom_dm(1, "6-31G", {1.0, 1.0, 1.0, 1.0}), std::runtime_error,
          Message("Atomic density matrix for Z = 1/6-31G already exists"));
    }

    SECTION("Add density matrix with another basis set") {
        REQUIRE_NOTHROW(pt.add_atom_dm(1, "3-21G", {0.5, 0.5, 0.5, 0.5}));
    }

    SECTION("Density matrix exists 1") {
        PeriodicTable::atom_dm_t corr = {0.18269721, 0.28443345, 0.28443345,
                                         0.44282224};

        REQUIRE(corr == pt.get_atom_dm(1, "6-31G"));
        REQUIRE(corr == pt.get_atom_dm("H", "6-31G"));
    }

    SECTION("Density matrix exists 2") {
        PeriodicTable::atom_dm_t corr = {0.70112023, 0.60816932, 0.60816932,
                                         0.52754136};

        REQUIRE(corr == pt.get_atom_dm(2, "6-31G"));
        REQUIRE(corr == pt.get_atom_dm("He", "6-31G"));
    }

    SECTION("Density matrix exists without atom") {
        pt.add_atom_dm(42, "6-31G", {9.0, 18.0, 15.0});
        PeriodicTable::atom_dm_t corr = {9.0, 18.0, 15.0};

        REQUIRE(corr == pt.get_atom_dm(42, "6-31G"));
        REQUIRE_THROWS_MATCHES(corr == pt.get_atom_dm("Mo", "6-31G"),
                               std::out_of_range,
                               Message("Unrecognized atomic symbol: Mo"));
    }
}

TEST_CASE("PeriodicTable::get_elec_conf") {
    PeriodicTable pt;
    load_elements(pt);

    SECTION("No config") {
        PeriodicTable::elec_conf_t corr = {3};
        REQUIRE(corr == pt.get_elec_conf(3));
    }

    SECTION("Add existing config") {
        REQUIRE_THROWS_MATCHES(
          pt.add_elec_config(1, {1, 0, 0, 0}), std::runtime_error,
          Message("Elec. config for Z = 1 already exists"));
    }

    SECTION("Config exists 0") {
        PeriodicTable::elec_conf_t corr = {};
        pt.add_elec_config(0, {});

        REQUIRE(corr == pt.get_elec_conf(0));
        REQUIRE(corr == pt.get_elec_conf("Ez"));
    }

    SECTION("Config exists 1") {
        PeriodicTable::elec_conf_t corr = {1};

        REQUIRE(corr == pt.get_elec_conf(1));
        REQUIRE(corr == pt.get_elec_conf("H"));
    }

    SECTION("Config exists 2") {
        PeriodicTable::elec_conf_t corr = {2};

        REQUIRE(corr == pt.get_elec_conf(2));
        REQUIRE(corr == pt.get_elec_conf("He"));
    }

    SECTION("Config exists without atom") {
        pt.add_elec_config(42, {9, 18, 15});
        PeriodicTable::elec_conf_t corr = {9, 18, 15};

        REQUIRE(corr == pt.get_elec_conf(42));
        REQUIRE_THROWS_MATCHES(corr == pt.get_elec_conf("Mo"),
                               std::out_of_range,
                               Message("Unrecognized atomic symbol: Mo"));
    }
}

TEST_CASE("PeriodicTable::get_elec_conf_full") {
    PeriodicTable pt;
    load_elements(pt);

    SECTION("No config") {
        std::map<std::pair<size_t, size_t>, size_t> corr = {{{1, 0}, 2},  // 1s2
                                                            {{2, 0}, 1}}; // 2s1
        REQUIRE(corr == pt.get_elec_conf_full(3));
    }
    SECTION("Config exists 2 (full)") {
        std::map<std::pair<size_t, size_t>, size_t> corr = {{{1, 0}, 2}}; // 1s2

        REQUIRE(corr == pt.get_elec_conf_full(2));
        REQUIRE(corr == pt.get_elec_conf_full("He"));
    }

    SECTION("Config exists 42 (full)") {
        pt.insert(42,
                  chemist::Atom("Mo", 42ul, 174906.15025012242, 0.0, 0.0, 0.0));
        pt.add_elec_config(42, {9, 18, 15, 0});
        std::map<std::pair<size_t, size_t>, size_t> corr = {
          {{1, 0}, 2},  // 1s2
          {{2, 0}, 2},  // 2s2
          {{2, 1}, 6},  // 2p6
          {{3, 0}, 2},  // 3s2
          {{3, 1}, 6},  // 3p6
          {{3, 2}, 10}, // 3d10
          {{4, 0}, 2},  // 4s2
          {{4, 1}, 6},  // 4p6
          {{4, 2}, 5},  // 4d5
          {{5, 0}, 1},  // 5s1
        };

        REQUIRE(corr == pt.get_elec_conf_full(42));
        REQUIRE(corr == pt.get_elec_conf_full("Mo"));
    }
}

TEST_CASE("PeriodicTable::get_elec_conf_frac") {
    PeriodicTable pt;
    load_elements(pt);

    SECTION("One exists") {
        PeriodicTable::elec_conf_frac_t corr = {2.3};
        REQUIRE(corr == pt.get_elec_conf_frac(2.3));
    }

    SECTION("Both exist") {
        PeriodicTable::elec_conf_frac_t corr = {1.2};
        REQUIRE(corr == pt.get_elec_conf_frac(1.2));
    }

    SECTION("Neither exist") {
        PeriodicTable::elec_conf_frac_t corr = {3.8};
        REQUIRE(corr == pt.get_elec_conf_frac(3.8));
    }

    SECTION("Outside tol") {
        PeriodicTable::elec_conf_frac_t corr = {4.0, 0.001};
        REQUIRE_THAT(pt.get_elec_conf_frac(4.001, 1e-4), Catch::Approx(corr));
    }

    SECTION("Larger tol") {
        PeriodicTable::elec_conf_frac_t corr = {4.0};
        REQUIRE(corr == pt.get_elec_conf_frac(4.001, 1e-2));
    }

    SECTION("Within tol above") {
        PeriodicTable::elec_conf_frac_t corr = {4.0};
        REQUIRE(corr == pt.get_elec_conf_frac(4.0000001));
    }

    SECTION("Within tol below") {
        PeriodicTable::elec_conf_frac_t corr = {4.0};
        REQUIRE(corr == pt.get_elec_conf_frac(3.9999999));
    }

    SECTION("Multiple diff") {
        pt.add_elec_config(24, {8, 12, 4});
        pt.add_elec_config(25, {6, 12, 7});
        PeriodicTable::elec_conf_frac_t corr = {7.8, 12, 4.3};
        REQUIRE_THAT(pt.get_elec_conf_frac(24.1), Catch::Approx(corr));
    }

    SECTION("Length mismatch") {
        pt.add_elec_config(58, {12, 24, 22});
        pt.add_elec_config(59, {12, 24, 21, 2});
        PeriodicTable::elec_conf_frac_t corr = {12.0, 24.0, 21.3, 1.4};
        REQUIRE_THAT(pt.get_elec_conf_frac(58.7), Catch::Approx(corr));
    }
}

TEST_CASE("PeriodicTable::get_isotope") {
    PeriodicTable pt;
    load_elements(pt);

    SECTION("No element") {
        REQUIRE_THROWS_MATCHES(pt.get_isotope(3, 6), std::out_of_range,
                               Message("Element does not exist with Z = 3"));
    }

    SECTION("Element exists, but no isotopes") {
        REQUIRE_THROWS_MATCHES(pt.get_isotope(0, 0), std::out_of_range,
                               Message("No isotope data for Z = 0, A = 0"));
    }

    SECTION("Element exists, but incorrect isotope") {
        REQUIRE_THROWS_MATCHES(pt.get_isotope(1, 10), std::out_of_range,
                               Message("No isotope data for Z = 1, A = 10"));
    }

    SECTION("Element exists, isotope exists") {
        auto corr = Atom("H", 1ul, 1837.1526472934618, 0.0, 0.0, 0.0);

        REQUIRE(corr == pt.get_isotope(1, 1));
    }
}
