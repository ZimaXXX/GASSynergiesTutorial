# GAS Synergies Tutorial

![Synergy Showcase](Images/GST_Synergy.gif)

## Overview

GASSynergiesTutorial is a prototype implementation of a weapon and tool system designed for a game where players navigate a hovering skimmer. The system allows for the addition and modification of upgrades that influence environmental interactions, combat mechanics, and gameplay strategies.

## Project Goals

The main goal of this project is to design a system that enables:

- Interaction of tools and weapons with various surface types (e.g., sand, rock, bio-matter).
- Dynamic combat where enemy types and environmental elements affect attack strategies.
- A modular upgrade system that enhances tools and weapons with additional abilities and synergies.
- A developer-friendly architecture that facilitates easy expansion and modification of equipment and upgrades.

## Game Context

The game revolves around piloting a **skimmer**, a hovering vehicle that glides over diverse terrain types. Players engage in combat with various enemy types and interact with the environment using an array of tools and weapons. Success depends on skillful maneuvering, strategic use of equipment, and leveraging environmental advantages.

### Combat Mechanics

- Enemies have different attack behaviors: some charge directly, others shoot projectiles, and some combine melee and ranged attacks.
- Combat involves strategic positioning, using terrain for cover, and exploiting enemy vulnerabilities.
- (Implemented) Some attacks are automatic, while others require manual activation and cooldown management.

## Equipment System

### Tools & Weapons

Each piece of equipment has unique properties and upgrade paths. Some examples include:

#### **Cannon**

- (Implemented) Automatically fires at the nearest target within range.
- (Implemented) Fires only to the left or right at fixed intervals.
- **Upgrades:**
  - Increased fire rate.
  - Enhanced damage against specific materials (e.g., crystalline enemies).
  - Homing projectiles.

#### **Burrowing Claws**

- (Implemented) Enables skimmer to dig into sand to avoid surface threats.
- **Upgrades:**
  - Increased burrow duration.
  - Reduced cooldown.
  - Ability to attack from underground.

#### **Harpoon**

- (Implemented) Used for grabbing valuable objects during exploration.
- (Implemented) In combat, it can anchor the skimmer to the environment or enemies.
- **Upgrades:**
  - Ability to anchor onto sand-based enemies.
  - (Implemented) Reinforced tip for gripping crystal surfaces.
  - (Implemented) Increased range and launch force.

#### **Orb Lightning**

- (Implemented) Functions as both a weapon and a shield.
- An electric discharge orbits the skimmer, damaging enemies and deflecting projectiles.
- **Upgrades:**
  - (Implemented) Additional lightning orbs.
  - Increased damage output.

#### **Explosive Barrel**

- (Implemented) Can be dropped behind the skimmer.
- (Implemented) Explodes on impact with a hard surface or when hit by an attack.
- **Upgrades:**
  - Timed detonation.
  - Increased number of active barrels.
  - Larger explosion radius.

### Upgrade Synergies

Upgrades interact dynamically, creating additional bonuses when combined:

- **(Implemented) Harpoon + Reinforced Tip + Increased Launch Force** → Extra damage when anchoring to an enemy.
- **Burrowing Claws + Crystal Upgrade** → Ability to dig into rock surfaces.
- **Explosive Barrel + Bio Upgrade** → Barrels stick to surfaces before detonating.

## Technical Implementation

The system is built using **Unreal Engine's Gameplay Ability System (GAS)** to facilitate modularity and scalability. Key architectural elements include:

- **(Implemented) Base Equipment Classes** – Abstract base classes for tools and weapons.
- **(Implemented) Upgrade Components** – Modular upgrade system that allows stacking effects and synergies.
- **(Implemented) Gameplay Effects & Abilities** – Encapsulate active and passive behaviors of equipment.
- **(Implemented) Blueprint Interfaces** – Expose properties and functions for designers to extend equipment.

## Developer Workflow

To ensure flexibility for designers, the system supports:

1. **(Implemented) Modular Equipment Definitions** – New tools and weapons can be added through blueprints and data tables.
2. **(Implemented) Dynamic Upgrade Application** – Upgrades can be defined and modified without code changes.
3. **(Implemented) Synergy Rules** – Defined via data-driven systems, allowing new synergies to be created easily.

[![Features Preview](https://img.youtube.com/vi/eqNZ7IJg_8A/0.jpg)](https://www.youtube.com/watch?v=eqNZ7IJg_8A)

## Getting Started

### Prerequisites

- **Unreal Engine 5+**
- **GAS Plugin Enabled**

### Installation

1. Clone the repository:
   ```sh
   git clone https://github.com/ZimaXXX/GASSynergiesTutorial.git
   ```
2. Open the project in Unreal Engine.
3. Ensure **Gameplay Ability System (GAS)** is enabled.
4. Compile and run the project.

### Contribution

We welcome contributions! If you’d like to add a feature or fix a bug:

1. Fork the repository.
2. Create a feature branch.
3. Submit a pull request with a detailed description of changes.

## License

This project is licensed under the MIT License.

## Contact

For any questions or discussions, please open an issue or reach out via GitHub discussions.

---

This project serves as a foundation for designing modular equipment and upgrade systems in Unreal Engine. Developers are encouraged to expand on its functionality and adapt it to their own game projects.
