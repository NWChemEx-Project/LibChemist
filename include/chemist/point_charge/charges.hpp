/*
 * Copyright 2023 NWChemEx-Project
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
#include <chemist/point_charge/point_charge_view.hpp>
#include <utilities/containers/indexable_container_base.hpp>
namespace chemist {
namespace detail_ {
template<typename T>
class ChargesPIMPL;
}

template<typename T>
class Charges : public utilities::IndexableContainerBase<Charges<T>> {
private:
    using base_type = utilities::IndexableContainerBase<Charges<T>>;

public:
    /// The type of the PIMPL
    using pimpl_type = detail_::ChargesPIMPL<T>;

    /// The type of a pointer to a PIMPL
    using pimpl_pointer = std::unique_ptr<pimpl_type>;

    /// The elements in the container
    using value_type = PointCharge<T>;

    /// Read/write reference to an element
    using reference = PointChargeView<value_type>;

    /// Read-only reference to an element
    using const_reference = PointChargeView<const value_type>;

    /// Integral type used for indexing
    using size_type = typename base_type::size_type;

    Charges() noexcept;
    ~Charges() noexcept;

    // void push_back(value_type q);

    size_type size() const noexcept;

    void push_back(value_type q);

private:
    friend base_type;

    reference at_(size_type i);

    const_reference at_(size_type i) const;

    bool has_pimpl_() const noexcept;

    pimpl_pointer m_pimpl_;
};

extern template class Charges<float>;
extern template class Charges<double>;

} // namespace chemist
