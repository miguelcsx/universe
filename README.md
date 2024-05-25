# Universe
:collision: Universe simulator


## Functionalities

### Description

The universe simulator is a program that simulates the movement of numerous objects in outer space. The program is written in C++ and creates a 3D scene with all the interactions. The program allows the user to interact with the simulation by moving the camera, zooming in and out, and rotating the camera to view the universe from different angles. The program also allows the user to pause and resume the simulation, as well as to reset the simulation to its initial state. The program also allows the user to toggle between different modes, such as viewing the universe from a preset location or following a specific object in outer space.

### Libraries

- The program uses the OpenGL library to render the 3D scene of the universe. The program uses OpenGL to create a window and to draw the objects in the universe. The program uses OpenGL to render the objects in the universe with different colors and sizes. The program uses OpenGL to render the trajectory of the objects in the universe as a line that shows the path of the object in space. The program uses OpenGL to render the octree view of the universe as cubes that represent the regions of the octree and the objects that are in each region.

- The program uses the OpenMP library to parallelize the calculation of forces between objects and the detection of collisions between objects. The program uses OpenMP to distribute the work across multiple threads to efficiently calculate the forces between objects and to detect collisions between objects. The program updates the positions of the objects in the universe at each frame of the simulation based on the forces applied to each object.

- The program uses the GLFW library to create a window and to handle user input. The program uses GLFW to create a window that displays the 3D scene of the universe. The program uses GLFW to handle user input, such as moving the camera, zooming in and out, and rotating the camera to view the universe from different angles. The program uses GLFW to handle keyboard input, such as pausing and resuming the simulation, resetting the simulation to its initial state, and toggling between different modes.

- The program uses the GLM library to perform mathematical operations on vectors and matrices. The program uses GLM to calculate the forces between objects and to update the positions of the objects in the universe based on the forces applied to each object. The program uses GLM to perform transformations on the camera, such as moving the camera, zooming in and out, and rotating the camera to view the universe from different angles.

- The program uses the imgui library to create a menu for the user to interact with the simulation. The program uses imgui to create a menu that displays the current state of the simulation, such as the number of objects in the universe, the current time, and the current mode. The program uses imgui to display the controls for the simulation, such as the keys to move the camera, pause and resume the simulation, reset the simulation, and toggle between different modes.


### Objects

- The universe is populated with objects that are randomly generated at the start of the simulation. The objects are of different types, such as stars, planets, and asteroids. Each object has a unique mass, radius, and velocity. The objects are randomly placed in the universe and move in random directions.

- The objects move in based on the forces applied to each object until they collide with another object. Upon collision, the objects merge to form a new object with a combined mass, radius, and velocity. The new object is placed at the location of the collision and moves in the direction of the combined velocity of the two objects, and so on.

### Octree

- The octree is a data structure that divides the universe into smaller regions to efficiently calculate the forces between objects and to detect collisions between objects. The octree is used to store the objects in the universe and to quickly determine which objects are close to each other and may collide. The octree is updated at each frame of the simulation to reflect the current positions of the objects in the universe. The program uses the octree to calculate the forces between objects and to detect collisions between objects. When two objects collide, the program merges the objects into a new object and updates the octree to reflect the new object's position in the universe.

- By using octree, the program compares each object with the others in the same region, and if they are close enough, the program calculates the forces between them. This allows the program to efficiently calculate the forces between objects and to detect collisions between objects. The program updates the positions of the objects in the universe at each frame of the simulation based on the forces applied to each object.

### Integration

- The program integrates the movement of objects in the universe by calculating the forces between objects and updating the positions of the objects at each frame of the simulation. The program calculates the forces between objects using Newton's law of universal gravitation and the law of conservation of momentum. The program updates the positions of the objects in the universe based on the forces applied to each object.

- The program uses different integration methods to update the positions of the objects in the universe. The program uses the Euler method to update the positions of the objects based on the forces applied to each object. The program uses the Verlet method to update the positions of the objects based on the forces applied to each object. The program uses the Adams-Bashforth method to update the positions of the objects based on the forces applied to each object.

- Implements different integration methods that can be switched to test the performance:
    - Euler method
    - Verlet method
    - Adams-Bashforth method
    - Runge-Kutta method
    - Leapfrog method
    - Stormer-Verlet method

### Forces

- The program calculates the forces between objects using Newton's law of universal gravitation. The program calculates the gravitational force between each pair of objects in the universe and applies the force to each object to determine its acceleration and velocity. The program uses the octree data structure to efficiently calculate the forces between objects and to detect collisions between objects. The program updates the positions of the objects in the universe at each frame of the simulation based on the forces applied to each object.

