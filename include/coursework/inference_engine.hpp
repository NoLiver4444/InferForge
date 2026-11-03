#pragma once

#include <memory>

#include <ml/models/model.hpp>
#include <ml/tensor/tensor.hpp>

namespace coursework {

class InferenceEngine {
public:
    ml::Tensor<float> run(const std::shared_ptr<ml::Model>& model,
                          const ml::Tensor<float>& input) const;
};

}  // namespace coursework