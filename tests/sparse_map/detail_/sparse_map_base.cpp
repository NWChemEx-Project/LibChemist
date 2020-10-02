#include "libchemist/sparse_map/sparse_map_class.hpp"
#include <catch2/catch.hpp>

using namespace libchemist::sparse_map;
using namespace libchemist::sparse_map::detail_;

using index_list = std::tuple<
  std::pair<ElementIndex, ElementIndex>, std::pair<ElementIndex, TileIndex>,
  std::pair<TileIndex, ElementIndex>, std::pair<TileIndex, TileIndex>>;

/* General notes on testing:
 *
 * - We know that the Domain class works from unit testing it. We use a variety
 *   of Domains in these unit tests, but do not attempt to be exhaustive for
 *   that reason. For testing, what matters more is the interaction of the
 *   Domain with the SparseMap class.
 *
 */

TEMPLATE_LIST_TEST_CASE("SparseMapBase", "", index_list) {
    using ind_idx_t = std::tuple_element_t<0, TestType>;
    using dep_idx_t = std::tuple_element_t<1, TestType>;
    using derived_t = SparseMap<ind_idx_t, dep_idx_t>;
    using base_t    = SparseMapBase<derived_t, ind_idx_t, dep_idx_t>;
    using domain_t  = Domain<dep_idx_t>;

    ind_idx_t i0{}, i1{1}, i12{2}, i2{1, 2}, i22{2, 3};
    dep_idx_t d0{}, d1{1}, d12{2}, d2{1, 2}, d22{2, 3};

    std::map<std::string, derived_t> sms;
    sms["Empty"];
    sms["Ind == rank 0"] = derived_t{{i0, {d1, d12}}};
    sms["Ind == rank 1"] = derived_t{{i12, {}}, {i1, {d1}}};
    sms["Ind == rank 2"] = derived_t{{i2, {d2}}, {i22, {d22}}};
    derived_t temp(std::move(sms["No PIMPL"]));

    SECTION("CTors") {
        SECTION("Typedefs") {
            using traits = SparseMapTraits<ind_idx_t, dep_idx_t>;

            SECTION("size_type") {
                using corr_t = typename traits::size_type;
                using the_t  = typename base_t::size_type;
                STATIC_REQUIRE(std::is_same_v<corr_t, the_t>);
            }

            SECTION("key_type") {
                using corr_t = typename traits::key_type;
                using the_t  = typename base_t::key_type;
                STATIC_REQUIRE(std::is_same_v<corr_t, the_t>);
            }

            SECTION("mapped_type") {
                using corr_t = typename traits::mapped_type;
                using the_t  = typename base_t::mapped_type;
                STATIC_REQUIRE(std::is_same_v<corr_t, the_t>);
            }

            SECTION("iterator") {
                using corr_t = typename traits::iterator;
                using the_t  = typename base_t::iterator;
                STATIC_REQUIRE(std::is_same_v<corr_t, the_t>);
            }

            SECTION("const_iterator") {
                using corr_t = typename traits::const_iterator;
                using the_t  = typename base_t::const_iterator;
                STATIC_REQUIRE(std::is_same_v<corr_t, the_t>);
            }
        }

        SECTION("Default Ctor") {
            auto& sm = sms.at("Empty");
            REQUIRE(sm.size() == 0);
            REQUIRE(sm.empty());
            REQUIRE(sm.ind_rank() == 0);
            REQUIRE(sm.dep_rank() == 0);
        }

        SECTION("Initializer list") {
            SECTION("Empty") {
                base_t sm_empty({});
                REQUIRE(sm_empty == sms.at("Empty"));
            }

            SECTION("Ind == rank 0") {
                auto& sm0 = sms.at("Ind == rank 0");
                REQUIRE(sm0.size() == 1);
                REQUIRE_FALSE(sm0.empty());
                REQUIRE(sm0.ind_rank() == 0);
                REQUIRE(sm0.dep_rank() == 1);
            }

            SECTION("Ind == rank 1") {
                auto& sm1 = sms.at("Ind == rank 1");
                REQUIRE(sm1.size() == 2);
                REQUIRE_FALSE(sm1.empty());
                REQUIRE(sm1.ind_rank() == 1);
                REQUIRE(sm1.dep_rank() == 1);
            }

            SECTION("Ind == rank 2") {
                auto& sm2 = sms.at("Ind == rank 2");
                REQUIRE(sm2.size() == 2);
                REQUIRE_FALSE(sm2.empty());
                REQUIRE(sm2.ind_rank() == 2);
                REQUIRE(sm2.dep_rank() == 2);
            }
        }

        SECTION("Copy ctor") {
            for(auto [k, v] : sms) {
                SECTION(k) {
                    base_t copy(v);
                    REQUIRE(copy == v);
                }
            }
        }

        SECTION("Move ctor") {
            for(auto [k, v] : sms) {
                SECTION(k) {
                    base_t corr(v);
                    base_t moved2(std::move(v));
                    REQUIRE(moved2 == corr);
                }
            }
        }

        SECTION("Copy assignment") {
            for(auto [k, v] : sms) {
                SECTION(k) {
                    base_t copy;
                    auto pcopy = &(copy = v);
                    SECTION("Value") { REQUIRE(copy == v); }
                    SECTION("Returns *this") { REQUIRE(pcopy == &copy); }
                }
            }
        }

        SECTION("Move ctor") {
            for(auto [k, v] : sms) {
                SECTION(k) {
                    base_t corr(v);
                    base_t moved2;
                    auto pmoved = &(moved2 = std::move(v));
                    SECTION("Value") { REQUIRE(moved2 == corr); }
                    SECTION("Returns *this") { REQUIRE(pmoved == &moved2); }
                }
            }
        }

    } // SECTION("CTORS")

    SECTION("swap") {
        for(auto& [lhs_k, lhs_v] : sms) {
            for(auto& [rhs_k, rhs_v] : sms) {
                SECTION(lhs_k + " swapped with " + rhs_k) {
                    base_t corr_lhs(rhs_v);
                    base_t corr_rhs(lhs_v);
                    lhs_v.swap(rhs_v);
                    REQUIRE(lhs_v == corr_lhs);
                    REQUIRE(rhs_v == corr_rhs);
                }
            }
        }
    }

    SECTION("size") {
        SECTION("Empty") { REQUIRE(sms.at("Empty").size() == 0); }

        SECTION("Ind == rank 0") {
            auto& sm0 = sms.at("Ind == rank 0");
            REQUIRE(sm0.size() == 1);
        }

        SECTION("Ind == rank 1") {
            auto& sm1 = sms.at("Ind == rank 1");
            REQUIRE(sm1.size() == 2);
        }

        SECTION("Ind == rank 2") {
            auto& sm2 = sms.at("Ind == rank 2");
            REQUIRE(sm2.size() == 2);
        }

        SECTION("No PIMPL") {
            auto& mf = sms.at("No PIMPL");
            REQUIRE(mf.size() == 0);
        }
    }

    SECTION("empty") {
        for(auto [k, v] : sms) {
            SECTION(k) {
                bool is_empty = (v.size() == 0);
                REQUIRE(v.empty() == is_empty);
            }
        }
    }

    SECTION("count") {
        SECTION("Empty") { REQUIRE_FALSE(sms.at("Empty").count(i0)); }

        SECTION("Ind == rank 0") {
            auto& sm0 = sms.at("Ind == rank 0");
            SECTION("Has index") { REQUIRE(sm0.count(i0)); }
            SECTION("Doesn't have") { REQUIRE_FALSE(sm0.count(i1)); }
        }

        SECTION("Ind == rank 1") {
            auto& sm1 = sms.at("Ind == rank 1");
            SECTION("Has index") { REQUIRE(sm1.count(i1)); }
            SECTION("Doesn't have") { REQUIRE_FALSE(sm1.count(i0)); }
        }

        SECTION("Ind == rank 2") {
            auto& sm2 = sms.at("Ind == rank 2");
            SECTION("Has index") { REQUIRE(sm2.count(i2)); }
            SECTION("Doesn't have") { REQUIRE_FALSE(sm2.count(i0)); }
        }

        SECTION("No PIMPL") { REQUIRE_FALSE(sms.at("No PIMPL").count(i0)); }
    }

    SECTION("ind_rank") {
        SECTION("Empty") { REQUIRE(sms.at("Empty").ind_rank() == 0); }

        SECTION("Ind == rank 0") {
            auto& sm0 = sms.at("Ind == rank 0");
            REQUIRE(sm0.ind_rank() == 0);
        }

        SECTION("Ind == rank 1") {
            auto& sm1 = sms.at("Ind == rank 1");
            REQUIRE(sm1.ind_rank() == 1);
        }

        SECTION("Ind == rank 2") {
            auto& sm2 = sms.at("Ind == rank 2");
            REQUIRE(sm2.ind_rank() == 2);
        }

        SECTION("No PIMPL") {
            auto& mf = sms.at("No PIMPL");
            REQUIRE(mf.ind_rank() == 0);
        }
    }

    SECTION("dep_rank") {
        SECTION("Empty") { REQUIRE(sms.at("Empty").dep_rank() == 0); }

        SECTION("Ind == rank 0") {
            auto& sm0 = sms.at("Ind == rank 0");
            REQUIRE(sm0.dep_rank() == 1);
        }

        SECTION("Ind == rank 1") {
            auto& sm1 = sms.at("Ind == rank 1");
            REQUIRE(sm1.dep_rank() == 1);
        }

        SECTION("Ind == rank 2") {
            auto& sm2 = sms.at("Ind == rank 2");
            REQUIRE(sm2.dep_rank() == 2);
        }

        SECTION("No PIMPL") {
            auto& mf = sms.at("No PIMPL");
            REQUIRE(mf.dep_rank() == 0);
        }
    }

    SECTION("add_to_domain") {
        SECTION("Empty") {
            auto& sm = sms.at("Empty");
            sm.add_to_domain(i0, d0);
            base_t corr{{i0, {d0}}};
            REQUIRE(sm == corr);
        }

        SECTION("Ind == rank 0") {
            auto& sm0 = sms.at("Ind == rank 0");
            SECTION("Throws if independent rank is wrong") {
                REQUIRE_THROWS_AS(sm0.add_to_domain(i1, d1),
                                  std::runtime_error);
            }
            SECTION("Throws if dependent rank is wrong") {
                REQUIRE_THROWS_AS(sm0.add_to_domain(i0, d0),
                                  std::runtime_error);
            }
            SECTION("Add to existing independent index") {
                sm0.add_to_domain(i0, dep_idx_t{3});
                base_t corr{{i0, {d1, d12, dep_idx_t{3}}}};
                REQUIRE(sm0 == corr);
            }
        }

        SECTION("Ind == rank 1") {
            auto& sm1 = sms.at("Ind == rank 1");
            SECTION("Throws if independent rank is wrong") {
                REQUIRE_THROWS_AS(sm1.add_to_domain(i0, d1),
                                  std::runtime_error);
            }
            SECTION("Throws if dependent rank is wrong") {
                REQUIRE_THROWS_AS(sm1.add_to_domain(i1, d0),
                                  std::runtime_error);
            }
            SECTION("Add to existing independent index") {
                sm1.add_to_domain(i12, d12);
                base_t corr{{i1, {d1}}, {i12, {d12}}};
                REQUIRE(sm1 == corr);
            }
            SECTION("Add to non-existing independent index") {
                sm1.add_to_domain(ind_idx_t{4}, d12);
                base_t corr{{i12, {}}, {i1, {d1}}, {ind_idx_t{4}, {d12}}};
                REQUIRE(sm1 == corr);
            }
        }

        SECTION("Ind == rank 2") {
            auto& sm2 = sms.at("Ind == rank 2");
            SECTION("Throws if independent rank is wrong") {
                REQUIRE_THROWS_AS(sm2.add_to_domain(i0, d2),
                                  std::runtime_error);
            }
            SECTION("Throws if dependent rank is wrong") {
                REQUIRE_THROWS_AS(sm2.add_to_domain(i2, d0),
                                  std::runtime_error);
            }
            SECTION("Add to existing independent index") {
                sm2.add_to_domain(i2, dep_idx_t{3, 4});
                base_t corr{{i2, {d2, dep_idx_t{3, 4}}}, {i22, {d22}}};
                REQUIRE(sm2 == corr);
            }
            SECTION("Add to non-existing independent index") {
                sm2.add_to_domain(ind_idx_t{3, 4}, d2);
                base_t corr{{i2, {d2}}, {i22, {d22}}, {ind_idx_t{3, 4}, {d2}}};
                REQUIRE(sm2 == corr);
            }
        }

        SECTION("No PIMPL") {
            auto& mf = sms.at("No PIMPL");
            mf.add_to_domain(i0, d0);
            REQUIRE(mf == base_t{{i0, {d0}}});
        }
    }

    /* To ensure that the returned instance is a read/write reference we grab
     * the address from one call to operator[] and then call it again. If the
     * returned object is not by reference the addresses should be different.
     */
    SECTION("operator[]") {
        SECTION("Empty") {
            auto& sm     = sms.at("Empty");
            auto pdomain = &(sm[i0]);
            SECTION("Returns read/write") { REQUIRE(pdomain == &(sm[i0])); }
            SECTION("Value") { REQUIRE(sm[i0] == domain_t{}); }
        }

        SECTION("Ind == rank 0") {
            auto& sm     = sms.at("Ind == rank 0");
            auto pdomain = &(sm[i0]);
            SECTION("Throws if wrong ind rank") {
                REQUIRE_THROWS_AS(sm[i1], std::runtime_error);
            }
            SECTION("Returns read/write") { REQUIRE(pdomain == &(sm[i0])); }
            SECTION("Value") { REQUIRE(sm[i0] == domain_t{d1, d12}); }
        }

        SECTION("Ind == rank 1") {
            auto& sm     = sms.at("Ind == rank 1");
            auto pdomain = &(sm[i1]);
            SECTION("Throws if wrong ind rank") {
                REQUIRE_THROWS_AS(sm[i0], std::runtime_error);
            }
            SECTION("Returns read/write") { REQUIRE(pdomain == &(sm[i1])); }
            SECTION("Value") { REQUIRE(sm[i1] == domain_t{d1}); }
        }

        SECTION("Ind == rank 2") {
            auto& sm     = sms.at("Ind == rank 2");
            auto pdomain = &(sm[i2]);
            SECTION("Throws if wrong ind rank") {
                REQUIRE_THROWS_AS(sm[i1], std::runtime_error);
            }
            SECTION("Returns read/write") { REQUIRE(pdomain == &(sm[i2])); }
            SECTION("Value") { REQUIRE(sm[i2] == domain_t{d2}); }
        }

        SECTION("No PIMPL") {
            auto& sm     = sms.at("No PIMPL");
            auto pdomain = &(sm[i0]);
            SECTION("Returns read/write") { REQUIRE(pdomain == &(sm[i0])); }
            SECTION("Value") { REQUIRE(sm[i0] == domain_t{}); }
        }
    }

    SECTION("operator[] const") {
        SECTION("Empty") {
            const auto& sm = sms.at("Empty");
            REQUIRE_THROWS_AS(sm[i0], std::out_of_range);
        }

        SECTION("Ind == rank 0") {
            const auto& sm = sms.at("Ind == rank 0");
            SECTION("Throws if wrong ind rank") {
                REQUIRE_THROWS_AS(sm[i1], std::runtime_error);
            }
            SECTION("Value") { REQUIRE(sm[i0] == domain_t{d1, d12}); }
        }

        SECTION("Ind == rank 1") {
            const auto& sm = sms.at("Ind == rank 1");
            SECTION("Throws if wrong ind rank") {
                REQUIRE_THROWS_AS(sm[i0], std::runtime_error);
            }
            SECTION("Throws if value is not present") {
                REQUIRE_THROWS_AS(sm[ind_idx_t{4}], std::out_of_range);
            }
            SECTION("Value") { REQUIRE(sm[i1] == domain_t{d1}); }
        }

        SECTION("Ind == rank 2") {
            const auto& sm = sms.at("Ind == rank 2");
            SECTION("Throws if wrong ind rank") {
                REQUIRE_THROWS_AS(sm[i1], std::runtime_error);
            }
            SECTION("Throws if value is not present") {
                ind_idx_t i23{3, 4};
                REQUIRE_THROWS_AS(sm[i23], std::out_of_range);
            }
            SECTION("Value") { REQUIRE(sm[i2] == domain_t{d2}); }
        }

        SECTION("No PIMPL") {
            const auto& sm = sms.at("No PIMPL");
            REQUIRE_THROWS_AS(sm[i0], std::out_of_range);
        }
    } // SECTION("operator[]const")

    SECTION("at() const") {
        SECTION("Empty") {
            const auto& sm = sms.at("Empty");
            REQUIRE_THROWS_AS(sm.at(i0), std::out_of_range);
        }

        SECTION("Ind == rank 0") {
            const auto& sm = sms.at("Ind == rank 0");
            SECTION("Throws if wrong ind rank") {
                REQUIRE_THROWS_AS(sm.at(i1), std::runtime_error);
            }
            SECTION("Value") { REQUIRE(sm.at(i0) == domain_t{d1, d12}); }
        }

        SECTION("Ind == rank 1") {
            const auto& sm = sms.at("Ind == rank 1");
            SECTION("Throws if wrong ind rank") {
                REQUIRE_THROWS_AS(sm.at(i0), std::runtime_error);
            }
            SECTION("Throws if value is not present") {
                REQUIRE_THROWS_AS(sm.at(ind_idx_t{4}), std::out_of_range);
            }
            SECTION("Value") { REQUIRE(sm.at(i1) == domain_t{d1}); }
        }

        SECTION("Ind == rank 2") {
            const auto& sm = sms.at("Ind == rank 2");
            SECTION("Throws if wrong ind rank") {
                REQUIRE_THROWS_AS(sm.at(i1), std::runtime_error);
            }
            SECTION("Throws if value is not present") {
                ind_idx_t i23{3, 4};
                REQUIRE_THROWS_AS(sm.at(i23), std::out_of_range);
            }
            SECTION("Value") { REQUIRE(sm.at(i2) == domain_t{d2}); }
        }

        SECTION("No PIMPL") {
            const auto& sm = sms.at("No PIMPL");
            REQUIRE_THROWS_AS(sm.at(i0), std::out_of_range);
        }
    } // SECTION("operator[]const")

    /* With respect to direct product operator*= does all the work and
     * operator* simply calls operator*= on a copy. Therefore we test
     * operator*= in depth and simply make sure operator* works for one
     * scenario.
     */
    SECTION("operator*=") {
        SECTION("LHS == Empty") {
            auto& lhs = sms.at("Empty");
            derived_t corr(lhs);

            for(auto [key, rhs] : sms) {
                SECTION("RHS == " + key) {
                    auto plhs = &(lhs *= rhs);
                    SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                    SECTION("Value") { REQUIRE(lhs == corr); }
                }
            }
        }

        SECTION("LHS == rank 0") {
            auto& lhs = sms.at("Ind == rank 0");

            SECTION("RHS == Empty") {
                auto& rhs = sms.at("Empty");
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == rhs); }
            }

            SECTION("RHS == Ind == rank 0") {
                auto& rhs = sms.at("Ind == rank 0");
                derived_t corr{{i0,
                                {dep_idx_t{1, 1}, dep_idx_t{1, 2},
                                 dep_idx_t{2, 1}, dep_idx_t{2, 2}}}};
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == corr); }
            }

            SECTION("RHS == Ind == rank 1") {
                auto& rhs = sms.at("Ind == rank 1");
                derived_t corr{{i1, {dep_idx_t{1, 1}, dep_idx_t{2, 1}}},
                               {i12, {}}};
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == corr); }
            }

            SECTION("RHS == Ind == rank 2") {
                auto& rhs = sms.at("Ind == rank 2");
                derived_t corr{{i2, {dep_idx_t{1, 1, 2}, dep_idx_t{2, 1, 2}}},
                               {i22, {dep_idx_t{1, 2, 3}, dep_idx_t{2, 2, 3}}}};
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == corr); }
            }

            SECTION("RHS == No PIMPL") {
                auto& rhs = sms.at("No PIMPL");
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == rhs); }
            }
        }

        SECTION("LHS == rank 1") {
            auto& lhs = sms.at("Ind == rank 1");

            SECTION("RHS == empty") {
                auto& rhs = sms.at("Empty");
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == rhs); }
            }

            SECTION("RHS == Ind == rank 0") {
                auto& rhs = sms.at("Ind == rank 0");
                derived_t corr{{i1, {dep_idx_t{1, 1}, dep_idx_t{1, 2}}},
                               {i12, {}}};
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == corr); }
            }

            SECTION("RHS == Ind == rank 1") {
                auto& rhs = sms.at("Ind == rank 1");
                derived_t corr{{ind_idx_t{1, 1}, {dep_idx_t{1, 1}}},
                               {i2, {}},
                               {ind_idx_t{2, 1}, {}},
                               {ind_idx_t{2, 2}, {}}};
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == corr); }
            }

            SECTION("RHS == Ind == rank 2") {
                auto& rhs = sms.at("Ind == rank 2");
                derived_t corr{{ind_idx_t{1, 1, 2}, {dep_idx_t{1, 1, 2}}},
                               {ind_idx_t{1, 2, 3}, {dep_idx_t{1, 2, 3}}},
                               {ind_idx_t{2, 1, 2}, {}},
                               {ind_idx_t{2, 2, 3}, {}}};
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == corr); }
            }

            SECTION("RHS == No PIMPL") {
                auto& rhs = sms.at("No PIMPL");
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == rhs); }
            }
        }

        SECTION("LHS == rank 2") {
            auto& lhs = sms.at("Ind == rank 2");

            SECTION("RHS == empty") {
                auto& rhs = sms.at("Empty");
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == rhs); }
            }

            SECTION("RHS == Ind == rank 0") {
                auto& rhs = sms.at("Ind == rank 0");
                derived_t corr{{i2, {dep_idx_t{1, 2, 1}, dep_idx_t{1, 2, 2}}},
                               {i22, {dep_idx_t{2, 3, 1}, dep_idx_t{2, 3, 2}}}};
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == corr); }
            }

            SECTION("RHS == Ind == rank 1") {
                auto& rhs = sms.at("Ind == rank 1");
                derived_t corr{{ind_idx_t{1, 2, 1}, {dep_idx_t{1, 2, 1}}},
                               {ind_idx_t{1, 2, 2}, {}},
                               {ind_idx_t{2, 3, 1}, {dep_idx_t{2, 3, 1}}},
                               {ind_idx_t{2, 3, 2}, {}}};
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == corr); }
            }

            SECTION("RHS == Ind == rank 2") {
                auto& rhs = sms.at("Ind == rank 2");
                derived_t corr{
                  {ind_idx_t{1, 2, 1, 2}, {dep_idx_t{1, 2, 1, 2}}},
                  {ind_idx_t{1, 2, 2, 3}, {dep_idx_t{1, 2, 2, 3}}},
                  {ind_idx_t{2, 3, 1, 2}, {dep_idx_t{2, 3, 1, 2}}},
                  {ind_idx_t{2, 3, 2, 3}, {dep_idx_t{2, 3, 2, 3}}}};
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == corr); }
            }

            SECTION("RHS == No PIMPL") {
                auto& rhs = sms.at("No PIMPL");
                auto plhs = &(lhs *= rhs);
                SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                SECTION("Value") { REQUIRE(lhs == rhs); }
            }
        }

        SECTION("LHS == No PIMPL") {
            auto& lhs = sms.at("No PIMPL");
            derived_t corr(lhs);

            for(auto [key, rhs] : sms) {
                SECTION("RHS == " + key) {
                    auto plhs = &(lhs *= rhs);
                    SECTION("Returns *this") { REQUIRE(plhs == &lhs); }
                    SECTION("Value") { REQUIRE(lhs == corr); }
                }
            }
        }
    }

    SECTION("operator*") {
        auto& lhs = sms.at("Ind == rank 2");
        auto& rhs = sms.at("Ind == rank 1");
        derived_t corr{{ind_idx_t{1, 2, 1}, {dep_idx_t{1, 2, 1}}},
                       {ind_idx_t{1, 2, 2}, {}},
                       {ind_idx_t{2, 3, 1}, {dep_idx_t{2, 3, 1}}},
                       {ind_idx_t{2, 3, 2}, {}}};
        auto r = lhs * rhs;
        REQUIRE(r == corr);
    }

    /* With respect to union operator+= does all the work and operator+
     * simply calls operator+= on a copy. Therefore we test operator+= in depth
     * and make sure operator+ works for one scenario.
     */
    SECTION("operator+=") {
        SECTION("Empty / Empty") {
            derived_t sm, sm2;
            auto psm = &(sm += sm2);
            SECTION("Value") { REQUIRE(sm == sm2); }
            SECTION("Returns *this") { REQUIRE(psm == &sm); }
        }

        SECTION("Empty / Non-empty") {
            derived_t sm;
            derived_t sm2{{ind_idx_t{1}, {dep_idx_t{0}, dep_idx_t{3}}},
                          {ind_idx_t{2}, {dep_idx_t{1}, dep_idx_t{2}}}};
            SECTION("sm += sm2") {
                auto psm = &(sm += sm2);
                REQUIRE(sm == sm2);
                SECTION("Returns *this") { REQUIRE(psm == &sm); }
            }

            SECTION("sm += sm2") {
                auto psm = &(sm += sm2);
                REQUIRE(sm == sm2);
                SECTION("Returns *this") { REQUIRE(psm == &sm); }
            }
        }

        SECTION("Non-empty / Non-empty") {
            derived_t sm{{ind_idx_t{1}, {dep_idx_t{0}, dep_idx_t{3}}},
                         {ind_idx_t{2}, {dep_idx_t{1}, dep_idx_t{2}}}};

            SECTION("Compatible") {
                derived_t sm2{{ind_idx_t{0}, {dep_idx_t{0}, dep_idx_t{3}}},
                              {ind_idx_t{1}, {dep_idx_t{1}, dep_idx_t{2}}},
                              {ind_idx_t{2}, {dep_idx_t{1}, dep_idx_t{2}}},
                              {ind_idx_t{3}, {dep_idx_t{1}, dep_idx_t{2}}}};
                derived_t corr{
                  {ind_idx_t{0}, {dep_idx_t{0}, dep_idx_t{3}}},
                  {ind_idx_t{1},
                   {dep_idx_t{0}, dep_idx_t{1}, dep_idx_t{2}, dep_idx_t{3}}},
                  {ind_idx_t{2}, {dep_idx_t{1}, dep_idx_t{2}}},
                  {ind_idx_t{3}, {dep_idx_t{1}, dep_idx_t{2}}}};
                SECTION("sm += sm2") {
                    auto psm = &(sm += sm2);
                    SECTION("Value") { REQUIRE(sm == corr); }
                    SECTION("Returns *this") { REQUIRE(psm == &sm); }
                }
                SECTION("sm += sm2") {
                    auto psm2 = &(sm2 += sm);
                    SECTION("Value") { REQUIRE(sm2 == corr); }
                    SECTION("Returns *this") { REQUIRE(psm2 == &sm2); }
                }
                SECTION("sm += corr") {
                    auto psm = &(sm += corr);
                    SECTION("Value") { REQUIRE(sm == corr); }
                    SECTION("Returns *this") { REQUIRE(psm == &sm); }
                }
            }

            SECTION("Incompatible independent indices") {
                derived_t incompatible{
                  {ind_idx_t{1, 2}, {dep_idx_t{0}, dep_idx_t{3}}},
                  {ind_idx_t{2, 3}, {dep_idx_t{1}, dep_idx_t{2}}}};
                REQUIRE_THROWS_AS(sm += incompatible,std::runtime_error);
            }

            SECTION("Incompatible dependent indices") {
                derived_t incompatible{
                  {ind_idx_t{1}, {dep_idx_t{0, 1}, dep_idx_t{3, 4}}},
                  {ind_idx_t{2}, {dep_idx_t{1, 2}, dep_idx_t{2, 3}}}};
                REQUIRE_THROWS_AS(sm += incompatible, std::runtime_error);
            }
        }
    }

    SECTION("operator+"){
        auto& lhs = sms.at("Empty");
        auto& rhs = sms.at("Ind == rank 0");
        auto r = lhs + rhs;
        REQUIRE(r == rhs);
    }

    SECTION("comparisons") {
        SECTION("Empty == Empty") {
            REQUIRE(sms.at("Empty") == derived_t{});
            REQUIRE_FALSE(sms.at("Empty") != derived_t{});
        }

        SECTION("Empty == No PIMPL") {
            REQUIRE(sms.at("Empty") == sms.at("No PIMPL"));
            REQUIRE_FALSE(sms.at("Empty") != sms.at("No PIMPL"));
        }

        SECTION("Empty != non-empty") {
            auto& lhs = sms.at("Empty");
            for(std::size_t i = 0; i < 3; ++i) {
                std::string key = "Ind == rank " + std::to_string(i);
                auto& rhs       = sms.at(key);
                SECTION(key) {
                    REQUIRE_FALSE(lhs == rhs);
                    REQUIRE(lhs != rhs);
                }
            }
        }

        SECTION("Same non-empty") {
            auto& lhs = sms.at("Ind == rank 0");
            derived_t copy(lhs);
            REQUIRE(lhs == copy);
            REQUIRE_FALSE(lhs != copy);
        }

        SECTION("Domain is subset/superset") {
            auto& lhs = sms.at("Ind == rank 0");
            derived_t copy(lhs);
            copy.add_to_domain(i0, dep_idx_t{3});
            REQUIRE_FALSE(lhs == copy);
            REQUIRE(lhs != copy);
        }

        SECTION("Different independent indices") {
            auto& lhs = sms.at("Ind == rank 1");
            derived_t copy(lhs);
            copy.add_to_domain(ind_idx_t{3}, dep_idx_t{3});
            REQUIRE_FALSE(lhs == copy);
            REQUIRE(lhs != copy);
        }

        SECTION("No PIMPL != non-empty") {
            auto& lhs = sms.at("No PIMPL");
            for(std::size_t i = 0; i < 3; ++i) {
                std::string key = "Ind == rank " + std::to_string(i);
                auto& rhs       = sms.at(key);
                SECTION(key) {
                    REQUIRE_FALSE(lhs == rhs);
                    REQUIRE(lhs != rhs);
                }
            }
        }
    }

    SECTION("print") {
        std::stringstream ss;

        SECTION("empty") {
            auto pss = &(sms.at("Empty").print(ss));
            SECTION("Value") { REQUIRE(ss.str() == "{}"); }
            SECTION("Returns ostream") { REQUIRE(pss == &ss); }
        }

        SECTION("Non-empty") {
            auto pss = &(sms.at("Ind == rank 0").print(ss));
            SECTION("Value") {
                std::string corr = "{({} : {{1}, {2}})}";
                REQUIRE(ss.str() == corr);
            }
            SECTION("Returns ostream") { REQUIRE(pss == &ss); }
        }
    }

    SECTION("hash") {
        SECTION("Empty == Empty") {
            auto h  = sde::hash_objects(sms.at("Empty"));
            auto h2 = sde::hash_objects(derived_t{});
            REQUIRE(h == h2);
        }

        SECTION("Empty == No PIMPL") {
            auto h  = sde::hash_objects(sms.at("Empty"));
            auto h2 = sde::hash_objects(sms.at("No PIMPL"));
            REQUIRE(h == h2);
        }

        SECTION("Empty != non-empty") {
            auto h = sde::hash_objects(sms.at("Empty"));
            for(std::size_t i = 0; i < 3; ++i) {
                std::string key = "Ind == rank " + std::to_string(i);
                auto& rhs       = sms.at(key);
                SECTION(key) {
                    auto h2 = sde::hash_objects(rhs);
                    REQUIRE(h != h2);
                }
            }
        }

        SECTION("Same non-empty") {
            auto& lhs = sms.at("Ind == rank 0");
            auto h    = sde::hash_objects(lhs);
            auto h2   = sde::hash_objects(derived_t(lhs));
            REQUIRE(h == h2);
        }

        SECTION("Domain is subset/superset") {
            auto& lhs = sms.at("Ind == rank 0");
            auto h    = sde::hash_objects(lhs);
            derived_t copy(lhs);
            copy.add_to_domain(i0, dep_idx_t{3});
            auto h2 = sde::hash_objects(copy);
            REQUIRE(h != h2);
        }

        SECTION("Different independent indices") {
            auto& lhs = sms.at("Ind == rank 1");
            auto h    = sde::hash_objects(lhs);
            derived_t copy(lhs);
            copy.add_to_domain(ind_idx_t{3}, dep_idx_t{3});
            auto h2 = sde::hash_objects(copy);
            REQUIRE(h != h2);
        }

        SECTION("No PIMPL != non-empty") {
            auto& lhs = sms.at("No PIMPL");
            auto h    = sde::hash_objects(lhs);
            for(std::size_t i = 0; i < 3; ++i) {
                std::string key = "Ind == rank " + std::to_string(i);
                auto& rhs       = sms.at(key);
                auto h2         = sde::hash_objects(rhs);
                SECTION(key) { REQUIRE(h != h2); }
            }
        }
    }
}

