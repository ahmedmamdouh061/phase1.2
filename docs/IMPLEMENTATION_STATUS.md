# Implementation status

## Implemented and locally verified

- Added `ServiceType` and `ServiceRequest` as a small value class for one requested service.
- Added a CMake C++11 build target.
- Added a focused test executable covering the default constructor, value constructor, getters, and setters.
- Local verification (GNU C++ 14.2.0, C++11 mode): CMake configure succeeded, compilation succeeded, and CTest passed 1/1 tests.

## Confirmed project design from Phase 1.1

The report and recommended-DS sheet specify pointer-based car/bay lists and course-provided structures:

- IDLE: derived from `LinkedQueue<car*>`, with sorted insertion for rescheduled cars.
- ERLY: derived from `PriQueue<car*>`, with random removal.
- LATE: derived from `LinkedQueue<car*>`, with random cancellation.
- REDY and free-bay lists: linked queues.
- DIAG and in-service cars: priority queues ordered by completion time.
- WAIT_MU, WAIT_EU, WAIT_WU: queue-derived lists maintaining total waiting service time.
- Cancelled and finished cars: array stacks.
- Each car points to its assigned bay while it is being diagnosed or serviced.
- The required-services list is queue-based; it must support removing a service by type for Flexible Sequence cars.

## Correctness boundary

The complete simulator must match the assignment's Appendix A and use the exact lab-provided `LinkedQueue`, `PriQueue`, and `ArrayStack` APIs. The exact Appendix A declarations and lab header files are not present in the repository or the searchable text of the uploaded project documents. The assignment screenshots were shared earlier, but their full declarations are not currently available in the retrievable project text.

I have deliberately not invented car/bay fields, method signatures, priority-queue comparison rules, input parsing, or lab container methods. Doing so could produce a program that compiles but violates the assignment.

## Remaining work

- Bring the exact Appendix A class declarations into the implementation reference.
- Integrate the course lab data-structure headers without replacing them with STL or homemade containers.
- Implement the car, bay, center, all derived lists, simulator, input/output, and every required operation.
- Add deterministic tests for list ordering, random removal/cancellation, bay assignment, diagnosis/service completion, and full simulation behavior.
- Run the full project against the required sample input/output and record the result.
