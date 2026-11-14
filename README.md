# Car Service Center — Phase 1.2

C++ Data Structures and Algorithms project (Team T14, Fall 2026).

## Project status

This repository is being prepared for the Phase 1.2 implementation. The Phase 1.1 design has been reviewed, but the implementation is intentionally not guessed: the official Phase 1.2 specification, Appendix A class definitions, and course lab headers are still needed before the code can be completed and tested against the required interfaces.

## Planned source layout

- `include/` — project class declarations
- `src/` — project implementations and simulator
- `LabStructures/` — the exact course-provided stack, queue, and priority-queue headers
- `docs/` — implementation notes and requirement checklist

## Data-structure design carried forward from Phase 1.1

The design uses pointer-based lists and the course data structures (or derived classes), not STL containers:

- `IDLE_List`: derived from `LinkedQueue<car*>`, with sorted insertion for rescheduled cars
- `ERLY_List`: derived from `PriQueue<car*>`, with random removal for rescheduling
- `LATE_List`: derived from `LinkedQueue<car*>`, with random cancellation
- `REDY_List`: `LinkedQueue<car*>`
- Diagnosis, mechanical, electrical, and wash free-bay queues: queues of bay pointers
- `DIAG_Cars` and `InServ_cars`: priority queues ordered by completion time
- `WAIT_MU`, `WAIT_EU`, and `WAIT_WU`: queue-derived lists that maintain total waiting service time
- `Cancelled_cars` and `Finished_cars`: `ArrayStack<car*>`
- Each car points to its assigned bay while it is in diagnosis or service

## Build status

No build or runtime test is claimed yet. The exact lab headers and assignment-required class members must be added first; then the project can be compiled and tested using the course's expected compiler and interfaces.

See [docs/REQUIREMENTS_CHECKLIST.md](docs/REQUIREMENTS_CHECKLIST.md) for the remaining inputs and implementation checklist.
