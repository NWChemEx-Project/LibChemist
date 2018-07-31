# #!/usr/bin/env python3
import os
import re

"""This script is used to create the experimental data look up tables for the
atom class.

Original author: Ben Pritchard

In order to run this script simply needs to know where you want the generated
files to live.

For readability and convenience we use a few abbreviations throughout this
script:

- Z the atomic number of an atom
- Sym the atomic symbol of an atom (e.g. H for hydrogen, He for helium)

"""

class AtomicData:
    def __init__(self):
        self.sym = ""
        self.name = ""
        self.Z = 0
        self.isotopes = []
        self.mass = 0.0
        self.isotope_masses = {}

    def add_isotope(self, num, mass):
        self.isotope_masses[num] = mass
        if not num in self.isotopes:
            self.isotopes.append(num)

    def __repr__(self):
        rv = "{} {} {}[".format(self.Z, self.name, self.mass)
        for x in self.isotopes:
            rv +="{}: {}".format(x, self.isotope_masses[x])
            if x!= self.isotopes[-1]:
                rv+=", "
        rv += "]"
        return rv


def parse_symbols(name_file, atoms):
    for line in open(name_file).readlines():
        z, sym, name = line.strip().split()
        if not z in atoms:
            atoms[z] = AtomicData()
        atoms[z].sym = sym
        atoms[z].name = name
        atoms[z].Z = z
    return atoms

def parse_isotopes(iso_file, atoms):
    new_atom =  r"(\d{1,2})\s+[a-zA-Z]{1,2}\s+[a-zA-Z]+\s+"
    new_iso = r"(\d+)\**\s+(\d+\.\d+)\s((\d+\s?)+)+"
    new_atom+=new_iso
    def par_fxn(match, atom):
        as_str = match[1].replace(" ", "") + match[2].replace(" ", "")
        atom.add_isotope(match[0], float(as_str))
    with open(iso_file, 'r') as f:
        Z = 0
        for line in f.readlines():
            if re.match( new_atom, line):
                match = re.match(new_atom, line).groups()
                Z = match[0]
                par_fxn(match[1:], atoms[Z])
            elif re.match(new_iso, line):
                match = re.match(new_iso, line).groups()
                par_fxn(match, atoms[Z])
    return atoms

def parse_mass(mass_file, atoms):
    mass = r"((?:\d+\.\d+(?:\s\d+)*,*\s?)+)"
    with open(mass_file, 'r') as f:
        for line in f:
            if re.search(mass, line):
                Z = line.split()[0]
                masses = re.search(mass, line).groups()[0].replace(" ","")
                if len(masses.split(',')) == 2: # Has error bars
                    temp = masses.split(',')
                    atoms[Z].mass = (float(temp[0]) + float(temp[1]))/2.0
                else:
                    atoms[Z].mass = float(masses)
    return atoms

def write_ptable(out_dir, amu2me, atoms):
    out_file = os.path.join(out_dir, "NWXPeriodicTablePIMPL.cpp")
    in_file = "LibChemist/Implementations/PeriodicTablePIMPL.hpp"
    sorted_keys = sorted([int(x) for x in atoms.keys()])
    max_Z = sorted_keys[-1]
    tab = "    "
    with open(out_file, 'w') as f:
        f.write(
"""/* 
 * This file has been autogenerated by generate_atomicinfo.py.  Any 
 * modifications made to this file will be lost next time generate_atomicinfo.py
 * is run.
 */

#include \"{}\"
        
namespace LibChemist::detail_ {{

class NWXPeriodicTablePIMPL : public PeriodicTablePIMPL {{
public:
    NWXPeriodicTablePIMPL() = default;
protected:
    using my_type = NWXPeriodicTablePIMPL;
    NWXPeriodicTablePIMPL(const my_type& rhs) = default;
private:
    std::unique_ptr<PeriodicTablePIMPL> clone_() const override {{
        return std::unique_ptr<my_type>(new my_type(*this));
    }}            
            
    size_type max_Z_() const noexcept override {{
        return {};
    }}
            
    Atom get_atom_(size_type Z) const override {{
        switch(Z) {{
""".format(in_file, max_Z))
        for k in sorted_keys:
            ai = atoms[str(k)]
            f.write("{}case({}): {{ return Atom(".format(tab*3, k))
            f.write("{}ul, {}, \"{}\");}}\n".format(k, ai.mass*amu2me, ai.sym))

        f.write(
"""            default : {{ throw std::out_of_range(\" Z > {}\"); }}
        }}
    }}
            
    Atom get_isotope_(size_type Z, size_type mass_num) const override {{
        switch(Z) {{
""".format(max_Z))

        for k in sorted_keys:
            f.write("{}case({}): {{\n".format(tab*3, k)) #Start case 1
            f.write("{}switch(mass_num) {{\n".format(tab*4)) #Start switch 2
            ai = atoms[str(k)]
            for mn in sorted([int(x) for x in ai.isotopes]):
                mi = ai.isotope_masses[str(mn)] * amu2me
                f.write("{}case({}): {{ return Atom(".format(tab*5, mn))
                f.write("{}ul, {}, \"{}\");}}\n".format(k, mi, ai.sym))
            f.write("{}}}\n".format(tab*4)) #Close switch 2
            f.write("{}}}\n".format(tab*3)) #Close case 1
        f.write(
"""            default : { throw std::out_of_range(\"No isotope data\"); }
        }
    }
    
    isotope_list isotopes_(size_type Z) const override {
        switch(Z) {
""")
        for k in sorted_keys:
            f.write("{}case({}) : {{ return {{".format(tab*3, k))
            ai = atoms[str(k)]
            for mn in sorted([int(x) for x in ai.isotopes]):
                f.write("{}, ".format(mn))
            f.write("}; }\n")
        f.write(
"""            default : {{ throw std::out_of_range(\" Z > {}\"); }}
        }}
    }}        
}};
        
std::unique_ptr<PeriodicTablePIMPL> nwx_default_ptable() {{
    return std::make_unique<NWXPeriodicTablePIMPL>();
}}
}} // namespace LibChemist::detail_
""".format(max_Z))