/* operator<< just calls SparseMap::print. So as long as that works, this will
 *  work too. We just test an empty SparseMap to make sure it gets forwarded
 *  correctly and the ostream is returend.
 */
TEST_CASE("operator<<(std::ostream, SparseMapBase)") {
    std::stringstream ss;
    SparseMap<TileIndex, TileIndex> sm;
    auto pss = &(ss << sm);
    REQUIRE(pss == &ss);
    REQUIRE(ss.str() == "{}");
}

// TEST_CASE("SparseMap : indices()"){
//    SECTION("vector") {
//        SparseMap sm{{index_type{1}, {index_type{0}, index_type{3}}},
//                     {index_type{2}, {index_type{1}, index_type{2}}}};
//        auto result = sm.indices();
//        index_set_map corr{
//          {index_type{1}, index_set_array{index_set{0, 3}}},
//          {index_type{2}, index_set_array{index_set{1, 2}}}
//        };
//        REQUIRE(result == corr);
//    }
//
//    SECTION("matrix") {
//        SparseMap sm{{index_type{1}, {index_type{0, 1}, index_type{3, 2}}},
//                     {index_type{2}, {index_type{2, 0}, index_type{3, 1}}}};
//        auto result = sm.indices();
//        index_set_map corr{
//          {index_type{1}, index_set_array{index_set{0, 3}, index_set{1, 2}}},
//          {index_type{2}, index_set_array{index_set{2, 3}, index_set{0, 1}}}
//        };
//        REQUIRE(result == corr);
//    }
//}
//
// TEST_CASE("SparseMap : indices(key_type)"){
//    SECTION("vector") {
//        SparseMap sm{{index_type{1}, {index_type{0}, index_type{3}}}};
//        auto result = sm.indices(index_type{1});
//        index_set_array corr{index_set{0, 3}};
//        REQUIRE(result == corr);
//    }
//
//    SECTION("matrix") {
//        SparseMap sm{{index_type{1}, {index_type{0, 1}, index_type{3, 2}}}};
//        auto result = sm.indices(index_type{1});
//        index_set_array corr{index_set{0, 3}, index_set{1, 2}};
//        REQUIRE(result == corr);
//    }
//}
//
// TEST_CASE("SparseMap : indices(key_type, size_type)"){
//    SECTION("vector") {
//        SparseMap sm{{index_type{1}, {index_type{0}, index_type{3}}}};
//        auto result = sm.indices(index_type{1}, 0);
//        index_set corr{0, 3};
//        REQUIRE(result == corr);
//    }
//
//    SECTION("matrix") {
//        SparseMap sm{{index_type{1}, {index_type{0, 1}, index_type{3, 2}}}};
//
//        SECTION("mode 0"){
//            auto result = sm.indices(index_type{1}, 0);
//            index_set corr{0, 3};
//            REQUIRE(result == corr);
//        }
//
//        SECTION("mode 1"){
//            auto result = sm.indices(index_type{1}, 1);
//            index_set corr{1, 2};
//            REQUIRE(result == corr);
//        }
//    }
//}
//

