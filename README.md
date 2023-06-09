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
