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

#pragma once
#include <chemist/chemical_system/electron/electron.hpp>
#include <chemist/chemical_system/nucleus/nucleus.hpp>
#include <chemist/quantum_mechanics/operator/operator_fwd.hpp>

/** @file typedefs.hpp
 *
 *  To make the user's life easier we have typedefed common template
 *  instantiations of various operators. Types are derived using the following
 *  commonly used conventions:
 *
 *  - all typedefs end with "_type" as we suspect the user may name their
 *    variable using similar conventions.
 *  - uppercase letter denotes a many-particle operator (or more generally an
 *    operator with an indefinite number of particles)
 *  - lowercase letter denotes a one-particle operator (or more generally an
 *    operator with a definite number of particles)
 *  - after the letter for the operator we denote the particles involved using
 *    the convention that e = electron(s) and n = nucleus or nuclei
 *  - t/T is kinetic energy operator
 *  - v/V is Coulomb operator
 *  - k/K is the (SCF) exchange operator
 *  - xc/XC is DFT exchange-correlation operator
 *
 */

namespace chemist::qm_operator {

using t_e_type = Kinetic<Electron>;
using T_e_type = Kinetic<ManyElectrons>;
using t_n_type = Kinetic<Nucleus>;
using T_n_type = Kinetic<Nuclei>;

using v_ee_type = Coulomb<Electron, Electron>;
using V_ee_type = Coulomb<ManyElectrons, ManyElectrons>;
using v_en_type = Coulomb<Electron, Nuclei>;
using V_en_type = Coulomb<ManyElectrons, Nuclei>;
using V_nn_type = Coulomb<Nuclei, Nuclei>;

using k_ee_type = Exchange<Electron, Electron>;
using K_ee_type = Exchange<ManyElectrons, ManyElectrons>;
using k_en_type = Exchange<Electron, Nuclei>;
using K_en_type = Exchange<ManyElectrons, Nuclei>;
using K_nn_type = Exchange<Nuclei, Nuclei>;

using xc_ee_type = ExchangeCorrelation<Electron, Electron>;
using XC_ee_type = ExchangeCorrelation<ManyElectrons, ManyElectrons>;
using xc_en_type = ExchangeCorrelation<Electron, Nuclei>;
using XC_en_type = ExchangeCorrelation<ManyElectrons, Nuclei>;
using XC_nn_type = ExchangeCorrelation<Nuclei, Nuclei>;

} // namespace chemist::qm_operator