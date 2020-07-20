#pragma once
#include "libchemist/sparse_map/domain.hpp"
#include <utility>

namespace libchemist::sparse_map {
namespace detail_ {
class SparseMapPIMPL;
}

/** @brief Stores the sparsity relation among indices.
 *
 */
class SparseMap {
private:
    template<typename T>
    using il_type =
      std::initializer_list<std::pair<T, std::initializer_list<T>>>;

    using stl_map_t = std::map<std::vector<std::size_t>, Domain>;
public:
    using mapped_type = Domain;
    using key_type    = typename mapped_type::value_type;
    using value_type      = std::pair<const key_type, mapped_type>;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using iterator        = typename stl_map_t::iterator;
    using const_iterator  = typename stl_map_t::const_iterator;
    using size_type       = typename mapped_type::size_type;
    using index_set       = std::set<size_type>;
    using index_set_array = std::vector<index_set>;
    using index_set_map   = std::map<key_type, index_set_array>;

    SparseMap();
    SparseMap(const SparseMap& rhs);
    SparseMap(SparseMap&& rhs) noexcept;
    SparseMap& operator=(const SparseMap& rhs);
    SparseMap& operator=(SparseMap&& rhs) noexcept;
    SparseMap(il_type<key_type> il);
    ~SparseMap() noexcept;

    void swap(SparseMap& rhs) noexcept { m_pimpl_.swap(rhs.m_pimpl_); }

    size_type size() const noexcept;
    bool empty() const noexcept { return begin() == end(); }
    bool count(const key_type& i) const noexcept;
    template<typename BeginItr, typename EndItr>
    bool count(BeginItr&& b, EndItr&& e) const {
        const key_type k(std::forward<BeginItr>(b), std::forward<EndItr>(e));
        return count(k);
    }
    size_type ind_rank() const noexcept;

    /** @brief The number of independent modes associated with each Domain.
     *
     *  The
     * @return
     */
    size_type dep_rank() const noexcept;

    index_set_map indices() const;

    /** @brief Returns the .
    *
    *   This is a convenience function for calling indices(key_type, size_type)
    *   for each dependent index mode.
    *
    *   @result A vector of length dep_rank() such that the i-th element
    *           is the set of non-zero indices for mode i.
    */
    index_set_array indices(key_type ind) const;

    /** @brief Returns the set of indices @p ind maps to for a given mode.
     *
     *  Particularly when manipulating the full tensor corresponding to the
     *  dependent indices we need to know what values a given mode will take.
     *  For independent index @p ind, this function will return the set of valid
     *  dependent indices for a given mode.
     *
     * @param[in] ind The independent index whose dependent indices will be
     *                considered.
     * @param[in] mode the mode of each dependent index to be collected.
     * @return A set of the non-zero indices along mode @p mode for independent
     *         index @p ind.
     */
    index_set indices(key_type ind, size_type mode) const;


    mapped_type& operator[](const key_type& i);
    const mapped_type& operator[](const key_type& i)const;

    template<typename IndexType>
    mapped_type& at(IndexType&& idx);
    template<typename IndexType>
    const mapped_type& at(IndexType&& idx) const;


    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;

    /** @brief Adds single rank indices to the domain of a single rank index.
     *
     *  This is a convenience function for creating a SparseMap from single rank
     *  indices to single rank indices, which does not require you to wrap the
     *  input indices in an std::vector.
     *
     *  @param[in] ind The value of the independent index.
     *  @param[in] dep The value of the dependent index to add to @p ind 's
     *                 domain.
     */
    void add_to_domain(size_type ind, size_type dep);

    /** @brief Adds multi-rank indices to the domain of a single rank index.
     *
     *  This is a convenience function for creating a SparseMap from single rank
     *  indices to (possibly) multi-rank indices. It is designed so that you do
     *  not have to wrap the input indices in an std::vector.
     *
     *  @param[in] ind The value of the independent index.
     *  @param[in] dep The value of the dependent index to add to @p ind 's
     *                 domain.
     */
    void add_to_domain(size_type ind, key_type dep);
    void add_to_domain(key_type ind, key_type dep);
    template<typename BeginItr, typename EndItr>
    void add_to_domain(key_type ind, BeginItr first, EndItr last);

    SparseMap operator*(const SparseMap& rhs) const;
    SparseMap& operator*=(const SparseMap& rhs);

    bool operator==(const SparseMap& rhs) const noexcept;
    bool operator!=(const SparseMap& rhs) const noexcept;

    void hash(sde::Hasher& h) const;

    std::ostream& print(std::ostream& os) const;

    /** @brief Constructs the inverse SparseMap.
     *
     *  Creates a SparseMap which maps from the dependent indices to the
     *  independent indices of the current map.
     *
     *  sparse_map.inverse().inverse() == sparse_map
     *
     * @return The inverse of the SparseMap.
     */
    SparseMap inverse() const;

