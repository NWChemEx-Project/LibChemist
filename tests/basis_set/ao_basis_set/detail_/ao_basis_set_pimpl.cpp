#include <catch2/catch.hpp>
#include <libchemist/basis_set/ao_basis_set/detail_/ao_basis_set_pimpl.hpp>

using pimpl_t  = libchemist::detail_::AOBasisSetPIMPL<double>;
using center_t = libchemist::Center<double>;

TEST_CASE("AOBasisSetPIMPL : default ctor") {
    pimpl_t p{};
    REQUIRE(p.size() == 0);
}

TEST_CASE("AOBasisSetPIMPL : size") {
    pimpl_t p;
    p.add_center(center_t(1.0, 2.0, 3.0));
    REQUIRE(p.size() == 1);
}

TEST_CASE("AOBasisSetPIMPL : add_center") {
    pimpl_t p;
    p.add_center(center_t(1.0, 2.0, 3.0));
    REQUIRE(p.at(0) == center_t(1.0, 2.0, 3.0));
}

TEST_CASE("AOBasisSetPIMPL : at") {
    pimpl_t p;
    p.add_center(center_t(1.0, 2.0, 3.0));
    REQUIRE(p.at(0) == center_t(1.0, 2.0, 3.0));
}

TEST_CASE("AOBasisSetPIMPL : at() const") {
    pimpl_t p;
    p.add_center(center_t(1.0, 2.0, 3.0));
    REQUIRE(std::as_const(p).at(0) == center_t(1.0, 2.0, 3.0));
}
