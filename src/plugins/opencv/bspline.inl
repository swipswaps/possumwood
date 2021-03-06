#include <opencv2/opencv.hpp>

#include "bspline.h"

namespace possumwood {
namespace opencv {

template <unsigned DEGREE>
double BSpline<DEGREE>::B(double t, unsigned k) {
	assert(t >= 0.0 && t <= 1.0);
	assert(k < 4);

	switch(k) {
		case 0:
			t = 1.0 - t;
			return t * t * t / 6.0;
		case 1:
			return (3.0 * t * t * t - 6.0 * t * t + 4.0) / 6.0;
		case 2:
			return (-3.0 * t * t * t + 3.0 * t * t + 3.0 * t + 1.0) / 6.0;
		case 3:
			return t * t * t / 6.0;
		default:
			return 0.0;
	}

	// t = t - 0.5;

	// switch(k) {
	// 	case 0: return std::pow(2.0 + t, 3);
	// 	case 1: return 4.0 - 6.0*t*t - 3.0*t*t*t;
	// 	case 2: return 4.0 - 6.0*t*t + 3.0*t*t*t;
	// 	case 3: return std::pow(2.0 - t, 3);

	// 	default: assert(false); return 0.0;
	// }
}

template <unsigned DEGREE>
std::array<double, DEGREE> BSpline<DEGREE>::initArray(double val) {
	std::array<double, DEGREE> result;
	result.fill(val);
	return result;
}

template <unsigned DEGREE>
BSpline<DEGREE>::BSpline(unsigned subdiv, const std::array<double, DEGREE>& min, const std::array<double, DEGREE>& max)
    : m_subdiv(subdiv), m_controls(std::pow(subdiv + 3, DEGREE), std::make_pair(0.0f, 0.0f)), m_min(min), m_max(max) {
	assert(subdiv > 0);
}

template <unsigned DEGREE>
template <typename FN>
void BSpline<DEGREE>::visit(const std::array<double, DEGREE>& _coords, const FN& fn) const {
	std::array<double, DEGREE> coords = _coords;
	std::array<unsigned, DEGREE> offset;

	for(unsigned d = 0; d < DEGREE; ++d) {
		coords[d] = (coords[d] - m_min[d]) / (m_max[d] - m_min[d]);

		if(coords[d] < 0.0 || coords[d] > 1.0) {
			std::stringstream ss;
			ss << "Coordinate #" << d << " out of range - " << _coords[d] << " (expected between " << m_min[d]
			   << " and " << m_max[d] << ")";
			throw std::runtime_error(ss.str());
		}

		coords[d] *= (double)m_subdiv;

		const double rounded = std::min(floor(coords[d]), (double)(m_subdiv - 1));

		offset[d] = rounded;
		coords[d] = coords[d] - rounded;
	}

	const unsigned end = pow(4, DEGREE);
	for(unsigned i = 0; i < end; ++i) {
		double weight = 1.0;
		unsigned j = i;
		std::size_t index = 0;

		for(unsigned d = 0; d < DEGREE; ++d) {
			weight *= B(coords[d], j % 4);
			index = index * m_subdiv + j % 4 + offset[d];

			j /= 4;
		}
		assert(index < m_controls.size());

		fn(index, weight);
	}
}

template <unsigned DEGREE>
void BSpline<DEGREE>::addSample(const std::array<double, DEGREE>& coords, double value) {
	visit(coords, [&](unsigned index, double weight) {
		m_controls[index].first += weight * value;
		m_controls[index].second += weight;
	});
}

template <unsigned DEGREE>
double BSpline<DEGREE>::sample(const std::array<double, DEGREE>& coords) const {
	double result = 0;
	visit(coords, [&](unsigned index, double weight) {
		if(m_controls[index].second > 0.0)
			result += m_controls[index].first / m_controls[index].second * weight;
	});

	return result;
}

template <unsigned DEGREE>
bool BSpline<DEGREE>::operator==(const BSpline& b) const {
	return m_subdiv == b.m_subdiv && m_controls == b.m_controls && m_min == b.m_min && m_max == b.m_max;
}

template <unsigned DEGREE>
bool BSpline<DEGREE>::operator!=(const BSpline& b) const {
	return m_subdiv != b.m_subdiv || m_controls != b.m_controls || m_min != b.m_min || m_max != b.m_max;
}

}  // namespace opencv
}  // namespace possumwood
