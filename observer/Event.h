#ifndef EVENT_H
#define EVENT_H

/**
 * All the things you can be notify about.
 */
enum class ObservableEvent {
  // Described by CmdView
  GEOMETRIES_LOADED,
  CHARGES_LOADED,
  FILE_SAVED,
  CALCULATIONS_FINISHED,

  // Described by CalculationState
  EHSS_STARTED,
  PA_STARTED,
  TM_STARTED,
  TRAJECTORY_NUMBER_UPDATE,
  EHSS_ENDED,
  PA_ENDED,
  TM_ENDED,
  ONE_CALCULATION_FINISHED
};

#endif