def write_tests(out_dir, amu2me, atoms):
    sorted_keys = sorted([int(x) for x in atoms.keys()])
    max_Z = sorted_keys[-1]
    with open(os.path.join(out_dir, "TestPeriodicTable.cpp"), 'w') as f:
        f.write(
"""/*
* This file has been autogenerated by generate_atomicinfo.py.  Any changes made
* to it will be lost next time generate_atomicinfo.py is run.
*/

#include <LibChemist/PeriodicTable.hpp>
#include <catch/catch.hpp>

using namespace LibChemist;
using size_type = typename PeriodicTable::size_type;
using isotope_list = typename PeriodicTable::isotope_list;

void test_ptable(const PeriodicTable& ptable) {{
    REQUIRE(ptable.max_Z() == {});
""".format(max_Z))
        for Z in range(1, max_Z + 1):
            ai = atoms[str(Z)]
            f.write("    REQUIRE(ptable.get_atom({}) == Atom{{{}, \"{}\", "
                    "{}ul}});\n".format(Z, ai.mass*amu2me, ai.sym, Z))
            f.write("    REQUIRE(ptable.isotopes({}) == isotope_list{{"
                    "".format(Z))
            sorted_mn = sorted([int(mn) for mn in ai.isotopes])
            for mn in sorted_mn:
                f.write("{}, ".format(mn))
            f.write("});\n")
            for mn in sorted_mn:
                mi = ai.isotope_masses[str(mn)]*amu2me
                f.write("    REQUIRE(ptable.get_isotope({}, {}) == Atom{{{}ul, "
                        "\"{}\", {}}});\n".format(Z, mn, Z, ai.sym, mi))
        f.write(
"""}

TEST_CASE("PeriodicTable Class") {
    SECTION("Typedefs") {
        REQUIRE(std::is_same_v<size_type, std::size_t>);
        REQUIRE(std::is_same_v<isotope_list, std::vector<size_type>>);
    }
    
    PeriodicTable ptable;
    SECTION("Default CTor") { 
        test_ptable(ptable); 
    }
    SECTION("Copy CTor") {
        PeriodicTable ptable2(ptable); 
        test_ptable(ptable2);  
    }
    SECTION("Copy assign") {
        PeriodicTable ptable2;
        auto& pptable = (ptable2 = ptable);
        test_ptable(ptable2);
        REQUIRE(&pptable == &ptable2);
    }
    SECTION("Move CTor") {
        PeriodicTable ptable2(std::move(ptable)); 
        test_ptable(ptable2); 
    }
    SECTION("Move assign") {
        PeriodicTable ptable2;
        auto& pptable = (ptable2 = std::move(ptable));
        test_ptable(ptable2);
        REQUIRE(&pptable == &ptable2);
    }
}
""")

def main(dest_dir, amu2me):
    # Get and set some paths
    my_dir =os.path.dirname(os.path.realpath(__file__))
    data_dir = os.path.join(my_dir, "physical_data") #Dir w/ files
    out_dir = os.path.join(os.path.dirname(my_dir), dest_dir) #Dir to write to
    name_file = os.path.join(data_dir, "ElementNames.txt")
    iso_file = os.path.join(data_dir, "CIAAW-ISOTOPEMASSES.txt")
    mass_file = os.path.join(data_dir, "CIAAW-MASSES.txt")
    #cov_file = os.path.join(data_dir, "CovRadii.txt")
    #vdw_file = os.path.join(data_dir, "VanDerWaalRadius.txt")
    #mult_file = os.path.join(data_dir, "NIST-ATOMICION.formatted.txt")

    atoms = parse_symbols(name_file, {})
    atoms = parse_isotopes(iso_file, atoms)
    atoms = parse_mass(mass_file, atoms)

    write_ptable(out_dir, amu2me, atoms)
    write_tests(os.path.join(os.path.dirname(my_dir), "LibChemist_Test"),
                amu2me, atoms)

if __name__ == '__main__' :
    # Knobs that you may want to tweak to control settings
    amu2me = 1822.888486192 # ratio of mass of electron to a Dalton
    dest_dir = os.path.join("LibChemist", "Defaults") #Directory 4 output
    main(dest_dir, amu2me)

