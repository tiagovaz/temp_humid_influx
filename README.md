Somewhere on the Internet there's someone willing to plot.

```
<tvaz> I've set a fancy telegraf -> influxdb -> grafana thing for the home sensors, and I regret it
<tvaz> it seems everybody is willing to use these things for such small projects, but seems really overkill
<tvaz> in terms of resources and learning curve
<tvaz> then the basic feature I wanted, which is embedding static graphs into a public website doesn't exist
<tvaz> wise pollo for using a set of "old-fashion" stuff for his thing :-)
<pollo> tvaz: just cram all this in a container!
* pollo hides
<tvaz> it's borderline offensive :-)
<lavamind> blasphemous, even
<tvaz> my goal was -> get temp / humid and make a two lines simple chart
<tvaz> then I got into this kind of stuff: "As I understand it, you cannot connect Grafana 7 with Influx 2.0 OSS using influxql. Only Flux. Which has caused me to start a crash course on Flux in the past week."
<pollo> Fwiw, you can make pretty graph with Python
<pollo> Although I still have a preference for R :9
<tvaz> the irony is that I wanted to learn these modern things to not feel too old. But more I play with them, older I feel.
<pollo> That kind of tech is written for very large companies, not for us
<tvaz> I'm surely stepping back to something likea 5-line matplotlib solution 
<tvaz> pollo, TIL, at the experience level
<tvaz> I had some hope, because when I started my research I saw so many small tech-peasants like me using that workflow for their small arduino adventures
<tvaz> BUILD YOUR WEATHER STATION WITH ARDUINO DHT11 TELEGRAF INFLUXDB GRAFANA GIRAFE IN 5 MINUTES!
<tvaz> oh I forgot to add DOCKER
<tvaz> so you spend 5$ in your microcontroler setup, then you need a 200$/year VPS and 7 crash courses in order to plot your data
<tvaz> then you go to bed around 3am and wakeup at 6am with baby's fingers inside your nose
<tvaz> that was my monday mild rant, thanks for listening
<pollo> Stick the baby in a container!
<pollo> That way you can manage stray fingers more easily :)
<tvaz> lol
```
