#include "export_point_charge.hpp"
#include <chemist/point_charge/point_charge_view.hpp>
#include <pybind11/operators.h>

namespace chemist {
namespace detail_ {

template<typename T>
void export_charge_view_(const char* name, python_module_reference m) {
    using point_charge_type = PointCharge<T>;
    using charge_view_type  = PointChargeView<point_charge_type>;
    using charge_type       = typename charge_view_type::charge_type;
    using point_charge_reference =
      typename charge_view_type::point_charge_reference;
    using point_view_type = typename charge_view_type::point_view_type;

    python_class_type<charge_view_type, point_view_type>(m, name)
      .def(pybind11::init<point_charge_reference>())
      .def_property(
        "charge", [](charge_view_type& self) { return self.charge(); },
        [](charge_view_type& self, charge_type q) { self.charge() = q; })
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self == point_charge_type())
      .def(point_charge_type() == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def(pybind11::self != point_charge_type())
      .def(point_charge_type() != pybind11::self);
}

} // namespace detail_

void export_charge_view(python_module_reference m) {
    detail_::export_charge_view_<float>("PointChargeViewF", m);
    detail_::export_charge_view_<double>("PointChargeViewD", m);
}

} // namespace chemist
