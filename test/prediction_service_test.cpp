#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <coursework/inference_engine.hpp>
#include <coursework/model_manager.hpp>
#include <coursework/prediction_service.hpp>

#include <ml/layers/linear.hpp>
#include <ml/models/model.hpp>
#include <ml/models/sequential.hpp>
#include <ml/tensor/tensor.hpp>

using namespace coursework;

namespace {

std::shared_ptr<ml::Model> makeLinearModel(const std::string& id, float w0, float w1,
                                           float bias_value) {
    ml::Tensor<float> weights({2, 1});
    ml::Tensor<float> bias({1});

    weights(0, 0) = w0;
    weights(1, 0) = w1;
    bias(0) = bias_value;

    ml::Sequential<float> network;

    network.add(std::make_unique<ml::Linear<float>>(weights, bias));

    return std::make_shared<ml::Model>(id, std::move(network));
}

}  // namespace

TEST(PredictionServiceTest, PredictReturnsModelResult) {
    ModelManager manager;
    InferenceEngine engine;

    manager.add(makeLinearModel("model_1", 2.0f, 3.0f, -1.0f));

    PredictionService service(manager, engine);

    ml::Tensor<float> input({1, 2});

    input(0, 0) = 2.0f;
    input(0, 1) = 1.0f;

    auto result = service.predict("model_1", input);

    // 2 * 2 + 1 * 3 - 1 = 6
    ASSERT_EQ(result.ndim(), 2);
    ASSERT_EQ(result.shape()[0], 1);
    ASSERT_EQ(result.shape()[1], 1);

    EXPECT_FLOAT_EQ(result(0, 0), 6.0f);
}

TEST(PredictionServiceTest, UnknownModelThrows) {
    ModelManager manager;
    InferenceEngine engine;

    PredictionService service(manager, engine);

    ml::Tensor<float> input({1, 2});

    EXPECT_THROW(service.predict("unknown_model", input), std::out_of_range);
}

TEST(PredictionServiceTest, UsesModelSelectedById) {
    ModelManager manager;
    InferenceEngine engine;

    manager.add(makeLinearModel("model_1", 1.0f, 1.0f, 0.0f));

    manager.add(makeLinearModel("model_2", 2.0f, 3.0f, -1.0f));

    PredictionService service(manager, engine);

    ml::Tensor<float> input({1, 2});

    input(0, 0) = 2.0f;
    input(0, 1) = 1.0f;

    auto result_1 = service.predict("model_1", input);

    auto result_2 = service.predict("model_2", input);

    // model_1: 2 * 1 + 1 * 1 = 3
    EXPECT_FLOAT_EQ(result_1(0, 0), 3.0f);

    // model_2: 2 * 2 + 1 * 3 - 1 = 6
    EXPECT_FLOAT_EQ(result_2(0, 0), 6.0f);
}