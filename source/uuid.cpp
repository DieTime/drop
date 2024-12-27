/**
 * SPDX-FileCopyrightText: 2024 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#include "uuid.hpp"

#include <random>
#include <sstream>

namespace uuid {

std::string v4()
{
    std::mt19937_64 generator(std::random_device{}());

    std::uniform_int_distribution<int> from_0_to_15(0, 15);
    std::uniform_int_distribution<int> from_8_to_11(8, 11);

    std::stringstream uuid;
    uuid << std::hex;

    for (int i = 0; i < 8; i++) {
        uuid << from_0_to_15(generator);
    }

    uuid << "-";

    for (int i = 0; i < 4; i++) {
        uuid << from_0_to_15(generator);
    }

    uuid << "-4";

    for (int i = 0; i < 3; i++) {
        uuid << from_0_to_15(generator);
    }

    uuid << "-" << from_8_to_11(generator);

    for (int i = 0; i < 3; i++) {
        uuid << from_0_to_15(generator);
    }

    uuid << "-";

    for (int i = 0; i < 12; i++) {
        uuid << from_0_to_15(generator);
    }

    return uuid.str();
}

} /* namespace uuid */
