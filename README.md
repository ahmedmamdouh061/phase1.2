# Car Service Center: Phase 1.2

CMPS103 Data Structures and Algorithms, Fall 2026, Team T14

Phase 1.2: implement and test all the data structures of the project, plus the
**random simulator** that moves cars through every list.

## How to run

**Visual Studio:** open `CarServiceCenter.sln` and press **Ctrl+F5**.
If VS asks to "Retarget Projects", click **OK**.

**Other compilers:** `g++ -std=c++17 *.cpp -o CarServiceCenter`, or use `CMakeLists.txt`.

The program runs in **interactive mode** (the only mode required in Phase 1): it prints all
lists each timestep and waits for the user ("PRESS N TO MOVE TO NEXT STEP, OR E TO GO TO THE END !").
A run ends by itself when all cars are in FNSH or CNCL, after about **430–550 timesteps**.

- **N** → next timestep (step by step). Any other key is ignored.
- **E** → go to the end: the remaining timesteps run without stopping, then the last
  timestep's lists and the result are printed

## Phase 1.2 only

This code contains only what Phase 1.2 asks for. None of the Phase 2 logic is included:
no input/output files, no silent mode, no real assignment rules (ERLY/LATE/FS logic),
no rescheduling, no statistics calculations. The data members for those values (Car TW/TS,
Bay busy time, Center Pcancel/Presc) are declared because Phase 1.2 asks for the
"Full data members of classes".

The list operations `IDLE_List::InsertCar` and `ERLY_List::RemoveRandomCar` are kept because
Phase 1.2 requires "Full implementation for all data structures" and "all the lists of the project
should be fully operating in phase 1". They are list operations from our Phase 1.1 report, not
Phase 2 logic.

## Project rules followed

- Every data structure is the **lab code** (`DS/`) or a class **derived** from it (`Lists/`).
- **No STL, no global variables, no `friend`.** Constants are enum values in `Defs.h`.
- **Lists of pointers:** each `Car` and `Bay` is allocated once. Moving a car between
  lists moves the pointer only.
- No "busy bays" list: each car in DIAG/SERV points to its bay (`Car::assignedBay`).

## Files

| Folder / file | What it is |
|---|---|
| `DS/` | Lab 3 files: `Node`, `QueueADT`, `LinkedQueue`, `priNode`, `priQueue`, `StackADT`, `ArrayStack` |
| `Lists/IDLE_List.h` | Queue + `InsertCar()` (sorted by VT, for rescheduled cars) |
| `Lists/ERLY_List.h` | PriQueue by PT (priority = -PT) + `RemoveRandomCar()` (rescheduling) |
| `Lists/LATE_List.h` | Queue + `CancelRandomCar()` (cancellation) |
| `Lists/WaitLists.h` | `WAIT_MU`, `WAIT_EU`, `WAIT_WU`: queues + running `GetTotalServiceTime()` (for FS cars) |
| `Lists/RequiredServices.h` | Car's service list: queue of `[type, duration]` + `RemoveByType()` (for FS cars) |
| `Car.h/.cpp` | Car data members, `Print()`, `operator<<(Car*)` (prints based on the car's status) |
| `Bay.h/.cpp` | `Bay` base + `DiagBay`, `MechBay`, `ElecBay`, `WashBay` (capacity), `operator<<(Bay*)` |
| `Center.h/.cpp` | Owns all cars, bays and lists. Runs the random simulator |
| `UI.h/.cpp` | All console input/output: the timestep screen and the N / E keys |
| `Defs.h` | Enums (`CarStatus`, `UnitType`, `SequenceType`) and the Phase 1 simulator constants |

## Changes to the lab code

The project document asks to "add Print and count to ALL DS". Lab-code changes are marked `[Project]`:

- `LinkedQueue`, `priQueue`, `ArrayStack`: added `count` / `getCount()` and `print(separator, maxItems)`.
- `ArrayStack`: the size is now a constructor parameter. The lab's fixed 100 is too small for 1000+ cars.
- `Node.h`: moved `#endif` to the end of the file (it was before the function definitions).
- `priQueue`, `ArrayStack`: copying disabled (they own their memory).

## Lists in `Center` (from the Phase 1.1 report)

| List | Type | Main ops |
|---|---|---|
| IDLE | `IDLE_List` (Queue) | enqueue/dequeue O(1), InsertCar O(n) |
| ERLY | `ERLY_List` (PriQueue by PT) | enqueue O(n), dequeue O(1), RemoveRandomCar O(n) |
| LATE | `LATE_List` (Queue) | enqueue/dequeue O(1), CancelRandomCar O(n) |
| REDY | `LinkedQueue<Car*>` | O(1) |
| Free diagnosis bays | `LinkedQueue<DiagBay*>` | O(1) |
| DIAG | `priQueue<Car*>` by diagnosis end time | enqueue O(n), dequeue O(1) |
| WAIT_MU / EU / WU | `WAIT_MU`, `WAIT_EU`, `WAIT_WU` | O(1) incl. total service time |
| Free MB / EB | `LinkedQueue<MechBay*>`, `LinkedQueue<ElecBay*>` | O(1) |
| Free WB | `LinkedQueue<WashBay*>`: a bay stays while it has capacity, re-enters when a car leaves | O(1) |
| SERV | `priQueue<Car*>` by service end time | enqueue O(n), dequeue O(1) |
| CNCL / FNSH | `ArrayStack<Car*>`: top = latest time, so printing is already descending | O(1) |

## Random simulator (project document, "Random Simulation Function Details")

`Center::SimulateRandomTimestep()` runs steps 2.1–2.10 in order; each step is its own
function (`Step_...`). Step 2.11 is `Center::PrintInterface()`. The run ends when all
cars are in FNSH or CNCL.

Choices we made where the document leaves it open:

- **1000 cars** generated sorted by VT, PT within ±5 of VT, DD 1–5.
- **Bays:** 10 diagnosis, 15 mechanical, 8 electrical, 3 wash (capacity 3). These are in `Defs.h`.
- **Random WAIT list** uses the same odds as the bays in step 2.8 (MU 60%, EU 30%, WU 10%).
  With equal odds a third of the cars wait for the wash unit while a wash bay is picked only
  10% of the time. The WB list then keeps growing and a run takes ~1400 timesteps.
  To switch back, change `RandomBayType()` to `RandomServiceUnit()` in
  `Center::MoveToRandomWaitList`.
- At diagnosis each car gets a random service list (1–3 services, durations 1–10) and SS/FS.
  The phase 1 timing still uses the fixed 3 (diagnosis) and 4 (service) timesteps.
- Bay IDs print as `D1`, `M3`, `E2`, `W1`. Free wash bays print their remaining capacity: `W1(2)`.
