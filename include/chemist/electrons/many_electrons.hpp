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

#pragma once
#include "chemist/detail_/hashing.hpp"

namespace chemist {

/** @brief Class representing an indeterminate number of electrons.
 *
 *  This class models a swarm of electrons. In practice the swarm may have 0 or
 *  more electrons in it. The number is not known at compile time.
 */
struct ManyElectrons {
    using size_type = unsigned long;

    /** @brief Hashes a ManyElectrons instance.
     *
     *  This function updates the internal hash of @p h with the hash of this
     *  instance's state (*i.e.*, it's number of electrons).
     *
     *  @param[in,out] h The hasher instance to use for hashing. After this call
     *                   @p h will be updated with the hash of this instance.
     */
    void hash(detail_::Hasher& h) const noexcept { h(size); }

    /// The number of electrons
    size_type size = 0;
};

inline bool operator==(const ManyElectrons& lhs, const ManyElectrons& rhs) {
    return lhs.size == rhs.size;
}

inline bool operator!=(const ManyElectrons& lhs, const ManyElectrons& rhs) {
    return !(lhs == rhs);
}

} // namespace chemist
