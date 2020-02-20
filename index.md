# devicetodatabase

When we are a client we can write to a topic and publish our data. In this case we publish a JSON including temperature, humidity, pressure, illuminance, uva, uvb and soil moisture. We will subscribe to the corresponding topics. On that arduino IDE topics are subscribed using the following commands:

```markdown
String temperatureTopic = "itp/" + DEVICE_ID + "/temperature";
String humidityTopic = "itp/" + DEVICE_ID + "/humidity";
String ledTopic = "itp/" + DEVICE_ID + "/led";
String soilTopic = "itp/" + DEVICE_ID + "/soil";
String pressureTopic = "itp/" + DEVICE_ID + "/pressure";
String illuminanceTopic = "itp/" + DEVICE_ID + "/illuminance";
String uvaTopic = "itp/" + DEVICE_ID + "/uva";
String uvbTopic = "itp/" + DEVICE_ID + "/uvb";
```


MQTT in addition to be lightweight gives us a serve path so its easier than HTTP we will be using MQTT. MQTT broker does not store anything, it receives a message, it asks who is subscribing to these topics, and then sends it out to them. Our broker is itpdtd.com.

After adding new topic for soil we add this command so we can read and send data

```markdown
int soil = analogRead(A0);
mqtt.beginMessage(soilTopic);
mqtt.print(soil);
mqtt.endMessage();
```

The soil sensor is sending values through an analog pin "analogRead(A0)". For the remaining parameters such as temperature, pressure and illuminance, the ENV sheild is doing the sensing so we have included its library in the beginning 

```markdown
#include <Arduino_MKRENV.h>
```

The soil sensor is inserted into a pot of cilantro for testing, and later into a praying plant in a pot of fresh soil. 

The idea is to monitor data over a period of 48 hours so we need to connect the MKR 1010 to a power outlet. For this we need to tweak the code:

```markdown
//while (!Serial) { }
```
We have to comment this bit out. This is because the code is waiting for a Serial connection before it starts running. The computer provides a serial connection. The power strip does not. 

The data can now be viewed on www/subscribe index.html



______________
 - We’ll use a MKR ENV shield we’ll get one of these, it has sensors. We don’t spend time wiring things up you just plug it in and get going. It has ENV libraries that are really easy to use. 

### Blog Post

- The task was to monitor data


## Welcome to Designing Meaningful Interactions by Katherine Dillon

You can view [Week 4 presentation](https://docs.google.com/presentation/d/1d0VlyH_sJjfWlDtPem4eYXyoOKvPOLYIcXUFmmIPWgE/edit?usp=sharing) to see the problem we are trying to solve and the future state journey map!


### 5 characteristics of good UX:
```markdown
Intuitive - without instructional manual
Efficient - worth pepoles time,
Memorable
Anticipates errors
Satisfying - so people want to come back
```

#### A lot of ux designers call themselves product designers.
#### Product shouldn’t come with a manual with instructions it should be intuitive. 
#### Focus on the user and all else will follow 
#### Desirable experience = good business 

```markdown
