# Phobia

A first-person horror maze game built with OpenGL 3.3

## Map & Entity System — Design Overview

This section documents the map and entity system used by the project. The world is authored using simple, human-readable text files so teachers and designers can edit maps and behavior without rebuilding code.

### 1. General idea

- The game world is built from two complementary file types:
	- `.cub` — a visual 2D grid that defines layout and places objects.
	- `.json` — a companion file that provides parameters for dynamic entities (enemies, lights, etc.).
- Only one map is loaded at a time. You can keep multiple `.cub` files in the assets folder for selection from a menu.

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
| `E`    | Enemy (zombie)                  |
| `B`    | Enemy (bat)                     |
| `L`    | Light source                    |
| `S`    | Spike trap (lower floor height) |

Notes:
- The map loader reads the grid line-by-line and converts characters to tiles or entity placeholders.
- The order in which dynamic tokens (like `E`, `B`, `L`) appear in the file is important — it is the same ordering expected by the companion `.json` arrays.

### 3. Entities file (`.json`)

Each `.cub` map has a companion JSON file that defines parameters for dynamic elements in the same order they appear in the `.cub`. This keeps layout and tuning separate.

Example — `map1_entities.json`:

```json
{
	"enemies": [
		{ "type": "zombie", "speed": 1.2, "patrol": [[2,3], [2,6], [5,6]] },
		{ "type": "bat",    "speed": 3.0, "patrol": [[3,4], [8,4]] }
	],
	"lights": [
		{ "intensity": 1.0, "color": [255, 220, 180] }
	]
}
```

Notes:
- The `enemies` array aligns with the `E`, `B`, ... tokens in the `.cub` file in reading order. The map loader uses an index per token type when spawning.
- Lights and other dynamic objects are described similarly in arrays (e.g., `lights`).

### 4. How it works in code

The loader (`map.cpp`) performs two reads: the `.cub` layout and the matching `.json` entities file. When it encounters a dynamic token, it pulls the next entry from the relevant JSON array and spawns the object at the grid coordinate.

Pseudo-C++ example:

```cpp
// inside map loader loop
if (cell == 'E') spawnEnemy(jsonEnemies[eIndex++], x, y);
if (cell == 'B') spawnEnemy(jsonEnemies[bIndex++], x, y);
if (cell == 'L') spawnLight(jsonLights[lIndex++], x, y);
```

Each spawn function constructs the correct subclass (for example `Zombie` or `Bat` inheriting from `Enemy`) and initializes it using the JSON parameters.

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


