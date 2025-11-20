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

# 5. Articulated System Proposals

To satisfy the course requirement for a "System composed of various articulated objects", the project will implement one of the following hierarchical models. These objects rely on parent–child matrix transformations (a scene graph), not just loading a single mesh.

Current plan: **Implement Option A** — it doubles as a lighting mechanic and satisfies multiple requirements.

---

## Option A — The "Watcher" (Mechanical Ceiling Trap)

- **Overview:** A ceiling-mounted security camera / turret that tracks the player.
- **Hierarchy:** Base (static) → Swivel Joint (rotate Y) → Arm (rotate X) → Camera Head
- **Movement:** Automatic tracking. The code computes the vector to the player and updates the joint rotation matrices each frame so the head remains pointed at the target.
- **Horror element:** Emits a red spotlight. If the spotlight touches the player, an alarm triggers (and potentially other consequences).
- **Notes:** Good fit for demonstrating scene-graph transforms and shader-driven lighting effects (spotlight cone, falloff, and detection).

---

## Option B — The Clockwork Spider (Enemy)

- **Overview:** A mechanical spider enemy assembled from primitives or separate mesh parts.
- **Hierarchy:** Thorax (root) → Hip Joint → Upper Leg → Knee Joint → Lower Leg
- **Movement:** Procedural animation driven by cyclic functions (e.g., sine waves) to rotate leg joints and simulate walking.
- **Horror element:** Fast, skittering movement and unsettling mechanical sounds.
- **Notes:** Demonstrates hierarchical kinematics and procedural animation; adds emergent locomotion behavior.

---

## Option C — The "Iron Maiden" Gate (Interactive Object)

- **Overview:** A heavy door / gating mechanism that uses gears to open and close.
- **Hierarchy:** Door Frame → Gear A → Gear B → Locking Bars
- **Movement:** Interactive: when the player holds a lever, Gear A rotates, driving Gear B (inverse rotation), which translates the locking bars to unlock the passage.
- **Horror element:** Slow, suspenseful mechanical action — can be used to gate player progress or trigger scares when opening.
- **Notes:** Demonstrates parent-child rotations with mechanical coupling and player-driven interactions.

---

### Implementation considerations (common to all options)

- Use a scene graph to propagate transforms from parent to child (local → world matrices).
- Expose joint limits and smooth interpolation (e.g., slerp / damped rotation) to avoid instant snapping.
- Provide authoring-friendly parameters (rotation limits, speeds, sound triggers) in JSON so designers can tweak behavior without rebuilding.
- Option A is prioritized because it also ties into the lighting system (spotlight detection + alarm), covering multiple course objectives.
```


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


