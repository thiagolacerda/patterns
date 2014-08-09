#include "manager.h"

#include "databasedecoder.h"
#include "factory.h"

void Manager::start()
{
    m_dbDecoder = Factory::dbDecoderInstance();
    if (m_dbDecoder)
        retrieveData();
}

void Manager::retrieveData()
{
    m_dbDecoder->retrievePoints();
}
