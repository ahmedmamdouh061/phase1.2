#ifndef SERVICE_REQUEST_H
#define SERVICE_REQUEST_H

// One service required by a car (mechanical, electrical, or wash).
// This is a value object; the project queues will store pointers to cars,
// while each car's required-services queue will store ServiceRequest values.

enum class ServiceType
{
    Mechanical,
    Electrical,
    Wash
};

class ServiceRequest
{
private:
    ServiceType type;
    int duration;

public:
    ServiceRequest();
    ServiceRequest(ServiceType serviceType, int serviceDuration);

    ServiceType GetType() const;
    int GetDuration() const;

    void SetType(ServiceType serviceType);
    void SetDuration(int serviceDuration);
};

#endif
