#include "libchemist/basis_set/center/center.hpp"
#include "libchemist/basis_set/center/detail_/center_pimpl.hpp"
#include "libchemist/point/detail_/point_pimpl.hpp"

namespace libchemist {

template<typename T>
Center<T>::Center() :
  Center(std::make_unique<center_pimpl_t>(),
         std::make_unique<point_pimpl_t>()) {}

template<typename T>
Center<T>::Center(const Center<T>& rhs) :
  Center(std::make_unique<center_pimpl_t>(*rhs.m_pimpl_),
         std::make_unique<point_pimpl_t>(rhs.x(), rhs.y(), rhs.z())) {}

template<typename T>
Center<T>::Center(Center<T>&& rhs) noexcept = default;

template<typename T>
Center<T>::Center(T x, T y, T z) :
  Center(std::make_unique<center_pimpl_t>(),
         std::make_unique<point_pimpl_t>(x, y, z)) {}

template<typename T>
Center<T>::Center(center_pimpl_ptr_t cpimpl,
                  point_pimpl_ptr_t ppimpl) noexcept :
  m_pimpl_(std::move(cpimpl)),
  Point<T>(std::move(ppimpl)),
  utilities::IndexableContainerBase<Center<T>>() {}

template<typename T>
Center<T>& Center<T>::operator=(const Center<T>& rhs) {
    return *this = std::move(Center<T>(rhs));
}

template<typename T>
Center<T>& Center<T>::operator=(Center<T>&& rhs) noexcept = default;

template<typename T>
Center<T>::~Center() noexcept = default;

template<typename T>
void Center<T>::add_shell(pure_type pure, am_type l, param_set cs,
                          param_set es) {
    m_pimpl_->add_shell(pure, l, std::move(cs), std::move(es));
}

template<typename T>
typename Center<T>::size_type Center<T>::size_() const noexcept {
    return m_pimpl_->size();
}

template<typename T>
typename Center<T>::reference Center<T>::at_(size_type i) {
    auto ptr1 = m_pimpl_->at(i);
    auto ptr2 = this->point_alias();
    Shell<T> shell(std::move(ptr1), std::move(ptr2));
    return reference(std::move(shell));
}

template<typename T>
typename Center<T>::const_reference Center<T>::at_(size_type i) const {
    auto ptr1 = m_pimpl_->at(i);
    auto ptr2 = this->point_alias();
    Shell<T> shell(std::move(ptr1), std::move(ptr2));
    return const_reference(std::move(shell));
}

template class Center<double>;
template class Center<float>;

} // namespace libchemist
