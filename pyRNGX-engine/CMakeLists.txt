cmake_minimum_required(VERSION 3.16)
project(fastflow-executor CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(fastflow_core
    core/scheduler/queue_manager.cpp
    core/executor/worker_pool.cpp
    connectors/kafka_connector.cpp
)

add_executable(service_chain examples/microservices/service_chain.cpp)
target_link_libraries(service_chain PRIVATE fastflow_core)
