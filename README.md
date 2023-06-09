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
electrogmagnetic field, which in many cases corresponds to the intensity of a laser. Therefore, any change in the laser intensity will result in uncontrolled change in the oscillation, giving us inaccurate determination of the PI time, for instance. For laser cooling experiments, we often want a specifict laser intensity to achieve the best cooling effect or to avoid heating up an trapped ion. Therefore, we expect the laser intensity to be what we set in a control computer. However, in practice, many factors could affect laser intensity: thermal fluctuation, mechanical vibration, etc. So the goal of project is to counteract these undesired drifts in laser intensity.
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/branch_Tommy/Rabi Freq.png" width="500">

# Goal
Our goal for this project is to build a reliable laser intensity stabilizing device, exploiting PID feedback control, to stabilize a pulsed laser. We aim to suppress power flunctuation down to 1% for a pulse width of 5 us.

# List of Components
- Laser: [PL202 Thorlab Compact Laser](https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=12994) 
- Acousto-Optics Modulator: [tbd](https://www.lambdaphoto.co.uk/intraaction-atm-series-acousto-optic-modulator.html)
- Voltage Variable Attenuator: [ZX73-2500+](https://www.minicircuits.com/pdfs/ZX73-2500+.pdf)
- RF Amplifier: [ZHL-1-2W+](https://www.minicircuits.com/WebStore/dashboard.html?model=ZHL-1-2W%2B)
- Photodiode: [PDA10A2](https://www.thorlabs.com/thorproduct.cfm?partnumber=PDA10A2)
- Function Generator, DC Power Supply, Oscilloscope, Electronics(op amp, resistors, capacitors, mosfet(potential) Optics(mirrors, converging lens, rotating filter, polarizer, beam splitter, aperture etc.) 


# Schematics
## Experiment Setup Schematics
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/branch_Tommy/Setup_Schematics.png" width="1000">
We have a 635nm laser first shining into a sequence of optics, where the light is split into two portions. A portion (how large this portion is depends on the [dynamic range] (#Acousto-Optics-Modulator-(AOM)) of our setup) of the laser power goes directly into a photodiode and becomes our raw signal - we monitor the change of laser intensity using this signal. The other portion of the laser power goes through an acousto-optic modulator, or AOM, which acts as a diffraction grating and can diffract incident light into different angles. For our setup, we take the first order light as our output, and it is fed back to the Arduino pin A1. This is the signal we want to stabilize. The Arduino then calculates a correction signal, outputting the voltage from pin DAC1, feeding it into a Voltage Variable Attenuator, or VVA, to control how much power the AOM gets by attenuating the pulsed signal from a function generator. The more power the AOM gets, the more optical power is distributed to the first order light.
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/branch_Tommy/Experimental_Setup.png" width="1000">

## Acousto-Optics Modulator (AOM)
## Voltage Variable Attenuator (VVA)



<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/AOM_first_order_power_versus_VVA_tune.png" width="600">
The first order diffraction efficiency of AOM is crucial in determining our dynamic range and its dependency on the VVA tune voltage will also determine the circuit design which maps between arduino output and power correction. By carefully adjusting the AOM position，VVA power, VCO power, VCO tune voltage, we are able to achieve a maximum diffraction efficiency up to ~60%. (At beginning stage we were using a Voltage Controlled Oscillator(VCO) in generating RF signal that feeds into AOM; later on we switched to a function generator) Then we manually change VVA tune and record the corresponding laser power passing through the AOM with a power meter. The incident power is ~0.8mV 




## Arduino DUE
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/branch_tommy/Arduino_DUE.png" width="1000">
Arduino DUE is a common, accessible microcontroller that allows us to read input voltage, output voltage, and calculate corrections using a PID algorithm. Voltage generated from the photodiode taking the stabilized signal is fed into one of its Analog-to-Digital Converter pin, and after calculating the PID error signal, a correction voltage is outputted from one of its Digital-to-Analog Converter pin to vary the power received by the AOM. 

Arduino DUE is the fastest among the Arduino family, in terms of sampling rate and processing speed. However, it turned out that for our goal of stabilzing a 5us pulse, the speed of the Arduino DUE is still insufficient. Below is a summary of the time taken by different Arduino commands: 

digitalRead(): 1.2us <br />
read_adc(): 1.8us <br />
micros(): 1.3us <br />
analogWrite(): 3.8us <br />
Serial.println(“ “): 1.3ms <br />
loop(): 25us [nothing is in the loop()] 


# Feedback Loop
## PID Controller

# Future Plan
