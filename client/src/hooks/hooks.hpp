#pragma once

#include <string>
#include <vector>

namespace hot_spotter::hooks {
bool initHooks();
bool removeHooks();

// Drain any class names intercepted by the hook. This is thread-safe and
// intended to be called from the GUI/main thread to process classes
// captured on the hook thread.
std::vector<std::string> DrainPendingClasses();
} // namespace hot_spotter::hooks
