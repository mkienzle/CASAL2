/**
 * @file Partition_YearCrossAgeMatrix.cpp
 * @author  Marco Kienzle (Marco.Kienzle@niwa.co.nz) based on Partition.cpp by Scott Rasmussen (scott.rasmussen@zaita.com)
 * @version 1.0
 * @date 26/08/2018
 * @section LICENSE
 *
 * Copyright NIWA Science ©2013 - www.niwa.co.nz
 *
 * $Date: Sunday 26 August  08:19:08 NZST 2018 $
 */

// Headers
#include "PartitionYearCrossAgeMatrix.h"

#include <iostream>
#include <iomanip>
#include <math.h>
#include <algorithm>
#include <string.h>

#include "Model/Model.h"
#include "Partition/Accessors/All.h"

// Namespaces
namespace niwa {
namespace reports {
namespace age {

/**
 * Default constructor
 */
Partition_YearCrossAgeMatrix::Partition_YearCrossAgeMatrix(Model* model) :
    Report(model) {
  run_mode_ = (RunMode::Type) (RunMode::kBasic | RunMode::kProjection);
  model_state_ = State::kExecute;

  parameters_.Bind<string>(PARAM_TIME_STEP, &time_step_, "Time Step label", "",
      "");
  parameters_.Bind<unsigned>(PARAM_YEARS, &years_, "Years", "", true);
}

void Partition_YearCrossAgeMatrix::DoValidate() {
  if (!parameters_.Get(PARAM_YEARS)->has_been_defined()) {
    years_ = model_->years();
  }
}
/**
 *
 */
void Partition_YearCrossAgeMatrix::DoPrepare() {

  unsigned lowest = 9999;
  unsigned highest = 0;
  string partition_name;
  
  niwa::partition::accessors::All all_view(model_);

  for (auto iterator = all_view.Begin(); iterator != all_view.End();
      ++iterator) {
    if (lowest > (*iterator)->min_age_)
      lowest = (*iterator)->min_age_;
    if (highest < (*iterator)->max_age_)
      highest = (*iterator)->max_age_;

    partition_name = (*iterator)->name_;
  }

 
  // Print the header
  cache_ << "*" << type_ << "[" << label_ << "]" << "\n";
  cache_ << "category: " << partition_name << "\n";
  cache_ << "time_step: " << time_step_ << "\n";

  // Print the age-groups

  cache_ << "values "<< REPORT_R_DATAFRAME << "\n";
  cache_ << std::left << std::setw(numWidth) << std::setfill(separator) << "year"; // leave an empty space in the years's column

  for (unsigned i = lowest; i <= highest; ++i)
    cache_ << std::left << std::setw(numWidth) << std::setfill(separator) << "AG" + std::to_string(i);
  cache_ << "\n";
}

/**
 *
 */
void Partition_YearCrossAgeMatrix::DoExecute() {
  //cerr << "execute " << label_ << "\n";
  // First, figure out the lowest and highest ages/length
  unsigned lowest = 9999;
  unsigned highest = 0;
  unsigned longest_length = 0;

  niwa::partition::accessors::All all_view(model_);

  for (auto iterator = all_view.Begin(); iterator != all_view.End();
      ++iterator) {
    if (lowest > (*iterator)->min_age_)
      lowest = (*iterator)->min_age_;
    if (highest < (*iterator)->max_age_)
      highest = (*iterator)->max_age_;
    if (longest_length < (*iterator)->name_.length())
      longest_length = (*iterator)->name_.length();

  }

  //// Determine the number of digits of the largest number
  unsigned int max_digit_nb = 0;

  // Loop over all partitions
  for (auto iterator = all_view.Begin(); iterator != all_view.End();
       ++iterator) {

    unsigned age = (*iterator)->min_age_;

    // Loop over all elements of a partition
    for (auto values = (*iterator)->data_.begin();
	 values != (*iterator)->data_.end(); ++values, age++) {
      if (age >= lowest && age <= highest) {

        Double value = *values;
	
	// Count how many digits each number
	while( ( fabs(value) / pow(10, max_digit_nb) ) > 1) max_digit_nb++;

	numWidth = std::max(numWidth, max_digit_nb);
	  
      }} // End of loop over all elements of a partition

  } // End of loop on iterator
  
  // Formatted output of values in the partition
  for (auto iterator = all_view.Begin(); iterator != all_view.End();
      ++iterator) {
    //cache_ << (*iterator)->name_;
    cache_ << std::left << std::setw(numWidth) << std::setfill(separator) << std::setprecision(1) << std::fixed << model_->current_year();

    cache_<< numWidth << std::setfill(separator);
    
    unsigned age = (*iterator)->min_age_;
    //unsigned year = (*iterator)->year_;
    //cout << "The year at this stage is " << year << endl;
    for (auto values = (*iterator)->data_.begin();
        values != (*iterator)->data_.end(); ++values, age++) {
      if (age >= lowest && age <= highest) {
        Double value = *values;
        //cache_ << "\t" << std::fixed << AS_DOUBLE(value);
        cache_ << std::right << std::setw(numWidth + 2) << std::setfill(separator) << std::setprecision(0) << std::fixed << AS_DOUBLE(value);

	//std::cout << "These are the value send to file " << AS_DOUBLE(value) << std::endl;
	
      } else
        cache_ << " " << "null";
    }
    cache_ << "\n";
  }
  //  ready_for_writing_ = true;
  ready_for_writing_ = false;
}


void Partition_YearCrossAgeMatrix::DoFinalise() {
  ready_for_writing_ = true;
}

} /* namespace age */
} /* namespace reports */
} /* namespace niwa */
