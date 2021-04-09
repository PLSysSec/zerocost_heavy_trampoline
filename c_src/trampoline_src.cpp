
#include "ctx_save_trampoline.hpp"

#include <map>
// map is keyed by the heavy trampoline instance
static thread_local std::map<void*, TransitionContext*>  saved_transition_context;
static thread_local void* last_set_key;

TransitionContext* get_saved_transition_context(void* key) {
  auto iter = saved_transition_context.find(key);
  if (iter == saved_transition_context.end()) {
    return nullptr;
  } else {
    return iter->second;
  }
}

void set_saved_transition_context(void* key, TransitionContext* val) {
  last_set_key = key;
  if (val == nullptr) {
    auto iter = saved_transition_context.find(key);
    if (iter != saved_transition_context.end()) {
      saved_transition_context.erase(iter);
    }
  } else {
    saved_transition_context[key] = val;
  }
}

TransitionContext* get_saved_transition_context_with_last_set_key() {
  return get_saved_transition_context(last_set_key);
}
