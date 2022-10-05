// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef RANDOM_H
#define RANDOM_H
#include <array>
#include <string>

namespace random {
	double GetRandomValue();
	float GetRandomRange(float min, float max);
	double GetRandomRange(double min, double max);
	int GetRandomRange(int min, int max);
	std::size_t GetRandomRange(std::size_t min, std::size_t max);

	template<std::size_t S>
	std::array<double, S> GetRandomDistribution(double amountToDistribute);
	template<std::size_t S>
	std::array<float, S> GetRandomDistribution(float amountToDistribute);

	std::string GetPreString();
	std::string GetMidString();
	std::string GetPostString();

	template<size_t S>
	std::array<double, S> random::GetRandomDistribution(double amountToDistribute) {
		std::array<double, S> arr;
		double total = 0;
		for (int i = 0; i < S; i++) {
			arr[i] = random::GetRandomValue();
			total += arr[i];
		}
		for (int i = 0; i < S; i++) {
			arr[i] = (arr[i] / total) * amountToDistribute;
		}
		return arr;
	}

	template<size_t S>
	std::array<float, S> random::GetRandomDistribution(float amountToDistribute) {
		std::array<float, S> arr;
		float total = 0;
		for (int i = 0; i < S; i++) {
			arr[i] = (float)random::GetRandomValue();
			total += arr[i];
		}
		for (int i = 0; i < S; i++) {
			arr[i] = (arr[i] / total) * amountToDistribute;
		}
		return arr;
	}
};

#endif //RANDOM_H
