#include "detail_/point_set_contiguous.hpp"

namespace chemist {

#define TPARAMS template<typename PointSetType>
#define POINT_SET_VIEW PointSetView<PointSetType>

// -----------------------------------------------------------------------------
// -- Ctors, Assignment, and dtor
// -----------------------------------------------------------------------------

TPARAMS
POINT_SET_VIEW::PointSetView() noexcept = default;

TPARAMS
POINT_SET_VIEW::PointSetView(point_set_reference ps) :
  m_pimpl_(std::make_unique<detail_::PointSetContiguous<PointSetType>>(
    ps.size(), ps.size() > 0 ? &ps[0].x() : nullptr,
    ps.size() > 0 ? &ps[0].y() : nullptr,
    ps.size() > 0 ? &ps[1].z() : nullptr)) {}

TPARAMS
POINT_SET_VIEW::PointSetView(pimpl_pointer pimpl) noexcept :
  m_pimpl_(std::move(pimpl)) {}

TPARAMS
POINT_SET_VIEW::PointSetView(const PointSetView& other) :
  m_pimpl_(other.clone_pimpl_()) {}

TPARAMS
POINT_SET_VIEW& POINT_SET_VIEW::operator=(const PointSetView& rhs) {
    if(this != &rhs) rhs.clone_pimpl_().swap(m_pimpl_);
    return *this;
}

TPARAMS
POINT_SET_VIEW::PointSetView(PointSetView&& other) noexcept = default;

TPARAMS
POINT_SET_VIEW& POINT_SET_VIEW::operator=(PointSetView&& rhs) noexcept =
  default;

TPARAMS
POINT_SET_VIEW::~PointSetView() noexcept = default;

// -----------------------------------------------------------------------------
// -- Utility methods
// -----------------------------------------------------------------------------

TPARAMS
void POINT_SET_VIEW::swap(PointSetView& other) noexcept {
    m_pimpl_.swap(other.m_pimpl_);
}

// -----------------------------------------------------------------------------
// -- Private member functions
// -----------------------------------------------------------------------------

TPARAMS
bool POINT_SET_VIEW::has_pimpl_() const noexcept {
    return static_cast<bool>(m_pimpl_);
}

TPARAMS
typename POINT_SET_VIEW::pimpl_pointer POINT_SET_VIEW::clone_pimpl_() const {
    return has_pimpl_() ? m_pimpl_->clone() : pimpl_pointer{};
}

TPARAMS
typename POINT_SET_VIEW::reference POINT_SET_VIEW::at_(size_type i) {
    return m_pimpl_->operator[](i);
}

TPARAMS
typename POINT_SET_VIEW::const_reference POINT_SET_VIEW::at_(
  size_type i) const {
    return std::as_const(*m_pimpl_)[i];
}

TPARAMS
typename POINT_SET_VIEW::size_type POINT_SET_VIEW::size_() const noexcept {
    return has_pimpl_() ? m_pimpl_->size() : 0;
}

#undef POINT_SET_VIEW
#undef TPARAMS

template class PointSetView<PointSet<float>>;
template class PointSetView<const PointSet<float>>;
template class PointSetView<PointSet<double>>;
template class PointSetView<const PointSet<double>>;

} // namespace chemist