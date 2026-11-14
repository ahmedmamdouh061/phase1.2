#include "ServiceRequest.h"

#include <cassert>

int main()
{
    ServiceRequest defaultRequest;
    assert(defaultRequest.GetType() == ServiceType::Mechanical);
    assert(defaultRequest.GetDuration() == 0);

    ServiceRequest request(ServiceType::Electrical, 25);
    assert(request.GetType() == ServiceType::Electrical);
    assert(request.GetDuration() == 25);

    request.SetType(ServiceType::Wash);
    request.SetDuration(12);
    assert(request.GetType() == ServiceType::Wash);
    assert(request.GetDuration() == 12);

    return 0;
}