- The program also calculates the forces between objects using the law of conservation of momentum. The program calculates the momentum of each object in the universe and applies the force to each object to determine its acceleration and velocity. The program uses the octree data structure to efficiently calculate the forces between objects and to detect collisions between objects. The program updates the positions of the objects in the universe at each frame of the simulation based on the forces applied to each object.

### Collision

To efficiently calculate collisions between objects, the program uses an octree data structure to divide the universe into smaller regions. The octree is used to store the objects in the universe and to quickly determine which objects are close to each other and may collide. The octree is updated at each frame of the simulation to reflect the current positions of the objects in the universe. The program uses the octree to calculate the forces between objects and to detect collisions between objects. When two objects collide, the program merges the objects into a new object and updates the octree to reflect the new object's position in the universe.


### Camera
- The camera can be toggled to move between a preset location near the edge of the universe and that of the next object in outer space. If the camera is set to an object, the camera will follow the object throughout its movements in space. Upon collision, the camera will follow the new object as it did with the first.

- The camera can zoom in and out of the universe, allowing the user to view the universe from a closer or further distance.

- The camera can be rotated to view the universe from different angles.

- The octree view can be toggled on and off to view the universe from a different perspective.

- The camera can be paused and resumed to stop and start the simulation.

- The camera can be reset to its initial state to start the simulation from the beginning.

- The camera can be toggled to show the trajectory of the objects in the universe.


### Control

- Camera movements: The user can move the camera using the arrow keys, zoom in and out using the mouse wheel, and rotate the camera using the mouse.
- Pause and resume: The user can pause and resume the simulation using the space bar.
- Reset: The user can reset the simulation using the R key.
- Toggle modes: The user can toggle between different modes using the M key.
- Toggle octree view: The user can toggle the octree view using the O key.
- Toggle trajectory: The user can toggle the trajectory of the objects in the universe using the T key.

### Menu

- The program has a menu that allows the user to interact with the simulation. The menu displays the current state of the simulation, such as the number of objects in the universe, the current time, and the current mode. The menu also displays the controls for the simulation, such as the keys to move the camera, pause and resume the simulation, reset the simulation, and toggle between different modes.

- If an object is selected, the menu will display the object's properties, such as its mass, radius, and velocity. The menu will also display the object's trajectory in the universe.


### Modes

- The program has different modes that allow the user to view the universe from different perspectives. The modes include a free mode, a follow mode, and a preset mode. In the free mode, the user can move the camera freely around the universe. In the following mode, the camera follows a specific object in outer space. In the preset mode, the camera is set to a preset location near the edge of the universe.

### Trajectory

- The program can display the trajectory of the objects in the universe. The trajectory is displayed as a line that shows the path of the object in space. The trajectory is updated at each frame of the simulation to reflect the current position of the object in the universe. The trajectory can be toggled on and off using the T key.

### Octree view

- The program can display the octree view of the universe. The octree view shows the division of the universe into smaller regions using the octree data structure. The octree view is updated at each frame of the simulation to reflect the current positions of the objects in the universe. The octree view can be toggled on and off using the O key.

- This octree view consists of 8 regions, each region is divided into 8 regions, and so on. The octree view shows the division of the universe into smaller regions using the octree data structure. The octree view is updated at each frame of the simulation to reflect the current positions of the objects in the universe.

- When toggled the user can see cubes of different sizes that represent the regions of the octree and the objects that are in each region. The cubes are colored based on the number of objects in each region, with green indicating a high number of objects and blue indicating a low number of objects.

### Interaction

- The user can interact with the simulation by moving the camera, zooming in and out, and rotating the camera to view the universe from different angles. The user can also pause and resume the simulation, reset the simulation to its initial state, and toggle between different modes. The user can also toggle the octree view and the trajectory of the objects in the universe.

### Performance

- The program is optimized for performance by using the octree data structure to efficiently calculate the forces between objects and to detect collisions between objects. The program uses the octree to store the objects in the universe and to quickly determine which objects are close to each other and may collide. The program updates the octree at each frame of the simulation to reflect the current positions of the objects in the universe. The program uses the octree to calculate the forces between objects and to detect collisions between objects. The program updates the positions of the objects in the universe at each frame of the simulation based on the forces applied to each object.

### Parallelism

- The program is parallelized using OpenMP to take advantage of multiple cores in the CPU. The program uses OpenMP to parallelize the calculation of forces between objects and the detection of collisions between objects. The program uses OpenMP to distribute the work across multiple threads to efficiently calculate the forces between objects and to detect collisions between objects. The program updates the positions of the objects in the universe at each frame of the simulation based on the forces applied to each object.

### Console

