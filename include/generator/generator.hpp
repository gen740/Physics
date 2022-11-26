// @file  generator.hpp
// @brief これは、C++20 を用いた Generator ベースのタスクです。
//        主に、
//
//        Flier::Task::Generator g = Flier::Task::empty_coroutine();
//
//        から初期化をして使います。

#pragma once

#include <concepts>
#include <coroutine>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

namespace Generator {

struct Generator;

//! get() で Generator に変換可能なオブジェクトを返すもの
template <class T>
concept CORO_GETTER = requires(T& x) {
                        { x.get() } -> std::convertible_to<Generator>;
                      };

//! Generator: co_yield, co_await を用いて、task
//! フローを管理することができるクラス
struct Generator {
  struct promise_type;
  using handle = std::coroutine_handle<promise_type>;
  struct promise_type {
    std::unique_ptr<Generator> gen_stack = nullptr;
    bool now_value = false;
    Generator* gen_wait = nullptr;
    static auto get_return_object_on_allocation_failure() {
      return Generator(nullptr);
    }
    auto get_return_object() { return Generator{handle::from_promise(*this)}; }
    auto initial_suspend() noexcept { return std::suspend_always{}; }
    auto final_suspend() noexcept { return std::suspend_always{}; }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}
    //  BUG: これは clang
    //  のバグ対策でしていることなので、バグが修正されたら戻すこと。
    auto yield_value([[maybe_unused]] bool value) {
      now_value = value;
      return std::suspend_always{};
    }
    auto await_transform(Generator&& gen) {
      struct Awaiter {
        std::coroutine_handle<promise_type> h_coro;
        bool await_ready() { return false; }
        auto await_suspend([[maybe_unused]] std::coroutine_handle<> h) {
          return true;
        }
        void await_resume() {}
        Awaiter() {}
      };
      if (gen_stack != nullptr && gen_stack->coro) {
        gen_stack->coro.destroy();
      }
      gen_stack = std::make_unique<Generator>(std::move(gen));
      gen_stack->task_name = nullptr;
      return Awaiter();
    };
    auto await_transform(Generator* gen) {
      struct Awaiter {
        std::coroutine_handle<promise_type> h_coro;
        bool await_ready() { return false; }
        auto await_suspend(
            [[maybe_unused]] std::coroutine_handle<promise_type> h) {
          h_coro = h;
          return true;
        }
        void await_resume() { h_coro.promise().gen_wait = nullptr; }
        Awaiter() {}
      };
      gen_wait = gen;
      return Awaiter();
    };
  };
  Generator() = delete;
  Generator(Generator& rhs) = delete;
  Generator(Generator&& rhs) noexcept : coro(rhs.coro) { rhs.coro = nullptr; }
  ~Generator() {
    if (coro) {
      coro = nullptr;
    }
  }

  bool next();

  //! 終了判定をするための関数
  bool done() { return coro.done(); }

  //! task を abort する
  void abort();

  //! task を置き換える
  auto replace(Generator&& rhs, const char* name) {
    if (task_name != nullptr) {
      if (!coro.done() && active) {
        task_name = nullptr;
        active = false;
      }
    }
    coro = std::exchange(rhs.coro, nullptr);
    task_name = name;
    return this;
  };

  template <CORO_GETTER T>
  auto replace(T&& once, const char* name) {
    if (task_name != nullptr) {
      if (!coro.done() && active) {
        task_name = nullptr;
        active = false;
      }
    }
    auto gen = once.get();
    coro = std::exchange(gen.coro, nullptr);
    task_name = name;
    return this;
  }

  // Generator の名前を取得する。
  const char* name() {
    if (task_name != nullptr) {
      return task_name;
    }
    return "Unnamed";
  }

  void* address() { return coro.address(); }

  //! 引数に渡された Genetator の実行を待ち、実行終了時に元の Genetator を再開
  //! する意図しない実装なので、ここはちゃんとする
  void wait_for(Generator&& gen) {
    if (coro.promise().gen_stack != nullptr &&
        coro.promise().gen_stack->done()) {
      coro.promise().gen_stack->wait_for(std::move(gen));
      return;
    }
    coro.promise().gen_stack = std::make_unique<Generator>(std::move(gen));
  }

 private:
  const char* task_name{};
  handle coro;
  Generator(handle h) : coro(h) {}

  bool active = false;
};

// 何もしないコルーチン（宣言だけしたい時とかに使う）
Generator empty_coroutine();

//! Utility Classes (できれば別ファイルに実装する方がいい)
//! この struct のようにして、関数（または他のもの）をラップして get() でジェネ
//! レーターを返す struct を定義すれば、 replace にコンストラクターを通じて右辺
//! 値を渡してあげることができる。
//! Times で特定回数繰り返すジェネレーターを実装したりできる

//! 一回しかfuncを実行しないジェネレーターを返す
//! @example: g.replace(Once{[](){ /* やりたい処理 */ }}, "task_name");
struct Once {
  Once() = delete;
  Once(Once&) = delete;
  Once(Once&&) = delete;
  Once& operator=(const Once&) = delete;
  Once& operator=(Once&&) = delete;
  Once(std::function<void()> func) : func(func) {}

 protected:
  friend struct Generator;
  std::function<void()> func;
  Generator get() {
    return [](auto func) -> Generator {
      func();
      co_return;
    }(std::move(func));
  }
};

//! 以降ずっとfuncを実行し続けるジェネレーターを返す
//! @example: g.replace(Loop{[](){ /* やりたい処理 */ }}, "task_name");
struct Loop {
  Loop() = delete;
  Loop(Loop&) = delete;
  Loop(Loop&&) = delete;
  Loop& operator=(const Loop&) = delete;
  Loop& operator=(Loop&&) = delete;
  explicit Loop(std::function<void()> func) : func(std::move(func)) {}

 protected:
  std::function<void()> func;
  friend struct Generator;
  Generator get() {
    return [](auto func) -> Generator {
      while (true) {
        func();
        co_yield true;
      }
    }(std::move(func));
  }
};

}  // namespace Luna
