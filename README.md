# InferForge

InferForge is a C++17 inference server project built around a custom machine learning library.

The project is developed as part of a coursework focused on the architecture of a client-server system for machine learning model inference.

## Project goal

The main goal of InferForge is to build a modular inference system that can:

- load and manage machine learning models;
- execute inference through a custom C++ ML library;
- expose predictions through a server API;
- collect execution statistics;
- separate ML logic from server and application logic.

The ML library is maintained as a separate project and connected to InferForge as a dependency.

## Architecture

The planned architecture is:

```text
Client
  ↓
HTTP / REST API
  ↓
RequestHandler
  ↓
PredictionService
  ↓
ModelManager + InferenceEngine
  ↓
Model
  ↓
Sequential
  ↓
Linear / ReLU / Sigmoid
  ↓
Tensor
```

Responsibilities are separated between components.

### ModelManager

Responsible for:

- registering models;
- searching models by ID;
- removing models;
- managing model lifetime.

Models are stored through `std::shared_ptr`, allowing an inference request to safely keep using a model even if it is removed from the registry during execution.

### InferenceEngine

Responsible for executing model inference.

At the current stage it delegates execution to:

```cpp
model->forward(input);
```

Later it can be extended with:

- latency measurements;
- batch inference;
- execution statistics;
- preprocessing and postprocessing;
- thread pools.

### PredictionService

Will coordinate the prediction use case:

```text
model lookup
    ↓
input preparation
    ↓
inference
    ↓
result
```

### RequestHandler

Will be responsible for interaction with the HTTP layer:

- request parsing;
- basic validation;
- calling `PredictionService`;
- response formatting.

## ML library

InferForge uses a separate custom C++ machine learning library.

The library currently contains:

```text
Vector<T>
Matrix<T>
Tensor<T>

Layer<T>
├── Linear<T>
├── ReLU<T>
└── Sigmoid<T>

Sequential<T>
Model
```

The library supports basic feed-forward inference.

Example network:

```text
Input
  ↓
Linear
  ↓
ReLU
  ↓
Linear
  ↓
Sigmoid
  ↓
Prediction
```

## Project structure

```text
InferForge/
├── CMakeLists.txt
├── .clang-format
├── .gitignore
├── README.md
│
├── include/
│   └── coursework/
│       ├── model_manager.hpp
│       ├── inference_engine.hpp
│       ├── prediction_service.hpp
│       └── request_handler.hpp
│
├── src/
│   ├── main.cpp
│   ├── model_manager.cpp
│   ├── inference_engine.cpp
│   ├── prediction_service.cpp
│   └── request_handler.cpp
│
└── test/
    ├── model_manager_test.cpp
    ├── inference_engine_test.cpp
    └── prediction_service_test.cpp
```

The custom ML library is stored separately:

```text
Workspace/
├── ml/
└── InferForge/
```

InferForge currently connects it through CMake using:

```cmake
add_subdirectory(
    ../ml
    ${CMAKE_BINARY_DIR}/ml
)
```

## Requirements

- C++17
- CMake 3.16+
- GCC or Clang
- GoogleTest
- clang-format

Optional development tools:

- AddressSanitizer
- UndefinedBehaviorSanitizer
- clang-tidy
- gcovr

## Build

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/coursework
```

## Tests

Run all tests:

```bash
ctest --test-dir build --output-on-failure
```

Or run GoogleTest directly:

```bash
./build/coursework_tests
```

Run only `ModelManager` tests:

```bash
./build/coursework_tests --gtest_filter="ModelManagerTest.*"
```

## Formatting

Format InferForge source files:

```bash
cmake --build build --target coursework-format
```

Check formatting without modifying files:

```bash
cmake --build build --target coursework-format-check
```

## Current status

Implemented:

- separate custom ML library integration;
- `Model`;
- `ModelManager`;
- model registration;
- model lookup by ID;
- model removal;
- duplicate model detection;
- shared ownership model lifetime behavior;
- unit tests for `ModelManager`.

In progress:

- `InferenceEngine`.

Planned:

- `PredictionService`;
- `RequestHandler`;
- REST API;
- model loading;
- statistics collection;
- PostgreSQL integration;
- frontend;
- Docker;
- multithreaded inference;
- batching and performance benchmarks.

## Coursework topic

The project is developed within the coursework topic:

> Разработка и обоснование архитектуры клиент-серверной системы для выполнения инференса моделей машинного обучения с использованием собственной библиотеки на языке C++.

## License

This project is currently intended for educational and coursework purposes.