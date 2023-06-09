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
The first order diffraction efficiency of AOM is crucial in determining our dynamic range and its dependency on the VVA tune voltage will also determine the circuit design which maps between arduino output and power correction. By carefully adjusting the AOM position，VVA power, VCO power, VCO tune voltage, we are able to achieve a maximum diffraction efficiency up to ~60%. (At beginning stage we were using a Voltage Controlled Oscillator(VCO) in generating RF signal that feeds into AOM; later on we switched to a function generator) Then we manually change VVA tune and record the corresponding laser power passing through the AOM with a power meter. The incident power is ~0.8mV 




## Arduino DUE


# Feedback Loop
## PID Controller

# Future Plan
