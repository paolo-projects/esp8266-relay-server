#ifndef RESPONSE_H
#define RESPONSE_H

#include "SerialMap.h"
#include "RelayManager.h"
#include "Common.h"

class Response
{
public:
    static ResponseMap successResponse()
    {
        ResponseMap response;
        response.put("result", "ok");
        return response;
    }

    static ResponseMap errorResponse()
    {
        ResponseMap response;
        response.put("result", "error");
        return response;
    }

    static ResponseMap statusResponse(RELAY_STATE state)
    {
        ResponseMap response;
        response.put("state", state == RELAY_ON ? "on" : "off");
        return response;
    }
};

#endif