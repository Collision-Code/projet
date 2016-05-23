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
 * \file Observable.h
 * \author Anthony Breant, Clement Poinsot, Jeremie Pantin, Mohamed Takhtoukh, Thomas Capet
 * \version 1.0
 * \date 23 may 2016
 * \brief Implements the "Observable" part of the pattern Observer/Observable.
 */

#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <vector>
#include <algorithm>

#include "Event.h"

/**
 * Forward declaration of Observer class.
 */
class Observer;

class Observable
{
  public:
    /**
     * Constructor.
     */
    Observable();

    /**
     * Destructor.
     */
    virtual ~Observable();

    /**
     * Add an observer to the list of observers to notify.
     * \param obs the observer to add to the list.
     */
    void addObserver(Observer* obs);

    /**
     * Remove an observer from the list of observers to notify.
     * \param obs the observer to remove from the list.
     */
    void removeObserver(Observer* obs);

    /**
     * Notify all observers of the condition cond.
     * \param cond the condition for notify observers.
     */
    void notifyObservers(ObservableEvent cond);

  protected:
    /**
     * A list containing all observers.
     */
    std::vector<Observer*> m_observers;
};

#endif
