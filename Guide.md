**Diggr** is a roguelike game with a very different set of game mechanics and rules when compared to the typical AD&D-derived RPG.

This document is a brief, non-spoilery explanation of how the Diggr world works.


## 1. ##
Diggr does not have 'hitpoints'. Instead of hitpoints, the PC has five counters:

  * Health
  * Warmth
  * Fatigue
  * Sleep
  * Thirst
  * Hunger

Each of these counters can take a floating-point value anywhere from +3.0 to -3.0.
On the game screen this is represented with a corresponding number of 'plus' or 'minus' symbols.

When the player's 'health' counter reaches -3.0, you die and the game ends.

Reaching -3.0 on any other of the four remaining counters is non-lethal, but results in nasty effects.


## 2. ##
Diggr does not have a player 'inventory' in the traditional sense.

The player character has 7 so-called 'equipment slots', and 2 'backpack slots'.

Each item in the game can have a 'slot marker', corresponding to one of the 'equipment slots'.

An item can be carried only in a slot corresponding to its 'slot marker', or in one of the 'backpack slots'.
('Backpack slots' can hold any item.)

If an item has no 'slot marker', then it can only be carried in a backpack slot.

Many items only have an effect when carried in their proper slot.

Coincidentally, that means that the player can carry at most 9 items at a time.


## 3. ##
Diggr has nothing like the traditional 'experience points'.

A player character's level is always equal to the highest level among the monsters he killed.

Coincidentally, that means that killing a monster with a level equal to or lower than your own
is almost always useless.


## 4. ##
Diggr's combat system is heavily biased towards level.

What that means, for example, is that a level 2 player, on average, does twice as much damage as a
level 1 player when using the same weapon.
A level 4 player does twice as much damage, on average, as a level 2 player, etc.

The same mechanic works for monsters.


## 5. ##
In combat, each player or monster has two floating-point numbers: the 'attack' score and the
'defence' score.

These numbers depend on the equipment carried, and are not made visible during the game in any way.

Thus, when the player attacks a monster, combat depends on the four numbers
(player level, player attack, monster level, monster defence).

Correspondingly, when a monster attacks a player, it's
(monster level, monster attack, player level, player defence).


## 6. ##
Diggr has dungeons that only go down. That is, you can descend to a lower dungeon level, but
you cannot go back once you do so.

Also, the monsters on a dungeon level are, most of the time, of a corresponding level.
(That is, dungeon level 2 mostly has level 2 monsters, dungeon level 3 has mostly level 3 monsters, etc.)


## 7. ##
Diggr has nothing corresponding to traditional 'attributes'.
That is, all player characters have the same strength, dexterity, etc.


## 8. ##
Diggr has nothing like player 'race' or 'class'.
All players begin the game with the same set of starting equipment and the same stats.