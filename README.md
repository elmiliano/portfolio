# portfolio

This repository contains Emiliano Machado's most relevant projects.

## FruitRGB

### Ever struggled to tell if a banana is ripe due to colorblindness?

![FruitRGB demo](/fruit-rgb/Assets/demo.gif)

FruitRGB, an innovative banana ripeness detector, empowers the colorblind to confidently choose ripe bananas using an advanced RGB sensor. 

This collaborative project, developed during the pursuit of a **Bachelor's of Engineering in Cyber Electronics at CETYS University**, leverages the sophisticated TCS34725FN sensor and a custom algorithm to analyze the banana peel's hue and translate it into accurate ripeness stages. 

Its potential extends beyond personal use, offering automation solutions for tasks like fruit sorting in grocery stores or farms.

### Hardware:
![Hardware connections](/fruit-rgb/Assets/hardware.jpg)
- STM32F767ZI Board.
- TCS34725FN RGB Sensor.
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


## audioFinder

### Ever wondered what specific words were said in a podcast, lecture, or long audio snippet?

![audioFinder demo](/audio-finder/assets/demo.gif)

> The audio file used for the demonstration is from the AMC series *Better Call Saul*, Season 3, Episode 5 titled "Chicanery." All credits go to AMC, and creators Vince Gilligan and Peter Gould.

Introducing audioFinder, an innovative iOS app that helps you effortlessly discover all occurrences of a chosen word within an audio file. Perfect for students, researchers, journalists, and anyone who wants to quickly navigate audio content.

audioFinder was a submission to the **2023 Apple Worldwide Developers Conference (WWDC23)** [Student Swift Challenge](https://developer.apple.com/wwdc23/swift-student-challenge/). Created and tested using Xcode, ensuring compatibility with Apple devices.

### Key features:

<div class="row">
    <img src="/audio-finder/assets/intro.png" height="500">
    <img src="/audio-finder/assets/instructions1.png" height="500">
    <img src="/audio-finder/assets/instructions2.png" height="500">
</div>

- **Swiftly Search:** Enter your desired keyword and let audioFinder scan audio files with precision.
- **Real-time Highlights:** See the time-stamp of the word highlighted as the audio plays, offering instant visual reference.
- **Intuitive Interface:** Explore a user-friendly interface with clear instructions and seamless navigation.
- **Powerful Integrations:** Leverage Apple technologies like AV Audio Player for playback control and SF Speech Recognizer for accurate speech-to-text conversion.