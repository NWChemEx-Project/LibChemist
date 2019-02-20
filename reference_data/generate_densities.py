"""
Reads xml files with atomic densities and write to cpp files
"""
import os
import xml.etree.ElementTree as ET
from generate_atomicinfo import parse_symbols

def print_pimpl_header(f):
    f.write(
"""
/*
 * This file is autogenerated by generate_densities.py.  Any modifications made in
 * this file will be lost next time generate_densities.py is run.
 */
       
#include \"LibChemist/Defaults/NWXAtomicDensities.hpp\"
            
    std::vector<std::vector<double>> get_atomic_density_(const std::string& name, size_type Z) const override {         
""")

def print_pimpl_footer(f):
    f.write(
"""throw std::out_of_range(\"Basis not available for SAD guess\");
    }//end get_basis_
};
     
} // namespace LibChemist::detail_
""")

def print_basis_header(f, bs_name):
    f.write(
"""/*
 * This file is autogenerated by generate_densities.py.  Any modifications made in
 * this file will be lost next time generate_densities.py is run.
 */
 
#include \"LibChemist/Defaults/NWXAtomicDensities.hpp\"
 
namespace LibChemist::detail_ {{
 
std::vector<std::vector<double>> {}(std::size_t Z) {{
    switch(Z) {{         
""".format(bs_name))

def print_basis_list(f):
    f.write(
"""/*
 * This file is autogenerated by generate_densities.py.  Any modifications made in
 * this file will be lost next time generate_densities.py is run.
 */
         
namespace LibChemist::detail_ {
""")

def print_basis_footer(f):
    tab = "    "
    f.write(
"""{}default : {{ 
{}throw std::out_of_range(\"Atomic density not available for Z\");
{}}}\n{}}} // end switch\n
}} //end function
}} //end LibChemist::detail_""".format(tab*2, tab*3, tab*2, tab))

def print_atom_basis(f, z, density):
    tab = "    "
    f.write("{}case({}) : {{\n{}return std::vector<std::vector<double>>{{\n".format(tab*2, z, tab*3))
    for line in density.lstrip().rstrip().splitlines():
        f.write("{}{{".format(tab*4))
        for x in line.split():
            f.write(" {},".format(x))
        f.write("},\n")
    f.write("{}}}; //End atomic density\n{}}} //End case\n".format(tab*3, tab*2))

def sanitize_name(bs_name):
    temp = bs_name.replace("6-", "six_dash_")
    temp = temp.replace("3-", "three_dash_")
    temp = temp.replace("-", "_dash_")
    temp = temp.replace("-", "_dash_")
    temp = temp.replace("+", "_plus_")
    return temp

def desanitize_name(bs_name):
    temp = bs_name.replace("_star", "*")
    temp = temp.replace("-", "-")
    return temp

def write_bases(out_dir, bases):
    tab = "    "
    with open(os.path.join(out_dir,"NWXAtomicDensities.cpp"),'w') as f:
        print_pimpl_header(f)
        with open(os.path.join(out_dir, "NWXAtomicDensities.hpp"), 'w') as g:
            print_basis_list(g)
            f.write("{}".format(tab*2))
            for bs_name, bs in sorted(bases.items()):
                s_name = sanitize_name(bs_name)
                d_name = desanitize_name(bs_name)
                f.write("if(name == \"{}\") {{ ".format(d_name))
                f.write("return {}(Z); ".format(s_name))
                g.write("std::vector<std::vector<double>> {}(std::size_t Z);\n".format(s_name))
                bs_file_name = "{}.cpp".format(bs_name)
                bs_path = os.path.join(out_dir,"atomic_densities", bs_file_name)
                with open(bs_path, 'w') as h:
                    print_basis_header(h, s_name)
                    for z in sorted([int(x) for x in bs.keys()]):
                        print_atom_basis(h, z, bs[str(z)])
                    print_basis_footer(h)
                f.write("}}\n{}else ".format(tab*2))
            g.write("} //end namespace\n")
        print_pimpl_footer(f)
    with open(os.path.join(out_dir,"atomic_densities", "AddDensity.cmake"), "w") as f:
        f.write("set(LIBCHEMIST_DENSITY_SOURCE\n")
        for bs_name, bs in sorted(bases.items()):
            f.write("    Defaults/atomic_densities/{}.cpp\n".format(bs_name))
        f.write(")")

def parse_bases(basis_sets, sym2Z):
    bases = {}
    for bs in basis_sets:
        bases[bs] = {}
        root = ET.parse(os.path.join("atomic_densities","default",bs+".xml"))
        for atom in root.findall('atomicguess'):
            atom_z = sym2Z[atom.get('symbol').lower()]
            bases[bs][atom_z] = atom.find('guessdensitymatrix').text
    return bases

def main():

    basis_sets = [f.replace(".xml","") for f in os.listdir("atomic_densities/default") if os.path.isfile(os.path.join("atomic_densities","default",f))]
    my_dir = os.path.dirname(os.path.realpath(__file__))
    out_dir = os.path.join(os.path.dirname(my_dir), "LibChemist",
                           "Defaults")
    atoms = parse_symbols(os.path.join(my_dir, "physical_data",
                                       "ElementNames.txt"), {})
    sym2Z = {ai.sym.lower() : ai.Z for ai in atoms.values()}
    bases = parse_bases(basis_sets, sym2Z)
    write_bases(out_dir, bases)


if __name__ == "__main__":
    main()
