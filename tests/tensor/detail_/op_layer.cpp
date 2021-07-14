#include "libchemist/tensor/tensor.hpp"
#include <catch2/catch.hpp>

/* Testing strategy:
 *
 * OpLayer simply exposes two functions downcast. Here we instantiate a
 * LabeledTensorWrapper instance and make sure that downcast properly downcasts
 * to it.
 */

using namespace libchemist::tensor;

TEST_CASE("OpLayer") {
    using tensor         = type::SparseTensorWrapper;
    using labeled_tensor = detail_::LabeledTensorWrapper<tensor>;
    using base_type      = detail_::OpLayer<labeled_tensor>;

    auto& world = TA::get_default_world();
    tensor t(libchemist::type::tensor<double>(world, {1, 2, 3}));
    labeled_tensor lt("i", t);

    SECTION("downcast") {
        base_type& as_base(lt);
        labeled_tensor& as_derived = as_base.downcast();
        REQUIRE(&lt == &as_derived);
    }

    SECTION("downcast const") {
        const base_type& as_base(lt);
        const labeled_tensor& as_derived = as_base.downcast();
        REQUIRE(&lt == &as_derived);
    }
}