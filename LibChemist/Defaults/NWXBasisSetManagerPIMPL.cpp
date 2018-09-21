
/*
 * This file is autogenerated by generate_basis.py.  Any modifications made in
 * this file will be lost next time generate_basis.py is run.
 */
       
#include "LibChemist/Implementations/BasisSetManagerPIMPL.hpp"
#include "LibChemist/Defaults/NWXBasisList.hpp"
        
namespace LibChemist::detail_ {
       
class HardCodedBSMan : public BasisSetManagerPIMPL {
public:
    HardCodedBSMan() = default;
protected:
    HardCodedBSMan(const HardCodedBSMan& rhs) = default;
private:
    std::unique_ptr<BasisSetManagerPIMPL> clone_() const override {
        return std::unique_ptr<HardCodedBSMan>(new HardCodedBSMan(*this));
    }
            
    AOBasisSet get_basis_(const std::string& name, size_type Z) const override {         
        if(name == "cc-pvdz") { return cc_dash_pvdz(Z); }
        else if(name == "sto-3g") { return sto_dash_3g(Z); }
        else if(name == "cc-pvdz-jkfit") { return cc_dash_pvdz_dash_jkfit(Z); }
        else throw std::out_of_range("Unrecognized basis name");
    }//end get_basis_
};
        
std::unique_ptr<BasisSetManagerPIMPL> nwx_default_bs() {
    return std::make_unique<HardCodedBSMan>();
}
        
} // namespace LibChemist::detail_
