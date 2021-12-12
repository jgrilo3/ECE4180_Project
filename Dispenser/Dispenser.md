# Dispenser
The dispenser is composed of an mbed, a Bluetooth LE module, an LCD, Sonar, and 2 solenoids with their driving MOSFETs. 
The following black diagram provides a simple representation of how these are connected.

![4180 block diagram](https://user-images.githubusercontent.com/46701545/145703157-8107220d-f023-4b13-8215-ff876651ba93.png)

To use the dispenser, a user must first order a drink by typing in the name of the soda they want in the app ("coke" or "sprite" in this case). A light will come on in the upper left of the corresponding selection on the LCD.
Then, the train has to be driven towards the dispenser. Once it is close enough for the sonar to detect (indicated by the green dot at the top right of the LCD), the dispenser will drop the chosen drink into the train after a few seconds. 

## Demo Video

<!DOCTYPE html>
<html>
<body>
  
  <iframe src="https://drive.google.com/file/d/1lhXcp-h30BZg4jmAzaObrC8Te0O7h2Sb/view?usp=sharing" ></iframe>

</body>
</html>
