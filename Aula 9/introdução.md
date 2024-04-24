## Cálculo do Timer T2

$$k_{prescaler} =   \frac{20 000 000}{(2^{16} * 10)} = \frac{20 000 000}{655360} \approx 31$$

1:32 prescale value
<br>
Logo, **TCKPS = 5** (101)

$$f_{out} = \frac{ \frac{f_{pbclk}}{k_{prescaler}} }{(PR2 + 1)}$$
$$10 = \frac{ \frac{20 000 000}{32} }{(PR2 + 1)}$$
$$PR2 = ( \frac{20 000 000}{32} ) - 1 \approx 62499$$

$$f_{outPrescaler} = \frac{f_{pbclk}}{32} = 625 000Hz = 625KHz$$

## Cálculo do OC1RS

$$dutyCycle = \frac{t_{on}}{T} * 100$$
<p style="text-align: center;">[%] 20% a 10Hz neste caso</p>

$$t_{on} = 0.2 * TPWM = 0.2 * (1/10) = 20ms = 20*10^{-3}$$
$$f_{outPrescaler} = 625KHz$$
$$T_{outPrescaler} = 1 / 625 000 = 1.6µs = 1.6*10^{-6}$$

$$OC1RS = \frac{20*10^{-3}}{1.6*10^{-6}} = 12 500$$

## Outra forma de calcular o OC1RS

$$OC1RS = \frac{(PR2 + 1) * dutyCycle}{100} = 12 500$$
