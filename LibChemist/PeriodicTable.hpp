#pragma once
#include "LibChemist/Atom.hpp"
#include <memory> //For unique_ptr
#include <vector>

namespace LibChemist {
namespace detail_ {
/// Forward declaration of the PIMPL class
class PeriodicTablePIMPL;
} // namespace detail_

/**
 * @brief A class serving as a factory for Atom instances
 *
 * The main purpose of the PeriodicTable class is to facilitate the creation of
 * new Atom instances, which are populated with the default data for that atom.
 */
class PeriodicTable {
public:
    /// The type of a counting number
    using size_type = std::size_t;

    /// The type of a list of isotopes
    using isotope_list = std::vector<size_type>;

    /**
     * @brief Constructs a PTable loaded with the NWChemEx defaults.
     *
     * Default values for an NWChemEx PTable instance are populated using the
     * generate_atomicinfo.py script, which parses the accompanying data files
     * taken from IUPAC's CIAWW division.
     *
     * @throw std::bad_alloc if there is insufficient memory to allocate the
     *        PIMPL.  Strong throw guarantee.
     */
    PeriodicTable();

    /**
     * @defgroup Copy/Move CTors and Assignment Ops
     * @brief Functions for setting the state of the current PeriodicTable
     * instance off of another instance.
     *
     * All copies are deep copies.
     *
     * @param[in] rhs The instance to copy/move from.  For move operations @p
     * rhs is in a valid, but otherwise undefined state.
     *
     * @throw std::bad_alloc if the copy ctor/assignment operator has
     * insufficient memory to complete the operation.  Strong throw guarantee.
     * All move operations are no throw guarantee.
     */
    ///@{
    PeriodicTable(const PeriodicTable& rhs);
    PeriodicTable(PeriodicTable&& rhs) noexcept;
    PeriodicTable& operator=(const PeriodicTable& rhs);
    PeriodicTable& operator=(PeriodicTable&& rhs) noexcept;
    ///@}

    /// Default dtor
    ~PeriodicTable() noexcept;

    /**
     * @brief Returns the maximum atomic number for which an Atom instance can
     *        be made.
     *
     * For the most part creation of an Atom instance amounts to specifying the
     * mass and atomic symbol for the atom.  It is assumed that the
     * implementation amounts to a look up table, hence this data is hard
     * coded up to some value, which should be the return of this function.  It
     * should be noted that properties may not be available for all atomic
     * numbers in the range [1, max_Z()).
     *
     * @return The largest atomic number for which an Atom instance can be
     * made.
     *
     * @throw None no throw guarantee.
     */
    size_type max_Z() const noexcept;

    /**
     * @brief Returns a list of available mass numbers for a particular atom
     *
     * @param Z The atomic number of the atom, for which mass numbers are
     *        wanted.  Should be in range [0, max_Z())
     * @return A list of available mass numbers for the requested atom.  If no
     *         isotope data is available for the particular value of @p Z then
     *         the list is empty.
     * @throw std::bad_alloc if there is insufficient memory to allocate the
     *        list.  Strong throw guarantee.
     * @throw std::out_of_range if @p Z is not in the range [0, max_Z()).
     *        Strong throw guarantee.
     */
    isotope_list isotopes(size_type Z) const;

    /**
     * @defgroup Atom Creation Functions
     * @brief Returns an Atom instance pre-loaded with the data for the
     *        specified atomic number.
     *
     * There are two masses one can reasonably use for an atom: the abundance
     * weighted mass and the isotope mass.  We provide two different functions
     * to distinguish between these use cases.  `get_atom` returns the
     * abundance weighted variant, whereas `get_isotope` returns an Atom
     * whose mass is for a particular isotope.
     *
     * @param Z The atomic number of the atom to create.  Should be in the range
     *        [0, max_Z()).
     * @param mass_num The mass number of the isotope to create.
     * @return The requested atom as new instance.
     * @throw std::bad_alloc if there is insufficient memory to make the Atom
     *        instance.  Strong throw guarantee.
     * @throw std::out_of_range if @p Z is greater than max_Z() or if there is
     *        no isotope for a particular @p Z @p mass_num pair.  Strong throw
     *        guarantee.
     */
    ///@{
    Atom get_atom(size_type Z) const;
    Atom get_isotope(size_type Z, size_type mass_num) const;
    ///@}
private:
    /// The instance actually implementing the class.
    std::unique_ptr<detail_::PeriodicTablePIMPL> pimpl_;
};

} // namespace LibChemist
