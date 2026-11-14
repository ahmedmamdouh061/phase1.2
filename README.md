# Car Service Center — Phase 1.2

C++ Data Structures and Algorithms project (Team T14, Fall 2026).

## Project status

Implementation has started on the `phase1.2-development` branch. The first service-request class and CMake build target are in place. The Phase 1.1 design has been reviewed; project-specific list classes will be built on the course lab data structures, not on replacement containers.

## Planned source layout

- `include/` — project class declarations (currently includes `ServiceRequest.h`)
- `src/` — project implementations (currently includes `ServiceRequest.cpp`)
- `CMakeLists.txt` — C++11 build configuration
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

The initial CMake target is configured, but no successful build or runtime test is claimed yet. The remaining project classes must match the assignment's Appendix A and use the course lab data-structure APIs.

See [docs/REQUIREMENTS_CHECKLIST.md](docs/REQUIREMENTS_CHECKLIST.md) for the remaining inputs and implementation checklist.
