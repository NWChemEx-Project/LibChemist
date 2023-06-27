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
#include <chemist/basis_set/primitive/primitive.hpp>
#include <chemist/detail_/view/traits.hpp>
#include <chemist/point/point_view2.hpp>

namespace chemist {

/** @brief Models a reference to a Gaussian Primitive.
 *
 *  This class has the same API as Primitive; however, instances of 
 *  PrimitiveView do NOT own their state, they alias it.
 * 
 * @tparam PrimitiveType Expected to be a, possibly cv-qualified, 
 *                       Primitive type.
 */
template<typename PrimitiveType>
class PrimitiveView {
private:
    /// Used to work out view types
    using traits_type = ViewTraits<PrimitiveType>;

    /// Is the type of *this acting like it's const?
    static constexpr bool is_const_v = traits_type::is_const_v;

    /// Typedefs so we don't need "typename" and "template" below

    template<typename T>
    using apply_const = typename traits_type::template apply_const<T>;

    template<typename T>
    using apply_const_ref = typename traits_type::template apply_const_ref<T>;

    template<typename T>
    using ptr_type = typename traits_type::template apply_const_ptr<T>;

public:
    /// The unqualified type *this is a view of
    using primitive_type = typename traits_type::type;

    /// A read-only view of a primitive
    using const_primitive_reference = PrimitiveView<const primitive_type>;

    /// rank 1 tensor-like type used to hold the center
    using center_type = typename primitive_type::center_type;

    /// Type of a possibly mutable reference to the center
    using center_reference = PointView2<apply_const<center_type>>;

    /// Type of a read-only reference to the center
    using const_center_reference = PointView2<const center_type>;

    /// Floating-point type used to hold coefficient
    using coefficient_type = typename primitive_type::coefficient_type;

    /// Type of a possibly mutable reference to the coefficient
    using coefficient_reference = apply_const_ref<coefficient_type>;

    /// Type of a read-only reference to the coefficient
    using const_coefficient_reference = const coefficient_type&;

    /// Floating-point type used to hold exponent
    using exponent_type = typename primitive_type::exponent_type;

    /// Type of a possibly mutable reference to the exponent
    using exponent_reference = apply_const_ref<exponent_type>;

    /// Type of a read-only reference to the exponent
    using const_exponent_reference = const exponent_type&;

    /** @brief Creates a PrimitiveView that aliases the provided Primitive
     *
     *  This ctor is used to create a new PrimitiveView that aliases the
     *  provided Primitive. This ctor is primarily designed to be used when the
     *  Primitive's state is unpacked (such as when it is stored in a
     *  ContractedGaussian instance).
     *
     *  @param[in] coef The expansion coefficient of the Primitive.
     *  @param[in] exp The exponent of the Primitive.
     *  @param[in] x The x-coordinate where the Primitive is centered.
     *  @param[in] y The y-coordinate where the Primitive is centered.
     *  @param[in] z The z-coordinate where the Primitive is centered.
     *
     *  @throw None No throw guarantee.
     */
    PrimitiveView(coefficient_reference coef, 
                  exponent_reference exp, 
                  typename center_reference::coord_reference x, 
                  typename center_reference::coord_reference y, 
                  typename center_reference::coord_reference z) noexcept :
       PrimitiveView(coef, exp, center_reference(x, y, z)) {}

    /** @brief Creates a PrimitiveView that aliases the provided Primitive
     *
     *  This ctor is used to create a new PrimitiveView that aliases the
     *  provided Primitive. This ctor is primarily designed to be used when the
     *  Primitive's state is unpacked (such as when it is stored in a
     *  ContractedGaussian instance).
     *
     *  @param[in] coef The expansion coefficient of the Primitive.
     *  @param[in] exp The exponent of the Primitive.
     *  @param[in] center Where the Primitive is centered
     *
     *  @throw None No throw guarantee
     */
    PrimitiveView(coefficient_reference coef,
                  exponent_reference exp,
                  center_reference center) noexcept :
        m_center_(center), m_coef_(&coef), m_exp_(&exp) {}

    /** @brief Returns a possibly mutable view of the center.
     * 
     *  This method is used to get a view of the object representing the point
     *  in space where *this is centered. The resulting object is a modifiable 
     *  view if @p PrimitiveType is non-const and read-only otherwise.
     * 
     *  @return A view of the primitive's center. 
     * 
     *  @throw None No throw guarantee.
    */
    center_reference center() noexcept { return m_center_; }

    /** @brief Returns a read-only view of the center
     * 
     *  This method is the same as the non-const version, except that the
     *  returned center is guarantee to be read-only.
     * 
     *  @return A read-only view of the point where the primitive is centered.
     * 
     *  @throw None No throw guarantee.
     */
    const_center_reference center() const noexcept { return m_center_; }

    /** @brief Returns the coefficient of the aliased Primitive.
     *
     *  This function can be used to access the coefficient of the aliased
     *  Primitive in a (possibly) read/write format. The resulting reference is
     *  read/write as long as @p PrimitiveType is non-const.
     *
     *  @return The aliased Primitive's coefficient in a (possibly) read/write
     *          format.
     * 
     *  @throw none No throw guarantee.
     */
    coefficient_reference coefficient() noexcept { return *m_coef_;}

