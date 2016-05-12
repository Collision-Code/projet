#include "SystemParameters.h"

SystemParameters* SystemParameters::m_instance = new SystemParameters();

SystemParameters::SystemParameters()
  : m_maxNumberThreads(20)
{

}

SystemParameters::~SystemParameters()
{

}
