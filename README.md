# Pulsed Laser Intensity Stabilization

***For mmmmmooooorrrrreeeee information, check our [development notebook (page 40 - 135)](https://docs.google.com/document/d/1LQWBTo_NxfFsRtgr4UB_30VIzrIzUXYY4XpfK6IdY2c/edit#).***

## Table of Contents
- [Motivation](#Motivation)
- [List of Components](#List-of-Components)
  - [Acousto-Optics Modulator (AOM)](#Acousto-Optics-Modulator-(AOM))
  - [Voltage Variable Attenuator](#Voltage-Variable-Attenuator-(VVA))
  - [Arduino DUE](#Arduino-DUE)
- [Schematics](#Schematics)
  - [Experiment Setup Schematics](#Experiment-Setup-Schematics)
  - [Circuit](#Circuit)
- [Feedback Loop](#Feedback-Loop)
- [Results](#Results)
- [Future Plans](#Future-Plan)

# Motivation
An unstable laser intensity could have negative impact on experiments, one example being the Rabi Flopping. Rabi Flopping occurs when an electromagnetic field, in many cases is the laser, interacts with a two level system causing an oscillation between the two quantum states. The frequency of this oscillation is directly related to the amplitude of the pertubring EM field. <br />
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/background/Rabi Freq.png" width="600">

When the laser intensity varies, the excitation and de-excitation rates in the two level system can deviate from their expected values. This leads to shifts in the Rabi frequency, causing a mismatch between the observed transitions and the actual energy differences between the quantum states. As a result, the spectroscopic measurements may yield imprecise values for the system's energy levels. Therefore, maintaining a stable laser intensity is essential for precise spectroscopic analyses, ensuring accurate determination of the system's energy levels.<br />
<br />
Another example is laser cooling, where we often want a specifict laser intensity to achieve the best cooling effect or to avoid heating up an trapped ion. Therefore, we expect the laser intensity to be what we set in a control computer. However, in practice, many factors could affect laser intensity: thermal fluctuation, mechanical vibration, etc.

# Goal
Our goal for this project is to build a reliable laser intensity stabilizing device, exploiting PID feedback control, to stabilize a **PULSED** laser. We aim to suppress power flunctuation down to 1% for a pulse width of 5 μs.

# List of Components
- Laser: [PL202 Thorlab Compact Laser](https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=12994) 
- Acousto-Optics Modulator: [ATM-2001A2.12](https://www.lambdaphoto.co.uk/intraaction-atm-series-acousto-optic-modulator.html)
- Voltage Variable Attenuator: [ZX73-2500+](https://www.minicircuits.com/pdfs/ZX73-2500+.pdf)
- Microcontroller: [Arduino DUE](https://store.arduino.cc/products/arduino-due)
- RF Amplifier: [ZHL-1-2W+](https://www.minicircuits.com/WebStore/dashboard.html?model=ZHL-1-2W%2B)
- Photodiode: [PDA10A2](https://www.thorlabs.com/thorproduct.cfm?partnumber=PDA10A2)
- Function Generator, DC Power Supply, Oscilloscope, Electronics, Optics

## Acousto-Optics Modulator (AOM)

<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/AOM/aom_diffraction.png" width="1000">
An Acousto-Optics Modulator (AOM) is a device which uses sound waves to control the diffraction, frequency, and intensity of light. When a sinusoidal eletric signal is applied to a transducer inside, it begins generating sound waves within the AOM crystal. This sound wave moving through the crystal causes periodic variations in the material's density, which affects the refractive index of the crystal. This pattern of refractive indices effectively makes the AOM a diffraction grating. <br />
<br />
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/AOM/AOM_diffraction.png.jpg" width="1000">
When a laser is directed into the AOM, it is diffracted into different angles. As shown in the picture above, the middle, brightest dot is the 0th order light; the two weaker dots are the +1st and -1st order light - they are bent away from the 0th order light. How much power is distributed into higher order light depends on the amplitude of the powering sinusoidal wave. This ability to change power distribution within different orders of light allows us to change the output intensity in response to fluctuations due to the laser source. For our setup, we use the +1st order beam from the AOM, because its optical power ranges from 0 to ideally 60% of the total incident light. Hence, we can create pulse and obtain a large dynamic range.  <br />

## Voltage Variable Attenuator (VVA)
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/AOM/AOM_first_order_power_versus_VVA_tune.png" width="1000">
The first order diffraction efficiency of AOM is crucial in determining our dynamic range and its dependency on the VVA tune voltage will also determine the circuit design which maps between arduino output and power correction. By carefully adjusting the AOM position，VVA power, VCO power, VCO tune voltage, we are able to achieve a maximum diffraction efficiency up to ~60% (incident laser optical power ~0.8mW, first order laser power ~0.5mW). (At beginning stage we were using a Voltage Controlled Oscillator(VCO) in generating RF signal that feeds into AOM; later on we switched to a function generator) Then we manually change the VVA tune and record the corresponding laser power passing through the AOM with a power meter. This bell-shape figure indicates that we should not go beyond ~6V VVA tune voltage as the AOM saturates and starts to lose modulation efficiency (also potentially damaging the AOM). <br />
<br />
It is also worth noting that if the VVA tune goes beyond this saturation voltage level, the feedback usually fails. After the VVA tune voltage crosses the peak, if the received power is still below the setpoint, the Arduino will attempt to increase its DAC output to increase the diffraction efficiency of the AOM, so more power can be delivered to the 1st order light, which is our output. However, we are already in the region on the right off the peak, so the higher the voltage, actually the less diffraction efficiency. The Arduino then detects an even lower optical power and increases more DAC power, resulting in even smaller diffraction efficiency. This ends up into a loop stuck and maximum arduino DAC output with zero stabilization ability and potentially damages the AOM. We confirmed this prediction by observing spontaneous dropping of received laser power. By using a differential op-amp, we were able to linearly map the arduino DAC output (0.5~2.7 V) to (0~6 V) that perfectly terminates at the saturation point. However due to some external considerations we switched to an op-amp with much higher gain which again causes this problem (DAC automatically slides to 2.7V once it reaches around 1.1V）. Thus one major next step is to add it back and also implement an algorithm for double protection.





## Arduino DUE
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/arduino/Arduino_DUE.png" width="1000">
Arduino DUE is a common, accessible microcontroller that allows us to read input voltage, output voltage, and calculate corrections using a PID algorithm. Voltage generated from the photodiode taking the stabilized signal is fed into one of its Analog-to-Digital Converter (ADC) pin, and after calculating the PID error signal, a correction voltage is outputted from one of its Digital-to-Analog Converter (DAC) pin to vary the power received by the AOM. <br />
<br />
Arduino DUE is the fastest among the Arduino family, in terms of sampling rate and processing speed. However, it turned out that for our purpose of stabilzing a 5μs pulse, the speed of the Arduino DUE is still insufficient. Below is a summary of the time taken by different Arduino commands: <br />
<br />
digitalRead(): 1.2μs <br />
read_adc(): 1.8μs <br />
micros(): 1.3μs <br />
analogWrite(): 3.8μs <br />
Serial.println(“ “): 1.3ms <br />
loop(): 25μs [nothing is in the loop()] <br />
<br />

read_adc() and analogWrite() take time on the order of the pulse length. So when the pulse length approaches these commands' runtimes, it happens frequently that a read_adc() which we expect to be executed when the pulse is high, actually returns a value (typically 0) when the pulse is low. This greatly compromises our stability because a reading of 0 misleads the Arduino to think that the intensity of the laser has dropped significantly so it needs to increase the power giving to the AOM. The result is a sudden jump on the laser intensity. This has been observed and is mitigated by our Peak Averaging algorithm. <br />
<br />

P.S.: When working with Arduino code (.ino), be mindful that many C++ standard libraries (e.g., std::arr, vector) are not perfectly compatible with Arduino, and the error message may not point this out.

# Schematics
## Experiment Setup Schematics
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/set up/Setup_Schematics.png" width="1000">
We have a 635nm laser first shining into a sequence of optics, where the light is split into two portions. A portion (how large this portion is depends on the dynamic range of our setup) of the laser power goes directly into a photodiode and becomes our raw signal - we monitor the change of laser intensity using this signal. The other portion of the laser power goes through an acousto-optic modulator, or AOM, which acts as a diffraction grating and can diffract incident light into different angles. For our setup, we take the first order light as our output, and it is fed back to the Arduino pin A1. This is the signal we want to stabilize. The Arduino then calculates a correction signal, outputting the voltage from pin DAC1, feeding it into a Voltage Variable Attenuator, or VVA, to control how much power the AOM gets by attenuating the pulsed signal from a function generator, which is set to output a 100kHz pulse signal with 50% duty cycle. The more power the AOM gets, the more optical power is distributed to the first order light.
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/set up/Experimental_Setup.png" width="1000">

## Circuit
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/set up/PLIS Circuit.png" width="1000">
To increase our dynamic range, we want the control voltage going into the VVA to have a large range. The control voltage is generated by the Arduino's DAC, which has a limited range of output: 0.5-2.7V. Hence, we added a non-inverting op-amp with gain 5x to expand its range. Note: a differential op-amp was used in the past. It seemed to work better...<br />
<br />
We also added a non-inverting op-amp with gain 7x between the photodiode and the Arduino ADC. In practice, our device should disturb with the experiments minimally, that implies only taking a small portion of laser power to feed into our stabilization loop. Hence, the signal given by the detecting photodiode will be weak and we want to amplify it so the Arduino can resolve the signal better.


# Feedback Loop
## Short Pulse Sampling
We are currently implementing a peak average algorithm to capture the laser beam intensity and calculate error signal. Essentially we calculate the error signal based on past 100 ADC readings by sorting this array and averaging the leading 10 terms. These two numbers (100, 10) are very conservative choice for a 100kHz pulse with 50% duty cycle and we have tested them to perform good stabilization. Ideally we hope the sampling window and number of valid leading terms to be able to self-adjust to different frequency and duty cycle. We have attempted synchronizing arduino ADC sampling with the real laser pulse by either using a Transistor-Transistor Logic(TTL) or directly sending pulse signal synchronized with laser pulse to arduino's digital pin. However, due to constraints from arduino runtime, we observe severe phase mismatch between two pulses which makes the ADC sample plenty of unwanted random intermediate and off-pulse voltage levels. We also attempted just taking one single max over a smaller sampling window; however, we sometimes observed large jumps in voltage levels, which is dangerous to our equipments, because of sudden fluctuations in the raw signal. Below is a graph of typical arduino ADC readout:
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/arduino code/ADC_readout.PNG" width="1000">


## Proportional-Integral-Derivative (PID) Controller
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/PID control/PID_Control.png" width="1000">
After the Arduino successfully samples one ADC reading, it passes that value, called the signal, to our PID controller. First, the signal is compared with a setpoint, which represents the desired intensity level set by the user, and the difference of these two values are multiplied by the k_p coefficient - this gives the proportional (P) correction. The P correction acts like a reaction force - the more deviated the signal is from the setpoint, the stronger the correction is. And the P coefficient gives a measure of how strong the reaction is. A judicious choice of the P coefficient is important, because a large P coefficient will result in oscillation whereas a small P coefficient makes the stabilizating process slow. <br />
<br />
By P correction alone, often the intensity cannot get back to the setpoint. This is because when the difference between the setpoint and the signal becomes smaller, the P correction also diminishes, resulting in weaker correcting power. Eventually, the geometric series converges to some fixed value, away from the setpoint. Thus, we introduce the integral (I) correction. The I correction sums all the previous errors (defined as signal - setpoint), and tries to minimize this cumulated error. It is therefore able to detect the aforementioned constant offset and bring the intensity to the wanted value. <br />
<br />
The derivative (D) correction is used as a damping term to suppress overshoots and oscillations. When the P coefficient is too large, the controller tends to overcorrect for an error, which introduces extra noise and could sometimes damage the system. The D correction is proportional to the derivative of the signal, so when the signal is changing too fast, either because of overcorrecting or the signal is truly flunctuating suddenly, the D correction kicks in to slow down these abrupt changes. Below is a plot from the scope that shows how turning on the D term helps calming down the oscillation.
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/results/D_coefficient.png" width="1000">

# Results
## Long term analysis
### Single Peak and Single Sampling Window Inspection
<img src ="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/results/peak fluctuation revised.png" width="1000">
Every time we update our setup we perform an overnight stabilization trial with data obtained from the oscilloscope. We choose not to directly communicate with the scope (which takes data eevery 2ns and leads to as many as 5*10^8 data point per second) but instead through a server that takes data every 200μs (about the same size as our sampling window). This appears to be a valid choice since no significant fluctuation at sub millisecond scale is expected; nonetheless every data set contains a consistent ~5% percentage error regardless of the time scale even after we fix the overshooting issue caused by arduino runtime and inconsitent pulse sampling. Consequently, we suspect the laser, the pulse, and the photodiode exhibit innate fluctuation at sub millisecond or down to microsecond level that we essentially cannot control. We should have characterized this at the very beginning of the experiment but we made false assumption that the laser and pulsing mechanism are ideal at sub millisecond scale. Above is a two pulse data sample taken directly from the scope with 2ns time resolution that exhibit ~5% percentage error.

<img src ="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/results/300 us stabilized.png" width="1000">
Then we collect data for one sampling window (~300 μs) again using the scope to maximize resolution. By plotting a histogram we can select threshold for being on-pulse (~0.04V). Then we apply this threshold cut to the raw data and calculate the percentage error for on-pulse data to be 4.3%. Inside one sampling window there is no stabilization event happening, so this percentage error characterizes the statistical fluctuation in our measurement, for the stabilized signal. For data we present here and below, we use percentage error as a measure for how stabilized the intensity is: the higher the percentage error, the more spread out the intensity is, the worse the stabilizability.


<img src ="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/results/300 us raw.png" width="1000">
To single out the fluctuation of the laser itself, we also take sample data of the raw laser power over one sampling window and calculate its percentage error to be 1.2%. The raw signal being more stabilized than the stabilized signal within one sampling window can be explained by extra uncertainties introduced by the AOM. <br />


### Overnight Trial
It is worth noting that an overnight trial contains tens of thousands of data with both on and off pulse and intermediate value which makes the graph really messy and hard to analyze. Thus we plot it as a histogram, calculate the percentage error of high peak, and compare it with the percentage error of short-term pulse. 

<img src ="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/results/6 hour raw.png" width = "1000">

We observe a drift in the raw signal's intensity for this 6-hour trial: the percentage error of the raw signal is 3.2%, 3 times the percentage error for a 300μs trial, which suggests a drift in the laser intensity in the long run. If there is no stabilization happening, we would expect the stabilized signal to have a 3 times larger percentage error as well, compared to the percentage error within one sampling window (4.3%). 

<img src ="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/results/6 hour stabilized.png" width = "1000">


The histogram presented above illustrates the stabilized intensity signals of a pulsed laser over a 6-hour overnight trial. The percentage error for this stabilized signal was 4.4%, no where near the 3x drift of the raw signal. While this value falls short of our goal of 1%, it aligns closely with the 4.3% error of the system's short-term fluctuation over 1 sampling window, which shows that our device is suppressing any detectable errors (longer than 1 sampling window).



## Simulating Practical Fluctuations
Our testing laser does not drift significantly over long time, therefore, to show the effect of our stabilization, we placed a variable filter in front of the laser which blocked a portion of light depending on how much it is rotated. Below are two situations which could happen in practice. <br />

### Adiabatic Fluctuation
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/results/Gradual.png" width="1000">
For this plot, we were changing the filter gradually. The raw signal is changing wildly in amplitude, however, the stabilized signal remains at the setpoint. This shows that the stabilization is taking place. This artificial fluctuation takes 18s to complete, which may seem to be slow, but the speed of this fluctuation is still very large compared to more realistic drifts, so we expect our device to handle slower, more realistic fluctuations just as well.<br />
<br />
At the right end, the stabilized signal is brought up by a larger fluctuation and the stabilization fails. This is because when the raw signal is higher than the setpoint, the Arduino tries to output a low voltage to increase the attenuation of the VVA, so the AOM gets less power and less optical power will be delivered; however, the Arduino output has got to the minimum and it cannot lower the intensity any more. So the stabilized signal is now swinging with the fluctuation. This is what happens when the error is out of the dynamic range. <br />

### Long Deadtime 
<img src="https://github.com/vinb7/pulsed_laser_intensity_stabilization/blob/main/results/Deadtime.png" width="1000">
This is simulating when people stop running experiments and only come back sometimes later. When no experiment is running, the laser is blocked but is still on and drifting, yet the Arduino receives no information regarding how the laser intensity is changing Then, when people restart the experiment, we expect our device to bring back the free-running laser despite this lack of information. As we see here, the stabilization is able to function even after a period of long deadtime. How long it takes to bring back the optical power depends on the settings of PID coefficients. With our default setting, we typically saw stabilization within 10ms.<br />
<br />

# Future Plan
The top priority is to find a microcontroller with faster sampling rate and shorter processing time. We made the Arduino DUE to work by designing sophisticated algorithm, which sacrifices short-term stability for long-term reliability. Currently, the Arduino is ignorant of fluctuations faster than 100ms, which makes our goal of suppressing the flunctuation down to 1% unreachable. However, if a new microcontroller can keep up with the short pulse length, a more straightforward algorithm can be employed which should make this blind time much shorter and therefore we can address those fast fluctuations. <br />
<br />
Our current setup concerns most about fast iteration, so we used breadboards for circuit. However, breadboard is more susceptible to noise than a printed circuit board, so we should switch to PCB once we are confident with our final circuit design. We should also replace the two exterior DC power supplies with Acopian AC-DC supplies to make the PCB more compact and portable. 

# Contributors
This project was made for UCSB's PHYS CS15C course by Zhuoru Li, Tommy Dan, and Vinay Baid.<br />
<br />
This project was built upon the [Continuous-Wave Intensity Stabilization](https://github.com/Jayich-Lab/laser_intensity_stabilization) by Chaoshen Zhang, Sam Gebretsadkan, Vivian Liao and Rei Landsberger. <br />
<br />
We especially want to thank Luka Sever-Walter, Sam Gebretsadkan, Chaoshen Zhang, Professor Andrew Jayich, Eric Deck, and Robert Kwapisz for their continuous support over the course of this project. <br />
