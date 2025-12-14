# SPARK MAX Motor Controller - A Guide
 
A guide to using the REV Robotics device outside the FRC ecosystem, as well as a thorough breakdown and reverse-engineering of the internals.

![SPARK MAX](../images/spark-max.jpeg)

## Overview

Ever since I posted on Reddit about how I was able to use Arduino to control a Spark Max outside of FRC uses, I’ve had quite a few questions about it, so I will go through the entire setup and methodology here.

## Contents

- Disassembly and Circuit schematic
- How to control via an Arduino/STM32/etc instead of FRC hardware

## Dissasembly

1st PCB
![PCB1 Top View](../images/sm1-top.jpeg)
![PCB1 Bottom View](../images/sm1-bottom.jpeg)

2nd PCB
![PCB2 Top View](../images/sm2-top.jpeg)
![PCB2 Bottom View](../images/sm2-bottom.jpeg)

### Parts Used

Microcontroller (1st PCB, U1) - STM32F302CC

Motor Driver (2nd PCB, U1) - DRV8320S

MOSFET (Q1-6) - PSMN1R0-30YLD

CAN Transceiver (1st PCB, U3) - 6563MKD

### Further References
A similar breakdown can be found here: https://www.chiefdelphi.com/t/rev-spark-max-teardown/461514/3

## Circuit Schematic
In progress

## Guide to controlling via an Arduino/STM32/non-FRC hardware
In progress

## Frequently Asked Questions
In progress
