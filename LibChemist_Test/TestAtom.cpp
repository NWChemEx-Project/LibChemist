#include <LibChemist/Atom.hpp>
#include <catch/catch.hpp>

using namespace LibChemist;
using xyz_type = typename Atom::xyz_type;
using Property = typename Atom::atom_property_type;
using properties_map = typename Atom::properties_map;
using shell_container = typename Atom::shell_container;
using basis_lut_type = typename Atom::basis_lut_type;


TEST_CASE("Atom typedefs") {
    //We only check typedefs that we didn't forward above
    REQUIRE(std::is_same<typename Atom::property_value, double>::value);
}

void check_state(const Atom& a, const xyz_type& coords,
                 const properties_map& ps, const basis_lut_type& bases){
    REQUIRE(a.coords == coords);
    REQUIRE(a.properties == ps);
    REQUIRE(a.bases == bases);
}

TEST_CASE("Empty Atom") {
    Atom empty;
    check_state(empty, {}, {}, {});
}

TEST_CASE("Aggregate Initialized"){
    xyz_type origin{0.0, 0.0, 0.0};
    properties_map props{{Property::mass, 238.02891},{Property::Z, 92.0}};
    properties_map null_map{ {Property::Z, 0.0}, {Property::mass, 0.0},
                             {Property::isotope_mass, 0.0},
                             {Property::cov_radius, 0.0},
                             {Property::vdw_radius, 0.0}};
    basis_lut_type bases{{"PRIMARY", shell_container{}}};
    Atom a{origin, props, bases};
    check_state(a, origin, props, bases);
    REQUIRE(is_real_atom(a));
    SECTION("Copy ctor"){
        Atom b{a};
        REQUIRE(b == a);
    }
    SECTION("Copy assignment"){
        Atom b;
        REQUIRE( b != a);
        b = a;
        REQUIRE( b == a);
    }
    SECTION("Move ctor"){
        Atom b{std::move(a)};
        check_state(b, origin, props, bases);
    }
    SECTION("Move assignment"){
        Atom b;
        REQUIRE( b != a);
        b = std::move(a);
        check_state(b, origin, props, bases);
    }
    SECTION("Ghost atom") {
        Atom b = create_ghost(a);
        check_state(b, origin, null_map, bases);
        REQUIRE(is_ghost_atom(b));
        REQUIRE(!is_ghost_atom(a));
        REQUIRE(!is_real_atom(b));
    }
    SECTION("Dummy atom") {
        Atom b = create_dummy(a);
        check_state(b, origin, null_map, {});
        REQUIRE(is_dummy_atom(b));
        REQUIRE(!is_dummy_atom(a));
        REQUIRE(!is_real_atom(b));
    }
    SECTION("Point charge") {
        Atom b = create_charge(a, 3.9);
        null_map[Property::Z] = 3.9;
        check_state(b, origin, null_map, {});
        REQUIRE(is_charge(b));
        REQUIRE(!is_charge(a));
        REQUIRE(!is_real_atom(b));
    }
}
