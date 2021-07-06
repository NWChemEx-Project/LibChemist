#pragma once
#include "libchemist/basis_set/basis_set.hpp"
#include "libchemist/orbital_space/base_space.hpp"
#include "libchemist/orbital_space/dependent_space.hpp"

namespace libchemist::orbital_space {

/** @brief Models a space spanned by atomic orbitals.
 *
 *  The AO space takes on a special role in quantum chemistry in that it is a
 *  sort of lowest common denominator among spaces. This is because we usually
 *  know how to compute integrals in the AO space, but not directly in the
 *  derived spaces.
 *
 *  @tparam BasisType The type of the object holding the basis set parameters.
 *  @tparam BaseType  The type of the class this class derives from. It is
 *                    assumed to be either BaseSpace or DependentSpace.
 */
template<typename BasisType, typename BaseType>
class AOSpace : public BaseType {
private:
    /// Type of this instance
    using my_type = AOSpace<BasisType, BaseType>;

public:
    /// Type of the object holding the AO basis set parameters
    using basis_type = BasisType;

    /// Type used for indexing and offsets
    using size_type = typename BaseType::size_type;

    /** @brief Creates a new AOSpace. The AOSpace has no AO basis set and the
     *         base class is also default initialized.
     *
     *  @throw ??? Throws if initialization of the base class throws. Same throw
     *             gurantee.
     */
    AOSpace() = default;

    /** @brief Creates a new AOSpace which is a deep copy of @p rhs.
     *
     *  This ctor creates a new AOSpace instance by deep copying the AOSpace
     *  part of the polymorphic object @p rhs. Note that the "AOSpace part"
     *  includes the class AOSpace derives from.
     *
     *  @param[in] rhs The AOSpace we are copying.
     *
     *  @throw std::bad_alloc if there is a problem allocating the memory for
     *                        the new state. Strong throw guarantee.
     *
     */
    AOSpace(const AOSpace& rhs) = default;

    /** @brief Creates a new AOSpace by taking ownership of @p rhs.
     *
     *  This ctor creates a new AOSpace instance by taking ownerhip of the
     *  state inside the AOSpace part of the polymorphic object @p rhs. Note
     *  that the "AOSpace part" includes the class AOSpace derives from and that
     *  if the most derived type of @p rhs is not AOSpace, this will cause a
     *  slice to happen.
     *
     *  @param[in,out] rhs The AOSpace we are taking the state from. After this
     *                     operation AOSpace will be in a valid but otherwise
     *                     undefined state.
     *
     *  @throw ??? If moving the base class throws the move ctor will throw.
     *             Same throw guarantee as the base class.
     */
    AOSpace(AOSpace&& rhs) = default;

    /** @brief Replaces the current AOSpace's state with a deep copy of @p rhs.
     *
     *  The copy assignment operator replaces this AOSpace instance's state with
     *  the state inside the AOSpace part of the polymorphic object @p rhs. Note
     *  that the "AOSpace part" includes the class AOSpace derives from and that
     *  if the most derived type of @p rhs is not AOSpace, this will cause a
     *  slice to happen.
     *
     *  @param[in] rhs The AOSpace we are copying the state from.
     *
     *  @return The current instance with its state replaced by that of @p rhs.
     *
     *  @throw std::bad_alloc if there is a problem allocating the memory for
     *                        the new state. Strong throw guarantee.
     */
    AOSpace& operator=(const AOSpace& rhs) = default;

    /** @brief Replaces the current AOSpace's state with that of @p rhs.
     *
     *  The move assignment operator replaces this AOSpace instance's state with
     *  the state inside the AOSpace part of the polymorphic object @p rhs. Note
     *  that the "AOSpace part" includes the class AOSpace derives from and that
     *  if the most derived type of @p rhs is not AOSpace, this will cause a
     *  slice to happen.
     *
     *  @param[in,out] rhs The AOSpace we are taking the state from. After this
     *                     operation AOSpace will be in a valid but otherwise
     *                     undefined state.
     *
     *  @return The current instance with its state replace by that of @p rhs.
     *
     *  @throw ??? If moving the base class throws the move assignment operator
     *             will throw. Same throw guarantee as the base class.
     */
    AOSpace& operator=(AOSpace&& rhs) = default;

    /** @brief Creates a new atomic orbital space.
     *
     * @tparam Args The types of the arguments which will be forwarded to the
     *              base class's ctor. These types will be deduced automatically
     *              and the user should not have to specify them.
     *
     * @param[in] bs The parameters of the atomic orbitals.
     * @param[in] args Inputs to forward to the base class's ctor.
     */
    template<typename... Args>
    explicit AOSpace(basis_type bs, Args&&... args);

    /** @brief Accessor for read/write access to the basis set parameters
     *
     *  @return The basis set parameters in a read/write state.
     */
    auto& basis_set() { return m_bs_; }

    /** @brief Accessor for read-only access to the basis set parameters
     *
     *  @return The basis set parameters in a read-only state.
     */
    const auto& basis_set() const { return m_bs_; }

protected:
    /// Get the type of the container holding the mode offsets from the base
    using mode_container = typename BaseType::mode_container;

    /** @brief Overrides the size member so that it returns the number of AOs.
     *
     *  This function returns the number of AOs in the basis set unless it
     *  inherits from `DerivedSpace`, in which case it returns the total number
     *  of AOs for each independent tuple.
     *
     *  @return The number of orbitals in this space.
     *
     *  @throw None No throw guarantee.
     */
    virtual size_type size_() const noexcept override;

