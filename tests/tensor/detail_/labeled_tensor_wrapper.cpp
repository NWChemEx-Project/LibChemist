#include "libchemist/ta_helpers/ta_helpers.hpp"
#include "libchemist/tensor/detail_/labeled_tensor_wrapper.hpp"
#include "libchemist/tensor/tensor_wrapper.hpp"
#include "libchemist/tensor/types.hpp"
#include <catch2/catch.hpp>

using namespace libchemist::tensor;

TEMPLATE_LIST_TEST_CASE("LabeledTensorWrapper", "", tensor_variant_t) {
    auto& world    = TA::get_default_world();
    using TWrapper = TensorWrapper<tensor_variant_t>;
    using t_type   = TestType;

    TWrapper vec(t_type(world, {1.0, 2.0, 3.0}));
    TWrapper mat(t_type(world, {{1.0, 2.0}, {3.0, 4.0}}));
    TWrapper t3(
      t_type(world, {{{1.0, 2.0}, {3.0, 4.0}}, {{5.0, 6.0}, {7.0, 8.0}}}));

    auto lvec = vec("i");
    TWrapper result(t_type{});
    SECTION("operator+") {
        result("i") = lvec + lvec;
        auto& rv    = result.get<t_type>();
        t_type corr(world, {2.0, 4.0, 6.0});
        REQUIRE(libchemist::ta_helpers::allclose(rv, corr));
    }

    SECTION("operator*") {
        result("i") = lvec * lvec;
        t_type corr(world, {1.0, 4.0, 9.0});
        std::cout << result << std::endl;
    }
}