# Pulsed Laser Intensity Stabilization

## Table of Contents
- [Motivation](#Motivation)
- [List of Components](#List-of-Components)
- [Schematics](#Schematics)
  - [Experiment Setup Schematics](#Experiment-Setup-Schematics)
  - [Acousto-Optics Modulator (AOM)](#Acousto-Optics-Modulator-(AOM))
  - [Arduino DUE](#Arduino-DUE)
  - [Voltage Variable Attenuator](#Voltage-Variable-Attenuator-(VVA))
- [Feedback Loop](#Feedback-Loop)
- [Future Plans](#Future-Plan)

# Motivation
Many physical processes depend critically on the intensity of the laser, for example, in a Rabi Flopping experiment, the frequency of probability amplitude changing is proportional to the amplitude of the pertubring
electrogmagnetic field, which in many cases corresponds to the intensity of a laser. Therefore, any change in the laser intensity will result in uncontrolled change in the oscillation, giving us inaccurate determination of the PI time, for instance. In practice, many factors could affect laser intensity: thermal fluctuation, mechanical vibration, etc. So the goal of project is to counteract these undesired drifts in laser intensity, with the means of PID feedback control.

<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/branch_Tommy/Rabi Freq.png" width="600">

# Setup
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/branch_Tommy/Setup_Schematics.png" width="1000">
We have a 635nm laser first shining into a sequence of optics, where the light is split into two portions. A portion goes directly into a photodiode and becomes our raw signal - we monitor the change of laser intensity using this signal. The other portion of the laser goes through an acousto-optic modulator, or AOM, which acts as a diffraction grating and can diffract incident light into different angles. For our setup, we take the first order light. This first order light will be the output, and be fed back to the Arduino. This is the signal we want to stabilize. The Arduino then calculates a correction signal, feeding it into a Voltage Variable Attenuator, or VVA, to control how much power the AOM gets by attenuating the pulsed signal from a function generator. The more power the AOM gets, the more optical power is distributed to the first order light.
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/branch_Tommy/Experimental_Setup.png" width="1000">

# List of Components
- Laser: [PL202 Thorlab Compact Laser](https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=12994) 
- Acousto-Optics Modulator: [tbd](https://www.lambdaphoto.co.uk/intraaction-atm-series-acousto-optic-modulator.html)
- Voltage Variable Attenuator: [ZX73-2500+](https://www.minicircuits.com/pdfs/ZX73-2500+.pdf)
- RF Amplifier: [ZHL-1-2W+](https://www.minicircuits.com/WebStore/dashboard.html?model=ZHL-1-2W%2B)
- Photodiode: [PDA10A2](https://www.thorlabs.com/thorproduct.cfm?partnumber=PDA10A2)
- Function Generator, DC Power Supply, Oscilloscope, Electronics(op amp, resistors, capacitors, mosfet(potential) Optics(mirrors, converging lens, rotating filter, polarizer, beam splitter, aperture etc.) 


# Schematics
## Experiment Setup Schematics
## Acousto-Optics Modulator (AOM)
## Voltage Variable Attenuator (VVA)



<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/AOM_first_order_power_versus_VVA_tune.png" width="600">
The first order diffraction efficiency of AOM is crucial in determining our dynamic range and its dependency on the VVA tune voltage will also determine the circuit design which maps between arduino output and power correction. By carefully adjusting the AOM position，VVA power, VCO power, VCO tune voltage, we are able to achieve a maximum diffraction efficiency up to ~60% (incident laser power ~0.8mV, first order laser power ~0.5mV). (At beginning stage we were using a Voltage Controlled Oscillator(VCO) in generating RF signal that feeds into AOM; later on we switched to a function generator) Then we manually change VVA tune and record the corresponding laser power passing through the AOM with a power meter. This bell-shape figure indicates that we should not go beyond ~6V VVA tune voltage as the AOM saturates and starts to lose modulation efficiency (also potentially damaging the AOM). It is also worth noting that if the arduino DAC output pushes VVA tune beyond this saturation voltage level, the whole relation will invert and we may observe abnormal behavior. If the maximum received power is still below the setpoint, the falling behavior of AOM first order efficiency will send a false signal to arduino which triggers a even larger error signal that in turn further brings up the VVA tune voltage. This ends up into a loop stuck and maximum arduino DAC output with zero stabilization ability and potentially damages the AOM. We confirmed this prediction by observing spontaneous dropping of received laser power. By using a differential op-amp, we were able to linearly map the arduino DAC output (0.5~2.7 V) to (0~6 V) that perfectly terminates at the saturation point. However due to some external considerations we switched to an op-amp with much higher gain which again causes this problem (DAC automatically slides to 2.7V once it reaches around 1.1V）. Thus one major next step is to add it back and also implement an algorithm for double protection.





## Arduino DUE


# Feedback Loop
## PID Controller

# Future Plan
