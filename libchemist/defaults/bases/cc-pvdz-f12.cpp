/*
 * This file is autogenerated by generate_basis.py.  Any modifications made in
 * this file will be lost next time generate_basis.py is run.
 */

#include "libchemist/defaults/nwx_basis_list.hpp"

namespace libchemist::detail_ {

Center<double> cc_dash_pvdz_dash_f12(std::size_t Z) {
    switch(Z) {
        case(3): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 0,
                         std::vector<double>{0.0001330, 0.0010250, 0.0052720,
                                             0.0209290, 0.0663400, 0.1657750,
                                             0.3150380, 0.3935230, 0.1908700},
                         std::vector<double>{5988.0000000, 898.9000000,
                                             205.9000000, 59.2400000,
                                             19.8700000, 7.4060000, 2.9300000,
                                             1.1890000, 0.4798000});
            rv.add_shell(
              ShellType::pure, 0,
              std::vector<double>{-0.0000210, -0.0001610, -0.0008200,
                                  -0.0033260, -0.0105190, -0.0280970,
                                  -0.0559360, -0.0992370, -0.1121890},
              std::vector<double>{5988.0000000, 898.9000000, 205.9000000,
                                  59.2400000, 19.8700000, 7.4060000, 2.9300000,
                                  1.1890000, 0.4798000});
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.0750900});
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.0283200});
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.0076000});
            rv.add_shell(ShellType::pure, 1,
                         std::vector<double>{0.0086300, 0.0475380},
                         std::vector<double>{3.2660000, 0.6511000});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.1696000});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.0557800});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.0205000});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.0091000});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.1484380});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.0684730});
            return rv;
        } // End case
        case(4): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(ShellType::pure, 0,
                         std::vector<double>{0.0002360, 0.0018260, 0.0094520,
                                             0.0379570, 0.1199650, 0.2821620,
                                             0.4274040, 0.2662780, -0.0072750},
                         std::vector<double>{6863.0000000, 1030.0000000,
                                             234.7000000, 66.5600000,
                                             21.6900000, 7.7340000, 2.9160000,
                                             1.1300000, 0.1101000});
            rv.add_shell(ShellType::pure, 0,
                         std::vector<double>{-0.0000430, -0.0003330, -0.0017360,
                                             -0.0070120, -0.0231260, -0.0581380,
                                             -0.1145560, -0.1359080, 0.5774410},
                         std::vector<double>{6863.0000000, 1030.0000000,
                                             234.7000000, 66.5600000,
                                             21.6900000, 7.7340000, 2.9160000,
                                             1.1300000, 0.1101000});
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.2577000});
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.0440900});
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.0147000});
            rv.add_shell(ShellType::pure, 1,
                         std::vector<double>{0.0107360, 0.0628540},
                         std::vector<double>{7.4360000, 1.5770000});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.4352000});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.1438000});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.0499400});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.0093000});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.2992840});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.1250500});
            return rv;
        } // End case
        case(11): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(
              ShellType::pure, 0,
              std::vector<double>{0.0000181, 0.0001404, 0.0007384, 0.0031118,
                                  0.0112081, 0.0352828, 0.0959897, 0.2137350,
                                  0.3486880, 0.3245660, 0.1126330, 0.0070680,
                                  0.0005980, -0.0000053},
              std::vector<double>{
                423000.0000000, 63340.0000000, 14410.0000000, 4077.0000000,
                1328.0000000, 478.6000000, 186.2000000, 76.9200000, 33.3200000,
                15.0000000, 6.8690000, 2.6830000, 1.1090000, 0.0601500});
            rv.add_shell(
              ShellType::pure, 0,
              std::vector<double>{
                -0.0000044, -0.0000343, -0.0001801, -0.0007639, -0.0027525,
                -0.0088602, -0.0247939, -0.0605995, -0.1164460, -0.1624370,
                -0.0438891, 0.3379170, 0.5613470, 0.0040675},
              std::vector<double>{
                423000.0000000, 63340.0000000, 14410.0000000, 4077.0000000,
                1328.0000000, 478.6000000, 186.2000000, 76.9200000, 33.3200000,
                15.0000000, 6.8690000, 2.6830000, 1.1090000, 0.0601500});
            rv.add_shell(
              ShellType::pure, 0,
              std::vector<double>{0.0000007, 0.0000052, 0.0000271, 0.0001146,
                                  0.0004151, 0.0013298, 0.0037560, 0.0091403,
                                  0.0179859, 0.0251477, 0.0076352, -0.0614589,
                                  -0.1157210, 0.6264060},
              std::vector<double>{
                423000.0000000, 63340.0000000, 14410.0000000, 4077.0000000,
                1328.0000000, 478.6000000, 186.2000000, 76.9200000, 33.3200000,
                15.0000000, 6.8690000, 2.6830000, 1.1090000, 0.0601500});
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.4540000});
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.0238200});
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.0067000});
            rv.add_shell(ShellType::pure, 1,
                         std::vector<double>{0.0022439, 0.0173997, 0.0774125,
                                             0.2191020, 0.3785220, 0.3949020,
                                             0.1604240, 0.0023331, 0.0019954},
                         std::vector<double>{243.3000000, 57.3900000,
                                             18.1000000, 6.5750000, 2.5210000,
                                             0.9607000, 0.3512000, 0.0982700,
                                             0.0373400});
            rv.add_shell(ShellType::pure, 1,
                         std::vector<double>{-0.0002224, -0.0017428, -0.0077546,
                                             -0.0225187, -0.0384330, -0.0450177,
                                             -0.0192132, 0.1826970, 0.5578970},
                         std::vector<double>{243.3000000, 57.3900000,
                                             18.1000000, 6.5750000, 2.5210000,
                                             0.9607000, 0.3512000, 0.0982700,
                                             0.0373400});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.0150000});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.0060000});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{3.3342860});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.0883620});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.8963110});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.1107450});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.0561760});
            return rv;
        } // End case
        case(12): {
            Center<double> rv(0.0, 0.0, 0.0);
            rv.add_shell(
              ShellType::pure, 0,
              std::vector<double>{0.0000730, 0.0005667, 0.0029627, 0.0122962,
                                  0.0427324, 0.1230130, 0.2748320, 0.4018180,
                                  0.2646970, 0.0332612, -0.0044133, 0.0020602,
                                  0.0007082},
              std::vector<double>{164900.0000000, 24710.0000000, 5628.0000000,
                                  1596.0000000, 521.0000000, 188.0000000,
                                  73.0100000, 29.9000000, 12.5400000, 4.3060000,
                                  1.8260000, 0.7417000, 0.0761200});
            rv.add_shell(
              ShellType::pure, 0,
              std::vector<double>{-0.0000184, -0.0001435, -0.0007487,
                                  -0.0031441, -0.0110481, -0.0336058,
                                  -0.0825946, -0.1593140, -0.1528880, 0.1908490,
                                  0.5799640, 0.3720290, -0.0119344},
              std::vector<double>{164900.0000000, 24710.0000000, 5628.0000000,
                                  1596.0000000, 521.0000000, 188.0000000,
                                  73.0100000, 29.9000000, 12.5400000, 4.3060000,
                                  1.8260000, 0.7417000, 0.0761200});
            rv.add_shell(
              ShellType::pure, 0,
              std::vector<double>{0.0000036, 0.0000276, 0.0001444, 0.0006057,
                                  0.0021353, 0.0064993, 0.0161446, 0.0315766,
                                  0.0316374, -0.0439140, -0.1510930, -0.2176680,
                                  0.5472450},
              std::vector<double>{164900.0000000, 24710.0000000, 5628.0000000,
                                  1596.0000000, 521.0000000, 188.0000000,
                                  73.0100000, 29.9000000, 12.5400000, 4.3060000,
                                  1.8260000, 0.7417000, 0.0761200});
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.1457000});
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.0331000});
            rv.add_shell(ShellType::pure, 0, std::vector<double>{1.0000000},
                         std::vector<double>{0.0120000});
            rv.add_shell(ShellType::pure, 1,
                         std::vector<double>{0.0020753, 0.0162869, 0.0738697,
                                             0.2142970, 0.3821540, 0.3981780,
                                             0.1528780, -0.0043754, 0.0062958},
                         std::vector<double>{316.9000000, 74.8600000,
                                             23.7200000, 8.6690000, 3.3630000,
                                             1.3100000, 0.4911000, 0.2364000,
                                             0.0873300});
            rv.add_shell(ShellType::pure, 1,
                         std::vector<double>{-0.0003297, -0.0025875, -0.0119120,
                                             -0.0350227, -0.0639968, -0.0704436,
                                             -0.0375836, 0.1770430, 0.5667070},
                         std::vector<double>{316.9000000, 74.8600000,
                                             23.7200000, 8.6690000, 3.3630000,
                                             1.3100000, 0.4911000, 0.2364000,
                                             0.0873300});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.0323700});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.0090000});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{3.1820390});
            rv.add_shell(ShellType::pure, 1, std::vector<double>{1.0000000},
                         std::vector<double>{0.1718480});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{1.4313080});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.1994320});
            rv.add_shell(ShellType::pure, 2, std::vector<double>{1.0000000},
                         std::vector<double>{0.0909370});
            return rv;
        } // End case
        default: {
            throw std::out_of_range("Basis not available for Z");
        }
    } // end switch

} // end function
} // namespace libchemist::detail_