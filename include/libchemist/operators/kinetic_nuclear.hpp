#pragma once
#include "libchemist/electrons/electrons.hpp"
#include "libchemist/molecule/molecule.hpp"
#include "libchemist/operators/derivative.hpp"
#include "libchemist/operators/detail_/operator_impl.hpp"
#include "libchemist/operators/kinetic.hpp"

namespace libchemist::operators {

/** @brief Models the kinetic energy operator of a particle
 *
 *  @tparam Particle The type of the particle.
 */
template<typename Particle>
using Kinetic_Nuclear = Derivative<Kinetic<Particle>, libchemist::Nuclei>;

/// Type of the electronic kinetic energy operator
using ElectronKinetic_Nuclear = Kinetic_Nuclear<libchemist::Electron>;

/// Type of the many electron kinetic energy operator
using NElectronKinetic_Nuclear = Kinetic_Nuclear<libchemist::ManyElectrons>;

} // namespace libchemist::operators
