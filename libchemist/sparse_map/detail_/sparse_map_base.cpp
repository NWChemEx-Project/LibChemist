#include "libchemist/sparse_map/detail_/sparse_map_base.hpp"
#include "libchemist/sparse_map/sparse_map_class.hpp"

#define SPARSEMAPBASE SparseMapBase<DerivedType, IndIndex, DepIndex>

namespace libchemist::sparse_map::detail_ {

template<typename IndIndex, typename DepIndex>
struct SparseMapPIMPL {
    using my_type     = SparseMapPIMPL<IndIndex, DepIndex>;
    using key_type    = IndIndex;
    using mapped_type = Domain<DepIndex>;
    std::map<key_type, mapped_type> m_sm;

    std::unique_ptr<my_type> clone() const {
        return std::make_unique<my_type>(*this);
    }
};

//------------------------------------------------------------------------------
//                            CTors
//------------------------------------------------------------------------------

template<typename DerivedType, typename IndIndex, typename DepIndex>
SPARSEMAPBASE::SparseMapBase() : m_pimpl_(new_pimpl_()) {}

template<typename DerivedType, typename IndIndex, typename DepIndex>
SPARSEMAPBASE::SparseMapBase(il_t il) :
  SparseMapBase() {
    for(auto&& [k, v] : il) (*this)[k] = mapped_type(v);
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
SPARSEMAPBASE::SparseMapBase(const SparseMapBase& rhs) :
  m_pimpl_(rhs.m_pimpl_ ? rhs.pimpl_().clone() : new_pimpl_()) {}

template<typename DerivedType, typename IndIndex, typename DepIndex>
SPARSEMAPBASE::SparseMapBase(SparseMapBase&&) noexcept = default;

template<typename DerivedType, typename IndIndex, typename DepIndex>
SPARSEMAPBASE& SPARSEMAPBASE::operator=(const SparseMapBase& rhs) {
    if(this == &rhs) return *this;
    rhs.pimpl_().clone().swap(m_pimpl_);
    return *this;
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
SPARSEMAPBASE& SPARSEMAPBASE::operator=(SparseMapBase&&) noexcept = default;

template<typename DerivedType, typename IndIndex, typename DepIndex>
SPARSEMAPBASE::~SparseMapBase() noexcept = default;

//------------------------------------------------------------------------------
//                                 Accessors
//------------------------------------------------------------------------------

template<typename DerivedType, typename IndIndex, typename DepIndex>
typename SPARSEMAPBASE::size_type SPARSEMAPBASE::size() const noexcept {
    if(!m_pimpl_) return 0;
    return pimpl_().m_sm.size();
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
bool SPARSEMAPBASE::count(const key_type& i) const noexcept {
    if(!m_pimpl_) return false;
    return pimpl_().m_sm.count(i);
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
typename SPARSEMAPBASE::size_type SPARSEMAPBASE::ind_rank() const noexcept {
    if(!m_pimpl_ || pimpl_().m_sm.empty()) return 0;
    return pimpl_().m_sm.begin()->first.size();
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
typename SPARSEMAPBASE::size_type SPARSEMAPBASE::dep_rank() const noexcept {
    if(!m_pimpl_ || pimpl_().m_sm.empty()) return 0;
    for(auto [k, v] : pimpl_().m_sm)
        if(v.rank() > 0) return v.rank();
    return 0;
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
void SPARSEMAPBASE::add_to_domain(const key_type& key, DepIndex value) {
    check_ind_rank_(key.size());
    check_dep_rank_(value.size());
    if(!m_pimpl_) m_pimpl_ = new_pimpl_();
    pimpl_().m_sm[key].insert(std::move(value));
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
typename SPARSEMAPBASE::mapped_type&
SPARSEMAPBASE::operator[](const key_type& key) {
    check_ind_rank_(key.size());
    if(!m_pimpl_) m_pimpl_ = new_pimpl_();
    return pimpl_().m_sm[key];
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
const typename SPARSEMAPBASE::mapped_type&
SPARSEMAPBASE::operator[](const key_type& key) const {
    check_ind_rank_(key.size());
    check_contains_(key);
    return pimpl_().m_sm.at(key);
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
typename SPARSEMAPBASE::mapped_type&
SPARSEMAPBASE::at(const key_type& key) {
    check_contains_(key);
    return (*this)[key];
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
typename SPARSEMAPBASE::iterator SPARSEMAPBASE::begin() {
    return pimpl_().m_sm.begin();
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
typename SPARSEMAPBASE::const_iterator SPARSEMAPBASE::begin() const {
    return pimpl_().m_sm.begin();
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
typename SPARSEMAPBASE::iterator SPARSEMAPBASE::end() {
    return pimpl_().m_sm.end();
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
typename SPARSEMAPBASE::const_iterator SPARSEMAPBASE::end() const {
    return pimpl_().m_sm.end();
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
DerivedType SPARSEMAPBASE::operator*(const SparseMapBase& rhs) const {
    DerivedType rv(downcast_());
    rv *= rhs;
    return rv;
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
DerivedType& SPARSEMAPBASE::operator*=(const SparseMapBase& rhs) {
    if(!m_pimpl_ || empty()) return downcast_();
    if(!rhs.m_pimpl_ || rhs.empty()){
        m_pimpl_ = new_pimpl_();
        return downcast_();
    }
    auto new_pimpl = new_pimpl_();
    using vector_type = std::vector<size_type>;
    auto new_rank = ind_rank() + rhs.ind_rank();
    for(auto [lkey, lval] : *this){
        for(const auto& [rkey, rval] : rhs) {
            vector_type new_index;
            new_index.reserve(new_rank);
            new_index.insert(new_index.end(), lkey.begin(), lkey.end());
            new_index.insert(new_index.end(), rkey.begin(), rkey.end());
            new_pimpl->m_sm[key_type(new_index)] = lval * rval;
        }
    }
    m_pimpl_.swap(new_pimpl);
    return downcast_();
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
DerivedType& SPARSEMAPBASE::operator^=(const SparseMapBase& rhs) {
    if(!m_pimpl_ || empty()) return downcast_();
    if(!rhs.m_pimpl_ || rhs.empty()){
        m_pimpl_ = new_pimpl_();
        return downcast_();
    }
    auto new_pimpl = new_pimpl_();

    // Check for the same ranks, if they don't have the same rank the
    // intersection is empty.
    if(ind_rank() != rhs.ind_rank() || dep_rank() != rhs.dep_rank()){
        m_pimpl_.swap(new_pimpl);
        return downcast_();
    }

    for(const auto& [lind, ldomain] : *this){
        if(rhs.count(lind)) new_pimpl->m_sm[lind] = ldomain ^ rhs.at(lind);
    }
    m_pimpl_.swap(new_pimpl);

    return downcast_();
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
DerivedType SPARSEMAPBASE::operator^(const SparseMapBase& rhs) const {
    DerivedType rv(downcast_());
    rv ^= rhs;
    return rv;
}

//template<typename DerivedType, typename IndIndex, typename DepIndex>
//SPARSEMAPBASE SPARSEMAPBASE::chain(const SparseMapBase& sm) const {
//    if (this->dep_rank() != sm.ind_rank())
//        throw std::runtime_error("Incompatible index ranks between chained maps");
//    SparseMapBase rv;
//    for (const auto& x : *this) {
//        for (const auto y : x.second) {
//            if (sm.count(y)) {
//                for (const auto z : sm.at(y)) {
//                    rv.add_to_domain(x.first, z);
//                }
//            }
//        }
//    }
//    return rv;
//}

template<typename DerivedType, typename IndIndex, typename DepIndex>
DerivedType& SPARSEMAPBASE::operator+=(const SparseMapBase& rhs) {
    if(empty()){
        if(!rhs.empty()) rhs.pimpl_().clone().swap(m_pimpl_);
        return downcast_();
    }
    else if(rhs.empty()) return downcast_();

    // We know neither of us are empty, check for compatible indices
    if(ind_rank() != rhs.ind_rank())
        throw std::runtime_error("Independent index ranks are not the same");
    else if(dep_rank() != rhs.dep_rank())
        throw std::runtime_error("Dependent index ranks are not the same");

    //They're compatible do the union
    auto new_pimpl = pimpl_().clone();
    for(const auto& [ind, domain]: rhs) new_pimpl->m_sm[ind] += domain;
    m_pimpl_.swap(new_pimpl);

    return downcast_();
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
DerivedType SPARSEMAPBASE::operator+(const SparseMapBase& rhs) const {
    auto rv = DerivedType(downcast_());
    rv += rhs;
    return rv;
}

//template<typename DerivedType, typename IndIndex, typename DepIndex>
//SPARSEMAPBASE SPARSEMAPBASE::intersection(const SparseMapBase& sm) const {
//    if (!this->empty() && !sm.empty() &&
//        (this->dep_rank() != sm.dep_rank() || this->ind_rank() != sm.ind_rank()))
//        throw std::runtime_error("Incompatible index ranks between maps for intersection");
//    SparseMapBase rv;
//    for (const auto& x : *this) {
//        if (sm.count(x.first)) {
//            for (const auto y : x.second) {
//                if (sm.at(x.first).count(y)) {
//                    rv.add_to_domain(x.first,y);
//                }
//            }
//        }
//    }
//    return rv;
//}

template<typename DerivedType, typename IndIndex, typename DepIndex>
bool SPARSEMAPBASE::operator==(const SparseMapBase& rhs) const noexcept {
    if(!m_pimpl_) return !rhs.m_pimpl_ || rhs.empty();
    else if(!rhs.m_pimpl_) return empty();
    return pimpl_().m_sm == rhs.pimpl_().m_sm;
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
void SPARSEMAPBASE::hash(sde::Hasher& h) const {
    if(m_pimpl_) h(pimpl_().m_sm);
    else h(nullptr);
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
std::ostream& SPARSEMAPBASE::print(std::ostream& os) const {
    using utilities::printing::operator<<;
    os << pimpl_().m_sm;
    return os;
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
typename SPARSEMAPBASE::pimpl_type& SPARSEMAPBASE::pimpl_() {
    if(m_pimpl_) return *m_pimpl_;
    throw std::runtime_error("PIMPL not set. Did you move from this instance?");
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
const typename SPARSEMAPBASE::pimpl_type& SPARSEMAPBASE::pimpl_() const {
    if(m_pimpl_) return *m_pimpl_;
    throw std::runtime_error("PIMPL not set. Did you move from this instance?");
}

//------------------------------------------------------------------------------
//                            Private Methods
//------------------------------------------------------------------------------

template<typename DerivedType, typename IndIndex, typename DepIndex>
typename SPARSEMAPBASE::pimpl_ptr SPARSEMAPBASE::new_pimpl_() {
    return std::make_unique<pimpl_type>();
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
DerivedType& SPARSEMAPBASE::downcast_() {
    return static_cast<DerivedType&>(*this);
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
const DerivedType& SPARSEMAPBASE::downcast_() const {
    return static_cast<const DerivedType&>(*this);
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
void SPARSEMAPBASE::check_ind_rank_(size_type idx_rank) const {
    if(empty() || ind_rank() == idx_rank) return;
    using namespace std::literals;
    throw std::runtime_error(
      "Independent indices have ranks of "s + std::to_string(ind_rank()) +
      " but provided index has rank " + std::to_string(idx_rank) + "."s);
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
void SPARSEMAPBASE::check_dep_rank_(size_type idx_rank) const {
    if(empty() || dep_rank() == idx_rank) return;
    // We can also be okay if all the Domains are empty
    bool all_empty = true;
    for(auto [k, v] : pimpl_().m_sm){
        if(!v.empty()){
            all_empty = false;
            break;
        }
    }
    if(all_empty) return;
    using namespace std::literals;
    throw std::runtime_error(
      "Dependent indices have ranks of "s + std::to_string(dep_rank()) +
      " but provided index has rank " + std::to_string(idx_rank) + "."s);
}

template<typename DerivedType, typename IndIndex, typename DepIndex>
void SPARSEMAPBASE::check_contains_(const key_type& key) const {
    if(!m_pimpl_ || !pimpl_().m_sm.count(key)) {
        std::stringstream ss;
        ss << key;
        throw std::out_of_range("SparseMap does not contain key: " + ss.str());
    }
}

#undef SPARSEMAPBASE

//------------------------------------------------------------------------------
//                          Template Instantiations
//------------------------------------------------------------------------------

#define DEFINE_SPARSEMAPBASE(IndIndex, DepIndex) \
   template class SparseMapBase<SparseMap<IndIndex, DepIndex>, \
                                IndIndex, DepIndex>

DEFINE_SPARSEMAPBASE(ElementIndex, ElementIndex);
DEFINE_SPARSEMAPBASE(ElementIndex, TileIndex);
DEFINE_SPARSEMAPBASE(TileIndex, ElementIndex);
DEFINE_SPARSEMAPBASE(TileIndex, TileIndex);

#undef DEFINE_SPARSEMAPBASE

}