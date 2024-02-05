#include <functional>
#include <map>

using FunctionId = int;

template <typename Func>
class FunctionSet {
    std::map<FunctionId, std::function<Func>> functions_;
    FunctionId function_id_counter_ = 0;

  public:
    FunctionId set_function(std::function<Func> &&func) {
        this->functions_[this->function_id_counter_] = std::move(func);
        return function_id_counter_++;
    }

    // 範囲ベースforで回せるように
    auto begin() const { return this->functions_.begin(); }
    auto end() const { return this->functions_.end(); }
};