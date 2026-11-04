#include <coursework/request_handler.hpp>

namespace coursework {

RequestHandler::RequestHandler(PredictionService& prediction_service)
    : prediction_service_(prediction_service) {}

PredictionResponse RequestHandler::handle(const PredictionRequest& request) const {
    PredictionResponse response;

    response.output = prediction_service_.predict(request.model_id, request.input);

    return response;
}

}  // namespace coursework