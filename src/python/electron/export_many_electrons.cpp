/*
 * Copyright 2024 NWChemEx-Project
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

#include "export_electron.hpp"
#include <chemist/electron/many_electrons.hpp>
#include <pybind11/operators.h>

namespace chemist {

void export_many_electrons(python_module_reference m) {
    using size_type = typename ManyElectrons::size_type;

    python_class_type<ManyElectrons>(m, "ManyElectrons")
      .def(pybind11::init<>())
      .def(pybind11::init<size_type>())
      .def("at", [](ManyElectrons& self, size_type i) { return self.at(i); })
      .def("size", [](ManyElectrons& self) { return self.size(); })
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self);
}

} // namespace chemist