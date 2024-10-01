#pragma once

#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "TTask.hpp"

class TTaskScheduler {
    using TTaskId = size_t;

   private:
    TTaskId id_ = 0;
    std::unordered_map<TTaskId, std::vector<TTaskId>> dependencies_;
    std::vector<std::unique_ptr<TTaskBase>> task_list_;
    std::vector<bool> calculated_list_;

    template <typename T>
    void ParseImpl(const T& target, const TTaskId& id) {}

    template <typename T>
    void ParseImpl(const TFutureResult<T>& target, const TTaskId& id) {
        dependencies_[id].push_back(target.Id());
    }

    template <typename... Args>
    void Parse(const TTaskId& id, const Args&... args) {
        (ParseImpl(args, id), ...);
    }

    void Runner(const TTaskId& id) {
        if (calculated_list_[id]) return;
        for (const auto& dep : dependencies_[id]) {
            Runner(dep);
        }
        task_list_[id]->Run();
        calculated_list_[id] = true;
    }

   public:
    TTaskScheduler() = default;

    template <typename Func, typename... Args>
    TTaskId add(const Func& func, const Args&... args) {
        Parse(id_, args...);
        task_list_.emplace_back(
            std::make_unique<TTask<Func, Args...>>(id_, func, args...));
        calculated_list_.emplace_back(false);
        return id_++;
    }

    template <typename ResultType>
    auto getFutureResult(const TTaskId& id) {
        if (id >= task_list_.size()) {
            std::cerr << "\nError: wrong task id.\n";
            exit(EXIT_FAILURE);
        }
        return TFutureResult<ResultType>(task_list_[id]->getResult(),
                                         task_list_[id]->Id());
    }

    void executeAll() {
        for (auto& task : task_list_) {
            if (!calculated_list_[task->Id()]) {
                task->Run();
                calculated_list_[task->Id()] = true;
            }
        }
    }

    void Reset() {
        calculated_list_ = std::vector<bool>(calculated_list_.size(), false);
    }

    void PrintDependencies() {
        for (TTaskId i = 0; i < id_; ++i) {
            std::cout << i << ": ";
            for (TTaskId j : dependencies_[i]) {
                std::cout << j << " ";
            }
            std::cout << "\n";
        }
    }

    template <typename ResultType>
    ResultType getResult(const TTaskId& id) {
        if (id >= task_list_.size()) {
            std::cerr << "\nError: wrong task id.\n";
            exit(EXIT_FAILURE);
        }
        Runner(id);
        try {
            return task_list_[id]->getResult().template cast<ResultType>();
        } catch (const std::bad_cast&) {
            std::cerr << "\nError: wrong result type. Process id = "
                      << task_list_[id]->Id() << "."
                      << "\n";
            exit(EXIT_FAILURE);
        }
    }
};