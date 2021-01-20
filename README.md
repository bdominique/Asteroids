# Asteroids
After building some basic C++ std console games such as Snake and Tic-Tac Toe in school, I decided to build a more advanced Asteroids game that can also be played by a Reinforment Learning Agent.

![alt text](https://github.com/bdominique/Asteroids/blob/master/Screenshot%20(114).png)

Asteroids is a C++ std console game where you take control of a spaceship. Your job is to survive for as long as possible while avoiding asteroids and lasers. The game's speed increases according to the score of the player and has 5 basic levels of speed, and 5 difficulty levels. The difficulty levels coorespond to how many asteroids you'll see on screen (A difficulty of 1 means that there'll be 1 asteroid on screen and a difficulty of 5 means that there will be 5 asteroids on screen). Regardless of what difficulty the game is at, the first asteroid is set to always follow the location of the player to force them to keep moving. The others are set to randomly pick a new column once they reach the bottom of the screen.

![alt text](https://github.com/bdominique/Asteroids/blob/master/Screenshot%20(115).png)

The game runs in two loops: an _i_ loop that generates output onto the console that is updated continuously, and a _j_ loop that is updated whenever _i_ reaches the bottom of the screen (when  _i_ == 29).  _j_ helps us keep track of how long the game has been running for, and depending on the value of  _j_ certain things will happen on screen. For example, whenever  _j_ modulus 100 is equal to 50, this signifies the beginning of a Laser Round, where the player has to now dodge laser beams instead of asteroids.

![alt text](https://github.com/bdominique/Asteroids/blob/master/Screenshot%20(116).png)

The location of laser beams is also random, and at most 2 will come out at once. Laser Rounds last until _j_ modulus 100 is equal to 87, and after the complete of a Laser Round the player is given a Powerup of +10000 to their score.

![alt text](https://github.com/bdominique/Asteroids/blob/master/Screenshot%20(117).png)

![alt text](https://github.com/bdominique/Asteroids/blob/master/Screenshot%20(120).png)

As an added feature, the player can also pause at anytime with the Spacebar.

![alt text](https://github.com/bdominique/Asteroids/blob/master/Screenshot%20(119).png)

The player only has 1 life. Once the player has been hit by an asteroid or a laser, the game will check to see if the player has a score that's worthy of being on the High Score list. One of the score on the list is 0 by default, so the player will make it onto the list at least once while playing. Whether the player gets a High Score or not, they'll be asked if they want to play again (press 'r') or exit the game (press the Spacebar).

![alt text](https://github.com/bdominique/Asteroids/blob/master/Screenshot%20(118).png)

Inspired by some videos on youtube, I'm currently attempting to create a Deep Reinforment Learning Agent that's capable of playing this game. I believe that because of this game's simple controls that this is a perfect project to try and improve my AI skills on. More updates coming soon!
