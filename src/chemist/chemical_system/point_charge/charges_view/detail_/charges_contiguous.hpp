#pragma once
#include "charges_view_pimpl.hpp"

namespace chemist::detail_ {

/** @brief Used to alias the charges for a Charges object when the charges
 *         are in a contiguous array.
 *
 *  @tparam ChargesType the type *this is acting like it aliases.
 */
template<typename ChargesType>
class ChargesContiguous : public ChargesViewPIMPL<ChargesType> {
private:
    /// Type of *this
    using my_type = ChargesContiguous<ChargesType>;

    /// Type of the base class
    using base_type = ChargesViewPIMPL<ChargesType>;

public:
    using typename base_type::reference;

    using typename base_type::const_reference;

    using typename base_type::point_charge_traits;

    using typename base_type::pimpl_pointer;

    using charges_traits = typename base_type::charges_traits;

    using point_set_traits = typename charges_traits::point_set_traits;

    using point_set_reference = typename charges_traits::reference;

    using charge_pointer = typename point_charge_traits::charge_pointer;

    using typename base_type::size_type;

    ChargesContiguous() = default;

    ChargesContiguous(const ChargesContiguous& other) = default;

    ChargesContiguous(point_set_reference points, charge_pointer pcharges) :
      m_points_(std::move(points)), m_pcharges_(pcharges) {}

protected:
    pimpl_pointer clone_() const { return std::make_unique<my_type>(*this); }

    reference at_(size_type i) noexcept override {
        return reference(m_pcharges_[i], m_points_[i]);
    }

    const_reference at_(size_type i) const noexcept override {
        return const_reference(m_pcharges_[i], m_points_[i]);
    }

    size_type size_() const noexcept override { return m_points_.size(); }

private:
    point_set_reference m_points_;

    charge_pointer m_pcharges_;
};

} // namespace chemist::detail_