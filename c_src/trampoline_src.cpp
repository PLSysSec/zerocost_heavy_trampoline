
#include "ctx_save_trampoline.hpp"

thread_local static TransitionContext* saved_transition_context = nullptr;

TransitionContext* get_saved_transition_context() {
  return saved_transition_context;
}

void set_saved_transition_context(TransitionContext* val) {
  saved_transition_context = val;
}
