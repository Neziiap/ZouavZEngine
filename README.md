# ZouavZ EnginE

-----

> Développeur :
> Gavelle Anthony
> Grondin Jérémy
> Deprez Nicolas

<!--<div style="text-align:center">
    
</div>-->

## Description :
Projet moteur : moteur de jeu 3d et création d'un rpg

## Features :
Différentes fenêtre d'éditeur dockable:
-> Hierarchy, Inspector, Console, Project, Generation Procédurale, fenêtre de la scène, fenêtre du jeu
Déplacement de gameObject dans la hierarchie en glissant déposant vers un autre parent
Création, modification, sauvegarde de scène
Affichage de mesh avec texture et shader editables
Modification des composants d'un gameObject dans l'inspector
Création et sauvegarde de prefab
Génération procédural de terrain
Skybox
Animation
Sauvegarde et chargement de scènes
PhysX : Rigidbody/RigidStatic et collision de base (box, sphère, capsule), callback de détection de collisions
Création de fichier
Sons (spatialisé ou non)

##Librairie externe : 
Assimp
PhysX4.0
OpenAL
cereal
ImGui
ImGUiGizmos
std_image
glfw
glad
libsndfile


## Comment executer : 
Executer l'éditeur : Release\ZouavZEnginE.exe
Jeu: lancer dans l'éditeur la scene aller dans l'onglet Project ouvrir le dossier Scene et appuyer sur World.zes ou appuyer sur Load

## Bug connus
Warning Physx dans la console lors de certaines opérations
Lorsqu'on crée une scène et qu'il y avait déjà la génération procédurale, le mesh du terrain reste en background 

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
