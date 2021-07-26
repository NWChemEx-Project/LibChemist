/** @brief tensor_wrapper.ipp
 *
 *  Inline implementations for the TensorWrapper class. This file should not be
 *  explicitly included in any other file aside from tensor_wrapper.hpp.
 */

#pragma once

namespace libchemist::tensor {
#define TENSOR_WRAPPER TensorWrapper<VariantType>

template<typename VariantType>
auto TENSOR_WRAPPER::operator()(const annotation_type& annotation) {
    return labeled_tensor_type(annotation, *this);
}

template<typename VariantType>
auto TENSOR_WRAPPER::operator()(const annotation_type& annotation) const {
    return const_labeled_tensor_type(annotation, *this);
}

template<typename VariantType>
auto TENSOR_WRAPPER::make_annotation(const annotation_type& letter) const {
    auto r          = rank();
    auto outer_rank = (is_tot_() ? outer_rank_() : r);
    annotation_type x;
    if(r == 0) return x;
    for(decltype(r) i = 0; i < r - 1; ++i) {
        x += letter + std::to_string(i);
        x += (i + 1 == outer_rank ? ";" : ",");
    }
    x += letter + std::to_string(r - 1);
    return x;
}

template<typename VariantType>
auto TENSOR_WRAPPER::rank() const {
    return outer_rank_() + inner_rank_();
}

template<typename VariantType>
auto TENSOR_WRAPPER::extents() const {
    auto l = [&](auto&& arg) {
        extents_type rv;
        if(!arg.is_initialized()) return rv;
        const auto& tr = arg.trange();
        for(size_type i = 0; i < tr.rank(); ++i)
            rv.push_back(tr.dim(i).extent());
        return rv;
    };
    return std::visit(l, m_tensor_);
}

template<typename VariantType>
std::ostream& TENSOR_WRAPPER::print(std::ostream& os) const {
    auto l = [&](auto&& arg) { os << arg; };
    std::visit(l, m_tensor_);
    return os;
}

template<typename VariantType>
void TENSOR_WRAPPER::hash(sde::Hasher& h) const {
    auto l = [&](auto&& arg) { h(arg); };
    std::visit(l, m_tensor_);
}

template<typename VariantType>
template<typename RHSType>
bool TENSOR_WRAPPER::operator==(const TensorWrapper<RHSType>& rhs) const {
    auto l = [&](auto&& lhs) {
        auto m = [&](auto&& rhs) { return lhs == rhs; };
        return std::visit(m, rhs.m_tensor_);
    };
    return std::visit(l, m_tensor_);
}

template<typename VariantType>
bool TENSOR_WRAPPER::is_tot_() const noexcept {
    auto l = [](auto&& arg) {
        using clean_t = std::decay_t<decltype(arg)>;
        return TensorTraits<clean_t>::is_tot;
    };
    return std::visit(l, m_tensor_);
}

template<typename VariantType>
auto TENSOR_WRAPPER::outer_rank_() const noexcept {
    auto l = [](auto&& arg) {
        return arg.is_initialized() ? TA::rank(arg) : 0;
    };
    return std::visit(l, m_tensor_);
}

template<typename VariantType>
auto TENSOR_WRAPPER::inner_rank_() const {
    auto l = [](auto&& arg) {
        using clean_t   = std::decay_t<decltype(arg)>;
        using size_type = decltype(std::declval<clean_t>().range().rank());
        constexpr bool is_tot = TensorTraits<clean_t>::is_tot;
        if constexpr(!is_tot)
            return size_type{0};
        else {
            if(!arg.is_initialized()) return size_type{0};
            const auto& tile0 = arg.begin()->get();
            return size_type{tile0[0].range().rank()};
        }
    };
    return std::visit(l, m_tensor_);
}

#undef TENSOR_WRAPPER
} // namespace libchemist::tensor