//}
//
// TEST_CASE("SparseMap : inverse"){
//    SECTION("Empty"){
//        SparseMap sm;
//        REQUIRE(sm.inverse() == sm);
//    }
//
//    SECTION("Non-empty") {
//        SparseMap sm{{index_type{1}, {index_type{0}, index_type{3}}},
//                     {index_type{2}, {index_type{1}, index_type{2}}}};
//        SparseMap corr;
//        corr.add_to_domain(index_type{0},index_type{1});
//        corr.add_to_domain(index_type{3},index_type{1});
//        corr.add_to_domain(index_type{1},index_type{2});
//        corr.add_to_domain(index_type{2},index_type{2});
//        REQUIRE(sm.inverse() == corr);
//        REQUIRE(sm.inverse().inverse() == sm);
//    }
//}
//
// TEST_CASE("SparseMap : chain"){
//    SECTION("Empty / Empty") {
//        SparseMap sm;
//        REQUIRE(sm.chain(sm) == sm);
//    }
//
//    SECTION("Empty / Non-empty") {
//        SparseMap sm;
//        SparseMap sm2{{index_type{1}, {index_type{0}, index_type{3}}},
//                      {index_type{2}, {index_type{1}, index_type{2}}}};
//        REQUIRE_THROWS_AS(sm.chain(sm2), std::runtime_error);
//        REQUIRE_THROWS_AS(sm2.chain(sm), std::runtime_error);
//    }
//
//    SECTION("Non-empty / Non-empty"){
//        SparseMap sm{{index_type{1}, {index_type{0}, index_type{3}}},
//                      {index_type{2}, {index_type{1}, index_type{2}}}};
//        SparseMap sm2{{index_type{0}, {index_type{0}, index_type{3}}},
//                      {index_type{1}, {index_type{1}, index_type{2}}},
//                      {index_type{2}, {index_type{1}, index_type{2}}},
//                      {index_type{3}, {index_type{1}, index_type{2}}}};
//        SparseMap incompatible{{index_type{1,2}, {index_type{0},
//        index_type{3}}},
//                               {index_type{2,3}, {index_type{1},
//                               index_type{2}}}};
//        SparseMap corr{{index_type{1}, {index_type{0}, index_type{1},
//        index_type{2}, index_type{3}}},
//                       {index_type{2}, {index_type{1}, index_type{2}}}};
//        REQUIRE(sm.chain(sm2) == corr);
//        REQUIRE_THROWS_AS(sm.chain(incompatible), std::runtime_error);
//    }
//}
//

