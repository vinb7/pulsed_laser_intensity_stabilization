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
<img src="[https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/vinay/Screen%20Shot%202023-06-08%20at%207.19.34%20PM.png]" width="1000">
In spectroscopy experiments, an unstable laser intensity can have significant implications, particularly in the context of Rabi oscillations. Rabi oscillations occur when an electromagnetic field (in our experiment,  a laser beam) interacts with a two level system causing an oscillation between the two quantum states. This results in periodic energy exchanges between the two states in our system.
When the laser intensity varies, the excitation and de-excitation rates in the Rabi oscillation can deviate from their expected values. This leads to shifts in the Rabi oscillation frequencies, causing a mismatch between the observed transitions and the actual energy differences between the quantum states. As a result, the spectroscopic measurements may yield incorrect or imprecise values for the system's energy levels.
These shifts can introduce errors in spectroscopy measurements, as the observed transitions between energy levels may not accurately correspond to the actual energy differences in the sample. Therefore, maintaining a stable laser intensity is essential for precise and reliable spectroscopic analyses, ensuring accurate determination of the system's energy levels and avoiding systematic errors in the obtained results.
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
