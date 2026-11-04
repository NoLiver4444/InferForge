#include <coursework/json_converter.hpp>

#include <cstddef>
#include <string>
#include <ml/core/vector.hpp>

#include <nlohmann/json.hpp>
#include <ml/tensor/tensor.hpp>

namespace coursework {

using json = nlohmann::json;

PredictionRequest JsonConverter::parsePredictionRequest(const std::string& json_text) const {
    json j = json::parse(json_text);

    std::string model_id = j.at("model_id").get<std::string>();
    Vector<float> raw_input;

    for (const auto& value : j.at("input")) {
        raw_input.push_back(value.get<float>());
    }

    ml::Tensor<float> input({1, raw_input.size()});

    for (std::size_t i = 0; i < raw_input.size(); ++i) {
        input(0, i) = raw_input[i];
    }

    return {model_id, input};
}

std::string JsonConverter::serializePredictionResponse(const PredictionResponse& response) const {
    
}

}  // namespace coursework