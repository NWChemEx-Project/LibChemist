"""
This script will turn each of the xyz files in "molecules" into the C++ commands
to make said molecule in a source file
"""
import os
from helper_fxns import *

class molecule:
    def __init__(self,name):
        self.name = name
        self.carts = []
        self.atoms = []
    def cxxify(self, f):
        f.write("    molecule {:s};\n".format(self.name))
        indent = "        "
        for i in range(len(self.atoms)):
            f.write("    {\n")
            f.write(indent + "auto temp = wikipedia.atomic_info(\"{"
                             ":s}\");\n".format(self.atoms[i]))
            f.write(indent + "temp.coord = std::array<double,3>( \n")
            f.write(indent + "    {")
            for j in range(2):
                f.write("{:s},".format(self.carts[3*i+j]))
            f.write("{:s};\n".format(self.carts[3*i+2]+'})'))
            f.write(indent + "{:s}.insert(temp);\n".format(self.name))
            f.write("    }\n")
        f.write("    rv[\"{0:s}\"] = {0:s};\n".format(self.name))

def main():
    mols = ["water"]
    my_path = os.path.dirname(os.path.realpath(__file__))
    data_dir = os.path.join(os.path.dirname(my_path), "LibChemist",
                            "Defaults")
    with open(os.path.join(data_dir, "DefaultMols.cpp"),'w') as f:
        f.write("#include \"SDE/MoleculeFileParser.hpp\"\n")
        f.write("#include <sstream>\n")
        write_header(__file__, f)
        f.write("using return_t = typename "
                "ChemistryRuntime::molecule_lut_type;\n")
        f.write("return_t default_molecules() {\n")
        f.write("    return_t rv;\n")
        f.write("    ChemistryRuntime crt{{}, {}, {}, default_atoms(), "
                " {}, default_symbols(), {}};\n")
        f.write("    XYZParser xyz;\n")
        for m in mols:
            f.write("    std::stringstream {}(\n".format(m))
            with open(os.path.join(my_path, "molecules","{}.xyz".format(m)),
                      'r') as g:
                    for l in g:
                        f.write("    \"{}\\n\"\n".format(l.rstrip()))
            f.write("    );\n")
            f.write("    rv[\"{0}\"] = parse_molecule_file({0}, ".format(m))
            f.write("xyz, crt);\n")
        write_footer(f)

if __name__ == "__main__" :
    main()

