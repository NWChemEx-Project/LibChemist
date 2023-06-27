/*
 * Copyright 2022 NWChemEx-Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include <chemist/basis_set/shell/shell_view.hpp>
#include <utilities/containers/indexable_container_base.hpp>

namespace chemist {
namespace detail_ {
template<typename ShellType>
class AtomicBasisSetPIMPL;
}

template<typename ShellType>
class AtomicBasisSet
  : public utilities::IndexableContainerBase<AtomicBasisSet<ShellType>> {
private:
    /// Type of this class
    using my_type = AtomicBasisSet<ShellType>;

    /// Type of the IndexableContainerBase base class
    using container_base = utilities::IndexableContainerBase<my_type>;

public:
    /// Type of the PIMPL
    using pimpl_type = detail_::AtomicBasisSetPIMPL<ShellType>;

    /// Type of a pointer to a PIMPL
    using pimpl_pointer = std::unique_ptr<pimpl_type>;

    /// String-like type used to store the basis set name
    using name_type = std::string;

    /// Mutable reference to the basis set's name
    using name_reference = name_type&;

    /// Read-only reference to the basis set's name
    using const_name_reference = const name_type&;

    /// Unsigned integral type used to store the atomic number
    using atomic_number_type = std::size_t;

    /// Mutable reference to the basis set's atomic number
    using atomic_number_reference = atomic_number_type&;

    /// Read-only reference to the basis set's atomic number
    using const_atomic_number_reference = const atomic_number_type&;

    /// Unsigned integral type used for indexing/offsets
    using size_type = typename container_base::size_type;

    // -------------------------------------------------------------------------
    // -- Shell types
    // -------------------------------------------------------------------------

    /// Type of the Shells on this AtomicBasisSet
    using value_type = ShellType;

    /// Type of a read-/write-able reference to a AtomicBasisSet
    using reference = ShellView<value_type>;

    /// Type of a read-only reference to a AtomicBasisSet
    using const_reference = ShellView<const value_type>;

    /// Type used to specify angular momentum
    using angular_momentum = typename value_type::angular_momentum;

    /// Type used to specify whether a Shell is pure or not
    using pure_type = typename value_type::pure_type;

    /// Type of a reference to a contracted Gaussian function
    using contracted_gaussian_reference =
      typename value_type::contracted_gaussian_reference;

    // -------------------------------------------------------------------------
    // -- Primitive types
    // -------------------------------------------------------------------------

    using primitive_reference = typename value_type::primitive_reference;

    using const_primitive_reference =
      typename value_type::const_primitive_reference;

    using center_type = typename primitive_reference::center_type;

    using coefficient_type = typename primitive_reference::coefficient_type;

    using exponent_type = typename primitive_reference::exponent_type;

    using coord_type = typename center_type::coord_type;

    // -------------------------------------------------------------------------
    // -- Ctors, assignment, and dtor
    // -------------------------------------------------------------------------

    /** @brief Creates a null AtomicBasisSet object.
     *
     *  @throw None No throw guarantee
     */
    AtomicBasisSet();

    /** @brief Creates a new AtomicBasisSet by deep copying another instance.
     *
     *  @param[in] rhs The AtomicBasisSet instance to deep copy.
     *
     *  @throw std::bad_alloc if there is insufficient memory to create the new
     *                        instance. Strong throw guarantee.
     */
    AtomicBasisSet(const AtomicBasisSet<T>& rhs);

    /** @brief Creates a new AtomicBasisSet instance by taking ownership of an
     *         already existing instance's state.
     *
     *  @param[in, out] rhs The instance to take the state from. After this ctor
     *                      @p rhs will no longer contain a PIMPL and can not
     *                      be used until a AtomicBasisSet instance with a valid
     *                      PIMPL is copy/move assigned to @p rhs.
     *
     *  @throw None no throw guarantee.
     */
    AtomicBasisSet(AtomicBasisSet<T>&& rhs) noexcept;

    /** @brief Causes the current AtomicBasisSet to contain a deep copy of
     *         another AtomicBasisSet.
     *
     *  This function will assign to the current AtomicBasisSet a deep copy of
     *  another instance. This instance's previous state will be released.
     *
     *  @param[in] rhs The AtomicBasisSet instance to deep copy.
     *
     *  @return The current instance after setting it to a deep copy of @p rhs.
     *
     *  @throw std::bad_alloc if there is insufficient memory to create the new
     *                        instance. Strong throw guarantee.
     */
    AtomicBasisSet<T>& operator=(const AtomicBasisSet<T>& rhs);

    /** @brief Causes the current AtomicBasisSet instance to take ownership of
     *         an already existing instance's state.
     *
     *  @param[in, out] rhs The instance to take the state from. After this ctor
     *                      @p rhs will no longer contain a PIMPL and can not
     *                      be used until a AtomicBasisSet instance with a valid
     *                      PIMPL is copy/move assigned to @p rhs.
     *
     *  @return The current instance after taking ownership of @p rhs's state.
     *
     *  @throw None no throw guarantee.
     */
    AtomicBasisSet<T>& operator=(AtomicBasisSet<T>&& rhs) noexcept;

    /** @brief Creates a new AtomicBasisSet centered on the provided point.
     *
     *  This ctor is used to create a new AtomicBasisSet instance with the
     *  provided Cartesian coordinates.
     *
     *  @param[in] name The name associated with the basis set.
     *  @param[in] atomic_n The atomic number associated with the basis set.
     *  @param[in] x The x-coordinate for the resulting AtomicBasisSet
     *  @param[in] y The y-coordinate for the resulting AtomicBasisSet
     *  @param[in] z The z-coordinate for the resulting AtomicBasisSet
     *
     *  @throw std::bad_alloc if there is insufficient memory to create the
     *                        PIMPL. Strong throw guarantee.
     */
    AtomicBasisSet(const_name_reference name, atomic_number_type atomic_n,
                   coord_type x, coord_type y, coord_type z);

    /** @brief Creates a new AtomicBasisSet centered on the provided point.
     *
     *  This ctor is used to create a new AtomicBasisSet instance with the
     *  provided Cartesian coordinates, while the basis set name and atomic
     *  number are defaulted.
     *
     *  @param[in] x The x-coordinate for the resulting AtomicBasisSet
     *  @param[in] y The y-coordinate for the resulting AtomicBasisSet
     *  @param[in] z The z-coordinate for the resulting AtomicBasisSet
     *
     *  @throw std::bad_alloc if there is insufficient memory to create the
     *                        PIMPL. Strong throw guarantee.
     */
    AtomicBasisSet(coord_type x, coord_type y, coord_types z);

    /** @brief Creates a new AtomicBasisSet with the provided name and atomic
     *         number.
     *
     *  This ctor is used to create a new AtomicBasisSet instance, centered
     *  on the origin, with the provided basis set name and atomic number.
     *
     *  @param[in] name The name associated with the basis set.
     *  @param[in] atomic_n The atomic number associated with the basis set.
     *
     *  @throw std::bad_alloc if there is insufficient memory to create the
     *                        PIMPL. Strong throw guarantee.
     */
    AtomicBasisSet(const_name_reference name, size_type atomic_n);

    /// Defaulted no throw dtor
    ~AtomicBasisSet() noexcept override;

    /** @brief Adds a shell to the center.
     *
     *  This function will create a Shell instance, with the specified
     *  parameters, on the current AtomicBasisSet.
     *
     *  @param[in] pure Whether the new shell is pure or not.
     *  @param[in] l The total angular momentum of the new shell.
     *  @param[in] cs The contraction coefficients for the primitives comprising
     *                the new shell's CGTO.
     *  @param[in] es The exponents for the primitives comprising the new
     *                shell's CGTO.
     *
     *  @throw std::bad_alloc if there is insufficient memory to create the new
     *                        Shell. Weak throw guarantee.
     */
    void add_shell(pure_type pure, am_type l, param_set cs, param_set es);

    /** @brief Returns the name of the basis set. */
    std::string basis_set_name() const;

    /** @brief Returns the atomic number of the basis set. */
    size_type atomic_number() const;

    /// The type of the AOs comprising a shell
    using ao_type = typename value_type::value_type;
    /// A read-/write-able reference to an AO
    using ao_reference = typename value_type::reference;
    /// A read-only reference to an AO
    using const_ao_reference = typename value_type::const_reference;

    /** @brief Returns the total number of AOs on this center.
     *
     *  Each shell is comprised of AOs. This function will add up the total
     *  number of AOs in each shell.
     *
     *  @return The total number of AOs on this center.
     *
     *  @throw None no throw gurantee.
     */
    size_type n_aos() const noexcept;

    /** @brief Returns the @p i-th AO on the center.
     *
     *  AOs on the center are numbered by flattening out the shells. In other
     *  words, if the first shell has n AOs, the first n AOs are the n AOs of
     *  that shell. The n + 1-th AO is then the 0-th AO of the second shell and
     *  so on.
     *
     *  @param[in] i The index of the requested AO. Must be in the range [0,
     *                n_aos()).
     *
     *  @return A read-/write-able reference to the requested AO
     *
     *  @throw std::out_of_range if @p i is not in the range [0, n_aos()).
     *                            Strong throw guarantee.
     *  @throw std::bad_alloc if there is insufficient memory to create the
     *                        PIMPL for the resulting view. Strong throw
     *                        guarantee.
     *
     */
    ao_reference ao(size_type i);

    /** @brief Returns the @p i-th AO on the center.
     *
     *  AOs on the center are numbered by flattening out the shells. In other
     *  words, if the first shell has n AOs, the first n AOs are the n AOs of
     *  that shell. The n + 1-th AO is then the 0-th AO of the second shell and
     *  so on.
     *
     *  @param[in] i The index of the requested AO. Must be in the range [0,
     *                n_aos()).
     *
     *  @return A read-only reference to the requested AO.
     *
     *  @throw std::out_of_range if @p i is not in the range [0, n_aos()).
     *                            Strong throw guarantee.
     *  @throw std::bad_alloc if there is insufficient memory to create the
     *                        PIMPL for the resulting view. Strong throw
     *                        guarantee.
     *
     */
    const_ao_reference ao(size_type i) const;

    /// The type of a Primitive Gaussian used in an AO
    using primitive_type = typename value_type::primitive_type;
    /// The type of a read-/write-able reference to a primitive
    using primitive_reference = typename value_type::primitive_reference;
    /// The type of a read-only reference to a primitive
    using const_primitive_reference =
      typename value_type::const_primitive_reference;

    /** @brief Computes the total number of unique primitives on the center.
     *
     *  It is common for shells to only store the unique primitives (i.e. for
     *  the AOs in a shell to use the same primitives). This function sums up
     *  the total number of unique primitives each Shell on this center has.
     *
     *  @return The total number of unique primitives on this center.
     *
     *  @throw None No throw guarantee.
     */
    size_type n_unique_primitives() const noexcept;

    /** @brief Returns the @p i-th unique primitive on the center.
     *
     *  Primitives on the center are numbered by flattening out the shells and
     *  then flattening out the unique primitives comprising the AOs. Typically
     *  there is only one set of unique primitives per shell, nevertheless this
     *  function encapsulates that detail.
     *
     *  @param[in] i The index of the requested primitive. Must be in the range
     *               [0, n_unique_primitives()).
     *
     *  @return A read-/write-able reference to the requested primitive
     *
     *  @throw std::out_of_range if @p i is not in the range [0, n_aos()).
     *                            Strong throw guarantee.
     *  @throw std::bad_alloc if there is insufficient memory to create the
     *                        PIMPL for the resulting view. Strong throw
     *                        guarantee.
     *
     */
    primitive_reference unique_primitive(size_type i);

    /** @brief Returns the @p i-th unique primitive on the center.
     *
     *  Primitives on the center are numbered by flattening out the shells and
     *  then flattening out the unique primitives comprising the AOs. Typically
     *  there is only one set of unique primitives per shell, nevertheless this
     *  function encapsulates that detail.
     *
     *  @param[in] i The index of the requested primitive. Must be in the range
     *               [0, n_unique_primitives()).
     *
     *  @return A read-only reference to the requested primitive
     *
     *  @throw std::out_of_range if @p i is not in the range [0, n_aos()).
     *                            Strong throw guarantee.
     *  @throw std::bad_alloc if there is insufficient memory to create the
     *                        PIMPL for the resulting view. Strong throw
     *                        guarantee.
     *
     */
    const_primitive_reference unique_primitive(size_type i) const;

    /** @brief Serialize AtomicBasisSet instance
     *
     * @param ar The archive object
     */
    template<typename Archive>
    void save(Archive& ar) const {
        ar& n_aos() & n_unique_primitives() & this->size() & this->coord(0) &
          this->coord(1) & this->coord(2);
        for(const auto& s : *this) {
            ar& bool(s.pure()) & s.l() & s.size() & s[0].size();
            for(const auto& p : s[0]) { ar& p.coefficient() & p.exponent(); }
        }
    }

    /** @brief Deserialize for AtomicBasisSet instance
     *
     * @param ar The archive object
     */
    template<typename Archive>
    void load(Archive& ar) {
        size_type np, ns, nc;
        bool ispure;
        am_type myl;
        ar& n_aos() & n_unique_primitives() & ns& this->coord(0) &
          this->coord(1) & this->coord(2);
        for(int si = 0; si < ns; ++si) {
            ar& ispure& myl& nc& np;
            std::vector<T> cs(np, 0);
            std::vector<T> es(np, 0);
            for(int pi = 0; pi < np; ++pi) { ar& cs[pi] & es[pi]; }
            add_shell(pure_type(ispure), myl, cs, es);
        }
    }

