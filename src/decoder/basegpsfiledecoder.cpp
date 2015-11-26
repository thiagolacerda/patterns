#include "basegpsfiledecoder.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include "filedbmanager.h"

BaseGPSFileDecoder::BaseGPSFileDecoder(const std::vector<std::string>& parameters, char separator)
    : DatabaseDecoder(new FileDBManager(separator))
    , m_path(parameters.empty() ? "" : parameters.at(0))
{
}

uint64_t BaseGPSFileDecoder::doRetrievePoints(int64_t batchSize)
{
    return m_manager->retrievePoints("", batchSize);
}

uint64_t BaseGPSFileDecoder::numberOfRecords()
{
    std::ifstream inFile(m_path);
    return std::count(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>(), '\n');
}
