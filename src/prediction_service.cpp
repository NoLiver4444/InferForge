#include <coursework/prediction_service.hpp>

namespace coursework {

PredictionService::PredictionService(ModelManager& model_manager, InferenceEngine& inference_engine)
    : model_manager_(model_manager), inference_engine_(inference_engine) {}

ml::Tensor<float> PredictionService::predict(const std::string& model_id,
                                             const ml::Tensor<float>& input) const {
    auto model = model_manager_.get(model_id);

    return inference_engine_.run(model, input);
}

}  // namespace coursework