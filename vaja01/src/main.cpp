#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct InputData
{
    int sizeOfBox;
    int goldSize;
    int goldValue;
    int silverSize;
    int silverValue;
};

bool readInputData(const std::string& filename, std::vector<InputData>& data)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        InputData entry{};
        if (iss >> entry.sizeOfBox >> entry.goldSize >> entry.goldValue >> entry.silverSize >> entry.silverValue)
        {
            data.push_back(entry);
        }
    }

    return true;
}

int64_t calculateBestValue(const InputData& row)
{
    const int64_t boxSize = row.sizeOfBox;
    const int64_t goldSize = row.goldSize;
    const int64_t goldValue = row.goldValue;
    const int64_t silverSize = row.silverSize;
    const int64_t silverValue = row.silverValue;

    if (boxSize < goldSize && boxSize < silverSize)
        return 0;

    int64_t bestValue = 0;

    if (goldSize >= silverSize)
    {
        //For every possible combination of goldCount / silverCount
        for (int goldCount = 0; goldCount <= boxSize / goldSize; ++goldCount)
        {
            int64_t remainingSpace = boxSize - goldCount * goldSize;
            int64_t silverCount = remainingSpace / silverSize;

            int64_t totalValue = static_cast<int64_t>(goldCount) * goldValue +
                                   static_cast<int64_t>(silverCount) * silverValue;

            if (totalValue > bestValue)
                bestValue = totalValue;
        }
    }
    else
    {
        //For every combination of silverCount / goldCount
        for (int64_t silverCount = 0; silverCount <= boxSize / silverSize; ++silverCount)
        {
            int64_t remainingSpace = boxSize - silverCount * silverSize;
            int64_t goldCount = remainingSpace / goldSize;

            int64_t totalValue = static_cast<int64_t>(goldCount) * goldValue +
                                   static_cast<int64_t>(silverCount) * silverValue;

            if (totalValue > bestValue)
                bestValue = totalValue;
        }
    }

    return bestValue;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::vector<InputData> data;
    if (!readInputData(argv[1], data))
        return 1;

    for (const auto& row : data)
    {
        std::cout << calculateBestValue(row) << '\n';
    }

    return 0;
}
