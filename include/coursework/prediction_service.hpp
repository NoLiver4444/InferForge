#pragma once

#include <string>

#include <coursework/inference_engine.hpp>
#include <coursework/model_manager.hpp>

#include <ml/tensor/tensor.hpp>

namespace coursework {

class PredictionService {
private:
    ModelManager& model_manager_;
    InferenceEngine& inference_engine_;

public:
    PredictionService(ModelManager& model_manager, InferenceEngine& inference_engine);

    ml::Tensor<float> predict(const std::string& model_id, const ml::Tensor<float>& input) const;
};

}  // namespace coursework