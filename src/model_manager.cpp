#include <coursework/model_manager.hpp>
#include <stdexcept>

namespace coursework {

void ModelManager::add(std::shared_ptr<Model> model) {
    if (!model) {
        throw std::invalid_argument("Model cannot be null");
    }

    const std::string& id = model->id();

    if (models_.find(id) != models_.end()) {
        throw std::invalid_argument("Model with this id already exists");
    }

    models_.emplace(id, std::move(model));
}

std::shared_ptr<Model> ModelManager::get(const std::string& id) const {
    const auto it = models_.find(id);

    if (it == models_.end()) {
        throw std::out_of_range("A model with this ID does not exist");
    }

    return it->second;
}

void ModelManager::remove(const std::string& id) {
    const auto it = models_.find(id);

    if (it == models_.end()) {
        throw std::out_of_range("A model with this ID does not exist");
    }

    models_.erase(it);
}

bool ModelManager::contains(const std::string& id) const {
    return models_.find(id) != models_.end();
}

std::size_t ModelManager::size() const noexcept {
    return models_.size();
}

bool ModelManager::empty() const noexcept {
    return models_.empty();
}

}  // namespace coursework