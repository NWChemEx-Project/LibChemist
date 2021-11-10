#include "libchemist/ta_helpers/ta_helpers.hpp"
#include "libchemist/tensor/allocators/allocators.hpp"
#include "libchemist/tensor/types.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Allocator") {
    using variant_type = libchemist::tensor::type::tensor_variant;

    // Assumes default_allocator will just use the first type in the variant
    using tensor_type = std::variant_alternative_t<0, variant_type>;

    auto palloc = libchemist::tensor::default_allocator<variant_type>();

    using allocator_type = typename decltype(palloc)::element_type;
    using extents_type   = typename allocator_type::extents_type;

    // Assumes allocator uses this world too
    auto& world = TA::get_default_world();

    SECTION("new_tensor(shape)") {
        extents_type shape0{2};

        auto t = palloc->new_tensor(shape0);

        // b/c the tensor isn't initialized we can't directly compare them, so
        // instead we compare the tiled ranges

        auto corr = palloc->make_tiled_range(shape0);
        REQUIRE(std::get<0>(t).trange() == corr);
    }

    SECTION("new_tensor(vector)") {
        auto t  = palloc->new_tensor({1.0, 2.0, 3.0});
        auto tr = palloc->make_tiled_range(extents_type{3});
        tensor_type corr(world, tr, {1.0, 2.0, 3.0});
        REQUIRE(corr == std::get<0>(t));
    }

    SECTION("new_tensor(matrix)") {
        auto t  = palloc->new_tensor({{1.0, 2.0}, {3.0, 4.0}});
        auto tr = palloc->make_tiled_range(extents_type{2, 2});
        tensor_type corr(world, tr, {{1.0, 2.0}, {3.0, 4.0}});
        REQUIRE(corr == std::get<0>(t));
    }

    SECTION("new_tensor(rank 3 tensor)") {
        auto t = palloc->new_tensor(
          {{{1.0, 2.0}, {3.0, 4.0}}, {{5.0, 6.0}, {7.0, 8.0}}});
        auto tr = palloc->make_tiled_range(extents_type{2, 2, 2});
        tensor_type corr(world, tr,
                         {{{1.0, 2.0}, {3.0, 4.0}}, {{5.0, 6.0}, {7.0, 8.0}}});
        REQUIRE(corr == std::get<0>(t));
    }

    SECTION("new_tensor(rank 4 tensor)") {
        auto t = palloc->new_tensor(
          {{{{1.0, 2.0}, {3.0, 4.0}}, {{5.0, 6.0}, {7.0, 8.0}}},
           {{{9.0, 10.0}, {11.0, 12.0}}, {{13.0, 14.0}, {15.0, 16.0}}}});
        auto tr = palloc->make_tiled_range(extents_type{2, 2, 2, 2});
        tensor_type corr(
          world, tr,
          {{{{1.0, 2.0}, {3.0, 4.0}}, {{5.0, 6.0}, {7.0, 8.0}}},
           {{{9.0, 10.0}, {11.0, 12.0}}, {{13.0, 14.0}, {15.0, 16.0}}}});
        REQUIRE(corr == std::get<0>(t));
    }

    SECTION("Comparisons") {
        const auto prhs = libchemist::tensor::default_allocator<variant_type>();
        REQUIRE(*palloc == *prhs);
        REQUIRE_FALSE(*palloc != *prhs);

        REQUIRE(palloc->is_equal(*prhs));
    }
}