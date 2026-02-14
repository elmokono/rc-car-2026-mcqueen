# McQueen RC Car Hack
So this is an improvement to Disney stock Lightning McQueen RC Car.

I already made a RC Car using this chassis, electronic and mechanics, so decided to hack into this toy to make races with my nephew

- the RC receiver will remain the same but driven by 5v step up
- the rear motor will be driven by a DRV8833 at 7.4v
- the steer will be driven by a microservo
- the "converter" will be a RP2040 Zero, small and powerfull

So instead of feeding the motor, I will feed the RP2040 and it will convert into signals for the DRV8833 and microservo

![alt text](<pics/board (1).jpeg>)
![alt text](<pics/board (2).jpeg>)
![alt text](<pics/board (3).jpeg>)
![alt text](pics/schematics.jpeg)
![alt text](pics/logic-signals.jpeg)
![alt text](pics/models.jpeg)