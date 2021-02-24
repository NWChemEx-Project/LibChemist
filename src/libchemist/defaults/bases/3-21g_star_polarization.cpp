/*
 * This file is autogenerated by generate_basis.py.  Any modifications made in
 * this file will be lost next time generate_basis.py is run.
 */

#include "libchemist/defaults/nwx_basis_list.hpp"

namespace libchemist::detail_ {

Center<double> three_dash_21g_star_polarization(std::size_t Z) {
    switch(Z) {
        case(11): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.1750000});
            return rv;
        } // End case
        case(12): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.1750000});
            return rv;
        } // End case
        case(13): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.3250000});
            return rv;
        } // End case
        case(14): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.4500000});
            return rv;
        } // End case
        case(15): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.5500000});
            return rv;
        } // End case
        case(16): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.6500000});
            return rv;
        } // End case
        case(17): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.7500000});
            return rv;
        } // End case
        case(18): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.8500000});
            return rv;
        } // End case
        default: {
            throw std::out_of_range("Basis not available for Z");
        }
    } // end switch

} // end function
} // namespace libchemist::detail_