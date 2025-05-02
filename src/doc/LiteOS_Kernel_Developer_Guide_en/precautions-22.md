# Precautions<a name="EN-US_TOPIC_0311018412"></a>

-   Do not make calls to the LOS\_EventRead and LOS\_EventWrite APIs prior to the operating system being initialized. Otherwise, the operating system exhibits unexpected behavior.
-   While an interrupt is underway, events can be written into an event control block but event reads are not allowed.
-   Task blocking and event reading are not allowed while task scheduling is locked.
-   The input parameter of LOS\_EventClear is \~events \(reverse code of the type of events to be cleared\).
-   Bit 25 of the event mask is merely used to distinguish whether the LOS\_EventRead API returns an event or error code.

