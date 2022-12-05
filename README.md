# UE4_LineTracing
A demonstration of all the UE4 LineTrace* functions in the form of a laser pointer

## Installation
Clone this repository into the Plugins folder of your Unreal project. (You may need to create the Plugins folder)

## Usage
Drag and drop the LaserPointerByChannel actor and/or LaserPointerByObject actor into your scene. Point it at whatever you'd like.
A green line representing the laser's path will be visible in the editor and will penetrate all geometry. A pink dot indicates
a blocking hit. Yellow dots indicate non-blocking hits. In the "Laser Pointer by Object" section of the properties panel there
are options for the different parameters to the LineTrace* functions.
