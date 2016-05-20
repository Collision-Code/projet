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
     * Update the observer.
     * \param cond the condition that triggered the notification.
     * \param obs the Observable which triggered the call. May be null.
     */
    virtual void update(ObservableEvent cond, Observable* obs) = 0;
};

#endif
