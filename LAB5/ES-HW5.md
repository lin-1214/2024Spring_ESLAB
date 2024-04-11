# Lab5 PWM and Logic Analyzer Report

楊竣凱
學號：b10901027
Repo: [https://github.com/lin-1214/2024Spring_ESLAB](https://github.com/lin-1214/2024Spring_ESLAB)

---
## Discussions of Work

1. Mbed Studio or Stm32cubeide (choose one for your group) projects (in github)

Stm32cubeide

2. A report about your lab steps, PWM waveforms observation using LA and discussions.

The following Step are associated with problem 3 (dead time insertion):

    1. adjust bus clock to 64 MHz
![Screenshot 2024-04-11 at 4.48.11 PM](https://hackmd.io/_uploads/BkP0QXHlA.png)

    2. change Timer (TIM1)'s Channel 1 to PWM Generation CH1 CH1N
![Screenshot 2024-04-11 at 4.49.29 PM](https://hackmd.io/_uploads/rJ8QV7HxR.png)
    
    3. under Configuration
        - change "Prescaler" to 64-1
        - change "Counter Period" to 1000 (1s)
    
![Screenshot 2024-04-11 at 4.52.07 PM](https://hackmd.io/_uploads/H1PTEXHeC.png)
    
    4. change "dead time" to 232
        - means 232 ticks, which can insert dead time of 10us
![Screenshot 2024-04-11 at 4.57.35 PM](https://hackmd.io/_uploads/rJeGI7BxR.png)

    5. change "Pulse" to 500 for 50% duty period
    (500/1000 = 50%)
![Screenshot 2024-04-11 at 4.59.35 PM](https://hackmd.io/_uploads/rk7KIQre0.png)

    6. connect LA to pin "PA7" & "PE9"( PE9 cannot connect)
![Screenshot 2024-04-11 at 5.02.50 PM](https://hackmd.io/_uploads/BJUSDmHxA.png)

    7. we can only observe "PA7"
![Screenshot 2024-04-11 at 5.22.12 PM](https://hackmd.io/_uploads/BkWAjmBx0.png)

3. Discuss how to generate complementary PWM with dead time insertion in STM32 platform. 

As describe above, but can only observe PA7 pin.