#ifndef RESPONSE_H
#define RESPONSE_H

#include "SerialMap.h"
#include "RelayManager.h"

class Response
{
public:
    static SerialMap<1> successResponse()
    {
        SerialMap<1> response;
        response.put("result", "ok");
        return response;
    }

    static SerialMap<1> errorResponse()
    {
        SerialMap<1> response;
        response.put("result", "error");
        return response;
    }

    static SerialMap<1> statusResponse(RELAY_STATE state)
    {
        SerialMap<1> response;
        response.put("state", state == RELAY_ON ? "on" : "off");
        return response;
    }
};

#endif