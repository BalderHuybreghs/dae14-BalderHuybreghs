<a name="readme-top"></a>

<!-- GENERAL GAME INFO -->
<br />
<div align="center">

  <h1 align="center">Celeste</h1>

  <p align="center">
    Celeste is a challenging indie platformer with tight controls and precise gameplay mechanics, inviting players to navigate through difficult levels and obstacles as they climb Celeste Mountain.
    <br />
    <strong>Original game : </strong>
    <a href="https://store.steampowered.com/app/504230/Celeste/"><strong>General info »</strong></a>
    ·
    <a href="https://youtu.be/ttJFyvCfFn8"><strong>Youtube video »<strong></a>
    <br />
    <br />
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#my-version">My version</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
    </li>
    <li><a href="#how-to-play">How To Play</a></li>
    <li><a href="#class-structure">Class structure</a></li>
    <li><a href="#checklist">Checklist</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project
![Celeste Image](https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fimages7.alphacoders.com%2F901%2Fthumb-1920-901149.png&f=1&nofb=1&ipt=fa9d60b584b1a86815d76b8e161bf6c6bbadfda9e44e3ee7ad3d31a0d0c6e0b1&ipo=images)

Here's why:
* It is a well designed parkour-platformer game
* There are only 6 main controls in the game
* The player has the following abilities: move, jump, grab, dash and crouch
* The graphics are not too complex to recreate, while still providing a good challenge
* The levels are simple in design, only consisting of a few core mechanics used in intelligent ways
* The mechanics are well documented on the internet, making it easier to recreate an as-close-as-possible feeling
* There are some asset dumps and tools available to help with the development
* The game is very open to modding and provides a lot of information on its inner workings

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## My version
This section gives a clear and detailed overview of which parts of the original game I planned to make.

### The minimum I will most certainly develop:
* Movement Mechanics
* Dash ability
* Basic Level
* Basic parkour mechanics
* Berries
* Wood platforms

### What I will probably make as well:
* Particles
* B-side timed level
* Death animation
* Breaking platforms
* Moving platforms

### What I plan to create if I have enough time left:
* Hair physics
* Particles
* Bubble when picking up b-side
* Visual effects / details

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started
### Requirements
* Visual Studio 2022+

### Compiling
* Simply build (and run) the project using visual studio

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- HOW TO PLAY -->
## How to play
My implementation of the game has 2 modes: play and edit, you can switch between modes live by simply pressing tab. The control schemes for both modes will be listed here.

### Controls
- Tab to switch mode

#### Play
- F1 to toggle debug mode
- a & d to move
- space to jump
- Shift to dash in the direction you are looking
- w & s to climb up or down (or crouch when grounded)
- j to grab
- a & d against a wall to wall slide

#### Edit
- wasd (or drag with middle mouse) to pan the camera
- z & x to zoom the camera
- q and e to switch editor tools
- space to save the level
- LMB, RMB and scrolling are tool-specific
- g to place the player spawn location

##### Tools
- Tilemap (there are 2 tilemap tools, front tilemap and back tilemap), LMB to place, RMB to remove, scroll to select tile
- Camera click and drag to draw camera rectangles, when a player enters one of these, the camera moves to the bottom left of the rectangle the player is in. RMB inside a rectangle to remove it
- Deathzone tool, this tool is used to mark areas in which the player should die
- Object tool, the tool responsible for placing and managing objects
- Background tool, this tool is used to mark the area used in the background calculation 
- Cassette tool, this tool is used to mark the cassette music area

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CLASS STRUCTURE -->
## Class structure
I will provide a quick and abstract overview of the program structure, rather than the class structure, since the class structure is incredibly complex.

First of all my entire game makes use of a screen manager system, which keeps track of the current screen and uses that to draw, update, etc. 

There are 2 big screens (not accounting for the loading screen) inside te game. The level editor screen, and the playing screen. Both of them keep track of the Level class.

The level class keeps track of the TileMap class, which can be edited. The level class loads in its data from a .cev file, which is a binary format that is compressed using a simple compression algorithm.

The level class keeps track of most level related things such as the parallax background (class) and more.

Textures are loaded in a unique way, as they are stored, managed, and deleted globally by the TextureManager class (singleton pattern). This class is called whenever I need a new texture.

The level editor actually has a vector of EditTools, which receive some information (by choice) in their constructor, and they can then manipulate that information in the update function.

Last is the ObjectManager. The reason this exists is because of the way levels are stored. The object manager is essentially a registry linking object IDs to an actual (copyable) instance of an object, which the ObjectBlueprint class uses to instance an object (The level class keeps track of a vector of ObjectBlueprints, which are written to the file in edit mode, and used to build the level in play mode).

### Object composition 
Since there are a large amount of classes that make use of object composition, I will simply list them without further elaboration.
As of today, there are so many classes making use of this pattern that I decided to neglect writing them all down.

- Limb
- ObjectManager
- GameScreen
- ScreenManager
- TextureManager
- Particle
- ParticleEmitter
- Sprite
- Tilemap
- ParallaxBackground
- Game
- Hair
- Player
- EditTool
- CameraTool
- TilemapTool
- Level
- EditorScreen
- LoadingScreen
- MenuScreen
- PlayingScreen

### Inheritance 
Inheritance is applied in 3 core parts of the project. 

The first being for shapes. For which the base class Shape is defined, and and a couple children classes that inherit from Shape (RectangleShape & CircleShape). This is useful because that means you can ask for a Shape pointer in the game, instead of asking for a specific shape.

The second part would be for game objects. Since the level format makes use of a blueprint system to create objects based on a list of IDs stored per level, a base class GameObject exists, which all objects that can be loaded into a level inherit.

This system is used in combination with an ObjectManager class, which can copy an instance of an existing GameObject based on its ID, such that those objects serve as a blueprint to build a level.

It is designed this way because storing instances of GameObject raw in a binary file would mean we lose the info on what type of class the object was created with, thus the game cannot know how many bytes (or what type they are) to read next.

The third one, for which the implementation is more complex, is the screen system. There exists a base class GameScreen, which the ScreenManager class keeps hold of. The ScreenManager is part of the main game class, which calls GameScreen functions to draw, update etc. Any class that inherits GameScreen can be rendered on its own in the game.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CHECKLIST -->
## Checklist

- [x] Accept / set up github project
- [x] week 01 topics applied
    - [x] const keyword applied proactively (variables, functions,..)
    - [x] static keyword applied proactively (class variables, static functions,..)
    - [x] object composition (optional)
- [x] week 02 topics applied
- [x] week 03 topics applied
- [x] week 04 topics applied
- [x] week 05 topics applied
- [x] week 06 topics applied
- [x] week 07 topics applied
- [x] week 08 topics applied
- [x] week 09 topics applied (optional)
- [ ] week 10 topics applied (optional)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Your Name - balder.huybreghs@student.howest.be

Project Link: https://github.com/BalderHuybreghs/dae14-BalderHuybreghs

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments
Resources I found useful when working on this project

* [Celeste Wiki](https://celeste.ink/wiki/)
* [Celeste Analysis](https://aran.ink/posts/celeste-tilesets)
* [Celeste Asset Extractor](https://github.com/TeWu/CelesteExtractor/releases)
* [Unordered Map Documentation](https://en.cppreference.com/w/cpp/container/unordered_map)
* [Pair Documentation](https://en.cppreference.com/w/cpp/utility/pair)
* [Binary File Writing & Reading in C++](https://cplusplus.com/doc/tutorial/files/)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

