/*
 * This file is autogenerated by generate_basis.py.  Any modifications made in
 * this file will be lost next time generate_basis.py is run.
 */

#include "libchemist/defaults/nwx_basis_list.hpp"

namespace libchemist::detail_ {

Center<double> aug_dash_cc_dash_pvdz_dash_pp_dash_ri_diffuse(std::size_t Z) {
    switch(Z) {
        case(29): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.55704100000e-01});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.64987200000e-01});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.14546100000});
            rv.add_shell(ShellType::pure, 3, std::vector<double>{1.0000000},
                         std::vector<double>{0.12179900000});
            rv.add_shell(ShellType::pure, 4, std::vector<double>{1.0000000},
                         std::vector<double>{0.32154800000});
            rv.add_shell(ShellType::pure, 5, std::vector<double>{1.0000000},
                         std::vector<double>{1.6632700000});
            return rv;
        } // End case
        case(30): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.61992000000e-01});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.61171600000e-01});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.19500400000});
            rv.add_shell(ShellType::pure, 3, std::vector<double>{1.0000000},
                         std::vector<double>{0.13690400000});
            rv.add_shell(ShellType::pure, 4, std::vector<double>{1.0000000},
                         std::vector<double>{0.43605400000});
            rv.add_shell(ShellType::pure, 5, std::vector<double>{1.0000000},
                         std::vector<double>{2.0347300000});
            return rv;
        } // End case
        case(47): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.64866800000e-01});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.65380500000e-01});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.11179800000});
            rv.add_shell(ShellType::pure, 3, std::vector<double>{1.0000000},
                         std::vector<double>{0.12604400000});
            rv.add_shell(ShellType::pure, 4, std::vector<double>{1.0000000},
                         std::vector<double>{0.22158300000});
            rv.add_shell(ShellType::pure, 5, std::vector<double>{1.0000000},
                         std::vector<double>{0.83975800000});
            return rv;
        } // End case
        case(48): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.48975300000e-01});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.45810800000e-01});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.14155000000});
            rv.add_shell(ShellType::pure, 3, std::vector<double>{1.0000000},
                         std::vector<double>{0.15761000000});
            rv.add_shell(ShellType::pure, 4, std::vector<double>{1.0000000},
                         std::vector<double>{0.29413400000});
            rv.add_shell(ShellType::pure, 5, std::vector<double>{1.0000000},
                         std::vector<double>{0.99072700000});
            return rv;
        } // End case
        case(79): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.57228400000e-01});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.63956300000e-01});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.11861300000});
            rv.add_shell(ShellType::pure, 3, std::vector<double>{1.0000000},
                         std::vector<double>{0.11456100000});
            rv.add_shell(ShellType::pure, 4, std::vector<double>{1.0000000},
                         std::vector<double>{0.16500000000});
            rv.add_shell(ShellType::pure, 5, std::vector<double>{1.0000000},
                         std::vector<double>{0.67710900000});
            return rv;
        } // End case
        case(80): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.65624900000e-01});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.55269200000e-01});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.12609800000});
            rv.add_shell(ShellType::pure, 3, std::vector<double>{1.0000000},
                         std::vector<double>{0.12903400000});
            rv.add_shell(ShellType::pure, 4, std::vector<double>{1.0000000},
                         std::vector<double>{0.19023200000});
            rv.add_shell(ShellType::pure, 5, std::vector<double>{1.0000000},
                         std::vector<double>{0.80621500000});
            return rv;
        } // End case
        default: {
            throw std::out_of_range("Basis not available for Z");
        }
    } // end switch

} // end function
} // namespace libchemist::detail_