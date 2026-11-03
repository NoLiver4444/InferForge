#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <ml/models/model.hpp>

using namespace ml;

namespace coursework {

class ModelManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Model>> models_;

public:
    void add(std::shared_ptr<Model> model);

    std::shared_ptr<Model> get(const std::string& id) const;

    void remove(const std::string& id);

    bool contains(const std::string& id) const;

    std::size_t size() const noexcept;

    bool empty() const noexcept;
};

}  // namespace coursework