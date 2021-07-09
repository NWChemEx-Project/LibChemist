#include "libchemist/chemical_system/chemical_system.hpp"
#include <catch2/catch.hpp>

using namespace libchemist;

TEST_CASE("ChemicalSystem") {
    SECTION("Typedefs") {
        SECTION("pimpl_t") {
            using pimpl_t = typename ChemicalSystem::pimpl_t;
            using corr_t  = detail_::ChemicalSystemPIMPL;
            STATIC_REQUIRE(std::is_same_v<pimpl_t, corr_t>);
        }

        SECTION("pimpl_ptr_t") {
            using pimpl_ptr_t = typename ChemicalSystem::pimpl_ptr_t;
            using corr_t      = std::unique_ptr<detail_::ChemicalSystemPIMPL>;
            STATIC_REQUIRE(std::is_same_v<pimpl_ptr_t, corr_t>);
        }

        SECTION("molecule_t") {
            using molecule_t = typename ChemicalSystem::molecule_t;
            using corr_t     = Molecule;
            STATIC_REQUIRE(std::is_same_v<molecule_t, corr_t>);
        }

        SECTION("mol_ref_t") {
            using mol_ref_t = typename ChemicalSystem::mol_ref_t;
            using corr_t    = Molecule&;
            STATIC_REQUIRE(std::is_same_v<mol_ref_t, corr_t>);
        }

        SECTION("const_mol_ref_t") {
            using const_mol_ref_t = typename ChemicalSystem::const_mol_ref_t;
            using corr_t          = const Molecule&;
            STATIC_REQUIRE(std::is_same_v<const_mol_ref_t, corr_t>);
        }

        SECTION("charge_type") {
            using t = typename ChemicalSystem::charge_type;
            using corr = double;
            STATIC_REQUIRE(std::is_same_v<t, corr>);
        }

        SECTION("multiplicity_type") {
            using t = typename ChemicalSystem::multiplicity_type;
            using corr = unsigned int;
            STATIC_REQUIRE(std::is_same_v<t, corr>);
        }

        SECTION("epot_t") {
            using epot_t = typename ChemicalSystem::epot_t;
            using corr_t = potentials::Electrostatic;
            STATIC_REQUIRE(std::is_same_v<epot_t, corr_t>);
        }

        SECTION("epot_ref_t") {
            using epot_ref_t = typename ChemicalSystem::epot_ref_t;
            using corr_t     = potentials::Electrostatic&;
            STATIC_REQUIRE(std::is_same_v<epot_ref_t, corr_t>);
        }

        SECTION("const_epot_ref_t") {
            using const_epot_ref_t = typename ChemicalSystem::const_epot_ref_t;
            using corr_t           = const potentials::Electrostatic&;
            STATIC_REQUIRE(std::is_same_v<const_epot_ref_t, corr_t>);
        }
    }

    libchemist::Molecule default_mol, h{libchemist::Atom(1ul)};

    libchemist::potentials::Electrostatic default_v, v;
    v.add_charge(libchemist::PointCharge<double>());

    SECTION("Default ctor") {
        ChemicalSystem sys;
        REQUIRE(sys.molecule() == default_mol);
        REQUIRE(sys.multiplicity() == 1);
        REQUIRE(sys.charge() == 0.0);
        REQUIRE(sys.external_electrostatic_potential() == default_v);
    }

    SECTION("Copy ctor") {
        ChemicalSystem sys(h, 2, -2, v);
        ChemicalSystem copy(sys);
        REQUIRE(sys == copy);
        REQUIRE(copy.molecule() == h);
        REQUIRE(copy.multiplicity() == 2);
        REQUIRE(copy.charge() == -2);
        REQUIRE(copy.external_electrostatic_potential() == v);
    }

    SECTION("Move ctor") {
        ChemicalSystem sys(h, 2, -2, v);
        ChemicalSystem moved(std::move(sys));
        REQUIRE(moved.molecule() == h);
        REQUIRE(moved.multiplicity() == 2);
        REQUIRE(moved.charge() == -2);
        REQUIRE(moved.external_electrostatic_potential() == v);
    }

    SECTION("value ctor") {
        SECTION("Default multiplicity,charge, and potential") {
            ChemicalSystem sys(h);
            REQUIRE(sys.molecule() == h);
            REQUIRE(sys.multiplicity() == 1);
            REQUIRE(sys.charge() == 0.0);
            REQUIRE(sys.external_electrostatic_potential() == default_v);
        }
        SECTION("Default charge and potential"){
            ChemicalSystem sys(h, 2);
            REQUIRE(sys.molecule() == h);
            REQUIRE(sys.multiplicity() == 2);
            REQUIRE(sys.charge() == 0.0);
            REQUIRE(sys.external_electrostatic_potential() == default_v);
        }

        SECTION("Default potential") {
            ChemicalSystem sys(h, 2, -2);
            REQUIRE(sys.molecule() == h);
            REQUIRE(sys.multiplicity() == 2);
            REQUIRE(sys.charge() == -2);
            REQUIRE(sys.external_electrostatic_potential() == default_v);
        }

        SECTION("Set all") {
            ChemicalSystem sys(h, 2, -2, v);
            REQUIRE(sys.molecule() == h);
            REQUIRE(sys.multiplicity() == 2);
            REQUIRE(sys.charge() == -2);
            REQUIRE(sys.external_electrostatic_potential() == v);
        }
    }

    SECTION("copy assignment") {
        ChemicalSystem sys(h, 2, -2, v);
        ChemicalSystem copy;
        auto pcopy = &(copy = sys);
        REQUIRE(sys == copy);
        REQUIRE(pcopy == &copy);
        REQUIRE(copy.molecule() == h);
        REQUIRE(copy.multiplicity() == 2);
        REQUIRE(copy.charge() == -2);
        REQUIRE(copy.external_electrostatic_potential() == v);
    }

    SECTION("move assignment") {
        ChemicalSystem sys(h, 2, -2, v);
        ChemicalSystem moved;
        auto pmoved = &(moved = std::move(sys));
        REQUIRE(pmoved == &moved);
        REQUIRE(moved.molecule() == h);
        REQUIRE(moved.multiplicity() == 2);
        REQUIRE(moved.charge() == -2);
        REQUIRE(moved.external_electrostatic_potential() == v);
    }

    SECTION("molecule()") {
        ChemicalSystem sys(h);

        SECTION("value") { REQUIRE(sys.molecule() == h); }

        SECTION("is read/write") {
            sys.molecule() = Molecule{};
            REQUIRE(sys.molecule() == Molecule{});
        }

        SECTION("Allocates new PIMPL if no PIMPL") {
            ChemicalSystem buffer(std::move(sys));
            REQUIRE(sys.molecule() == Molecule{});
        }
    }

    SECTION("molecule() const") {
        ChemicalSystem sys(h);

        SECTION("value") { REQUIRE(std::as_const(sys).molecule() == h); }

        SECTION("Throws if no PIMPL") {
            ChemicalSystem buffer(std::move(sys));
            const auto& csys = sys;
            REQUIRE_THROWS_AS(csys.molecule(), std::runtime_error);
        }
    }

    SECTION("multiplicity()") {
        ChemicalSystem sys(h, 2);

        SECTION("value") { REQUIRE(sys.multiplicity() == 2); }

        SECTION("is read/write") {
            sys.multiplicity() = 3;
            REQUIRE(sys.multiplicity() == 3);
        }

        SECTION("Allocates new PIMPL if no PIMPL") {
            ChemicalSystem buffer(std::move(sys));
            REQUIRE(sys.multiplicity() == 1);
        }
    }

    SECTION("multiplicity() const") {
        ChemicalSystem sys(h, 2);

        SECTION("value") { REQUIRE(std::as_const(sys).multiplicity() == 2); }

        SECTION("Throws if no PIMPL") {
            ChemicalSystem buffer(std::move(sys));
            const auto& csys = sys;
            REQUIRE_THROWS_AS(csys.multiplicity(), std::runtime_error);
        }
    }

    SECTION("charge()") {
        ChemicalSystem sys(h, 2, -2);

        SECTION("value") { REQUIRE(sys.charge() == -2); }

        SECTION("is read/write") {
            sys.charge() = 1;
            REQUIRE(sys.charge() == 1);
        }

        SECTION("Allocates new PIMPL if no PIMPL") {
            ChemicalSystem buffer(std::move(sys));
            REQUIRE(sys.charge() == 0.0);
        }
    }

    SECTION("charge() const") {
        ChemicalSystem sys(h, 2, -2);

        SECTION("value") { REQUIRE(std::as_const(sys).charge() == -2); }

        SECTION("Throws if no PIMPL") {
            ChemicalSystem buffer(std::move(sys));
            const auto& csys = sys;
            REQUIRE_THROWS_AS(csys.charge(), std::runtime_error);
        }
    }

    SECTION("external_electrostatic_potential()") {
        ChemicalSystem sys(h, 2, -2, v);

        SECTION("value") {
            REQUIRE(sys.external_electrostatic_potential() == v);
        }

        SECTION("is read/write") {
            potentials::Electrostatic new_v;
            sys.external_electrostatic_potential() = new_v;
            REQUIRE(sys.external_electrostatic_potential() == new_v);
        }

        SECTION("Allocates new PIPML if no PIMPL") {
            ChemicalSystem buffer(std::move(sys));
            potentials::Electrostatic corr;
            REQUIRE(sys.external_electrostatic_potential() == corr);
        }
    }

    SECTION("external_electrostatic_potential() const") {
        ChemicalSystem sys(h, 2, -2, v);

        SECTION("value") {
            REQUIRE(std::as_const(sys).external_electrostatic_potential() == v);
        }

        SECTION("Throws if no PIMPL") {
            ChemicalSystem buffer(std::move(sys));
            const ChemicalSystem& csys = sys;
            REQUIRE_THROWS_AS(csys.external_electrostatic_potential(),
                              std::runtime_error);
        }
    }

    SECTION("hash") {
        SECTION("LHS is default") {
            ChemicalSystem lhs;
            auto lhs_hash = sde::hash_objects(lhs);

            SECTION("RHS is default") {
                ChemicalSystem rhs;
                REQUIRE(lhs_hash == sde::hash_objects(rhs));
            }

            SECTION("RHS has a different molecule") {
                ChemicalSystem rhs(h);
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }

            SECTION("RHS has a different multiplicity") {
                ChemicalSystem rhs(default_mol, 2);
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }

            SECTION("RHS has a different charge") {
                ChemicalSystem rhs(default_mol, 1, -3.2);
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }

            SECTION("RHS has a different potential") {
                ChemicalSystem rhs(default_mol, 1, 0.0, v);
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }
        }

        SECTION("LHS has values") {
            ChemicalSystem lhs(h, 2, -3.2, v);
            auto lhs_hash = sde::hash_objects(lhs);

            SECTION("RHS is default") {
                ChemicalSystem rhs;
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }

            SECTION("RHS has a different molecule") {
                ChemicalSystem rhs(default_mol, 2, -3.2, v);
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }

            SECTION("RHS has a different multiplicity") {
                ChemicalSystem rhs(h, 1, -3.2, v);
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }

            SECTION("RHS has a different charge") {
                ChemicalSystem rhs(h, 2, 0.0, v);
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }
            SECTION("RHS has a different potential") {
                ChemicalSystem rhs(h, 2, -3.2, default_v);
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }
        }
    }

    SECTION("operator==") {
        SECTION("LHS is default") {
            ChemicalSystem lhs;

            SECTION("RHS is default") {
                ChemicalSystem rhs;
                REQUIRE(lhs == rhs);
            }

            SECTION("RHS has a different molecule") {
                ChemicalSystem rhs(h);
                REQUIRE_FALSE(lhs == rhs);
            }

            SECTION("RHS has a different multiplity"){
                ChemicalSystem rhs(default_mol, 2);
                REQUIRE_FALSE(lhs == rhs);
            }

            SECTION("RHS has a differnt charge") {
                ChemicalSystem rhs(default_mol, 1, -3.2);
            }

            SECTION("RHS has a different potential") {
                ChemicalSystem rhs(default_mol, 1, 0.0, v);
                REQUIRE_FALSE(lhs == rhs);
            }

        }

        SECTION("LHS has values") {
            ChemicalSystem lhs(h, 2, -3.2, v);

            SECTION("RHS is default") {
                ChemicalSystem rhs;
                REQUIRE_FALSE(lhs == rhs);
            }

            SECTION("RHS has a different molecule") {
                ChemicalSystem rhs(default_mol, 2, -3.2, v);
                REQUIRE_FALSE(lhs == rhs);
            }

            SECTION("RHS has different multiplicity"){
                ChemicalSystem rhs(h, 1, -3.2, v);
                REQUIRE_FALSE(lhs == rhs);
            }

            SECTION("RHS has different charge") {
                ChemicalSystem rhs(h, 2, 0.0, v);
                REQUIRE_FALSE(lhs == rhs);
            }

            SECTION("RHS has a different potential") {
                ChemicalSystem rhs(h, 2, -3.2, default_v);
                REQUIRE_FALSE(lhs == rhs);
            }
        }
    }
}
