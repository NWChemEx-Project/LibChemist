#include "libchemist/chemical_system/chemical_system_pimpl.hpp"
#include <catch2/catch.hpp>

TEST_CASE("ChemicalSystemPIMPL") {
    using chem_sys_t       = libchemist::ChemicalSystem;
    using chem_sys_pimpl_t = libchemist::detail_::ChemicalSystemPIMPL;

    libchemist::Molecule h(libchemist::Atom(1ul));

    libchemist::potentials::Electrostatic v;
    v.add_charge(libchemist::PointCharge<double>());

    // For all of these typedefs we are just ensuring that the ChemicalSystem
    // and the ChemicalSystemPIMPL class are synched
    SECTION("Typedefs") {
        SECTION("chem_sys_t") {
            using t = typename chem_sys_pimpl_t::chem_sys_t;
            STATIC_REQUIRE(std::is_same_v<t, chem_sys_t>);
        }

        SECTION("pimpl_ptr_t") {
            using t    = typename chem_sys_pimpl_t::pimpl_ptr_t;
            using corr = typename chem_sys_t::pimpl_ptr_t;
        }

        SECTION("molecule_t") {
            using t    = typename chem_sys_pimpl_t::molecule_t;
            using corr = typename chem_sys_t::molecule_t;
            STATIC_REQUIRE(std::is_same_v<t, corr>);
        }

        SECTION("mol_ref_t") {
            using t    = typename chem_sys_pimpl_t::mol_ref_t;
            using corr = typename chem_sys_t::mol_ref_t;
            STATIC_REQUIRE(std::is_same_v<t, corr>);
        }

        SECTION("const_mol_ref_t") {
            using t    = typename chem_sys_pimpl_t::const_mol_ref_t;
            using corr = typename chem_sys_t::const_mol_ref_t;
            STATIC_REQUIRE(std::is_same_v<t, corr>);
        }

        SECTION("mol_ref_t") {
            using t    = typename chem_sys_pimpl_t::mol_ref_t;
            using corr = typename chem_sys_t::mol_ref_t;
            STATIC_REQUIRE(std::is_same_v<t, corr>);
        }

        SECTION("epot_t") {
            using t    = typename chem_sys_pimpl_t::epot_t;
            using corr = typename chem_sys_t::epot_t;
            STATIC_REQUIRE(std::is_same_v<t, corr>);
        }

        SECTION("epot_ref_t") {
            using t    = typename chem_sys_pimpl_t::epot_ref_t;
            using corr = typename chem_sys_t::epot_ref_t;
            STATIC_REQUIRE(std::is_same_v<t, corr>);
        }

        SECTION("const_epot_ref_t") {
            using t    = typename chem_sys_pimpl_t::const_epot_ref_t;
            using corr = typename chem_sys_t::const_epot_ref_t;
            STATIC_REQUIRE(std::is_same_v<t, corr>);
        }
    }

    SECTION("Default ctor") {
        chem_sys_pimpl_t pimpl;
        REQUIRE(pimpl.molecule() == libchemist::Molecule());
        REQUIRE(pimpl.external_electrostatic_potential() ==
                libchemist::potentials::Electrostatic());
    }

    SECTION("Value ctor") {
        SECTION("Defaulted potential") {
            chem_sys_pimpl_t pimpl(h);
            REQUIRE(pimpl.molecule() == h);
            REQUIRE(pimpl.external_electrostatic_potential() ==
                    libchemist::potentials::Electrostatic());
        }

        SECTION("All values set") {
            chem_sys_pimpl_t pimpl(h, v);
            REQUIRE(pimpl.molecule() == h);
            REQUIRE(pimpl.external_electrostatic_potential() == v);
        }
    }

    SECTION("clone") {
        SECTION("Default") {
            chem_sys_pimpl_t pimpl;
            auto copy = pimpl.clone();
            REQUIRE(*copy == pimpl);
        }

        SECTION("Non-default") {
            chem_sys_pimpl_t pimpl(h, v);
            auto copy = pimpl.clone();
            REQUIRE(*copy == pimpl);
        }
    }

    SECTION("molecule()") {
        SECTION("Default") {
            chem_sys_pimpl_t pimpl;
            REQUIRE(pimpl.molecule() == libchemist::Molecule());
        }

        SECTION("Has value") {
            chem_sys_pimpl_t pimpl(h);
            REQUIRE(pimpl.molecule() == h);
        }

        SECTION("Is writeable") {
            chem_sys_pimpl_t pimpl;
            pimpl.molecule() = h;
            REQUIRE(pimpl.molecule() == h);
        }
    }

    SECTION("molecule() const") {
        SECTION("Default") {
            const chem_sys_pimpl_t pimpl;
            REQUIRE(pimpl.molecule() == libchemist::Molecule());
        }

        SECTION("Has value") {
            const chem_sys_pimpl_t pimpl(h);
            REQUIRE(pimpl.molecule() == h);
        }
    }

    SECTION("external_electrostatic_potential()") {
        SECTION("default") {
            chem_sys_pimpl_t pimpl;
            libchemist::potentials::Electrostatic corr;
            REQUIRE(pimpl.external_electrostatic_potential() == corr);
        }

        SECTION("Has value") {
            chem_sys_pimpl_t pimpl(h, v);
            REQUIRE(pimpl.external_electrostatic_potential() == v);
        }

        SECTION("Is writeable") {
            chem_sys_pimpl_t pimpl;
            pimpl.external_electrostatic_potential() = v;
            REQUIRE(pimpl.external_electrostatic_potential() == v);
        }
    }

    SECTION("external_electrostatic_potential() const") {
        SECTION("default") {
            const chem_sys_pimpl_t pimpl;
            libchemist::potentials::Electrostatic corr;
            REQUIRE(pimpl.external_electrostatic_potential() == corr);
        }

        SECTION("Has value") {
            const chem_sys_pimpl_t pimpl(h, v);
            REQUIRE(pimpl.external_electrostatic_potential() == v);
        }
    }

    SECTION("hash") {
        SECTION("LHS is default") {
            chem_sys_pimpl_t lhs;
            auto lhs_hash = sde::hash_objects(lhs);

            SECTION("RHS is default") {
                chem_sys_pimpl_t rhs;
                REQUIRE(lhs_hash == sde::hash_objects(rhs));
            }

            SECTION("RHS has a different molecule") {
                chem_sys_pimpl_t rhs(h);
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }

            SECTION("RHS has a different potential") {
                chem_sys_pimpl_t rhs(libchemist::Molecule(), v);
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }

            SECTION("RHS has a different molecule and potential") {
                chem_sys_pimpl_t rhs(h, v);
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }
        }

        SECTION("LHS has values") {
            chem_sys_pimpl_t lhs(h, v);
            auto lhs_hash = sde::hash_objects(lhs);

            SECTION("RHS is default") {
                chem_sys_pimpl_t rhs;
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }

            SECTION("RHS has a different molecule") {
                chem_sys_pimpl_t rhs(libchemist::Molecule(), v);
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }

            SECTION("RHS has a different potential") {
                chem_sys_pimpl_t rhs(h,
                                     libchemist::potentials::Electrostatic());
                REQUIRE_FALSE(lhs_hash == sde::hash_objects(rhs));
            }

            SECTION("RHS has same molecule and potential") {
                chem_sys_pimpl_t rhs(h, v);
                REQUIRE(lhs_hash == sde::hash_objects(rhs));
            }
        }
    }

    SECTION("operator==") {
        SECTION("LHS is default") {
            chem_sys_pimpl_t lhs;

            SECTION("RHS is default") {
                chem_sys_pimpl_t rhs;
                REQUIRE(lhs == rhs);
            }

            SECTION("RHS has a different molecule") {
                chem_sys_pimpl_t rhs(h);
                REQUIRE_FALSE(lhs == rhs);
            }

            SECTION("RHS has a different potential") {
                chem_sys_pimpl_t rhs(libchemist::Molecule(), v);
                REQUIRE_FALSE(lhs == rhs);
            }

            SECTION("RHS has a different molecule and potential") {
                chem_sys_pimpl_t rhs(h, v);
                REQUIRE_FALSE(lhs == rhs);
            }
        }

        SECTION("LHS has values") {
            chem_sys_pimpl_t lhs(h, v);

            SECTION("RHS is default") {
                chem_sys_pimpl_t rhs;
                REQUIRE_FALSE(lhs == rhs);
            }

            SECTION("RHS has a different molecule") {
                chem_sys_pimpl_t rhs(libchemist::Molecule(), v);
                REQUIRE_FALSE(lhs == rhs);
            }

            SECTION("RHS has a different potential") {
                chem_sys_pimpl_t rhs(h,
                                     libchemist::potentials::Electrostatic());
                REQUIRE_FALSE(lhs == rhs);
            }

            SECTION("RHS has same molecule and potential") {
                chem_sys_pimpl_t rhs(h, v);
                REQUIRE(lhs == rhs);
            }
        }
    }
}