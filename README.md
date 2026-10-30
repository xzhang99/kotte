# Bomberman World — [Edgerunner(Enemey)]

**Student:** [Xuyang Zhang]

> Replace the prompts below and remove these instructions. Keep answers brief. Your implementation explanations and reflection belong in the presentation; you do not need to write them out here. Complete the final section only if seeking Pass with Distinction (VG).

## How to play

- **Controls:** Move with `WASD` or Arrow keys. Press `Space` to place a bomb. Walk into a solid bomb (after stepping off it) to kick it along a cardinal direction. Press `R` to restart the current round, `1` for the default gameplay configuration (`15 x 11`), `2` for the stress configuration (`200 x 120`), and `Q` or `Escape` to quit.

- **Objective and rules:** Defeat all enemies in the room to reach the `WON` state. Contacting an enemy, a standard cross-shaped bomb blast, or a Kamikaze enemy's explosion triggers player death and transitions the round to `LOST`.   

 - **Priority rule:** Player death takes priority over objective completion if both happen in the same frame.
 - **Extension #6 (Multiple bombs per owner):** The player can have max 3 active bombs in the world at the same time; capacity restores as each bomb detonates.
 - **Extension #9 (New enemy behaviours):** Enemies spawn in three colors: pink **Patrol** enemies march and turn randomly(in direction) when blocked; purple **Pursue** enemies re-evaluate their heading every `0.25s` to steer toward the player; red **Kamikaze** enemies patrol until the player comes within 160 px^2, then charge at the player with a visible 2s countdown on their heads before detonating in a 5 x 5 tile square blast.
 - **Extension #12 (Kickable bombs):** Once the player has stepped clear of a placed bomb (`owner_overlapping == false`), walking into its collision box kicks it in the movement direction until it hits a wall, crate, bomb, or enemy.

- **Configurations / demo setup:**Default launches the 15 x 11 room (deterministic seed) with a guaranteed green blast-range pickup at tile (3, 2) right next to the player spawn at (2, 2). Press 2 at any time to switch to the 200 x 120 room, or 1 to return to the default gameplay room.



## Features and validation

(delete this pre-amble before submitting the assignment) Complete the five required features and three extensions of your choice. For each row, document **at least one useful check: what you tested, what you observed, and how the result compares with the expected behaviour**. One or two sentences per feature are enough. Choose a check that could reveal a mistake, such as a boundary case or interaction.

Use the practical checks in the assignment to validate the acceptance conditions during development; this table is a small sample of that work. Manual play in a small, repeatable setup is sufficient. You can also use a debugger, temporary logging, or small scenarios set up in code. Screenshots, recordings, automated tests etc are welcome, but not required. Be ready to explain the relevant rules in your code.

| Feature | Check, observed result, and expected behaviour |
| --- | --- |
| 1. Player damage and death | I walked just past the top visual edge of a pink enemy and stood diagonally adjacent to a detonating bomb to test broad-phase candidates vs. narrow-phase hits, then walked directly into an enemy's foot collision box and into a blast ray. Near-misses did not trigger death, whereas actual `entity_collision_bounds` contact destroyed the player once and switched the HUD to `LOST` as expected. |
| 2. Round states and objective | I lured the final enemy next to a bomb and stood inside the same blast radius when the fuse expired to test simultaneous win/loss resolution. The round transitioned to `LOST` rather than `WON`, so the player death is prior than the objective completion, and movement/bomb inputs were locked afterward. |
| 3. Safe restart | I placed 3 live bombs, triggered a Kamikaze enemy's charging countdown, picked up the range upgrade, and pressed `R` three times in rapid succession while blasts were active. All old entities, spatial grid entries, and pending request queues cleared cleanly; no ghost explosions occurred |
| 4. Solid bombs with owner escape | I placed a bomb at my feet, wiggled around while overlapping its collision box, stepped completely off the tile, and tried walking back onto it. Movement was allowed while `owner_overlapping` was true, blocked as soon as I stepped clear and tried to re-enter, and enemies were blocked by the bomb at all times. |
| 5. Blast-range pickup | I placed a bomb at default range 3, immediately stepped onto the green pickup at (3, 2) before the first bomb detonated, and then placed a second bomb. The pickup disappeared on contact and incremented the HUD range to 4/6 (walking over (3, 2) again had no effect); the first bomb still detonated with a 3-tile ray while the second bomb used the new 4-tile range. |
| #6 Multiple bombs per owner | I just smashed `Space` four times on the same tile and then across four adjacent floor tiles in a row. Same-tile placement was rejected after the first bomb, and across adjacent tiles the 4th press was ignored once active bombs reached 3/3 until the earliest bomb detonated and freed a slot. |
| #9 A second enemy behaviour | I positioned a purple Pursue enemy behind a crate and approached a red Kamikaze enemy within 160 px^2 before ducking behind a wall. The Pursue enemy turned along the crate instead of jittering every frame because its `0.25s` `decision_timer` preserved blocked turns, and the Kamikaze enemy displayed its yellow countdown timer and cleared nearby crates/enemies with a 5 x 5 square blast when the timer hit 0 |
| #12 Kickable bombs | I placed a bomb, stepped off it, and walked into it toward a wall and toward an oncoming enemy while its fuse was ticking. The bomb slid along the corridor while updating its `SpatialGrid` cells, stopped upon hitting the obstacle or enemy, detonated at its new tile, and left its original tile passable. |