    /** @brief Implements transform for an AOSpace
     *
     *  We presently assume that the integral is computed using the current
     *  AOSpace (we do not verify this assumption) and simply return the tensor
     *  we were provided.
     *
     *  @param[in] t The tensor to transform.
     *  @param[in] modes The modes of @p t to transform to this orbital space.
     *
     *  @return A copy of @p t.
     */
    virtual type::tensor_wrapper transform_(
      const type::tensor_wrapper& t,
      const mode_container& modes) const override;

    virtual type::tot_wrapper transform_(
      const type::tot_wrapper& t,
      const mode_container& modes) const override;

    /** @brief Overrides hashing to account for the basis set
     *
     *  @param[in,out] h The hasher instance being used. After this call the
     *                   AO basis set wil be added to the internal hashed state.
     */
    virtual void hash_(sde::Hasher& h) const override;

    /** @brief Overrides polymorphic comparison to account for basis set.
     *
     *  @param[in] rhs The orbital space we are comparing to.
     *
     *  @return True if @p rhs is the same as this instance and false otherwise.
     *
     *  @throw None No throw guarantee.
     */
    virtual bool equal_(const BaseSpace& rhs) const noexcept override;

private:
    /// The object holding the basis set parameters
    basis_type m_bs_;
};

/** @brief Comapres two AOSpace instances for equality.
 *
 *  Two AOSpace instances are equal if the AOBasisSet instances they contain are
 *  the same and if the state of the base classes are the same. In particular
 *  this means that both @p lhs and @p rhs must have the same type to be
 *  considered equal.
 *
 *  @tparam LHSAO The type of the AO basis set in @p lhs. Assumed to be an
 *                 instantiation of `libchemist::AOBasisSet`.
 *  @tparam LHSBase The class AOSpace inherits from. Assumed to be either
 *                  `libchemist::BaseSpace` or `libchemist::DependentSpace`.
 *  @tparam RHSAO The type of the AO basis set in @p rhs. Assumed to be an
 *                 instantiation of `libchemist::AOBasisSet`.
 *  @tparam RHSBase The class AOSpace inherits from. Assumed to be either
 *                  `libchemist::BaseSpace` or `libchemist::DependentSpace`.
 *
 *  @param[in] lhs The instance on the left of the equality.
 *  @param[in] rhs The instance on the right of the equality.
 *
 *  @return True if the AOSpace part of @p lhs compares equal to the AOSpace
 *          part of @p rhs. False otherwise.
 *
 *  @throw ??? Throws if comparing the base classes throws. Same throw
 *             guarantee.
 */
template<typename LHSAO, typename LHSBase, typename RHSAO, typename RHSBase>
inline bool operator==(const AOSpace<LHSAO, LHSBase>& lhs,
                       const AOSpace<RHSAO, RHSBase>& rhs) {
    // Must be same type
    if constexpr(!std::is_same_v<decltype(lhs), decltype(rhs)>)
        return false;
    else {
        // Compare the basis sets
        if(lhs.basis_set() != rhs.basis_set()) return false;

        const LHSBase& lbase = lhs;
        const RHSBase& rbase = rhs;
        return lbase == rbase;
    }
}

/** @brief Determines if two AOSpace instances are different.
 *
 *  Two AOSpace instances are equal if the AOBasisSet instances they contain are
 *  the same and if the state of the base classes are the same. In particular
 *  this means that both @p lhs and @p rhs must have the same type to be
 *  considered equal.
 *
 *  @tparam LHSAO The type of the AO basis set in @p lhs. Assumed to be an
 *                 instantiation of `libchemist::AOBasisSet`.
 *  @tparam LHSBase The class AOSpace inherits from. Assumed to be either
 *                  `libchemist::BaseSpace` or `libchemist::DependentSpace`.
 *  @tparam RHSAO The type of the AO basis set in @p rhs. Assumed to be an
 *                 instantiation of `libchemist::AOBasisSet`.
 *  @tparam RHSBase The class AOSpace inherits from. Assumed to be either
 *                  `libchemist::BaseSpace` or `libchemist::DependentSpace`.
 *
 *  @param[in] lhs The instance on the left of the inequality.
 *  @param[in] rhs The instance on the right of the inequality.
 *
 *  @return False if the AOSpace part of @p lhs compares equal to the AOSpace
 *          part of @p rhs. True otherwise.
 *
 *  @throw ??? Throws if comparing the base classes throws. Same throw
 *             guarantee.
 */
template<typename LHSAO, typename LHSBase, typename RHSAO, typename RHSBase>
inline bool operator!=(const AOSpace<LHSAO, LHSBase>& lhs,
                       const AOSpace<RHSAO, RHSBase>& rhs) {
    return !(lhs == rhs);
}

// ------------------------------ Typedefs -------------------------------------
using AOSpaceD    = AOSpace<AOBasisSetD, BaseSpace>;
using AOSpaceF    = AOSpace<AOBasisSetF, BaseSpace>;
using DepAOSpaceD = AOSpace<AOBasisSetD, DependentSpace>;
using DepAOSpaceF = AOSpace<AOBasisSetF, DependentSpace>;

// ----------------- Forward Declare Explicit Instantiations -------------------
extern template class AOSpace<AOBasisSetD, BaseSpace>;
extern template class AOSpace<AOBasisSetF, BaseSpace>;
extern template class AOSpace<AOBasisSetD, DependentSpace>;
extern template class AOSpace<AOBasisSetF, DependentSpace>;

// ------------------- Inline Implementations ----------------------------------
template<typename BasisType, typename BaseType>
template<typename... Args>
AOSpace<BasisType, BaseType>::AOSpace(basis_type bs, Args&&... args) :
  BaseType(std::forward<Args>(args)...), m_bs_(std::move(bs)) {}

} // namespace libchemist::orbital_space
