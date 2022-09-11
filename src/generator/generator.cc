#include <generator/generator.hpp>

namespace Generator {

bool Generator::next() {
  // coro_stack に実行していないものがあるのなら実行し、それ以外は普通に実行する
  if (coro.promise().gen_stack != nullptr &&
      !coro.promise().gen_stack->done()) {
    coro.promise().gen_stack->next();
    if (coro.promise().gen_stack == nullptr ||
        !coro.promise().gen_stack->done()) {
      return true;
    }
  }
  if (coro.promise().gen_wait != nullptr && !coro.promise().gen_wait->done()) {
    return true;
  }
  if (task_name != nullptr && !active) {  // 初めの一回のみ
    active = true;
  }
  if (!coro.done()) {  // 終わってないなら一個進める
    coro();
    if (coro.done() && active) {  // 終わってしまって初めの一回
      task_name = nullptr;
      active = false;
    }
    return true;
  }
  return false;
}

void Generator::abort() {
  // すでに終わっているなら置き換える必要はない
  if (coro.done()) {
    return;
  }
  auto gen = []() -> Generator { co_return; }();
  if (coro) {
    coro.destroy();
  }
  coro = std::exchange(gen.coro, nullptr);
  task_name = nullptr;
  active = false;
}

Generator empty_coroutine() { co_yield true; }

}  // namespace Generator
