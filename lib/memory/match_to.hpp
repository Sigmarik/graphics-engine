/**
 * @file match_to.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Match<...>::To<...>::From<...> syntax definition
 * @version 0.1
 * @date 2024-07-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <optional>

#include "abstract_holder.hpp"

/**
 * @brief Runtime class-to-class map.
 * @brief
 *
 * @tparam Ts
 */
template <class... Ts>
struct Match {
    template <class Root, class... Us>
    struct To {
        template <class... Vs>
        struct From {
            using Actor = std::function<Root*(Vs... args)>;

            template <class Source>
            static std::optional<Actor> constructor(const Source* initiator) {
                std::size_t match_id = 0;

                bool loop1 =
                    (check_and_iterate<Ts>(initiator, match_id) || ...);

                if (!loop1) return {};

                Actor result;

                std::size_t output_iterator = 0;

                bool loop2 =
                    (count_and_iterate<Us>(result, match_id, output_iterator) ||
                     ...);

                assert(loop2);

                return result;
            }

           private:
            template <class CurrentClass, class Source>
            static bool check_and_iterate(const Source* initiator,
                                          std::size_t& iterator) {
                if (!dynamic_cast<const CurrentClass*>(initiator)) {
                    ++iterator;
                    return false;
                }

                return true;
            }

            template <class CurrentClass>
            static bool count_and_iterate(Actor& output, std::size_t index,
                                          std::size_t& iterator) {
                if (index != iterator) {
                    ++iterator;
                    return false;
                }

                output = [](Vs... args) -> Root* {
                    return static_cast<Root*>(
                        new CurrentClass(std::forward<Vs>(args)...));
                };

                return true;
            }
        };
    };
};

#define COMMA ,

#define MATCH_TO_FROM(what, to, from) \
    Match##what::To##to::From##from::constructor
