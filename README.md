# Phobia

A first-person horror maze game built with OpenGL 3.3

## Map & Entity System — Design Overview

This section documents the map and entity system used by the project. The world is authored using simple, human-readable text files so teachers and designers can edit maps and behavior without rebuilding code.

### 1. General idea

- The game world is built from two complementary file types:
	- `.cub` — a visual 2D grid that defines layout and places objects.
	- `.json` — A companion file that provides stat definitions for the types of entities found in the map (e.g., how fast a "Zombie" is, what color a "Lamp" is).
- Only one map is loaded at a time

### 2. Map file (`.cub`)

A `.cub` file is a human-editable text grid where each character represents a tile or placed object. Example:

```
111111111111
1N00000000L1
10000E000001
10000B000001
1S0000000001
111111111111
```

Legend:

| Symbol | Meaning                         |
| ------ | ------------------------------- |
| `1`    | Wall block                      |
| `0`    | Floor (walkable)                |
| `N`    | Player spawn (facing North)     |
| `Z`    | Enemy (zombie)                  |
| `B`    | Enemy (bat)                     |
| `L`    | Light source                    |
| `S`    | Spike trap (lower floor height) |

Notes:
- The map loader reads the grid line-by-line and converts characters to tiles or entity placeholders.
- The order in which dynamic tokens (like `E`, `B`, `L`) appear in the file is important — it is the same ordering expected by the companion `.json` arrays.

### 3. Entities file (`.json`)

Unlike the previous iteration, this JSON file does not list every single enemy instance. Instead, it defines the Archetypes (Types). When the map loader finds a 'Z', it looks up the "zombie" config in this file to know how fast it should be or what texture to load.

Example — `map1_config.json`:

```json
{
    "definitions": {
        "zombie": {
            "map_token": "Z",
            "mesh": "zombie_parts.obj",
            "speed": 1.5,
            "hp": 100,
            "behavior": "chase"
        },
        "bat": {
            "map_token": "B",
            "mesh": "bat.obj",
            "speed": 3.5,
            "hp": 20,
            "behavior": "random_patrol"
        },
        "ceiling_lamp": {
            "map_token": "L",
            "color": [0.9, 0.8, 0.6],
            "intensity": 0.8,
            "flicker": true
        }
    }
}
```

### 4. How it works in code

The loader (`map.cpp`) reads the `.cub` file character by character.
It loads the JSON config into a map/dictionary in memory.When it hits a token (e.g., 'Z'):
	It looks up the definition associated with 'Z'.
	It instantiates a new Enemy object at that $(x, y)$ coordinate.
	It applies the stats (Speed: 1.5) from the JSON to that specific instance.
This allows us to place 50 Zombies in the map file without writing 50 entries in the JSON file.

5. Articulated System ProposalsTo satisfy the course requirement for a "System composed of various articulated objects"3, the project will implement one of the following hierarchical models. These objects rely on parent-child matrix transformations (scene graph), not just loading a single mesh.Option A: The "Watcher" (Mechanical Ceiling Trap)A security camera/turret attached to the ceiling that tracks the player.Hierarchy: Base (Static) $\rightarrow$ Swivel Joint (Rotate Y) $\rightarrow$ Arm (Rotate X) $\rightarrow$ Camera Head.Movement: Automatic. The code calculates the vector to the player and updates the joint rotation matrices to keep the head pointed at the target.Horror Element: It emits a red spotlight. If the light touches the player, an alarm triggers.Option B: The Clockwork Spider (Enemy)A mechanical enemy built from primitive shapes or separated mesh parts.Hierarchy: Thorax (Root) $\rightarrow$ Hip Joint $\rightarrow$ Upper Leg $\rightarrow$ Knee Joint $\rightarrow$ Lower Leg.Movement: Procedural animation. As the enemy moves across the floor, the legs cycle through sine-wave rotations to simulate walking.Horror Element: fast, skittering movement sounds.Option C: The "Iron Maiden" Gate (Interactive Object)A heavy door mechanism that requires gears to open.Hierarchy: Door Frame $\rightarrow$ Gear A $\rightarrow$ Gear B $\rightarrow$ Locking Bars.Movement: Interactive. When the player holds a lever, Gear A rotates, driving Gear B (inverse rotation), which translates the Locking Bars to unlock the path.(Current Plan: Implement Option A as it doubles as a lighting mechanic, satisfying multiple requirements).
### 5. Project folder structure


Example layout inside the repository:

```
project/
 ├── assets/
 │    ├── maps/
 │    │    ├── map1.cub
 │    │    ├── map1_entities.json
 │    │    └── map2.cub
 │    └── textures/
 │         ├── walls/
 │         ├── enemies/
 │         └── lights/
 ├── src/
 │    ├── core/
 │    │    ├── map.cpp
 │    │    ├── entity.cpp
 │    │    ├── enemy.cpp
 │    │    ├── light.cpp
 │    │    ├── player.cpp
 │    │    └── spike.cpp
 │    ├── ai/
 │    │    └── patrol.cpp     ← handles path following & vision checks
 │    └── main.cpp
 └── README.md
```

### 6. AI & dynamic behavior

- Enemies follow patrol paths defined in JSON (`patrol` arrays of grid coordinates).
- On detecting the player (line-of-sight / vision checks handled by `ai/patrol.cpp`) an enemy can switch to chase behavior.
- Different enemy types subclass a base `Enemy` class and override movement, attack, and detection specifics.
- Lights can be parameterized to flicker or vary intensity using JSON fields (for example `intensity`, `flickerRate`).

### Summary

| Component | Format | Purpose |
| --------- | ------ | ------- |
| `map1.cub` | Text grid | Defines map layout visually |
| `map1_entities.json` | JSON | Defines parameters (enemies, lights, etc.) |
| `map.cpp` | C++ | Loads both files and spawns objects |
| `enemy.cpp`, `light.cpp` | C++ | Logic for each object type |
| `ai/patrol.cpp` | C++ | Handles enemy movement & detection |

If you want, I can add a small sample `.cub` and `.json` pair to `assets/maps/` as a working example.


