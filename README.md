# portfolio

This repository contains Emiliano Machado's most relevant projects.

## FruitRGB

### Ever struggled to tell if a banana is ripe due to colorblindness?

![FruitRGB demo](/fruit-rgb/Assets/demo.gif)

FruitRGB, an innovative banana ripeness detector, empowers the colorblind to confidently choose ripe bananas using an advanced RGB sensor. 

This project, developed for a Bachelor's of Engineering in Cyber Electronics at CETYS University, leverages the sophisticated TCS34725FN sensor and a custom algorithm to analyze the banana peel's hue and translate it into accurate ripeness stages. 

Its potential extends beyond personal use, offering automation solutions for tasks like fruit sorting in grocery stores or farms.

### Hardware:
![Hardware connections](/fruit-rgb/Assets/hardware.jpg)
- TCS34725FN RGB Sensor.
- STM32F767ZI Board.
- 16x2 LCD Screen.

### Software:
![Web portal for viewing readings](/fruit-rgb/Assets/web-portal.png)
- STM32 CubeMX and C in the STM32 IDE.
- Python:
    - Serial connection with STM32 board.
    - Client-Server Socket.
    - Banana ripeness algorithm.
    - MySQL Database connection.
- MySQL Database.
- JavaScript:
    - Express server.
    - React website portal.
- Tailwind CSS.