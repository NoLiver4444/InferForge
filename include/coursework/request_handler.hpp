#pragma once

#include <string>

#include <coursework/prediction_service.hpp>
#include <ml/tensor/tensor.hpp>

namespace coursework {

struct PredictionRequest {
    std::string model_id;
    ml::Tensor<float> input;
};

struct PredictionResponse {
    ml::Tensor<float> output;
};

class RequestHandler {
private:
    PredictionService& prediction_service_;

public:
    explicit RequestHandler(PredictionService& prediction_service);

    PredictionResponse handle(const PredictionRequest& request) const;
};

}  // namespace coursework