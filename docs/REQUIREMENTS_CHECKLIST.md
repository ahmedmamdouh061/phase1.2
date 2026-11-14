# Phase 1.2 implementation checklist

## Confirmed requirements from the team's Phase 1.1 report and recommended-DS sheet

- [x] Preserve pointer-based car and bay lists.
- [x] Use the lab-provided data structures, or derive from them; do not use STL containers or replacement containers.
- [x] Preserve the Phase 1.1 list choices and add their required project-specific operations.
- [x] Cars in diagnosis/service must hold a pointer to their assigned bay.
- [x] Required-services list is queue-based and supports removing a service by type for Flexible Sequence cars.
- [x] Initial C++11 build target and a focused unit test for ServiceRequest.
- [ ] Implement and test the full simulator and all required operations.

## Exact interfaces still needed for a correct full implementation

The user previously supplied Phase 1.2 requirement screenshots. Their full Appendix A declarations are not present in the repository or the searchable text currently available to the implementation. The course lab headers for `LinkedQueue`, `PriQueue`, and `ArrayStack` are also not in the repository.

The implementation must match Appendix A exactly and derive from the actual lab structures. Do not guess fields, signatures, or base-class methods.

## Implementation and verification gates

- [ ] Match every Appendix A data member, constructor, destructor, and method signature exactly.
- [ ] Integrate the exact lab APIs and preserve their supplied behavior.
- [ ] Implement all project-specific list operations without replacing the supplied structures.
- [ ] Ensure ownership/deletion of dynamically allocated cars and bays is consistent and leak-free.
- [ ] Implement stream output and required print/count operations exactly as specified.
- [ ] Implement the random simulator with at least 1,000 IDLE cars and every required event/operation from the assignment.
- [ ] Compile with the target compiler and resolve errors/warnings affecting correctness.
- [ ] Run deterministic edge-case tests and the official sample input/output.
- [ ] Record actual build and test results; never mark unrun tests as passed.
