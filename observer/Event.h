/*
 * Collision-Code
 * Free software to calculate collision cross-section with Helium.
 * Université de Rouen
 * 2016
 *
 * Anthony BREANT
 * Clement POINSOT
 * Jeremie PANTIN
 * Mohamed TAKHTOUKH
 * Thomas CAPET
 */

/**
 * \file Event.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Describes all events that can be launched by the model.
 */

#ifndef EVENT_H
#define EVENT_H

enum class ObservableEvent {
  /// Launched when geometries are loaded by the model.
  GEOMETRIES_LOADED,
  /// Launched when a file containing charges is loaded by the model.
  CHARGES_LOADED,
  /// Launched when results are saved in a file.
  FILE_SAVED,
  /// Launched when all calculations on all geometries are finished.
  CALCULATIONS_FINISHED,
  /// Launched when a calculation by EHSS method starts.
  EHSS_STARTED,
  /// Launched when a calculation by PA method starts.
  PA_STARTED,
  /// Launched when a calculation by TM method starts.
  TM_STARTED,
  /// Launched when some trajectories are calculated by TM method.
  TRAJECTORY_NUMBER_UPDATE,
  /// Launched when a calculation by EHSS method ends.
  EHSS_ENDED,
  /// Launched when a calculation by PA method ends.
  PA_ENDED,
  /// Launched when a calculation by TM method ends.
  TM_ENDED,
  /// Launched when a calculation is finished.
  ONE_CALCULATION_FINISHED
};

#endif
