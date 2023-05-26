/**
 * @file embedded_runge_kutta_template.hpp
 * @brief Class for Embedded Runge-Kutta method
 */
#ifndef S2E_LIBRARY_NUMERICAL_INTEGRATION_RUNGE_KUTTA_TEMPLATE_HPP_
#define S2E_LIBRARY_NUMERICAL_INTEGRATION_RUNGE_KUTTA_TEMPLATE_HPP_

#include "embedded_runge_kutta.hpp"

namespace libra {

template <size_t N>
void EmbeddedRungeKutta<N>::Integrate() {
  std::vector<Vector<N>> slope = CalcSlope();  //!< k vector in the equation

  Vector<N> lower_current_state = current_state_;   //!< eta in the equation
  Vector<N> higher_current_state = current_state_;  //!< eta_hat in the equation
  for (size_t i = 0; i < number_of_stages_; i++) {
    lower_current_state = lower_current_state + weights_[i] * step_width_s_ * slope[i];
    higher_current_state = higher_current_state + higher_order_weights_[i] * step_width_s_ * slope[i];
  }

  // Error evaluation
  Vector<N> truncation_error = lower_current_state - higher_current_state;
  local_truncation_error_ = truncation_error.CalcNorm();

  // State update
  current_state_ = higher_current_state;
  current_time_s_ += step_width_s_;
}

template <size_t N>
void EmbeddedRungeKutta<N>::ControlStepWidth(double error_tolerance) {
  double updated_step_width_s = pow(error_tolerance / local_truncation_error_, 1.0 / ((double)(approximation_order_ + 1))) * step_width_s_;
  if (updated_step_width_s <= 0.0) return; // TODO: Error handling
  step_width_s_ = updated_step_width_s;
}

}  // namespace libra

#endif  // S2E_LIBRARY_NUMERICAL_INTEGRATION_RUNGE_KUTTA_TEMPLATE_HPP_
