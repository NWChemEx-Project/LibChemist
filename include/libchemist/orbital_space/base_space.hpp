#pragma once
#include "libchemist/types.hpp"
#include <sde/detail_/memoization.hpp>

namespace libchemist::orbital_space {

/** @brief Models the most fundamental properties of an orbital space
 *
 *  The BaseSpace_ class contains the properties all orbital spaces have in
 *  common. For the moment this amounts to orthonormality. The orthonormality
 *  information is stored by means of the overlap matrix.
 *
 *  @note This class's name ends in an underscore because we intend for users to
 *        declare instances of it with typdefs.
 *
 *  @tparam OverlapType The type of the tensor holding the overlap matrix.
 */
template<typename OverlapType>
class BaseSpace_ {
public:
    /// Type of the tensor holding the overlap matrix
    using overlap_type = OverlapType;

    /** @brief Creates a BaseSpace with no overlap matrix.
     *
     *  This ctor creates a
     *  @throw None no throw guarantee.
     */
    BaseSpace_() = default;

    /** @brief Creates a base space with the specified overlap matrix
     *
     *  @param[in] s the value of the overlap matrix.
     *
     *  @throw ??? if moving `s` throws. Same exception guarantee as
     *             overlap_type's move ctor.
     */
    BaseSpace_(overlap_type s) : m_S_(std::move(s)) {}

    /** @brief The overlap matrix in read/write format
     *
     *  @return The contained overlap matrix in read/write format
     *
     *  @throws std::bad_optional_access the default implementation throws if
     *          the overlap matrix has not been set. Derived instances can
     *          override the internal mechanism to compute the overlap matrix on
     *          the fly.
     */
    auto& S() { return S_(); }

    /** @brief The overlap matrix in read-only format
     *
     *  @return The contained overlap matrix in read-only format.
     *
     *  @throws std::bad_optional_access the default implementation throws if
     *          the overlap matrix has not been set. Derived instances can
     *          override the internal mechanism to compute the overlap matrix on
     *          the fly.
     */
    const auto& S() const { return S_(); }

    /** @brief Returns True if the overlap matrix has been set and false
     *         otherwise.
     *
     *  @return True if this instance contains an overlap matrix and false
     *          otherwise.
     *
     *  @throw None No throw guarantee.
     */
    bool has_overlap() const noexcept { return m_S_.has_value(); }

    /** @brief Hashes the current instance.
     *
     *  @param[in,out] h The hasher instance to use for hashing. The internal
     *                   state of h will be modified so that its internal hash
     *                   includes state information about this BaseSpace_
     *                   instance.
     */
    virtual void hash(sde::Hasher& h) const { hash_(h); }

    virtual overlap_type transform(const overlap_type& t, const std::vector<std::size_t>& = {}) const {
        return t;
    }

protected:
    /// Actually implements hash. Should be overridden by derived classes
    virtual void hash_(sde::Hasher& h) const {
        if(has_overlap()) h(S());
    }

    /// Can be used by derived classes to set the value of the overlap matrix
    void set_overlap_(overlap_type S) const { m_S_ = std::move(S); }

    /// Should be overriden by the derived class to return the overlap matrix
    virtual overlap_type& S_() { return m_S_.value(); }

    /// Should be overriden by the derived class to return the overlap matrix
    virtual const overlap_type& S_() const { return m_S_.value(); }

private:
    /// The actual instance of the overlap matrix
    mutable std::optional<overlap_type> m_S_;
};

template<typename OverlapType>
bool operator==(const BaseSpace_<OverlapType>& lhs,
                const BaseSpace_<OverlapType>& rhs) {
    // TODO: Actually compare the tensors
    if(lhs.has_overlap() != rhs.has_overlap())
        return false;
    else if(!lhs.has_overlap())
        return true; // Both don't have an overlap
    return sde::hash_objects(lhs) == sde::hash_objects(rhs);
}

template<typename OverlapType>
bool operator!=(const BaseSpace_<OverlapType>& lhs,
                const BaseSpace_<OverlapType>& rhs) {
    return !(lhs == rhs);
}

/// BaseSpace that uses a normal tensor for the overlap matrix
template<typename T>
using BaseSpace = BaseSpace_<type::tensor<T>>;

/// BaseSpace that uses a tensor-of-tensors for the overlap matrix
template<typename T>
using SparseBase = BaseSpace_<type::tensor_of_tensors<T>>;

} // namespace libchemist::orbital_space
