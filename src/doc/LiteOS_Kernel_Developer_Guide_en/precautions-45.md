# Precautions<a name="EN-US_TOPIC_0311018438"></a>

-   The time management module depends on  **OS\_SYS\_CLOCK**  and  **LOSCFG\_BASE\_CORE\_TICK\_PER\_SECOND**.
-   When measured in ticks, system runtime is not accurate, because it is not measured while interrupts are disabled.

