/*
 * This file is autogenerated by generate_basis.py.  Any modifications made in
 * this file will be lost next time generate_basis.py is run.
 */

#include "libchemist/defaults/nwx_basis_list.hpp"

namespace libchemist::detail_ {

Center<double> aug_dash_cc_dash_pvtz_dash_nr_diffuse(std::size_t Z) {
    switch(Z) {
        case(21): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.000000e+00},
                         std::vector<double>{1.110000e-02});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.000000e+00},
                         std::vector<double>{1.066000e-02});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.000000e+00},
                         std::vector<double>{1.244000e-02});
            rv.add_shell(ShellType::pure, 3, std::vector<double>{1.000000e+00},
                         std::vector<double>{4.063000e-02});
            rv.add_shell(ShellType::pure, 4, std::vector<double>{1.000000e+00},
                         std::vector<double>{9.473000e-02});
            return rv;
        } // End case
        default: {
            throw std::out_of_range("Basis not available for Z");
        }
    } // end switch

} // end function
} // namespace libchemist::detail_