private:
    /// Allows the IndexableContainerBase to access implementations
    friend container_base;

    /** @brief Creates a new AtomicBasisSet with the provided state.
     *
     *  This ctor can be used to create a AtomicBasisSet which uses the provided
     *  PIMPL.
     *
     *  @param[in] pimpl The PIMPL to use for the AtomicBasisSet part of the
     *                    resulting instance.
     *
     *  @throw None no throw guarantee.
     */
    AtomicBasisSet(pimpl_pointer pimpl) noexcept;

    /// Implements size()
    size_type size_() const noexcept;

    /// Implements non-const version of operator[]/at
    reference at_(size_type i);

    /// Implements const version of operator[]/at
    const_reference at_(size_type i) const;

    /// The instance that actually implements this class
    center_pimpl_ptr_t m_pimpl_;
};

/** @brief Compares two AtomicBasisSet instances for equality.
 *
 *  Two AtomicBasisSet instances are considered equal if they contain the
 *  same number of shells and if the i-th shell of @p lhs is equal to
 *  the i-th shell of @p rhs for all i in the range `[0, lhs.size())`.
 *
 *  @tparam T The type used to hold the AtomicBasisSet's parameters.
 *
 *  @param[in] lhs The instance on the left side of the operator.
 *  @param[in] rhs The instance on the right side of the operator.
 *
 *  @return True if the the two instances are equal and false otherwise.
 *
 *  @throw std::bad_alloc if there is insufficient memory to make the PIMPL
 * for one of the ShellView instances. Strong throw guarantee.
 */
