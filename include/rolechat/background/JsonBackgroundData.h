#include <nlohmann/json.hpp>
#include "rolechat/background/IBackgroundData.h"

namespace rolechat::background {

class JsonBackgroundData : public IBackgroundData {
public:
    JsonBackgroundData(){};
    void loadBackground(const std::string& backgroundPath);
};

}