**Known issues:** Kicked bombs halt at their exact continuous contact coordinate rather than snapping to `tile_centre()`. As analysed in the architectural investigation below, this is an intentional trade-off to avoid teleporting stopped bombs into crates or back onto a closely following player; blast rays still resolve from the halted tile's grid coordinates (`std::floor(pos / tile_size_)`).

## Sources and assistance

This project builds on the shared Kotte foundations.

**AI assistance and verification:**
- **Bridging starter branches to the Week 6 reference baseline:** Earlier in the course, I worked on my own branches starting from `2026-week-03-starter`, `2026-week-04-starter` and `2026-week-05-starter`. Before writing code for the final assignment, I used AI to compare my earlier workshop implementations and study notes against the `2026-week-06-reference` baseline. This helped me quickly map out what had changed between Week 2/3/4/5 and Week 6—specifically how raw vector indices were replaced by generational `EntityHandle` lookups in `EntityStore`, how `SpatialGrid` broad-phase queries were integrated into movement, and how `run_frame()` was structured into the 8-phase pipeline with deferred mutations in Phase 6. I verified this myself by stepping through `kotte.tests` and tracing `run_frame()` in the Visual Studio debugger.

- **Debugging Extension #12 (Kickable bombs):** I also used AI to help diagnose a collision bug when implementing kickable bombs. In my initial attempt, kicked bombs slid straight into enemies and trapped them, and my first fix—snapping halted bombs to `tile_centre()`—caused bombs to teleport backward into the player. AI helped me trace how `entity_movement_is_blocked()` filters out non-solid entities (`is_solid(enemy) == false`) and why axis-separated movement probes in `try_move_player()` could overwrite slide directions during diagonal input. I tested and verified the final fix myself by adding the narrow-phase enemy contact check in `update_bombs()`, locking `is_moving` per kick, and removing the forced grid snap.

Using AI as a "diff and architecture explainer" between my Week 3/4/5 code and the Week 6 reference tag saved me a lot of time—it helped me see *why* the generational handle and 8-phase pipeline existed instead of fighting against them. I think it is much easier for me if I could start week 6 from the code I wrote from previous weeks.

## Architectural investigation — VG only

### Decision

