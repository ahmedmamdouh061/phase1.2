#include "ServiceRequest.h"

ServiceRequest::ServiceRequest()
    : type(ServiceType::Mechanical), duration(0)
{
}

ServiceRequest::ServiceRequest(ServiceType serviceType, int serviceDuration)
    : type(serviceType), duration(serviceDuration)
{
}

ServiceType ServiceRequest::GetType() const
{
    return type;
}

int ServiceRequest::GetDuration() const
{
    return duration;
}

void ServiceRequest::SetType(ServiceType serviceType)
{
    type = serviceType;
}

void ServiceRequest::SetDuration(int serviceDuration)
{
    duration = serviceDuration;
}
