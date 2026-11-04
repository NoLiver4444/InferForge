#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <coursework/inference_engine.hpp>
#include <coursework/model_manager.hpp>
#include <coursework/prediction_service.hpp>
#include <coursework/request_handler.hpp>

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

TEST(RequestHandlerTest, HandlesPredictionRequest) {
    ModelManager manager;
    InferenceEngine engine;

    manager.add(makeLinearModel("model_1", 2.0f, 3.0f, -1.0f));

    PredictionService service(manager, engine);
    RequestHandler handler(service);

    PredictionRequest request;

    request.model_id = "model_1";
    request.input = ml::Tensor<float>({1, 2});

    request.input(0, 0) = 2.0f;
    request.input(0, 1) = 1.0f;

    PredictionResponse response = handler.handle(request);

    ASSERT_EQ(response.output.ndim(), 2);
    ASSERT_EQ(response.output.shape()[0], 1);
    ASSERT_EQ(response.output.shape()[1], 1);

    // 2 * 2 + 1 * 3 - 1 = 6
    EXPECT_FLOAT_EQ(response.output(0, 0), 6.0f);
}

TEST(RequestHandlerTest, UnknownModelThrows) {
    ModelManager manager;
    InferenceEngine engine;

    PredictionService service(manager, engine);
    RequestHandler handler(service);

    PredictionRequest request;

    request.model_id = "unknown_model";
    request.input = ml::Tensor<float>({1, 2});

    EXPECT_THROW(handler.handle(request), std::out_of_range);
}

TEST(RequestHandlerTest, UsesRequestedModel) {
    ModelManager manager;
    InferenceEngine engine;

    manager.add(makeLinearModel("model_1", 1.0f, 1.0f, 0.0f));

    manager.add(makeLinearModel("model_2", 2.0f, 3.0f, -1.0f));

    PredictionService service(manager, engine);
    RequestHandler handler(service);

    PredictionRequest request;

    request.model_id = "model_2";
    request.input = ml::Tensor<float>({1, 2});

    request.input(0, 0) = 2.0f;
    request.input(0, 1) = 1.0f;

    auto response = handler.handle(request);

    EXPECT_FLOAT_EQ(response.output(0, 0), 6.0f);
}