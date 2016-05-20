#include "Observable.h"

#include "Observer.h"


Observable::Observable()
{

}

Observable::~Observable()
{

}

void Observable::addObserver(Observer* obs) {
  m_observers.push_back(obs);
}

void Observable::removeObserver(Observer* obs) {
  for (auto it = m_observers.begin(); it != m_observers.end(); ++it) {
    if (*it == obs) {
      m_observers.erase(it);
    }
  }
}

void Observable::notifyObservers(ObservableEvent cond) {
  std::for_each(m_observers.begin(), m_observers.end(), [&](Observer* obs){ obs->update(cond, this); });
}
