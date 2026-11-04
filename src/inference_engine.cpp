#include <coursework/inference_engine.hpp>

namespace coursework {

ml::Tensor<float> InferenceEngine::run(const std::shared_ptr<ml::Model>& model,
                                       const ml::Tensor<float>& input) const {
    if (!model) {
        throw std::invalid_argument("Model cannot be null");
    }

    return model->forward(input);
}

}  // namespace coursework