#include <chemist/fragmenting/fragmented_chemical_system.hpp>

namespace chemist::fragmenting {
namespace detail_ {

template<typename ChemicalSystemType>
class FragmentedChemicalSystemPIMPL {
private:
    /// Type of *this
    using my_type = FragmentedChemicalSystemPIMPL<ChemicalSystemType>;

public:
    /// Type *this implements
    using parent_type = FragmentedChemicalSystem<ChemicalSystemType>;

    /// Pull in types from parent_type
    ///@{
    using fragmented_molecule_type =
      typename parent_type::fragmented_molecule_type;
    using supersystem_reference = typename parent_type::supersystem_reference;
    using const_supersystem_reference =
      typename parent_type::const_supersystem_reference;
    using size_type = typename parent_type::size_type;
    ///@}

    auto operator[](size_type i) { return supersystem_reference(m_frags_[i]); }
    const auto operator[](size_type i) const {
        return const_supersystem_reference(m_frags_[i]);
    }
    size_type size() const noexcept { return m_frags_.size(); }

    auto supersystem() { return supersystem_reference(m_frags_.supersystem()); }
    auto supersystem() const {
        return const_supersystem_reference(m_frags_.supersystem());
    }

private:
    fragmented_molecule_type m_frags_;
};

} // namespace detail_

#define TPARAMS template<typename ChemicalSystemType>
#define FRAGMENTED_CHEMICAL_SYSTEM FragmentedChemicalSystem<ChemicalSystemType>

// -----------------------------------------------------------------------------
// -- Ctors, assignment, and dtor
// -----------------------------------------------------------------------------

TPARAMS
FRAGMENTED_CHEMICAL_SYSTEM::FragmentedChemicalSystem() noexcept = default;

TPARAMS
FRAGMENTED_CHEMICAL_SYSTEM::~FragmentedChemicalSystem() noexcept = default;

// -----------------------------------------------------------------------------
// -- Utility methods
// -----------------------------------------------------------------------------

TPARAMS
void FRAGMENTED_CHEMICAL_SYSTEM::swap(
  FragmentedChemicalSystem& other) noexcept {
    m_pimpl_.swap(other.m_pimpl_);
}

TPARAMS
bool FRAGMENTED_CHEMICAL_SYSTEM::operator==(
  const FragmentedChemicalSystem& rhs) const noexcept {
    if(this->supersystem() != rhs.supersystem()) return false;
    if(this->size() != rhs.size()) return false;
    if(this->size() == 0) return true; // Both empty and have same super sys
    for(size_type i = 0; i < this->size(); ++i)
        if((*m_pimpl_)[i] != (*rhs.m_pimpl_)[i]) return false;
    return true;
}

TPARAMS
bool FRAGMENTED_CHEMICAL_SYSTEM::operator!=(
  const FragmentedChemicalSystem& rhs) const noexcept {
    return !((*this) == rhs);
}

// -----------------------------------------------------------------------------
// -- Protected methods
// -----------------------------------------------------------------------------

TPARAMS
typename FRAGMENTED_CHEMICAL_SYSTEM::reference FRAGMENTED_CHEMICAL_SYSTEM::at_(
  size_type i) {
    return (*m_pimpl_)[i];
}

TPARAMS
typename FRAGMENTED_CHEMICAL_SYSTEM::const_reference
FRAGMENTED_CHEMICAL_SYSTEM::at_(size_type i) const {
    return std::as_const(*m_pimpl_)[i];
}

TPARAMS
typename FRAGMENTED_CHEMICAL_SYSTEM::size_type
FRAGMENTED_CHEMICAL_SYSTEM::size_() const noexcept {
    return has_pimpl_() ? m_pimpl_->size() : 0;
}

TPARAMS
typename FRAGMENTED_CHEMICAL_SYSTEM::supersystem_reference
FRAGMENTED_CHEMICAL_SYSTEM::supersystem_() {
    return has_pimpl_() ? m_pimpl_->supersystem() : supersystem_reference{};
}

TPARAMS
typename FRAGMENTED_CHEMICAL_SYSTEM::const_supersystem_reference
FRAGMENTED_CHEMICAL_SYSTEM::supersystem_() const {
    return has_pimpl_() ? std::as_const(*m_pimpl_).supersystem() :
                          const_supersystem_reference{};
}

// -----------------------------------------------------------------------------
// -- Private methods
// -----------------------------------------------------------------------------

TPARAMS
bool FRAGMENTED_CHEMICAL_SYSTEM::has_pimpl_() const noexcept {
    return static_cast<bool>(m_pimpl_);
}

#undef FRAGMENTED_CHEMICAL_SYSTEM
#undef TPARAMS

template class FragmentedChemicalSystem<ChemicalSystem>;
template class FragmentedChemicalSystem<const ChemicalSystem>;

} // namespace chemist::fragmenting
