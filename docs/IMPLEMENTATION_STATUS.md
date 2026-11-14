# Implementation status

## Started

- Added `ServiceType` and `ServiceRequest` as the value stored in a car's required-services queue.
- Added a CMake build target for the first project source file.
- Kept the code independent of STL containers, global variables, and friend functions.

## Confirmed project design

The Phase 1.1 report and recommended-DS sheet specify pointer-based car/bay lists and the following structures:

- IDLE: derived from `LinkedQueue<car*>`, with sorted insertion for rescheduled cars.
- ERLY: derived from `PriQueue<car*>`, with random removal.
- LATE: derived from `LinkedQueue<car*>`, with random cancellation.
- REDY and free-bay lists: linked queues.
- DIAG and in-service cars: priority queues ordered by completion time.
- WAIT_MU, WAIT_EU, WAIT_WU: queue-derived lists maintaining total waiting service time.
- Cancelled and finished cars: array stacks.
- Each car points to its assigned bay while it is being diagnosed or serviced.

## Not guessed

The exact Appendix A member declarations and the course lab class APIs are not present in the currently available project files. The custom lists will be derived from the actual lab classes once their interfaces are available; replacing them with homemade containers would contradict the course's recommended-DS sheet.

## Verification

The initial CMake target is configured for C++11. A successful compile has not yet been recorded.
