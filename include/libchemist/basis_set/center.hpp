#pragma once
#include "libchemist/basis_set/shell_view.hpp"
#include "libchemist/point/point.hpp"
#include <utilities/containers/indexable_container_base.hpp>

namespace libchemist {
namespace detail_ {
template<typename T>
class CenterPIMPL;
}
template<typename T>
class Center : public Point<T>,
               public utilities::IndexableContainerBase<Center<T>> {
private:
    /// Type of this class
    using my_type = Center<T>;
    /// Type of the IndexableContainerBase base class
    using container_base = utilities::IndexableContainerBase<my_type>;
    /// Type of the PIMPL implementing the center part
    using center_pimpl_t = detail_::CenterPIMPL<T>;
    /// Type of a pointer to the PIMPL for the center
    using center_pimpl_ptr_t = std::unique_ptr<center_pimpl_t>;
    /// Type of the PIMPL for the Point part
    using point_pimpl_t = detail_::PointPIMPL<T>;
    /// Type of a pointer to the PIMPL for the Point part
    using point_pimpl_ptr_t = std::unique_ptr<point_pimpl_t>;

public:
    /// Type of the Shells on this Center
    using value_type = Shell<T>;
    /// Type of a read-/write-able reference to a Center
    using reference = ShellView<T>;
    /// Type of a read-only reference to a Center
    using const_reference = ShellView<const T>;
    /// Unsigned integral type used for indexing/offsets
    using size_type = typename container_base::size_type;
    /// Type used to specify whether a Shell is pure or not
    using pure_type = ShellType;
    /// Type used to store the total angular momentum of a shell
    using am_type = size_type;
    /// Type used to specify the coefficients/exponets for a CTGO
    using param_set = std::vector<T>;

    /** @brief Creates a new Center instance at the origin.
     *
     *  The Center instance resulting from this ctor will have no shells and
     *  will be centered at the origin. The center can be translated by
     *  modifying the coordinates through `coord(size_type)` or the `x()`,
     *  `y()`, and `z()` member functions. Shells can be added via `add_shell`.
     *
     *  @throw std::bad_alloc if there is insufficient memory to allocate the
     *                        PIMPL. Strong throw guarantee.
     */
    Center();

    /** @brief Creates a new Center by deep copying another instance.
     *
     *  @param[in] rhs The Center instance to deep copy.
     *
     *  @throw std::bad_alloc if there is insufficient memory to create the new
     *                        instance. Strong throw guarantee.
     */
    Center(const Center<T>& rhs);

    /** @brief Creates a new Center instance by taking ownership of an already
     *         existing instance's state.
     *
     *  @param[in, out] rhs The instance to take the state from. After this ctor
     *                      @p rhs will no longer contain a PIMPL and can not
     *                      be used until a Center instance with a valid PIMPL
     *                      is copy/move assigned to @p rhs.
     *
     *  @throw None no throw guarantee.
     */
    Center(Center<T>&& rhs) noexcept;

    /** @brief Causes the current Center to contain a deep copy of another
     *         Center.
     *
     *  This function will assign to the current Center a deep copy of another
     *  instance. This instance's previous state will be released.
     *
     *  @param[in] rhs The Center instance to deep copy.
     *
     *  @return The current instance after setting it to a deep copy of @p rhs.
     *
     *  @throw std::bad_alloc if there is insufficient memory to create the new
     *                        instance. Strong throw guarantee.
     */
    Center<T>& operator=(const Center<T>& rhs);

    /** @brief Causes the current Center instance to take ownership of an
     * already existing instance's state.
     *
     *  @param[in, out] rhs The instance to take the state from. After this ctor
     *                      @p rhs will no longer contain a PIMPL and can not
     *                      be used until a Center instance with a valid PIMPL
     *                      is copy/move assigned to @p rhs.
     *
     *  @return The current instance after taking ownership of @p rhs's state.
     *
     *  @throw None no throw guarantee.
     */
    Center<T>& operator=(Center<T>&& rhs) noexcept;

    /** @brief Creates a new Center centered on the provided point.
     *
     *  This ctor is used to create a new Center instance with the provided
     *  Cartesian coordinates.
     *
     *  @param[in] x The x-coordinate for the resulting Center
     *  @param[in] y The y-coordinate for the resulting Center
     *  @param[in] z The z-coordinate for the resulting Center
     *
     *  @throw std::bad_alloc if there is insufficient memory to create the
     *                        PIMPL. Strong throw guarantee.
     */
    Center(T x, T y, T z);

    /** @brief Creates a new Center with the provided state.
     *
     *  This ctor can be used to create a Center which uses the provided PIMPLs.
     *
     *  @param[in] cpimpl The PIMPL to use for the Center part of the resulting
     *                    instance.
     *  @param[in] ppimpl The PIMPL to use for the Point part of the resulting
     *                    instance.
     *
     *  @throw None no throw guarantee.
     */
    Center(center_pimpl_ptr_t cpimpl, point_pimpl_ptr_t ppimpl) noexcept;

    /// Defaulted no throw dtor
    ~Center() noexcept override;

    /** @brief Adds a shell to the center.
     *
     *  This function will create a Shell instance, with the specified
     *  parameters, on the current Center.
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

    /** @brief Serialize Center instance
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

    /** @brief Deserialize for Center instance
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
    /// Implements size()
    size_type size_() const noexcept;
    /// Implements operator[](size_type)
    reference at_(size_type i);
    /// Implements operator[](size_type)const
    const_reference at_(size_type i) const;
    /// The instance that actually implements this class
    center_pimpl_ptr_t m_pimpl_;
};

/** @brief Compares two Center instances for equality.
 *
 *  Two Center instances are considered equal if they contain the
 *  same number of shells and if the i-th shell of @p lhs is equal to
 *  the i-th shell of @p rhs for all i in the range `[0, lhs.size())`.
 *
 *  @tparam T The type used to hold the Center's parameters.
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
bool operator==(const Center<T>& lhs, const Center<T>& rhs) {
    using base_t = utilities::IndexableContainerBase<Center<T>>;
    return static_cast<const base_t&>(lhs) == static_cast<const base_t&>(rhs);
}

/** @brief Determines if two Center instances are different.
 *
 *  Two Center instances are considered equal if they contain the
 *  same number of shells and if the i-th shell of @p lhs is equal to
 *  the i-th shell of @p rhs for all i in the range `[0, lhs.size())`.
 *  Different is defined as not equal.
 *
 *  @tparam T The type used to hold the Center's parameters.
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
bool operator!=(const Center<T>& lhs, const Center<T>& rhs) {
    return !(lhs == rhs);
}

extern template class Center<double>;
extern template class Center<float>;

} // namespace libchemist
