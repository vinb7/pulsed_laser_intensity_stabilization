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
## [PL202 Thorlab Compact Laser](https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=12994) 
# Schematics
## Experiment Setup Schematics
## Acousto-Optics Modulator (AOM)
## Voltage Variable Attenuator (VVA)
### 

<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/AOM_first_order_power_versus_VVA_tune.png" width="1000">

## Arduino DUE


# Feedback Loop
## PID Controller

# Future Plan
