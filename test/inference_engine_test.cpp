#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <utility>

#include <coursework/inference_engine.hpp>

#include <ml/layers/linear.hpp>
#include <ml/models/model.hpp>
#include <ml/models/sequential.hpp>
#include <ml/tensor/tensor.hpp>

using namespace coursework;

TEST(InferenceEngineTest, RunsModelForward) {
    ml::Tensor<float> weights({2, 1});
    ml::Tensor<float> bias({1});

    weights(0, 0) = 2.0f;
    weights(1, 0) = 3.0f;

    bias(0) = -1.0f;

    ml::Sequential<float> network;

    network.add(std::make_unique<ml::Linear<float>>(weights, bias));

    auto model = std::make_shared<ml::Model>("test_model", std::move(network));

    ml::Tensor<float> input({1, 2});

    input(0, 0) = 2.0f;
    input(0, 1) = 1.0f;

    InferenceEngine engine;

    auto result = engine.run(model, input);

    // 2 * 2 + 1 * 3 - 1 = 6
    ASSERT_EQ(result.ndim(), 2);
    ASSERT_EQ(result.shape()[0], 1);
    ASSERT_EQ(result.shape()[1], 1);

    EXPECT_FLOAT_EQ(result(0, 0), 6.0f);
}

TEST(InferenceEngineTest, NullModelThrows) {
    InferenceEngine engine;

    std::shared_ptr<ml::Model> model = nullptr;

    ml::Tensor<float> input({1, 2});

    EXPECT_THROW(engine.run(model, input), std::invalid_argument);
}

TEST(InferenceEngineTest, DoesNotModifyInput) {
    ml::Tensor<float> weights({2, 1});
    ml::Tensor<float> bias({1});

    weights(0, 0) = 1.0f;
    weights(1, 0) = 1.0f;
    bias(0) = 0.0f;

    ml::Sequential<float> network;

    network.add(std::make_unique<ml::Linear<float>>(weights, bias));

    auto model = std::make_shared<ml::Model>("test_model", std::move(network));

    ml::Tensor<float> input({1, 2});

    input(0, 0) = 5.0f;
    input(0, 1) = -2.0f;

    InferenceEngine engine;

    auto result = engine.run(model, input);

    EXPECT_FLOAT_EQ(result(0, 0), 3.0f);

    EXPECT_FLOAT_EQ(input(0, 0), 5.0f);
    EXPECT_FLOAT_EQ(input(0, 1), -2.0f);
}