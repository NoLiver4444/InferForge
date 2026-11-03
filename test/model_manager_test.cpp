#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <coursework/model_manager.hpp>

#include <ml/layers/linear.hpp>
#include <ml/models/model.hpp>
#include <ml/models/sequential.hpp>

using namespace coursework;

namespace {

std::shared_ptr<ml::Model> makeModel(const std::string& id) {
    ml::Sequential<float> network;

    network.add(std::make_unique<ml::Linear<float>>(2, 1));

    return std::make_shared<ml::Model>(id, std::move(network));
}

}  // namespace

TEST(ModelManagerTest, AddAndGetModel) {
    ModelManager manager;

    auto model = makeModel("model_1");

    manager.add(model);

    auto result = manager.get("model_1");

    EXPECT_EQ(result, model);
    EXPECT_EQ(result->id(), "model_1");
}

TEST(ModelManagerTest, AddDuplicateIdThrows) {
    ModelManager manager;

    auto model_1 = makeModel("model_1");
    auto model_2 = makeModel("model_1");

    manager.add(model_1);

    EXPECT_THROW(manager.add(model_2), std::invalid_argument);
}

TEST(ModelManagerTest, AddNullModelThrows) {
    ModelManager manager;

    EXPECT_THROW(manager.add(nullptr), std::invalid_argument);
}

TEST(ModelManagerTest, GetMissingModelThrows) {
    ModelManager manager;

    EXPECT_THROW(manager.get("unknown"), std::out_of_range);
}

TEST(ModelManagerTest, RemoveModel) {
    ModelManager manager;

    auto model = makeModel("model_1");

    manager.add(model);

    EXPECT_TRUE(manager.contains("model_1"));

    manager.remove("model_1");

    EXPECT_FALSE(manager.contains("model_1"));
    EXPECT_TRUE(manager.empty());
}

TEST(ModelManagerTest, RemoveMissingModelThrows) {
    ModelManager manager;

    EXPECT_THROW(manager.remove("unknown"), std::out_of_range);
}

TEST(ModelManagerTest, ContainsSizeAndEmpty) {
    ModelManager manager;

    EXPECT_TRUE(manager.empty());
    EXPECT_EQ(manager.size(), 0);

    manager.add(makeModel("model_1"));

    EXPECT_FALSE(manager.empty());
    EXPECT_TRUE(manager.contains("model_1"));
    EXPECT_FALSE(manager.contains("model_2"));
    EXPECT_EQ(manager.size(), 1);

    manager.add(makeModel("model_2"));

    EXPECT_TRUE(manager.contains("model_2"));
    EXPECT_EQ(manager.size(), 2);
}

TEST(ModelManagerTest, ModelRemainsAliveAfterRemoveIfSharedPointerExists) {
    ModelManager manager;

    manager.add(makeModel("model_1"));

    auto model = manager.get("model_1");

    manager.remove("model_1");

    EXPECT_FALSE(manager.contains("model_1"));

    ASSERT_NE(model, nullptr);
    EXPECT_EQ(model->id(), "model_1");
}