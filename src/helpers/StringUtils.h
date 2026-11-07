//
// Created by SharkBlocks00 on 11/7/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <algorithm>
#include <string>
#include <vector>

inline int levenshteinDistance(const std::string& a, const std::string& b) {
    std::vector<int> previous(b.size() + 1);
    std::vector<int> current(b.size() + 1);

    for (std::size_t j = 0; j <= b.size(); ++j) {
        previous[j] = static_cast<int>(j);
    }

    for (std::size_t i = 1; i <= a.size(); ++i) {
        current[0] = static_cast<int>(i);

        for (std::size_t j = 1; j <= b.size(); ++j) {
            const int cost = a[i - 1] == b[j - 1] ? 0 : 1;

            current[j] = std::min({
                previous[j] + 1,
                current[j - 1] + 1,
                previous[j - 1] + cost
            });
        }

        previous.swap(current);
    }

    return previous[b.size()];
}

inline std::vector<std::string> getCloseMatches(const std::string& name, const std::vector<std::string>& candidates,
    const std::size_t n = 1, const double cutoff = 0.6) {
    std::vector<std::pair<double, std::string>> matches;

    for (const auto& candidate : candidates) {
        const int distance = levenshteinDistance(name, candidate);

        const double similarity = 1.0 - static_cast<double>(distance) / std::max(
            name.size(), candidate.size()
        );

        if (similarity > cutoff) {
            matches.emplace_back(similarity, candidate);
        }
    }

    std::ranges::sort(matches,
                      [](const auto& a, const auto& b) {
                          return a.first > b.first;
                      }
    );

    std::vector<std::string> result;

    for (std::size_t i = 0; i < std::min(n, matches.size()); ++i) {
        result.push_back(matches[i].second);
    }

    return result;
}