    /** @brief Returns the coefficient of the aliased Primitive.
     *
     *  This function can be used to access the coefficient of the aliased
     *  Primitive in a read-only state.
     *
     *  @return The aliased Primitive's coefficient in a read-only format.
     *
     *  @throw none No throw guarantee.
     */
    const_coefficient_reference coefficient() const noexcept { return *m_coef_; }

    /** @brief Returns the exponent of the aliased Primitive.
     *
     *  This function can be used to access the exponent of the aliased
     *  Primitive in a (possibly) read/write format. The resulting reference is
     *  read/write as long as @p T is non-const.
     *
     *  @return The aliased Primitive's exponent in a (possibly) read/write
     *          format.
     *  @throw none No throw guarantee.
     */
    exponent_reference exponent() noexcept { return *m_exp_; }

    /** @brief Returns the exponent of the aliased Primitive.
     *
     *  This function can be used to access the exponent of the aliased
     *  Primitive in a read-only.
     *
     *  @return The aliased Primitive's exponent in a read-only format.
     *
     *  @throw none No throw guarantee.
     */
    const_exponent_reference exponent() const noexcept { return *m_exp_; }

    /** @brief Allows *this to be implicitly converted to a read-only view.
     * 
     *  @return a read-only view of the same state aliased by *this.
     * 
     *  @throw None No throw guarantee
     */
    const_primitive_reference() const noexcept {
        return const_primitive_reference(*m_coef_, *m_exp_, m_center_);
    }

    /** @brief Determines if *this is value equal to @p rhs.
     * 
     *  This method value compares the center, coefficient, and exponent
     *  aliased by *this to the respective center, coefficient, and exponent
     *  aliased by @p rhs. If each respective comparison is true, then *this is
     *  value equal to @p rhs, otherwise they are different. Note that this
     *  is a value comparison, and not a determination of whether or not
     *  *this "points to" the same primitive as @p rhs.
     * 
     *  @param[in] rhs The view we are comparing to.
     * 
     *  @return True if *this is value equal to @p rhs and false otherwise.
     * 
     *  @throw None No throw guarantee. 
    */
    bool operator==(const PrimitiveView& rhs) const noexcept {
        return get_state_() == rhs.get_state_();
    }

    /** @brief Determines if *this is value equal to @p rhs.
     * 
     *  This method is similar to the comparison between two PrimitiveView
     *  objects, except @p rhs owns its value (and does not alias it). 
     * 
     *  @param[in] rhs The primitive we are comparing to.
     * 
     *  @return True if the state aliased by *this is value equal to the state
     *          owned by @p rhs and false otherwise.
     * 
     * @throw None No throw guarantee.
    */
    bool operator==(const primitive_type& rhs) const noexcept {
        if(rhs.is_null()) return false;
        if(m_center_ != rhs.center()) return false;
        if(coef() != rhs.coef()) return false;
        if(exp() != rhs.exp()) return false;
    }

    /** @brief Determines if *this is different from @p rhs.
     * 
     *  This method simply defines different as not value equal. See the 
     *  description for operator== for the definition of value equal.
     * 
     *  @param[in] rhs The PrimitiveView we are comparing to.
     * 
     *  @return False if *this is value equal to @p rhs and true otherwise.
     * 
     *  @throw None No throw guarantee.
    */
    bool operator!=(const PrimitiveView& rhs) const noexcept {
        return !(*this == rhs);
    }

    /** @brief Determines if *this is different from @p rhs.
     * 
     *  This method simply defines different as not value equal. See the 
     *  description for operator== for the definition of value equal.
     * 
     *  @param[in] rhs The Primitive we are comparing to.
     * 
     *  @return False if *this is value equal to @p rhs and true otherwise.
     * 
     *  @throw None No throw guarantee.
    */
    bool operator!=(const primitive_type& rhs) const noexcept {
        return !(*this == rhs);
    }

private:
    /// Calls std::tie on the state in *this
    auto get_state_() const noexcept {
        return std::tie(m_center_, *m_coef_, *m_exp_);
    }

    /// The point in space where *this is centered
    center_reference m_center_;

    /// The scale factor on *this
    apply_const_ptr<coefficient_type> m_coef_;

    /// The exponent on *ths
    apply_const_ptr<exponent_type> m_exp_;
};

// Ensures PrimitiveType can appear on the left side of operator==
template<typename PrimitiveType>
bool operator==(const std::decay_t<PrimitiveType>& lhs,
                const PrimitiveView<PrimitiveType>& rhs) {
                    return rhs == lhs;
}

// Ensures PrimitiveType can appear on the left side of operator!=
template<typename PrimitiveType>
bool operator!=(const std::decay_t<PrimitiveType>& lhs,
                const PrimitiveView<PrimitiveType>& rhs) {
                    return rhs != lhs;
}


// ------------------------------------Implemenations--------------------------

extern template class PrimitiveView<double>;
extern template class PrimitiveView<const double>;
extern template class PrimitiveView<float>;
extern template class PrimitiveView<const float>;

} // namespace chemist