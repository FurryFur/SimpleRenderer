#pragma once

namespace GLMUtils {
	// Limits a vector to the specified magnitude
	template <typename VecT>
	VecT limitVec(const VecT& vec, float maxMagnitude)
	{
		auto magnitude = glm::length(vec);
		if (magnitude > maxMagnitude) {
			return glm::normalize(vec) * maxMagnitude;
		}
		return vec;
	}
}