    /** @brief Creates a SparseMap from chaining two maps together.
     *
     *  Given the SparseMaps sm1(f -> g) and sm2(g -> h),
     *  the chained map from (f -> h) is formed by mapping each element
     *  f_i to a set of {g_i} using sm1, and then to any element in h which is mapped to from
     *  an element of {g_i} by sm2.
     *
     *  Requires that the maps share a common set of indices, g, as the dependent and
     *  independent indices respectively.
     *
     * @param[in] sm The SparseMap to chain with this instance.
     * @return The chained map.
     * @throw std::runtime_error if the rank of the dependent indices of this instance
     *                           is not equal to the rank of the independent indices
     *                           of \p sm.
     */
    SparseMap chain(const SparseMap& sm) const;

    /** @brief Creates a SparseMap which is the union of two maps.
     *
     *  Given two SparseMaps sm1(f -> g) and sm2(f -> g), the union is formed by
     *  mapping f_i to any element in g which f_i is mapped to by either sm1 or sm2.
     *
     *  Requires that either one of the sets is empty or both sets have the same
     *  rank in independent and dependent indices.
     *
     * @param[in] sm The SparseMap to take the union with this instance.
     * @return The union of the two maps.
     * @throw std::runtime_error if neither map is empty and the rank of the dependent
     *                           indices of this instance is not equal to the rank of
     *                           the independent indices of \p sm.
     */
    SparseMap map_union(const SparseMap& sm) const;

    /** @brief
     *
     *  Given two SparseMaps sm1(f -> g) and sm2(f -> g), the intersection is formed by
     *  mapping f_i to any element in g which f_i is mapped to by both sm1 and sm2.
     *
     * @param[in] sm The SparseMap to take the intersection with this instance.
     * @return The intersection of the two maps.
     * @throw std::runtime_error if neither map is empty and the rank of the dependent
     *                           indices of this instance is not equal to the rank of
     *                           the independent indices of \p sm.
     */
    SparseMap intersection(const SparseMap& sm) const;

private:
    mapped_type& at_(const key_type& i);
    const mapped_type& at_(const key_type& i) const;


    /** @brief Code factorization for ensuring an input index has the correct
     *         rank.
     *
     *  This function is used internally to ensure that a user-provided index
     *  has a rank consistent with the indices already in the SparseMap
     *  instance.
     *
     *  @param[in] idx_rank The rank of the incoming index. Will be compared
     *                      against ind_rank()/dep_rank() if @p dep is
     *                      false/true.
     *  @param[in] dep True if we should compare to the rank of the independent
     *                 indices and false otherwise.
     *  @throws std::runtime_error if the rank of the index is not the same as
     *                             the rank of the indices already in this
     *                             instance. Strong throw guarantee.
     */
    void check_rank_(size_type idx_rank, bool dep) const;

    std::unique_ptr<detail_::SparseMapPIMPL> m_pimpl_;
}; // class SparseMap

inline std::ostream& operator<<(std::ostream& os, const SparseMap& sm) {
    return sm.print(os);
}


//------------------------------------------------------------------------------
// Inline Implementations
//------------------------------------------------------------------------------

inline SparseMap::SparseMap(il_type<key_type> il) : SparseMap() {
    for(auto&& [k, v] : il) add_to_domain(k, v.begin(), v.end());
}

inline
typename SparseMap::mapped_type& SparseMap::operator[](const key_type& i) {
    return at(i);
}

inline const typename SparseMap::mapped_type&
SparseMap::operator[](const key_type& i) const{
    return at(i);
}

template<typename IndexType>
typename SparseMap::mapped_type& SparseMap::at(IndexType&& idx){
    return at_(key_type{idx.begin(), idx.end()});
}
template<typename IndexType>
const typename SparseMap::mapped_type& SparseMap::at(IndexType&& idx) const {
    return at_(key_type{idx.begin(), idx.end()});
}

inline void SparseMap::add_to_domain(size_type ind, size_type dep) {
    add_to_domain(key_type{ind}, key_type{dep});
}

inline void SparseMap::add_to_domain(size_type ind, key_type dep){
    add_to_domain(key_type{ind}, std::move(dep));
}

template<typename BeginItr, typename EndItr>
void SparseMap::add_to_domain(key_type ind, BeginItr first, EndItr last){
    if(first == last) throw std::runtime_error("Range can not be empty");
    std::for_each(first, last, [=](auto&& i){ add_to_domain(ind, i); });
}

inline bool SparseMap::operator!=(const SparseMap& rhs) const noexcept {
    return !((*this) == rhs);
}

} // end namespace
