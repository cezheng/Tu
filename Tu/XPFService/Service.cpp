#include "Service.h"
NS_XPF_BEGIN
Json Service::API::call() {
    this->requireParams();
    return this->internalCall();
}

void Service::API::requireParams() const {
    std::string err;
    if (!_params.has_shape(this->getRequirements(), err)) {
        throw std::invalid_argument(err);
    }
}
NS_XPF_END