My engineering question is: **When the player kicks a bomb (Extension #12), how should it check for collisions and where should it stop when it hits a moving enemy (Extension #9) or a crate, without breaking the "solid bomb with owner escape" rule (Required Feature #4)?**

In my current code (`Application::try_move_player()` and `Application::update_bombs()` in `application.cpp`), walking into a bomb sets `bomb_state->is_moving = true` and slides it at `220 px/s` during Phase 3 (`update_gameplay`). To know when the bomb should stop, I check `entity_movement_is_blocked(bomb_handle, proposed_position)` and also run a `SpatialGrid` check specifically for `EntityKind::enemy`. When the bomb hits something, it sets `is_moving = false` and stops *right where it is*, without snapping to the centre of the tile.

I compared this with two other approaches that I actually tried while coding:
1. **Only using `entity_movement_is_blocked()`:** Letting Kotte's default movement function handle sliding bombs without any special check for enemies.
2. **Snapping to `tile_centre()` when stopped:** Calling `bomb->world_position = tile_centre(bomb->world_position)` as soon as the bomb hits an obstacle so it always sits neatly in the middle of a grid tile.

### Method

I tested these options in the default `15 x 11` room (`seed: 0x4b4f545445`) by playing through three specific situations:
1. **Kicking a bomb into an enemy:** I placed a bomb, walked off it so `owner_overlapping` became `false`, and kicked it down a hallway into a purple Pursue enemy (`EnemyBehaviour::pursue`).
2. **Walking right behind a kicked bomb:** I placed a bomb one tile away from a crate/wall, stepped back, and held down the move key (`D` or `W`) so my player kept walking right behind the bomb as it hit the obstacle.
3. **Pressing two keys at once (diagonal input):** I walked into a solid bomb while holding `W + D` to see how the separate X and Y movement steps in `update_player()` affected the kick direction.

### Evidence

Here is what actually happened in the game during those three tests:

| What I tested | Alternative 1: Only `entity_movement_is_blocked()` | Alternative 2: Enemy check + `tile_centre()` snap | Current approach: Enemy check + stop in place |
| --- | --- | --- | --- |
| **1. Kicking a bomb into an enemy** | The bomb slid straight into the enemy's body. On the next frame, the enemy got stuck inside the bomb and spun in place. | The bomb stopped in front of the enemy and didn't trap it. | The bomb stopped in front of the enemy, and the enemy turned away on its next move. |
| **2. Walking right behind a kicked bomb** | The bomb stopped at the wall, and the player stopped behind it. | Snapping to `tile_centre()` pulled the bomb backward onto the player *after* `owner_overlapping` was already `false`, freezing the player in place. | The bomb stopped at the wall, and the player stayed outside the bomb and could still walk around freely. |
| **3. Kicking a bomb upward into a crate** | The bomb stopped at the bottom edge of the crate. | The bomb's position point had just crossed into the crate's tile, so `tile_centre()` teleported the bomb *inside* the crate. | The bomb stopped right at the bottom edge of the crate. |

### Interpretation

All three bugs come down to how Kotte handles solid objects, bounding boxes, and player movement:
- **Bombs are solid, but enemies aren't:** For Required Feature #1 (player dies when touching an enemy), `Application::is_solid()` returns `false` for `EntityKind::enemy` so the player and enemy collision boxes can touch. At the same time, `is_solid()` returns `true` for `EntityKind::bomb`. When a sliding bomb calls `entity_movement_is_blocked()`, that function skips anything where `is_solid()` is `false`. So the bomb ignores enemies and slides right into them. Once they overlap, the enemy tries to move in `update_enemies()`, sees a solid bomb that it doesn't own, and gets blocked in all four directions. Adding a separate enemy check in `update_bombs()` fixes this without making enemies solid to the player.
- **Why `tile_centre()` teleports the bomb into crates or the player:** In `entity_collision_bounds()`, every entity's `world_position` is its **bottom-centre point**, and its collision box extends upward from there (`top_left = world_position - {size.x / 2, size.y}`). Also, crates (`32 x 32`) and bombs (`18 x 12`) are smaller than a full `40 x 40` tile. Because a kicked bomb moves pixel by pixel (`bomb_slide_speed_ * delta_time`), it hits walls or crates before it reaches the middle of a tile—or when sliding up, its bottom point can cross into the crate's tile just before the collision boxes touch. Calling `tile_centre()` forces the bomb to jump forward into the crate or backward into the player walking behind it. Since `owner_overlapping` was already set to `false` before the kick, jumping backward onto the player makes `entity_movement_is_blocked(player_handle_, ...)` return `true` forever, locking the player in place.
- **Holding `W + D` overwrites the kick direction:** In `update_player()`, the game tests X movement first (`try_move_player({dx, 0})`) and Y movement second (`try_move_player({0, dy})`). If I hold `W + D` while touching a bomb, the X step starts the bomb moving, and then the Y step immediately overwrites `move_direction`. Adding `!candidate->bomb_state->is_moving` to the check makes sure a bomb that is already sliding keeps its original direction.

### Conclusion

Letting the bomb stop at its last valid position (along with the enemy check and direction lock) is the most reliable choice for Kotte. It keeps the game **correct** (neither enemies nor the player get stuck inside bombs) and **easy to maintain** (I didn't have to hack special rules into `is_solid()`). Plus, when the bomb explodes in Phase 5 (`resolve_gameplay_facts()`), `std::floor(fact.world_position / tile_size_)` still finds the right grid row and column for the blast rays anyway.


<!-- Submission: commit and push this README with your source to GitHub or the internal Gitea server. Submit the final commit link and a runnable Release ZIP from that version in Studium. Ensure the teacher can access the repository throughout assessment. -->
