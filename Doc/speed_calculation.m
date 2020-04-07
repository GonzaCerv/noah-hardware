speed = 30.0
microstep = 1.0
stepsPerTurn = 200
TIMER_INTERRPT_FREQ = 10000

 ((TIMER_INTERRPT_FREQ / ((speed * (stepsPerTurn * microstep)) / (2*pi()))) / 2)