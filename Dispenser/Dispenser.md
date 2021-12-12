# Dispenser
The dispenser is composed of an mbed, a Bluetooth LE module, an LCD, Sonar, and 2 solenoids with their driving MOSFETs. 
The following black diagram provides a simple representation of how these are connected.

![4180 block diagram](https://user-images.githubusercontent.com/46701545/145703055-f64e3d48-0edb-41c2-990a-2bac41b045a4.png)

To use the dispenser, a user must first order a drink by typing in the name of the soda they want in the app ("coke" or "sprite" in this case). A light will come on in the upper left of the corresponding selection on the LCD.
Then, the train has to be driven towards the dispenser. Once it is close enough for the sonar to detect (indicated by the green dot at the top right of the LCD), the dispenser will drop the chosen drink into the train after a few seconds. 
