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

#include "chemist/operators/coulomb_nuclear.hpp"
#include "chemist/operators/hamiltonian/electronic_hamiltonian_nuclear.hpp"
#include "chemist/operators/kinetic_nuclear.hpp"

namespace chemist::operators {

ElectronicHamiltonian_Nuclear::ElectronicHamiltonian_Nuclear(
  const Hamiltonian_Nuclear& H) {
    using T_type = NElectronKinetic_Nuclear;
    using V_type = NElectronNuclearAttraction_Nuclear;
    using G_type = NElectronRepulsion_Nuclear;

    for(const auto& T : H.get_terms<T_type>()) add_terms(*T);
    for(const auto& V : H.get_terms<V_type>()) add_terms(*V);
    for(const auto& G : H.get_terms<G_type>()) add_terms(*G);
}

} // namespace chemist::operators
