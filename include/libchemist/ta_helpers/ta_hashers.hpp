#pragma once
#include <madness/world/safempi.h>
#include <runtime/hasher.hpp>
#include <tiledarray.h>

namespace TiledArray {
//------------------------------------------------------------------------------
// Hashing for TiledArray Classes
//------------------------------------------------------------------------------

/** @brief Enables hashing for TA Range class.
 *
 * Free function to enable hashing.
 *
 * @param[in] R Range object
 * @param[in, out] h runtime::Hasher object.
 */
inline void hash_object(const TA::Range& R, runtime::Hasher& h) {
    // To create a unique hash for this type with the default constructor
    const char* mytype = "TA::Range";
    h(mytype);
    // Alternative to above but no guarantee for reproducibility
    // h(typeid(tr).hash_code());
    for(std::size_t i = 0; i < R.rank(); ++i) {
        h(R.dim(i).first);
        h(R.dim(i).second);
    }
}

/** @brief Enables hashing for TA TiledRange1 class.
 *
 * Free function to enable hashing.
 *
 * @param[in] tr TiledRange1 object
 * @param[in, out] h runtime::Hasher object.
 */
inline void hash_object(const TA::TiledRange1& tr, runtime::Hasher& h) {
    const char* mytype = "TA::TiledRange1";
    h(mytype);
    for(const auto& tile : tr) h(tile.first);
    h(tr.extent());
}

/** @brief Enables hashing for TA TiledRange class.
 *
 * Free function to enable hashing with BPHash library.
 *
 * @param[in] tr TiledRange object
 * @param[in, out] h runtime::Hasher object.
 */
inline void hash_object(const TiledArray::TiledRange& tr, runtime::Hasher& h) {
    const char* mytype = "TA::TiledRange";
    h(mytype);
    for(std::size_t i = 0; i < tr.rank(); ++i) h(tr.dim(i));
}

/** @brief Enables hashing for TA Pmap class.
 *
 * Free function to enable hashing with BPHash library.
 *
 * @param[in] p Pmap object
 * @param[in, out] h runtime::Hasher object.
 */
inline void hash_object(const TiledArray::Pmap& p, runtime::Hasher& h) {
    const char* mytype = "TA::Pmap";
    h(mytype);
    h(p.rank());
    h(p.size());
}

/** @brief Enables hashing for TA Tensor class.
 *
 * Free function to enable hashing with BPHash library.
 *
 * @tparam ValueType Type of value for the @p A tensor.
 * @tparam AllocatorType Type of allocator for the @p A tensor.
 * @param[in] A Tensor object
 * @param[in, out] h runtime::Hasher object.
 */
template<typename ValueType, typename AllocatorType>
void hash_object(const TA::Tensor<ValueType, AllocatorType>& A,
                 runtime::Hasher& h) {
    const char* mytype = "TA::Tensor";
    h(A.range());
    const auto n = A.range().volume();
    for(auto i = 0ul; i < n; ++i) h(A[i]);
}

/** @brief Returns a 128 bit hash value for TA::DistArray object.
 *
 * Involves MPI collective operation (MPI_Allreduce)
 *
 * @tparam TileType Type of tensor for @p A.
 * @tparam PolicyType Type of policy for @p A. Either DensePolicy or
 * SparsePolicy.
 * @param[in] A DistArray object
 * @return runtime::HashValue for TA::DistArray
 */
template<typename TensorType, typename PolicyType>
runtime::HashValue get_tile_hash_sum(
  const TA::DistArray<TensorType, PolicyType>& A) {
    auto& madworld = A.world();
    auto& mpiworld = madworld.mpi.Get_mpi_comm();

    // Note: Without the fence orbital space hash tests hang on parallel runs.
    madworld.gop.fence();
    runtime::HashValue myhash;
    runtime::HashValue mytotal(16, 0); // runtime::HashType::Hash128 has 16
                                       // uint8_t
    runtime::HashValue hashsum(16, 0);

    for(auto it = A.begin(); it != A.end(); ++it) {
        auto tile = A.find(it.index()).get();
        myhash    = runtime::make_hash(tile);
        for(auto i = 0; i < myhash.size(); i++) { mytotal[i] += myhash[i]; }
    }
    if(madworld.size() > 1 && !A.pmap().get()->is_replicated()) {
        // Note: Using &mytotal, &hashsum gives Seg Fault 11.
        madworld.mpi.Allreduce(mytotal.data(), hashsum.data(), mytotal.size(),
                               MPI_UINT8_T, MPI_SUM);
    } else {
        hashsum = mytotal;
    }
    madworld.gop.fence();
    return hashsum;
}

/** @brief Enables hashing for TA DistArray class.
 *
 * Free function to enable hashing with BPHash library.
 *
 * @tparam TensorType Type of tensor (TA::DistArray) for @p A.
 * @tparam PolicyType Type of policy for @p A. Either DensePolicy or
 * SparsePolicy.
 * @param[in] A DistArray object
 * @param[in, out] h runtime::Hasher object.
 */

template<typename TensorType, typename PolicyType>
void hash_object(const TA::DistArray<TensorType, PolicyType>& A,
                 runtime::Hasher& h) {
    const char* mytype = "TA::DistArray";
    h(mytype);
    if(A.is_initialized()) {
        h(A.range());
        h(get_tile_hash_sum(A));
    }
}

/** @brief Enables comparison between TA DistArray objects
 *
 * Free function to enable comparison between TA DistArray objects.
 *
 * @tparam TensorType Type of tensor (TA::DistArray) for @p A.
 * @tparam PolicyType Type of policy for @p A. Either DensePolicy or
 * SparsePolicy.
 * @tparam TensorType Type of tensor (TA::DistArray) for @p B.
 * @tparam PolicyType Type of policy for @p B. Either DensePolicy or
 * SparsePolicy.
 * @param[in] A DistArray object
 * @param[in] B DistArray object
 */

template<typename TensorTypeA, typename TensorTypeB, typename PolicyTypeA,
         typename PolicyTypeB>
bool operator==(const TA::DistArray<TensorTypeA, PolicyTypeA>& A,
                const TA::DistArray<TensorTypeB, PolicyTypeB>& B) {
    return runtime::hash_objects(A) == runtime::hash_objects(B);
}

/** @brief Enables comparison between TA DistArray objects
 *
 * Free function to enable comparison between TA DistArray objects.
 *
 * @tparam TensorType Type of tensor (TA::DistArray) for @p A.
 * @tparam PolicyType Type of policy for @p A. Either DensePolicy or
 * SparsePolicy.
 * @tparam TensorType Type of tensor (TA::DistArray) for @p B.
 * @tparam PolicyType Type of policy for @p B. Either DensePolicy or
 * SparsePolicy.
 * @param[in] A DistArray object
 * @param[in] B DistArray object
 */

template<typename TensorTypeA, typename TensorTypeB, typename PolicyTypeA,
         typename PolicyTypeB>
bool operator!=(const TA::DistArray<TensorTypeA, PolicyTypeA>& A,
                const TA::DistArray<TensorTypeB, PolicyTypeB>& B) {
    return !(A == B);
}

/** @brief Enables comparison between TA tensor objects
 *
 * Free function to enable comparison between TA tensor objects.
 *
 * @tparam ValueType Type of TA::tensor for @p A.
 * @tparam AllocatorType Type of TA::tensor for @p A.
 * @tparam ValueType Type of TA::tensor for @p B.
 * @tparam AllocatorType Type of TA::tensor for @p B.
 * @param[in] A TA tensor object
 * @param[in] B TA tensor object
 */
template<typename ValueTypeA, typename AllocatorTypeA, typename ValueTypeB,
         typename AllocatorTypeB>
bool operator==(const TA::Tensor<ValueTypeA, AllocatorTypeA>& A,
                const TA::Tensor<ValueTypeB, AllocatorTypeB>& B) {
    return runtime::hash_objects(A) == runtime::hash_objects(B);
}

/** @brief Enables comparison between TA tensor objects
 *
 * Free function to enable comparison between TA tensor objects.
 *
 * @tparam ValueType Type of TA::tensor for @p A.
 * @tparam AllocatorType Type of TA::tensor for @p A.
 * @tparam ValueType Type of TA::tensor for @p B.
 * @tparam AllocatorType Type of TA::tensor for @p B.
 * @param[in] A TA tensor object
 * @param[in] B TA tensor object
 */
template<typename ValueTypeA, typename AllocatorTypeA, typename ValueTypeB,
         typename AllocatorTypeB>
bool operator!=(const TA::Tensor<ValueTypeA, AllocatorTypeA>& A,
                const TA::Tensor<ValueTypeB, AllocatorTypeB>& B) {
    return !(A == B);
}
} // namespace TiledArray
