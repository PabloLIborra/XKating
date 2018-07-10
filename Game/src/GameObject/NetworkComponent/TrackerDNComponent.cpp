#include "TrackerDNComponent.h"

TrackerDNComponent::TrackerDNComponent(GameObject& newGameObject, int server_id, char t) : IComponent(newGameObject)
{
    objective = server_id;
    type = t;
}