template<typename T>
bool operator==(const AtomicBasisSet<T>& lhs, const AtomicBasisSet<T>& rhs) {
    using base_t = utilities::IndexableContainerBase<AtomicBasisSet<T>>;
    return static_cast<const base_t&>(lhs) == static_cast<const base_t&>(rhs);
}

/** @brief Determines if two AtomicBasisSet instances are different.
 *
 *  Two AtomicBasisSet instances are considered equal if they contain the
 *  same number of shells and if the i-th shell of @p lhs is equal to
 *  the i-th shell of @p rhs for all i in the range `[0, lhs.size())`.
 *  Different is defined as not equal.
 *
 *  @tparam T The type used to hold the AtomicBasisSet's parameters.
 *
 *  @param[in] lhs The instance on the left side of the operator.
 *  @param[in] rhs The instance on the right side of the operator.
 *
 *  @return False if the the two instances are equal and true otherwise.
 *
 *  @throw std::bad_alloc if there is insufficient memory to make the PIMPL
 * for one of the ShellView instances. Strong throw guarantee.
 */
template<typename T>
bool operator!=(const AtomicBasisSet<T>& lhs, const AtomicBasisSet<T>& rhs) {
    return !(lhs == rhs);
}

extern template class AtomicBasisSet<double>;
extern template class AtomicBasisSet<float>;

} // namespace chemist