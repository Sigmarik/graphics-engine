#include "shouter.h"

#include "logger/logger.h"

ShouterComponent::ShouterComponent(const std::string& name)
    : name_(name), input_([name = name_](const std::string& message) {
          log_dup(ABSOLUTE_IMPORTANCE, "output", "%s: %s\n", name.c_str(),
                  message.c_str());
      }) {
    register_input("shout", input_);
}
