The Trophy of the Future
=======================

The Trophy of the Future is the world's first internet-enabled trophy. It was created for my fantasy football league, the Westfield Athletics Club. It comprises an LED matrix embedded in polyurethane resin and mounted on a mahogany base. It is controlled by an Arduino Yún, which pulls NFL news, fantasy league scores and past league champion names from the internet and displays them on the LED matrix.

Check out the documentation for the [fabrication](http://samjbrenner.com/notes/making-the-worlds-first-internet-enabled-fantasy-football-trophy-part-1-fabrication/) and the [programming](http://samjbrenner.com/notes/making-the-worlds-first-internet-enabled-fantasy-football-trophy-part-2-programming/) of the trophy.

![Trophy](http://samjbrenner.com/notes/wp-content/uploads/2014/02/P1090435-800x460.jpg "Trophy")

References
----

The trophy is built on top of these third-party technologies:

* [Arduino Yun](http://arduino.cc/en/Main/ArduinoBoardYun)
* [Parola](https://parola.codeplex.com/) (LED scrolling library)
* [Kimono](http://kimonolabs.com/) (HTML scraper -> API)

I also built these tools to help me out along the way:

* [linino-to-serial](https://github.com/sambrenner/linino-to-serial) (Yun communication from Linino -> ATmega)
* [yun-easy-wifi-switch](https://github.com/sambrenner/yun-easy-wifi-switch) (Toggling Yun wifi networks)

Other miscellaneous links:

* [Smooth On Crystal Clear](http://www.smooth-on.com/Urethane-Plastic-a/c5_1120_1156/index.html)
* [Python the Hard Way](http://learnpythonthehardway.org/book/)
* [Scheduling Jobs With cron on OpenWrt](http://martybugs.net/wireless/openwrt/cron.cgi)