//
// TEST_CASE("SparseMap : intersection"){
//    SECTION("Empty / Empty") {
//        SparseMap sm;
//        REQUIRE(sm.intersection(sm) == sm);
//    }
//
//    SECTION("Empty / Non-empty") {
//        SparseMap sm;
//        SparseMap sm2{{index_type{1}, {index_type{0}, index_type{3}}},
//                      {index_type{2}, {index_type{1}, index_type{2}}}};
//        REQUIRE(sm.intersection(sm2) == sm);
//        REQUIRE(sm2.intersection(sm) == sm);
//    }
//
//    SECTION("Non-empty / Non-empty") {
//        SparseMap sm{{index_type{1}, {index_type{0}, index_type{3}}},
//                     {index_type{2}, {index_type{1}, index_type{2}}}};
//        SparseMap sm2{{index_type{0}, {index_type{0}, index_type{3}}},
//                      {index_type{1}, {index_type{1}, index_type{2}}},
//                      {index_type{2}, {index_type{1}, index_type{2}}},
//                      {index_type{3}, {index_type{1}, index_type{2}}},
//        };
//        SparseMap incompatible{{index_type{1, 2}, {index_type{0},
//        index_type{3}}},
//                               {index_type{2, 3}, {index_type{1},
//                               index_type{2}}}};
//        SparseMap corr{{index_type{2}, {index_type{1}, index_type{2}}}};
//        REQUIRE(sm.intersection(sm2) == corr);
//        REQUIRE(sm2.intersection(sm) == corr);
//        REQUIRE(sm.intersection(corr) == corr);
//        REQUIRE_THROWS_AS(sm.intersection(incompatible), std::runtime_error);
//    }
//}
//
// TEST_CASE("SparseMap : hash"){
//    SECTION("Empty"){
//        SparseMap sm;
//        REQUIRE(sde::hash_objects(sm) == "50e20c9ba664054fb322f0499e108b5f");
//    }
//
//    SECTION("Non-empty"){
//        SparseMap sm{{index_type{}, {index_type{}}}};
//        REQUIRE(sde::hash_objects(sm) == "64d6c4b037c182902ce14d4fabe15ce2");
//    }
//
//}
//