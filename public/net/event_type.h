#ifndef __EVENT_TYPE_H__
#define __EVENT_TYPE_H__

typedef enum {
    Connected,
    Timeout,
    Closed,
    Error,
    Read,
    Write,
    Readable,
    Writable,
    Accept
} EventType;

#endif  // __EVENT_TYPE_H__