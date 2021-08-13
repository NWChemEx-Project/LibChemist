#pragma once
#include "libchemist/density/density.hpp"
#include "libchemist/electrons/electrons.hpp"
#include "libchemist/operators/detail_/operator_impl.hpp"

namespace libchemist::operators {

/** @brief Exchange operator
 *
 *  The exchange operator is the product of the particle permutation operator
 *  and the Coulomb operator, i.e. @f$\frac{\widehat{P}_{12}}}{r_{12}}@f$
 *
 *  @tparam Particle1 The type of the first particle.
 *  @tparam Particle2 The type of the second particle.
 *
 */
template<typename Particle1, typename Particle2>
class Exchange : public detail_::OperatorImpl<Exchange, Particle1, Particle2> {
private:
    /// Type of the object this class inherits from
    using base_type = detail_::OperatorImpl<Exchange, Particle1, Particle2>;

public:
    Exchange()                = default;
    Exchange(const Exchange&) = default;
    Exchange(Exchange&&)      = default;
    Exchange(Particle1 p1, Particle2 p2) :
      base_type(std::move(p1), std::move(p2)) {}
    Exchange& operator=(const Exchange&) = default;
    Exchange& operator=(Exchange&&) = default;

protected:
    /// Gives a more traditional representation than the default implementation
    std::string as_string_impl() const override { return "k\u0302"; }
};

/// Type of Hartree-Fock Exchange
using ElectronEDensityExchange =
  Exchange<libchemist::Electron, libchemist::OneElectronDensity>;

} // namespace libchemist::operators
