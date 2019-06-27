#pragma once

#include <memory>
#include <iostream>

#include <opencv2/opencv.hpp>

#include <actions/traits.h>

namespace possumwood { namespace opencv {

class CameraResponse {
	public:
		CameraResponse();
		CameraResponse(const cv::Mat& responseCurve, const std::vector<float>& exposures);

		const cv::Mat& matrix() const;
		const std::vector<float>& exposures() const;

		bool operator == (const CameraResponse& f) const;
		bool operator != (const CameraResponse& f) const;

	private:
		cv::Mat m_matrix;
		std::vector<float> m_exposures;
};

std::ostream& operator << (std::ostream& out, const CameraResponse& f);

}

template<>
struct Traits<opencv::CameraResponse> {
	static constexpr std::array<float, 3> colour() {
		return std::array<float, 3>{{1, 0, 1}};
	}
};

}
