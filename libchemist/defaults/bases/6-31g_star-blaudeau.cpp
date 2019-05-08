/*
 * This file is autogenerated by generate_basis.py.  Any modifications made in
 * this file will be lost next time generate_basis.py is run.
 */

#include "libchemist/defaults/nwx_basis_list.hpp"

namespace libchemist::detail_ {

AOBasisSet six_dash_31g_star_dash_blaudeau(std::size_t Z) {
    switch(Z) {
        case(19): {
            return AOBasisSet{
              AOShell{
                0,
                AOPrimitive{31478.7000000, 0.00398387},
                AOPrimitive{4726.8900000, 0.03050180},
                AOPrimitive{1075.4300000, 0.15073800},
                AOPrimitive{303.3980000, 0.51912900},
                AOPrimitive{98.3271000, 1.03670000},
                AOPrimitive{33.6362000, 0.76399000},
              },
              AOShell{
                0,
                AOPrimitive{65.6392000, -0.2824260},
                AOPrimitive{7.3162600, 1.6914900},
                AOPrimitive{2.8902600, 1.2965300},
              },
              AOShell{
                0,
                AOPrimitive{4.5459700, -0.00763436},
                AOPrimitive{0.7040410, 0.0256357},
                AOPrimitive{0.2826690, 0.0166069},
              },
              AOShell{
                0,
                AOPrimitive{0.0290582, 1.0000000},
              },
              AOShell{
                0,
                AOPrimitive{0.0121116, 1.0000000},
              },
              AOShell{
                1,
                AOPrimitive{361.2250000, 0.0209065},
                AOPrimitive{84.6702000, 0.1504360},
                AOPrimitive{26.4691000, 0.5544010},
                AOPrimitive{9.2658100, 1.0409000},
                AOPrimitive{3.3423400, 0.6782530},
              },
              AOShell{
                1,
                AOPrimitive{1.6354600, 0.2409020},
                AOPrimitive{0.8319680, 0.2994240},
                AOPrimitive{0.4368420, 0.4460790},
              },
              AOShell{
                1,
                AOPrimitive{0.1798370, 1.0000000},
              },
              AOShell{
                2,
                AOPrimitive{10.0700000, 0.0480790},
                AOPrimitive{2.5580000, 0.2162930},
                AOPrimitive{0.7968000, 0.4236910},
              },
              AOShell{
                2,
                AOPrimitive{0.2350000, 1.0000000},
              },
            }; // End AOBasis
        }      // End case
        case(20): {
            return AOBasisSet{
              AOShell{
                0,
                AOPrimitive{35138.7000000, 0.00394825},
                AOPrimitive{5276.4100000, 0.0302342},
                AOPrimitive{1200.4700000, 0.1495200},
                AOPrimitive{338.7180000, 0.5159730},
                AOPrimitive{109.8540000, 1.0339500},
                AOPrimitive{37.6089000, 0.7693790},
              },
              AOShell{
                0,
                AOPrimitive{73.1080000, -0.2826850},
                AOPrimitive{8.2407700, 1.6796100},
                AOPrimitive{3.2959800, 1.2803800},
              },
              AOShell{
                0,
                AOPrimitive{5.2341800, -0.00768686},
                AOPrimitive{0.8418720, 0.02538240},
                AOPrimitive{0.3651030, 0.01651220},
              },
              AOShell{
                0,
                AOPrimitive{0.0512224, 1.0000000},
              },
              AOShell{
                0,
                AOPrimitive{0.0198250, 1.0000000},
              },
              AOShell{
                1,
                AOPrimitive{413.1130000, 0.0203270},
                AOPrimitive{96.9358000, 0.1473030},
                AOPrimitive{30.3722000, 0.5488720},
                AOPrimitive{10.6848000, 1.0440700},
                AOPrimitive{3.8821300, 0.6865350},
              },
              AOShell{
                1,
                AOPrimitive{1.9099100, 0.2686680},
                AOPrimitive{0.9793250, 0.2901870},
                AOPrimitive{0.5497690, 0.4429810},
              },
              AOShell{
                1,
                AOPrimitive{0.2312330, 1.0000000},
              },
              AOShell{
                2,
                AOPrimitive{10.9700000, 0.0600080},
                AOPrimitive{2.7680000, 0.2610100},
                AOPrimitive{0.8281000, 0.5086920},
              },
              AOShell{
                2,
                AOPrimitive{0.2160000, 1.0000000},
              },
            }; // End AOBasis
        }      // End case
        default: { throw std::out_of_range("Basis not available for Z"); }
    } // end switch

} // end function
} // namespace libchemist::detail_
