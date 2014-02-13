/**
 * @file None.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 24/07/2013
 * @section LICENSE
 *
 * Copyright NIWA Science �2013 - www.niwa.co.nz
 *
 */

// headers
#include "None.h"

// namespaces
namespace isam {
namespace agesizes {

/**
 * default constructor
 */
None::None() {
  parameters_.Bind<string>(PARAM_SIZE_WEIGHT, &size_weight_, "Not Implemented");
}

} /* namespace agesizes */
} /* namespace isam */