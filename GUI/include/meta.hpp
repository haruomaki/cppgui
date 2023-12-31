#include <functional>
#include <utility>

// 可変長のis_same
template <class T, class... Ts>
using are_same = std::conjunction<std::is_same<T, Ts>...>;
template <class T, class... Ts>
constexpr bool are_same_v = std::conjunction<std::is_same<T, Ts>...>::value;

template <typename T = void, typename... Rest>
struct FirstType {
    using type = T;
};

// 型パラメータパックから先頭要素を取得
template <typename... Args>
using getFirstType = typename FirstType<Args...>::type;

// TODO: 引数が空のときvoidを返す
template <auto x, auto... rest>
struct FirstArgType {
    using type = decltype(x);
};

// 値パラメータパックから先頭要素の型を取得
template <auto... args>
using getFirstArgType = typename FirstArgType<args...>::type;

static_assert(std::is_same_v<getFirstArgType<1, 2, 3>, int>);
static_assert(std::is_same_v<getFirstArgType<true, 'a'>, bool>);
// static_assert(std::is_same_v<getFirstArgType<>, void>);

// メンバ関数ポインタ型から各型名を抽出する、特殊化フィルタ
template <typename T>
struct MemberFunctionPointerTypeFilter;
template <typename C, typename R, typename... Args>
struct MemberFunctionPointerTypeFilter<R (C::*)(Args...)> {
    using ClassType = C;
    using FirstArgType = getFirstType<Args...>;
    using ReturnType = R;
};
template <typename C, typename R, typename... Args>
struct MemberFunctionPointerTypeFilter<R (C::*)(Args...) const> {
    using ClassType = C;
    using FirstArgType = getFirstType<Args...>;
    using ReturnType = R;
};
// メンバ関数ポインタ型から所属クラスを取得
template <typename Pointer>
using getMemberFunctionClass = typename MemberFunctionPointerTypeFilter<Pointer>::ClassType;
// メンバ関数ポインタ型から（第1引数の）引数型を取得
template <typename Pointer>
using getMemberFunctionArg = typename MemberFunctionPointerTypeFilter<Pointer>::FirstArgType;
// メンバ関数ポインタ型から戻り値型を取得
template <typename Pointer>
using getMemberFunctionRet = typename MemberFunctionPointerTypeFilter<Pointer>::ReturnType;

// T型とS型の値に二項演算子Opを適用した際の型を取得する
template <template <class = void> typename Op, typename T, typename S>
struct OperatorReturnTypeChecker {
    using type = decltype(Op()(std::declval<T>(), std::declval<S>()));
};
template <template <class> typename Op, typename T, typename S>
using getOperatorResult = typename OperatorReturnTypeChecker<Op, T, S>::type;

// xとyに二項演算子Opを適用した結果を返す
template <template <class = void> typename Op, auto x, auto y>
constexpr auto applyOperator = Op()(x, y); // NOLINT(readability-identifier-naming)

static_assert(applyOperator<std::plus, 8, 5> == 13); // 8 + 5 = 13 をコンパイル時に計算
