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
 * \file Observer.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Implements the "Observer" part of the pattern Observer/Observable.
 */

#ifndef OBSERVER_H
#define OBSERVER_H

#include "Observable.h"
#include "Event.h"

class Observer
{
  public:
    /**
     * Constructor.
     */
    Observer();

    /**
     * Destructor.
     */
    virtual ~Observer();

    /**
     * Updates the observer.
     * \param cond the condition that triggered the notification.
     * \param obs the Observable which triggered the call. May be null.
     */
    virtual void update(ObservableEvent cond, Observable* obs) = 0;
};

#endif
