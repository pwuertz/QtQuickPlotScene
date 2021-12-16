#include "utils.hpp"
#include <cmath>

Utils::Utils(QObject *parent) : QObject {parent} { }


std::pair<qreal, qreal> Utils::calcNiceNum(const qreal number, const bool logMode) noexcept
{
  const auto exponent = std::floor(std::log10(number));
  const auto fraction = number / std::pow(qreal(10), exponent);
  auto nicePrecision = -exponent;
  const auto niceFrac = [&]() -> qreal {
    if (fraction <= 1) { return 1; }
    else if (fraction <= 2) { return 2; }
    else if (fraction <= 2.5) { nicePrecision += 1; return 2.5; }
    else if (fraction <= 5) { return 5; }
    else { nicePrecision -= 1; return 10; }
  }();
  const auto niceNumber = niceFrac * std::pow(qreal(10), exponent);

  if (logMode) {
    return { std::ceil(std::max(qreal(1), niceNumber)), std::max(nicePrecision, qreal(0)) };
  }
  return { niceNumber, std::max(nicePrecision, qreal(0)) };
}
