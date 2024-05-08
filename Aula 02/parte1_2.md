```C
void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < K * ms);
}
```

a) **Resposta:** Para que `ms = 1` gere um atraso de 1 ms, `K = 20000`.

b) O core timer é de 32 bits, logo basta calcular o número de ciclos através de:

$$(2^{32}-1)*(1/20 000 000) \approx 214.75 sec \approx 3.58 min$$

**Resposta:** O valor máximo da função delay() é de cerca de 3.58 minutos.