- The program has a console that displays information about the simulation, such as the number of objects in the universe, the current time, and the current mode. The console also displays error messages to the user when an error occurs and logs errors to a file for debugging purposes.

### Configuration

- The program has a configuration file that allows the user to customize the simulation. The configuration file contains settings for the number of objects in the universe, the size of the universe, the mass of the objects, and the velocity of the objects. The user can modify the configuration file to change the settings of the simulation.

- The configuration file contains settings for the number of objects in the universe, the size of the universe, the mass of the objects, and the velocity of the objects. The user can modify the configuration file to change the settings of the simulation.

- The program can run with or without the configuration file. If the configuration file is present, the program will read the settings from the configuration file. If the configuration file is not present, the program will use default settings for the simulation.

### Graphics

- The program uses OpenGL to render the 3D scene of the universe. The program uses OpenGL to create a window and to draw the objects in the universe. The program uses OpenGL to render the objects in the universe with different colors and sizes. The program uses OpenGL to render the trajectory of the objects in the universe as a line that shows the path of the object in space. The program uses OpenGL to render the octree view of the universe as cubes that represent the regions of the octree and the objects that are in each region.

### Error handling

- The program handles errors by displaying error messages to the user and exiting the program. The program checks for errors when reading input from the user, calculating forces between objects, and detecting collisions between objects. The program displays error messages to the user when an error occurs and exits the program. The program also logs errors to a file for debugging purposes.

### Logging

- The program logs information about the simulation to a file for debugging purposes. The program logs the current state of the simulation, such as the number of objects in the universe, the current time, and the current mode. The program logs information about errors that occur during the simulation, such as errors when reading input from the user, calculating forces between objects, and detecting collisions between objects. The program logs information about the performance of the simulation, such as the time taken to calculate forces between objects and to detect collisions between objects. The program logs information about the graphics of the simulation, such as the frame rate and the number of objects rendered in the scene.

### Testing

- The program is tested using unit tests and integration tests. The program uses unit tests to test individual components of the program, such as the calculation of forces between objects and the detection of collisions between objects. The program uses integration tests to test the interaction between components of the program, such as the movement of objects in the universe and the rendering of the 3D scene. The program uses test cases to test different scenarios, such as the collision of objects with different masses and velocities, the movement of objects in different directions, and the rendering of objects with different colors and sizes.

### Benchmarking

- The program is benchmarked using performance tests to measure the time taken to calculate forces between objects and to detect collisions between objects. The program uses performance tests to measure the time taken to update the positions of the objects in the universe at each frame of the simulation. The program uses performance tests to measure the time taken to render the 3D scene of the universe. The program uses performance tests to measure the time taken to parallelize the calculation of forces between objects and the detection of collisions between objects using OpenMP.

### Documentation

- The program is documented using Doxygen to generate documentation for the code. The program uses Doxygen to document the classes, functions, and variables in the code. The program uses Doxygen to generate HTML documentation that describes the purpose of each class, function, and variable in the code. The program uses Doxygen to generate diagrams that show the relationships between classes, functions, and variables in the code.

### Build

- The program is built using CMake to generate the build files. The program uses CMake to generate the build files for the program. The program uses CMake to compile the source code, link the libraries, and create the executable file.

### Installation

- The program is installed using the make command to compile and install the program. The program uses the make command to compile the source code, link the libraries, and create the executable file. The program uses the make install command to install the program on the system.

### Deployment

- The program is deployed using the make command to compile and deploy the program. The program uses the make command to compile the source code, link the libraries, and create the executable file. The program uses the make deploy command to deploy the program on the system.

### Execution

- The program is executed using the ./universe command to run the program. The program uses the ./universe command to run the program and display the 3D scene of the universe.

### Packaging

- The program is packaged using the make package command to create a package of the program. The program uses the make package command to create a package of the program that contains the executable file, the libraries, and the configuration file. The package is created in a compressed format, such as a tarball or a zip file, that can be distributed to users.


## Folder structure

```bash
universe/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── .gitignore
├── include/
│   ├── camera/
│   ├── graphics/
│   ├── octree/
│   ├── simulation/
│   ├── utils/
│   └── ui/
├── src/
│   ├── main.cpp
│   ├── camera/
│   ├── graphics/
│   ├── octree/
│   ├── simulation/
│   ├── utils/
│   └── ui/
├── tests/
│   ├── camera/
│   ├── graphics/
│   ├── octree/
│   ├── simulation/
│   ├── utils/
│   └── ui/
├── docs/
│   └── (documentation files)
├── data/
│   └── (input data files, if any)
├── build/
│   └── (build artifacts)
├── bin/
│   └── (executable files)
├── dist/
│   └── (packaged files)
└── resources/
    └── (textures, models, etc.)
```
