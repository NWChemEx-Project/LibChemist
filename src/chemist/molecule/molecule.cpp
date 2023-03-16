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

#include "chemist/molecule/molecule.hpp"
#include "mol_pimpl.hpp"
#include <iostream> //For std::endl

namespace chemist {

using value_type      = typename Molecule::value_type;
using reference       = typename Molecule::reference;
using const_reference = typename Molecule::const_reference;
using size_type       = typename Molecule::size_type;
using iterator        = typename Molecule::iterator;
using const_iterator  = typename Molecule::const_iterator;

Molecule::Molecule() : pimpl_(std::make_unique<detail_::ContiguousMol>()) {}

Molecule::Molecule(const Molecule& rhs) : Molecule() {
    for(auto ai : rhs) push_back(ai);
}

Molecule::Molecule(Molecule&& rhs) noexcept = default;

Molecule& Molecule::operator=(const Molecule& rhs) {
    return *this = std::move(Molecule(rhs));
}

Molecule& Molecule::operator=(Molecule&& rhs) noexcept = default;

Molecule::Molecule(std::initializer_list<Atom> atoms) : Molecule() {
    for(auto& a : atoms) push_back(a);
}

Molecule::~Molecule() noexcept = default;

size_type Molecule::size() const noexcept { return pimpl_->size(); }

reference Molecule::at(size_type i) noexcept { return pimpl_->at(i); }

void Molecule::push_back(value_type atom) {
    pimpl_->push_back(std::move(atom));
}

iterator Molecule::begin() noexcept { return pimpl_->begin(); }

const_iterator Molecule::begin() const noexcept { return pimpl_->begin(); }

iterator Molecule::end() noexcept { return pimpl_->end(); }

const_iterator Molecule::end() const noexcept { return pimpl_->end(); }

std::ostream& operator<<(std::ostream& os, const chemist::Molecule& mol) {
    for(const auto& ai : mol) os << ai << std::endl;
    return os;
}

bool operator==(const Molecule& lhs, const Molecule& rhs) noexcept {
    // Check for equivalent sizes before iterating through all atoms
    if(lhs.size() != rhs.size()) return false;

    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

} // namespace chemist
