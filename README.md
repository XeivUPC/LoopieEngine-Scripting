

# Loopie Engine README  

<img width="352" height="352" alt="logo - Copy" src="https://github.com/user-attachments/assets/105c7d03-2966-4a57-a751-971171ffb169" />

Videogame Design & Development  
Game Engines \- Manish Thani  
5th Semester \- CITM (UPC)  

## Summary

1. [Introduction](#introduction)
2. [Team Members](#team-members)
3. [Sub-systems brief explanation](#sub-systems-brief-explanation)
4. [In-Depth Scripting description](#in-depth-scripting-description)
5. [Mono Integration](#mono-integration)
6. [C++ to C\# conversions](#c-to-c-conversions)
7. [Text Editor](#text-editor)
8. [Tank Scripts](#tank-scripts)
9. [Links](#links)

## Introduction

The Loopie Engine was born as a project to explore our capabilities while working together as a team. Since the end of the first year, proposals to make works together were sparked, but never ignited. 

With the Engines subject, the idea of getting together the very obviously two best programmers in the class alongside a bright promising young man in the field finally came to fruition.

We wanted to make the best possible engine, and, while far from it due to the lack of documentation and team-working practices, it’s safe to say that there’s a lot of work put into it, and ideas that will carry some of the members of this team far in their respective fields.

With heavy inspiration from Unity, and hundreds of hours of view-time of videos from *Cherno*, *Casey Muratori*, and other programmers out there, the main idea was to create an Engine that was similar in functionality to Unity, although more limited and with its own twist and aesthetic. And thus, Loopie Engine (Loopie because Game-*Loop*), was born.

What’s more, a lot was learnt from tackling this project, in team-working, problem-solving, architecture and raw coding.

Without further ado, here’s the team members who made this Engine possible:

## Team Members

<img width="262" height="395" alt="photo" src="https://github.com/user-attachments/assets/4ae48e9b-3614-486a-a8b6-b6d26f82ad9b" />

Adrià García [XeivUPC](https://github.com/XeivUPC) → AKA: *The master-mind*. This guy just codes for fun. 

<img width="256" height="320" alt="photo" src="https://github.com/user-attachments/assets/47979cae-c388-448d-9ef6-b669fc7a1b9e" />

Ana Alcazar [Ana-Alcazar-Cobo-CITM-UPC](https://github.com/Ana-Alcazar-Cobo-CITM-UPC) → AKA: *The great Side-kick*. As consistent as a busy bee.

<img width="322" height="322" alt="photo" src="https://github.com/user-attachments/assets/e0cee04b-42d8-46f1-824c-a44713004735" />

Pol Sierra [AbyssWatcher64](https://github.com/AbyssWatcher64) → *He kinda codes after work-hours*. Tries his best despite his little free time.

## Sub-systems brief explanation

The Engine, as of now, counts with multiple sub-systems that have been expanded throughout this semester.

- **Rendering**: Renders things on the screen, transforming objects and mesh data from 2D to 3D, works with light, shaders, etc.  
  - 3D → Meshes, Geometry, Textures.  
  - Shaders → VBO, EBO, VAO.  
- **Scene**: Focuses on holding all together by connecting the different modules and making them ready for usage.  
  - Entities (AKA Game-Objects)  
  - Serialization (Save & Load of scene)  
  - Space Partitioning (Octrees) & Mouse Picking  
- **Component System**: Used by entities to hold different data.  
  - Camera, Mesh Renderer, Script, Transform.  
- **Time Management**: Allows to play, stop, pause and resume the game.  
- **Scripting**: Allows the usage of C\# to program scripts for game functionality.   
- **Engine library & Editor separation**: The whole engine is separated into the Engine library which can be imported, and then the Editor which is the part that uses said library and exposes it through its front-end.  
- **Resource Manager**: Imports resources to lighter-weight versions then used by the Engine. Also manages the directories and files.  
  - Directory Manager.  
  - Material, Mesh, Script, Texture importers.  
- **Event System**: Allows systems to be connected and notify each other by being subscribed to their topics.  
- **UI**: Creates UI systems by using the ImGui library.  
  - Windows.  
  - Docking system.  
- **Log**: Log system for asserts and debugging information.  
- **Save / Load of projects**: Allows for the creation of different projects while working on the Engine.

## In-Depth Scripting description

Our team picked the sub-system Scripting.

We all investigated and saw different Cherno videos recommended by Adrià in order to further understand how it worked ([example here](https://www.youtube.com/watch?v=EKnJOwGhPj4)), in the meantime that we were waiting for Manish, our teacher, to publish our grades and see if there were any corrections needed for it. Adrià also used that time to polish some features of the Resource Manager.

The way we divided our work was the following:

- Adrià: Mono integration, actual tank scripts, helping with Ana.  
- Ana: C++ to C\# conversion (bulk of the work).  
- Pol: Text Editor.

We will explain in more detail each of these in the following pages.

### 

### Mono Integration

Adrià: We installed mono locally in our PC, then we selected the required files, and copied them inside our vendor folder. Making use of CMake, we copied those files inside our build directory. 

With that done, we just initialized it inside the engine, and created all the required .dll’s. One of the most difficult ones was the game .dll. 

We created a new Loopie subdirectory, which is in charge of compiling all those scripts, and creating a .dll. With all this, we were able to start using it.

### 

### C\+\+ to C\# conversions

Ana: We’ve created a serialization bridge between the cpp native engine language and the desired user scripting language, for this we’ve done scripts in charge of serializing internal engine functions such as component manipulations,  time managing and entity control called “ScriptGlue” in charge of revealing and serialize those functions to Mono.

Then a second script called “InternalCalls”, that receives those serialized functions and links those into C\# language.

Finally we’ve created some custom internal C\# classes to collect and group those functions and generate a clean and more user friendly interface avoiding the user scripting to access those internal call functions.

Also we’ve created custom variable types (Vector2, Vector3, Vector4, KeyCode) to help users to manage these codes.

### 

### Text Editor

Pol: I was interested in doing the C++ to C\# conversion. However (as per usual) my lack of free time made my winter a very busy one with all the other deliveries \+ exams. I was unable to work on the engine until very late (which I’m guessing many students have done as well), and I didn’t want a blocker TODO for this assignment, otherwise my team would suffer and halt their progress until I was done with mine, with very few days of margin. After a bit of back and forth, I decided to take the Text Editor.

There was [this repository given](github.com/BalazsJako/ImGuiColorTextEdit) as a way to start this part of the code. The way I understood it, I was supposed to create something from scratch resembling something similar to this, however, it is a big project. I now suspect that Manish wanted us to just integrate that into our code. After all, hindsight is 20/20.

However, I felt more interested in getting more familiar with how text-editing works, so I made something more akin to the *Notepad* as a docking window in our Game Engine. It doesn’t have language functionality, or syntax highlighting, but it does the basic things a text editor does, and I did it mostly from scratch, that helped me learn a ton.

The way it works is that when you select a file with a .txt or .cs extension, the Text Editor will be populated with that specific file text.  

<img width="830" height="658" alt="image" src="https://github.com/user-attachments/assets/6820f4f6-3705-4e4a-9c01-f628d2d716b3" />
  
You can then modify the text directly, do Ctrl+S to save it, Ctrl+Z to undo, Ctrl+Y to re-do.

You can select things with the mouse, you can press Home or End to move and select multiple things with the cursor just like you would in a normal text editor.  
You can also see the name of the file you’re currently working on, and an asterisk will appear if changes have been made to the original file.

There are two things that could be improved upon this basic Text Editor:

1. The undo system currently makes a snapshot for each modification. This is fairly inefficient. A command pattern will be considered if the text editor feature wants to be expanded upon.  
2. The scripts and their variables are currently not shown in the editor. This could be done for ease of usage.

Overall, a more neat integration with C\# is in place, but it’s a good place to start.

![ezgif-33489de2d76c4e73](https://github.com/user-attachments/assets/3490a0b9-cd30-45be-bef0-86bb018ab8d8)

### 

### Tank Scripts

Adrià: Making use of our wrapper classes, and the InternalCalls, we created multiple C\# scripts that created the behavior we were looking for. We made use of functions like Entity.Clone / GetComponent / Rotate / … Functions that as we said before, were linked to c++ code. Thanks to that, we just need it to think about the logic, and not about the way of doing that logic work.

![ezgif-3601f3a366763003](https://github.com/user-attachments/assets/278e63e2-56c7-4f3a-9501-b2b9737a4f2a)

## Links

[Demo Video](https://youtu.be/lAaqZoWc5hA)

[Github Repository](https://github.com/XeivUPC/LoopieEngine-Scripting)

[Github Release](https://github.com/XeivUPC/LoopieEngine-Scripting/releases/tag/v0.3)
