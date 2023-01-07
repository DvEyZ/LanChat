#include "AddressedResponseMessage.h"

AddressedResponseMessage::AddressedResponseMessage(std::vector <std::string> receivers, ReadableMessageBody body, Status s)
    :Addressed(receivers), ResponseMessage(body, s)
{}