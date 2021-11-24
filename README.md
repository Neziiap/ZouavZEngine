# ZouavZ EnginE

3D Game Engine, allow to create rpg with procedural generation
![#Isart](Media/ISART-Digital.png)

___

| Name | Role|
|:---:|:---:|
| GAVELLE Antony | Game Programmer
| DEPREZ Nicolas | Game Programmer
| GRONDIN Jérémy | Game Programmer


## Features :
- Dockable editor windows : Hierarchy, Inspector, Console, Project, Procedural Generation, Scene, Game
- GameObject's parent can be moved in Hierarchy
- Create, edit, save and load of the Scene
- Mesh display with texture and editable shader 
- Component modification of a gameObject in inspector
- Create and save of prefab
- Procedural generation
- Skybox
- Animation
- PhysX : Rigidbody/RigidStatic and base collision (box, sphere, capsule), collision detection callback
- File creation
- Sons (Spatialized or not)

## External Librairies : 
- Assimp
- PhysX4.0
- OpenAL
- cereal
- ImGui
- ImGUiGizmos
- std_image
- glfw
- glad
- libsndfile


## How To Build : 
Execute editor : [Here](ZouavZEngine\ZouavZEnginE.exe)
Game : Launch editor, go to Project, open Scene folder and click on World.zes or Load

## Known Bugs :
Warning Physx in the console during certain operations
Mesh of field stay on background if a scene is create and procedural generation was on previous scene

## License :

    MIT License

    Copyright (c) 2020 Grondin Jérémy Gavelle Anthony Deprez Nicolas

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.