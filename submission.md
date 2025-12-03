## Project 6: Final Project Gear Up

The project handout can be found [here](https://cs1230.graphics/projects/final/gear-up).

### Test Cases

|                     What am I testing                     |                                                  Expected Output                                                   |                                              Your Output                                               |
| :-------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------: |
|                        Basic force                        |                                     Objects accelerate under a constant force                                      |  https://github.com/user-attachments/assets/4d4491f6-789f-4c75-acfb-3f98025ababf      |
|       Constant force on objects of different masses       |                        Lighter object accelerates faster; heavier object accelerates slower                        |  https://github.com/user-attachments/assets/8254044b-d521-4dcc-bfff-242830b54f51 |
|                 Motion at constant speed                  |                        Given an initial impulse, an object moves at constant speed forever                         | https://github.com/user-attachments/assets/fe5ab125-f92d-41b2-ba05-510b908d1878 |
|                 Torque around the X axis                  |                            Object rotates around X axis only, and angularly accelerates                            |       https://github.com/user-attachments/assets/9162103b-85db-4747-967e-be3657e62ce1 |
|                 Torque around the Y axis                  |                            Object rotates around Y axis only, and angularly accelerates                            |      https://github.com/user-attachments/assets/366bb41d-a2a7-4654-bf24-f1f98b74c7ac|
|             Motion at constant angular speed              |                   Given an initial angular impulse, an object rotates at constant speed forever                    |  https://github.com/user-attachments/assets/9c6fb5eb-a208-431a-bf2b-15c00f047ff6|
|        Cube rotation with non-axis-aligned torque         |     Cube rotates smoothly because it's rotationally symmetrical around any axis that passes through the center     |    https://github.com/user-attachments/assets/6fa92c34-141b-4cf1-bea0-777b125e5b9a|
|        Wobbly cone with slightly misaligned torque        | Cone rotates primarily in a circle around its vertical axis, but wobbles due to misaligned impulse = (0, 0.5, 0.1) |     https://github.com/user-attachments/assets/5a1e3d8c-9530-4cc2-92c3-b2f7fa7f61f1|
|             Combine linear and angular motion             |               After applying an initial impulse linearly and angularly, object rotates while moving                |  https://github.com/user-attachments/assets/dd30a08f-e1f7-4038-8a0e-8a9d099d2c5f|
|                 Equal and opposite forces                 |                                              Object should be static                                               |     https://github.com/user-attachments/assets/fcb18177-2b33-472b-a432-dba30c8a2df8|
|                Equal and opposite torques                 |                                              Object should be static                                               |    https://github.com/user-attachments/assets/922b2a5a-5e0f-43e2-b12f-c0fc7ed6309d|

### Design Choices

The way I chose to integrate my physics equations was lifted from what I learned in 2D Game Engines: the symplectic Euler method takes force/torque as updates to momentum, and uses momentum as a source of base truth for everything else. From momentum we can get velocity and angular velocity, which we then use to step forward our position and orientation as well.

### Collaboration/References

No Collaborators, but some references:

https://www.cs.cmu.edu/~baraff/sigcourse/notesd1.pdf - All the rigid body physics baseline knowledge.
[2D Game Engines Slides](https://docs.google.com/presentation/d/1iy-68UyvcTXwMwwg1xu5RUM2jxpYVOHonDNyyCwKG9M/edit?slide=id.g53862d0377_2_671#slide=id.g53862d0377_2_671) - Symplectic Euler stuff w/ pseudocode
https://resources.wolframcloud.com/FormulaRepository/resources/Moment-of-Inertia-of-a-Cone - Formula for moment of inertia of a cone
https://resources.wolframcloud.com/FormulaRepository/resources/Moment-of-Inertia-of-a-Cylinder - Above but for cylinder
https://resources.wolframcloud.com/FormulaRepository/resources/Moment-of-Inertia-of-a-Cuboid - For cube
https://resources.wolframcloud.com/FormulaRepository/resources/Moment-of-Inertia-of-a-Sphere - Sphere

### Known Bugs

N/A

### Extra Credit

N/A
