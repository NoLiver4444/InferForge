#include <coursework/request_handler.hpp>
#include <string>

namespace coursework {

class JsonConverter {
public:
    PredictionRequest parsePredictionRequest(const std::string& json_text) const;

    std::string serializePredictionResponse(const PredictionResponse& response) const;
};

}  // namespace coursework