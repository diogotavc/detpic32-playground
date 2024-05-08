No coprocessador 0 está implementado o _core timer_ (32 bits) que é atualizado a cada dois ciclos do relógio (que opera a 40 Mhz).

$$(40/2) MHz = 20 MHz = 20 000 000 Hz = 1/20 000 000 sec$$

- `resetCoreTimer()` : core timer está a 0
- `while(readCoreTimer() < 200000)` : espera 200_000 ciclos do core timer

**Resposta:** $10 ms = 0.01 sec = (1/20 000 000)*200 000$
