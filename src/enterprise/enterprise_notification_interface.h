#ifndef BITCOIN_ENTERPRISE_NOTIFICATION_INTERFACE_H
#define BITCOIN_ENTERPRISE_NOTIFICATION_INTERFACE_H

#include <validationinterface.h>

class EnterpriseNotificationInterface final : public CValidationInterface
{
public:
    static EnterpriseNotificationInterface* Create();
};

#endif // BITCOIN_ENTERPRISE_NOTIFICATION_INTERFACE_H